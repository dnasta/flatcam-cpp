// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#pragma once

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
#include "gui/widgets/FCTable.h"
#include "gui/widgets/FCTree.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `GeometryObjectUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class GeometryObjectUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit GeometryObjectUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_area_button{nullptr};  ///< `add_area_button` del original.
    FCDoubleSpinner* addtool_entry{nullptr};  ///< `addtool_entry` del original.
    FCLabel* addtool_entry_lbl{nullptr};  ///< `addtool_entry_lbl` del original.
    FCButton* addtool_from_db_btn{nullptr};  ///< `addtool_from_db_btn` del original.
    FCButton* apply_param_to_all{nullptr};  ///< `apply_param_to_all` del original.
    RadioSet* area_shape_radio{nullptr};  ///< `area_shape_radio` del original.
    FCDoubleSpinner* cncfeedrate_entry{nullptr};  ///< `cncfeedrate_entry` del original.
    FCSpinner* cncspindlespeed_entry{nullptr};  ///< `cncspindlespeed_entry` del original.
    QGridLayout* common_grid{nullptr};  ///< `common_grid` del original.
    QVBoxLayout* custom_box{nullptr};  ///< `custom_box` del original.
    FCDoubleSpinner* cutz_entry{nullptr};  ///< `cutz_entry` del original.
    FCLabel* cutzlabel{nullptr};  ///< `cutzlabel` del original.
    FCButton* delete_area_button{nullptr};  ///< `delete_area_button` del original.
    FCButton* delete_sel_area_button{nullptr};  ///< `delete_sel_area_button` del original.
    FCButton* deltool_btn{nullptr};  ///< `deltool_btn` del original.
    FCCheckBox* dwell_cb{nullptr};  ///< `dwell_cb` del original.
    FCDoubleSpinner* dwelltime_entry{nullptr};  ///< `dwelltime_entry` del original.
    FCDoubleSpinner* e_cut_entry{nullptr};  ///< `e_cut_entry` del original.
    FCButton* editor_button{nullptr};  ///< `editor_button` del original.
    NumericalEvalTupleEntry* endxy_entry{nullptr};  ///< `endxy_entry` del original.
    FCDoubleSpinner* endz_entry{nullptr};  ///< `endz_entry` del original.
    FCLabel* endz_label{nullptr};  ///< `endz_label` del original.
    QVBoxLayout* exclusion_box{nullptr};  ///< `exclusion_box` del original.
    FCCheckBox* exclusion_cb{nullptr};  ///< `exclusion_cb` del original.
    QFrame* exclusion_frame{nullptr};  ///< `exclusion_frame` del original.
    FCTable* exclusion_table{nullptr};  ///< `exclusion_table` del original.
    FCCheckBox* extracut_cb{nullptr};  ///< `extracut_cb` del original.
    FCDoubleSpinner* feedrate_probe_entry{nullptr};  ///< `feedrate_probe_entry` del original.
    FCLabel* feedrate_probe_label{nullptr};  ///< `feedrate_probe_label` del original.
    FCDoubleSpinner* feedrate_rapid_entry{nullptr};  ///< `feedrate_rapid_entry` del original.
    FCDoubleSpinner* feedrate_z_entry{nullptr};  ///< `feedrate_z_entry` del original.
    FCLabel* fr_rapidlabel{nullptr};  ///< `fr_rapidlabel` del original.
    FCLabel* frlabel{nullptr};  ///< `frlabel` del original.
    FCLabel* frzlabel{nullptr};  ///< `frzlabel` del original.
    FCLabel* gen_param_label{nullptr};  ///< `gen_param_label` del original.
    FCButton* generate_cnc_button{nullptr};  ///< `generate_cnc_button` del original.
    FCButton* generate_ncc_button{nullptr};  ///< `generate_ncc_button` del original.
    QVBoxLayout* geo_param_box{nullptr};  ///< `geo_param_box` del original.
    QFrame* geo_param_frame{nullptr};  ///< `geo_param_frame` del original.
    QVBoxLayout* geo_table_box{nullptr};  ///< `geo_table_box` del original.
    QFrame* geo_table_frame{nullptr};  ///< `geo_table_frame` del original.
    QVBoxLayout* geo_tools_box{nullptr};  ///< `geo_tools_box` del original.
    QFrame* geo_tools_frame{nullptr};  ///< `geo_tools_frame` del original.
    FCTable* geo_tools_table{nullptr};  ///< `geo_tools_table` del original.
    QGridLayout* grid3{nullptr};  ///< `grid3` del original.
    QGridLayout* grid4{nullptr};  ///< `grid4` del original.
    FCLabel* icon{nullptr};  ///< `icon` del original.
    FCLabel* level{nullptr};  ///< `level` del original.
    FCDoubleSpinner* maxdepth_entry{nullptr};  ///< `maxdepth_entry` del original.
    FCButton* milling_button{nullptr};  ///< `milling_button` del original.
    FCCheckBox* mpass_cb{nullptr};  ///< `mpass_cb` del original.
    FCCheckBox* multicolored_cb{nullptr};  ///< `multicolored_cb` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    QHBoxLayout* name_hlay{nullptr};  ///< `name_hlay` del original.
    FCButton* offset_button{nullptr};  ///< `offset_button` del original.
    NumericalEvalTupleEntry* offsetvector_entry{nullptr};  ///< `offsetvector_entry` del original.
    FCDoubleSpinner* over_z_entry{nullptr};  ///< `over_z_entry` del original.
    FCLabel* over_z_label{nullptr};  ///< `over_z_label` del original.
    FCButton* paint_tool_button{nullptr};  ///< `paint_tool_button` del original.
    FCDoubleSpinner* pdepth_entry{nullptr};  ///< `pdepth_entry` del original.
    FCLabel* pdepth_label{nullptr};  ///< `pdepth_label` del original.
    FCCheckBox* plot_cb{nullptr};  ///< `plot_cb` del original.
    FCLabel* plot_options_label{nullptr};  ///< `plot_options_label` del original.
    FCCheckBox* polish_cb{nullptr};  ///< `polish_cb` del original.
    FCDoubleSpinner* polish_dia_entry{nullptr};  ///< `polish_dia_entry` del original.
    FCLabel* polish_dia_lbl{nullptr};  ///< `polish_dia_lbl` del original.
    FCDoubleSpinner* polish_margin_entry{nullptr};  ///< `polish_margin_entry` del original.
    FCLabel* polish_margin_lbl{nullptr};  ///< `polish_margin_lbl` del original.
    FCComboBox2* polish_method_combo{nullptr};  ///< `polish_method_combo` del original.
    FCLabel* polish_method_lbl{nullptr};  ///< `polish_method_lbl` del original.
    FCDoubleSpinner* polish_over_entry{nullptr};  ///< `polish_over_entry` del original.
    FCLabel* polish_over_lbl{nullptr};  ///< `polish_over_lbl` del original.
    FCDoubleSpinner* polish_pressure_entry{nullptr};  ///< `polish_pressure_entry` del original.
    FCLabel* polish_pressure_lbl{nullptr};  ///< `polish_pressure_lbl` del original.
    FCDoubleSpinner* polish_travelz_entry{nullptr};  ///< `polish_travelz_entry` del original.
    FCLabel* polish_travelz_lbl{nullptr};  ///< `polish_travelz_lbl` del original.
    FCComboBox* pp_geometry_name_cb{nullptr};  ///< `pp_geometry_name_cb` del original.
    QVBoxLayout* properties_box{nullptr};  ///< `properties_box` del original.
    FCButton* properties_button{nullptr};  ///< `properties_button` del original.
    QFrame* properties_frame{nullptr};  ///< `properties_frame` del original.
    FCButton* scale_button{nullptr};  ///< `scale_button` del original.
    NumericalEvalEntry* scale_entry{nullptr};  ///< `scale_entry` del original.
    FCButton* search_and_add_btn{nullptr};  ///< `search_and_add_btn` del original.
    FCLabel* spindle_label{nullptr};  ///< `spindle_label` del original.
    FCLabel* strategy_label{nullptr};  ///< `strategy_label` del original.
    RadioSet* strategy_radio{nullptr};  ///< `strategy_radio` del original.
    FCDoubleSpinner* tipangle_entry{nullptr};  ///< `tipangle_entry` del original.
    FCLabel* tipanglelabel{nullptr};  ///< `tipanglelabel` del original.
    FCDoubleSpinner* tipdia_entry{nullptr};  ///< `tipdia_entry` del original.
    FCLabel* tipdialabel{nullptr};  ///< `tipdialabel` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* title_label{nullptr};  ///< `title_label` del original.
    FCLabel* tool_data_label{nullptr};  ///< `tool_data_label` del original.
    FCDoubleSpinner* tool_offset_entry{nullptr};  ///< `tool_offset_entry` del original.
    FCLabel* tool_offset_lbl{nullptr};  ///< `tool_offset_lbl` del original.
    FCLabel* tool_sel_label{nullptr};  ///< `tool_sel_label` del original.
    FCCheckBox* toolchangeg_cb{nullptr};  ///< `toolchangeg_cb` del original.
    FCDoubleSpinner* toolchangez_entry{nullptr};  ///< `toolchangez_entry` del original.
    FCLabel* tools_label{nullptr};  ///< `tools_label` del original.
    FCLabel* tools_table_label{nullptr};  ///< `tools_table_label` del original.
    FCLabel* transform_label{nullptr};  ///< `transform_label` del original.
    FCButton* transformations_button{nullptr};  ///< `transformations_button` del original.
    FCDoubleSpinner* travelz_entry{nullptr};  ///< `travelz_entry` del original.
    FCLabel* travelzlabel{nullptr};  ///< `travelzlabel` del original.
    FCTree* treeWidget{nullptr};  ///< `treeWidget` del original.
    ///@}
};

}  // namespace flatcam::gui
