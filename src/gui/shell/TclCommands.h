/**
 * @file TclCommands.h
 * @brief Las tandas de comandos de la consola Tcl (cada una en su archivo) que arma `createTclCommands` (TclCommand.h).
 */
#pragma once

#include <memory>
#include <vector>

namespace flatcam::gui {

class App;
class TclCommand;

/**
 * @brief help, version, get_sys, set_sys, list_sys, save_sys, get_names, set_active, clear, new, quit_flatcam,
 * get_path, set_path, plot_all y plot_objects.
 * @param app La aplicación. @param out Donde se agregan.
 */
void addAppTclCommands(App* app, std::vector<std::unique_ptr<TclCommand>>& out);

}  // namespace flatcam::gui
