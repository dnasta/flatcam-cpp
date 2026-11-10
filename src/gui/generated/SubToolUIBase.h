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
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"

namespace flatcam::gui {

/**
 * @brief Panel `SubToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class SubToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit SubToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCCheckBox* close_paths_cb{nullptr};  ///< `close_paths_cb` del original.
    FCCheckBox* delete_sources_cb{nullptr};  ///< `delete_sources_cb` del original.
    QLabel* geo_title{nullptr};  ///< `geo_title` del original.
    QLabel* gerber_title{nullptr};  ///< `gerber_title` del original.
    FCButton* intersect_btn{nullptr};  ///< `intersect_btn` del original.
    FCButton* intersect_geo_btn{nullptr};  ///< `intersect_geo_btn` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    QPushButton* reset_button{nullptr};  ///< `reset_button` del original.
    FCComboBox* sub_geo_combo{nullptr};  ///< `sub_geo_combo` del original.
    QLabel* sub_geo_label{nullptr};  ///< `sub_geo_label` del original.
    FCComboBox* sub_gerber_combo{nullptr};  ///< `sub_gerber_combo` del original.
    QLabel* sub_gerber_label{nullptr};  ///< `sub_gerber_label` del original.
    FCComboBox* target_geo_combo{nullptr};  ///< `target_geo_combo` del original.
    QLabel* target_geo_label{nullptr};  ///< `target_geo_label` del original.
    FCComboBox* target_gerber_combo{nullptr};  ///< `target_gerber_combo` del original.
    QLabel* target_gerber_label{nullptr};  ///< `target_gerber_label` del original.
    QVBoxLayout* tools_box{nullptr};  ///< `tools_box` del original.
    QFrame* tools_frame{nullptr};  ///< `tools_frame` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
