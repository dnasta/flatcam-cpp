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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `ImageToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class ImageToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit ImageToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QLabel* detail_label{nullptr};  ///< `detail_label` del original.
    FCSpinner* dpi_entry{nullptr};  ///< `dpi_entry` del original.
    QLabel* dpi_label{nullptr};  ///< `dpi_label` del original.
    QLabel* emty_lbl{nullptr};  ///< `emty_lbl` del original.
    RadioSet* image_type{nullptr};  ///< `image_type` del original.
    QLabel* image_type_label{nullptr};  ///< `image_type_label` del original.
    QPushButton* import_button{nullptr};  ///< `import_button` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCSpinner* mask_b_entry{nullptr};  ///< `mask_b_entry` del original.
    QLabel* mask_b_label{nullptr};  ///< `mask_b_label` del original.
    FCSpinner* mask_bw_entry{nullptr};  ///< `mask_bw_entry` del original.
    QLabel* mask_bw_label{nullptr};  ///< `mask_bw_label` del original.
    FCSpinner* mask_g_entry{nullptr};  ///< `mask_g_entry` del original.
    QLabel* mask_g_label{nullptr};  ///< `mask_g_label` del original.
    FCSpinner* mask_r_entry{nullptr};  ///< `mask_r_entry` del original.
    QLabel* mask_r_label{nullptr};  ///< `mask_r_label` del original.
    FCComboBox* tf_type_obj_combo{nullptr};  ///< `tf_type_obj_combo` del original.
    QLabel* tf_type_obj_combo_label{nullptr};  ///< `tf_type_obj_combo_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
