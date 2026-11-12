#include "gui/editors/ExcEditor.h"

#include "gui/App.h"

namespace flatcam::gui {

ExcEditor::ExcEditor(App* app) : QObject(app), app_(app) {}

void ExcEditor::edit(ExcellonObject* obj) {
    (void)obj;
    active_ = true;
}

void ExcEditor::save(ExcellonObject* obj) {
    (void)obj;
    deactivate();
}

void ExcEditor::deactivate() { active_ = false; }

bool ExcEditor::handleKey(int key, Qt::KeyboardModifiers modifiers) {
    (void)key;
    (void)modifiers;
    return false;
}

}  // namespace flatcam::gui
