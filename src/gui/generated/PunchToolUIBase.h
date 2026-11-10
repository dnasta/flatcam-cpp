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
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `PunchToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class PunchToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit PunchToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCTable* apertures_table{nullptr};  ///< `apertures_table` del original.
    FCCheckBox* circular_cb{nullptr};  ///< `circular_cb` del original.
    FCDoubleSpinner* circular_ring_entry{nullptr};  ///< `circular_ring_entry` del original.
    QLabel* circular_ring_label{nullptr};  ///< `circular_ring_label` del original.
    FCDoubleSpinner* dia_entry{nullptr};  ///< `dia_entry` del original.
    QLabel* dia_label{nullptr};  ///< `dia_label` del original.
    FCComboBox* exc_combo{nullptr};  ///< `exc_combo` del original.
    QLabel* exc_label{nullptr};  ///< `exc_label` del original.
    FCDoubleSpinner* factor_entry{nullptr};  ///< `factor_entry` del original.
    QLabel* factor_label{nullptr};  ///< `factor_label` del original.
    QLabel* fixed_label{nullptr};  ///< `fixed_label` del original.
    FCComboBox* gerber_object_combo{nullptr};  ///< `gerber_object_combo` del original.
    QLabel* grb_label{nullptr};  ///< `grb_label` del original.
    QGridLayout* grid1{nullptr};  ///< `grid1` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    QLabel* method_label{nullptr};  ///< `method_label` del original.
    RadioSet* method_punch{nullptr};  ///< `method_punch` del original.
    FCCheckBox* oblong_cb{nullptr};  ///< `oblong_cb` del original.
    FCDoubleSpinner* oblong_ring_entry{nullptr};  ///< `oblong_ring_entry` del original.
    QLabel* oblong_ring_label{nullptr};  ///< `oblong_ring_label` del original.
    FCCheckBox* other_cb{nullptr};  ///< `other_cb` del original.
    FCDoubleSpinner* other_ring_entry{nullptr};  ///< `other_ring_entry` del original.
    QLabel* other_ring_label{nullptr};  ///< `other_ring_label` del original.
    QLabel* padt_label{nullptr};  ///< `padt_label` del original.
    QLabel* prop_label{nullptr};  ///< `prop_label` del original.
    QPushButton* punch_object_button{nullptr};  ///< `punch_object_button` del original.
    FCCheckBox* rectangular_cb{nullptr};  ///< `rectangular_cb` del original.
    FCDoubleSpinner* rectangular_ring_entry{nullptr};  ///< `rectangular_ring_entry` del original.
    QLabel* rectangular_ring_label{nullptr};  ///< `rectangular_ring_label` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    QVBoxLayout* ring_box{nullptr};  ///< `ring_box` del original.
    QFrame* ring_frame{nullptr};  ///< `ring_frame` del original.
    QLabel* ring_label{nullptr};  ///< `ring_label` del original.
    FCCheckBox* select_all_cb{nullptr};  ///< `select_all_cb` del original.
    FCCheckBox* square_cb{nullptr};  ///< `square_cb` del original.
    FCDoubleSpinner* square_ring_entry{nullptr};  ///< `square_ring_entry` del original.
    QLabel* square_ring_label{nullptr};  ///< `square_ring_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
