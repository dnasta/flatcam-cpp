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
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `CalibrationToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class CalibrationToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit CalibrationToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QPushButton* adj_gcode_button{nullptr};  ///< `adj_gcode_button` del original.
    FCComboBox* adj_object_combo{nullptr};  ///< `adj_object_combo` del original.
    QLabel* adj_object_label{nullptr};  ///< `adj_object_label` del original.
    FCComboBox* adj_object_type_combo{nullptr};  ///< `adj_object_type_combo` del original.
    QLabel* adj_object_type_label{nullptr};  ///< `adj_object_type_label` del original.
    QPushButton* cal_button{nullptr};  ///< `cal_button` del original.
    QLabel* cal_source_lbl{nullptr};  ///< `cal_source_lbl` del original.
    RadioSet* cal_source_radio{nullptr};  ///< `cal_source_radio` del original.
    QPushButton* gcode_button{nullptr};  ///< `gcode_button` del original.
    QLabel* gcode_title_label{nullptr};  ///< `gcode_title_label` del original.
    QPushButton* generate_factors_button{nullptr};  ///< `generate_factors_button` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCComboBox* obj_type_combo{nullptr};  ///< `obj_type_combo` del original.
    QLabel* obj_type_label{nullptr};  ///< `obj_type_label` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    QLabel* object_label{nullptr};  ///< `object_label` del original.
    FCTable* points_table{nullptr};  ///< `points_table` del original.
    QLabel* points_table_label{nullptr};  ///< `points_table_label` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    QPushButton* scale_button{nullptr};  ///< `scale_button` del original.
    FCDoubleSpinner* scalex_entry{nullptr};  ///< `scalex_entry` del original.
    QLabel* scalex_label{nullptr};  ///< `scalex_label` del original.
    FCDoubleSpinner* scaley_entry{nullptr};  ///< `scaley_entry` del original.
    QLabel* scaley_label{nullptr};  ///< `scaley_label` del original.
    RadioSet* second_point_radio{nullptr};  ///< `second_point_radio` del original.
    QPushButton* skew_button{nullptr};  ///< `skew_button` del original.
    FCDoubleSpinner* skewx_entry{nullptr};  ///< `skewx_entry` del original.
    QLabel* skewx_label{nullptr};  ///< `skewx_label` del original.
    FCDoubleSpinner* skewy_entry{nullptr};  ///< `skewy_entry` del original.
    QLabel* skewy_label{nullptr};  ///< `skewy_label` del original.
    QPushButton* start_button{nullptr};  ///< `start_button` del original.
    FCEntry* toolchange_xy_entry{nullptr};  ///< `toolchange_xy_entry` del original.
    FCDoubleSpinner* toolchangez_entry{nullptr};  ///< `toolchangez_entry` del original.
    FCDoubleSpinner* travelz_entry{nullptr};  ///< `travelz_entry` del original.
    FCDoubleSpinner* verz_entry{nullptr};  ///< `verz_entry` del original.
    FCCheckBox* zeroz_cb{nullptr};  ///< `zeroz_cb` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
