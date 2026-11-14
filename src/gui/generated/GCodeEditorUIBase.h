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
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCPrefWidgets.h"
#include "gui/widgets/FCTable.h"

namespace flatcam::gui {

/**
 * @brief Panel `GCodeEditorUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class GCodeEditorUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit GCodeEditorUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCTextArea* append_text{nullptr};  ///< `append_text` del original.
    FCTable* cnc_tools_table{nullptr};  ///< `cnc_tools_table` del original.
    QVBoxLayout* edit_box{nullptr};  ///< `edit_box` del original.
    QFrame* edit_frame{nullptr};  ///< `edit_frame` del original.
    FCTable* exc_cnc_tools_table{nullptr};  ///< `exc_cnc_tools_table` del original.
    FCButton* exit_editor_button{nullptr};  ///< `exit_editor_button` del original.
    QLabel* icon{nullptr};  ///< `icon` del original.
    QHBoxLayout* name_box{nullptr};  ///< `name_box` del original.
    FCEntry* name_entry{nullptr};  ///< `name_entry` del original.
    FCTextArea* prepend_text{nullptr};  ///< `prepend_text` del original.
    QHBoxLayout* title_box{nullptr};  ///< `title_box` del original.
    QLabel* title_label{nullptr};  ///< `title_label` del original.
    FCButton* update_gcode_button{nullptr};  ///< `update_gcode_button` del original.
    FCButton* update_gcode_sec_button{nullptr};  ///< `update_gcode_sec_button` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
