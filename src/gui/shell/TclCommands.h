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

/**
 * @brief Archivos y objetos: open_gerber, open_excellon, open_gcode, open_project, save_project, open_svg,
 * open_dxf, export_gerber, export_excellon, export_svg, export_dxf, export_gcode, delete, new_geometry, new_gerber,
 * new_excellon, offset, scale, skew, mirror, set_origin, bounds, join_geometries, join_excellons, add_circle,
 * add_rectangle, add_polygon, add_polyline, geo_union, exteriors, interiors, subtract_poly y subtract_rectangle.
 * @param app La aplicación. @param out Donde se agregan.
 */
void addObjectTclCommands(App* app, std::vector<std::unique_ptr<TclCommand>>& out);

}  // namespace flatcam::gui
