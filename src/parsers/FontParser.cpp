#include "parsers/FontParser.h"

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <set>
#include <stdexcept>

#ifdef FLATCAM_HAVE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

namespace flatcam {

namespace fs = std::filesystem;

namespace {

/// str.replace de Python: todas las apariciones.
std::string replaceAll(std::string s, const std::string& from, const std::string& to) {
    std::size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
    return s;
}

bool contains(const std::string& s, const std::string& sub) { return s.find(sub) != std::string::npos; }

void appendUtf8(std::string& out, char32_t c) {
    if (c < 0x80) {
        out += static_cast<char>(c);
    } else if (c < 0x800) {
        out += static_cast<char>(0xC0 | (c >> 6));
        out += static_cast<char>(0x80 | (c & 0x3F));
    } else if (c < 0x10000) {
        out += static_cast<char>(0xE0 | (c >> 12));
        out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (c & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (c >> 18));
        out += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (c & 0x3F));
    }
}

/// Los caracteres de un texto UTF-8 (lo que recorre `for char in char_string`).
std::vector<char32_t> decodeUtf8(const std::string& s) {
    std::vector<char32_t> out;
    for (std::size_t i = 0; i < s.size();) {
        const auto b = static_cast<unsigned char>(s[i]);
        int n = b < 0x80 ? 1 : (b >> 5) == 6 ? 2 : (b >> 4) == 14 ? 3 : (b >> 3) == 30 ? 4 : 1;
        char32_t c = n == 1 ? b : n == 2 ? (b & 0x1F) : n == 3 ? (b & 0x0F) : (b & 0x07);
        for (int k = 1; k < n && i + static_cast<std::size_t>(k) < s.size(); ++k)
            c = (c << 6) | (static_cast<unsigned char>(s[i + static_cast<std::size_t>(k)]) & 0x3F);
        out.push_back(c);
        i += static_cast<std::size_t>(n);
    }
    return out;
}

/// Los directorios y todas sus subcarpetas que existen (`os.walk`).
void walk(const std::string& dir, std::vector<std::string>& out) {
    std::error_code ec;
    if (!fs::is_directory(dir, ec)) return;
    out.push_back(dir);
    for (auto it = fs::recursive_directory_iterator(dir, fs::directory_options::skip_permission_denied, ec);
         !ec && it != fs::recursive_directory_iterator(); it.increment(ec)) {
        std::error_code e2;
        if (it->is_directory(e2)) out.push_back(it->path().string());
    }
}

std::string expandHome(const std::string& p) {
    if (!p.empty() && p[0] == '~') {
        const char* home = std::getenv("HOME");
        if (home) return std::string(home) + p.substr(1);
    }
    return p;
}

}  // namespace

bool FontParser::available() {
#ifdef FLATCAM_HAVE_FREETYPE
    return true;
#else
    return false;
#endif
}

std::vector<std::string> FontParser::systemFontDirectories() {
    std::vector<std::string> out;
#if defined(_WIN32)
    const char* windir = std::getenv("WINDIR");
    out.push_back((fs::path(windir ? windir : "C:\\Windows") / "Fonts").string());
#elif defined(__APPLE__)
    for (const char* d : {"~/Library/Fonts/", "/Library/Fonts/", "/Network/Library/Fonts/", "/System/Library/Fonts/",
                          "System Folder:Fonts:"})
        walk(expandHome(d), out);
#else
    for (const char* d : {"/usr/X11R6/lib/X11/fonts/TTF/", "/usr/lib/openoffice/share/fonts/truetype/",
                          "/usr/share/fonts", "/usr/local/share/fonts", "~/.fonts"})
        walk(expandHome(d), out);
#endif
    return out;
}

std::vector<std::string> FontParser::fontFiles(const std::vector<std::string>& dirs) {
    std::vector<std::string> out;
    std::set<std::string> seen;
    for (const std::string& d : dirs) {
        std::error_code ec;
        for (auto it = fs::directory_iterator(d, ec); !ec && it != fs::directory_iterator(); it.increment(ec)) {
            std::string ext = it->path().extension().string();
#ifdef _WIN32
            for (char& c : ext) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
#endif
            if (ext != ".ttf") continue;
            const std::string abs = fs::absolute(it->path()).lexically_normal().string();
            if (seen.insert(abs).second) out.push_back(abs);
        }
    }
    return out;
}

