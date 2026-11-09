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
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `CutOutToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class CutOutToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit CutOutToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_newtool_button{nullptr};  ///< `add_newtool_button` del original.
    FCButton* addtool_from_db_btn{nullptr};  ///< `addtool_from_db_btn` del original.
    FCCheckBox* big_cursor_cb{nullptr};  ///< `big_cursor_cb` del original.
    FCCheckBox* convex_box_cb{nullptr};  ///< `convex_box_cb` del original.
    QLabel* convex_box_label{nullptr};  ///< `convex_box_label` del original.
    FCDoubleSpinner* cutz_entry{nullptr};  ///< `cutz_entry` del original.
    FCDoubleSpinner* dia{nullptr};  ///< `dia` del original.
    QLabel* dia_label{nullptr};  ///< `dia_label` del original.
    FCButton* ff_cutout_object_btn{nullptr};  ///< `ff_cutout_object_btn` del original.
    FCComboBox* gaps{nullptr};  ///< `gaps` del original.
    FCDoubleSpinner* gapsize{nullptr};  ///< `gapsize` del original.
    QLabel* gapsize_label{nullptr};  ///< `gapsize_label` del original.
    FCLabel* gaptype_label{nullptr};  ///< `gaptype_label` del original.
    RadioSet* gaptype_radio{nullptr};  ///< `gaptype_radio` del original.
    QLabel* kindlabel{nullptr};  ///< `kindlabel` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCButton* man_gaps_creation_btn{nullptr};  ///< `man_gaps_creation_btn` del original.
    FCButton* man_geo_creation_btn{nullptr};  ///< `man_geo_creation_btn` del original.
    FCComboBox* man_object_combo{nullptr};  ///< `man_object_combo` del original.
    QLabel* man_object_label{nullptr};  ///< `man_object_label` del original.
    FCDoubleSpinner* margin{nullptr};  ///< `margin` del original.
    QLabel* margin_label{nullptr};  ///< `margin_label` del original.
    FCDoubleSpinner* maxdepth_entry{nullptr};  ///< `maxdepth_entry` del original.
    FCDoubleSpinner* mb_dia_entry{nullptr};  ///< `mb_dia_entry` del original.
    FCLabel* mb_dia_label{nullptr};  ///< `mb_dia_label` del original.
    FCDoubleSpinner* mb_spacing_entry{nullptr};  ///< `mb_spacing_entry` del original.
    FCLabel* mb_spacing_label{nullptr};  ///< `mb_spacing_label` del original.
    FCCheckBox* mpass_cb{nullptr};  ///< `mpass_cb` del original.
    FCComboBox* obj_combo{nullptr};  ///< `obj_combo` del original.
    RadioSet* obj_kind_combo{nullptr};  ///< `obj_kind_combo` del original.
    QLabel* object_label{nullptr};  ///< `object_label` del original.
    QLabel* param_label{nullptr};  ///< `param_label` del original.
    FCButton* rect_cutout_object_btn{nullptr};  ///< `rect_cutout_object_btn` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCDoubleSpinner* thin_depth_entry{nullptr};  ///< `thin_depth_entry` del original.
    FCLabel* thin_depth_label{nullptr};  ///< `thin_depth_label` del original.
    FCLabel* tool_sel_label{nullptr};  ///< `tool_sel_label` del original.
    QLabel* type_obj_combo_label{nullptr};  ///< `type_obj_combo_label` del original.
    RadioSet* type_obj_radio{nullptr};  ///< `type_obj_radio` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
