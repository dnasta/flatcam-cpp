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
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `FilmToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class FilmToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit FilmToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCDoubleSpinner* boundary_entry{nullptr};  ///< `boundary_entry` del original.
    FCLabel* boundary_label{nullptr};  ///< `boundary_label` del original.
    FCComboBox* exc_combo{nullptr};  ///< `exc_combo` del original.
    FCLabel* exc_label{nullptr};  ///< `exc_label` del original.
    FCLabel* file_type_label{nullptr};  ///< `file_type_label` del original.
    RadioSet* file_type_radio{nullptr};  ///< `file_type_radio` del original.
    FCLabel* film_adj_label{nullptr};  ///< `film_adj_label` del original.
    RadioSet* film_mirror_axis{nullptr};  ///< `film_mirror_axis` del original.
    FCLabel* film_mirror_axis_label{nullptr};  ///< `film_mirror_axis_label` del original.
    FCCheckBox* film_mirror_cb{nullptr};  ///< `film_mirror_cb` del original.
    FCButton* film_object_button{nullptr};  ///< `film_object_button` del original.
    FCLabel* film_param_label{nullptr};  ///< `film_param_label` del original.
    FCCheckBox* film_scale_cb{nullptr};  ///< `film_scale_cb` del original.
    FCDoubleSpinner* film_scale_stroke_entry{nullptr};  ///< `film_scale_stroke_entry` del original.
    FCLabel* film_scale_stroke_label{nullptr};  ///< `film_scale_stroke_label` del original.
    FCDoubleSpinner* film_scalex_entry{nullptr};  ///< `film_scalex_entry` del original.
    FCLabel* film_scalex_label{nullptr};  ///< `film_scalex_label` del original.
    FCDoubleSpinner* film_scaley_entry{nullptr};  ///< `film_scaley_entry` del original.
    FCLabel* film_scaley_label{nullptr};  ///< `film_scaley_label` del original.
    FCCheckBox* film_skew_cb{nullptr};  ///< `film_skew_cb` del original.
    FCLabel* film_skew_ref_label{nullptr};  ///< `film_skew_ref_label` del original.
    RadioSet* film_skew_reference{nullptr};  ///< `film_skew_reference` del original.
    FCDoubleSpinner* film_skewx_entry{nullptr};  ///< `film_skewx_entry` del original.
    FCLabel* film_skewx_label{nullptr};  ///< `film_skewx_label` del original.
    FCDoubleSpinner* film_skewy_entry{nullptr};  ///< `film_skewy_entry` del original.
    FCLabel* film_skewy_label{nullptr};  ///< `film_skewy_label` del original.
    RadioSet* film_type{nullptr};  ///< `film_type` del original.
    FCLabel* film_type_label{nullptr};  ///< `film_type_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCLabel* orientation_label{nullptr};  ///< `orientation_label` del original.
    RadioSet* orientation_radio{nullptr};  ///< `orientation_radio` del original.
    FCComboBox* pagesize_combo{nullptr};  ///< `pagesize_combo` del original.
    FCLabel* pagesize_label{nullptr};  ///< `pagesize_label` del original.
    FCLabel* png_dpi_label{nullptr};  ///< `png_dpi_label` del original.
    FCSpinner* png_dpi_spinner{nullptr};  ///< `png_dpi_spinner` del original.
    FCCheckBox* punch_cb{nullptr};  ///< `punch_cb` del original.
    QFrame* punch_frame{nullptr};  ///< `punch_frame` del original.
    FCLabel* punch_size_label{nullptr};  ///< `punch_size_label` del original.
    FCDoubleSpinner* punch_size_spinner{nullptr};  ///< `punch_size_spinner` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCLabel* source_label{nullptr};  ///< `source_label` del original.
    RadioSet* source_punch{nullptr};  ///< `source_punch` del original.
    FCComboBox* tf_box_combo{nullptr};  ///< `tf_box_combo` del original.
    FCComboBox* tf_object_combo{nullptr};  ///< `tf_object_combo` del original.
    RadioSet* tf_type_box_combo{nullptr};  ///< `tf_type_box_combo` del original.
    FCLabel* tf_type_box_combo_label{nullptr};  ///< `tf_type_box_combo_label` del original.
    RadioSet* tf_type_obj_combo{nullptr};  ///< `tf_type_obj_combo` del original.
    FCLabel* tf_type_obj_combo_label{nullptr};  ///< `tf_type_obj_combo_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
