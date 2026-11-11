// Port de la biblioteca de Python qrcode 7.4.2 (Copyright (c) 2011, Lincoln Loop; licencia BSD, ver NOTICE) y de
// ToolQRCode.execute / make de FlatCAM 2024.4.
#include "tools/QrCode.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

#include "parsers/SvgImport.h"
#include "tools/generated/QrTables.h"

namespace flatcam {

namespace {

// ---------------------------------------------------------------------------------------------- util.py

constexpr int kModeNumber = 1 << 0;
constexpr int kModeAlphaNum = 1 << 1;
constexpr int kModeByte = 1 << 2;

const std::string kAlphaNum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

const std::vector<std::vector<int>> kPatternPosition = {
    {},
    {6, 18},
    {6, 22},
    {6, 26},
    {6, 30},
    {6, 34},
    {6, 22, 38},
    {6, 24, 42},
    {6, 26, 46},
    {6, 28, 50},
    {6, 30, 54},
    {6, 32, 58},
    {6, 34, 62},
    {6, 26, 46, 66},
    {6, 26, 48, 70},
    {6, 26, 50, 74},
    {6, 30, 54, 78},
    {6, 30, 56, 82},
    {6, 30, 58, 86},
    {6, 34, 62, 90},
    {6, 28, 50, 72, 94},
    {6, 26, 50, 74, 98},
    {6, 30, 54, 78, 102},
    {6, 28, 54, 80, 106},
    {6, 32, 58, 84, 110},
    {6, 30, 58, 86, 114},
    {6, 34, 62, 90, 118},
    {6, 26, 50, 74, 98, 122},
    {6, 30, 54, 78, 102, 126},
    {6, 26, 52, 78, 104, 130},
    {6, 30, 56, 82, 108, 134},
    {6, 34, 60, 86, 112, 138},
    {6, 30, 58, 86, 114, 142},
    {6, 34, 62, 90, 118, 146},
    {6, 30, 54, 78, 102, 126, 150},
    {6, 24, 50, 76, 102, 128, 154},
    {6, 28, 54, 80, 106, 132, 158},
    {6, 32, 58, 84, 110, 136, 162},
    {6, 26, 54, 82, 110, 138, 166},
    {6, 30, 58, 86, 114, 142, 170},
};

constexpr int kG15 = (1 << 10) | (1 << 8) | (1 << 5) | (1 << 4) | (1 << 2) | (1 << 1) | (1 << 0);
constexpr int kG18 = (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8) | (1 << 5) | (1 << 2) | (1 << 0);
constexpr int kG15Mask = (1 << 14) | (1 << 12) | (1 << 10) | (1 << 4) | (1 << 1);

int bchDigit(int data) {
    int digit = 0;
    while (data != 0) {
        ++digit;
        data >>= 1;
    }
    return digit;
}

int bchTypeInfo(int data) {
    int d = data << 10;
    while (bchDigit(d) - bchDigit(kG15) >= 0) d ^= kG15 << (bchDigit(d) - bchDigit(kG15));
    return ((data << 10) | d) ^ kG15Mask;
}

int bchTypeNumber(int data) {
    int d = data << 12;
    while (bchDigit(d) - bchDigit(kG18) >= 0) d ^= kG18 << (bchDigit(d) - bchDigit(kG18));
    return (data << 12) | d;
}

bool maskFunc(int pattern, int i, int j) {
    switch (pattern) {
        case 0: return (i + j) % 2 == 0;
        case 1: return i % 2 == 0;
        case 2: return j % 3 == 0;
        case 3: return (i + j) % 3 == 0;
        case 4: return (i / 2 + j / 3) % 2 == 0;
        case 5: return (i * j) % 2 + (i * j) % 3 == 0;
        case 6: return ((i * j) % 2 + (i * j) % 3) % 2 == 0;
        default: return ((i * j) % 3 + (i + j) % 2) % 2 == 0;
    }
}

/// mode_sizes_for_version: bits del largo según el modo.
int modeSize(int mode, int version) {
    const int idx = version < 10 ? 0 : version < 27 ? 1 : 2;
    static const int number[] = {10, 12, 14}, alpha[] = {9, 11, 13}, byte[] = {8, 16, 16};
    return mode == kModeNumber ? number[idx] : mode == kModeAlphaNum ? alpha[idx] : byte[idx];
}
int sizeClass(int version) { return version < 10 ? 0 : version < 27 ? 1 : 2; }

struct BitBuffer {
    std::vector<int> buffer;
    int length{0};
    void putBit(bool bit) {
        const std::size_t idx = static_cast<std::size_t>(length / 8);
        if (buffer.size() <= idx) buffer.push_back(0);
        if (bit) buffer[idx] |= 0x80 >> (length % 8);
        ++length;
    }
    void put(long long num, int len) {
        for (int i = 0; i < len; ++i) putBit(((num >> (len - i - 1)) & 1) == 1);
    }
};

struct QrData {
    int mode;
    std::string data;
    void write(BitBuffer& b) const {
        if (mode == kModeNumber) {
            static const int numberLength[] = {0, 4, 7, 10};
            for (std::size_t i = 0; i < data.size(); i += 3) {
                const std::string chars = data.substr(i, 3);
                b.put(std::stoll(chars), numberLength[chars.size()]);
            }
        } else if (mode == kModeAlphaNum) {
            for (std::size_t i = 0; i < data.size(); i += 2) {
                const std::string chars = data.substr(i, 2);
                if (chars.size() > 1)
                    b.put(static_cast<long long>(kAlphaNum.find(chars[0]) * 45 + kAlphaNum.find(chars[1])), 11);
                else
                    b.put(static_cast<long long>(kAlphaNum.find(chars[0])), 6);
            }
        } else {
            for (unsigned char c : data) b.put(c, 8);
        }
    }
};

bool isDigit(unsigned char c) { return c >= '0' && c <= '9'; }
bool isAlpha(unsigned char c) { return kAlphaNum.find(static_cast<char>(c)) != std::string::npos; }

/// re.search del patrón: con `anchored`, `^X+$` (el `$` también vale antes de un "\n" final); si no, `X{min,}`.
/// @return [inicio, fin) del primer acierto, o vacío.
std::optional<std::pair<std::size_t, std::size_t>> searchRun(const std::string& d, bool (*cls)(unsigned char),
                                                             bool anchored, std::size_t minimum) {
    if (anchored) {
        std::size_t n = 0;
        while (n < d.size() && cls(static_cast<unsigned char>(d[n]))) ++n;
        if (n == 0) return std::nullopt;
        if (n == d.size() || (n == d.size() - 1 && d[n] == '\n')) return std::make_pair(std::size_t{0}, n);
        return std::nullopt;
    }
    for (std::size_t i = 0; i < d.size();) {
        if (!cls(static_cast<unsigned char>(d[i]))) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < d.size() && cls(static_cast<unsigned char>(d[j]))) ++j;
        if (j - i >= minimum) return std::make_pair(i, j);
        i = j;
    }
    return std::nullopt;
}

/// _optimal_split.
std::vector<std::pair<bool, std::string>> optimalSplit(std::string data, bool (*cls)(unsigned char), bool anchored,
                                                       std::size_t minimum) {
    std::vector<std::pair<bool, std::string>> out;
    while (!data.empty()) {
        const auto m = searchRun(data, cls, anchored, minimum);
        if (!m) break;
        if (m->first) out.push_back({false, data.substr(0, m->first)});
        out.push_back({true, data.substr(m->first, m->second - m->first)});
        data = data.substr(m->second);
    }
    if (!data.empty()) out.push_back({false, data});
    return out;
}

/// optimal_data_chunks(data, minimum=20).
std::vector<QrData> optimalDataChunks(const std::string& data, std::size_t minimum = 20) {
    const bool anchored = data.size() <= minimum;
    std::vector<QrData> out;
    for (const auto& [isNum, chunk] : optimalSplit(data, isDigit, anchored, minimum)) {
        if (isNum) {
            out.push_back({kModeNumber, chunk});
            continue;
        }
        for (const auto& [alpha, sub] : optimalSplit(chunk, isAlpha, anchored, minimum))
            out.push_back({alpha ? kModeAlphaNum : kModeByte, sub});
    }
    return out;
}

// ---------------------------------------------------------------------------------------------- base.py

struct GaloisTables {
    std::array<int, 256> exp{}, log{};
    GaloisTables() {
        for (int i = 0; i < 256; ++i) exp[i] = log[i] = i;
        for (int i = 0; i < 8; ++i) exp[i] = 1 << i;
        for (int i = 8; i < 256; ++i) exp[i] = exp[i - 4] ^ exp[i - 5] ^ exp[i - 6] ^ exp[i - 8];
        for (int i = 0; i < 255; ++i) log[exp[i]] = i;
    }
};
const GaloisTables& gf() {
    static const GaloisTables t;
    return t;
}
int glog(int n) {
    if (n < 1) throw QrError("glog(" + std::to_string(n) + ")");
    return gf().log[n];
}
int gexp(int n) { return gf().exp[((n % 255) + 255) % 255]; }

/// Polynomial(num, shift): sin los ceros iniciales (si todos son cero queda el último) y con @p shift ceros al final.
std::vector<int> polynomial(const std::vector<int>& num, int shift) {
    if (num.empty()) throw QrError("polinomio vacío");
    std::size_t offset = 0;
    for (offset = 0; offset < num.size(); ++offset)
        if (num[offset] != 0) break;
    if (offset == num.size()) offset = num.size() - 1;
    std::vector<int> r(num.begin() + static_cast<long>(offset), num.end());
    r.insert(r.end(), static_cast<std::size_t>(shift), 0);
    return r;
}

std::vector<int> polyMul(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> num(a.size() + b.size() - 1, 0);
    for (std::size_t i = 0; i < a.size(); ++i)
        for (std::size_t j = 0; j < b.size(); ++j) num[i + j] ^= gexp(glog(a[i]) + glog(b[j]));
    return polynomial(num, 0);
}

std::vector<int> polyMod(std::vector<int> self, const std::vector<int>& other) {
    while (true) {
        const long difference = static_cast<long>(self.size()) - static_cast<long>(other.size());
        if (difference < 0) return self;
        const int ratio = glog(self[0]) - glog(other[0]);
        std::vector<int> num;
        for (std::size_t i = 0; i < other.size(); ++i) num.push_back(self[i] ^ gexp(glog(other[i]) + ratio));
        if (difference) num.insert(num.end(), self.end() - difference, self.end());
        self = polynomial(num, 0);
    }
}

struct RsBlock {
    int total;
    int data;
};

std::vector<RsBlock> rsBlocks(int version, QrErrorCorrection ec) {
    static const int offsetOf[] = {1, 0, 3, 2};  // M, L, H, Q -> RS_BLOCK_OFFSET (L 0, M 1, Q 2, H 3)
    const std::vector<int>& row =
        qr_tables::kRsBlockTable[static_cast<std::size_t>((version - 1) * 4 + offsetOf[static_cast<int>(ec)])];
    std::vector<RsBlock> blocks;
    for (std::size_t i = 0; i + 2 < row.size(); i += 3)
        for (int k = 0; k < row[i]; ++k) blocks.push_back({row[i + 1], row[i + 2]});
    return blocks;
}

int bitLimit(int version, QrErrorCorrection ec) {
    int n = 0;
    for (const RsBlock& b : rsBlocks(version, ec)) n += b.data;
    return 8 * n;
}

std::vector<int> createBytes(const BitBuffer& buffer, const std::vector<RsBlock>& blocks) {
    std::size_t offset = 0;
    std::size_t maxDc = 0, maxEc = 0;
    std::vector<std::vector<int>> dcdata, ecdata;
    for (const RsBlock& b : blocks) {
        const int dcCount = b.data;
        const int ecCount = b.total - dcCount;
        maxDc = std::max(maxDc, static_cast<std::size_t>(dcCount));
        maxEc = std::max(maxEc, static_cast<std::size_t>(ecCount));
        std::vector<int> dc;
        for (int i = 0; i < dcCount; ++i) dc.push_back(0xFF & buffer.buffer[i + offset]);
        offset += static_cast<std::size_t>(dcCount);
        std::vector<int> rsPoly;
        const auto lut = qr_tables::kRsPolyLut.find(ecCount);
        if (lut != qr_tables::kRsPolyLut.end()) {
            rsPoly = polynomial(lut->second, 0);
        } else {
            rsPoly = polynomial({1}, 0);
            for (int i = 0; i < ecCount; ++i) rsPoly = polyMul(rsPoly, polynomial({1, gexp(i)}, 0));
        }
        const std::vector<int> raw = polynomial(dc, static_cast<int>(rsPoly.size()) - 1);
        const std::vector<int> mod = polyMod(raw, rsPoly);
        std::vector<int> ec;
        const long modOffset = static_cast<long>(mod.size()) - ecCount;
        for (int i = 0; i < ecCount; ++i) {
            const long idx = i + modOffset;
            ec.push_back(idx >= 0 ? mod[static_cast<std::size_t>(idx)] : 0);
        }
        dcdata.push_back(dc);
        ecdata.push_back(ec);
    }
    std::vector<int> data;
    for (std::size_t i = 0; i < maxDc; ++i)
        for (const auto& dc : dcdata)
            if (i < dc.size()) data.push_back(dc[i]);
    for (std::size_t i = 0; i < maxEc; ++i)
        for (const auto& ec : ecdata)
            if (i < ec.size()) data.push_back(ec[i]);
    return data;
}

std::vector<int> createData(int version, QrErrorCorrection ec, const std::vector<QrData>& list) {
    BitBuffer buffer;
    for (const QrData& d : list) {
        buffer.put(d.mode, 4);
        buffer.put(static_cast<long long>(d.data.size()), modeSize(d.mode, version));
        d.write(buffer);
    }
    const std::vector<RsBlock> blocks = rsBlocks(version, ec);
    int limit = 0;
    for (const RsBlock& b : blocks) limit += b.data * 8;
    if (buffer.length > limit)
        throw QrError("Code length overflow. Data size (" + std::to_string(buffer.length) + ") > size available (" +
                      std::to_string(limit) + ")");
    for (int i = 0; i < std::min(limit - buffer.length, 4); ++i) buffer.putBit(false);
    if (const int delimit = buffer.length % 8)
        for (int i = 0; i < 8 - delimit; ++i) buffer.putBit(false);
    const int toFill = (limit - buffer.length) / 8;
    for (int i = 0; i < toFill; ++i) buffer.put(i % 2 == 0 ? 0xEC : 0x11, 8);
    return createBytes(buffer, blocks);
}

// ---------------------------------------------------------------------------------------------- lost_point

using Modules = std::vector<std::vector<int8_t>>;  // -1 = None, 0 = claro, 1 = oscuro

int lostLevel1(const Modules& m, int n) {
    std::vector<int> container(static_cast<std::size_t>(n + 1), 0);
    for (int row = 0; row < n; ++row) {
        int prev = m[row][0], length = 0;
        for (int col = 0; col < n; ++col) {
            if (m[row][col] == prev) {
                ++length;
            } else {
                if (length >= 5) ++container[length];
                length = 1;
                prev = m[row][col];
            }
        }
        if (length >= 5) ++container[length];
    }
    for (int col = 0; col < n; ++col) {
        int prev = m[0][col], length = 0;
        for (int row = 0; row < n; ++row) {
            if (m[row][col] == prev) {
                ++length;
            } else {
                if (length >= 5) ++container[length];
                length = 1;
                prev = m[row][col];
            }
        }
        if (length >= 5) ++container[length];
    }
    int lost = 0;
    for (int len = 5; len <= n; ++len) lost += container[len] * (len - 2);
    return lost;
}

int lostLevel2(const Modules& m, int n) {
    int lost = 0;
    for (int row = 0; row < n - 1; ++row) {
        const auto& r = m[row];
        const auto& next = m[row + 1];
        for (int col = 0; col < n - 1; ++col) {
            const int topRight = r[col + 1];
            if (topRight != next[col + 1]) {
                ++col;  // next(modules_range_iter, None): se saltea la columna siguiente
            } else if (topRight != r[col]) {
                continue;
            } else if (topRight != next[col]) {
                continue;
            } else {
                lost += 3;
            }
        }
    }
    return lost;
}

int lostLevel3(const Modules& m, int n) {
    int lost = 0;
    auto at = [&](int row, int col) { return m[row][col] == 1; };
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n - 10; ++col) {
            auto v = [&](int k) { return at(row, col + k); };
            if (!v(1) && v(4) && !v(5) && v(6) && !v(9) &&
                ((v(0) && v(2) && v(3) && !v(7) && !v(8) && !v(10)) ||
                 (!v(0) && !v(2) && !v(3) && v(7) && v(8) && v(10))))
                lost += 40;
            if (v(10)) ++col;
        }
    }
    for (int col = 0; col < n; ++col) {
        for (int row = 0; row < n - 10; ++row) {
            auto v = [&](int k) { return at(row + k, col); };
            if (!v(1) && v(4) && !v(5) && v(6) && !v(9) &&
                ((v(0) && v(2) && v(3) && !v(7) && !v(8) && !v(10)) ||
                 (!v(0) && !v(2) && !v(3) && v(7) && v(8) && v(10))))
                lost += 40;
            if (v(10)) ++row;
        }
    }
    return lost;
}

