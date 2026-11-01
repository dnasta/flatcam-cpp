#include "io/JsonValue.h"

#include <cmath>
#include <locale>
#include <sstream>

#include "cam/PyNumber.h"

namespace flatcam {

JsonValue JsonValue::boolean(bool b) {
    JsonValue v;
    v.type_ = Type::Bool;
    v.bool_ = b;
    return v;
}

JsonValue JsonValue::number(std::string text) {
    JsonValue v;
    v.type_ = Type::Number;
    v.text_ = std::move(text);
    return v;
}

JsonValue JsonValue::real(double d) {
    if (std::isnan(d)) return number("NaN");
    if (std::isinf(d)) return number(d > 0 ? "Infinity" : "-Infinity");
    return number(pyFloatRepr(d));
}

JsonValue JsonValue::integer(long long i) { return number(std::to_string(i)); }

JsonValue JsonValue::string(std::string s) {
    JsonValue v;
    v.type_ = Type::String;
    v.text_ = std::move(s);
    return v;
}

JsonValue JsonValue::array() {
    JsonValue v;
    v.type_ = Type::Array;
    return v;
}

JsonValue JsonValue::object() {
    JsonValue v;
    v.type_ = Type::Object;
    return v;
}

bool JsonValue::isInteger() const {
    return type_ == Type::Number && text_.find_first_of(".eEnNI") == std::string::npos;
}

bool JsonValue::toBool(bool def) const {
    if (type_ == Type::Bool) return bool_;
    if (type_ == Type::Number) return toDouble() != 0.0;
    return def;
}

double JsonValue::toDouble(double def) const {
    if (type_ == Type::Bool) return bool_ ? 1.0 : 0.0;
    if (type_ != Type::Number) return def;
    if (text_ == "NaN") return std::nan("");
    if (text_ == "Infinity") return HUGE_VAL;
    if (text_ == "-Infinity") return -HUGE_VAL;
    // Siempre con punto decimal, sin importar el idioma del sistema.
    std::istringstream in(text_);
    in.imbue(std::locale::classic());
    double d = def;
    in >> d;
    return d;
}

const JsonValue* JsonValue::find(const std::string& key) const {
    for (const auto& [k, v] : members_)
        if (k == key) return &v;
    return nullptr;
}

JsonValue* JsonValue::find(const std::string& key) {
    for (auto& [k, v] : members_)
        if (k == key) return &v;
    return nullptr;
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    static const JsonValue null;
    const JsonValue* v = find(key);
    return v ? *v : null;
}

JsonValue& JsonValue::set(const std::string& key, JsonValue v) {
    type_ = Type::Object;
    if (JsonValue* old = find(key)) {
        *old = std::move(v);
        return *old;
    }
    members_.emplace_back(key, std::move(v));
    return members_.back().second;
}

}  // namespace flatcam
