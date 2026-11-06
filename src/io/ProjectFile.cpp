#include "io/ProjectFile.h"

#include <lzma.h>

#include <algorithm>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <memory>
#include <stdexcept>

namespace flatcam {

namespace {

// ---------------------------------------------------------------- fuentes de bytes

/// Algo de donde leer bytes en bloques.
class ByteSource {
public:
    virtual ~ByteSource() = default;
    /// Lee hasta @p n bytes; devuelve 0 al final. Lanza std::runtime_error si falla.
    virtual std::size_t read(char* buf, std::size_t n) = 0;
};

class FileSource : public ByteSource {
public:
    explicit FileSource(std::FILE* f) : f_(f) {}
    std::size_t read(char* buf, std::size_t n) override { return std::fread(buf, 1, n, f_); }

private:
    std::FILE* f_;
};

class XzSource : public ByteSource {
public:
    explicit XzSource(std::FILE* f) : f_(f), in_(1 << 16) {
        if (lzma_stream_decoder(&strm_, UINT64_MAX, LZMA_CONCATENATED) != LZMA_OK)
            throw std::runtime_error("no se pudo iniciar el descompresor XZ");
    }
    ~XzSource() override { lzma_end(&strm_); }
    std::size_t read(char* buf, std::size_t n) override {
        if (done_) return 0;
        strm_.next_out = reinterpret_cast<uint8_t*>(buf);
        strm_.avail_out = n;
        while (strm_.avail_out == n) {
            if (strm_.avail_in == 0 && !eof_) {
                strm_.next_in = in_.data();
                strm_.avail_in = std::fread(in_.data(), 1, in_.size(), f_);
                eof_ = strm_.avail_in == 0;
            }
            const lzma_ret r = lzma_code(&strm_, eof_ ? LZMA_FINISH : LZMA_RUN);
            if (r == LZMA_STREAM_END) {
                done_ = true;
                break;
            }
            if (r != LZMA_OK) throw std::runtime_error("archivo XZ dañado");
        }
        return n - strm_.avail_out;
    }

private:
    std::FILE* f_;
    lzma_stream strm_ = LZMA_STREAM_INIT;
    std::vector<uint8_t> in_;
    bool eof_{false};
    bool done_{false};
};

class StringSource : public ByteSource {
public:
    explicit StringSource(const std::string& s) : s_(s) {}
    std::size_t read(char* buf, std::size_t n) override {
        const std::size_t k = std::min(n, s_.size() - pos_);
        std::memcpy(buf, s_.data() + pos_, k);
        pos_ += k;
        return k;
    }

private:
    const std::string& s_;
    std::size_t pos_{0};
};

// ---------------------------------------------------------------- lector en streaming

class Parser {
public:
    Parser(ByteSource& src, const JsonSkipFn& skip) : src_(src), skip_(skip), buf_(1 << 20) {}

    JsonValue parseDocument() {
        JsonValue v = parseValue(nullptr);
        skipWs();
        if (peek() != EOF) fail("texto de más al final");
        return v;
    }

private:
    int peek() {
        if (pos_ == len_) {
            len_ = src_.read(buf_.data(), buf_.size());
            pos_ = 0;
            if (len_ == 0) return EOF;
        }
        return static_cast<unsigned char>(buf_[pos_]);
    }
    int get() {
        const int c = peek();
        if (c != EOF) {
            ++pos_;
            ++offset_;
        }
        return c;
    }
    [[noreturn]] void fail(const std::string& what) const {
        throw std::runtime_error("JSON inválido cerca del byte " + std::to_string(offset_) + ": " + what);
    }
    void skipWs() {
        for (int c = peek(); c == ' ' || c == '\n' || c == '\r' || c == '\t'; c = peek()) get();
    }
    void expect(char c) {
        if (get() != c) fail(std::string("se esperaba '") + c + "'");
    }
    void expectWord(const char* w) {
        for (const char* p = w; *p; ++p)
            if (get() != *p) fail(std::string("se esperaba ") + w);
    }

