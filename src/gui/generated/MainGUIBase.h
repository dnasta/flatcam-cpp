// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#pragma once

#include <QAction>
#include <QPixmap>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabBar>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCDock.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCTree.h"
#include "gui/widgets/FlatCAMActivityView.h"
#include "gui/widgets/FlatCAMInfoBar.h"
#include "gui/widgets/KeySensitiveListView.h"
#include "gui/widgets/VerticalScrollArea.h"

namespace flatcam::gui {

/**
 * @brief Ventana principal con la misma estructura que `MainGUI` de FlatCAM 2024.4 (generada).
 *
 * `setupUi()` arma menús, acciones, barras, pestañas, barra de estado y paneles tal como quedan en el
 * original después del arranque (primera ejecución, diseño `minimal`). Los miembros tienen el nombre del
 * atributo del original. La lógica (señales, estados) va en la clase derivada `MainGUI`.
 */
class MainGUIBase : public QMainWindow {
public:
    /** @brief Crea la ventana vacía. @param parent Widget padre. */
    explicit MainGUIBase(QWidget* parent = nullptr) : QMainWindow(parent) {}

    /** @brief Construye toda la interfaz (equivale a `MainGUI.__init__` más el arranque de `App`). */
    void setupUi();

    /** @name Objetos de la interfaz (mismo nombre que en el original) */
    ///@{
    FlatCAMActivityView* activity_view{nullptr};  ///< `activity_view` del original.
    QAction* add_drill_array_btn{nullptr};  ///< `add_drill_array_btn` del original.
    QAction* add_drill_btn{nullptr};  ///< `add_drill_btn` del original.
    QAction* add_pad_ar_btn{nullptr};  ///< `add_pad_ar_btn` del original.
    QAction* add_slot_array_btn{nullptr};  ///< `add_slot_array_btn` del original.
    QAction* add_slot_btn{nullptr};  ///< `add_slot_btn` del original.
    QAction* align_btn{nullptr};  ///< `align_btn` del original.
    QAction* align_objects_tool_menuAction{nullptr};  ///< `align_objects_tool_menuAction` del original.
    QAction* aperture_buffer_btn{nullptr};  ///< `aperture_buffer_btn` del original.
    QAction* aperture_copy_btn{nullptr};  ///< `aperture_copy_btn` del original.
    QAction* aperture_delete_btn{nullptr};  ///< `aperture_delete_btn` del original.
    QAction* aperture_eraser_btn{nullptr};  ///< `aperture_eraser_btn` del original.
    QAction* aperture_markarea_btn{nullptr};  ///< `aperture_markarea_btn` del original.
    QAction* aperture_move_btn{nullptr};  ///< `aperture_move_btn` del original.
    QAction* aperture_scale_btn{nullptr};  ///< `aperture_scale_btn` del original.
    FCLabel* axis_status_label{nullptr};  ///< `axis_status_label` del original.
    QAction* cal_btn{nullptr};  ///< `cal_btn` del original.
    QAction* cal_exc_tool_menuAction{nullptr};  ///< `cal_exc_tool_menuAction` del original.
    QAction* calculator_tool_menuAction{nullptr};  ///< `calculator_tool_menuAction` del original.
    QAction* calculators_btn{nullptr};  ///< `calculators_btn` del original.
    QAction* clear_plot_btn{nullptr};  ///< `clear_plot_btn` del original.
    KeySensitiveListView* collection_view{nullptr};  ///< `collection_view` del original.
    QToolBar* coords_toolbar{nullptr};  ///< `coords_toolbar` del original.
    QAction* copper_thieving_tool_menuAction{nullptr};  ///< `copper_thieving_tool_menuAction` del original.
    QAction* copperfill_btn{nullptr};  ///< `copperfill_btn` del original.
    QAction* copy_btn{nullptr};  ///< `copy_btn` del original.
    QAction* copy_drill_btn{nullptr};  ///< `copy_drill_btn` del original.
    QAction* corner_snap_btn{nullptr};  ///< `corner_snap_btn` del original.
    QAction* corners_tool_btn{nullptr};  ///< `corners_tool_btn` del original.
    QAction* corners_tool_menuAction{nullptr};  ///< `corners_tool_menuAction` del original.
    QAction* cutout_btn{nullptr};  ///< `cutout_btn` del original.
    QAction* cutout_tool_menuAction{nullptr};  ///< `cutout_tool_menuAction` del original.
    QAction* dblsided_btn{nullptr};  ///< `dblsided_btn` del original.
    QAction* dblsidedtool_menuAction{nullptr};  ///< `dblsidedtool_menuAction` del original.
    QAction* delete_btn{nullptr};  ///< `delete_btn` del original.
    QAction* delete_drill_btn{nullptr};  ///< `delete_drill_btn` del original.
    QToolBar* delta_coords_toolbar{nullptr};  ///< `delta_coords_toolbar` del original.
    QAction* distance_btn{nullptr};  ///< `distance_btn` del original.
    QAction* distance_min_btn{nullptr};  ///< `distance_min_btn` del original.
    QAction* distance_min_tool_menuAction{nullptr};  ///< `distance_min_tool_menuAction` del original.
    QAction* distance_tool_menuAction{nullptr};  ///< `distance_tool_menuAction` del original.
    QAction* drill_btn{nullptr};  ///< `drill_btn` del original.
    QAction* drilling_tool_menuAction{nullptr};  ///< `drilling_tool_menuAction` del original.
    QAction* editgeo_btn{nullptr};  ///< `editgeo_btn` del original.
    QAction* edrills_tool_menuAction{nullptr};  ///< `edrills_tool_menuAction` del original.
    QAction* etch_btn{nullptr};  ///< `etch_btn` del original.
    QAction* etch_tool_menuAction{nullptr};  ///< `etch_tool_menuAction` del original.
    QAction* exc_add_array_drill_menuitem{nullptr};  ///< `exc_add_array_drill_menuitem` del original.
    QAction* exc_add_array_slot_menuitem{nullptr};  ///< `exc_add_array_slot_menuitem` del original.
    QAction* exc_add_drill_menuitem{nullptr};  ///< `exc_add_drill_menuitem` del original.
    QAction* exc_add_slot_menuitem{nullptr};  ///< `exc_add_slot_menuitem` del original.
    QAction* exc_copy_drill_menuitem{nullptr};  ///< `exc_copy_drill_menuitem` del original.
    QAction* exc_delete_drill_menuitem{nullptr};  ///< `exc_delete_drill_menuitem` del original.
    QToolBar* exc_edit_toolbar{nullptr};  ///< `exc_edit_toolbar` del original.
    QMenu* exc_editor_menu{nullptr};  ///< `exc_editor_menu` del original.
    QAction* exc_move_drill_menuitem{nullptr};  ///< `exc_move_drill_menuitem` del original.
    QAction* exc_resize_drill_menuitem{nullptr};  ///< `exc_resize_drill_menuitem` del original.
    QAction* extract_btn{nullptr};  ///< `extract_btn` del original.
    FlatCAMInfoBar* fcinfo{nullptr};  ///< `fcinfo` del original.
    QAction* fiducial_tool_menuAction{nullptr};  ///< `fiducial_tool_menuAction` del original.
    QAction* fiducials_btn{nullptr};  ///< `fiducials_btn` del original.
    QAction* file_open_btn{nullptr};  ///< `file_open_btn` del original.
    QAction* file_open_excellon_btn{nullptr};  ///< `file_open_excellon_btn` del original.
    QAction* file_open_gerber_btn{nullptr};  ///< `file_open_gerber_btn` del original.
    QAction* file_save_btn{nullptr};  ///< `file_save_btn` del original.
    QAction* film_btn{nullptr};  ///< `film_btn` del original.
    QAction* film_tool_menuAction{nullptr};  ///< `film_tool_menuAction` del original.
    QAction* geo_add_arc_btn{nullptr};  ///< `geo_add_arc_btn` del original.
    QAction* geo_add_arc_menuitem{nullptr};  ///< `geo_add_arc_menuitem` del original.
    QAction* geo_add_buffer_btn{nullptr};  ///< `geo_add_buffer_btn` del original.
    QAction* geo_add_circle_btn{nullptr};  ///< `geo_add_circle_btn` del original.
    QAction* geo_add_circle_menuitem{nullptr};  ///< `geo_add_circle_menuitem` del original.
    QAction* geo_add_paint_btn{nullptr};  ///< `geo_add_paint_btn` del original.
    QAction* geo_add_path_btn{nullptr};  ///< `geo_add_path_btn` del original.
    QAction* geo_add_path_menuitem{nullptr};  ///< `geo_add_path_menuitem` del original.
    QAction* geo_add_polygon_btn{nullptr};  ///< `geo_add_polygon_btn` del original.
    QAction* geo_add_polygon_menuitem{nullptr};  ///< `geo_add_polygon_menuitem` del original.
    QAction* geo_add_rectangle_btn{nullptr};  ///< `geo_add_rectangle_btn` del original.
    QAction* geo_add_rectangle_menuitem{nullptr};  ///< `geo_add_rectangle_menuitem` del original.
    QAction* geo_add_text_btn{nullptr};  ///< `geo_add_text_btn` del original.
    QAction* geo_add_text_menuitem{nullptr};  ///< `geo_add_text_menuitem` del original.
    QAction* geo_buffer_menuitem{nullptr};  ///< `geo_buffer_menuitem` del original.
    QAction* geo_copy_btn{nullptr};  ///< `geo_copy_btn` del original.
    QAction* geo_copy_menuitem{nullptr};  ///< `geo_copy_menuitem` del original.
    QAction* geo_cornersnap_menuitem{nullptr};  ///< `geo_cornersnap_menuitem` del original.
    QAction* geo_cutpath_btn{nullptr};  ///< `geo_cutpath_btn` del original.
    QAction* geo_cutpath_menuitem{nullptr};  ///< `geo_cutpath_menuitem` del original.
    QAction* geo_delete_btn{nullptr};  ///< `geo_delete_btn` del original.
    QAction* geo_delete_menuitem{nullptr};  ///< `geo_delete_menuitem` del original.
    QToolBar* geo_edit_toolbar{nullptr};  ///< `geo_edit_toolbar` del original.
    QMenu* geo_editor_menu{nullptr};  ///< `geo_editor_menu` del original.
    QAction* geo_eraser_btn{nullptr};  ///< `geo_eraser_btn` del original.
    QAction* geo_explode_btn{nullptr};  ///< `geo_explode_btn` del original.
    QAction* geo_intersection_btn{nullptr};  ///< `geo_intersection_btn` del original.
    QAction* geo_intersection_menuitem{nullptr};  ///< `geo_intersection_menuitem` del original.
    QAction* geo_move_btn{nullptr};  ///< `geo_move_btn` del original.
    QAction* geo_move_menuitem{nullptr};  ///< `geo_move_menuitem` del original.
    QAction* geo_paint_menuitem{nullptr};  ///< `geo_paint_menuitem` del original.
    QAction* geo_select_btn{nullptr};  ///< `geo_select_btn` del original.
    QAction* geo_subtract_btn{nullptr};  ///< `geo_subtract_btn` del original.
    QAction* geo_subtract_menuitem{nullptr};  ///< `geo_subtract_menuitem` del original.
    QAction* geo_transform_btn{nullptr};  ///< `geo_transform_btn` del original.
    QAction* geo_transform_menuitem{nullptr};  ///< `geo_transform_menuitem` del original.
    QAction* geo_union_btn{nullptr};  ///< `geo_union_btn` del original.
    QAction* geo_union_menuitem{nullptr};  ///< `geo_union_menuitem` del original.
    QAction* grb_add_buffer_menuitem{nullptr};  ///< `grb_add_buffer_menuitem` del original.
    QAction* grb_add_disc_btn{nullptr};  ///< `grb_add_disc_btn` del original.
    QAction* grb_add_disc_menuitem{nullptr};  ///< `grb_add_disc_menuitem` del original.
    QAction* grb_add_eraser_menuitem{nullptr};  ///< `grb_add_eraser_menuitem` del original.
    QAction* grb_add_markarea_menuitem{nullptr};  ///< `grb_add_markarea_menuitem` del original.
    QAction* grb_add_pad_array_menuitem{nullptr};  ///< `grb_add_pad_array_menuitem` del original.
    QAction* grb_add_pad_btn{nullptr};  ///< `grb_add_pad_btn` del original.
    QAction* grb_add_pad_menuitem{nullptr};  ///< `grb_add_pad_menuitem` del original.
    QAction* grb_add_region_btn{nullptr};  ///< `grb_add_region_btn` del original.
    QAction* grb_add_region_menuitem{nullptr};  ///< `grb_add_region_menuitem` del original.
    QAction* grb_add_scale_menuitem{nullptr};  ///< `grb_add_scale_menuitem` del original.
    QAction* grb_add_semidisc_btn{nullptr};  ///< `grb_add_semidisc_btn` del original.
    QAction* grb_add_semidisc_menuitem{nullptr};  ///< `grb_add_semidisc_menuitem` del original.
    QAction* grb_add_track_btn{nullptr};  ///< `grb_add_track_btn` del original.
    QAction* grb_add_track_menuitem{nullptr};  ///< `grb_add_track_menuitem` del original.
    QAction* grb_convert_poly_btn{nullptr};  ///< `grb_convert_poly_btn` del original.
    QAction* grb_convert_poly_menuitem{nullptr};  ///< `grb_convert_poly_menuitem` del original.
    QAction* grb_copy_menuitem{nullptr};  ///< `grb_copy_menuitem` del original.
    QAction* grb_delete_menuitem{nullptr};  ///< `grb_delete_menuitem` del original.
    QToolBar* grb_edit_toolbar{nullptr};  ///< `grb_edit_toolbar` del original.
    QMenu* grb_editor_menu{nullptr};  ///< `grb_editor_menu` del original.
    QAction* grb_move_menuitem{nullptr};  ///< `grb_move_menuitem` del original.
    QAction* grb_select_btn{nullptr};  ///< `grb_select_btn` del original.
    QAction* grb_transform_btn{nullptr};  ///< `grb_transform_btn` del original.
    QAction* grb_transform_menuitem{nullptr};  ///< `grb_transform_menuitem` del original.
    FCCheckBox* grid_gap_link_cb{nullptr};  ///< `grid_gap_link_cb` del original.
    FCEntry2* grid_gap_x_entry{nullptr};  ///< `grid_gap_x_entry` del original.
    FCEntry2* grid_gap_y_entry{nullptr};  ///< `grid_gap_y_entry` del original.
    QAction* grid_snap_btn{nullptr};  ///< `grid_snap_btn` del original.
    QToolBar* grid_toolbar{nullptr};  ///< `grid_toolbar` del original.
    FCLabel* hud_label{nullptr};  ///< `hud_label` del original.
    QAction* image_tool_menuAction{nullptr};  ///< `image_tool_menuAction` del original.
    QAction* invert_btn{nullptr};  ///< `invert_btn` del original.
    QAction* invert_tool_menuAction{nullptr};  ///< `invert_tool_menuAction` del original.
    QAction* isolation_btn{nullptr};  ///< `isolation_btn` del original.
    QAction* isolation_tool_menuAction{nullptr};  ///< `isolation_tool_menuAction` del original.
    QAction* jmp_btn{nullptr};  ///< `jmp_btn` del original.
    QAction* locate_btn{nullptr};  ///< `locate_btn` del original.
    QAction* menu_toggle_nb{nullptr};  ///< `menu_toggle_nb` del original.
    QMenu* menuedit{nullptr};  ///< `menuedit` del original.
    QMenu* menuedit_convert{nullptr};  ///< `menuedit_convert` del original.
    QAction* menuedit_convert_mg2sg{nullptr};  ///< `menuedit_convert_mg2sg` del original.
    QAction* menuedit_convert_sg2mg{nullptr};  ///< `menuedit_convert_sg2mg` del original.
    QAction* menuedit_move2origin{nullptr};  ///< `menuedit_move2origin` del original.
    QAction* menueditconvert_any2excellon{nullptr};  ///< `menueditconvert_any2excellon` del original.
    QAction* menueditconvert_any2geo{nullptr};  ///< `menueditconvert_any2geo` del original.
    QAction* menueditconvert_any2gerber{nullptr};  ///< `menueditconvert_any2gerber` del original.
    QAction* menueditcopyobject{nullptr};  ///< `menueditcopyobject` del original.
    QAction* menueditdelete{nullptr};  ///< `menueditdelete` del original.
    QAction* menueditedit{nullptr};  ///< `menueditedit` del original.
    QAction* menueditjump{nullptr};  ///< `menueditjump` del original.
    QAction* menueditlocate{nullptr};  ///< `menueditlocate` del original.
    QAction* menueditok{nullptr};  ///< `menueditok` del original.
    QAction* menueditorigin{nullptr};  ///< `menueditorigin` del original.
    QAction* menueditpreferences{nullptr};  ///< `menueditpreferences` del original.
    QAction* menueditselectall{nullptr};  ///< `menueditselectall` del original.
    QAction* menuedittoggleunits{nullptr};  ///< `menuedittoggleunits` del original.
    QMenu* menufile{nullptr};  ///< `menufile` del original.
    QMenu* menufile_backup{nullptr};  ///< `menufile_backup` del original.
    QAction* menufile_exit{nullptr};  ///< `menufile_exit` del original.
    QMenu* menufile_open{nullptr};  ///< `menufile_open` del original.
    QAction* menufile_print{nullptr};  ///< `menufile_print` del original.
    QMenu* menufile_save{nullptr};  ///< `menufile_save` del original.
    QMenu* menufile_scripting{nullptr};  ///< `menufile_scripting` del original.
    QMenu* menufileexport{nullptr};  ///< `menufileexport` del original.
    QAction* menufileexportdxf{nullptr};  ///< `menufileexportdxf` del original.
    QAction* menufileexportexcellon{nullptr};  ///< `menufileexportexcellon` del original.
    QAction* menufileexportgerber{nullptr};  ///< `menufileexportgerber` del original.
    QAction* menufileexportpng{nullptr};  ///< `menufileexportpng` del original.
    QAction* menufileexportpref{nullptr};  ///< `menufileexportpref` del original.
    QAction* menufileexportsvg{nullptr};  ///< `menufileexportsvg` del original.
    QMenu* menufileimport{nullptr};  ///< `menufileimport` del original.
    QAction* menufileimport_hpgl2_as_geo{nullptr};  ///< `menufileimport_hpgl2_as_geo` del original.
    QAction* menufileimportdxf{nullptr};  ///< `menufileimportdxf` del original.
    QAction* menufileimportdxf_as_gerber{nullptr};  ///< `menufileimportdxf_as_gerber` del original.
    QAction* menufileimportpref{nullptr};  ///< `menufileimportpref` del original.
    QAction* menufileimportsvg{nullptr};  ///< `menufileimportsvg` del original.
    QAction* menufileimportsvg_as_gerber{nullptr};  ///< `menufileimportsvg_as_gerber` del original.
    QMenu* menufilenew{nullptr};  ///< `menufilenew` del original.
    QAction* menufilenewdoc{nullptr};  ///< `menufilenewdoc` del original.
    QAction* menufilenewexc{nullptr};  ///< `menufilenewexc` del original.
    QAction* menufilenewgeo{nullptr};  ///< `menufilenewgeo` del original.
    QAction* menufilenewgrb{nullptr};  ///< `menufilenewgrb` del original.
    QAction* menufilenewproject{nullptr};  ///< `menufilenewproject` del original.
    QAction* menufilenewscript{nullptr};  ///< `menufilenewscript` del original.
    QAction* menufileopenconfig{nullptr};  ///< `menufileopenconfig` del original.
    QAction* menufileopenexcellon{nullptr};  ///< `menufileopenexcellon` del original.
    QAction* menufileopengcode{nullptr};  ///< `menufileopengcode` del original.
    QAction* menufileopengerber{nullptr};  ///< `menufileopengerber` del original.
    QAction* menufileopenproject{nullptr};  ///< `menufileopenproject` del original.
    QAction* menufileopenscript{nullptr};  ///< `menufileopenscript` del original.
    QAction* menufileopenscriptexample{nullptr};  ///< `menufileopenscriptexample` del original.
    QAction* menufilerunscript{nullptr};  ///< `menufilerunscript` del original.
    QAction* menufilesavedefaults{nullptr};  ///< `menufilesavedefaults` del original.
    QAction* menufilesaveproject{nullptr};  ///< `menufilesaveproject` del original.
    QAction* menufilesaveprojectas{nullptr};  ///< `menufilesaveprojectas` del original.
    QMenu* menuhelp{nullptr};  ///< `menuhelp` del original.
    QAction* menuhelp_about{nullptr};  ///< `menuhelp_about` del original.
    QAction* menuhelp_exc_spec{nullptr};  ///< `menuhelp_exc_spec` del original.
    QAction* menuhelp_gerber_spec{nullptr};  ///< `menuhelp_gerber_spec` del original.
    QAction* menuhelp_manual{nullptr};  ///< `menuhelp_manual` del original.
    QAction* menuhelp_readme{nullptr};  ///< `menuhelp_readme` del original.
    QAction* menuhelp_report_bug{nullptr};  ///< `menuhelp_report_bug` del original.
    QAction* menuhelp_shortcut_list{nullptr};  ///< `menuhelp_shortcut_list` del original.
    QAction* menuhelp_videohelp{nullptr};  ///< `menuhelp_videohelp` del original.
    QMenu* menuobjects{nullptr};  ///< `menuobjects` del original.
    QAction* menuobjects_selall{nullptr};  ///< `menuobjects_selall` del original.
    QAction* menuobjects_unselall{nullptr};  ///< `menuobjects_unselall` del original.
    QMenu* menuoptions{nullptr};  ///< `menuoptions` del original.
    QAction* menuoptions_tools_db{nullptr};  ///< `menuoptions_tools_db` del original.
    QAction* menuoptions_transform_flipx{nullptr};  ///< `menuoptions_transform_flipx` del original.
    QAction* menuoptions_transform_flipy{nullptr};  ///< `menuoptions_transform_flipy` del original.
    QAction* menuoptions_transform_rotate{nullptr};  ///< `menuoptions_transform_rotate` del original.
    QAction* menuoptions_transform_skewx{nullptr};  ///< `menuoptions_transform_skewx` del original.
    QAction* menuoptions_transform_skewy{nullptr};  ///< `menuoptions_transform_skewy` del original.
    QAction* menuoptions_view_source{nullptr};  ///< `menuoptions_view_source` del original.
    QMenu* menutool{nullptr};  ///< `menutool` del original.
    QAction* menutoolshell{nullptr};  ///< `menutoolshell` del original.
    QMenu* menuview{nullptr};  ///< `menuview` del original.
    QAction* menuview_replot{nullptr};  ///< `menuview_replot` del original.
    QAction* menuview_toggle_axis{nullptr};  ///< `menuview_toggle_axis` del original.
    QAction* menuview_toggle_code_editor{nullptr};  ///< `menuview_toggle_code_editor` del original.
    QAction* menuview_toggle_fscreen{nullptr};  ///< `menuview_toggle_fscreen` del original.
    QAction* menuview_toggle_grid{nullptr};  ///< `menuview_toggle_grid` del original.
    QAction* menuview_toggle_grid_lines{nullptr};  ///< `menuview_toggle_grid_lines` del original.
    QAction* menuview_toggle_hud{nullptr};  ///< `menuview_toggle_hud` del original.
    QAction* menuview_toggle_notebook{nullptr};  ///< `menuview_toggle_notebook` del original.
    QAction* menuview_toggle_parea{nullptr};  ///< `menuview_toggle_parea` del original.
    QAction* menuview_toggle_workspace{nullptr};  ///< `menuview_toggle_workspace` del original.
    QAction* menuview_zoom_fit{nullptr};  ///< `menuview_zoom_fit` del original.
    QAction* menuview_zoom_in{nullptr};  ///< `menuview_zoom_in` del original.
    QAction* menuview_zoom_out{nullptr};  ///< `menuview_zoom_out` del original.
    QAction* menuviewdisableall{nullptr};  ///< `menuviewdisableall` del original.
    QAction* menuviewdisableother{nullptr};  ///< `menuviewdisableother` del original.
    QAction* menuviewenable{nullptr};  ///< `menuviewenable` del original.
    QAction* menuviewenableother{nullptr};  ///< `menuviewenableother` del original.
    QAction* move2origin_btn{nullptr};  ///< `move2origin_btn` del original.
    QAction* move_drill_btn{nullptr};  ///< `move_drill_btn` del original.
    QAction* move_tool_menuAction{nullptr};  ///< `move_tool_menuAction` del original.
    QAction* ncc_btn{nullptr};  ///< `ncc_btn` del original.
    QAction* ncclear_tool_menuAction{nullptr};  ///< `ncclear_tool_menuAction` del original.
    QAction* new_script_btn{nullptr};  ///< `new_script_btn` del original.
    FCDetachableTab* notebook{nullptr};  ///< `notebook` del original.
    QAction* open_script_btn{nullptr};  ///< `open_script_btn` del original.
    QAction* optimal_btn{nullptr};  ///< `optimal_btn` del original.
    QAction* optimal_tool_menuAction{nullptr};  ///< `optimal_tool_menuAction` del original.
    QAction* origin_btn{nullptr};  ///< `origin_btn` del original.
    QAction* paint_btn{nullptr};  ///< `paint_btn` del original.
    QAction* paint_tool_menuAction{nullptr};  ///< `paint_tool_menuAction` del original.
    QAction* panelize_btn{nullptr};  ///< `panelize_btn` del original.
    QAction* panelize_tool_menuAction{nullptr};  ///< `panelize_tool_menuAction` del original.
    QAction* paste_tool_menuAction{nullptr};  ///< `paste_tool_menuAction` del original.
    QAction* pcb_wizard_tool_menuAction{nullptr};  ///< `pcb_wizard_tool_menuAction` del original.
    QAction* pdf_tool_menuAction{nullptr};  ///< `pdf_tool_menuAction` del original.
    QWidget* plot_tab{nullptr};  ///< `plot_tab` del original.
    FCDetachableTab2* plot_tab_area{nullptr};  ///< `plot_tab_area` del original.
    FCLabel* position_label{nullptr};  ///< `position_label` del original.
    FCLabel* pref_status_label{nullptr};  ///< `pref_status_label` del original.
    QFrame* project_frame{nullptr};  ///< `project_frame` del original.
    QVBoxLayout* project_frame_lay{nullptr};  ///< `project_frame_lay` del original.
    QWidget* project_tab{nullptr};  ///< `project_tab` del original.
    QVBoxLayout* project_tab_layout{nullptr};  ///< `project_tab_layout` del original.
    VerticalScrollArea* properties_scroll_area{nullptr};  ///< `properties_scroll_area` del original.
    QWidget* properties_tab{nullptr};  ///< `properties_tab` del original.
    QVBoxLayout* properties_tab_layout{nullptr};  ///< `properties_tab_layout` del original.
    QAction* properties_tool_menuAction{nullptr};  ///< `properties_tool_menuAction` del original.
    QAction* punch_btn{nullptr};  ///< `punch_btn` del original.
    QAction* punch_tool_menuAction{nullptr};  ///< `punch_tool_menuAction` del original.
    QAction* qrcode_btn{nullptr};  ///< `qrcode_btn` del original.
    QAction* qrcode_tool_menuAction{nullptr};  ///< `qrcode_tool_menuAction` del original.
    QMenu* recent{nullptr};  ///< `recent` del original.
    QMenu* recent_projects{nullptr};  ///< `recent_projects` del original.
    FCLabel* rel_position_label{nullptr};  ///< `rel_position_label` del original.
    QAction* replot_btn{nullptr};  ///< `replot_btn` del original.
    QAction* resize_drill_btn{nullptr};  ///< `resize_drill_btn` del original.
    QVBoxLayout* right_lay{nullptr};  ///< `right_lay` del original.
    QVBoxLayout* right_layout{nullptr};  ///< `right_layout` del original.
    QWidget* right_widget{nullptr};  ///< `right_widget` del original.
    QAction* rules_btn{nullptr};  ///< `rules_btn` del original.
    QAction* rules_tool_menuAction{nullptr};  ///< `rules_tool_menuAction` del original.
    QAction* run_script_btn{nullptr};  ///< `run_script_btn` del original.
    QAction* select_drill_btn{nullptr};  ///< `select_drill_btn` del original.
    QAction* shell_btn{nullptr};  ///< `shell_btn` del original.
    FCDock* shell_dock{nullptr};  ///< `shell_dock` del original.
    FCLabel* shell_status_label{nullptr};  ///< `shell_status_label` del original.
    QAction* snap_magnet{nullptr};  ///< `snap_magnet` del original.
    FCEntry* snap_max_dist_entry{nullptr};  ///< `snap_max_dist_entry` del original.
    QAction* solder_btn{nullptr};  ///< `solder_btn` del original.
    QSplitter* splitter{nullptr};  ///< `splitter` del original.
    QSplitter* splitter_left{nullptr};  ///< `splitter_left` del original.
    QToolBar* status_toolbar{nullptr};  ///< `status_toolbar` del original.
    QAction* sub_btn{nullptr};  ///< `sub_btn` del original.
    QAction* sub_tool_menuAction{nullptr};  ///< `sub_tool_menuAction` del original.
    VerticalScrollArea* tool_scroll_area{nullptr};  ///< `tool_scroll_area` del original.
    QWidget* tool_tab{nullptr};  ///< `tool_tab` del original.
    QVBoxLayout* tool_tab_layout{nullptr};  ///< `tool_tab_layout` del original.
    QToolBar* toolbaredit{nullptr};  ///< `toolbaredit` del original.
    QToolBar* toolbarfile{nullptr};  ///< `toolbarfile` del original.
    QToolBar* toolbarshell{nullptr};  ///< `toolbarshell` del original.
    QToolBar* toolbartools{nullptr};  ///< `toolbartools` del original.
    QToolBar* toolbarview{nullptr};  ///< `toolbarview` del original.
    QAction* transform_btn{nullptr};  ///< `transform_btn` del original.
    QAction* transform_tool_menuAction{nullptr};  ///< `transform_tool_menuAction` del original.
    FCLabel* units_label{nullptr};  ///< `units_label` del original.
    QAction* update_obj_btn{nullptr};  ///< `update_obj_btn` del original.
    FCLabel* wplace_label{nullptr};  ///< `wplace_label` del original.
    QAction* zoom_fit_btn{nullptr};  ///< `zoom_fit_btn` del original.
    QAction* zoom_in_btn{nullptr};  ///< `zoom_in_btn` del original.
    QAction* zoom_out_btn{nullptr};  ///< `zoom_out_btn` del original.
    ///@}
};

}  // namespace flatcam::gui
