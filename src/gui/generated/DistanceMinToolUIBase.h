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
#include "gui/widgets/FCEntry.h"

namespace flatcam::gui {

/**
 * @brief Panel `DistanceMinToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class DistanceMinToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit DistanceMinToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCEntry* angle_entry{nullptr};  ///< `angle_entry` del original.
    QLabel* angle_label{nullptr};  ///< `angle_label` del original.
    FCEntry* distance_x_entry{nullptr};  ///< `distance_x_entry` del original.
    QLabel* distance_x_label{nullptr};  ///< `distance_x_label` del original.
    FCEntry* distance_y_entry{nullptr};  ///< `distance_y_entry` del original.
    QLabel* distance_y_label{nullptr};  ///< `distance_y_label` del original.
    FCEntry* half_point_entry{nullptr};  ///< `half_point_entry` del original.
    QLabel* half_point_label{nullptr};  ///< `half_point_label` del original.
    QPushButton* jump_hp_btn{nullptr};  ///< `jump_hp_btn` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    QPushButton* measure_btn{nullptr};  ///< `measure_btn` del original.
    FCEntry* start_entry{nullptr};  ///< `start_entry` del original.
    QLabel* start_label{nullptr};  ///< `start_label` del original.
    FCEntry* stop_entry{nullptr};  ///< `stop_entry` del original.
    QLabel* stop_label{nullptr};  ///< `stop_label` del original.
    FCEntry* total_distance_entry{nullptr};  ///< `total_distance_entry` del original.
    QLabel* total_distance_label{nullptr};  ///< `total_distance_label` del original.
    QLabel* units_label{nullptr};  ///< `units_label` del original.
    QLabel* units_value{nullptr};  ///< `units_value` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