std::pair<std::string, std::string> FontParser::fontName(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("No se puede abrir " + path);
    const std::string data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    auto u16 = [&](std::size_t o) -> unsigned {
        if (o + 2 > data.size()) throw std::runtime_error("Fuente truncada");
        return (static_cast<unsigned char>(data[o]) << 8) | static_cast<unsigned char>(data[o + 1]);
    };
    auto u32 = [&](std::size_t o) -> std::uint32_t { return (static_cast<std::uint32_t>(u16(o)) << 16) | u16(o + 2); };
    const std::uint32_t version = u32(0);
    if (version != 0x00010000 && version != 0x4F54544F && version != 0x74727565)  // TrueType, 'OTTO', 'true'
        throw std::runtime_error("No es una fuente TrueType");
    const unsigned numTables = u16(4);
    std::size_t nameOff = 0;
    for (unsigned i = 0; i < numTables; ++i) {
        const std::size_t rec = 12 + 16 * static_cast<std::size_t>(i);
        if (data.compare(rec, 4, "name") == 0) nameOff = u32(rec + 8);
    }
    if (!nameOff) throw std::runtime_error("La fuente no tiene tabla 'name'");
    const unsigned count = u16(nameOff + 2);
    const std::size_t strings = nameOff + u16(nameOff + 4);
    std::string name, family;
    for (unsigned i = 0; i < count; ++i) {
        const std::size_t r = nameOff + 6 + 12 * static_cast<std::size_t>(i);
        const unsigned id = u16(r + 6), len = u16(r + 8), off = u16(r + 10);
        if (strings + off + len > data.size()) throw std::runtime_error("Fuente truncada");
        const std::string raw = data.substr(strings + off, len);
        std::string s;
        if (raw.find('\0') != std::string::npos) {
            // decode('utf-16-be') (con pares sustitutos)
            for (std::size_t k = 0; k + 1 < raw.size(); k += 2) {
                char32_t c = (static_cast<unsigned char>(raw[k]) << 8) | static_cast<unsigned char>(raw[k + 1]);
                if (c >= 0xD800 && c < 0xDC00 && k + 3 < raw.size()) {
                    const char32_t lo = (static_cast<unsigned char>(raw[k + 2]) << 8) |
                                        static_cast<unsigned char>(raw[k + 3]);
                    c = 0x10000 + ((c - 0xD800) << 10) + (lo - 0xDC00);
                    k += 2;
                }
                appendUtf8(s, c);
            }
        } else {
            for (char ch : raw) appendUtf8(s, static_cast<unsigned char>(ch));  // latin-1
        }
        if (id == 4 && name.empty()) name = s;
        else if (id == 1 && family.empty()) family = s;
        if (!name.empty() && !family.empty()) break;
    }
    return {name, family};
}

void FontParser::loadFonts(const std::vector<std::string>& files) {
    for (const std::string& font : files) {
        std::string name;
        try {
            name = fontName(font).first;
        } catch (const std::exception&) {
            continue;  // "Could not get the font name"
        }
        if (contains(name, "Bold") && contains(name, "Italic")) {
            boldItalic[replaceAll(name, " Bold Italic", "")] = font;
        } else if (contains(name, "Bold") && contains(name, "Oblique")) {
            boldItalic[replaceAll(name, " Bold Oblique", "")] = font;
        } else if (contains(name, "Bold")) {
            bold[replaceAll(name, " Bold", "")] = font;
        } else if (contains(name, "SemiBold")) {
            bold[replaceAll(name, " SemiBold", "")] = font;
        } else if (contains(name, "DemiBold")) {
            bold[replaceAll(name, " DemiBold", "")] = font;
        } else if (contains(name, "Demi")) {
            bold[replaceAll(name, " Demi", "")] = font;
        } else if (contains(name, "Italic")) {
            italic[replaceAll(name, " Italic", "")] = font;
        } else if (contains(name, "Oblique")) {
            italic[replaceAll(name, " Italic", "")] = font;  // así en el original: queda " Oblique"
        } else {
            regular[replaceAll(name, " Regular", "")] = font;
        }
    }
}

