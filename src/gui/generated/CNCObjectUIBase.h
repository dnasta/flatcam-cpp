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
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/FCTree.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `CNCObjectUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class CNCObjectUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit CNCObjectUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* add_bd_button{nullptr};  ///< `add_bd_button` del original.
    FCButton* al_add_button{nullptr};  ///< `al_add_button` del original.
    QVBoxLayout* al_box{nullptr};  ///< `al_box` del original.
    FCSpinner* al_columns_entry{nullptr};  ///< `al_columns_entry` del original.
    FCLabel* al_columns_label{nullptr};  ///< `al_columns_label` del original.
    FCComboBox* al_controller_combo{nullptr};  ///< `al_controller_combo` del original.
    FCLabel* al_controller_label{nullptr};  ///< `al_controller_label` del original.
    QFrame* al_frame{nullptr};  ///< `al_frame` del original.
    FCLabel* al_method_lbl{nullptr};  ///< `al_method_lbl` del original.
    RadioSet* al_method_radio{nullptr};  ///< `al_method_radio` del original.
    RadioSet* al_mode_radio{nullptr};  ///< `al_mode_radio` del original.
    FCTable* al_probe_points_table{nullptr};  ///< `al_probe_points_table` del original.
    FCSpinner* al_rows_entry{nullptr};  ///< `al_rows_entry` del original.
    FCLabel* al_rows_label{nullptr};  ///< `al_rows_label` del original.
    FCDetachableTab* al_toolbar{nullptr};  ///< `al_toolbar` del original.
    FCCheckBox* annotation_cb{nullptr};  ///< `annotation_cb` del original.
    FCComboBox* baudrates_list_combo{nullptr};  ///< `baudrates_list_combo` del original.
    FCLabel* baudrates_list_label{nullptr};  ///< `baudrates_list_label` del original.
    FCTable* cnc_tools_table{nullptr};  ///< `cnc_tools_table` del original.
    FCLabel* cnc_tools_table_label{nullptr};  ///< `cnc_tools_table_label` del original.
    RadioSet* cncplot_method_combo{nullptr};  ///< `cncplot_method_combo` del original.
    FCLabel* cncplot_method_label{nullptr};  ///< `cncplot_method_label` del original.
    FCButton* com_connect_button{nullptr};  ///< `com_connect_button` del original.
    FCComboBox* com_list_combo{nullptr};  ///< `com_list_combo` del original.
    FCLabel* com_list_label{nullptr};  ///< `com_list_label` del original.
    FCButton* com_search_button{nullptr};  ///< `com_search_button` del original.
    QGridLayout* common_grid{nullptr};  ///< `common_grid` del original.
    FCButton* controller_reset_button{nullptr};  ///< `controller_reset_button` del original.
    QVBoxLayout* custom_box{nullptr};  ///< `custom_box` del original.
    FCButton* del_bd_button{nullptr};  ///< `del_bd_button` del original.
    FCButton* editor_button{nullptr};  ///< `editor_button` del original.
    FCTable* exc_cnc_tools_table{nullptr};  ///< `exc_cnc_tools_table` del original.
    FCButton* export_gcode_button{nullptr};  ///< `export_gcode_button` del original.
    FCLabel* export_gcode_label{nullptr};  ///< `export_gcode_label` del original.
    FCDoubleSpinner* feedrate_probe_entry{nullptr};  ///< `feedrate_probe_entry` del original.
    FCLabel* feedrate_probe_label{nullptr};  ///< `feedrate_probe_label` del original.
    QWidget* gr_conn_tab{nullptr};  ///< `gr_conn_tab` del original.
    QVBoxLayout* gr_conn_tab_layout{nullptr};  ///< `gr_conn_tab_layout` del original.
    QWidget* gr_ctrl_tab{nullptr};  ///< `gr_ctrl_tab` del original.
    QVBoxLayout* gr_ctrl_tab_layout{nullptr};  ///< `gr_ctrl_tab_layout` del original.
    QWidget* gr_send_tab{nullptr};  ///< `gr_send_tab` del original.
    QVBoxLayout* gr_send_tab_layout{nullptr};  ///< `gr_send_tab_layout` del original.
    QVBoxLayout* grbl_box{nullptr};  ///< `grbl_box` del original.
    FCEntry* grbl_command_entry{nullptr};  ///< `grbl_command_entry` del original.
    FCLabel* grbl_command_label{nullptr};  ///< `grbl_command_label` del original.
    QFrame* grbl_frame{nullptr};  ///< `grbl_frame` del original.
    FCButton* grbl_get_heightmap_button{nullptr};  ///< `grbl_get_heightmap_button` del original.
    QToolButton* grbl_get_param_button{nullptr};  ///< `grbl_get_param_button` del original.
    FCLabel* grbl_get_param_label{nullptr};  ///< `grbl_get_param_label` del original.
    FCEntry* grbl_parameter_entry{nullptr};  ///< `grbl_parameter_entry` del original.
    FCButton* grbl_report_button{nullptr};  ///< `grbl_report_button` del original.
    QToolButton* grbl_save_height_map_button{nullptr};  ///< `grbl_save_height_map_button` del original.
    QToolButton* grbl_send_button{nullptr};  ///< `grbl_send_button` del original.
    FCButton* h_gcode_button{nullptr};  ///< `h_gcode_button` del original.
    FCLabel* icon{nullptr};  ///< `icon` del original.
    FCButton* import_heights_button{nullptr};  ///< `import_heights_button` del original.
    QFrame* jog_fr_entry{nullptr};  ///< `jog_fr_entry` del original.
    FCLabel* jog_fr_label{nullptr};  ///< `jog_fr_label` del original.
    QFrame* jog_step_entry{nullptr};  ///< `jog_step_entry` del original.
    FCLabel* jog_step_label{nullptr};  ///< `jog_step_label` del original.
    QFrame* jog_wdg{nullptr};  ///< `jog_wdg` del original.
    FCLabel* level{nullptr};  ///< `level` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    QHBoxLayout* name_hlay{nullptr};  ///< `name_hlay` del original.
    FCSpinner* new_baudrate_entry{nullptr};  ///< `new_baudrate_entry` del original.
    FCLabel* new_bd_label{nullptr};  ///< `new_bd_label` del original.
    FCButton* offset_button{nullptr};  ///< `offset_button` del original.
    NumericalEvalTupleEntry* offsetvector_entry{nullptr};  ///< `offsetvector_entry` del original.
    QToolButton* pause_resume_button{nullptr};  ///< `pause_resume_button` del original.
    FCDoubleSpinner* pdepth_entry{nullptr};  ///< `pdepth_entry` del original.
    FCLabel* pdepth_label{nullptr};  ///< `pdepth_label` del original.
    FCCheckBox* plot_cb{nullptr};  ///< `plot_cb` del original.
    FCCheckBox* plot_probing_pts_cb{nullptr};  ///< `plot_probing_pts_cb` del original.
    FCLabel* probe_gc_label{nullptr};  ///< `probe_gc_label` del original.
    QVBoxLayout* properties_box{nullptr};  ///< `properties_box` del original.
    FCButton* properties_button{nullptr};  ///< `properties_button` del original.
    QFrame* properties_frame{nullptr};  ///< `properties_frame` del original.
    FCDoubleSpinner* ptravelz_entry{nullptr};  ///< `ptravelz_entry` del original.
    FCLabel* ptravelz_label{nullptr};  ///< `ptravelz_label` del original.
    QToolButton* review_gcode_button{nullptr};  ///< `review_gcode_button` del original.
    FCButton* sal_btn{nullptr};  ///< `sal_btn` del original.
    FCButton* scale_button{nullptr};  ///< `scale_button` del original.
    NumericalEvalEntry* scale_entry{nullptr};  ///< `scale_entry` del original.
    FCCheckBox* show_al_table{nullptr};  ///< `show_al_table` del original.
    FCCheckBox* snippets_cb{nullptr};  ///< `snippets_cb` del original.
    FCEntry* t_distance_entry{nullptr};  ///< `t_distance_entry` del original.
    FCLabel* t_distance_label{nullptr};  ///< `t_distance_label` del original.
    FCEntry* t_time_entry{nullptr};  ///< `t_time_entry` del original.
    FCLabel* t_time_label{nullptr};  ///< `t_time_label` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* title_label{nullptr};  ///< `title_label` del original.
    FCDoubleSpinner* tooldia_entry{nullptr};  ///< `tooldia_entry` del original.
    FCLabel* transform_label{nullptr};  ///< `transform_label` del original.
    FCButton* transformations_button{nullptr};  ///< `transformations_button` del original.
    FCTree* treeWidget{nullptr};  ///< `treeWidget` del original.
    FCLabel* units_label{nullptr};  ///< `units_label` del original.
    FCLabel* units_time_label{nullptr};  ///< `units_time_label` del original.
    FCButton* updateplot_button{nullptr};  ///< `updateplot_button` del original.
    QToolButton* view_h_gcode_button{nullptr};  ///< `view_h_gcode_button` del original.
    QFrame* zero_axs_wdg{nullptr};  ///< `zero_axs_wdg` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
