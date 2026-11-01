/**
 * @file JsonValue.h
 * @brief Valor JSON liviano para los proyectos de FlatCAM (`.FlatPrj`).
 */
#pragma once

#include <string>
#include <utility>
#include <vector>

namespace flatcam {

/**
 * @brief Un valor JSON: nulo, booleano, número, texto, lista u objeto.
 *
 * Los números guardan su texto original (por ejemplo `-3.400000000000001` o `2`), para poder distinguir enteros de
 * reales y volver a escribirlos igual que Python. Los objetos conservan el orden de sus claves.
 */
class JsonValue {
public:
    /** @brief Tipo del valor. */
    enum class Type { Null, Bool, Number, String, Array, Object };

    /** @brief Crea un nulo. */
    JsonValue() = default;
    /** @brief Crea un booleano. @param b Valor. @return El valor. */
    static JsonValue boolean(bool b);
    /** @brief Crea un número a partir de su texto. @param text Texto (`2`, `-1.5`, `NaN`...). @return El valor. */
    static JsonValue number(std::string text);
    /** @brief Crea un número real escrito como `repr` de Python. @param v Valor. @return El valor. */
    static JsonValue real(double v);
    /** @brief Crea un número entero. @param v Valor. @return El valor. */
    static JsonValue integer(long long v);
    /** @brief Crea un texto. @param s Texto en UTF-8. @return El valor. */
    static JsonValue string(std::string s);
    /** @brief Crea una lista vacía. @return El valor. */
    static JsonValue array();
    /** @brief Crea un objeto vacío. @return El valor. */
    static JsonValue object();

    /** @brief Tipo. @return El tipo. */
    Type type() const { return type_; }
    /** @brief Si es nulo. @return `true` si lo es. */
    bool isNull() const { return type_ == Type::Null; }
    /** @brief Si es texto. @return `true` si lo es. */
    bool isString() const { return type_ == Type::String; }
    /** @brief Si es número. @return `true` si lo es. */
    bool isNumber() const { return type_ == Type::Number; }
    /** @brief Si es lista. @return `true` si lo es. */
    bool isArray() const { return type_ == Type::Array; }
    /** @brief Si es objeto. @return `true` si lo es. */
    bool isObject() const { return type_ == Type::Object; }
    /** @brief Si es booleano. @return `true` si lo es. */
    bool isBool() const { return type_ == Type::Bool; }
    /** @brief Si el número no tiene punto ni exponente (un `int` de Python). @return `true` si es entero. */
    bool isInteger() const;

    /** @brief Valor booleano (los números distintos de cero también cuentan). @param def Si no es booleano. @return El valor. */
    bool toBool(bool def = false) const;
    /** @brief Valor numérico. @param def Si no es número. @return El valor. */
    double toDouble(double def = 0.0) const;
    /** @brief Texto (de un texto o de un número). @return El texto. */
    const std::string& text() const { return text_; }

    /** @brief Elementos de una lista. @return Los elementos. */
    const std::vector<JsonValue>& items() const { return items_; }
    /** @brief Elementos de una lista (modificables). @return Los elementos. */
    std::vector<JsonValue>& items() { return items_; }
    /** @brief Miembros de un objeto, en orden. @return Los miembros. */
    const std::vector<std::pair<std::string, JsonValue>>& members() const { return members_; }
    /** @brief Miembros de un objeto (modificables). @return Los miembros. */
    std::vector<std::pair<std::string, JsonValue>>& members() { return members_; }

    /** @brief Busca un miembro. @param key Clave. @return El valor, o `nullptr` si no está (o no es objeto). */
    const JsonValue* find(const std::string& key) const;
    /** @brief Busca un miembro. @param key Clave. @return El valor, o `nullptr`. */
    JsonValue* find(const std::string& key);
    /** @brief Un miembro, o un nulo si no está. @param key Clave. @return El valor. */
    const JsonValue& operator[](const std::string& key) const;
    /**
     * @brief Pone un miembro (lo reemplaza si ya estaba).
     * @param key Clave.
     * @param v Valor.
     * @return Referencia al valor guardado.
     */
    JsonValue& set(const std::string& key, JsonValue v);
    /** @brief Agrega un elemento a una lista. @param v Valor. */
    void push(JsonValue v) { items_.push_back(std::move(v)); }

private:
    Type type_{Type::Null};
    bool bool_{false};
    std::string text_;  ///< Texto, o el número tal como se escribió.
    std::vector<JsonValue> items_;
    std::vector<std::pair<std::string, JsonValue>> members_;
};

}  // namespace flatcam
