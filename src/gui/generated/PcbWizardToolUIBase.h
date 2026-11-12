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
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `PcbWizardToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class PcbWizardToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit PcbWizardToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* excellon_brn{nullptr};  ///< `excellon_brn` del original.
    FCLabel* excellon_label{nullptr};  ///< `excellon_label` del original.
    FCSpinner* frac_entry{nullptr};  ///< `frac_entry` del original.
    FCLabel* frac_label{nullptr};  ///< `frac_label` del original.
    QPushButton* import_button{nullptr};  ///< `import_button` del original.
    FCButton* inf_btn{nullptr};  ///< `inf_btn` del original.
    FCLabel* inf_label{nullptr};  ///< `inf_label` del original.
    FCSpinner* int_entry{nullptr};  ///< `int_entry` del original.
    FCLabel* int_label{nullptr};  ///< `int_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCTable* tools_table{nullptr};  ///< `tools_table` del original.
    FCLabel* units_label{nullptr};  ///< `units_label` del original.
    RadioSet* units_radio{nullptr};  ///< `units_radio` del original.
    FCLabel* zeros_label{nullptr};  ///< `zeros_label` del original.
    RadioSet* zeros_radio{nullptr};  ///< `zeros_radio` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