int lostLevel4(const Modules& m, int n) {
    long dark = 0;
    for (const auto& r : m)
        for (int8_t v : r) dark += v == 1;
    const double percent = static_cast<double>(dark) / (static_cast<double>(n) * n);
    const int rating = static_cast<int>(std::abs(percent * 100 - 50) / 5);
    return rating * 10;
}

int lostPoint(const Modules& m) {
    const int n = static_cast<int>(m.size());
    return lostLevel1(m, n) + lostLevel2(m, n) + lostLevel3(m, n) + lostLevel4(m, n);
}

// ---------------------------------------------------------------------------------------------- main.py

class Builder {
public:
    Builder(int version, QrErrorCorrection ec, std::vector<QrData> list) : version_(version), ec_(ec), list_(std::move(list)) {}

    /// best_fit(start).
    void bestFit(int start) {
        if (start < 1 || start > 40) throw QrError("Invalid version (was " + std::to_string(start) + ", expected 1 to 40)");
        BitBuffer buffer;
        for (const QrData& d : list_) {
            buffer.put(d.mode, 4);
            buffer.put(static_cast<long long>(d.data.size()), modeSize(d.mode, start));
            d.write(buffer);
        }
        // bisect_left(BIT_LIMIT_TABLE[ec], needed_bits, start)
        int v = start;
        while (v <= 40 && bitLimit(v, ec_) < buffer.length) ++v;
        version_ = v;
        if (version_ == 41) throw QrError("DataOverflowError");
        if (sizeClass(start) != sizeClass(version_)) bestFit(version_);
    }

