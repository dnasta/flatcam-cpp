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

namespace flatcam::gui {

/**
 * @brief Panel `AboutDialogUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class AboutDialogUIBase : public QDialog {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit AboutDialogUIBase(QWidget* parent = nullptr) : QDialog(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    QWidget* attributions_tab{nullptr};  ///< `attributions_tab` del original.
    QVBoxLayout* attributions_tab_layout{nullptr};  ///< `attributions_tab_layout` del original.
    QWidget* components_tab{nullptr};  ///< `components_tab` del original.
    QVBoxLayout* components_tab_layout{nullptr};  ///< `components_tab_layout` del original.
    QWidget* license_tab{nullptr};  ///< `license_tab` del original.
    QVBoxLayout* license_tab_layout{nullptr};  ///< `license_tab_layout` del original.
    QGridLayout* prog_grid_lay{nullptr};  ///< `prog_grid_lay` del original.
    QWidget* programmmers_tab{nullptr};  ///< `programmmers_tab` del original.
    QVBoxLayout* programmmers_tab_layout{nullptr};  ///< `programmmers_tab_layout` del original.
    QWidget* splash_tab{nullptr};  ///< `splash_tab` del original.
    QHBoxLayout* splash_tab_layout{nullptr};  ///< `splash_tab_layout` del original.
    QGridLayout* translator_grid_lay{nullptr};  ///< `translator_grid_lay` del original.
    QWidget* translators_tab{nullptr};  ///< `translators_tab` del original.
    QVBoxLayout* translators_tab_layout{nullptr};  ///< `translators_tab_layout` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