    static void appendUtf8(std::string& out, unsigned cp) {
        if (cp < 0x80) {
            out += static_cast<char>(cp);
        } else if (cp < 0x800) {
            out += static_cast<char>(0xC0 | (cp >> 6));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            out += static_cast<char>(0xE0 | (cp >> 12));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        } else {
            out += static_cast<char>(0xF0 | (cp >> 18));
            out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        }
    }
    unsigned hex4() {
        unsigned v = 0;
        for (int i = 0; i < 4; ++i) {
            const int c = get();
            v <<= 4;
            if (c >= '0' && c <= '9') v |= static_cast<unsigned>(c - '0');
            else if (c >= 'a' && c <= 'f') v |= static_cast<unsigned>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') v |= static_cast<unsigned>(c - 'A' + 10);
            else fail("escape \\u inválido");
        }
        return v;
    }

    /// Lee un texto (después de la comilla de apertura); si @p out es nulo solo lo saltea.
    void parseString(std::string* out) {
        for (;;) {
            // Copia rápida de lo que no es comilla ni barra.
            if (pos_ == len_ && peek() == EOF) fail("texto sin cerrar");
            const char* start = buf_.data() + pos_;
            const char* end = buf_.data() + len_;
            const char* p = start;
            while (p < end && *p != '"' && *p != '\\') ++p;
            if (out) out->append(start, p);
            offset_ += static_cast<std::size_t>(p - start);
            pos_ += static_cast<std::size_t>(p - start);
            if (p == end) continue;
            const int c = get();
            if (c == '"') return;
            // Barra invertida.
            const int e = get();
            unsigned cp = 0;
            switch (e) {
                case '"': cp = '"'; break;
                case '\\': cp = '\\'; break;
                case '/': cp = '/'; break;
                case 'b': cp = '\b'; break;
                case 'f': cp = '\f'; break;
                case 'n': cp = '\n'; break;
                case 'r': cp = '\r'; break;
                case 't': cp = '\t'; break;
                case 'u': {
                    cp = hex4();
                    if (cp >= 0xD800 && cp < 0xDC00) {  // par sustituto
                        if (get() != '\\' || get() != 'u') fail("par sustituto incompleto");
                        const unsigned lo = hex4();
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                    }
                    break;
                }
                default: fail("escape inválido");
            }
            if (out) appendUtf8(*out, cp);
        }
    }

