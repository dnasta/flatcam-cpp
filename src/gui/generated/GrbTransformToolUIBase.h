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
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/NumericalEval.h"

namespace flatcam::gui {

/**
 * @brief Panel `GrbTransformToolUI` de FlatCAM 2024.4 (generado desde la interfaz original).
 *
 * `setupUi()` arma los mismos widgets, textos, valores de fábrica y layouts que el original. Los miembros tienen
 * el nombre del atributo del original; la lógica va en una clase derivada.
 */
class GrbTransformToolUIBase : public QWidget {
public:
    /** @brief Crea el panel vacío. @param parent Widget padre. */
    explicit GrbTransformToolUIBase(QWidget* parent = nullptr) : QWidget(parent) {}

    /** @brief Construye el panel. */
    void setupUi();

    /** @name Objetos del panel (mismo nombre que en el original) */
    ///@{
    FCButton* buffer_button{nullptr};  ///< `buffer_button` del original.
    FCDoubleSpinner* buffer_entry{nullptr};  ///< `buffer_entry` del original.
    FCButton* buffer_factor_button{nullptr};  ///< `buffer_factor_button` del original.
    FCDoubleSpinner* buffer_factor_entry{nullptr};  ///< `buffer_factor_entry` del original.
    FCLabel* buffer_factor_label{nullptr};  ///< `buffer_factor_label` del original.
    FCLabel* buffer_label{nullptr};  ///< `buffer_label` del original.
    FCCheckBox* buffer_rounded_cb{nullptr};  ///< `buffer_rounded_cb` del original.
    FCButton* flipx_button{nullptr};  ///< `flipx_button` del original.
    FCButton* flipy_button{nullptr};  ///< `flipy_button` del original.
    QVBoxLayout* layout{nullptr};  ///< `layout` del original.
    FCButton* offx_button{nullptr};  ///< `offx_button` del original.
    FCDoubleSpinner* offx_entry{nullptr};  ///< `offx_entry` del original.
    FCLabel* offx_label{nullptr};  ///< `offx_label` del original.
    FCButton* offy_button{nullptr};  ///< `offy_button` del original.
    FCDoubleSpinner* offy_entry{nullptr};  ///< `offy_entry` del original.
    FCLabel* offy_label{nullptr};  ///< `offy_label` del original.
    FCButton* point_button{nullptr};  ///< `point_button` del original.
    NumericalEvalTupleEntry* point_entry{nullptr};  ///< `point_entry` del original.
    FCLabel* point_label{nullptr};  ///< `point_label` del original.
    FCComboBox* ref_combo{nullptr};  ///< `ref_combo` del original.
    FCButton* rotate_button{nullptr};  ///< `rotate_button` del original.
    FCDoubleSpinner* rotate_entry{nullptr};  ///< `rotate_entry` del original.
    FCLabel* rotate_label{nullptr};  ///< `rotate_label` del original.
    FCCheckBox* scale_link_cb{nullptr};  ///< `scale_link_cb` del original.
    FCButton* scalex_button{nullptr};  ///< `scalex_button` del original.
    FCDoubleSpinner* scalex_entry{nullptr};  ///< `scalex_entry` del original.
    FCLabel* scalex_label{nullptr};  ///< `scalex_label` del original.
    FCButton* scaley_button{nullptr};  ///< `scaley_button` del original.
    FCDoubleSpinner* scaley_entry{nullptr};  ///< `scaley_entry` del original.
    FCLabel* scaley_label{nullptr};  ///< `scaley_label` del original.
    FCCheckBox* skew_link_cb{nullptr};  ///< `skew_link_cb` del original.
    FCButton* skewx_button{nullptr};  ///< `skewx_button` del original.
    FCDoubleSpinner* skewx_entry{nullptr};  ///< `skewx_entry` del original.
    FCLabel* skewx_label{nullptr};  ///< `skewx_label` del original.
    FCButton* skewy_button{nullptr};  ///< `skewy_button` del original.
    FCDoubleSpinner* skewy_entry{nullptr};  ///< `skewy_entry` del original.
    FCLabel* skewy_label{nullptr};  ///< `skewy_label` del original.
    ///@}

    /** @brief Los miembros por su nombre (después de setupUi). @return Nombre -> objeto. */
    std::map<std::string, QObject*> membersByName() const;
};

}  // namespace flatcam::gui
