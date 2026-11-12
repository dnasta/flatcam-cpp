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
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `PanelizeToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class PanelizeToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit PanelizeToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCComboBox* box_combo{nullptr};  ///< `box_combo` del original.
    FCLabel* box_label{nullptr};  ///< `box_label` del original.
    FCSpinner* columns{nullptr};  ///< `columns` del original.
    FCLabel* columns_label{nullptr};  ///< `columns_label` del original.
    FCCheckBox* constrain_cb{nullptr};  ///< `constrain_cb` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    FCLabel* object_label{nullptr};  ///< `object_label` del original.
    FCCheckBox* optimization_cb{nullptr};  ///< `optimization_cb` del original.
    FCLabel* panel_type_label{nullptr};  ///< `panel_type_label` del original.
    RadioSet* panel_type_radio{nullptr};  ///< `panel_type_radio` del original.
    FCButton* panelize_object_button{nullptr};  ///< `panelize_object_button` del original.
    RadioSet* reference_radio{nullptr};  ///< `reference_radio` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCSpinner* rows{nullptr};  ///< `rows` del original.
    FCLabel* rows_label{nullptr};  ///< `rows_label` del original.
    FCDoubleSpinner* spacing_columns{nullptr};  ///< `spacing_columns` del original.
    FCLabel* spacing_columns_label{nullptr};  ///< `spacing_columns_label` del original.
    FCDoubleSpinner* spacing_rows{nullptr};  ///< `spacing_rows` del original.
    FCLabel* spacing_rows_label{nullptr};  ///< `spacing_rows_label` del original.
    FCComboBox* type_box_combo{nullptr};  ///< `type_box_combo` del original.
    FCLabel* type_box_combo_label{nullptr};  ///< `type_box_combo_label` del original.
    FCComboBox* type_obj_combo{nullptr};  ///< `type_obj_combo` del original.
    FCLabel* type_object_label{nullptr};  ///< `type_object_label` del original.
    FCDoubleSpinner* x_width_entry{nullptr};  ///< `x_width_entry` del original.
    FCLabel* x_width_lbl{nullptr};  ///< `x_width_lbl` del original.
    FCDoubleSpinner* y_height_entry{nullptr};  ///< `y_height_entry` del original.
    FCLabel* y_height_lbl{nullptr};  ///< `y_height_lbl` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
