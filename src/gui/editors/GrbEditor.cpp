#include "gui/editors/GrbEditor.h"

#include "gui/App.h"

namespace flatcam::gui {

GrbEditor::GrbEditor(App* app) : QObject(app), app_(app) {}

void GrbEditor::edit(GerberObject* obj) {
    (void)obj;
    active_ = true;
}

void GrbEditor::save(GerberObject* obj) {
    (void)obj;
    deactivate();
}

void GrbEditor::deactivate() { active_ = false; }

bool GrbEditor::handleKey(int key, Qt::KeyboardModifiers modifiers) {
    (void)key;
    (void)modifiers;
    return false;
}

}  // namespace flatcam::gui