    void make() {
        bestFit(version_);
        int best = 0, minLost = 0;
        for (int i = 0; i < 8; ++i) {
            makeImpl(true, i);
            const int lost = lostPoint(modules_);
            if (i == 0 || minLost > lost) {
                minLost = lost;
                best = i;
            }
        }
        makeImpl(false, best);
    }

    QrMatrix result() const {
        QrMatrix r;
        r.version = version_;
        for (const auto& row : modules_) {
            std::vector<bool> out;
            for (int8_t v : row) out.push_back(v == 1);
            r.modules.push_back(out);
        }
        return r;
    }

private:
    void makeImpl(bool test, int mask) {
        count_ = version_ * 4 + 17;
        modules_.assign(static_cast<std::size_t>(count_), std::vector<int8_t>(static_cast<std::size_t>(count_), -1));
        probe(0, 0);
        probe(count_ - 7, 0);
        probe(0, count_ - 7);
        adjust();
        timing();
        typeInfo(test, mask);
        if (version_ >= 7) typeNumber(test);
        if (!dataCache_) dataCache_ = createData(version_, ec_, list_);
        mapData(*dataCache_, mask);
    }

    void probe(int row, int col) {
        for (int r = -1; r < 8; ++r) {
            if (row + r <= -1 || count_ <= row + r) continue;
            for (int c = -1; c < 8; ++c) {
                if (col + c <= -1 || count_ <= col + c) continue;
                const bool on = ((0 <= r && r <= 6 && (c == 0 || c == 6)) || (0 <= c && c <= 6 && (r == 0 || r == 6)) ||
                                 (2 <= r && r <= 4 && 2 <= c && c <= 4));
                modules_[row + r][col + c] = on ? 1 : 0;
            }
        }
    }

