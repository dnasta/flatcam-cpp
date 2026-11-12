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
 * @brief Panel `CornersToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class CornersToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit CornersToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_marker_button{nullptr};  ///< `add_marker_button` del original.
    FCCheckBox* bl_cb{nullptr};  ///< `bl_cb` del original.
    FCCheckBox* br_cb{nullptr};  ///< `br_cb` del original.
    FCButton* drill_button{nullptr};  ///< `drill_button` del original.
    FCDoubleSpinner* drill_dia_entry{nullptr};  ///< `drill_dia_entry` del original.
    FCLabel* drill_dia_label{nullptr};  ///< `drill_dia_label` del original.
    FCLabel* drills_label{nullptr};  ///< `drills_label` del original.
    FCDoubleSpinner* l_entry{nullptr};  ///< `l_entry` del original.
    FCLabel* l_label{nullptr};  ///< `l_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCDoubleSpinner* margin_entry{nullptr};  ///< `margin_entry` del original.
    FCLabel* margin_label{nullptr};  ///< `margin_label` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    FCLabel* object_label{nullptr};  ///< `object_label` del original.
    FCLabel* param_label{nullptr};  ///< `param_label` del original.
    FCLabel* points_label{nullptr};  ///< `points_label` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCDoubleSpinner* thick_entry{nullptr};  ///< `thick_entry` del original.
    FCLabel* thick_label{nullptr};  ///< `thick_label` del original.
    FCCheckBox* tl_cb{nullptr};  ///< `tl_cb` del original.
    FCCheckBox* toggle_all_cb{nullptr};  ///< `toggle_all_cb` del original.
    FCCheckBox* tr_cb{nullptr};  ///< `tr_cb` del original.
    FCLabel* type_label{nullptr};  ///< `type_label` del original.
    RadioSet* type_radio{nullptr};  ///< `type_radio` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
