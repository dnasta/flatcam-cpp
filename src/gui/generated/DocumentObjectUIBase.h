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
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"

namespace flatcam::gui {

/**
 * @brief Panel `DocumentObjectUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class DocumentObjectUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit DocumentObjectUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QToolButton* al_center_tb{nullptr};  ///< `al_center_tb` del original.
    QToolButton* al_justify_tb{nullptr};  ///< `al_justify_tb` del original.
    QToolButton* al_left_tb{nullptr};  ///< `al_left_tb` del original.
    QToolButton* al_right_tb{nullptr};  ///< `al_right_tb` del original.
    FCLabel* alignment_label{nullptr};  ///< `alignment_label` del original.
    FCCheckBox* autocomplete_cb{nullptr};  ///< `autocomplete_cb` del original.
    QVBoxLayout* custom_box{nullptr};  ///< `custom_box` del original.
    QToolButton* font_bold_tb{nullptr};  ///< `font_bold_tb` del original.
    FCButton* font_color_button{nullptr};  ///< `font_color_button` del original.
    FCEntry* font_color_entry{nullptr};  ///< `font_color_entry` del original.
    FCLabel* font_color_label{nullptr};  ///< `font_color_label` del original.
    QToolButton* font_italic_tb{nullptr};  ///< `font_italic_tb` del original.
    FCComboBox* font_size_cb{nullptr};  ///< `font_size_cb` del original.
    FCLabel* font_size_label{nullptr};  ///< `font_size_label` del original.
    QFontComboBox* font_type_cb{nullptr};  ///< `font_type_cb` del original.
    FCLabel* font_type_label{nullptr};  ///< `font_type_label` del original.
    QToolButton* font_under_tb{nullptr};  ///< `font_under_tb` del original.
    QFormLayout* form_box{nullptr};  ///< `form_box` del original.
    QHBoxLayout* form_box_child_1{nullptr};  ///< `form_box_child_1` del original.
    QHBoxLayout* form_box_child_2{nullptr};  ///< `form_box_child_2` del original.
    FCLabel* icon{nullptr};  ///< `icon` del original.
    FCLabel* level{nullptr};  ///< `level` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    QHBoxLayout* name_hlay{nullptr};  ///< `name_hlay` del original.
    FCCheckBox* plot_cb{nullptr};  ///< `plot_cb` del original.
    FCButton* sel_color_button{nullptr};  ///< `sel_color_button` del original.
    FCEntry* sel_color_entry{nullptr};  ///< `sel_color_entry` del original.
    FCLabel* sel_color_label{nullptr};  ///< `sel_color_label` del original.
    FCLabel* tab_size_label{nullptr};  ///< `tab_size_label` del original.
    FCSpinner* tab_size_spinner{nullptr};  ///< `tab_size_spinner` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    FCLabel* title_label{nullptr};  ///< `title_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
