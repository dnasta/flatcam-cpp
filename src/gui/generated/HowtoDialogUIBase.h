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

namespace flatcam::gui {

/**
 * @brief Panel `HowtoDialogUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class HowtoDialogUIBase : public QDialog {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit HowtoDialogUIBase(QWidget* parent = nullptr) : QDialog(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QGridLayout* grid_lay{nullptr};  ///< `grid_lay` del original.
    QHBoxLayout* howto_lay{nullptr};  ///< `howto_lay` del original.
    QWidget* howto_tab{nullptr};  ///< `howto_tab` del original.
    QVBoxLayout* howto_tab_layout{nullptr};  ///< `howto_tab_layout` del original.
    QWidget* intro_tab{nullptr};  ///< `intro_tab` del original.
    QHBoxLayout* intro_tab_layout{nullptr};  ///< `intro_tab_layout` del original.
    QHBoxLayout* links_lay{nullptr};  ///< `links_lay` del original.
    QWidget* links_tab{nullptr};  ///< `links_tab` del original.
    QVBoxLayout* links_tab_layout{nullptr};  ///< `links_tab_layout` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
