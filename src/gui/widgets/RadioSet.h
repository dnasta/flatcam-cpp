/**
 * @file RadioSet.h
 * @brief Grupo de botones de opción con valores (port de `RadioSet` de `appGUI/GUIElements.py`).
 */
#pragma once

#include <QRadioButton>
#include <QVariant>
#include <QWidget>
#include <vector>

namespace flatcam::gui {

/**
 * @brief Botones de opción excluyentes; cada uno tiene un texto y un valor (por ejemplo "Adelante" / `"fwd"`).
 */
class RadioSet : public QWidget {
    Q_OBJECT
public:
    /** @brief Una opción: texto visible y valor. */
    struct Choice {
        QString label;  ///< Texto del botón.
        QVariant value; ///< Valor que devuelve @ref value.
    };

    /**
     * @brief Crea el grupo.
     * @param choices Opciones, en orden.
     * @param orientation Horizontal (por defecto) o vertical.
     * @param stretch Si es `true` se agrega un espacio elástico al final (como el original salvo `stretch=False`).
     * @param parent Widget padre.
     */
    explicit RadioSet(std::vector<Choice> choices, Qt::Orientation orientation = Qt::Horizontal, bool stretch = true,
                      QWidget* parent = nullptr);

    /** @brief Valor de la opción elegida (`get_value`). @return El valor, o inválido si no hay ninguna. */
    QVariant value() const;

    /** @brief Elige la opción con ese valor (`set_value`). @param val Valor. */
    void setValue(const QVariant& val);

    /** @brief Habilita o deshabilita opciones por texto (`setOptionsDisabled`). @param labels Textos. @param disabled Deshabilitar. */
    void setOptionsDisabled(const QStringList& labels, bool disabled);

    /** @brief Botón de una opción. @param i Índice. @return El botón. */
    QRadioButton* button(int i) const { return buttons_.at(static_cast<std::size_t>(i)); }

Q_SIGNALS:
    /** @brief Se eligió otra opción (`activated_custom`). @param value Valor elegido, como texto. */
    void activatedCustom(const QString& value);

private:
    std::vector<Choice> choices_;
    std::vector<QRadioButton*> buttons_;
};

}  // namespace flatcam::gui
