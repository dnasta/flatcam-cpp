/**
 * @file NumericalEval.h
 * @brief Campos que aceptan fórmulas numéricas (port de `NumericalEvalEntry` y `NumericalEvalTupleEntry` de
 * `appGUI/GUIElements.py`).
 */
#pragma once

#include <optional>
#include <vector>

#include "gui/widgets/FCEntry.h"

namespace flatcam::gui {

/**
 * @brief Evalúa fórmulas numéricas: números, `+ - * / %`, signos y paréntesis.
 *
 * Reemplaza al `eval` de Python que usa el original. Solo acepta lo mismo que permiten los validadores de los campos
 * (dígitos, operadores, punto, coma, espacios, corchetes y paréntesis), así que da el mismo resultado sin ejecutar código.
 * `%` es el resto de la división, como en Python (con el signo del divisor).
 */
class NumericExpression {
public:
    /** @brief Evalúa una expresión. @param text Expresión. @return El valor, o vacío si no es válida. */
    static std::optional<double> evaluate(const QString& text);

    /**
     * @brief Evalúa una lista de expresiones separadas por comas, con o sin paréntesis o corchetes (por ejemplo
     * `(0, 0)` o `1.5, 2*3`).
     * @param text Texto.
     * @return Los valores, o vacío si alguno no es válido.
     */
    static std::optional<std::vector<double>> evaluateTuple(const QString& text);
};

/** @brief Campo que acepta una fórmula y devuelve su valor; la coma decimal vale como punto (`NumericalEvalEntry`). */
class NumericalEvalEntry : public FCEntry {
    Q_OBJECT
public:
    /** @brief Crea el campo con su validador. @param parent Widget padre. */
    explicit NumericalEvalEntry(QWidget* parent = nullptr);

    /** @brief Valor de la fórmula escrita (`get_value`). @return El valor, o vacío. */
    std::optional<double> evaluated() const;
};

/** @brief Campo que acepta una tupla de fórmulas, como `(0, 0)` (`NumericalEvalTupleEntry`). */
class NumericalEvalTupleEntry : public FCEntry {
    Q_OBJECT
public:
    /** @brief Crea el campo con su validador. @param parent Widget padre. */
    explicit NumericalEvalTupleEntry(QWidget* parent = nullptr);

    /** @brief Valores de la tupla escrita (`get_value`). @return Los valores, o vacío. */
    std::optional<std::vector<double>> evaluated() const;
};

}  // namespace flatcam::gui
