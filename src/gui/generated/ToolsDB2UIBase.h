// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#pragma once

#include <QDialog>
#include <QAction>
#include <QPixmap>
#include <QFormLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QSlider>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
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
#include "gui/widgets/FCButton.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTree.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `ToolsDB2UI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class ToolsDB2UIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit ToolsDB2UIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_entry_btn{nullptr};  ///< `add_entry_btn` del original.
    FCButton* add_tool_from_db{nullptr};  ///< `add_tool_from_db` del original.
    QHBoxLayout* buttons_box{nullptr};  ///< `buttons_box` del original.
    QFrame* buttons_frame{nullptr};  ///< `buttons_frame` del original.
    FCButton* cancel_tool_from_db{nullptr};  ///< `cancel_tool_from_db` del original.
    FCLabel* custom_offset_label{nullptr};  ///< `custom_offset_label` del original.
    QGroupBox* cutout_box{nullptr};  ///< `cutout_box` del original.
    FCCheckBox* cutout_convex_box{nullptr};  ///< `cutout_convex_box` del original.
    FCComboBox* cutout_gaps{nullptr};  ///< `cutout_gaps` del original.
    FCLabel* cutout_gaps_label{nullptr};  ///< `cutout_gaps_label` del original.
    FCDoubleSpinner* cutout_gapsize{nullptr};  ///< `cutout_gapsize` del original.
    FCLabel* cutout_gapsize_label{nullptr};  ///< `cutout_gapsize_label` del original.
    RadioSet* cutout_gaptype_radio{nullptr};  ///< `cutout_gaptype_radio` del original.
    FCDoubleSpinner* cutout_margin_entry{nullptr};  ///< `cutout_margin_entry` del original.
    FCLabel* cutout_margin_label{nullptr};  ///< `cutout_margin_label` del original.
    FCDoubleSpinner* cutout_mb_dia_entry{nullptr};  ///< `cutout_mb_dia_entry` del original.
    FCDoubleSpinner* cutout_mb_spacing_entry{nullptr};  ///< `cutout_mb_spacing_entry` del original.
    FCDoubleSpinner* cutout_thin_depth_entry{nullptr};  ///< `cutout_thin_depth_entry` del original.
    QVBoxLayout* cutout_vlay{nullptr};  ///< `cutout_vlay` del original.
    FCLabel* cutz_label{nullptr};  ///< `cutz_label` del original.
    FCLabel* cutzlabel{nullptr};  ///< `cutzlabel` del original.
    QVBoxLayout* description_vlay{nullptr};  ///< `description_vlay` del original.
    FCDoubleSpinner* dia_entry{nullptr};  ///< `dia_entry` del original.
    FCLabel* dia_label{nullptr};  ///< `dia_label` del original.
    FCLabel* dpp_drill_label{nullptr};  ///< `dpp_drill_label` del original.
    FCLabel* dpp_label{nullptr};  ///< `dpp_label` del original.
    QGroupBox* drill_box{nullptr};  ///< `drill_box` del original.
    FCDoubleSpinner* drill_cutz_entry{nullptr};  ///< `drill_cutz_entry` del original.
    FCCheckBox* drill_dwell_cb{nullptr};  ///< `drill_dwell_cb` del original.
    FCDoubleSpinner* drill_dwelltime_entry{nullptr};  ///< `drill_dwelltime_entry` del original.
    FCDoubleSpinner* drill_feedrate_rapid_entry{nullptr};  ///< `drill_feedrate_rapid_entry` del original.
    FCDoubleSpinner* drill_feedrate_z_entry{nullptr};  ///< `drill_feedrate_z_entry` del original.
    FCCheckBox* drill_last_drill_cb{nullptr};  ///< `drill_last_drill_cb` del original.
    FCDoubleSpinner* drill_maxdepth_entry{nullptr};  ///< `drill_maxdepth_entry` del original.
    FCCheckBox* drill_mpass_cb{nullptr};  ///< `drill_mpass_cb` del original.
    FCDoubleSpinner* drill_offset_entry{nullptr};  ///< `drill_offset_entry` del original.
    FCDoubleSpinner* drill_overlap_entry{nullptr};  ///< `drill_overlap_entry` del original.
    FCLabel* drill_overlap_label{nullptr};  ///< `drill_overlap_label` del original.
    FCCheckBox* drill_slots_cb{nullptr};  ///< `drill_slots_cb` del original.
    FCLabel* drill_slots_drill_lbl{nullptr};  ///< `drill_slots_drill_lbl` del original.
    FCSpinner* drill_spindlespeed_entry{nullptr};  ///< `drill_spindlespeed_entry` del original.
    FCDoubleSpinner* drill_travelz_entry{nullptr};  ///< `drill_travelz_entry` del original.
    QVBoxLayout* drill_vlay{nullptr};  ///< `drill_vlay` del original.
    FCLabel* dwell_drill_label{nullptr};  ///< `dwell_drill_label` del original.
    FCLabel* dwell_label{nullptr};  ///< `dwell_label` del original.
    FCLabel* dwelltime_drill_lbl{nullptr};  ///< `dwelltime_drill_lbl` del original.
    FCLabel* dwelltime_label{nullptr};  ///< `dwelltime_label` del original.
    FCLabel* ecut_label{nullptr};  ///< `ecut_label` del original.
    FCLabel* ecut_length_label{nullptr};  ///< `ecut_length_label` del original.
    FCButton* export_db_btn{nullptr};  ///< `export_db_btn` del original.
    FCLabel* feedrate_rapid_label{nullptr};  ///< `feedrate_rapid_label` del original.
    FCLabel* follow_label{nullptr};  ///< `follow_label` del original.
    FCLabel* frapids_label{nullptr};  ///< `frapids_label` del original.
    FCLabel* frxy_label{nullptr};  ///< `frxy_label` del original.
    FCLabel* frz_label{nullptr};  ///< `frz_label` del original.
    FCLabel* frzlabel{nullptr};  ///< `frzlabel` del original.
    QGridLayout* g_lay{nullptr};  ///< `g_lay` del original.
    FCLabel* gaptype_label{nullptr};  ///< `gaptype_label` del original.
    QGridLayout* grid0{nullptr};  ///< `grid0` del original.
    QGridLayout* grid2{nullptr};  ///< `grid2` del original.
    QGridLayout* grid3{nullptr};  ///< `grid3` del original.
    QGridLayout* grid4{nullptr};  ///< `grid4` del original.
    QGridLayout* grid5{nullptr};  ///< `grid5` del original.
    QGridLayout* grid6{nullptr};  ///< `grid6` del original.
    QGridLayout* grid_tool{nullptr};  ///< `grid_tool` del original.
    FCButton* import_db_btn{nullptr};  ///< `import_db_btn` del original.
    QGroupBox* iso_box{nullptr};  ///< `iso_box` del original.
    FCCheckBox* iso_follow_cb{nullptr};  ///< `iso_follow_cb` del original.
    FCLabel* iso_milling_type_label{nullptr};  ///< `iso_milling_type_label` del original.
    RadioSet* iso_milling_type_radio{nullptr};  ///< `iso_milling_type_radio` del original.
    FCDoubleSpinner* iso_overlap_entry{nullptr};  ///< `iso_overlap_entry` del original.
    FCSpinner* iso_passes_entry{nullptr};  ///< `iso_passes_entry` del original.
    FCLabel* iso_type_label{nullptr};  ///< `iso_type_label` del original.
    RadioSet* iso_type_radio{nullptr};  ///< `iso_type_radio` del original.
    QVBoxLayout* iso_vlay{nullptr};  ///< `iso_vlay` del original.
    FCLabel* last_drill_drill_lbl{nullptr};  ///< `last_drill_drill_lbl` del original.
    FCLabel* max_limit_label{nullptr};  ///< `max_limit_label` del original.
    FCLabel* mb_dia_label{nullptr};  ///< `mb_dia_label` del original.
    FCLabel* mb_spacing_label{nullptr};  ///< `mb_spacing_label` del original.
    FCDoubleSpinner* mill_custom_offset_entry{nullptr};  ///< `mill_custom_offset_entry` del original.
    FCDoubleSpinner* mill_cutz_entry{nullptr};  ///< `mill_cutz_entry` del original.
    FCCheckBox* mill_dwell_cb{nullptr};  ///< `mill_dwell_cb` del original.
    FCDoubleSpinner* mill_dwelltime_entry{nullptr};  ///< `mill_dwelltime_entry` del original.
    FCCheckBox* mill_ecut_cb{nullptr};  ///< `mill_ecut_cb` del original.
    FCDoubleSpinner* mill_ecut_length_entry{nullptr};  ///< `mill_ecut_length_entry` del original.
    FCDoubleSpinner* mill_frapids_entry{nullptr};  ///< `mill_frapids_entry` del original.
    FCDoubleSpinner* mill_frxy_entry{nullptr};  ///< `mill_frxy_entry` del original.
    FCDoubleSpinner* mill_frz_entry{nullptr};  ///< `mill_frz_entry` del original.
    FCCheckBox* mill_multidepth_cb{nullptr};  ///< `mill_multidepth_cb` del original.
    FCDoubleSpinner* mill_multidepth_entry{nullptr};  ///< `mill_multidepth_entry` del original.
    FCComboBox* mill_shape_combo{nullptr};  ///< `mill_shape_combo` del original.
    FCDoubleSpinner* mill_spindle_entry{nullptr};  ///< `mill_spindle_entry` del original.
    FCComboBox* mill_tooloffset_combo{nullptr};  ///< `mill_tooloffset_combo` del original.
    FCDoubleSpinner* mill_travelz_entry{nullptr};  ///< `mill_travelz_entry` del original.
    FCComboBox* mill_type_combo{nullptr};  ///< `mill_type_combo` del original.
    FCDoubleSpinner* mill_vangle_entry{nullptr};  ///< `mill_vangle_entry` del original.
    FCDoubleSpinner* mill_vdia_entry{nullptr};  ///< `mill_vdia_entry` del original.
    QGroupBox* milling_box{nullptr};  ///< `milling_box` del original.
    FCLabel* milling_type_label{nullptr};  ///< `milling_type_label` del original.
    QVBoxLayout* milling_vlay{nullptr};  ///< `milling_vlay` del original.
    FCLabel* min_limit_label{nullptr};  ///< `min_limit_label` del original.
    FCLabel* multidepth_drill_label{nullptr};  ///< `multidepth_drill_label` del original.
    FCLabel* multidepth_label{nullptr};  ///< `multidepth_label` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    FCLabel* name_label{nullptr};  ///< `name_label` del original.
    QGroupBox* ncc_box{nullptr};  ///< `ncc_box` del original.
    FCCheckBox* ncc_choice_offset_cb{nullptr};  ///< `ncc_choice_offset_cb` del original.
    FCCheckBox* ncc_connect_cb{nullptr};  ///< `ncc_connect_cb` del original.
    FCCheckBox* ncc_contour_cb{nullptr};  ///< `ncc_contour_cb` del original.
    FCDoubleSpinner* ncc_margin_entry{nullptr};  ///< `ncc_margin_entry` del original.
    FCComboBox2* ncc_method_combo{nullptr};  ///< `ncc_method_combo` del original.
    RadioSet* ncc_milling_type_radio{nullptr};  ///< `ncc_milling_type_radio` del original.
    FCDoubleSpinner* ncc_offset_spinner{nullptr};  ///< `ncc_offset_spinner` del original.
    RadioSet* ncc_op_radio{nullptr};  ///< `ncc_op_radio` del original.
    FCDoubleSpinner* ncc_overlap_entry{nullptr};  ///< `ncc_overlap_entry` del original.
    QVBoxLayout* ncc_vlay{nullptr};  ///< `ncc_vlay` del original.
    QGroupBox* paint_box{nullptr};  ///< `paint_box` del original.
    FCCheckBox* paint_contour_cb{nullptr};  ///< `paint_contour_cb` del original.
    FCComboBox2* paint_method_combo{nullptr};  ///< `paint_method_combo` del original.
    FCDoubleSpinner* paint_offset_entry{nullptr};  ///< `paint_offset_entry` del original.
    FCDoubleSpinner* paint_overlap_entry{nullptr};  ///< `paint_overlap_entry` del original.
    QVBoxLayout* paint_vlay{nullptr};  ///< `paint_vlay` del original.
    FCCheckBox* path_connect_cb{nullptr};  ///< `path_connect_cb` del original.
    FCButton* remove_entry_btn{nullptr};  ///< `remove_entry_btn` del original.
    FCButton* save_db_btn{nullptr};  ///< `save_db_btn` del original.
    FCLabel* shape_label{nullptr};  ///< `shape_label` del original.
    FCLabel* spindle_label{nullptr};  ///< `spindle_label` del original.
    FCLabel* thin_depth_label{nullptr};  ///< `thin_depth_label` del original.
    FCLabel* tol_label{nullptr};  ///< `tol_label` del original.
    FCDoubleSpinner* tol_max_entry{nullptr};  ///< `tol_max_entry` del original.
    FCDoubleSpinner* tol_min_entry{nullptr};  ///< `tol_min_entry` del original.
    QGroupBox* tool_description_box{nullptr};  ///< `tool_description_box` del original.
    FCLabel* tool_offset_label{nullptr};  ///< `tool_offset_label` del original.
    FCComboBox2* tool_op_combo{nullptr};  ///< `tool_op_combo` del original.
    FCLabel* tool_op_label{nullptr};  ///< `tool_op_label` del original.
    FCLabel* tooloffset_label{nullptr};  ///< `tooloffset_label` del original.
    FCLabel* travelz_label{nullptr};  ///< `travelz_label` del original.
    FCLabel* travelzlabel{nullptr};  ///< `travelzlabel` del original.
    FCTree* tree_widget{nullptr};  ///< `tree_widget` del original.
    FCLabel* type_label{nullptr};  ///< `type_label` del original.
    FCLabel* vangle_label{nullptr};  ///< `vangle_label` del original.
    FCLabel* vdia_label{nullptr};  ///< `vdia_label` del original.
    ///@}
};

}  // namespace flatcam::gui