    void timing() {
        for (int r = 8; r < count_ - 8; ++r)
            if (modules_[r][6] == -1) modules_[r][6] = r % 2 == 0;
        for (int c = 8; c < count_ - 8; ++c)
            if (modules_[6][c] == -1) modules_[6][c] = c % 2 == 0;
    }

    void adjust() {
        const std::vector<int>& pos = kPatternPosition[static_cast<std::size_t>(version_ - 1)];
        for (int row : pos)
            for (int col : pos) {
                if (modules_[row][col] != -1) continue;
                for (int r = -2; r < 3; ++r)
                    for (int c = -2; c < 3; ++c)
                        modules_[row + r][col + c] = (r == -2 || r == 2 || c == -2 || c == 2 || (r == 0 && c == 0)) ? 1 : 0;
            }
    }

    void typeNumber(bool test) {
        const int bits = bchTypeNumber(version_);
        for (int i = 0; i < 18; ++i)
            modules_[i / 3][i % 3 + count_ - 8 - 3] = !test && ((bits >> i) & 1) == 1;
        for (int i = 0; i < 18; ++i)
            modules_[i % 3 + count_ - 8 - 3][i / 3] = !test && ((bits >> i) & 1) == 1;
    }

    void typeInfo(bool test, int mask) {
        const int data = (static_cast<int>(ec_) << 3) | mask;
        const int bits = bchTypeInfo(data);
        for (int i = 0; i < 15; ++i) {
            const int8_t mod = !test && ((bits >> i) & 1) == 1;
            if (i < 6) modules_[i][8] = mod;
            else if (i < 8) modules_[i + 1][8] = mod;
            else modules_[count_ - 15 + i][8] = mod;
        }
        for (int i = 0; i < 15; ++i) {
            const int8_t mod = !test && ((bits >> i) & 1) == 1;
            if (i < 8) modules_[8][count_ - i - 1] = mod;
            else if (i < 9) modules_[8][15 - i - 1 + 1] = mod;
            else modules_[8][15 - i - 1] = mod;
        }
        modules_[count_ - 8][8] = !test;
    }

