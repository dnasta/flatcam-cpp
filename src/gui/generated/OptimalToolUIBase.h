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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCPrefWidgets.h"
#include "gui/widgets/FCSpinners.h"

namespace flatcam::gui {

/**
 * @brief Panel `OptimalToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class OptimalToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit OptimalToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* calculate_button{nullptr};  ///< `calculate_button` del original.
    QVBoxLayout* distances_box{nullptr};  ///< `distances_box` del original.
    FCLabel* distances_label{nullptr};  ///< `distances_label` del original.
    FCTextArea* distances_textb{nullptr};  ///< `distances_textb` del original.
    FCEntry* freq_entry{nullptr};  ///< `freq_entry` del original.
    FCLabel* freq_label{nullptr};  ///< `freq_label` del original.
    FCComboBox* gerber_object_combo{nullptr};  ///< `gerber_object_combo` del original.
    FCLabel* gerber_object_label{nullptr};  ///< `gerber_object_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCButton* locate_button{nullptr};  ///< `locate_button` del original.
    FCButton* locate_sec_button{nullptr};  ///< `locate_sec_button` del original.
    FCCheckBox* locations_cb{nullptr};  ///< `locations_cb` del original.
    FCLabel* locations_label{nullptr};  ///< `locations_label` del original.
    FCTextArea* locations_sec_textb{nullptr};  ///< `locations_sec_textb` del original.
    FCTextArea* locations_textb{nullptr};  ///< `locations_textb` del original.
    FCLabel* precision_label{nullptr};  ///< `precision_label` del original.
    FCSpinner* precision_spinner{nullptr};  ///< `precision_spinner` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCEntry* result_entry{nullptr};  ///< `result_entry` del original.
    FCLabel* result_label{nullptr};  ///< `result_label` del original.
    FCCheckBox* sec_locations_cb{nullptr};  ///< `sec_locations_cb` del original.
    QFrame* sec_locations_frame{nullptr};  ///< `sec_locations_frame` del original.
    FCLabel* title_res_label{nullptr};  ///< `title_res_label` del original.
    FCLabel* title_second_res_label{nullptr};  ///< `title_second_res_label` del original.
    FCLabel* units_lbl{nullptr};  ///< `units_lbl` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