    std::string parseNumberText() {
        std::string t;
        for (int c = peek(); c != EOF && (std::isdigit(c) || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E');
             c = peek())
            t += static_cast<char>(get());
        if (t == "-" && peek() == 'I') {
            expectWord("Infinity");
            return "-Infinity";
        }
        if (t.empty()) fail("valor inesperado");
        return t;
    }

    /// Lee un valor; si @p store es falso lo saltea sin guardarlo.
    JsonValue parseValue(const JsonValue* /*parent*/, bool store = true) {
        skipWs();
        const int c = peek();
        if (c == '{') {
            get();
            JsonValue obj = JsonValue::object();
            skipWs();
            if (peek() == '}') {
                get();
                return obj;
            }
            for (;;) {
                skipWs();
                expect('"');
                std::string key;
                parseString(&key);
                skipWs();
                expect(':');
                path_.push_back(key);
                const bool keep = store && !(skip_ && skip_(path_, obj));
                JsonValue v = parseValue(&obj, keep);
                path_.pop_back();
                if (store) obj.members().emplace_back(std::move(key), std::move(v));
                skipWs();
                const int d = get();
                if (d == '}') break;
                if (d != ',') fail("se esperaba ',' o '}'");
            }
            return store ? obj : JsonValue();
        }
        if (c == '[') {
            get();
            JsonValue arr = JsonValue::array();
            skipWs();
            if (peek() == ']') {
                get();
                return arr;
            }
            path_.push_back("[]");
            for (;;) {
                JsonValue v = parseValue(&arr, store);
                if (store) arr.push(std::move(v));
                skipWs();
                const int d = get();
                if (d == ']') break;
                if (d != ',') fail("se esperaba ',' o ']'");
            }
            path_.pop_back();
            return store ? arr : JsonValue();
        }
        if (c == '"') {
            get();
            if (!store) {
                parseString(nullptr);
                return {};
            }
            std::string s;
            parseString(&s);
            return JsonValue::string(std::move(s));
        }
        if (c == 't') {
            expectWord("true");
            return JsonValue::boolean(true);
        }
        if (c == 'f') {
            expectWord("false");
            return JsonValue::boolean(false);
        }
        if (c == 'n') {
            expectWord("null");
            return {};
        }
        if (c == 'N') {
            expectWord("NaN");
            return JsonValue::number("NaN");
        }
        if (c == 'I') {
            expectWord("Infinity");
            return JsonValue::number("Infinity");
        }
        std::string t = parseNumberText();
        return store ? JsonValue::number(std::move(t)) : JsonValue();
    }

    ByteSource& src_;
    const JsonSkipFn& skip_;
    std::vector<char> buf_;
    std::size_t pos_{0}, len_{0}, offset_{0};
    std::vector<std::string> path_;
};

// ---------------------------------------------------------------- escritura

void dumpString(std::string& out, const std::string& s) {
    // ensure_ascii: lo que no es ASCII va como \uXXXX (con pares sustitutos), como json.dumps.
    out += '"';
    static const char* hex = "0123456789abcdef";
    auto u = [&out](unsigned v) {
        out += "\\u";
        for (int sh = 12; sh >= 0; sh -= 4) out += hex[(v >> sh) & 0xF];
    };
    for (std::size_t i = 0; i < s.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        switch (c) {
            case '"': out += "\\\""; continue;
            case '\\': out += "\\\\"; continue;
            case '\n': out += "\\n"; continue;
            case '\r': out += "\\r"; continue;
            case '\t': out += "\\t"; continue;
            case '\b': out += "\\b"; continue;
            case '\f': out += "\\f"; continue;
            default: break;
        }
        if (c < 0x20) {
            u(c);
        } else if (c < 0x80) {
            out += static_cast<char>(c);
        } else {
            // Decodifica UTF-8.
            unsigned cp = 0;
            int extra = 0;
            if ((c & 0xE0) == 0xC0) cp = c & 0x1F, extra = 1;
            else if ((c & 0xF0) == 0xE0) cp = c & 0x0F, extra = 2;
            else cp = c & 0x07, extra = 3;
            for (int k = 0; k < extra && i + 1 < s.size(); ++k) cp = (cp << 6) | (static_cast<unsigned char>(s[++i]) & 0x3F);
            if (cp >= 0x10000) {
                cp -= 0x10000;
                u(0xD800 + (cp >> 10));
                u(0xDC00 + (cp & 0x3FF));
            } else {
                u(cp);
            }
        }
    }
    out += '"';
}

void dumpValue(std::string& out, const JsonValue& v, int indent, bool sortKeys) {
    const std::string pad(static_cast<std::size_t>(indent + 2), ' ');
    switch (v.type()) {
        case JsonValue::Type::Null: out += "null"; return;
        case JsonValue::Type::Bool: out += v.toBool() ? "true" : "false"; return;
        case JsonValue::Type::Number: out += v.text(); return;
        case JsonValue::Type::String: dumpString(out, v.text()); return;
        case JsonValue::Type::Array: {
            if (v.items().empty()) {
                out += "[]";
                return;
            }
            out += "[\n";
            for (std::size_t i = 0; i < v.items().size(); ++i) {
                out += pad;
                dumpValue(out, v.items()[i], indent + 2, sortKeys);
                out += i + 1 < v.items().size() ? ",\n" : "\n";
            }
            out += std::string(static_cast<std::size_t>(indent), ' ') + "]";
            return;
        }
        case JsonValue::Type::Object: {
            if (v.members().empty()) {
                out += "{}";
                return;
            }
            std::vector<const std::pair<std::string, JsonValue>*> sorted;
            for (const auto& m : v.members()) sorted.push_back(&m);
            if (sortKeys)
                std::stable_sort(sorted.begin(), sorted.end(), [](auto* a, auto* b) { return a->first < b->first; });
            out += "{\n";
            for (std::size_t i = 0; i < sorted.size(); ++i) {
                out += pad;
                dumpString(out, sorted[i]->first);
                out += ": ";
                dumpValue(out, sorted[i]->second, indent + 2, sortKeys);
                out += i + 1 < sorted.size() ? ",\n" : "\n";
            }
            out += std::string(static_cast<std::size_t>(indent), ' ') + "}";
            return;
        }
    }
}

}  // namespace

bool ProjectFile::parse(const std::string& text, JsonValue& root, std::string& error, const JsonSkipFn& skip) {
    try {
        StringSource src(text);
        Parser p(src, skip);
        root = p.parseDocument();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

bool ProjectFile::read(const std::string& path, JsonValue& root, std::string& error, const JsonSkipFn& skip) {
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) {
        error = "no se pudo abrir " + path;
        return false;
    }
    std::unique_ptr<std::FILE, int (*)(std::FILE*)> guard(f, &std::fclose);
    unsigned char magic[6] = {};
    const std::size_t n = std::fread(magic, 1, 6, f);
    std::rewind(f);
    static const unsigned char kXz[6] = {0xFD, '7', 'z', 'X', 'Z', 0x00};
    try {
        std::unique_ptr<ByteSource> src;
        if (n == 6 && std::memcmp(magic, kXz, 6) == 0) src = std::make_unique<XzSource>(f);
        else src = std::make_unique<FileSource>(f);
        Parser p(*src, skip);
        root = p.parseDocument();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

std::string ProjectFile::dump(const JsonValue& value, bool sortKeys) {
    std::string out;
    dumpValue(out, value, 0, sortKeys);
    return out;
}

bool ProjectFile::write(const std::string& path, const JsonValue& root, bool compressed, int level,
                        std::string& error) {
    const std::string text = dump(root);
    std::FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) {
        error = "no se pudo escribir " + path;
        return false;
    }
    std::unique_ptr<std::FILE, int (*)(std::FILE*)> guard(f, &std::fclose);
    if (!compressed) {
        if (std::fwrite(text.data(), 1, text.size(), f) != text.size()) {
            error = "no se pudo escribir " + path;
            return false;
        }
        return true;
    }
    // lzma.open(filename, "w", preset=level): formato XZ con verificación CRC64.
    lzma_stream strm = LZMA_STREAM_INIT;
    if (lzma_easy_encoder(&strm, static_cast<uint32_t>(std::clamp(level, 0, 9)), LZMA_CHECK_CRC64) != LZMA_OK) {
        error = "no se pudo iniciar el compresor XZ";
        return false;
    }
    std::vector<uint8_t> out(1 << 16);
    strm.next_in = reinterpret_cast<const uint8_t*>(text.data());
    strm.avail_in = text.size();
    lzma_ret r = LZMA_OK;
    do {
        strm.next_out = out.data();
        strm.avail_out = out.size();
        r = lzma_code(&strm, LZMA_FINISH);
        const std::size_t have = out.size() - strm.avail_out;
        if ((r != LZMA_OK && r != LZMA_STREAM_END) || std::fwrite(out.data(), 1, have, f) != have) {
            lzma_end(&strm);
            error = "no se pudo comprimir " + path;
            return false;
        }
    } while (r != LZMA_STREAM_END);
    lzma_end(&strm);
    return true;
}

}  // namespace flatcam
