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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `GrbEditorUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class GrbEditorUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit GrbEditorUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* addaperture_btn{nullptr};  ///< `addaperture_btn` del original.
    FCSpinner* apcode_entry{nullptr};  ///< `apcode_entry` del original.
    QLineEdit* apdim_entry{nullptr};  ///< `apdim_entry` del original.
    FCLabel* apdim_lbl{nullptr};  ///< `apdim_lbl` del original.
    QVBoxLayout* apertures_box{nullptr};  ///< `apertures_box` del original.
    QFrame* apertures_frame{nullptr};  ///< `apertures_frame` del original.
    FCTable* apertures_table{nullptr};  ///< `apertures_table` del original.
    FCLabel* apertures_table_label{nullptr};  ///< `apertures_table_label` del original.
    FCDoubleSpinner* apsize_entry{nullptr};  ///< `apsize_entry` del original.
    FCComboBox* aptype_cb{nullptr};  ///< `aptype_cb` del original.
    QVBoxLayout* array_box{nullptr};  ///< `array_box` del original.
    QFrame* array_frame{nullptr};  ///< `array_frame` del original.
    RadioSet* array_type_radio{nullptr};  ///< `array_type_radio` del original.
    FCButton* buffer_button{nullptr};  ///< `buffer_button` del original.
    FCComboBox* buffer_corner_cb{nullptr};  ///< `buffer_corner_cb` del original.
    FCLabel* buffer_corner_lbl{nullptr};  ///< `buffer_corner_lbl` del original.
    FCDoubleSpinner* buffer_distance_entry{nullptr};  ///< `buffer_distance_entry` del original.
    QFrame* buffer_tool_frame{nullptr};  ///< `buffer_tool_frame` del original.
    QVBoxLayout* buffer_tools_box{nullptr};  ///< `buffer_tools_box` del original.
    QFrame* circ_separator_line{nullptr};  ///< `circ_separator_line` del original.
    QVBoxLayout* custom_box{nullptr};  ///< `custom_box` del original.
    FCButton* delaperture_btn{nullptr};  ///< `delaperture_btn` del original.
    FCButton* exit_editor_button{nullptr};  ///< `exit_editor_button` del original.
    FCLabel* icon{nullptr};  ///< `icon` del original.
    QFrame* lin_separator_line{nullptr};  ///< `lin_separator_line` del original.
    FCLabel* linear_angle_label{nullptr};  ///< `linear_angle_label` del original.
    FCDoubleSpinner* linear_angle_spinner{nullptr};  ///< `linear_angle_spinner` del original.
    FCButton* ma_clear_button{nullptr};  ///< `ma_clear_button` del original.
    FCButton* ma_delete_button{nullptr};  ///< `ma_delete_button` del original.
    FCDoubleSpinner* ma_lower_threshold_entry{nullptr};  ///< `ma_lower_threshold_entry` del original.
    FCLabel* ma_lower_threshold_lbl{nullptr};  ///< `ma_lower_threshold_lbl` del original.
    FCButton* ma_threshold_button{nullptr};  ///< `ma_threshold_button` del original.
    QFrame* ma_tool_frame{nullptr};  ///< `ma_tool_frame` del original.
    QVBoxLayout* ma_tools_box{nullptr};  ///< `ma_tools_box` del original.
    FCDoubleSpinner* ma_upper_threshold_entry{nullptr};  ///< `ma_upper_threshold_entry` del original.
    FCLabel* ma_upper_threshold_lbl{nullptr};  ///< `ma_upper_threshold_lbl` del original.
    QHBoxLayout* name_box{nullptr};  ///< `name_box` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    FCDoubleSpinner* pad_angle_entry{nullptr};  ///< `pad_angle_entry` del original.
    FCLabel* pad_angle_label{nullptr};  ///< `pad_angle_label` del original.
    FCSpinner* pad_array_size_entry{nullptr};  ///< `pad_array_size_entry` del original.
    FCLabel* pad_axis_label{nullptr};  ///< `pad_axis_label` del original.
    RadioSet* pad_axis_radio{nullptr};  ///< `pad_axis_radio` del original.
    FCLabel* pad_direction_label{nullptr};  ///< `pad_direction_label` del original.
    RadioSet* pad_direction_radio{nullptr};  ///< `pad_direction_radio` del original.
    FCDoubleSpinner* pad_pitch_entry{nullptr};  ///< `pad_pitch_entry` del original.
    FCLabel* pad_pitch_label{nullptr};  ///< `pad_pitch_label` del original.
    FCLabel* padarray_label{nullptr};  ///< `padarray_label` del original.
    FCButton* scale_button{nullptr};  ///< `scale_button` del original.
    FCDoubleSpinner* scale_factor_entry{nullptr};  ///< `scale_factor_entry` del original.
    FCLabel* scale_factor_lbl{nullptr};  ///< `scale_factor_lbl` del original.
    QFrame* scale_tool_frame{nullptr};  ///< `scale_tool_frame` del original.
    QVBoxLayout* scale_tools_box{nullptr};  ///< `scale_tools_box` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* title_label{nullptr};  ///< `title_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
