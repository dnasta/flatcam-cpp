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

namespace flatcam::gui {

/**
 * @brief Panel `RulesToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class RulesToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit RulesToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCCheckBox* all_cb{nullptr};  ///< `all_cb` del original.
    FCCheckBox* all_obj_cb{nullptr};  ///< `all_obj_cb` del original.
    FCCheckBox* clearance_copper2copper_cb{nullptr};  ///< `clearance_copper2copper_cb` del original.
    FCDoubleSpinner* clearance_copper2copper_entry{nullptr};  ///< `clearance_copper2copper_entry` del original.
    FCLabel* clearance_copper2copper_lbl{nullptr};  ///< `clearance_copper2copper_lbl` del original.
    FCCheckBox* clearance_copper2ol_cb{nullptr};  ///< `clearance_copper2ol_cb` del original.
    FCDoubleSpinner* clearance_copper2ol_entry{nullptr};  ///< `clearance_copper2ol_entry` del original.
    FCLabel* clearance_copper2ol_lbl{nullptr};  ///< `clearance_copper2ol_lbl` del original.
    FCCheckBox* clearance_d2d_cb{nullptr};  ///< `clearance_d2d_cb` del original.
    FCDoubleSpinner* clearance_d2d_entry{nullptr};  ///< `clearance_d2d_entry` del original.
    FCLabel* clearance_d2d_lbl{nullptr};  ///< `clearance_d2d_lbl` del original.
    FCCheckBox* clearance_silk2ol_cb{nullptr};  ///< `clearance_silk2ol_cb` del original.
    FCDoubleSpinner* clearance_silk2ol_entry{nullptr};  ///< `clearance_silk2ol_entry` del original.
    FCLabel* clearance_silk2ol_lbl{nullptr};  ///< `clearance_silk2ol_lbl` del original.
    FCCheckBox* clearance_silk2silk_cb{nullptr};  ///< `clearance_silk2silk_cb` del original.
    FCDoubleSpinner* clearance_silk2silk_entry{nullptr};  ///< `clearance_silk2silk_entry` del original.
    FCLabel* clearance_silk2silk_lbl{nullptr};  ///< `clearance_silk2silk_lbl` del original.
    FCCheckBox* clearance_silk2sm_cb{nullptr};  ///< `clearance_silk2sm_cb` del original.
    FCDoubleSpinner* clearance_silk2sm_entry{nullptr};  ///< `clearance_silk2sm_entry` del original.
    FCLabel* clearance_silk2sm_lbl{nullptr};  ///< `clearance_silk2sm_lbl` del original.
    FCCheckBox* clearance_sm2sm_cb{nullptr};  ///< `clearance_sm2sm_cb` del original.
    FCDoubleSpinner* clearance_sm2sm_entry{nullptr};  ///< `clearance_sm2sm_entry` del original.
    FCLabel* clearance_sm2sm_lbl{nullptr};  ///< `clearance_sm2sm_lbl` del original.
    FCCheckBox* copper_b_cb{nullptr};  ///< `copper_b_cb` del original.
    FCComboBox* copper_b_object{nullptr};  ///< `copper_b_object` del original.
    FCLabel* copper_b_object_lbl{nullptr};  ///< `copper_b_object_lbl` del original.
    FCCheckBox* copper_t_cb{nullptr};  ///< `copper_t_cb` del original.
    FCComboBox* copper_t_object{nullptr};  ///< `copper_t_object` del original.
    FCLabel* copper_t_object_lbl{nullptr};  ///< `copper_t_object_lbl` del original.
    FCCheckBox* drill_size_cb{nullptr};  ///< `drill_size_cb` del original.
    FCDoubleSpinner* drill_size_entry{nullptr};  ///< `drill_size_entry` del original.
    FCLabel* drill_size_lbl{nullptr};  ///< `drill_size_lbl` del original.
    FCCheckBox* e1_cb{nullptr};  ///< `e1_cb` del original.
    FCComboBox* e1_object{nullptr};  ///< `e1_object` del original.
    FCLabel* e1_object_lbl{nullptr};  ///< `e1_object_lbl` del original.
    FCCheckBox* e2_cb{nullptr};  ///< `e2_cb` del original.
    FCComboBox* e2_object{nullptr};  ///< `e2_object` del original.
    FCLabel* e2_object_lbl{nullptr};  ///< `e2_object_lbl` del original.
    FCLabel* excellon_title_lbl{nullptr};  ///< `excellon_title_lbl` del original.
    QFormLayout* form_layout_1{nullptr};  ///< `form_layout_1` del original.
    FCLabel* gerber_title_lbl{nullptr};  ///< `gerber_title_lbl` del original.
    QGridLayout* grid_layout{nullptr};  ///< `grid_layout` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCCheckBox* out_cb{nullptr};  ///< `out_cb` del original.
    FCComboBox* outline_object{nullptr};  ///< `outline_object` del original.
    FCLabel* outline_object_lbl{nullptr};  ///< `outline_object_lbl` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCCheckBox* ring_integrity_cb{nullptr};  ///< `ring_integrity_cb` del original.
    FCDoubleSpinner* ring_integrity_entry{nullptr};  ///< `ring_integrity_entry` del original.
    FCLabel* ring_integrity_lbl{nullptr};  ///< `ring_integrity_lbl` del original.
    FCButton* run_button{nullptr};  ///< `run_button` del original.
    FCCheckBox* sm_b_cb{nullptr};  ///< `sm_b_cb` del original.
    FCComboBox* sm_b_object{nullptr};  ///< `sm_b_object` del original.
    FCLabel* sm_b_object_lbl{nullptr};  ///< `sm_b_object_lbl` del original.
    FCCheckBox* sm_t_cb{nullptr};  ///< `sm_t_cb` del original.
    FCComboBox* sm_t_object{nullptr};  ///< `sm_t_object` del original.
    FCLabel* sm_t_object_lbl{nullptr};  ///< `sm_t_object_lbl` del original.
    FCCheckBox* ss_b_cb{nullptr};  ///< `ss_b_cb` del original.
    FCComboBox* ss_b_object{nullptr};  ///< `ss_b_object` del original.
    FCLabel* ss_b_object_lbl{nullptr};  ///< `ss_b_object_lbl` del original.
    FCCheckBox* ss_t_cb{nullptr};  ///< `ss_t_cb` del original.
    FCComboBox* ss_t_object{nullptr};  ///< `ss_t_object` del original.
    FCLabel* ss_t_object_lbl{nullptr};  ///< `ss_t_object_lbl` del original.
    FCCheckBox* trace_size_cb{nullptr};  ///< `trace_size_cb` del original.
    FCDoubleSpinner* trace_size_entry{nullptr};  ///< `trace_size_entry` del original.
    FCLabel* trace_size_lbl{nullptr};  ///< `trace_size_lbl` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
