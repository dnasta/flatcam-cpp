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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"

namespace flatcam::gui {

/**
 * @brief Panel `SolderPasteToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class SolderPasteToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit SolderPasteToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QPushButton* addtool_btn{nullptr};  ///< `addtool_btn` del original.
    FCDoubleSpinner* addtool_entry{nullptr};  ///< `addtool_entry` del original.
    QLabel* addtool_entry_lbl{nullptr};  ///< `addtool_entry_lbl` del original.
    FCComboBox* cnc_obj_combo{nullptr};  ///< `cnc_obj_combo` del original.
    QLabel* cnc_object_label{nullptr};  ///< `cnc_object_label` del original.
    QPushButton* deltool_btn{nullptr};  ///< `deltool_btn` del original.
    FCDoubleSpinner* dwellfwd_entry{nullptr};  ///< `dwellfwd_entry` del original.
    QLabel* dwellfwd_label{nullptr};  ///< `dwellfwd_label` del original.
    FCDoubleSpinner* dwellrev_entry{nullptr};  ///< `dwellrev_entry` del original.
    QLabel* dwellrev_label{nullptr};  ///< `dwellrev_label` del original.
    FCDoubleSpinner* frxy_entry{nullptr};  ///< `frxy_entry` del original.
    QLabel* frxy_label{nullptr};  ///< `frxy_label` del original.
    FCDoubleSpinner* frz_dispense_entry{nullptr};  ///< `frz_dispense_entry` del original.
    QLabel* frz_dispense_label{nullptr};  ///< `frz_dispense_label` del original.
    FCDoubleSpinner* frz_entry{nullptr};  ///< `frz_entry` del original.
    QLabel* frz_label{nullptr};  ///< `frz_label` del original.
    QVBoxLayout* gcode_box{nullptr};  ///< `gcode_box` del original.
    QFormLayout* gcode_form_layout{nullptr};  ///< `gcode_form_layout` del original.
    QFrame* gcode_frame{nullptr};  ///< `gcode_frame` del original.
    QVBoxLayout* generation_box{nullptr};  ///< `generation_box` del original.
    QFrame* generation_frame{nullptr};  ///< `generation_frame` del original.
    FCComboBox* geo_obj_combo{nullptr};  ///< `geo_obj_combo` del original.
    QLabel* geo_object_label{nullptr};  ///< `geo_object_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCComboBox* obj_combo{nullptr};  ///< `obj_combo` del original.
    QLabel* object_label{nullptr};  ///< `object_label` del original.
    FCComboBox* pp_combo{nullptr};  ///< `pp_combo` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    QPushButton* solder_gcode_btn{nullptr};  ///< `solder_gcode_btn` del original.
    QPushButton* solder_gcode_save_btn{nullptr};  ///< `solder_gcode_save_btn` del original.
    QPushButton* solder_gcode_view_btn{nullptr};  ///< `solder_gcode_view_btn` del original.
    QPushButton* soldergeo_btn{nullptr};  ///< `soldergeo_btn` del original.
    FCSpinner* speedfwd_entry{nullptr};  ///< `speedfwd_entry` del original.
    QLabel* speedfwd_label{nullptr};  ///< `speedfwd_label` del original.
    FCSpinner* speedrev_entry{nullptr};  ///< `speedrev_entry` del original.
    QLabel* speedrev_label{nullptr};  ///< `speedrev_label` del original.
    FCTable* tools_table{nullptr};  ///< `tools_table` del original.
    QLabel* tools_table_label{nullptr};  ///< `tools_table_label` del original.
    FCEntry* xy_toolchange_entry{nullptr};  ///< `xy_toolchange_entry` del original.
    QLabel* xy_toolchange_label{nullptr};  ///< `xy_toolchange_label` del original.
    FCDoubleSpinner* z_dispense_entry{nullptr};  ///< `z_dispense_entry` del original.
    QLabel* z_dispense_label{nullptr};  ///< `z_dispense_label` del original.
    FCDoubleSpinner* z_start_entry{nullptr};  ///< `z_start_entry` del original.
    QLabel* z_start_label{nullptr};  ///< `z_start_label` del original.
    FCDoubleSpinner* z_stop_entry{nullptr};  ///< `z_stop_entry` del original.
    QLabel* z_stop_label{nullptr};  ///< `z_stop_label` del original.
    FCDoubleSpinner* z_toolchange_entry{nullptr};  ///< `z_toolchange_entry` del original.
    QLabel* z_toolchange_label{nullptr};  ///< `z_toolchange_label` del original.
    FCDoubleSpinner* z_travel_entry{nullptr};  ///< `z_travel_entry` del original.
    QLabel* z_travel_label{nullptr};  ///< `z_travel_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
