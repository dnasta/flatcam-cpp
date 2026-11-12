/**
 * @file GrbEditor.h
 * @brief El editor de Gerber (port de `appEditors/AppGerberEditor.py`).
 */
#pragma once

#include <QObject>

namespace flatcam::gui {

class App;
class GerberObject;

/** @brief El editor de Gerber (port de `appEditors/AppGerberEditor.py`). */
class GrbEditor : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param app La aplicación. */
    explicit GrbEditor(App* app);
    /** @brief Si está activo. @return `true` si lo está. */
    bool active() const { return active_; }
    /** @brief Abre el objeto en el editor. @param obj El objeto. */
    void edit(GerberObject* obj);
    /** @brief Devuelve lo editado al objeto y cierra. @param obj El objeto. */
    void save(GerberObject* obj);
    /** @brief Cierra sin guardar. */
    void deactivate();
    /** @brief Las teclas del editor. @param key Tecla. @param modifiers Modificadores. @return Si se usó. */
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);

private:
    App* app_;
    bool active_{false};
};

}  // namespace flatcam::gui
