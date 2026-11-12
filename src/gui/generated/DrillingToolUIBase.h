// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#pragma once

#include <map>
#include <string>
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
#include <QFontComboBox>
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
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `DrillingToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class DrillingToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit DrillingToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QPushButton* add_area_button{nullptr};  ///< `add_area_button` del original.
    FCButton* apply_param_to_all{nullptr};  ///< `apply_param_to_all` del original.
    RadioSet* area_shape_radio{nullptr};  ///< `area_shape_radio` del original.
    FCDoubleSpinner* cutz_entry{nullptr};  ///< `cutz_entry` del original.
    QLabel* cutzlabel{nullptr};  ///< `cutzlabel` del original.
    QPushButton* delete_area_button{nullptr};  ///< `delete_area_button` del original.
    QPushButton* delete_sel_area_button{nullptr};  ///< `delete_sel_area_button` del original.
    FCDoubleSpinner* drill_overlap_entry{nullptr};  ///< `drill_overlap_entry` del original.
    QLabel* drill_overlap_label{nullptr};  ///< `drill_overlap_label` del original.
    FCCheckBox* drill_slots_cb{nullptr};  ///< `drill_slots_cb` del original.
    FCCheckBox* dwell_cb{nullptr};  ///< `dwell_cb` del original.
    FCDoubleSpinner* dwelltime_entry{nullptr};  ///< `dwelltime_entry` del original.
    NumericalEvalTupleEntry* endxy_entry{nullptr};  ///< `endxy_entry` del original.
    FCDoubleSpinner* endz_entry{nullptr};  ///< `endz_entry` del original.
    QLabel* endz_label{nullptr};  ///< `endz_label` del original.
    NumericalEvalEntry* estartz_entry{nullptr};  ///< `estartz_entry` del original.
    QLabel* estartz_label{nullptr};  ///< `estartz_label` del original.
    QFrame* exc_param_frame{nullptr};  ///< `exc_param_frame` del original.
    QVBoxLayout* exc_tools_box{nullptr};  ///< `exc_tools_box` del original.
    QVBoxLayout* exclusion_box{nullptr};  ///< `exclusion_box` del original.
    FCCheckBox* exclusion_cb{nullptr};  ///< `exclusion_cb` del original.
    QFrame* exclusion_frame{nullptr};  ///< `exclusion_frame` del original.
    FCTable* exclusion_table{nullptr};  ///< `exclusion_table` del original.
    FCDoubleSpinner* feedrate_probe_entry{nullptr};  ///< `feedrate_probe_entry` del original.
    QLabel* feedrate_probe_label{nullptr};  ///< `feedrate_probe_label` del original.
    FCDoubleSpinner* feedrate_rapid_entry{nullptr};  ///< `feedrate_rapid_entry` del original.
    QLabel* feedrate_rapid_label{nullptr};  ///< `feedrate_rapid_label` del original.
    FCDoubleSpinner* feedrate_z_entry{nullptr};  ///< `feedrate_z_entry` del original.
    QLabel* frzlabel{nullptr};  ///< `frzlabel` del original.
    QLabel* gen_param_label{nullptr};  ///< `gen_param_label` del original.
    QPushButton* generate_cnc_button{nullptr};  ///< `generate_cnc_button` del original.
    QGridLayout* grid1{nullptr};  ///< `grid1` del original.
    QGridLayout* grid3{nullptr};  ///< `grid3` del original.
    QGridLayout* grid4{nullptr};  ///< `grid4` del original.
    FCCheckBox* last_drill_cb{nullptr};  ///< `last_drill_cb` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    QLabel* level{nullptr};  ///< `level` del original.
    FCDoubleSpinner* maxdepth_entry{nullptr};  ///< `maxdepth_entry` del original.
    FCCheckBox* mpass_cb{nullptr};  ///< `mpass_cb` del original.
    QLabel* obj_combo_label{nullptr};  ///< `obj_combo_label` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    FCDoubleSpinner* offset_entry{nullptr};  ///< `offset_entry` del original.
    QLabel* order_label{nullptr};  ///< `order_label` del original.
    RadioSet* order_radio{nullptr};  ///< `order_radio` del original.
    FCDoubleSpinner* over_z_entry{nullptr};  ///< `over_z_entry` del original.
    FCLabel* over_z_label{nullptr};  ///< `over_z_label` del original.
    FCDoubleSpinner* pdepth_entry{nullptr};  ///< `pdepth_entry` del original.
    QLabel* pdepth_label{nullptr};  ///< `pdepth_label` del original.
    FCComboBox* pp_excellon_name_cb{nullptr};  ///< `pp_excellon_name_cb` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCButton* search_load_db_btn{nullptr};  ///< `search_load_db_btn` del original.
    QLabel* spindle_label{nullptr};  ///< `spindle_label` del original.
    FCSpinner* spindlespeed_entry{nullptr};  ///< `spindlespeed_entry` del original.
    FCLabel* strategy_label{nullptr};  ///< `strategy_label` del original.
    RadioSet* strategy_radio{nullptr};  ///< `strategy_radio` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    QLabel* tool_data_label{nullptr};  ///< `tool_data_label` del original.
    QLabel* tool_offset_label{nullptr};  ///< `tool_offset_label` del original.
    FCCheckBox* toolchange_cb{nullptr};  ///< `toolchange_cb` del original.
    FCDoubleSpinner* toolchangez_entry{nullptr};  ///< `toolchangez_entry` del original.
    QLabel* toolchangez_label{nullptr};  ///< `toolchangez_label` del original.
    QVBoxLayout* tools_box{nullptr};  ///< `tools_box` del original.
    QFrame* tools_frame{nullptr};  ///< `tools_frame` del original.
    FCTable* tools_table{nullptr};  ///< `tools_table` del original.
    FCDoubleSpinner* travelz_entry{nullptr};  ///< `travelz_entry` del original.
    QLabel* travelzlabel{nullptr};  ///< `travelzlabel` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
