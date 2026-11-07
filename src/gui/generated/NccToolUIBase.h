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
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `NccToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class NccToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit NccToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_newtool_button{nullptr};  ///< `add_newtool_button` del original.
    FCButton* addtool_from_db_btn{nullptr};  ///< `addtool_from_db_btn` del original.
    FCButton* apply_param_to_all{nullptr};  ///< `apply_param_to_all` del original.
    FCLabel* area_shape_label{nullptr};  ///< `area_shape_label` del original.
    RadioSet* area_shape_radio{nullptr};  ///< `area_shape_radio` del original.
    FCButton* deltool_btn{nullptr};  ///< `deltool_btn` del original.
    QToolButton* find_optimal_button{nullptr};  ///< `find_optimal_button` del original.
    FCLabel* gen_param_label{nullptr};  ///< `gen_param_label` del original.
    FCButton* generate_ncc_button{nullptr};  ///< `generate_ncc_button` del original.
    QGridLayout* grid3{nullptr};  ///< `grid3` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCLabel* methodlabel{nullptr};  ///< `methodlabel` del original.
    FCLabel* milling_type_label{nullptr};  ///< `milling_type_label` del original.
    RadioSet* milling_type_radio{nullptr};  ///< `milling_type_radio` del original.
    FCCheckBox* ncc_choice_offset_cb{nullptr};  ///< `ncc_choice_offset_cb` del original.
    FCCheckBox* ncc_connect_cb{nullptr};  ///< `ncc_connect_cb` del original.
    FCCheckBox* ncc_contour_cb{nullptr};  ///< `ncc_contour_cb` del original.
    FCDoubleSpinner* ncc_margin_entry{nullptr};  ///< `ncc_margin_entry` del original.
    FCComboBox2* ncc_method_combo{nullptr};  ///< `ncc_method_combo` del original.
    FCDoubleSpinner* ncc_offset_spinner{nullptr};  ///< `ncc_offset_spinner` del original.
    FCLabel* ncc_order_label{nullptr};  ///< `ncc_order_label` del original.
    RadioSet* ncc_order_radio{nullptr};  ///< `ncc_order_radio` del original.
    FCDoubleSpinner* ncc_overlap_entry{nullptr};  ///< `ncc_overlap_entry` del original.
    FCCheckBox* ncc_rest_cb{nullptr};  ///< `ncc_rest_cb` del original.
    FCLabel* nccmarginlabel{nullptr};  ///< `nccmarginlabel` del original.
    FCLabel* nccoverlabel{nullptr};  ///< `nccoverlabel` del original.
    FCDoubleSpinner* new_tooldia_entry{nullptr};  ///< `new_tooldia_entry` del original.
    FCLabel* new_tooldia_lbl{nullptr};  ///< `new_tooldia_lbl` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    FCLabel* object_label{nullptr};  ///< `object_label` del original.
    RadioSet* op_radio{nullptr};  ///< `op_radio` del original.
    FCComboBox* reference_combo{nullptr};  ///< `reference_combo` del original.
    FCComboBox2* reference_combo_type{nullptr};  ///< `reference_combo_type` del original.
    FCLabel* reference_combo_type_label{nullptr};  ///< `reference_combo_type_label` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCCheckBox* rest_ncc_choice_offset_cb{nullptr};  ///< `rest_ncc_choice_offset_cb` del original.
    FCCheckBox* rest_ncc_connect_cb{nullptr};  ///< `rest_ncc_connect_cb` del original.
    FCCheckBox* rest_ncc_contour_cb{nullptr};  ///< `rest_ncc_contour_cb` del original.
    FCDoubleSpinner* rest_ncc_margin_entry{nullptr};  ///< `rest_ncc_margin_entry` del original.
    FCDoubleSpinner* rest_ncc_offset_spinner{nullptr};  ///< `rest_ncc_offset_spinner` del original.
    FCLabel* rest_nccmarginlabel{nullptr};  ///< `rest_nccmarginlabel` del original.
    FCComboBox2* select_combo{nullptr};  ///< `select_combo` del original.
    FCLabel* select_label{nullptr};  ///< `select_label` del original.
    FCLabel* tool_data_label{nullptr};  ///< `tool_data_label` del original.
    FCLabel* tool_sel_label{nullptr};  ///< `tool_sel_label` del original.
    QVBoxLayout* tools_box{nullptr};  ///< `tools_box` del original.
    QFrame* tools_frame{nullptr};  ///< `tools_frame` del original.
    FCTable* tools_table{nullptr};  ///< `tools_table` del original.
    FCLabel* tools_table_label{nullptr};  ///< `tools_table_label` del original.
    FCLabel* type_obj_combo_label{nullptr};  ///< `type_obj_combo_label` del original.
    RadioSet* type_obj_radio{nullptr};  ///< `type_obj_radio` del original.
    FCCheckBox* valid_cb{nullptr};  ///< `valid_cb` del original.
    ///@}
};

}  // namespace flatcam::gui
