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
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/FCTree.h"
#include "gui/widgets/NumericalEval.h"

namespace flatcam::gui {

/**
 * @brief Panel `ExcellonObjectUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class ExcellonObjectUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit ExcellonObjectUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCCheckBox* autoload_db_cb{nullptr};  ///< `autoload_db_cb` del original.
    QGridLayout* common_grid{nullptr};  ///< `common_grid` del original.
    QVBoxLayout* custom_box{nullptr};  ///< `custom_box` del original.
    FCButton* drill_button{nullptr};  ///< `drill_button` del original.
    QFrame* drills_frame{nullptr};  ///< `drills_frame` del original.
    FCButton* editor_button{nullptr};  ///< `editor_button` del original.
    FCButton* generate_milling_button{nullptr};  ///< `generate_milling_button` del original.
    FCButton* generate_milling_slots_button{nullptr};  ///< `generate_milling_slots_button` del original.
    FCLabel* icon{nullptr};  ///< `icon` del original.
    FCLabel* level{nullptr};  ///< `level` del original.
    FCLabel* mill_hole_label{nullptr};  ///< `mill_hole_label` del original.
    FCButton* milling_button{nullptr};  ///< `milling_button` del original.
    FCCheckBox* multicolored_cb{nullptr};  ///< `multicolored_cb` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    QHBoxLayout* name_hlay{nullptr};  ///< `name_hlay` del original.
    FCButton* offset_button{nullptr};  ///< `offset_button` del original.
    NumericalEvalTupleEntry* offsetvector_entry{nullptr};  ///< `offsetvector_entry` del original.
    FCCheckBox* plot_cb{nullptr};  ///< `plot_cb` del original.
    FCLabel* plot_options_label{nullptr};  ///< `plot_options_label` del original.
    QVBoxLayout* properties_box{nullptr};  ///< `properties_box` del original.
    FCButton* properties_button{nullptr};  ///< `properties_button` del original.
    QFrame* properties_frame{nullptr};  ///< `properties_frame` del original.
    FCButton* scale_button{nullptr};  ///< `scale_button` del original.
    NumericalEvalEntry* scale_entry{nullptr};  ///< `scale_entry` del original.
    FCDoubleSpinner* slot_tooldia_entry{nullptr};  ///< `slot_tooldia_entry` del original.
    FCCheckBox* solid_cb{nullptr};  ///< `solid_cb` del original.
    FCCheckBox* table_visibility_cb{nullptr};  ///< `table_visibility_cb` del original.
    FCLabel* tdlabel{nullptr};  ///< `tdlabel` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* title_label{nullptr};  ///< `title_label` del original.
    FCLabel* tool_lbl{nullptr};  ///< `tool_lbl` del original.
    FCDoubleSpinner* tooldia_entry{nullptr};  ///< `tooldia_entry` del original.
    QVBoxLayout* tools_box{nullptr};  ///< `tools_box` del original.
    FCTable* tools_table{nullptr};  ///< `tools_table` del original.
    FCLabel* tools_table_label{nullptr};  ///< `tools_table_label` del original.
    FCLabel* transform_label{nullptr};  ///< `transform_label` del original.
    FCButton* transformations_button{nullptr};  ///< `transformations_button` del original.
    FCTree* treeWidget{nullptr};  ///< `treeWidget` del original.
    QVBoxLayout* util_box{nullptr};  ///< `util_box` del original.
    FCButton* util_button{nullptr};  ///< `util_button` del original.
    QFrame* util_frame{nullptr};  ///< `util_frame` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
