#include "gui/widgets/NumericalEval.h"

#include <QRegularExpressionValidator>
#include <cmath>

namespace flatcam::gui {

namespace {

/// Analizador recursivo: expr = term (('+'|'-') term)*; term = factor (('*'|'/'|'%') factor)*; factor = ('+'|'-') factor
/// | número | '(' expr ')'.
class Parser {
public:
    explicit Parser(const QString& s) : s_(s) {}

    std::optional<double> parse() {
        auto v = expr();
        skip();
        if (!v || pos_ != s_.size()) return std::nullopt;
        return v;
    }

private:
    void skip() {
        while (pos_ < s_.size() && s_[pos_].isSpace()) ++pos_;
    }
    bool eat(QChar c) {
        skip();
        if (pos_ < s_.size() && s_[pos_] == c) {
            ++pos_;
            return true;
        }
        return false;
    }
    std::optional<double> expr() {
        auto v = term();
        while (v) {
            if (eat('+')) {
                auto r = term();
                if (!r) return std::nullopt;
                *v += *r;
            } else if (eat('-')) {
                auto r = term();
                if (!r) return std::nullopt;
                *v -= *r;
            } else {
                break;
            }
        }
        return v;
    }
    std::optional<double> term() {
        auto v = factor();
        while (v) {
            if (eat('*')) {
                auto r = factor();
                if (!r) return std::nullopt;
                *v *= *r;
            } else if (eat('/')) {
                auto r = factor();
                if (!r || *r == 0) return std::nullopt;  // ZeroDivisionError en Python
                *v /= *r;
            } else if (eat('%')) {
                auto r = factor();
                if (!r || *r == 0) return std::nullopt;
                const double m = std::fmod(*v, *r);
                *v = (m != 0 && ((m < 0) != (*r < 0))) ? m + *r : m;  // resto con el signo del divisor, como Python
            } else {
                break;
            }
        }
        return v;
    }
    std::optional<double> factor() {
        if (eat('+')) return factor();
        if (eat('-')) {
            auto v = factor();
            if (v) *v = -*v;
            return v;
        }
        if (eat('(')) {
            auto v = expr();
            if (!v || !eat(')')) return std::nullopt;
            return v;
        }
        skip();
        const int start = pos_;
        while (pos_ < s_.size() && (s_[pos_].isDigit() || s_[pos_] == '.')) ++pos_;
        if (pos_ == start) return std::nullopt;
        bool ok = false;
        const double d = s_.mid(start, pos_ - start).toDouble(&ok);
        return ok ? std::optional<double>(d) : std::nullopt;
    }

    QString s_;
    int pos_{0};
};

}  // namespace

std::optional<double> NumericExpression::evaluate(const QString& text) {
    const QString t = text.trimmed();
    if (t.isEmpty()) return std::nullopt;
    return Parser(t).parse();
}

std::optional<std::vector<double>> NumericExpression::evaluateTuple(const QString& text) {
    QString t = text.trimmed();
    if (t.isEmpty()) return std::nullopt;
    if ((t.startsWith('(') && t.endsWith(')')) || (t.startsWith('[') && t.endsWith(']'))) t = t.mid(1, t.size() - 2);
    std::vector<double> out;
    for (const QString& part : t.split(',')) {
        if (part.trimmed().isEmpty()) continue;  // "(1, 2,)" también es una tupla válida en Python
        const auto v = evaluate(part);
        if (!v) return std::nullopt;
        out.push_back(*v);
    }
    if (out.empty()) return std::nullopt;
    return out;
}

NumericalEvalEntry::NumericalEvalEntry(QWidget* parent) : FCEntry(parent) {
    setValidator(new QRegularExpressionValidator(QRegularExpression(R"([0-9\/\*\+\-\%\.\,\s]*)"), this));
}

std::optional<double> NumericalEvalEntry::evaluated() const {
    QString raw = text().trimmed();
    raw.replace(',', '.');
    return NumericExpression::evaluate(raw);
}

NumericalEvalTupleEntry::NumericalEvalTupleEntry(QWidget* parent) : FCEntry(parent) {
    setValidator(new QRegularExpressionValidator(QRegularExpression(R"([0-9\/\*\+\-\%\.\s\,\[\]\(\)]*)"), this));
}

std::optional<std::vector<double>> NumericalEvalTupleEntry::evaluated() const {
    return NumericExpression::evaluateTuple(text());
}

}  // namespace flatcam::gui