    void mapData(const std::vector<int>& data, int mask) {
        int inc = -1, row = count_ - 1, bitIndex = 7;
        std::size_t byteIndex = 0;
        for (int col = count_ - 1; col > 0; col -= 2) {
            int c0 = col;
            if (c0 <= 6) --c0;
            const int cols[2] = {c0, c0 - 1};
            while (true) {
                for (int c : cols) {
                    if (modules_[row][c] == -1) {
                        bool dark = false;
                        if (byteIndex < data.size()) dark = ((data[byteIndex] >> bitIndex) & 1) == 1;
                        if (maskFunc(mask, row, c)) dark = !dark;
                        modules_[row][c] = dark;
                        if (--bitIndex == -1) {
                            ++byteIndex;
                            bitIndex = 7;
                        }
                    }
                }
                row += inc;
                if (row < 0 || count_ <= row) {
                    row -= inc;
                    inc = -inc;
                    break;
                }
            }
        }
    }

    int version_;
    QrErrorCorrection ec_;
    std::vector<QrData> list_;
    int count_{0};
    Modules modules_;
    std::optional<std::vector<int>> dataCache_;
};

/// SvgFragmentImage.units: píxeles / 10 como el Decimal de Python (sin ceros de más).
std::string units(long px) {
    if (px % 10 == 0) return std::to_string(px / 10);
    return std::to_string(px / 10) + "." + std::to_string(px % 10);
}

