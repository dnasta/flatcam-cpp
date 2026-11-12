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
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `FiducialsToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class FiducialsToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit FiducialsToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QPushButton* add_cfid_button{nullptr};  ///< `add_cfid_button` del original.
    QPushButton* add_sm_opening_button{nullptr};  ///< `add_sm_opening_button` del original.
    FCDoubleSpinner* fid_size_entry{nullptr};  ///< `fid_size_entry` del original.
    QLabel* fid_type_label{nullptr};  ///< `fid_type_label` del original.
    RadioSet* fid_type_radio{nullptr};  ///< `fid_type_radio` del original.
    FCComboBox* grb_object_combo{nullptr};  ///< `grb_object_combo` del original.
    QLabel* grbobj_label{nullptr};  ///< `grbobj_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCDoubleSpinner* line_thickness_entry{nullptr};  ///< `line_thickness_entry` del original.
    QLabel* line_thickness_label{nullptr};  ///< `line_thickness_label` del original.
    FCDoubleSpinner* margin_entry{nullptr};  ///< `margin_entry` del original.
    QLabel* margin_label{nullptr};  ///< `margin_label` del original.
    QLabel* mode_label{nullptr};  ///< `mode_label` del original.
    RadioSet* mode_radio{nullptr};  ///< `mode_radio` del original.
    QLabel* param_label{nullptr};  ///< `param_label` del original.
    QLabel* points_label{nullptr};  ///< `points_label` del original.
    FCTable* points_table{nullptr};  ///< `points_table` del original.
    QLabel* pos_label{nullptr};  ///< `pos_label` del original.
    RadioSet* pos_radio{nullptr};  ///< `pos_radio` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    QLabel* size_label{nullptr};  ///< `size_label` del original.
    FCComboBox* sm_object_combo{nullptr};  ///< `sm_object_combo` del original.
    QLabel* sm_object_label{nullptr};  ///< `sm_object_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
