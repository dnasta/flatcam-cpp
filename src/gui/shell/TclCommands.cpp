// register_all_commands: los comandos portados y, para los demás del original, uno que avisa que todavía no está
// (así `help` lista lo mismo que el original).
#include "gui/shell/TclCommands.h"

#include <set>

#include "gui/shell/TclCommand.h"

namespace flatcam::gui {

namespace {

class NotPorted : public TclCommand {
public:
    NotPorted(App* a, const QString& cls) : TclCommand(a, cls) {}

protected:
    std::optional<QString> execute(const TclArgs&) override {
        raiseTclError(QString("El comando %1 todavía no está en el port.").arg(meta().aliases.value(0)));
    }
};

}  // namespace

std::vector<std::unique_ptr<TclCommand>> createTclCommands(App* app) {
    std::vector<std::unique_ptr<TclCommand>> out;
    addAppTclCommands(app, out);
    std::set<QString> done;
    for (const auto& c : out) done.insert(c->meta().className);
    for (const TclCommandMeta& m : TclCommandMeta::all())
        if (!done.count(m.className) && m.registered) out.push_back(std::make_unique<NotPorted>(app, m.className));
    return out;
}

}  // namespace flatcam::gui