/// Escapa un atributo como ElementTree.
std::string escapeAttr(const std::string& s) {
    std::string o;
    for (char c : s) {
        switch (c) {
            case '&': o += "&amp;"; break;
            case '<': o += "&lt;"; break;
            case '>': o += "&gt;"; break;
            case '"': o += "&quot;"; break;
            case '\n': o += "&#10;"; break;
            case '\r': o += "&#13;"; break;
            case '\t': o += "&#09;"; break;
            default: o += c;
        }
    }
    return o;
}

}  // namespace

QrErrorCorrection qrErrorCorrection(char letter) {
    switch (letter) {
        case 'L': return QrErrorCorrection::L;
        case 'Q': return QrErrorCorrection::Q;
        case 'H': return QrErrorCorrection::H;
        default: return QrErrorCorrection::M;
    }
}

QrMatrix qrMake(const std::string& data, int version, QrErrorCorrection ec) {
    if (version < 1 || version > 40)
        throw QrError("Invalid version (was " + std::to_string(version) + ", expected 1 to 40)");
    Builder b(version, ec, optimalDataChunks(data));
    b.make();
    return b.result();
}

std::string qrSvgFragment(const QrMatrix& qr, int boxSize, int border) {
    const long width = static_cast<long>(qr.modules.size());
    const std::string dim = units((width + border * 2) * boxSize) + "mm";
    const std::string unit = units(boxSize) + "mm";
    std::string s = "<svg:svg xmlns:svg=\"http://www.w3.org/2000/svg\" width=\"" + dim + "\" height=\"" + dim +
                    "\" version=\"1.1\">";
    for (long r = 0; r < width; ++r)
        for (long c = 0; c < width; ++c)
            if (qr.modules[r][c])
                s += "<svg:rect x=\"" + units((c + border) * boxSize) + "mm\" y=\"" + units((r + border) * boxSize) +
                     "mm\" width=\"" + unit + "\" height=\"" + unit + "\"/>";
    return s + "</svg:svg>";
}

