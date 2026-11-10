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
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `CalculatorsToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class CalculatorsToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit CalculatorsToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCDoubleSpinner* area_entry{nullptr};  ///< `area_entry` del original.
    FCLabel* area_label{nullptr};  ///< `area_label` del original.
    FCLabel* area_sel_label{nullptr};  ///< `area_sel_label` del original.
    RadioSet* area_sel_radio{nullptr};  ///< `area_sel_radio` del original.
    FCLabel* area_unit{nullptr};  ///< `area_unit` del original.
    FCButton* calculate_plate_button{nullptr};  ///< `calculate_plate_button` del original.
    FCButton* calculate_vshape_button{nullptr};  ///< `calculate_vshape_button` del original.
    FCDoubleSpinner* cdensity_entry{nullptr};  ///< `cdensity_entry` del original.
    FCLabel* cdensity_label{nullptr};  ///< `cdensity_label` del original.
    FCDoubleSpinner* cutDepth_entry{nullptr};  ///< `cutDepth_entry` del original.
    FCLabel* cutDepth_label{nullptr};  ///< `cutDepth_label` del original.
    FCDoubleSpinner* cvalue_entry{nullptr};  ///< `cvalue_entry` del original.
    FCLabel* cvaluelabel{nullptr};  ///< `cvaluelabel` del original.
    FCDoubleSpinner* effectiveToolDia_entry{nullptr};  ///< `effectiveToolDia_entry` del original.
    FCLabel* effectiveToolDia_label{nullptr};  ///< `effectiveToolDia_label` del original.
    FCDoubleSpinner* growth_entry{nullptr};  ///< `growth_entry` del original.
    FCLabel* growth_label{nullptr};  ///< `growth_label` del original.
    NumericalEvalEntry* inch_entry{nullptr};  ///< `inch_entry` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCLabel* length_unit{nullptr};  ///< `length_unit` del original.
    NumericalEvalEntry* mm_entry{nullptr};  ///< `mm_entry` del original.
    FCDoubleSpinner* pcblength_entry{nullptr};  ///< `pcblength_entry` del original.
    FCLabel* pcblengthlabel{nullptr};  ///< `pcblengthlabel` del original.
    FCDoubleSpinner* pcbwidth_entry{nullptr};  ///< `pcbwidth_entry` del original.
    FCLabel* pcbwidthlabel{nullptr};  ///< `pcbwidthlabel` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCDoubleSpinner* time_entry{nullptr};  ///< `time_entry` del original.
    FCLabel* timelabel{nullptr};  ///< `timelabel` del original.
    FCSpinner* tipAngle_entry{nullptr};  ///< `tipAngle_entry` del original.
    FCLabel* tipAngle_label{nullptr};  ///< `tipAngle_label` del original.
    FCDoubleSpinner* tipDia_entry{nullptr};  ///< `tipDia_entry` del original.
    FCLabel* tipDia_label{nullptr};  ///< `tipDia_label` del original.
    FCLabel* unists_spacer_label{nullptr};  ///< `unists_spacer_label` del original.
    FCLabel* v_shape_spacer_label{nullptr};  ///< `v_shape_spacer_label` del original.
    FCLabel* width_unit{nullptr};  ///< `width_unit` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
