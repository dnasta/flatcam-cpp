/**
 * @file ExcEditor.h
 * @brief El editor de Excellon (port de `appEditors/AppExcEditor.py`).
 */
#pragma once

#include <QObject>

namespace flatcam::gui {

class App;
class ExcellonObject;

/** @brief El editor de Excellon (port de `appEditors/AppExcEditor.py`). */
class ExcEditor : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param app La aplicación. */
    explicit ExcEditor(App* app);
    /** @brief Si está activo. @return `true` si lo está. */
    bool active() const { return active_; }
    /** @brief Abre el objeto en el editor. @param obj El objeto. */
    void edit(ExcellonObject* obj);
    /** @brief Devuelve lo editado al objeto y cierra. @param obj El objeto. */
    void save(ExcellonObject* obj);
    /** @brief Cierra sin guardar. */
    void deactivate();
    /** @brief Las teclas del editor. @param key Tecla. @param modifiers Modificadores. @return Si se usó. */
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);

private:
    App* app_;
    bool active_{false};
};

}  // namespace flatcam::gui
