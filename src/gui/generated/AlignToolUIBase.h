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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `AlignToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class AlignToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit AlignToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCLabel* a_type_lbl{nullptr};  ///< `a_type_lbl` del original.
    RadioSet* a_type_radio{nullptr};  ///< `a_type_radio` del original.
    FCButton* align_object_button{nullptr};  ///< `align_object_button` del original.
    FCLabel* aligned_label{nullptr};  ///< `aligned_label` del original.
    FCComboBox* aligner_object_combo{nullptr};  ///< `aligner_object_combo` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCComboBox* object_combo{nullptr};  ///< `object_combo` del original.
    FCButton* reset_button{nullptr};  ///< `reset_button` del original.
    RadioSet* type_aligner_obj_radio{nullptr};  ///< `type_aligner_obj_radio` del original.
    RadioSet* type_obj_radio{nullptr};  ///< `type_obj_radio` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