std::optional<std::vector<Shape>> FontParser::fontToGeometry(const std::string& text, const std::string& fontName,
                                                             const std::string& fontType, int fontSize, bool mm,
                                                             double coordx, double coordy) const {
    const std::map<std::string, std::string>* dict = fontType == "bi"        ? &boldItalic
                                                     : fontType == "bold"    ? &bold
                                                     : fontType == "italic"  ? &italic
                                                     : fontType == "regular" ? &regular
                                                                             : nullptr;
    if (!dict) return std::nullopt;  // el original busca en path_filename = "" y FreeType falla: igual, no hay fuente
    const auto it = dict->find(fontName);
    if (it == dict->end()) return std::nullopt;
#ifndef FLATCAM_HAVE_FREETYPE
    (void)text;
    (void)fontSize;
    (void)mm;
    (void)coordx;
    (void)coordy;
    return std::nullopt;
#else
    FT_Library lib = nullptr;
    if (FT_Init_FreeType(&lib)) throw std::runtime_error("No se pudo iniciar FreeType");
    FT_Face face = nullptr;
    if (FT_New_Face(lib, it->second.c_str(), 0, &face)) {
        FT_Done_FreeType(lib);
        throw std::runtime_error("FreeType no puede abrir " + it->second);
    }
    struct Guard {
        FT_Library l;
        FT_Face f;
        ~Guard() {
            FT_Done_Face(f);
            FT_Done_FreeType(l);
        }
    } guard{lib, face};
    // set_char_size(width=size * 64): el alto toma el ancho; 72 ppp.
    if (FT_Set_Char_Size(face, static_cast<FT_F26Dot6>(fontSize) * 64, 0, 72, 72))
        throw std::runtime_error("FreeType no acepta el tamaño");
    std::vector<Shape> path;
    double penX = coordx;
    FT_UInt previous = 0;
    for (char32_t ch : decodeUtf8(text)) {
        const FT_UInt glyph = FT_Get_Char_Index(face, ch);
        if (previous > 0 && glyph > 0) {
            FT_Vector delta;
            // Como el original: freetype-py `get_kerning` vuelve a buscar el índice de lo que recibe, y FlatCAM ya le
            // pasa índices, así que el kerning es el de las letras cuyo código es cada índice.
            if (!FT_Get_Kerning(face, FT_Get_Char_Index(face, previous), FT_Get_Char_Index(face, glyph),
                                FT_KERNING_DEFAULT, &delta))
                penX += static_cast<double>(delta.x);
        }
        if (FT_Load_Glyph(face, glyph, FT_LOAD_RENDER)) throw std::runtime_error("FreeType no puede cargar la letra");
        const FT_GlyphSlot slot = face->glyph;
        const FT_Outline& outline = slot->outline;
        int start = 0;
        for (int c = 0; c < outline.n_contours; ++c) {
            const int end = outline.contours[c];
            std::vector<Point2D> pts;
            for (int k = start; k <= end; ++k)
                pts.push_back({static_cast<double>(outline.points[k].x), static_cast<double>(outline.points[k].y)});
            if (!pts.empty()) pts.push_back(pts.front());
            path.push_back(Shape::polygon(pts).translate(penX, coordy));
            start = end + 1;
        }
        penX += static_cast<double>(slot->advance.x);
        previous = glyph;
    }
    const double k = mm ? 0.0080187969924812 : 0.00031570066;
    std::vector<Shape> scaled;
    scaled.reserve(path.size());
    for (const Shape& p : path) scaled.push_back(p.scale(k, k, Point2D{coordx, coordy}));
    return scaled;
#endif
}

}  // namespace flatcam
