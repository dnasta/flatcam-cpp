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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `DblSidedToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class DblSidedToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit DblSidedToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_drill_point_button{nullptr};  ///< `add_drill_point_button` del original.
    FCButton* add_point_button{nullptr};  ///< `add_point_button` del original.
    FCLabel* ah_label{nullptr};  ///< `ah_label` del original.
    FCLabel* align_ax_label{nullptr};  ///< `align_ax_label` del original.
    RadioSet* align_axis_radio{nullptr};  ///< `align_axis_radio` del original.
    FCLabel* align_ref_label{nullptr};  ///< `align_ref_label` del original.
    NumericalEvalTupleEntry* align_ref_label_val{nullptr};  ///< `align_ref_label_val` del original.
    NumericalEvalTupleEntry* alignment_holes{nullptr};  ///< `alignment_holes` del original.
    FCLabel* alignment_label{nullptr};  ///< `alignment_label` del original.
    RadioSet* axis_location{nullptr};  ///< `axis_location` del original.
    FCLabel* axloc_label{nullptr};  ///< `axloc_label` del original.
    FCComboBox* box_combo{nullptr};  ///< `box_combo` del original.
    FCLabel* box_type_label{nullptr};  ///< `box_type_label` del original.
    RadioSet* box_type_radio{nullptr};  ///< `box_type_radio` del original.
    FCLabel* bv_label{nullptr};  ///< `bv_label` del original.
    FCButton* calculate_bb_button{nullptr};  ///< `calculate_bb_button` del original.
    FCButton* center_btn{nullptr};  ///< `center_btn` del original.
    NumericalEvalTupleEntry* center_entry{nullptr};  ///< `center_entry` del original.
    FCButton* create_alignment_hole_button{nullptr};  ///< `create_alignment_hole_button` del original.
    FCButton* delete_drill_point_button{nullptr};  ///< `delete_drill_point_button` del original.
    FCDoubleSpinner* drill_dia{nullptr};  ///< `drill_dia` del original.
    FCLabel* dt_label{nullptr};  ///< `dt_label` del original.
    FCComboBox* exc_combo{nullptr};  ///< `exc_combo` del original.
    FCLabel* exc_hole_lbl{nullptr};  ///< `exc_hole_lbl` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCLabel* m_objects_label{nullptr};  ///< `m_objects_label` del original.
    FCLabel* mirax_label{nullptr};  ///< `mirax_label` del original.
    RadioSet* mirror_axis{nullptr};  ///< `mirror_axis` del original.
    FCButton* mirror_button{nullptr};  ///< `mirror_button` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    RadioSet* object_type_radio{nullptr};  ///< `object_type_radio` del original.
    FCLabel* param_label{nullptr};  ///< `param_label` del original.
    FCButton* pick_hole_button{nullptr};  ///< `pick_hole_button` del original.
    NumericalEvalTupleEntry* point_entry{nullptr};  ///< `point_entry` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCLabel* type_obj_combo_label{nullptr};  ///< `type_obj_combo_label` del original.
    FCButton* xmax_btn{nullptr};  ///< `xmax_btn` del original.
    FCDoubleSpinner* xmax_entry{nullptr};  ///< `xmax_entry` del original.
    FCButton* xmin_btn{nullptr};  ///< `xmin_btn` del original.
    FCDoubleSpinner* xmin_entry{nullptr};  ///< `xmin_entry` del original.
    FCButton* ymax_btn{nullptr};  ///< `ymax_btn` del original.
    FCDoubleSpinner* ymax_entry{nullptr};  ///< `ymax_entry` del original.
    FCButton* ymin_btn{nullptr};  ///< `ymin_btn` del original.
    FCDoubleSpinner* ymin_entry{nullptr};  ///< `ymin_entry` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
