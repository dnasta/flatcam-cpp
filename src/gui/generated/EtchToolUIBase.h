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
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/NumericalEval.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

/**
 * @brief Panel `EtchToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class EtchToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit EtchToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* compensate_btn{nullptr};  ///< `compensate_btn` del original.
    FCComboBox* etchants_combo{nullptr};  ///< `etchants_combo` del original.
    QLabel* etchants_label{nullptr};  ///< `etchants_label` del original.
    NumericalEvalEntry* factor_entry{nullptr};  ///< `factor_entry` del original.
    QLabel* factor_label{nullptr};  ///< `factor_label` del original.
    FCComboBox* gerber_combo{nullptr};  ///< `gerber_combo` del original.
    QLabel* gerber_label{nullptr};  ///< `gerber_label` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    NumericalEvalEntry* mils_entry{nullptr};  ///< `mils_entry` del original.
    FCEntry* mils_to_um_entry{nullptr};  ///< `mils_to_um_entry` del original.
    QLabel* mils_um_label{nullptr};  ///< `mils_um_label` del original.
    FCDoubleSpinner* offset_entry{nullptr};  ///< `offset_entry` del original.
    QLabel* offset_label{nullptr};  ///< `offset_label` del original.
    NumericalEvalEntry* oz_entry{nullptr};  ///< `oz_entry` del original.
    FCEntry* oz_to_um_entry{nullptr};  ///< `oz_to_um_entry` del original.
    QLabel* oz_um_label{nullptr};  ///< `oz_um_label` del original.
    QLabel* param_label{nullptr};  ///< `param_label` del original.
    QLabel* ratio_label{nullptr};  ///< `ratio_label` del original.
    RadioSet* ratio_radio{nullptr};  ///< `ratio_radio` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCDoubleSpinner* thick_entry{nullptr};  ///< `thick_entry` del original.
    QLabel* thick_label{nullptr};  ///< `thick_label` del original.
    QVBoxLayout* tools_box{nullptr};  ///< `tools_box` del original.
    QFrame* tools_frame{nullptr};  ///< `tools_frame` del original.
    QLabel* util_label{nullptr};  ///< `util_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
