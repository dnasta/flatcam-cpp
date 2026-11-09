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
#include "gui/widgets/FCButton.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"

namespace flatcam::gui {

/**
 * @brief Panel `DistanceToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class DistanceToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit DistanceToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCEntry* angle_entry{nullptr};  ///< `angle_entry` del original.
    FCLabel* angle_label{nullptr};  ///< `angle_label` del original.
    FCEntry* distance_x_entry{nullptr};  ///< `distance_x_entry` del original.
    FCLabel* distance_x_label{nullptr};  ///< `distance_x_label` del original.
    FCEntry* distance_y_entry{nullptr};  ///< `distance_y_entry` del original.
    FCLabel* distance_y_label{nullptr};  ///< `distance_y_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCButton* measure_btn{nullptr};  ///< `measure_btn` del original.
    FCCheckBox* snap_center_cb{nullptr};  ///< `snap_center_cb` del original.
    FCEntry* start_entry{nullptr};  ///< `start_entry` del original.
    FCLabel* start_label{nullptr};  ///< `start_label` del original.
    FCEntry* stop_entry{nullptr};  ///< `stop_entry` del original.
    FCLabel* stop_label{nullptr};  ///< `stop_label` del original.
    FCEntry* total_distance_entry{nullptr};  ///< `total_distance_entry` del original.
    FCLabel* total_distance_label{nullptr};  ///< `total_distance_label` del original.
    FCLabel* units_label{nullptr};  ///< `units_label` del original.
    FCLabel* units_value{nullptr};  ///< `units_value` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
