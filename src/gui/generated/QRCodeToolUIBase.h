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
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCPrefWidgets.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `QRCodeToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class QRCodeToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit QRCodeToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QPushButton* back_color_button{nullptr};  ///< `back_color_button` del original.
    FCEntry* back_color_entry{nullptr};  ///< `back_color_entry` del original.
    QLabel* back_color_label{nullptr};  ///< `back_color_label` del original.
    QLabel* bb_label{nullptr};  ///< `bb_label` del original.
    RadioSet* bb_radio{nullptr};  ///< `bb_radio` del original.
    FCSpinner* border_size_entry{nullptr};  ///< `border_size_entry` del original.
    QLabel* border_size_label{nullptr};  ///< `border_size_label` del original.
    FCSpinner* bsize_entry{nullptr};  ///< `bsize_entry` del original.
    QLabel* bsize_label{nullptr};  ///< `bsize_label` del original.
    QLabel* error_label{nullptr};  ///< `error_label` del original.
    RadioSet* error_radio{nullptr};  ///< `error_radio` del original.
    FCCheckBox* export_cb{nullptr};  ///< `export_cb` del original.
    QFrame* export_frame{nullptr};  ///< `export_frame` del original.
    QGridLayout* export_lay{nullptr};  ///< `export_lay` del original.
    QPushButton* export_png_button{nullptr};  ///< `export_png_button` del original.
    QPushButton* export_svg_button{nullptr};  ///< `export_svg_button` del original.
    QPushButton* fill_color_button{nullptr};  ///< `fill_color_button` del original.
    FCEntry* fill_color_entry{nullptr};  ///< `fill_color_entry` del original.
    QLabel* fill_color_label{nullptr};  ///< `fill_color_label` del original.
    FCComboBox* grb_object_combo{nullptr};  ///< `grb_object_combo` del original.
    QLabel* grbobj_label{nullptr};  ///< `grbobj_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    QLabel* pol_label{nullptr};  ///< `pol_label` del original.
    RadioSet* pol_radio{nullptr};  ///< `pol_radio` del original.
    QPushButton* qrcode_button{nullptr};  ///< `qrcode_button` del original.
    QLabel* qrcode_label{nullptr};  ///< `qrcode_label` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCTextArea* text_data{nullptr};  ///< `text_data` del original.
    QLabel* text_label{nullptr};  ///< `text_label` del original.
    FCCheckBox* transparent_cb{nullptr};  ///< `transparent_cb` del original.
    FCSpinner* version_entry{nullptr};  ///< `version_entry` del original.
    QLabel* version_label{nullptr};  ///< `version_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
