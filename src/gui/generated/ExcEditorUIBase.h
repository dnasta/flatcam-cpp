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
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `ExcEditorUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class ExcEditorUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit ExcEditorUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* addtool_btn{nullptr};  ///< `addtool_btn` del original.
    FCDoubleSpinner* addtool_entry{nullptr};  ///< `addtool_entry` del original.
    FCLabel* addtool_label{nullptr};  ///< `addtool_label` del original.
    QFrame* array_circular_frame{nullptr};  ///< `array_circular_frame` del original.
    QFrame* array_frame{nullptr};  ///< `array_frame` del original.
    QGridLayout* array_grid{nullptr};  ///< `array_grid` del original.
    QFrame* array_linear_frame{nullptr};  ///< `array_linear_frame` del original.
    RadioSet* array_type_radio{nullptr};  ///< `array_type_radio` del original.
    QGridLayout* circ_grid{nullptr};  ///< `circ_grid` del original.
    FCButton* convert_slots_btn{nullptr};  ///< `convert_slots_btn` del original.
    FCButton* deltool_btn{nullptr};  ///< `deltool_btn` del original.
    FCDoubleSpinner* drill_angle_entry{nullptr};  ///< `drill_angle_entry` del original.
    FCLabel* drill_array_angle_lbl{nullptr};  ///< `drill_array_angle_lbl` del original.
    FCLabel* drill_array_dir_lbl{nullptr};  ///< `drill_array_dir_lbl` del original.
    RadioSet* drill_array_dir_radio{nullptr};  ///< `drill_array_dir_radio` del original.
    FCLabel* drill_array_label{nullptr};  ///< `drill_array_label` del original.
    FCSpinner* drill_array_size_entry{nullptr};  ///< `drill_array_size_entry` del original.
    FCLabel* drill_array_size_label{nullptr};  ///< `drill_array_size_label` del original.
    FCLabel* drill_axis_label{nullptr};  ///< `drill_axis_label` del original.
    RadioSet* drill_axis_radio{nullptr};  ///< `drill_axis_radio` del original.
    FCDoubleSpinner* drill_pitch_entry{nullptr};  ///< `drill_pitch_entry` del original.
    FCLabel* drill_pitch_label{nullptr};  ///< `drill_pitch_label` del original.
    FCLabel* drillresize_label{nullptr};  ///< `drillresize_label` del original.
    QFrame* drills_frame{nullptr};  ///< `drills_frame` del original.
    FCButton* exit_editor_button{nullptr};  ///< `exit_editor_button` del original.
    FCLabel* icon{nullptr};  ///< `icon` del original.
    QGridLayout* lin_grid{nullptr};  ///< `lin_grid` del original.
    FCLabel* linear_angle_label{nullptr};  ///< `linear_angle_label` del original.
    FCDoubleSpinner* linear_angle_spinner{nullptr};  ///< `linear_angle_spinner` del original.
    QHBoxLayout* name_box{nullptr};  ///< `name_box` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    FCDoubleSpinner* resdrill_entry{nullptr};  ///< `resdrill_entry` del original.
    FCButton* resize_btn{nullptr};  ///< `resize_btn` del original.
    QFrame* resize_frame{nullptr};  ///< `resize_frame` del original.
    QGridLayout* resize_grid{nullptr};  ///< `resize_grid` del original.
    FCLabel* slot_angle_label{nullptr};  ///< `slot_angle_label` del original.
    FCDoubleSpinner* slot_angle_spinner{nullptr};  ///< `slot_angle_spinner` del original.
    FCDoubleSpinner* slot_array_angle_entry{nullptr};  ///< `slot_array_angle_entry` del original.
    FCLabel* slot_array_angle_label{nullptr};  ///< `slot_array_angle_label` del original.
    FCLabel* slot_array_axis_label{nullptr};  ///< `slot_array_axis_label` del original.
    RadioSet* slot_array_axis_radio{nullptr};  ///< `slot_array_axis_radio` del original.
    QGridLayout* slot_array_circ_grid{nullptr};  ///< `slot_array_circ_grid` del original.
    QFrame* slot_array_circular_frame{nullptr};  ///< `slot_array_circular_frame` del original.
    FCLabel* slot_array_direction_label{nullptr};  ///< `slot_array_direction_label` del original.
    RadioSet* slot_array_direction_radio{nullptr};  ///< `slot_array_direction_radio` del original.
    QFrame* slot_array_frame{nullptr};  ///< `slot_array_frame` del original.
    QGridLayout* slot_array_grid{nullptr};  ///< `slot_array_grid` del original.
    FCLabel* slot_array_label{nullptr};  ///< `slot_array_label` del original.
    QGridLayout* slot_array_lin_grid{nullptr};  ///< `slot_array_lin_grid` del original.
    FCLabel* slot_array_linear_angle_label{nullptr};  ///< `slot_array_linear_angle_label` del original.
    FCDoubleSpinner* slot_array_linear_angle_spinner{nullptr};  ///< `slot_array_linear_angle_spinner` del original.
    QFrame* slot_array_linear_frame{nullptr};  ///< `slot_array_linear_frame` del original.
    FCDoubleSpinner* slot_array_pitch_entry{nullptr};  ///< `slot_array_pitch_entry` del original.
    FCLabel* slot_array_pitch_label{nullptr};  ///< `slot_array_pitch_label` del original.
    FCSpinner* slot_array_size_entry{nullptr};  ///< `slot_array_size_entry` del original.
    FCLabel* slot_array_size_label{nullptr};  ///< `slot_array_size_label` del original.
    RadioSet* slot_array_type_radio{nullptr};  ///< `slot_array_type_radio` del original.
    FCLabel* slot_axis_label{nullptr};  ///< `slot_axis_label` del original.
    RadioSet* slot_axis_radio{nullptr};  ///< `slot_axis_radio` del original.
    QFrame* slot_frame{nullptr};  ///< `slot_frame` del original.
    QGridLayout* slot_grid{nullptr};  ///< `slot_grid` del original.
    FCLabel* slot_label{nullptr};  ///< `slot_label` del original.
    FCDoubleSpinner* slot_length_entry{nullptr};  ///< `slot_length_entry` del original.
    FCLabel* slot_length_label{nullptr};  ///< `slot_length_label` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* title_label{nullptr};  ///< `title_label` del original.
    FCTable* tools_table_exc{nullptr};  ///< `tools_table_exc` del original.
    FCLabel* tools_table_label{nullptr};  ///< `tools_table_label` del original.
    QVBoxLayout* ui_vertical_lay{nullptr};  ///< `ui_vertical_lay` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
