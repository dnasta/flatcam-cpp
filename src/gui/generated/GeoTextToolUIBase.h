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
#include "gui/widgets/FCLabel.h"

namespace flatcam::gui {

/**
 * @brief Panel `GeoTextToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class GeoTextToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit GeoTextToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* apply_button{nullptr};  ///< `apply_button` del original.
    QToolButton* font_bold_tb{nullptr};  ///< `font_bold_tb` del original.
    QToolButton* font_italic_tb{nullptr};  ///< `font_italic_tb` del original.
    FCComboBox* font_size_cb{nullptr};  ///< `font_size_cb` del original.
    QFontComboBox* font_type_cb{nullptr};  ///< `font_type_cb` del original.
    QFormLayout* form_layout{nullptr};  ///< `form_layout` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    QTextEdit* text_input_entry{nullptr};  ///< `text_input_entry` del original.
    QFrame* text_tool_frame{nullptr};  ///< `text_tool_frame` del original.
    QVBoxLayout* text_tools_box{nullptr};  ///< `text_tools_box` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
