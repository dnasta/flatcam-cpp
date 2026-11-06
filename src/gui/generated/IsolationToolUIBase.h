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
 * @brief Panel `IsolationToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class IsolationToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit IsolationToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* addtool_from_db_btn{nullptr};  ///< `addtool_from_db_btn` del original.
    FCButton* apply_param_to_all{nullptr};  ///< `apply_param_to_all` del original.
    FCLabel* area_shape_label{nullptr};  ///< `area_shape_label` del original.
    RadioSet* area_shape_radio{nullptr};  ///< `area_shape_radio` del original.
    FCCheckBox* combine_passes_cb{nullptr};  ///< `combine_passes_cb` del original.
    FCButton* create_buffer_button{nullptr};  ///< `create_buffer_button` del original.
    FCButton* deltool_btn{nullptr};  ///< `deltool_btn` del original.
    FCComboBox* exc_obj_combo{nullptr};  ///< `exc_obj_combo` del original.
    FCCheckBox* except_cb{nullptr};  ///< `except_cb` del original.
    QToolButton* find_optimal_button{nullptr};  ///< `find_optimal_button` del original.
    FCCheckBox* follow_cb{nullptr};  ///< `follow_cb` del original.
    FCLabel* follow_label{nullptr};  ///< `follow_label` del original.
    FCCheckBox* forced_rest_iso_cb{nullptr};  ///< `forced_rest_iso_cb` del original.
    FCLabel* gen_param_label{nullptr};  ///< `gen_param_label` del original.
    FCButton* generate_iso_button{nullptr};  ///< `generate_iso_button` del original.
    QGridLayout* grid3{nullptr};  ///< `grid3` del original.
    FCDoubleSpinner* iso_overlap_entry{nullptr};  ///< `iso_overlap_entry` del original.
    FCLabel* iso_type_label{nullptr};  ///< `iso_type_label` del original.
    RadioSet* iso_type_radio{nullptr};  ///< `iso_type_radio` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCLabel* level{nullptr};  ///< `level` del original.
    FCLabel* milling_type_label{nullptr};  ///< `milling_type_label` del original.
    RadioSet* milling_type_radio{nullptr};  ///< `milling_type_radio` del original.
    FCDoubleSpinner* new_tooldia_entry{nullptr};  ///< `new_tooldia_entry` del original.
    FCLabel* new_tooldia_lbl{nullptr};  ///< `new_tooldia_lbl` del original.
    FCLabel* obj_combo_label{nullptr};  ///< `obj_combo_label` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    FCLabel* order_label{nullptr};  ///< `order_label` del original.
    RadioSet* order_radio{nullptr};  ///< `order_radio` del original.
    FCSpinner* passes_entry{nullptr};  ///< `passes_entry` del original.
    FCCheckBox* poly_int_cb{nullptr};  ///< `poly_int_cb` del original.
    FCComboBox* reference_combo{nullptr};  ///< `reference_combo` del original.
    FCComboBox2* reference_combo_type{nullptr};  ///< `reference_combo_type` del original.
    FCLabel* reference_combo_type_label{nullptr};  ///< `reference_combo_type_label` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCCheckBox* rest_cb{nullptr};  ///< `rest_cb` del original.
    FCButton* search_and_add_btn{nullptr};  ///< `search_and_add_btn` del original.
    FCComboBox2* select_combo{nullptr};  ///< `select_combo` del original.
    FCLabel* select_label{nullptr};  ///< `select_label` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* tool_data_label{nullptr};  ///< `tool_data_label` del original.
    FCLabel* tool_sel_label{nullptr};  ///< `tool_sel_label` del original.
    QVBoxLayout* tools_box{nullptr};  ///< `tools_box` del original.
    QFrame* tools_frame{nullptr};  ///< `tools_frame` del original.
    FCTable* tools_table{nullptr};  ///< `tools_table` del original.
    FCLabel* tools_table_label{nullptr};  ///< `tools_table_label` del original.
    RadioSet* type_excobj_radio{nullptr};  ///< `type_excobj_radio` del original.
    FCCheckBox* valid_cb{nullptr};  ///< `valid_cb` del original.
    ///@}
};

}  // namespace flatcam::gui