std::string qrSvgPath(const QrMatrix& qr, int boxSize, int border, const std::string& fill, const std::string& back) {
    const long width = static_cast<long>(qr.modules.size());
    const long px = (width + border * 2) * boxSize;
    const std::string dim = units(px);
    std::string d;
    for (long r = 0; r < width; ++r)
        for (long c = 0; c < width; ++c)
            if (qr.modules[r][c]) {
                const std::string x0 = units((c + border) * boxSize), y0 = units((r + border) * boxSize);
                const std::string x1 = units((c + border) * boxSize + boxSize), y1 = units((r + border) * boxSize + boxSize);
                d += "M" + x0 + "," + y0 + "H" + x1 + "V" + y1 + "H" + x0 + "z";
            }
    return "<?xml version='1.0' encoding='UTF-8'?>\n<svg width=\"" + dim + "mm\" height=\"" + dim +
           "mm\" version=\"1.1\" viewBox=\"0 0 " + dim + " " + dim + "\" fill_color=\"" + escapeAttr(fill) +
           "\" back_color=\"" + escapeAttr(back) + "\" xmlns=\"http://www.w3.org/2000/svg\"><path d=\"" + d +
           "\" id=\"qr-path\" fill=\"#000000\" fill-opacity=\"1\" fill-rule=\"nonzero\" stroke=\"none\"/></svg>";
}

