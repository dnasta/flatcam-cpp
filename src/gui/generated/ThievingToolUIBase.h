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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `ThievingToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class ThievingToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit ThievingToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCDoubleSpinner* area_entry{nullptr};  ///< `area_entry` del original.
    FCLabel* area_label{nullptr};  ///< `area_label` del original.
    FCLabel* bbox_type_label{nullptr};  ///< `bbox_type_label` del original.
    RadioSet* bbox_type_radio{nullptr};  ///< `bbox_type_radio` del original.
    FCDoubleSpinner* clearance_entry{nullptr};  ///< `clearance_entry` del original.
    FCLabel* clearance_label{nullptr};  ///< `clearance_label` del original.
    FCDoubleSpinner* clearance_ppm_entry{nullptr};  ///< `clearance_ppm_entry` del original.
    FCLabel* clearance_ppm_label{nullptr};  ///< `clearance_ppm_label` del original.
    FCLabel* copper_fill_label{nullptr};  ///< `copper_fill_label` del original.
    FCDoubleSpinner* dot_dia_entry{nullptr};  ///< `dot_dia_entry` del original.
    FCDoubleSpinner* dot_spacing_entry{nullptr};  ///< `dot_spacing_entry` del original.
    FCLabel* dotdia_label{nullptr};  ///< `dotdia_label` del original.
    QFrame* dots_frame{nullptr};  ///< `dots_frame` del original.
    FCLabel* dots_label{nullptr};  ///< `dots_label` del original.
    FCLabel* dotspacing_label{nullptr};  ///< `dotspacing_label` del original.
    QPushButton* fill_button{nullptr};  ///< `fill_button` del original.
    FCLabel* fill_type_label{nullptr};  ///< `fill_type_label` del original.
    RadioSet* fill_type_radio{nullptr};  ///< `fill_type_radio` del original.
    FCComboBox* grb_object_combo{nullptr};  ///< `grb_object_combo` del original.
    FCLabel* grbobj_label{nullptr};  ///< `grbobj_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCDoubleSpinner* line_size_entry{nullptr};  ///< `line_size_entry` del original.
    FCLabel* line_size_label{nullptr};  ///< `line_size_label` del original.
    QFrame* lines_frame{nullptr};  ///< `lines_frame` del original.
    FCLabel* lines_label{nullptr};  ///< `lines_label` del original.
    FCDoubleSpinner* lines_spacing_entry{nullptr};  ///< `lines_spacing_entry` del original.
    FCLabel* lines_spacing_label{nullptr};  ///< `lines_spacing_label` del original.
    FCDoubleSpinner* margin_entry{nullptr};  ///< `margin_entry` del original.
    FCLabel* margin_label{nullptr};  ///< `margin_label` del original.
    FCLabel* patern_mask_label{nullptr};  ///< `patern_mask_label` del original.
    FCEntry* plated_area_entry{nullptr};  ///< `plated_area_entry` del original.
    FCLabel* plated_area_label{nullptr};  ///< `plated_area_label` del original.
    QPushButton* ppm_button{nullptr};  ///< `ppm_button` del original.
    FCLabel* ppm_choice_label{nullptr};  ///< `ppm_choice_label` del original.
    RadioSet* ppm_choice_radio{nullptr};  ///< `ppm_choice_radio` del original.
    QPushButton* rb_button{nullptr};  ///< `rb_button` del original.
    FCDoubleSpinner* rb_margin_entry{nullptr};  ///< `rb_margin_entry` del original.
    FCLabel* rb_margin_label{nullptr};  ///< `rb_margin_label` del original.
    FCDoubleSpinner* rb_thickness_entry{nullptr};  ///< `rb_thickness_entry` del original.
    FCLabel* rb_thickness_label{nullptr};  ///< `rb_thickness_label` del original.
    FCComboBox* ref_combo{nullptr};  ///< `ref_combo` del original.
    FCLabel* ref_combo_label{nullptr};  ///< `ref_combo_label` del original.
    FCComboBox* ref_combo_type{nullptr};  ///< `ref_combo_type` del original.
    FCLabel* ref_combo_type_label{nullptr};  ///< `ref_combo_type_label` del original.
    FCLabel* reference_label{nullptr};  ///< `reference_label` del original.
    RadioSet* reference_radio{nullptr};  ///< `reference_radio` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCLabel* robber_bar_label{nullptr};  ///< `robber_bar_label` del original.
    FCLabel* sm_obj_label{nullptr};  ///< `sm_obj_label` del original.
    FCComboBox* sm_object_combo{nullptr};  ///< `sm_object_combo` del original.
    FCDoubleSpinner* square_size_entry{nullptr};  ///< `square_size_entry` del original.
    FCLabel* square_size_label{nullptr};  ///< `square_size_label` del original.
    QFrame* squares_frame{nullptr};  ///< `squares_frame` del original.
    FCLabel* squares_label{nullptr};  ///< `squares_label` del original.
    FCDoubleSpinner* squares_spacing_entry{nullptr};  ///< `squares_spacing_entry` del original.
    FCLabel* squares_spacing_label{nullptr};  ///< `squares_spacing_label` del original.
    FCLabel* units_area_label{nullptr};  ///< `units_area_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