QrGeometry qrGeometry(const QrMatrix& qr, int boxSize, int border, int circleSteps) {
    QrGeometry g;
    const SvgImportResult r = importSvgShapes(qrSvgFragment(qr, boxSize, border), circleSteps);
    if (!r.ok) throw QrError(r.error);
    g.modules = r.geometry;
    g.utility = Shape::unaryUnion(g.modules).buffer(0.0000001).buffer(-0.0000001);
    return g;
}

QrGerber qrInsert(const Gerber& g, const QrGeometry& qr, const Point2D& pos, int boxSize, int border, bool rounded,
                  bool negative) {
    QrGerber out;
    const BoundingBox b = qr.utility.bounds().value_or(BoundingBox{});
    const double buffVal = border * (boxSize / 10.0);
    const Shape mask = Shape::box(b.xmin, b.ymin, b.xmax, b.ymax)
                           .buffer(buffVal, 16, rounded ? JoinStyle::Round : JoinStyle::Mitre);
    const Shape offsetMask = mask.translate(pos.x, pos.y);
    // El cobre que contiene entera la máscara se corta; el resto queda (sin repetir polígonos iguales: `not in`).
    ShapeList solid;
    for (const Shape& poly : g.solidGeometry()) {
        if (poly.contains(offsetMask)) {
            solid.push_back(poly.difference(offsetMask));
        } else {
            const bool repeated = std::any_of(solid.begin(), solid.end(), [&](const Shape& s) {
                return s.type() == poly.type() && s.equalsExact(poly, 0.0);
            });
            if (!repeated) solid.push_back(poly);
        }
    }
    const Shape working = negative ? mask.difference(qr.utility) : qr.utility;
    const ShapeList parts = working.parts();
    for (const Shape& p : parts.empty() ? ShapeList{working} : parts) solid.push_back(p.translate(pos.x, pos.y));
    out.solid = solid;

    out.apertures = g.apertures();
    const double box = boxSize / 10.0;
    std::string newId = "10";
    if (!out.apertures.empty()) {
        int mx = 0;
        bool first = true;
        for (const auto& [k, v] : out.apertures) {
            const int n = std::stoi(k);
            if (first || n > mx) mx = n;
            first = false;
        }
        newId = mx >= 10 ? std::to_string(mx + 1) : "10";
    }
    if (!out.apertures.count(newId)) {
        Aperture ap;
        ap.type = "R";
        ap.hasGeometry = true;
        ap.height = box * 1.01;
        ap.width = box * 1.01;
        ap.size = std::sqrt(box * box + box * box);
        out.apertures[newId] = ap;
    }
    if (!out.apertures.count("0")) {
        Aperture ap;
        ap.type = "REG";
        ap.size = 0.0;
        ap.hasGeometry = true;
        out.apertures["0"] = ap;
    }
    ApertureGeometry clear;
    clear.clear = offsetMask;
    out.apertures["0"].hasGeometry = true;
    out.apertures["0"].geometry.push_back(clear);
    Aperture& ap = out.apertures[newId];
    for (const Shape& m : qr.modules) {
        ApertureGeometry el;
        el.solid = m.translate(pos.x, pos.y);
        el.follow = Shape::point(m.centroid()).translate(pos.x, pos.y);
        ap.geometry.push_back(el);
    }
    out.newAperture = newId;
    return out;
}

}  // namespace flatcam
