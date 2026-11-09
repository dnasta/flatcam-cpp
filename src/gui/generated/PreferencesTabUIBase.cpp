// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PreferencesTabUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void PreferencesTabUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setProperty("objectName", QString::fromUtf8("preferences_tab"));
    pref_tab_layout = new QVBoxLayout();
    pref_tab_layout->setContentsMargins(2, 2, 2, 2);
    pref_tab_layout->setSpacing(6);
    pref_tab_area = new FCTab(nullptr);
    pref_tab_area->setTabsClosable(false);
    pref_tab_area->tabBar()->setDrawBase(false);
    pref_tab_area->tabBar()->setProperty("objectName", QString::fromUtf8("qt_tabwidget_tabbar"));
    pref_tab_area->tabBar()->setStyleSheet(QString::fromUtf8("QTabBar::tab{min-width:90px;}"));
    general_tab = new QWidget(nullptr);
    general_tab->setProperty("objectName", QString::fromUtf8("general_tab"));
    general_tab_lay = new QVBoxLayout();
    general_tab_lay->setContentsMargins(2, 2, 2, 2);
    general_tab_lay->setSpacing(6);
    hlay1 = new QHBoxLayout();
    hlay1->setContentsMargins(0, 0, 0, 0);
    hlay1->setSpacing(6);
    hlay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    general_tab_lay->addLayout(hlay1, 0);
    general_scroll_area = new QScrollArea(nullptr);
    general_defaults_form = new QWidget(nullptr);
    general_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    general_app_group = new QGroupBox(nullptr);
    { QFont f = general_app_group->font(); f.setBold(true); f.setPixelSize(16); general_app_group->setFont(f); }
    general_app_group->setMinimumSize(QSize(250, 0));
    general_app_group_layout = new QVBoxLayout();
    general_app_group_layout->setContentsMargins(9, 9, 9, 9);
    general_app_group_layout->setSpacing(6);
    auto* lay1 = new QGridLayout();
    lay1->setContentsMargins(0, 0, 0, 0);
    lay1->setHorizontalSpacing(6);
    lay1->setVerticalSpacing(6);
    general_app_group_unitslabel = new QLabel(nullptr);
    general_app_group_unitslabel->setText(QString::fromUtf8("<span style=\"color:red;\"><b>Unidades:</b></span>"));
    general_app_group_unitslabel->setToolTip(QString::fromUtf8("El valor por defecto para las unidades FlatCAM.\nLo que se selecciona aquí se establece cada vez\nSe inicia FLatCAM."));
    lay1->addWidget(general_app_group_unitslabel, 0, 0, 1, 1);
    general_app_group_units_radio = new RadioSet({{QString::fromUtf8("MM"), QVariant(QString::fromUtf8("MM"))}, {QString::fromUtf8("IN"), QVariant(QString::fromUtf8("IN"))}}, Qt::Horizontal, true, nullptr);
    general_app_group_units_radio->setValue(QVariant(QString::fromUtf8("MM")));
    lay1->addWidget(general_app_group_units_radio, 0, 1, 1, 1);
    general_app_group_precision_metric_label = new QLabel(nullptr);
    general_app_group_precision_metric_label->setText(QString::fromUtf8("Precisión MM:"));
    general_app_group_precision_metric_label->setToolTip(QString::fromUtf8("El número de decimales utilizados en toda la aplicación.\ncuando las unidades configuradas están en el sistema METRIC.\nCualquier cambio aquí requiere un reinicio de la aplicación."));
    lay1->addWidget(general_app_group_precision_metric_label, 1, 0, 1, 1);
    general_app_group_precision_metric_entry = new FCSpinner(nullptr);
    general_app_group_precision_metric_entry->setProperty("maximum", 16);
    general_app_group_precision_metric_entry->setProperty("minimum", 2);
    general_app_group_precision_metric_entry->setProperty("value", 4);
    general_app_group_precision_metric_entry->setWrapping(true);
    lay1->addWidget(general_app_group_precision_metric_entry, 1, 1, 1, 1);
    general_app_group_precision_inch_label = new QLabel(nullptr);
    general_app_group_precision_inch_label->setText(QString::fromUtf8("Precisión Pulgadas:"));
    general_app_group_precision_inch_label->setToolTip(QString::fromUtf8("El número de decimales utilizados en toda la aplicación.\ncuando las unidades configuradas están en el sistema PULGADA.\nCualquier cambio aquí requiere un reinicio de la aplicación."));
    lay1->addWidget(general_app_group_precision_inch_label, 2, 0, 1, 1);
    general_app_group_precision_inch_entry = new FCSpinner(nullptr);
    general_app_group_precision_inch_entry->setProperty("maximum", 16);
    general_app_group_precision_inch_entry->setProperty("minimum", 2);
    general_app_group_precision_inch_entry->setProperty("value", 4);
    general_app_group_precision_inch_entry->setWrapping(true);
    lay1->addWidget(general_app_group_precision_inch_entry, 2, 1, 1, 1);
    general_app_group_ge_label = new QLabel(nullptr);
    general_app_group_ge_label->setText(QString::fromUtf8("<b>Motor gráfico:</b>"));
    general_app_group_ge_label->setToolTip(QString::fromUtf8("Elija qué motor gráfico usar en FlatCAM.\nLegacy (2D) -> funcionalidad reducida, rendimiento lento pero compatibilidad mejorada.\nOpenGL (3D) -> funcionalidad completa, alto rendimiento\nAlgunas tarjetas gráficas son demasiado viejas y no funcionan en modo OpenGL (3D), como:\nIntel HD3000 o anterior. En este caso, el área de trazado será negra, por lo tanto\nuse el modo Legacy (2D)."));
    lay1->addWidget(general_app_group_ge_label, 3, 0, 1, 1);
    general_app_group_ge_radio = new RadioSet({{QString::fromUtf8("Legado (2D)"), QVariant(QString::fromUtf8("2D"))}, {QString::fromUtf8("OpenGL(3D)"), QVariant(QString::fromUtf8("3D"))}}, Qt::Vertical, true, nullptr);
    general_app_group_ge_radio->setValue(QVariant(QString::fromUtf8("3D")));
    lay1->addWidget(general_app_group_ge_radio, 3, 1, 1, 1);
    auto* w2 = new QFrame(nullptr);
    w2->setProperty("frameShadow", 48);
    w2->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w2->setSizePolicy(sp); }
    lay1->addWidget(w2, 4, 0, 1, 2);
    general_app_group_app_level_label = new QLabel(nullptr);
    general_app_group_app_level_label->setText(QString::fromUtf8("<span style=\"color:red;\"><b>NIVEL DE APLICACIÓN:</b></span>"));
    general_app_group_app_level_label->setToolTip(QString::fromUtf8("Elija el nivel de uso predeterminado para FlatCAM.\nNivel BÁSICO -> funcionalidad reducida, mejor para principiantes.\nNivel AVANZADO -> Funcionalidad completa.\n\nLa elección aquí influirá en los parámetros en\nLa pestaña seleccionada para todo tipo de objetos FlatCAM."));
    lay1->addWidget(general_app_group_app_level_label, 5, 0, 1, 2);
    general_app_group_app_level_radio = new RadioSet({{QString::fromUtf8("BASIC"), QVariant(QString::fromUtf8("b"))}, {QString::fromUtf8("Avanzado"), QVariant(QString::fromUtf8("a"))}}, Qt::Horizontal, true, nullptr);
    general_app_group_app_level_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay1->addWidget(general_app_group_app_level_radio, 6, 0, 1, 2);
    general_app_group_portability_cb = new FCCheckBox(nullptr);
    general_app_group_portability_cb->setText(QString::fromUtf8("Aplicación portátil"));
    general_app_group_portability_cb->setToolTip(QString::fromUtf8("Elija si la aplicación debe ejecutarse como portátil.\n\nSi está marcada, la aplicación se ejecutará portátil,\nlo que significa que los archivos de preferencias se guardarán\nen la carpeta de la aplicación, en la subcarpeta lib \\ config."));
    general_app_group_portability_cb->setHidden(true);
    lay1->addWidget(general_app_group_portability_cb, 7, 0, 1, 2);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay1->addWidget(w3, 8, 0, 1, 2);
    general_app_group_languagelabel = new QLabel(nullptr);
    general_app_group_languagelabel->setText(QString::fromUtf8("<b>Idiomas</b>"));
    general_app_group_languagelabel->setToolTip(QString::fromUtf8("Establezca el idioma utilizado en FlatCAM."));
    lay1->addWidget(general_app_group_languagelabel, 9, 0, 1, 2);
    general_app_group_language_cb = new FCComboBox(nullptr);
    lay1->addWidget(general_app_group_language_cb, 10, 0, 1, 2);
    general_app_group_language_apply_btn = new FCButton(nullptr);
    general_app_group_language_apply_btn->setText(QString::fromUtf8("Aplicar idioma"));
    general_app_group_language_apply_btn->setToolTip(QString::fromUtf8("Establezca el idioma utilizado en FlatCAM.\nLa aplicación se reiniciará después de hacer clic."));
    lay1->addWidget(general_app_group_language_apply_btn, 15, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay1->addWidget(w4, 16, 0, 1, 2);
    general_app_group_startup_label = new QLabel(nullptr);
    general_app_group_startup_label->setText(QString::fromUtf8("<b>Configuraciones de inicio</b>"));
    lay1->addWidget(general_app_group_startup_label, 17, 0, 1, 2);
    general_app_group_splash_cb = new FCCheckBox(nullptr);
    general_app_group_splash_cb->setText(QString::fromUtf8("Pantalla de bienvenida"));
    general_app_group_splash_cb->setToolTip(QString::fromUtf8("Habilite la visualización de la pantalla de inicio al iniciar la aplicación."));
    lay1->addWidget(general_app_group_splash_cb, 18, 0, 1, 2);
    general_app_group_systray_cb = new FCCheckBox(nullptr);
    general_app_group_systray_cb->setChecked(true);
    general_app_group_systray_cb->setText(QString::fromUtf8("Icono de la Sys Tray"));
    general_app_group_systray_cb->setToolTip(QString::fromUtf8("Habilite la visualización del icono de FlatCAM en la bandeja del sistema."));
    lay1->addWidget(general_app_group_systray_cb, 19, 0, 1, 2);
    general_app_group_shell_startup_cb = new FCCheckBox(nullptr);
    general_app_group_shell_startup_cb->setText(QString::fromUtf8("Mostrar la línea de comando"));
    general_app_group_shell_startup_cb->setToolTip(QString::fromUtf8("Marque esta casilla si desea que el shell\niniciar automáticamente en el inicio."));
    lay1->addWidget(general_app_group_shell_startup_cb, 20, 0, 1, 2);
    general_app_group_project_startup_cb = new FCCheckBox(nullptr);
    general_app_group_project_startup_cb->setText(QString::fromUtf8("Mostrar proyecto"));
    general_app_group_project_startup_cb->setToolTip(QString::fromUtf8("Marque esta casilla si desea que el área de la pestaña del proyecto / seleccionado / herramienta\npara ser mostrado automáticamente en el inicio."));
    lay1->addWidget(general_app_group_project_startup_cb, 21, 0, 1, 2);
    general_app_group_version_check_cb = new FCCheckBox(nullptr);
    general_app_group_version_check_cb->setText(QString::fromUtf8("Verificación de versión"));
    general_app_group_version_check_cb->setToolTip(QString::fromUtf8("Marque esta casilla si desea marcar\npara una nueva versión automáticamente en el inicio."));
    lay1->addWidget(general_app_group_version_check_cb, 22, 0, 1, 2);
    general_app_group_send_stats_cb = new FCCheckBox(nullptr);
    general_app_group_send_stats_cb->setEnabled(false);
    general_app_group_send_stats_cb->setText(QString::fromUtf8("Enviar estadísticas"));
    general_app_group_send_stats_cb->setToolTip(QString::fromUtf8("Marque esta casilla si acepta enviar anónimo\nEstadísticas automáticamente en el inicio, para ayudar a mejorar FlatCAM."));
    lay1->addWidget(general_app_group_send_stats_cb, 23, 0, 1, 2);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay1->addWidget(w5, 24, 0, 1, 2);
    general_app_group_worker_number_label = new QLabel(nullptr);
    general_app_group_worker_number_label->setText(QString::fromUtf8("Número de trabajadores:"));
    general_app_group_worker_number_label->setToolTip(QString::fromUtf8("El número de Qthreads disponibles para la aplicación.\nUn número más grande puede terminar los trabajos más rápidamente pero\nDependiendo de la velocidad de su computadora, podrá realizar la aplicación.\ninsensible. Puede tener un valor entre 2 y 16.\nEl valor predeterminado es 2.\nDespués del cambio, se aplicará en el próximo inicio de la aplicación."));
    lay1->addWidget(general_app_group_worker_number_label, 25, 0, 1, 1);
    general_app_group_worker_number_sb = new FCSpinner(nullptr);
    general_app_group_worker_number_sb->setProperty("maximum", 16);
    general_app_group_worker_number_sb->setProperty("minimum", 2);
    general_app_group_worker_number_sb->setProperty("value", 5);
    lay1->addWidget(general_app_group_worker_number_sb, 25, 1, 1, 1);
    auto* w6 = new QLabel(nullptr);
    w6->setText(QString::fromUtf8("Geo Tolerancia:"));
    w6->setToolTip(QString::fromUtf8("Este valor puede contrarrestar el efecto de los Pasos circulares\nparámetro. El valor predeterminado es 0.005.\nUn valor más bajo aumentará el detalle tanto en la imagen\ny en Gcode para los círculos, con un mayor costo en\nactuación. Un valor más alto proporcionará más\nrendimiento a expensas del nivel de detalle."));
    lay1->addWidget(w6, 26, 0, 1, 1);
    general_app_group_tol_entry = new FCDoubleSpinner(nullptr);
    general_app_group_tol_entry->setProperty("decimals", 6);
    general_app_group_tol_entry->setProperty("singleStep", 0.001);
    general_app_group_tol_entry->setProperty("value", 0.005);
    lay1->addWidget(general_app_group_tol_entry, 26, 1, 1, 1);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay1->addWidget(w7, 27, 0, 1, 2);
    general_app_group_save_label = new QLabel(nullptr);
    general_app_group_save_label->setText(QString::fromUtf8("<b>Configuraciones para guardar</b>"));
    lay1->addWidget(general_app_group_save_label, 28, 0, 1, 2);
    general_app_group_save_type_cb = new FCCheckBox(nullptr);
    general_app_group_save_type_cb->setChecked(true);
    general_app_group_save_type_cb->setText(QString::fromUtf8("Guardar proyecto comprimido"));
    general_app_group_save_type_cb->setToolTip(QString::fromUtf8("Ya sea para guardar un proyecto comprimido o sin comprimir.\nCuando esté marcado, guardará un proyecto comprimido de FlatCAM."));
    lay1->addWidget(general_app_group_save_type_cb, 29, 0, 1, 2);
    general_app_group_compress_label = new QLabel(nullptr);
    general_app_group_compress_label->setText(QString::fromUtf8("Compresión:"));
    general_app_group_compress_label->setToolTip(QString::fromUtf8("El nivel de compresión utilizado al guardar\nUn proyecto FlatCAM. Un valor más alto significa una mejor compresión\npero requieren más uso de RAM y más tiempo de procesamiento."));
    lay1->addWidget(general_app_group_compress_label, 30, 0, 1, 1);
    general_app_group_compress_spinner = new FCSpinner(nullptr);
    general_app_group_compress_spinner->setProperty("maximum", 9);
    general_app_group_compress_spinner->setProperty("value", 3);
    lay1->addWidget(general_app_group_compress_spinner, 30, 1, 1, 1);
    general_app_group_autosave_cb = new FCCheckBox(nullptr);
    general_app_group_autosave_cb->setText(QString::fromUtf8("Habilitar guardado auto"));
    general_app_group_autosave_cb->setToolTip(QString::fromUtf8("Marque para habilitar la función de autoguardado.\nCuando está habilitada, la aplicación intentará guardar un proyecto.\nen el intervalo establecido."));
    lay1->addWidget(general_app_group_autosave_cb, 31, 0, 1, 2);
    general_app_group_autosave_label = new QLabel(nullptr);
    general_app_group_autosave_label->setText(QString::fromUtf8("Intervalo:"));
    general_app_group_autosave_label->setToolTip(QString::fromUtf8("Intervalo de tiempo para guardar automáticamente. En milisegundos\nLa aplicación intentará guardar periódicamente pero solo\nsi el proyecto se guardó manualmente al menos una vez.\nMientras está activo, algunas operaciones pueden bloquear esta función."));
    lay1->addWidget(general_app_group_autosave_label, 32, 0, 1, 1);
    general_app_group_autosave_entry = new FCSpinner(nullptr);
    general_app_group_autosave_entry->setProperty("maximum", 9999999);
    general_app_group_autosave_entry->setProperty("value", 300000);
    lay1->addWidget(general_app_group_autosave_entry, 32, 1, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay1->addWidget(w8, 33, 0, 1, 2);
    general_app_group_pdf_param_label = new QLabel(nullptr);
    general_app_group_pdf_param_label->setText(QString::fromUtf8("<B>Parámetros de texto a PDF:</b>"));
    general_app_group_pdf_param_label->setToolTip(QString::fromUtf8("Se utiliza al guardar texto en el Editor de código o en objetos de documento FlatCAM."));
    lay1->addWidget(general_app_group_pdf_param_label, 34, 0, 1, 2);
    general_app_group_tmargin_label = new QLabel(nullptr);
    general_app_group_tmargin_label->setText(QString::fromUtf8("Margen superior:"));
    general_app_group_tmargin_label->setToolTip(QString::fromUtf8("Distancia entre el cuerpo del texto y la parte superior del archivo PDF."));
    lay1->addWidget(general_app_group_tmargin_label, 35, 0, 1, 1);
    general_app_group_tmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_tmargin_entry->setProperty("decimals", 4);
    general_app_group_tmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_tmargin_entry->setProperty("value", 15.0);
    lay1->addWidget(general_app_group_tmargin_entry, 35, 1, 1, 1);
    general_app_group_bmargin_label = new QLabel(nullptr);
    general_app_group_bmargin_label->setText(QString::fromUtf8("Margen inferior:"));
    general_app_group_bmargin_label->setToolTip(QString::fromUtf8("Distancia entre el cuerpo del texto y la parte inferior del archivo PDF."));
    lay1->addWidget(general_app_group_bmargin_label, 36, 0, 1, 1);
    general_app_group_bmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_bmargin_entry->setProperty("decimals", 4);
    general_app_group_bmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_bmargin_entry->setProperty("value", 10.0);
    lay1->addWidget(general_app_group_bmargin_entry, 36, 1, 1, 1);
    general_app_group_lmargin_label = new QLabel(nullptr);
    general_app_group_lmargin_label->setText(QString::fromUtf8("Margen izquierdo:"));
    general_app_group_lmargin_label->setToolTip(QString::fromUtf8("Distancia entre el cuerpo del texto y la izquierda del archivo PDF."));
    lay1->addWidget(general_app_group_lmargin_label, 37, 0, 1, 1);
    general_app_group_lmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_lmargin_entry->setProperty("decimals", 4);
    general_app_group_lmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_lmargin_entry->setProperty("value", 20.0);
    lay1->addWidget(general_app_group_lmargin_entry, 37, 1, 1, 1);
    general_app_group_rmargin_label = new QLabel(nullptr);
    general_app_group_rmargin_label->setText(QString::fromUtf8("Margen derecho:"));
    general_app_group_rmargin_label->setToolTip(QString::fromUtf8("Distancia entre el cuerpo del texto y la derecha del archivo PDF."));
    lay1->addWidget(general_app_group_rmargin_label, 38, 0, 1, 1);
    general_app_group_rmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_rmargin_entry->setProperty("decimals", 4);
    general_app_group_rmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_rmargin_entry->setProperty("value", 20.0);
    lay1->addWidget(general_app_group_rmargin_entry, 38, 1, 1, 1);
    lay1->setColumnStretch(1, 1);
    general_app_group_layout->addLayout(lay1, 0);
    general_app_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    general_app_group->setLayout(general_app_group_layout);
    layout->addWidget(general_app_group, 0);
    general_gui_group = new QGroupBox(nullptr);
    { QFont f = general_gui_group->font(); f.setBold(true); f.setPixelSize(16); general_gui_group->setFont(f); }
    general_gui_group->setMinimumSize(QSize(250, 0));
    general_gui_group_layout = new QVBoxLayout();
    general_gui_group_layout->setContentsMargins(9, 9, 9, 9);
    general_gui_group_layout->setSpacing(6);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    general_gui_group_theme_label = new QLabel(nullptr);
    general_gui_group_theme_label->setText(QString::fromUtf8("Tema:"));
    general_gui_group_theme_label->setToolTip(QString::fromUtf8("Seleccione un tema para la aplicación.\nTematizará el área de la trama."));
    lay9->addWidget(general_gui_group_theme_label, 0, 0, 1, 1);
    general_gui_group_theme_radio = new RadioSet({{QString::fromUtf8("Ligera"), QVariant(QString::fromUtf8("white"))}, {QString::fromUtf8("Oscuro"), QVariant(QString::fromUtf8("black"))}}, Qt::Vertical, true, nullptr);
    general_gui_group_theme_radio->setValue(QVariant(QString::fromUtf8("black")));
    lay9->addWidget(general_gui_group_theme_radio, 0, 1, 1, 1);
    general_gui_group_gray_icons_cb = new FCCheckBox(nullptr);
    general_gui_group_gray_icons_cb->setChecked(true);
    general_gui_group_gray_icons_cb->setText(QString::fromUtf8("Use iconos grises"));
    general_gui_group_gray_icons_cb->setToolTip(QString::fromUtf8("Marque esta casilla para usar un conjunto de iconos con\nun color más claro (gris). Para ser utilizado cuando un\nSe aplica el tema oscuro completo."));
    lay9->addWidget(general_gui_group_gray_icons_cb, 1, 0, 1, 3);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay9->addWidget(w10, 3, 0, 1, 2);
    general_gui_group_layout_label = new QLabel(nullptr);
    general_gui_group_layout_label->setText(QString::fromUtf8("Diseño:"));
    general_gui_group_layout_label->setToolTip(QString::fromUtf8("Seleccione un diseño para la aplicación.\nSe aplica de inmediato."));
    lay9->addWidget(general_gui_group_layout_label, 4, 0, 1, 1);
    general_gui_group_layout_combo = new FCComboBox(nullptr);
    general_gui_group_layout_combo->setProperty("currentText", QString::fromUtf8("compact"));
    general_gui_group_layout_combo->addItem(QString::fromUtf8("standard"));
    general_gui_group_layout_combo->addItem(QString::fromUtf8("compact"));
    general_gui_group_layout_combo->addItem(QString::fromUtf8("minimal"));
    general_gui_group_layout_combo->setCurrentIndex(1);
    lay9->addWidget(general_gui_group_layout_combo, 4, 1, 1, 1);
    general_gui_group_style_label = new QLabel(nullptr);
    general_gui_group_style_label->setText(QString::fromUtf8("Estilo:"));
    general_gui_group_style_label->setToolTip(QString::fromUtf8("Seleccione un estilo para la aplicación.\nSe aplicará en el próximo inicio de la aplicación."));
    lay9->addWidget(general_gui_group_style_label, 5, 0, 1, 1);
    general_gui_group_style_combo = new FCComboBox(nullptr);
    general_gui_group_style_combo->setProperty("currentText", QString::fromUtf8("Fusion"));
    general_gui_group_style_combo->addItem(QString::fromUtf8("macintosh"));
    general_gui_group_style_combo->addItem(QString::fromUtf8("Windows"));
    general_gui_group_style_combo->addItem(QString::fromUtf8("Fusion"));
    general_gui_group_style_combo->setCurrentIndex(2);
    lay9->addWidget(general_gui_group_style_combo, 5, 1, 1, 1);
    general_gui_group_hdpi_cb = new FCCheckBox(nullptr);
    general_gui_group_hdpi_cb->setText(QString::fromUtf8("Soporte HDPI"));
    general_gui_group_hdpi_cb->setToolTip(QString::fromUtf8("Habilite la compatibilidad con High DPI para la aplicación.\nSe aplicará en el próximo inicio de la aplicación."));
    lay9->addWidget(general_gui_group_hdpi_cb, 6, 0, 1, 3);
    general_gui_group_hover_cb = new FCCheckBox(nullptr);
    general_gui_group_hover_cb->setText(QString::fromUtf8("Forma flotante"));
    general_gui_group_hover_cb->setToolTip(QString::fromUtf8("Habilite la visualización de una forma flotante para los objetos de la aplicación.\nSe muestra cada vez que el cursor del mouse está flotando\nsobre cualquier tipo de objeto no seleccionado."));
    lay9->addWidget(general_gui_group_hover_cb, 8, 0, 1, 3);
    general_gui_group_selection_cb = new FCCheckBox(nullptr);
    general_gui_group_selection_cb->setChecked(true);
    general_gui_group_selection_cb->setText(QString::fromUtf8("Forma de selección"));
    general_gui_group_selection_cb->setToolTip(QString::fromUtf8("Habilite la visualización de una forma de selección para los objetos de la aplicación.\nSe muestra cada vez que el mouse selecciona un objeto\nya sea haciendo clic o arrastrando el mouse de izquierda a derecha o\nDe derecha a izquierda."));
    lay9->addWidget(general_gui_group_selection_cb, 9, 0, 1, 3);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    lay9->addWidget(w11, 14, 0, 1, 2);
    general_gui_group_sel_lr_label = new QLabel(nullptr);
    general_gui_group_sel_lr_label->setText(QString::fromUtf8("<b>Color de selección izquierda-derecha</b>"));
    lay9->addWidget(general_gui_group_sel_lr_label, 15, 0, 1, 2);
    general_gui_group_sl_color_label = new QLabel(nullptr);
    general_gui_group_sl_color_label->setText(QString::fromUtf8("Contorno:"));
    general_gui_group_sl_color_label->setToolTip(QString::fromUtf8("Establezca el color de línea para el cuadro de selección 'de izquierda a derecha'."));
    lay9->addWidget(general_gui_group_sl_color_label, 16, 0, 1, 1);
    general_gui_group_sl_color_entry = new FCColorEntry(nullptr);
    general_gui_group_sl_color_entry->setValue(QString::fromUtf8("#0000ffbf"));
    lay9->addWidget(general_gui_group_sl_color_entry, 16, 1, 1, 1);
    general_gui_group_sf_color_label = new QLabel(nullptr);
    general_gui_group_sf_color_label->setText(QString::fromUtf8("Llenado:"));
    general_gui_group_sf_color_label->setToolTip(QString::fromUtf8("Establecer el color de relleno para el cuadro de selección\nEn caso de que la selección se realice de izquierda a derecha.\nLos primeros 6 dígitos son el color y los 2 últimos.\nLos dígitos son para el nivel alfa (transparencia)."));
    lay9->addWidget(general_gui_group_sf_color_label, 17, 0, 1, 1);
    general_gui_group_sf_color_entry = new FCColorEntry(nullptr);
    general_gui_group_sf_color_entry->setValue(QString::fromUtf8("#a5a5ffbf"));
    lay9->addWidget(general_gui_group_sf_color_entry, 17, 1, 1, 1);
    general_gui_group_left_right_alpha_label = new QLabel(nullptr);
    general_gui_group_left_right_alpha_label->setText(QString::fromUtf8("Alfa:"));
    general_gui_group_left_right_alpha_label->setToolTip(QString::fromUtf8("Establezca la transparencia de relleno para el cuadro de selección 'de izquierda a derecha'."));
    lay9->addWidget(general_gui_group_left_right_alpha_label, 18, 0, 1, 1);
    general_gui_group_left_right_alpha_entry = new FCSliderWithSpinner(0, 255, 1, nullptr);
    general_gui_group_left_right_alpha_entry->setValue(191);
    lay9->addWidget(general_gui_group_left_right_alpha_entry, 18, 1, 1, 1);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    lay9->addWidget(w12, 19, 0, 1, 2);
    general_gui_group_sel_rl_label = new QLabel(nullptr);
    general_gui_group_sel_rl_label->setText(QString::fromUtf8("<b>Color de selección derecha-izquierda</b>"));
    lay9->addWidget(general_gui_group_sel_rl_label, 20, 0, 1, 2);
    general_gui_group_alt_sl_color_label = new QLabel(nullptr);
    general_gui_group_alt_sl_color_label->setText(QString::fromUtf8("Contorno:"));
    general_gui_group_alt_sl_color_label->setToolTip(QString::fromUtf8("Establezca el color de línea para el cuadro de selección 'de derecha a izquierda'."));
    lay9->addWidget(general_gui_group_alt_sl_color_label, 21, 0, 1, 1);
    general_gui_group_alt_sl_color_entry = new FCColorEntry(nullptr);
    general_gui_group_alt_sl_color_entry->setValue(QString::fromUtf8("#006E20bf"));
    lay9->addWidget(general_gui_group_alt_sl_color_entry, 21, 1, 1, 1);
    general_gui_group_alt_sf_color_label = new QLabel(nullptr);
    general_gui_group_alt_sf_color_label->setText(QString::fromUtf8("Llenado:"));
    general_gui_group_alt_sf_color_label->setToolTip(QString::fromUtf8("Establecer el color de la forma."));
    lay9->addWidget(general_gui_group_alt_sf_color_label, 22, 0, 1, 1);
    general_gui_group_alt_sf_color_entry = new FCColorEntry(nullptr);
    general_gui_group_alt_sf_color_entry->setValue(QString::fromUtf8("#BBF268bf"));
    lay9->addWidget(general_gui_group_alt_sf_color_entry, 22, 1, 1, 1);
    general_gui_group_right_left_alpha_label = new QLabel(nullptr);
    general_gui_group_right_left_alpha_label->setText(QString::fromUtf8("Alfa:"));
    general_gui_group_right_left_alpha_label->setToolTip(QString::fromUtf8("Establezca la transparencia de relleno para el cuadro de selección \"de derecha a izquierda\"."));
    lay9->addWidget(general_gui_group_right_left_alpha_label, 23, 0, 1, 1);
    general_gui_group_right_left_alpha_entry = new FCSliderWithSpinner(0, 255, 1, nullptr);
    general_gui_group_right_left_alpha_entry->setValue(191);
    lay9->addWidget(general_gui_group_right_left_alpha_entry, 23, 1, 1, 1);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    lay9->addWidget(w13, 24, 0, 1, 2);
    general_gui_group_editor_color_label = new QLabel(nullptr);
    general_gui_group_editor_color_label->setText(QString::fromUtf8("<b>Color del editor</b>"));
    lay9->addWidget(general_gui_group_editor_color_label, 25, 0, 1, 2);
    general_gui_group_draw_color_label = new QLabel(nullptr);
    general_gui_group_draw_color_label->setText(QString::fromUtf8("Dibujo:"));
    lay9->addWidget(general_gui_group_draw_color_label, 26, 0, 1, 1);
    general_gui_group_draw_color_entry = new FCColorEntry(nullptr);
    general_gui_group_draw_color_entry->setValue(QString::fromUtf8("#FF0000"));
    lay9->addWidget(general_gui_group_draw_color_entry, 26, 1, 1, 1);
    general_gui_group_sel_draw_color_label = new QLabel(nullptr);
    general_gui_group_sel_draw_color_label->setText(QString::fromUtf8("Selección:"));
    general_gui_group_sel_draw_color_label->setToolTip(QString::fromUtf8("Establecer el color de la forma cuando se selecciona."));
    lay9->addWidget(general_gui_group_sel_draw_color_label, 27, 0, 1, 1);
    general_gui_group_sel_draw_color_entry = new FCColorEntry(nullptr);
    general_gui_group_sel_draw_color_entry->setValue(QString::fromUtf8("#0000FF"));
    lay9->addWidget(general_gui_group_sel_draw_color_entry, 27, 1, 1, 1);
    auto* w14 = new QFrame(nullptr);
    w14->setProperty("frameShadow", 48);
    w14->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w14->setSizePolicy(sp); }
    lay9->addWidget(w14, 28, 0, 1, 2);
    general_gui_group_proj_settings_label = new QLabel(nullptr);
    general_gui_group_proj_settings_label->setText(QString::fromUtf8("<b>Color de los elementos del proyecto</b>"));
    lay9->addWidget(general_gui_group_proj_settings_label, 29, 0, 1, 2);
    general_gui_group_proj_color_label = new QLabel(nullptr);
    general_gui_group_proj_color_label->setText(QString::fromUtf8("Habilitado:"));
    general_gui_group_proj_color_label->setToolTip(QString::fromUtf8("Establecer el color de los elementos en el árbol de pestañas del proyecto."));
    lay9->addWidget(general_gui_group_proj_color_label, 30, 0, 1, 1);
    general_gui_group_proj_color_entry = new FCColorEntry(nullptr);
    lay9->addWidget(general_gui_group_proj_color_entry, 30, 1, 1, 1);
    general_gui_group_proj_color_dis_label = new QLabel(nullptr);
    general_gui_group_proj_color_dis_label->setText(QString::fromUtf8("Discapacitado:"));
    general_gui_group_proj_color_dis_label->setToolTip(QString::fromUtf8("Establecer el color de los elementos en el árbol de pestañas del proyecto,\npara el caso cuando los elementos están deshabilitados."));
    lay9->addWidget(general_gui_group_proj_color_dis_label, 31, 0, 1, 1);
    general_gui_group_proj_color_dis_entry = new FCColorEntry(nullptr);
    lay9->addWidget(general_gui_group_proj_color_dis_entry, 31, 1, 1, 1);
    general_gui_group_project_autohide_cb = new FCCheckBox(nullptr);
    general_gui_group_project_autohide_cb->setChecked(true);
    general_gui_group_project_autohide_cb->setText(QString::fromUtf8("Proyecto auto ocultar"));
    general_gui_group_project_autohide_cb->setToolTip(QString::fromUtf8("Marque esta casilla si desea que el área de la pestaña del proyecto / seleccionado / herramienta\nSe oculta automáticamente cuando no hay objetos cargados y\npara mostrar cada vez que se crea un nuevo objeto."));
    lay9->addWidget(general_gui_group_project_autohide_cb, 32, 0, 1, 2);
    auto* w15 = new QLabel(nullptr);
    lay9->addWidget(w15, 33, 0, 1, 2);
    lay9->setColumnStretch(1, 1);
    general_gui_group_layout->addLayout(lay9, 0);
    general_gui_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    general_gui_group->setLayout(general_gui_group_layout);
    layout->addWidget(general_gui_group, 0);
    general_app_set_group = new QGroupBox(nullptr);
    { QFont f = general_app_set_group->font(); f.setBold(true); f.setPixelSize(16); general_app_set_group->setFont(f); }
    general_app_set_group->setMinimumSize(QSize(250, 0));
    general_app_set_group_layout = new QVBoxLayout();
    general_app_set_group_layout->setContentsMargins(9, 9, 9, 9);
    general_app_set_group_layout->setSpacing(6);
    auto* lay16 = new QGridLayout();
    lay16->setContentsMargins(0, 0, 0, 0);
    lay16->setHorizontalSpacing(6);
    lay16->setVerticalSpacing(6);
    general_app_set_group_grid_label = new QLabel(nullptr);
    general_app_set_group_grid_label->setText(QString::fromUtf8("<b>Configuración de cuadrícula</b>"));
    lay16->addWidget(general_app_set_group_grid_label, 0, 0, 1, 2);
    general_app_set_group_gridx_label = new QLabel(nullptr);
    general_app_set_group_gridx_label->setText(QString::fromUtf8("Valor X:"));
    general_app_set_group_gridx_label->setToolTip(QString::fromUtf8("Este es el valor de ajuste de cuadrícula en el eje X."));
    lay16->addWidget(general_app_set_group_gridx_label, 1, 0, 1, 1);
    general_app_set_group_gridx_entry = new FCDoubleSpinner(nullptr);
    general_app_set_group_gridx_entry->setProperty("decimals", 4);
    general_app_set_group_gridx_entry->setProperty("singleStep", 0.1);
    general_app_set_group_gridx_entry->setProperty("value", 1.0);
    lay16->addWidget(general_app_set_group_gridx_entry, 1, 1, 1, 1);
    general_app_set_group_gridy_label = new QLabel(nullptr);
    general_app_set_group_gridy_label->setText(QString::fromUtf8("Valor Y:"));
    general_app_set_group_gridy_label->setToolTip(QString::fromUtf8("Este es el valor de ajuste de cuadrícula en el eje Y."));
    lay16->addWidget(general_app_set_group_gridy_label, 2, 0, 1, 1);
    general_app_set_group_gridy_entry = new FCDoubleSpinner(nullptr);
    general_app_set_group_gridy_entry->setProperty("decimals", 4);
    general_app_set_group_gridy_entry->setProperty("singleStep", 0.1);
    general_app_set_group_gridy_entry->setProperty("value", 1.0);
    lay16->addWidget(general_app_set_group_gridy_entry, 2, 1, 1, 1);
    general_app_set_group_snap_max_label = new QLabel(nullptr);
    general_app_set_group_snap_max_label->setText(QString::fromUtf8("Máx. de ajuste:"));
    general_app_set_group_snap_max_label->setToolTip(QString::fromUtf8("Distancia máxima del imán"));
    lay16->addWidget(general_app_set_group_snap_max_label, 3, 0, 1, 1);
    general_app_set_group_snap_max_dist_entry = new FCDoubleSpinner(nullptr);
    general_app_set_group_snap_max_dist_entry->setProperty("decimals", 4);
    general_app_set_group_snap_max_dist_entry->setProperty("singleStep", 0.1);
    general_app_set_group_snap_max_dist_entry->setProperty("value", 0.05);
    lay16->addWidget(general_app_set_group_snap_max_dist_entry, 3, 1, 1, 1);
    auto* w17 = new QFrame(nullptr);
    w17->setProperty("frameShadow", 48);
    w17->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w17->setSizePolicy(sp); }
    lay16->addWidget(w17, 4, 0, 1, 2);
    general_app_set_group_workspace_label = new QLabel(nullptr);
    general_app_set_group_workspace_label->setText(QString::fromUtf8("<b>Configuración del espacio de trabajo</b>"));
    lay16->addWidget(general_app_set_group_workspace_label, 5, 0, 1, 2);
    general_app_set_group_workspace_cb = new FCCheckBox(nullptr);
    general_app_set_group_workspace_cb->setText(QString::fromUtf8("Activo"));
    general_app_set_group_workspace_cb->setToolTip(QString::fromUtf8("Dibuja un rectángulo delimitador en el lienzo.\nEl propósito es ilustrar los límites de nuestro trabajo."));
    lay16->addWidget(general_app_set_group_workspace_cb, 6, 0, 1, 2);
    general_app_set_group_workspace_type_lbl = new QLabel(nullptr);
    general_app_set_group_workspace_type_lbl->setText(QString::fromUtf8("Tamaño:"));
    general_app_set_group_workspace_type_lbl->setToolTip(QString::fromUtf8("Seleccione el tipo de rectángulo a utilizar en el lienzo,\ncomo espacio de trabajo válido."));
    lay16->addWidget(general_app_set_group_workspace_type_lbl, 7, 0, 1, 1);
    general_app_set_group_wk_cb = new FCComboBox(nullptr);
    general_app_set_group_wk_cb->setProperty("currentText", QString::fromUtf8("A4"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A0"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A1"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A2"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A3"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A4"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A5"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A6"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A7"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A8"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A9"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("A10"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B0"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B1"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B2"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B3"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B4"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B5"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B6"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B7"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B8"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B9"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("B10"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C0"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C1"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C2"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C3"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C4"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C5"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C6"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C7"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C8"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C9"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("C10"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("LETTER"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("LEGAL"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("ELEVENSEVENTEEN"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("JUNIOR_LEGAL"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("HALF_LETTER"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("GOV_LETTER"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("GOV_LEGAL"));
    general_app_set_group_wk_cb->addItem(QString::fromUtf8("LEDGER"));
    general_app_set_group_wk_cb->setCurrentIndex(4);
    lay16->addWidget(general_app_set_group_wk_cb, 7, 1, 1, 1);
    general_app_set_group_wk_orientation_label = new QLabel(nullptr);
    general_app_set_group_wk_orientation_label->setText(QString::fromUtf8("Orientación:"));
    general_app_set_group_wk_orientation_label->setToolTip(QString::fromUtf8("Puede ser:\n- retrato\n- paisaje"));
    lay16->addWidget(general_app_set_group_wk_orientation_label, 8, 0, 1, 1);
    general_app_set_group_wk_orientation_radio = new RadioSet({{QString::fromUtf8("Retrato"), QVariant(QString::fromUtf8("p"))}, {QString::fromUtf8("Paisaje"), QVariant(QString::fromUtf8("l"))}}, Qt::Horizontal, false, nullptr);
    general_app_set_group_wk_orientation_radio->setValue(QVariant(QString::fromUtf8("p")));
    lay16->addWidget(general_app_set_group_wk_orientation_radio, 8, 1, 1, 1);
    auto* w18 = new QFrame(nullptr);
    w18->setProperty("frameShadow", 48);
    w18->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w18->setSizePolicy(sp); }
    lay16->addWidget(w18, 9, 0, 1, 2);
    general_app_set_group_font_size_label = new QLabel(nullptr);
    general_app_set_group_font_size_label->setText(QString::fromUtf8("<b>Tamaño de Fuente</b>"));
    lay16->addWidget(general_app_set_group_font_size_label, 10, 0, 1, 2);
    general_app_set_group_notebook_font_size_label = new QLabel(nullptr);
    general_app_set_group_notebook_font_size_label->setText(QString::fromUtf8("Cuaderno:"));
    general_app_set_group_notebook_font_size_label->setToolTip(QString::fromUtf8("Esto establece el tamaño de fuente para los elementos encontrados en el Cuaderno.\nEl cuaderno es el área plegable en el lado izquierdo de la aplicación GUI,\ne incluye las pestañas Proyecto, Seleccionado y Herramienta."));
    lay16->addWidget(general_app_set_group_notebook_font_size_label, 11, 0, 1, 1);
    general_app_set_group_notebook_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_notebook_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_notebook_font_size_spinner->setProperty("minimum", 8);
    general_app_set_group_notebook_font_size_spinner->setProperty("value", 12);
    general_app_set_group_notebook_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_notebook_font_size_spinner, 11, 1, 1, 1);
    general_app_set_group_axis_font_size_label = new QLabel(nullptr);
    general_app_set_group_axis_font_size_label->setText(QString::fromUtf8("Eje:"));
    general_app_set_group_axis_font_size_label->setToolTip(QString::fromUtf8("Esto establece el tamaño de fuente para el eje del lienzo."));
    lay16->addWidget(general_app_set_group_axis_font_size_label, 12, 0, 1, 1);
    general_app_set_group_axis_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_axis_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_axis_font_size_spinner->setProperty("value", 8);
    general_app_set_group_axis_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_axis_font_size_spinner, 12, 1, 1, 1);
    general_app_set_group_textbox_font_size_label = new QLabel(nullptr);
    general_app_set_group_textbox_font_size_label->setText(QString::fromUtf8("Caja de texto:"));
    general_app_set_group_textbox_font_size_label->setToolTip(QString::fromUtf8("Esto establece el tamaño de fuente para la aplicación Textbox GUI\nelementos que se usan en la aplicación."));
    lay16->addWidget(general_app_set_group_textbox_font_size_label, 13, 0, 1, 1);
    general_app_set_group_textbox_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_textbox_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_textbox_font_size_spinner->setProperty("minimum", 8);
    general_app_set_group_textbox_font_size_spinner->setProperty("value", 10);
    general_app_set_group_textbox_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_textbox_font_size_spinner, 13, 1, 1, 1);
    general_app_set_group_hud_font_size_label = new QLabel(nullptr);
    general_app_set_group_hud_font_size_label->setText(QString::fromUtf8("HUD:"));
    general_app_set_group_hud_font_size_label->setToolTip(QString::fromUtf8("Esto establece el tamaño de fuente para la pantalla de Heads Up."));
    lay16->addWidget(general_app_set_group_hud_font_size_label, 14, 0, 1, 1);
    general_app_set_group_hud_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_hud_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_hud_font_size_spinner->setProperty("minimum", 8);
    general_app_set_group_hud_font_size_spinner->setProperty("value", 8);
    general_app_set_group_hud_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_hud_font_size_spinner, 14, 1, 1, 1);
    auto* w19 = new QFrame(nullptr);
    w19->setProperty("frameShadow", 48);
    w19->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w19->setSizePolicy(sp); }
    lay16->addWidget(w19, 16, 0, 1, 2);
    general_app_set_group_mouse_lbl = new QLabel(nullptr);
    general_app_set_group_mouse_lbl->setText(QString::fromUtf8("<b>Configuraciones del mouse</b>"));
    lay16->addWidget(general_app_set_group_mouse_lbl, 21, 0, 1, 2);
    general_app_set_group_cursor_lbl = new QLabel(nullptr);
    general_app_set_group_cursor_lbl->setText(QString::fromUtf8("Forma del cursor:"));
    general_app_set_group_cursor_lbl->setToolTip(QString::fromUtf8("Elija la forma del cursor del mouse.\n- Pequeño -> con un tamaño personalizable.\n- Grande -> Líneas infinitas"));
    lay16->addWidget(general_app_set_group_cursor_lbl, 22, 0, 1, 1);
    general_app_set_group_cursor_radio = new RadioSet({{QString::fromUtf8("Pequeño"), QVariant(QString::fromUtf8("small"))}, {QString::fromUtf8("Grande"), QVariant(QString::fromUtf8("big"))}}, Qt::Horizontal, false, nullptr);
    general_app_set_group_cursor_radio->setValue(QVariant(QString::fromUtf8("small")));
    lay16->addWidget(general_app_set_group_cursor_radio, 22, 1, 1, 1);
    general_app_set_group_cursor_size_lbl = new QLabel(nullptr);
    general_app_set_group_cursor_size_lbl->setText(QString::fromUtf8("Tamaño del cursor:"));
    general_app_set_group_cursor_size_lbl->setToolTip(QString::fromUtf8("Establezca el tamaño del cursor del mouse, en píxeles."));
    lay16->addWidget(general_app_set_group_cursor_size_lbl, 23, 0, 1, 1);
    general_app_set_group_cursor_size_entry = new FCSpinner(nullptr);
    general_app_set_group_cursor_size_entry->setProperty("maximum", 70);
    general_app_set_group_cursor_size_entry->setProperty("minimum", 10);
    general_app_set_group_cursor_size_entry->setProperty("value", 20);
    general_app_set_group_cursor_size_entry->setWrapping(true);
    lay16->addWidget(general_app_set_group_cursor_size_entry, 23, 1, 1, 1);
    general_app_set_group_cursor_width_lbl = new QLabel(nullptr);
    general_app_set_group_cursor_width_lbl->setText(QString::fromUtf8("Ancho del cursor:"));
    general_app_set_group_cursor_width_lbl->setToolTip(QString::fromUtf8("Establezca el ancho de línea del cursor del mouse, en píxeles."));
    lay16->addWidget(general_app_set_group_cursor_width_lbl, 24, 0, 1, 1);
    general_app_set_group_cursor_width_entry = new FCSpinner(nullptr);
    general_app_set_group_cursor_width_entry->setProperty("maximum", 10);
    general_app_set_group_cursor_width_entry->setProperty("minimum", 1);
    general_app_set_group_cursor_width_entry->setProperty("value", 2);
    general_app_set_group_cursor_width_entry->setWrapping(true);
    lay16->addWidget(general_app_set_group_cursor_width_entry, 24, 1, 1, 1);
    general_app_set_group_mouse_cursor_color_cb = new FCCheckBox(nullptr);
    general_app_set_group_mouse_cursor_color_cb->setChecked(true);
    general_app_set_group_mouse_cursor_color_cb->setText(QString::fromUtf8("Color del cursor"));
    general_app_set_group_mouse_cursor_color_cb->setToolTip(QString::fromUtf8("Marque esta casilla para colorear el cursor del mouse."));
    lay16->addWidget(general_app_set_group_mouse_cursor_color_cb, 25, 0, 1, 2);
    general_app_set_group_mouse_color_label = new QLabel(nullptr);
    general_app_set_group_mouse_color_label->setText(QString::fromUtf8("Color del cursor:"));
    general_app_set_group_mouse_color_label->setToolTip(QString::fromUtf8("Establece el color del cursor del mouse."));
    lay16->addWidget(general_app_set_group_mouse_color_label, 26, 0, 1, 1);
    general_app_set_group_mouse_cursor_entry = new FCColorEntry(nullptr);
    general_app_set_group_mouse_cursor_entry->setValue(QString::fromUtf8("#FF0000"));
    lay16->addWidget(general_app_set_group_mouse_cursor_entry, 26, 1, 1, 1);
    general_app_set_group_panbuttonlabel = new QLabel(nullptr);
    general_app_set_group_panbuttonlabel->setText(QString::fromUtf8("Botón de pan:"));
    general_app_set_group_panbuttonlabel->setToolTip(QString::fromUtf8("Seleccione el botón del ratón para utilizarlo en la panorámica:\n- MMB -> Botón Central Del Ratón\n- RMB -> Botón derecho del ratón"));
    lay16->addWidget(general_app_set_group_panbuttonlabel, 27, 0, 1, 1);
    general_app_set_group_pan_button_radio = new RadioSet({{QString::fromUtf8("MMB"), QVariant(QString::fromUtf8("3"))}, {QString::fromUtf8("RMB"), QVariant(QString::fromUtf8("2"))}}, Qt::Horizontal, true, nullptr);
    general_app_set_group_pan_button_radio->setValue(QVariant(QString::fromUtf8("2")));
    lay16->addWidget(general_app_set_group_pan_button_radio, 27, 1, 1, 1);
    general_app_set_group_mselectlabel = new QLabel(nullptr);
    general_app_set_group_mselectlabel->setText(QString::fromUtf8("Selección múltiple:"));
    general_app_set_group_mselectlabel->setToolTip(QString::fromUtf8("Seleccione la clave utilizada para la selección múltiple."));
    lay16->addWidget(general_app_set_group_mselectlabel, 28, 0, 1, 1);
    general_app_set_group_mselect_radio = new RadioSet({{QString::fromUtf8("CTRL"), QVariant(QString::fromUtf8("Control"))}, {QString::fromUtf8("SHIFT"), QVariant(QString::fromUtf8("Shift"))}}, Qt::Horizontal, true, nullptr);
    general_app_set_group_mselect_radio->setValue(QVariant(QString::fromUtf8("Control")));
    lay16->addWidget(general_app_set_group_mselect_radio, 28, 1, 1, 1);
    auto* w20 = new QFrame(nullptr);
    w20->setProperty("frameShadow", 48);
    w20->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w20->setSizePolicy(sp); }
    lay16->addWidget(w20, 29, 0, 1, 2);
    general_app_set_group_delete_conf_cb = new FCCheckBox(nullptr);
    general_app_set_group_delete_conf_cb->setChecked(true);
    general_app_set_group_delete_conf_cb->setText(QString::fromUtf8("Eliminar confirmación de objeto"));
    general_app_set_group_delete_conf_cb->setToolTip(QString::fromUtf8("Cuando esté marcada, la aplicación solicitará la confirmación del usuario\ncada vez que se desencadena el evento Eliminar objeto (s), ya sea por\nacceso directo al menú o acceso directo a teclas."));
    lay16->addWidget(general_app_set_group_delete_conf_cb, 30, 0, 1, 2);
    general_app_set_group_allow_edit_cb = new FCCheckBox(nullptr);
    general_app_set_group_allow_edit_cb->setText(QString::fromUtf8("Permitir editar"));
    general_app_set_group_allow_edit_cb->setToolTip(QString::fromUtf8("Cuando está marcado, el usuario puede editar los nombres de los objetos en la pestaña Proyecto\nhaciendo clic en el nombre del objeto. Activo después del reinicio."));
    lay16->addWidget(general_app_set_group_allow_edit_cb, 31, 0, 1, 2);
    general_app_set_group_open_style_cb = new FCCheckBox(nullptr);
    general_app_set_group_open_style_cb->setChecked(true);
    general_app_set_group_open_style_cb->setText(QString::fromUtf8("Comportamiento \"abierto\""));
    general_app_set_group_open_style_cb->setToolTip(QString::fromUtf8("Cuando se verifica, la ruta del último archivo guardado se usa al guardar archivos,\ny la ruta del último archivo abierto se utiliza al abrir archivos.\n\nCuando no está marcada, la ruta para abrir archivos es la última utilizada:\nruta para guardar archivos o la ruta para abrir archivos."));
    lay16->addWidget(general_app_set_group_open_style_cb, 32, 0, 1, 2);
    general_app_set_group_toggle_tooltips_cb = new FCCheckBox(nullptr);
    general_app_set_group_toggle_tooltips_cb->setChecked(true);
    general_app_set_group_toggle_tooltips_cb->setText(QString::fromUtf8("Hab. info sobre Herram"));
    general_app_set_group_toggle_tooltips_cb->setToolTip(QString::fromUtf8("Marque esta casilla si desea que se muestre información sobre herramientas\nal pasar el mouse sobre los elementos de la aplicación."));
    lay16->addWidget(general_app_set_group_toggle_tooltips_cb, 33, 0, 1, 2);
    general_app_set_group_machinist_cb = new FCCheckBox(nullptr);
    general_app_set_group_machinist_cb->setText(QString::fromUtf8("Permitir configuraciones inseguras de Maquinista"));
    general_app_set_group_machinist_cb->setToolTip(QString::fromUtf8("Si está marcada, se permitirán algunas de las configuraciones de la aplicación\ntener valores que generalmente no son seguros de usar.\nComo los valores negativos de desplazamiento Z o los valores positivos de Z Cut.\nSe aplicará en el próximo inicio de la aplicación.\n<<ADVERTENCIA>>: ¡No cambie esto a menos que sepa lo que está haciendo!"));
    lay16->addWidget(general_app_set_group_machinist_cb, 34, 0, 1, 2);
    general_app_set_group_bm_limit_label = new QLabel(nullptr);
    general_app_set_group_bm_limit_label->setText(QString::fromUtf8("Límite de Marcadores:"));
    general_app_set_group_bm_limit_label->setToolTip(QString::fromUtf8("El número máximo de marcadores que se pueden instalar en el menú.\nEl número de marcadores en el administrador de marcadores puede ser mayor\npero el menú solo tendrá una cantidad considerable."));
    lay16->addWidget(general_app_set_group_bm_limit_label, 35, 0, 1, 1);
    general_app_set_group_bm_limit_spinner = new FCSpinner(nullptr);
    general_app_set_group_bm_limit_spinner->setProperty("maximum", 9999);
    general_app_set_group_bm_limit_spinner->setProperty("value", 10);
    lay16->addWidget(general_app_set_group_bm_limit_spinner, 35, 1, 1, 1);
    general_app_set_group_activity_label = new QLabel(nullptr);
    general_app_set_group_activity_label->setText(QString::fromUtf8("Ícono de actividad:"));
    general_app_set_group_activity_label->setToolTip(QString::fromUtf8("Seleccione el GIF que muestra actividad cuando FlatCAM está activo."));
    lay16->addWidget(general_app_set_group_activity_label, 36, 0, 1, 1);
    general_app_set_group_activity_combo = new FCComboBox(nullptr);
    general_app_set_group_activity_combo->setProperty("currentText", QString::fromUtf8("Ball green"));
    general_app_set_group_activity_combo->addItem(QString::fromUtf8("Ball black"));
    general_app_set_group_activity_combo->addItem(QString::fromUtf8("Ball green"));
    general_app_set_group_activity_combo->addItem(QString::fromUtf8("Arrow green"));
    general_app_set_group_activity_combo->addItem(QString::fromUtf8("Eclipse green"));
    general_app_set_group_activity_combo->setCurrentIndex(1);
    lay16->addWidget(general_app_set_group_activity_combo, 36, 1, 1, 1);
    lay16->setColumnStretch(1, 1);
    general_app_set_group_layout->addLayout(lay16, 0);
    general_app_set_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    general_app_set_group->setLayout(general_app_set_group_layout);
    layout->addWidget(general_app_set_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    general_defaults_form->setLayout(layout);
    general_scroll_area->setWidget(general_defaults_form);
    general_tab_lay->addWidget(general_scroll_area, 0);
    general_tab->setLayout(general_tab_lay);
    pref_tab_area->addTab(general_tab, QString::fromUtf8("General"));
    gerber_tab = new QWidget(nullptr);
    gerber_tab->setProperty("objectName", QString::fromUtf8("gerber_tab"));
    gerber_tab->setHidden(true);
    gerber_tab_lay = new QVBoxLayout();
    gerber_tab_lay->setContentsMargins(2, 2, 2, 2);
    gerber_tab_lay->setSpacing(6);
    gerber_scroll_area = new QScrollArea(nullptr);
    gerber_defaults_form = new QWidget(nullptr);
    gerber_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    gerber_gen_group = new QGroupBox(nullptr);
    { QFont f = gerber_gen_group->font(); f.setBold(true); f.setPixelSize(16); gerber_gen_group->setFont(f); }
    gerber_gen_group->setMinimumSize(QSize(250, 0));
    gerber_gen_group_layout = new QVBoxLayout();
    gerber_gen_group_layout->setContentsMargins(9, 9, 9, 9);
    gerber_gen_group_layout->setSpacing(6);
    gerber_gen_group_plot_options_label = new QLabel(nullptr);
    gerber_gen_group_plot_options_label->setText(QString::fromUtf8("<b>Opciones de parcela:</b>"));
    gerber_gen_group_plot_options_label->setToolTip(QString::fromUtf8("Trazar (mostrar) este objeto."));
    gerber_gen_group_layout->addWidget(gerber_gen_group_plot_options_label, 0);
    auto* lay21 = new QGridLayout();
    lay21->setContentsMargins(0, 0, 0, 0);
    lay21->setHorizontalSpacing(6);
    lay21->setVerticalSpacing(6);
    gerber_gen_group_plot_cb = new FCCheckBox(nullptr);
    gerber_gen_group_plot_cb->setChecked(true);
    gerber_gen_group_plot_cb->setText(QString::fromUtf8("Gráfico"));
    lay21->addWidget(gerber_gen_group_plot_cb, 0, 0, 1, 1);
    gerber_gen_group_solid_cb = new FCCheckBox(nullptr);
    gerber_gen_group_solid_cb->setChecked(true);
    gerber_gen_group_solid_cb->setText(QString::fromUtf8("Sólido"));
    gerber_gen_group_solid_cb->setToolTip(QString::fromUtf8("Polígonos de color liso."));
    lay21->addWidget(gerber_gen_group_solid_cb, 0, 1, 1, 1);
    gerber_gen_group_multicolored_cb = new FCCheckBox(nullptr);
    gerber_gen_group_multicolored_cb->setText(QString::fromUtf8("M-Color"));
    gerber_gen_group_multicolored_cb->setToolTip(QString::fromUtf8("Dibuja polígonos en diferentes colores."));
    lay21->addWidget(gerber_gen_group_multicolored_cb, 0, 2, 1, 1);
    gerber_gen_group_circle_steps_label = new QLabel(nullptr);
    gerber_gen_group_circle_steps_label->setText(QString::fromUtf8("Pasos del círculo:"));
    gerber_gen_group_circle_steps_label->setToolTip(QString::fromUtf8("El número de pasos de círculo para Gerber\nApertura circular de aproximación lineal."));
    lay21->addWidget(gerber_gen_group_circle_steps_label, 1, 0, 1, 1);
    gerber_gen_group_circle_steps_entry = new FCSpinner(nullptr);
    gerber_gen_group_circle_steps_entry->setProperty("maximum", 9999);
    gerber_gen_group_circle_steps_entry->setProperty("value", 64);
    lay21->addWidget(gerber_gen_group_circle_steps_entry, 1, 1, 1, 2);
    auto* w22 = new QLabel(nullptr);
    lay21->addWidget(w22, 2, 0, 1, 3);
    gerber_gen_group_gerber_default_label = new QLabel(nullptr);
    gerber_gen_group_gerber_default_label->setText(QString::fromUtf8("<b>Valores predeterminados:</b>"));
    gerber_gen_group_gerber_default_label->setToolTip(QString::fromUtf8("Esos valores se usarán como valores de reserva\nen caso de que no se encuentren en el archivo Gerber."));
    lay21->addWidget(gerber_gen_group_gerber_default_label, 3, 0, 1, 3);
    gerber_gen_group_gerber_units_label = new QLabel(nullptr);
    gerber_gen_group_gerber_units_label->setText(QString::fromUtf8("Unidades:"));
    gerber_gen_group_gerber_units_label->setToolTip(QString::fromUtf8("Las unidades utilizadas en el archivo Gerber."));
    lay21->addWidget(gerber_gen_group_gerber_units_label, 4, 0, 1, 1);
    gerber_gen_group_gerber_units_radio = new RadioSet({{QString::fromUtf8("Pulgada"), QVariant(QString::fromUtf8("IN"))}, {QString::fromUtf8("mm"), QVariant(QString::fromUtf8("MM"))}}, Qt::Horizontal, true, nullptr);
    gerber_gen_group_gerber_units_radio->setToolTip(QString::fromUtf8("Las unidades utilizadas en el archivo Gerber."));
    gerber_gen_group_gerber_units_radio->setValue(QVariant(QString::fromUtf8("IN")));
    lay21->addWidget(gerber_gen_group_gerber_units_radio, 4, 1, 1, 2);
    gerber_gen_group_gerber_zeros_label = new QLabel(nullptr);
    gerber_gen_group_gerber_zeros_label->setProperty("alignment", 1);
    gerber_gen_group_gerber_zeros_label->setText(QString::fromUtf8("Ceros:"));
    gerber_gen_group_gerber_zeros_label->setToolTip(QString::fromUtf8("Esto establece el tipo de ceros Gerber.\nSi LZ entonces los ceros iniciales se eliminan y\nSe guardan los ceros que se arrastran.\nSi se comprueba TZ, se eliminan los ceros finales\ny Leading Zeros se mantienen."));
    lay21->addWidget(gerber_gen_group_gerber_zeros_label, 5, 0, 1, 1);
    gerber_gen_group_gerber_zeros_radio = new RadioSet({{QString::fromUtf8("LZ"), QVariant(QString::fromUtf8("L"))}, {QString::fromUtf8("TZ"), QVariant(QString::fromUtf8("T"))}}, Qt::Horizontal, true, nullptr);
    gerber_gen_group_gerber_zeros_radio->setToolTip(QString::fromUtf8("Esto establece el tipo de ceros Gerber.\nSi LZ entonces los ceros iniciales se eliminan y\nSe guardan los ceros que se arrastran.\nSi se comprueba TZ, se eliminan los ceros finales\ny Leading Zeros se mantienen."));
    gerber_gen_group_gerber_zeros_radio->setValue(QVariant(QString::fromUtf8("L")));
    lay21->addWidget(gerber_gen_group_gerber_zeros_radio, 5, 1, 1, 2);
    auto* w23 = new QFrame(nullptr);
    w23->setProperty("frameShadow", 48);
    w23->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w23->setSizePolicy(sp); }
    lay21->addWidget(w23, 6, 0, 1, 3);
    gerber_gen_group_gerber_clean_cb = new FCCheckBox(nullptr);
    gerber_gen_group_gerber_clean_cb->setChecked(true);
    gerber_gen_group_gerber_clean_cb->setText(QString::fromUtf8("Aberturas limpias"));
    gerber_gen_group_gerber_clean_cb->setToolTip(QString::fromUtf8("Eliminará las aberturas que no tengan geometría\nbajando así el número de aberturas en el objeto Gerber."));
    lay21->addWidget(gerber_gen_group_gerber_clean_cb, 7, 0, 1, 3);
    gerber_gen_group_gerber_extra_buffering = new FCCheckBox(nullptr);
    gerber_gen_group_gerber_extra_buffering->setChecked(true);
    gerber_gen_group_gerber_extra_buffering->setText(QString::fromUtf8("Cambio de polaridad buffer"));
    gerber_gen_group_gerber_extra_buffering->setToolTip(QString::fromUtf8("Aplicará buffering adicional para el\ngeometría sólida cuando tenemos cambios de polaridad.\nPuede ayudar a cargar archivos Gerber que de otra manera\nNo cargar correctamente."));
    lay21->addWidget(gerber_gen_group_gerber_extra_buffering, 8, 0, 1, 3);
    gerber_gen_group_store_colors_cb = new FCCheckBox(nullptr);
    gerber_gen_group_store_colors_cb->setChecked(true);
    gerber_gen_group_store_colors_cb->setText(QString::fromUtf8("Almacena los colores"));
    gerber_gen_group_store_colors_cb->setToolTip(QString::fromUtf8("Almacenará los colores establecidos para los objetos Gerber.\nSe utilizarán cada vez que se inicie la aplicación."));
    lay21->addWidget(gerber_gen_group_store_colors_cb, 11, 0, 1, 1);
    gerber_gen_group_clear_colors_button = new FCButton(nullptr);
    gerber_gen_group_clear_colors_button->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    gerber_gen_group_clear_colors_button->setText(QString::fromUtf8("Borrar los colores"));
    gerber_gen_group_clear_colors_button->setToolTip(QString::fromUtf8("Restablece los colores asociados con los objetos Gerber."));
    lay21->addWidget(gerber_gen_group_clear_colors_button, 11, 1, 1, 2);
    auto* w24 = new QFrame(nullptr);
    w24->setProperty("frameShadow", 48);
    w24->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w24->setSizePolicy(sp); }
    lay21->addWidget(w24, 13, 0, 1, 3);
    gerber_gen_group_gerber_color_label = new QLabel(nullptr);
    gerber_gen_group_gerber_color_label->setText(QString::fromUtf8("<b>Color del objeto</b>"));
    lay21->addWidget(gerber_gen_group_gerber_color_label, 15, 0, 1, 3);
    gerber_gen_group_line_color_label = new QLabel(nullptr);
    gerber_gen_group_line_color_label->setText(QString::fromUtf8("Contorno:"));
    gerber_gen_group_line_color_label->setToolTip(QString::fromUtf8("Establecer el color de la línea para los objetos trazados."));
    lay21->addWidget(gerber_gen_group_line_color_label, 17, 0, 1, 1);
    gerber_gen_group_line_color_entry = new FCColorEntry(nullptr);
    gerber_gen_group_line_color_entry->setValue(QString::fromUtf8("#006E20bf"));
    lay21->addWidget(gerber_gen_group_line_color_entry, 17, 1, 1, 2);
    gerber_gen_group_fill_color_label = new QLabel(nullptr);
    gerber_gen_group_fill_color_label->setText(QString::fromUtf8("Llenado:"));
    gerber_gen_group_fill_color_label->setToolTip(QString::fromUtf8("Establecer el color de relleno para los objetos trazados.\nLos primeros 6 dígitos son el color y los 2 últimos.\nLos dígitos son para el nivel alfa (transparencia)."));
    lay21->addWidget(gerber_gen_group_fill_color_label, 20, 0, 1, 1);
    gerber_gen_group_fill_color_entry = new FCColorEntry(nullptr);
    gerber_gen_group_fill_color_entry->setValue(QString::fromUtf8("#BBF268bf"));
    lay21->addWidget(gerber_gen_group_fill_color_entry, 20, 1, 1, 2);
    gerber_gen_group_gerber_alpha_label = new QLabel(nullptr);
    gerber_gen_group_gerber_alpha_label->setText(QString::fromUtf8("Alfa:"));
    gerber_gen_group_gerber_alpha_label->setToolTip(QString::fromUtf8("Establecer la transparencia de relleno para los objetos trazados."));
    lay21->addWidget(gerber_gen_group_gerber_alpha_label, 22, 0, 1, 1);
    gerber_gen_group_gerber_alpha_entry = new FCSliderWithSpinner(0, 255, 1, nullptr);
    gerber_gen_group_gerber_alpha_entry->setValue(191);
    lay21->addWidget(gerber_gen_group_gerber_alpha_entry, 22, 1, 1, 2);
    gerber_gen_group_layout->addLayout(lay21, 0);
    gerber_gen_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    gerber_gen_group->setLayout(gerber_gen_group_layout);
    layout->addWidget(gerber_gen_group, 0);
    vlay = new QVBoxLayout();
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(6);
    gerber_opt_group = new QGroupBox(nullptr);
    { QFont f = gerber_opt_group->font(); f.setBold(true); f.setPixelSize(16); gerber_opt_group->setFont(f); }
    gerber_opt_group->setMinimumSize(QSize(250, 0));
    gerber_opt_group_layout = new QVBoxLayout();
    gerber_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    gerber_opt_group_layout->setSpacing(6);
    gerber_opt_group_clearcopper_label = new QLabel(nullptr);
    gerber_opt_group_clearcopper_label->setText(QString::fromUtf8("<b>Regiones no cobre:</b>"));
    gerber_opt_group_clearcopper_label->setToolTip(QString::fromUtf8("Crear polígonos que cubran el\náreas sin cobre en el PCB.\nEquivalente al inverso de este\nobjeto. Se puede usar para eliminar todo\ncobre de una región específica."));
    gerber_opt_group_layout->addWidget(gerber_opt_group_clearcopper_label, 0);
    auto* lay25 = new QGridLayout();
    lay25->setContentsMargins(0, 0, 0, 0);
    lay25->setHorizontalSpacing(6);
    lay25->setVerticalSpacing(6);
    auto* w26 = new QLabel(nullptr);
    w26->setText(QString::fromUtf8("Margen límite:"));
    w26->setToolTip(QString::fromUtf8("Especifique el borde de la PCB\ndibujando una caja alrededor de todos\nobjetos con este mínimo\ndistancia."));
    lay25->addWidget(w26, 0, 0, 1, 1);
    gerber_opt_group_noncopper_margin_entry = new FCDoubleSpinner(nullptr);
    gerber_opt_group_noncopper_margin_entry->setProperty("decimals", 4);
    gerber_opt_group_noncopper_margin_entry->setProperty("maximum", 9999.0);
    gerber_opt_group_noncopper_margin_entry->setProperty("minimum", -9999.0);
    gerber_opt_group_noncopper_margin_entry->setProperty("singleStep", 0.1);
    gerber_opt_group_noncopper_margin_entry->setProperty("value", 0.1);
    lay25->addWidget(gerber_opt_group_noncopper_margin_entry, 0, 1, 1, 1);
    gerber_opt_group_noncopper_rounded_cb = new FCCheckBox(nullptr);
    gerber_opt_group_noncopper_rounded_cb->setText(QString::fromUtf8("Geo redondeado"));
    gerber_opt_group_noncopper_rounded_cb->setToolTip(QString::fromUtf8("La geometría resultante tendrá esquinas redondeadas."));
    lay25->addWidget(gerber_opt_group_noncopper_rounded_cb, 1, 0, 1, 2);
    auto* w27 = new QFrame(nullptr);
    w27->setProperty("frameShadow", 48);
    w27->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w27->setSizePolicy(sp); }
    lay25->addWidget(w27, 2, 0, 1, 2);
    gerber_opt_group_layout->addLayout(lay25, 0);
    gerber_opt_group_boundingbox_label = new QLabel(nullptr);
    gerber_opt_group_boundingbox_label->setText(QString::fromUtf8("<b>Cuadro delimitador:</b>"));
    gerber_opt_group_layout->addWidget(gerber_opt_group_boundingbox_label, 0);
    auto* lay28 = new QGridLayout();
    lay28->setContentsMargins(0, 0, 0, 0);
    lay28->setHorizontalSpacing(6);
    lay28->setVerticalSpacing(6);
    auto* w29 = new QLabel(nullptr);
    w29->setText(QString::fromUtf8("Margen límite:"));
    w29->setToolTip(QString::fromUtf8("Distancia de los bordes de la caja.\nal polígono más cercano."));
    lay28->addWidget(w29, 0, 0, 1, 1);
    gerber_opt_group_bbmargin_entry = new FCDoubleSpinner(nullptr);
    gerber_opt_group_bbmargin_entry->setProperty("decimals", 4);
    gerber_opt_group_bbmargin_entry->setProperty("maximum", 9999.0);
    gerber_opt_group_bbmargin_entry->setProperty("minimum", -9999.0);
    gerber_opt_group_bbmargin_entry->setProperty("singleStep", 0.1);
    gerber_opt_group_bbmargin_entry->setProperty("value", 0.1);
    lay28->addWidget(gerber_opt_group_bbmargin_entry, 0, 1, 1, 1);
    gerber_opt_group_bbrounded_cb = new FCCheckBox(nullptr);
    gerber_opt_group_bbrounded_cb->setText(QString::fromUtf8("Geo redondeado"));
    gerber_opt_group_bbrounded_cb->setToolTip(QString::fromUtf8("Si el cuadro delimitador es\ntener esquinas redondeadas\nsu radio es igual a\nel margen."));
    lay28->addWidget(gerber_opt_group_bbrounded_cb, 1, 0, 1, 2);
    gerber_opt_group_layout->addLayout(lay28, 0);
    gerber_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    gerber_opt_group->setLayout(gerber_opt_group_layout);
    vlay->addWidget(gerber_opt_group, 0);
    gerber_adv_opt_group = new QGroupBox(nullptr);
    { QFont f = gerber_adv_opt_group->font(); f.setBold(true); f.setPixelSize(16); gerber_adv_opt_group->setFont(f); }
    gerber_adv_opt_group->setMinimumSize(QSize(200, 0));
    gerber_adv_opt_group_layout = new QVBoxLayout();
    gerber_adv_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    gerber_adv_opt_group_layout->setSpacing(6);
    gerber_adv_opt_group_adv_param_label = new QLabel(nullptr);
    gerber_adv_opt_group_adv_param_label->setText(QString::fromUtf8("<b>Opciones avanzadas:</b>"));
    gerber_adv_opt_group_adv_param_label->setToolTip(QString::fromUtf8("Una lista de los parámetros avanzados.\nEsos parámetros están disponibles sólo para\nAplicación avanzada Nivel."));
    gerber_adv_opt_group_layout->addWidget(gerber_adv_opt_group_adv_param_label, 0);
    auto* lay30 = new QGridLayout();
    lay30->setContentsMargins(0, 0, 0, 0);
    lay30->setHorizontalSpacing(6);
    lay30->setVerticalSpacing(6);
    gerber_adv_opt_group_follow_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_follow_cb->setText(QString::fromUtf8("\"Seguir\""));
    gerber_adv_opt_group_follow_cb->setToolTip(QString::fromUtf8("Generar una geometría 'Seguir'.\nEsto significa que cortará a través\nEl medio de la traza."));
    lay30->addWidget(gerber_adv_opt_group_follow_cb, 0, 0, 1, 2);
    gerber_adv_opt_group_aperture_table_visibility_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_aperture_table_visibility_cb->setText(QString::fromUtf8("Mostrar / ocultar tabla"));
    gerber_adv_opt_group_aperture_table_visibility_cb->setToolTip(QString::fromUtf8("Alternar la visualización de la tabla de herramientas."));
    lay30->addWidget(gerber_adv_opt_group_aperture_table_visibility_cb, 1, 0, 1, 2);
    auto* w31 = new QFrame(nullptr);
    w31->setProperty("frameShadow", 48);
    w31->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w31->setSizePolicy(sp); }
    lay30->addWidget(w31, 2, 0, 1, 2);
    auto* w32 = new QLabel(nullptr);
    w32->setText(QString::fromUtf8("Tamponamiento:"));
    w32->setToolTip(QString::fromUtf8("Tipo de almacenamiento en búfer:\n- Ninguno -> mejor rendimiento, carga rápida de archivos pero no tan buena visualización\n- Completo -> carga lenta de archivos pero buenas imágenes. Este es el valor predeterminado.\n<<ADVERTENCIA>>: ¡No cambie esto a menos que sepa lo que está haciendo!"));
    lay30->addWidget(w32, 9, 0, 1, 1);
    gerber_adv_opt_group_buffering_radio = new RadioSet({{QString::fromUtf8("Ninguno"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Completo"), QVariant(QString::fromUtf8("full"))}}, Qt::Horizontal, true, nullptr);
    gerber_adv_opt_group_buffering_radio->setValue(QVariant(QString::fromUtf8("full")));
    lay30->addWidget(gerber_adv_opt_group_buffering_radio, 9, 1, 1, 1);
    gerber_adv_opt_group_delayed_buffer_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_delayed_buffer_cb->setChecked(true);
    gerber_adv_opt_group_delayed_buffer_cb->setEnabled(false);
    gerber_adv_opt_group_delayed_buffer_cb->setText(QString::fromUtf8("Buffering Retrasado"));
    gerber_adv_opt_group_delayed_buffer_cb->setToolTip(QString::fromUtf8("Cuando está marcado, hará el almacenamiento en búfer en segundo plano."));
    lay30->addWidget(gerber_adv_opt_group_delayed_buffer_cb, 10, 0, 1, 2);
    gerber_adv_opt_group_simplify_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_simplify_cb->setText(QString::fromUtf8("Simplificar"));
    gerber_adv_opt_group_simplify_cb->setToolTip(QString::fromUtf8("Cuando esté marcado, todos los polígonos de Gerber serán\ncargado de simplificación con una tolerancia establecida.\n<<ADVERTENCIA>>: ¡No cambie esto a menos que sepa lo que está haciendo!"));
    lay30->addWidget(gerber_adv_opt_group_simplify_cb, 11, 0, 1, 2);
    gerber_adv_opt_group_simplification_tol_label = new QLabel(nullptr);
    gerber_adv_opt_group_simplification_tol_label->setEnabled(false);
    gerber_adv_opt_group_simplification_tol_label->setText(QString::fromUtf8("Tolerancia"));
    gerber_adv_opt_group_simplification_tol_label->setToolTip(QString::fromUtf8("Tolerancia para la simplificación de polígonos."));
    lay30->addWidget(gerber_adv_opt_group_simplification_tol_label, 12, 0, 1, 1);
    gerber_adv_opt_group_simplification_tol_spinner = new FCDoubleSpinner(nullptr);
    gerber_adv_opt_group_simplification_tol_spinner->setProperty("decimals", 5);
    gerber_adv_opt_group_simplification_tol_spinner->setEnabled(false);
    gerber_adv_opt_group_simplification_tol_spinner->setProperty("maximum", 0.01);
    gerber_adv_opt_group_simplification_tol_spinner->setProperty("singleStep", 0.0001);
    gerber_adv_opt_group_simplification_tol_spinner->setProperty("value", 0.0005);
    gerber_adv_opt_group_simplification_tol_spinner->setWrapping(true);
    lay30->addWidget(gerber_adv_opt_group_simplification_tol_spinner, 12, 1, 1, 1);
    gerber_adv_opt_group_layout->addLayout(lay30, 0);
    gerber_adv_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    gerber_adv_opt_group->setLayout(gerber_adv_opt_group_layout);
    vlay->addWidget(gerber_adv_opt_group, 0);
    gerber_exp_group = new QGroupBox(nullptr);
    { QFont f = gerber_exp_group->font(); f.setBold(true); f.setPixelSize(16); gerber_exp_group->setFont(f); }
    gerber_exp_group->setMinimumSize(QSize(230, 0));
    gerber_exp_group_layout = new QVBoxLayout();
    gerber_exp_group_layout->setContentsMargins(9, 9, 9, 9);
    gerber_exp_group_layout->setSpacing(6);
    gerber_exp_group_export_options_label = new QLabel(nullptr);
    gerber_exp_group_export_options_label->setText(QString::fromUtf8("<b>Opciones de export:</b>"));
    gerber_exp_group_export_options_label->setToolTip(QString::fromUtf8("Los parámetros establecidos aquí se utilizan en el archivo exportado.\ncuando se usa la entrada de menú Archivo -> Exportar -> Exportar Gerber."));
    gerber_exp_group_layout->addWidget(gerber_exp_group_export_options_label, 0);
    auto* lay33 = new QFormLayout();
    lay33->setContentsMargins(0, 0, 0, 0);
    lay33->setHorizontalSpacing(6);
    lay33->setVerticalSpacing(6);
    gerber_exp_group_gerber_units_label = new QLabel(nullptr);
    gerber_exp_group_gerber_units_label->setText(QString::fromUtf8("Unidades:"));
    gerber_exp_group_gerber_units_label->setToolTip(QString::fromUtf8("Las unidades utilizadas en el archivo Gerber."));
    lay33->setWidget(0, QFormLayout::ItemRole(0), gerber_exp_group_gerber_units_label);
    gerber_exp_group_gerber_units_radio = new RadioSet({{QString::fromUtf8("Pulgada"), QVariant(QString::fromUtf8("IN"))}, {QString::fromUtf8("mm"), QVariant(QString::fromUtf8("MM"))}}, Qt::Horizontal, true, nullptr);
    gerber_exp_group_gerber_units_radio->setToolTip(QString::fromUtf8("Las unidades utilizadas en el archivo Gerber."));
    gerber_exp_group_gerber_units_radio->setValue(QVariant(QString::fromUtf8("IN")));
    lay33->setWidget(0, QFormLayout::ItemRole(1), gerber_exp_group_gerber_units_radio);
    gerber_exp_group_digits_label = new QLabel(nullptr);
    gerber_exp_group_digits_label->setText(QString::fromUtf8("Entero/Decimales:"));
    gerber_exp_group_digits_label->setToolTip(QString::fromUtf8("El número de dígitos en la parte entera del número.\ny en la parte fraccionaria del número."));
    lay33->setWidget(1, QFormLayout::ItemRole(0), gerber_exp_group_digits_label);
    auto* lay34 = new QHBoxLayout();
    lay34->setContentsMargins(0, 0, 0, 0);
    lay34->setSpacing(6);
    gerber_exp_group_format_whole_entry = new FCSpinner(nullptr);
    gerber_exp_group_format_whole_entry->setProperty("maximum", 9);
    gerber_exp_group_format_whole_entry->setMinimumSize(QSize(30, 0));
    gerber_exp_group_format_whole_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nToda la parte de Gerber coordina."));
    gerber_exp_group_format_whole_entry->setProperty("value", 2);
    gerber_exp_group_format_whole_entry->setWrapping(true);
    lay34->addWidget(gerber_exp_group_format_whole_entry, 1);
    auto* w35 = new QLabel(nullptr);
    w35->setMaximumSize(QSize(5, 16777215));
    w35->setMinimumSize(QSize(5, 0));
    w35->setText(QString::fromUtf8(":"));
    lay34->addWidget(w35, 1);
    gerber_exp_group_format_dec_entry = new FCSpinner(nullptr);
    gerber_exp_group_format_dec_entry->setProperty("maximum", 9);
    gerber_exp_group_format_dec_entry->setMinimumSize(QSize(30, 0));
    gerber_exp_group_format_dec_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nLa parte decimal de las coordenadas de gerber."));
    gerber_exp_group_format_dec_entry->setProperty("value", 4);
    gerber_exp_group_format_dec_entry->setWrapping(true);
    lay34->addWidget(gerber_exp_group_format_dec_entry, 1);
    lay34->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    lay33->setLayout(1, QFormLayout::ItemRole(1), lay34);
    gerber_exp_group_zeros_label = new QLabel(nullptr);
    gerber_exp_group_zeros_label->setProperty("alignment", 1);
    gerber_exp_group_zeros_label->setText(QString::fromUtf8("Ceros:"));
    gerber_exp_group_zeros_label->setToolTip(QString::fromUtf8("Esto establece el tipo de ceros Gerber.\nSi LZ entonces los ceros iniciales se eliminan y\nSe guardan los ceros que se arrastran.\nSi se comprueba TZ, se eliminan los ceros finales\ny Leading Zeros se mantienen."));
    lay33->setWidget(2, QFormLayout::ItemRole(0), gerber_exp_group_zeros_label);
    gerber_exp_group_zeros_radio = new RadioSet({{QString::fromUtf8("LZ"), QVariant(QString::fromUtf8("L"))}, {QString::fromUtf8("TZ"), QVariant(QString::fromUtf8("T"))}}, Qt::Horizontal, true, nullptr);
    gerber_exp_group_zeros_radio->setToolTip(QString::fromUtf8("Esto establece el tipo de ceros Gerber.\nSi LZ entonces los ceros iniciales se eliminan y\nSe guardan los ceros que se arrastran.\nSi se comprueba TZ, se eliminan los ceros finales\ny Leading Zeros se mantienen."));
    gerber_exp_group_zeros_radio->setValue(QVariant(QString::fromUtf8("L")));
    lay33->setWidget(2, QFormLayout::ItemRole(1), gerber_exp_group_zeros_radio);
    gerber_exp_group_layout->addLayout(lay33, 0);
    gerber_exp_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    gerber_exp_group->setLayout(gerber_exp_group_layout);
    vlay->addWidget(gerber_exp_group, 0);
    vlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    layout->addLayout(vlay, 0);
    gerber_editor_group = new QGroupBox(nullptr);
    { QFont f = gerber_editor_group->font(); f.setBold(true); f.setPixelSize(16); gerber_editor_group->setFont(f); }
    gerber_editor_group->setMinimumSize(QSize(200, 0));
    gerber_editor_group_layout = new QVBoxLayout();
    gerber_editor_group_layout->setContentsMargins(9, 9, 9, 9);
    gerber_editor_group_layout->setSpacing(6);
    gerber_editor_group_param_label = new QLabel(nullptr);
    gerber_editor_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    gerber_editor_group_param_label->setToolTip(QString::fromUtf8("Una lista de los parámetros del editor Gerber."));
    gerber_editor_group_layout->addWidget(gerber_editor_group_param_label, 0);
    auto* lay36 = new QGridLayout();
    lay36->setContentsMargins(0, 0, 0, 0);
    lay36->setHorizontalSpacing(6);
    lay36->setVerticalSpacing(6);
    gerber_editor_group_sel_limit_label = new QLabel(nullptr);
    gerber_editor_group_sel_limit_label->setText(QString::fromUtf8("Límite de selección:"));
    gerber_editor_group_sel_limit_label->setToolTip(QString::fromUtf8("Establecer el número de geometría seleccionada de Gerber\nelementos por encima de los cuales la geometría de utilidad\nse convierte en sólo un rectángulo de selección.\nAumenta el rendimiento al mover un\nGran cantidad de elementos geométricos."));
    lay36->addWidget(gerber_editor_group_sel_limit_label, 0, 0, 1, 1);
    gerber_editor_group_sel_limit_entry = new FCSpinner(nullptr);
    gerber_editor_group_sel_limit_entry->setProperty("maximum", 9999);
    gerber_editor_group_sel_limit_entry->setProperty("value", 30);
    lay36->addWidget(gerber_editor_group_sel_limit_entry, 0, 1, 1, 1);
    gerber_editor_group_addcode_entry_lbl = new QLabel(nullptr);
    gerber_editor_group_addcode_entry_lbl->setText(QString::fromUtf8("Nuevo Código de Aper:"));
    gerber_editor_group_addcode_entry_lbl->setToolTip(QString::fromUtf8("Código para la nueva apertura"));
    lay36->addWidget(gerber_editor_group_addcode_entry_lbl, 1, 0, 1, 1);
    gerber_editor_group_addcode_entry = new FCSpinner(nullptr);
    gerber_editor_group_addcode_entry->setProperty("minimum", 10);
    gerber_editor_group_addcode_entry->setProperty("value", 10);
    gerber_editor_group_addcode_entry->setWrapping(true);
    lay36->addWidget(gerber_editor_group_addcode_entry, 1, 1, 1, 1);
    gerber_editor_group_addsize_entry_lbl = new QLabel(nullptr);
    gerber_editor_group_addsize_entry_lbl->setText(QString::fromUtf8("Nuevo Tamaño de Aper:"));
    gerber_editor_group_addsize_entry_lbl->setToolTip(QString::fromUtf8("Tamaño para la Nueva Aper"));
    lay36->addWidget(gerber_editor_group_addsize_entry_lbl, 2, 0, 1, 1);
    gerber_editor_group_addsize_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_addsize_entry->setProperty("decimals", 4);
    gerber_editor_group_addsize_entry->setProperty("maximum", 100.0);
    gerber_editor_group_addsize_entry->setProperty("value", 0.8);
    lay36->addWidget(gerber_editor_group_addsize_entry, 2, 1, 1, 1);
    gerber_editor_group_addtype_combo_lbl = new QLabel(nullptr);
    gerber_editor_group_addtype_combo_lbl->setText(QString::fromUtf8("Nuevo Tipo de Aper:"));
    gerber_editor_group_addtype_combo_lbl->setToolTip(QString::fromUtf8("Escriba para la nueva apertura.\nPuede ser 'C', 'R' u 'O'."));
    lay36->addWidget(gerber_editor_group_addtype_combo_lbl, 3, 0, 1, 1);
    gerber_editor_group_addtype_combo = new FCComboBox(nullptr);
    gerber_editor_group_addtype_combo->setProperty("currentText", QString::fromUtf8("C"));
    gerber_editor_group_addtype_combo->addItem(QString::fromUtf8("C"));
    gerber_editor_group_addtype_combo->addItem(QString::fromUtf8("R"));
    gerber_editor_group_addtype_combo->addItem(QString::fromUtf8("O"));
    lay36->addWidget(gerber_editor_group_addtype_combo, 3, 1, 1, 1);
    gerber_editor_group_grb_array_size_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_size_label->setText(QString::fromUtf8("Nº de almohadillas:"));
    gerber_editor_group_grb_array_size_label->setToolTip(QString::fromUtf8("Especifique cuántos pads estarán en la matriz."));
    lay36->addWidget(gerber_editor_group_grb_array_size_label, 4, 0, 1, 1);
    gerber_editor_group_grb_array_size_entry = new FCSpinner(nullptr);
    gerber_editor_group_grb_array_size_entry->setProperty("maximum", 9999);
    gerber_editor_group_grb_array_size_entry->setProperty("value", 5);
    lay36->addWidget(gerber_editor_group_grb_array_size_entry, 4, 1, 1, 1);
    gerber_editor_group_adddim_label = new QLabel(nullptr);
    gerber_editor_group_adddim_label->setText(QString::fromUtf8("Dim. de apertura:"));
    gerber_editor_group_adddim_label->setToolTip(QString::fromUtf8("Diámetros de las herramientas, separados por comas.\nEl valor del diámetro tiene que usar el separador de decimales de punto.\nValores válidos: 0.3, 1.0"));
    lay36->addWidget(gerber_editor_group_adddim_label, 5, 0, 1, 1);
    gerber_editor_group_adddim_entry = new NumericalEvalTupleEntry(nullptr);
    gerber_editor_group_adddim_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    gerber_editor_group_adddim_entry->setText(QString::fromUtf8("0.5, 0.5"));
    lay36->addWidget(gerber_editor_group_adddim_entry, 5, 1, 1, 1);
    gerber_editor_group_grb_array_linear_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_linear_label->setText(QString::fromUtf8("<b>Matriz lineal de Almohadilla:</b>"));
    lay36->addWidget(gerber_editor_group_grb_array_linear_label, 6, 0, 1, 2);
    gerber_editor_group_grb_axis_label = new QLabel(nullptr);
    gerber_editor_group_grb_axis_label->setText(QString::fromUtf8("Direccion lineal:"));
    gerber_editor_group_grb_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la matriz lineal:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la matriz"));
    lay36->addWidget(gerber_editor_group_grb_axis_label, 7, 0, 1, 1);
    gerber_editor_group_grb_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    gerber_editor_group_grb_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay36->addWidget(gerber_editor_group_grb_axis_radio, 7, 1, 1, 1);
    gerber_editor_group_grb_pitch_label = new QLabel(nullptr);
    gerber_editor_group_grb_pitch_label->setText(QString::fromUtf8("Paso:"));
    gerber_editor_group_grb_pitch_label->setToolTip(QString::fromUtf8("Paso = Distancia entre elementos de la matriz."));
    lay36->addWidget(gerber_editor_group_grb_pitch_label, 8, 0, 1, 1);
    gerber_editor_group_grb_pitch_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_pitch_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_pitch_entry->setProperty("value", 0.1);
    lay36->addWidget(gerber_editor_group_grb_pitch_entry, 8, 1, 1, 1);
    gerber_editor_group_grb_angle_label = new QLabel(nullptr);
    gerber_editor_group_grb_angle_label->setText(QString::fromUtf8("Ángulo:"));
    gerber_editor_group_grb_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    lay36->addWidget(gerber_editor_group_grb_angle_label, 9, 0, 1, 1);
    gerber_editor_group_grb_angle_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_angle_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_angle_entry->setProperty("maximum", 360.0);
    gerber_editor_group_grb_angle_entry->setProperty("minimum", -360.0);
    gerber_editor_group_grb_angle_entry->setProperty("singleStep", 5.0);
    lay36->addWidget(gerber_editor_group_grb_angle_entry, 9, 1, 1, 1);
    gerber_editor_group_grb_array_circ_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_circ_label->setText(QString::fromUtf8("<b>Matriz de Almohadilla Circ:</b>"));
    lay36->addWidget(gerber_editor_group_grb_array_circ_label, 10, 0, 1, 2);
    gerber_editor_group_grb_circular_direction_label = new QLabel(nullptr);
    gerber_editor_group_grb_circular_direction_label->setText(QString::fromUtf8("Dirección circular:"));
    gerber_editor_group_grb_circular_direction_label->setToolTip(QString::fromUtf8("Dirección para matriz circular.\nPuede ser CW = en sentido horario o CCW = en sentido antihorario."));
    lay36->addWidget(gerber_editor_group_grb_circular_direction_label, 11, 0, 1, 1);
    gerber_editor_group_grb_circular_dir_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    gerber_editor_group_grb_circular_dir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay36->addWidget(gerber_editor_group_grb_circular_dir_radio, 11, 1, 1, 1);
    gerber_editor_group_grb_circular_angle_label = new QLabel(nullptr);
    gerber_editor_group_grb_circular_angle_label->setText(QString::fromUtf8("Ángulo circular:"));
    gerber_editor_group_grb_circular_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    lay36->addWidget(gerber_editor_group_grb_circular_angle_label, 12, 0, 1, 1);
    gerber_editor_group_grb_circular_angle_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_circular_angle_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_circular_angle_entry->setProperty("maximum", 360.0);
    gerber_editor_group_grb_circular_angle_entry->setProperty("minimum", -360.0);
    gerber_editor_group_grb_circular_angle_entry->setProperty("singleStep", 5.0);
    lay36->addWidget(gerber_editor_group_grb_circular_angle_entry, 12, 1, 1, 1);
    gerber_editor_group_grb_array_tools_b_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_tools_b_label->setText(QString::fromUtf8("<b>Herramienta Buffer:</b>"));
    lay36->addWidget(gerber_editor_group_grb_array_tools_b_label, 13, 0, 1, 2);
    gerber_editor_group_grb_buff_label = new QLabel(nullptr);
    gerber_editor_group_grb_buff_label->setText(QString::fromUtf8("Dist. de buffer:"));
    gerber_editor_group_grb_buff_label->setToolTip(QString::fromUtf8("Distancia a la que buffer el elemento Gerber."));
    lay36->addWidget(gerber_editor_group_grb_buff_label, 14, 0, 1, 1);
    gerber_editor_group_grb_buff_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_buff_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_buff_entry->setProperty("maximum", 9999.0);
    gerber_editor_group_grb_buff_entry->setProperty("minimum", -9999.0);
    gerber_editor_group_grb_buff_entry->setProperty("value", 0.1);
    lay36->addWidget(gerber_editor_group_grb_buff_entry, 14, 1, 1, 1);
    gerber_editor_group_grb_array_tools_s_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_tools_s_label->setText(QString::fromUtf8("<b>Herramienta de escala:</b>"));
    lay36->addWidget(gerber_editor_group_grb_array_tools_s_label, 15, 0, 1, 2);
    gerber_editor_group_grb_scale_label = new QLabel(nullptr);
    gerber_editor_group_grb_scale_label->setText(QString::fromUtf8("Factor de escala:"));
    gerber_editor_group_grb_scale_label->setToolTip(QString::fromUtf8("Factoriza para escalar el elemento Gerber."));
    lay36->addWidget(gerber_editor_group_grb_scale_label, 16, 0, 1, 1);
    gerber_editor_group_grb_scale_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_scale_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_scale_entry->setProperty("maximum", 9999.0);
    gerber_editor_group_grb_scale_entry->setProperty("value", 1.0);
    lay36->addWidget(gerber_editor_group_grb_scale_entry, 16, 1, 1, 1);
    gerber_editor_group_grb_array_tools_ma_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_tools_ma_label->setText(QString::fromUtf8("<b>Herram. de Zona de Marca:</b>"));
    lay36->addWidget(gerber_editor_group_grb_array_tools_ma_label, 17, 0, 1, 2);
    gerber_editor_group_grb_ma_low_label = new QLabel(nullptr);
    gerber_editor_group_grb_ma_low_label->setText(QString::fromUtf8("Umbral bajo:"));
    gerber_editor_group_grb_ma_low_label->setToolTip(QString::fromUtf8("Valor de umbral por debajo del cual las aberturas no están marcadas."));
    lay36->addWidget(gerber_editor_group_grb_ma_low_label, 18, 0, 1, 1);
    gerber_editor_group_grb_ma_low_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_ma_low_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_ma_low_entry->setProperty("maximum", 9999.0);
    lay36->addWidget(gerber_editor_group_grb_ma_low_entry, 18, 1, 1, 1);
    gerber_editor_group_grb_ma_high_label = new QLabel(nullptr);
    gerber_editor_group_grb_ma_high_label->setText(QString::fromUtf8("Umbral alto:"));
    gerber_editor_group_grb_ma_high_label->setToolTip(QString::fromUtf8("Valor umbral sobre el cual las aberturas no están marcadas."));
    lay36->addWidget(gerber_editor_group_grb_ma_high_label, 19, 0, 1, 1);
    gerber_editor_group_grb_ma_high_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_ma_high_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_ma_high_entry->setProperty("maximum", 9999.0);
    gerber_editor_group_grb_ma_high_entry->setProperty("value", 1.0);
    lay36->addWidget(gerber_editor_group_grb_ma_high_entry, 19, 1, 1, 1);
    gerber_editor_group_layout->addLayout(lay36, 0);
    gerber_editor_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    gerber_editor_group->setLayout(gerber_editor_group_layout);
    layout->addWidget(gerber_editor_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    gerber_defaults_form->setLayout(layout);
    gerber_scroll_area->setWidget(gerber_defaults_form);
    gerber_tab_lay->addWidget(gerber_scroll_area, 0);
    gerber_tab->setLayout(gerber_tab_lay);
    pref_tab_area->addTab(gerber_tab, QString::fromUtf8("GERBER"));
    excellon_tab = new QWidget(nullptr);
    excellon_tab->setProperty("objectName", QString::fromUtf8("excellon_tab"));
    excellon_tab->setHidden(true);
    excellon_tab_lay = new QVBoxLayout();
    excellon_tab_lay->setContentsMargins(2, 2, 2, 2);
    excellon_tab_lay->setSpacing(6);
    excellon_scroll_area = new QScrollArea(nullptr);
    excellon_defaults_form = new QWidget(nullptr);
    excellon_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    excellon_gen_group = new QGroupBox(nullptr);
    { QFont f = excellon_gen_group->font(); f.setBold(true); f.setPixelSize(16); excellon_gen_group->setFont(f); }
    excellon_gen_group->setMinimumSize(QSize(240, 0));
    excellon_gen_group_layout = new QVBoxLayout();
    excellon_gen_group_layout->setContentsMargins(9, 9, 9, 9);
    excellon_gen_group_layout->setSpacing(6);
    excellon_gen_group_plot_options_label = new QLabel(nullptr);
    excellon_gen_group_plot_options_label->setText(QString::fromUtf8("<b>Opciones de parcela:</b>"));
    excellon_gen_group_layout->addWidget(excellon_gen_group_plot_options_label, 0);
    auto* lay37 = new QGridLayout();
    lay37->setContentsMargins(0, 0, 0, 0);
    lay37->setHorizontalSpacing(6);
    lay37->setVerticalSpacing(6);
    excellon_gen_group_plot_cb = new FCCheckBox(nullptr);
    excellon_gen_group_plot_cb->setChecked(true);
    excellon_gen_group_plot_cb->setText(QString::fromUtf8("Gráfico"));
    excellon_gen_group_plot_cb->setToolTip(QString::fromUtf8("Plot (show) this object."));
    lay37->addWidget(excellon_gen_group_plot_cb, 0, 0, 1, 1);
    excellon_gen_group_solid_cb = new FCCheckBox(nullptr);
    excellon_gen_group_solid_cb->setChecked(true);
    excellon_gen_group_solid_cb->setText(QString::fromUtf8("Sólido"));
    excellon_gen_group_solid_cb->setToolTip(QString::fromUtf8("Plot as solid circles."));
    lay37->addWidget(excellon_gen_group_solid_cb, 0, 1, 1, 1);
    excellon_gen_group_multicolored_cb = new FCCheckBox(nullptr);
    excellon_gen_group_multicolored_cb->setText(QString::fromUtf8("M-Color"));
    excellon_gen_group_multicolored_cb->setToolTip(QString::fromUtf8("Dibuja polígonos en diferentes colores."));
    lay37->addWidget(excellon_gen_group_multicolored_cb, 0, 2, 1, 1);
    auto* w38 = new QFrame(nullptr);
    w38->setProperty("frameShadow", 48);
    w38->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w38->setSizePolicy(sp); }
    lay37->addWidget(w38, 1, 0, 1, 3);
    excellon_gen_group_layout->addLayout(lay37, 0);
    auto* lay39 = new QGridLayout();
    lay39->setContentsMargins(0, 0, 0, 0);
    lay39->setHorizontalSpacing(6);
    lay39->setVerticalSpacing(6);
    excellon_gen_group_excellon_format_label = new QLabel(nullptr);
    excellon_gen_group_excellon_format_label->setText(QString::fromUtf8("<b>Formato Excellon:</b>"));
    excellon_gen_group_excellon_format_label->setToolTip(QString::fromUtf8("Los archivos de exploración NC, normalmente denominados archivos Excellon\nSon archivos que se pueden encontrar en diferentes formatos.\nAquí configuramos el formato utilizado cuando el proporcionado\nLas coordenadas no están usando el punto.\n\nPosibles presets:\n\nPROTEO 3: 3 MM LZ\nDipTrace 5: 2 MM TZ\nDipTrace 4: 3 MM LZ\n\nÁGUILA 3: 3 MM TZ\nÁGUILA 4: 3 MM TZ\nÁGUILA 2: 5 PULGADAS TZ\nÁGUILA 3: 5 PULGADAS TZ\n\nALTUM 2: 4 PULGADAS LZ\nSprint Layout 2: 4 PULGADAS LZ\nKiCAD 3: 5 PULGADAS TZ"));
    lay39->addWidget(excellon_gen_group_excellon_format_label, 0, 0, 1, 2);
    excellon_gen_group_excellon_format_in_label = new QLabel(nullptr);
    excellon_gen_group_excellon_format_in_label->setText(QString::fromUtf8("PULGADA:"));
    excellon_gen_group_excellon_format_in_label->setToolTip(QString::fromUtf8("Los valores predeterminados para INCH son 2:4"));
    lay39->addWidget(excellon_gen_group_excellon_format_in_label, 1, 0, 1, 1);
    auto* lay40 = new QHBoxLayout();
    lay40->setContentsMargins(0, 0, 0, 0);
    lay40->setSpacing(6);
    excellon_gen_group_excellon_format_upper_in_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_upper_in_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_upper_in_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_upper_in_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nCoordina toda la parte de Excellon."));
    excellon_gen_group_excellon_format_upper_in_entry->setProperty("value", 2);
    lay40->addWidget(excellon_gen_group_excellon_format_upper_in_entry, 0);
    auto* w41 = new QLabel(nullptr);
    w41->setMaximumSize(QSize(5, 16777215));
    w41->setMinimumSize(QSize(5, 0));
    w41->setText(QString::fromUtf8(":"));
    lay40->addWidget(w41, 0);
    excellon_gen_group_excellon_format_lower_in_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_lower_in_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_lower_in_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_lower_in_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nLa parte decimal de las coordenadas de Excellon."));
    excellon_gen_group_excellon_format_lower_in_entry->setProperty("value", 4);
    lay40->addWidget(excellon_gen_group_excellon_format_lower_in_entry, 0);
    lay39->addLayout(lay40, 1, 1, 1, 1);
    excellon_gen_group_excellon_format_mm_label = new QLabel(nullptr);
    excellon_gen_group_excellon_format_mm_label->setText(QString::fromUtf8("MÉTRICO:"));
    excellon_gen_group_excellon_format_mm_label->setToolTip(QString::fromUtf8("Los valores predeterminados para Métrica son 3: 3"));
    lay39->addWidget(excellon_gen_group_excellon_format_mm_label, 2, 0, 1, 1);
    auto* lay42 = new QHBoxLayout();
    lay42->setContentsMargins(0, 0, 0, 0);
    lay42->setSpacing(6);
    excellon_gen_group_excellon_format_upper_mm_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_upper_mm_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_upper_mm_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_upper_mm_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nCoordina toda la parte de Excellon."));
    excellon_gen_group_excellon_format_upper_mm_entry->setProperty("value", 3);
    lay42->addWidget(excellon_gen_group_excellon_format_upper_mm_entry, 0);
    auto* w43 = new QLabel(nullptr);
    w43->setMaximumSize(QSize(5, 16777215));
    w43->setMinimumSize(QSize(5, 0));
    w43->setText(QString::fromUtf8(":"));
    lay42->addWidget(w43, 1);
    excellon_gen_group_excellon_format_lower_mm_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_lower_mm_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_lower_mm_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_lower_mm_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nLa parte decimal de las coordenadas de Excellon."));
    excellon_gen_group_excellon_format_lower_mm_entry->setProperty("value", 3);
    lay42->addWidget(excellon_gen_group_excellon_format_lower_mm_entry, 0);
    lay39->addLayout(lay42, 2, 1, 1, 1);
    excellon_gen_group_excellon_zeros_label = new QLabel(nullptr);
    excellon_gen_group_excellon_zeros_label->setProperty("alignment", 1);
    excellon_gen_group_excellon_zeros_label->setText(QString::fromUtf8("Ceros:"));
    excellon_gen_group_excellon_zeros_label->setToolTip(QString::fromUtf8("Esto establece el tipo de ceros Excellon.\nSi LZ entonces Leading Zeros se mantienen y\nSe eliminan los ceros finales.\nSi se comprueba TZ, se mantienen los ceros finales.\ny Leading Zeros se eliminan.\n\nEsto se usa cuando no hay información\nalmacenado en el archivo Excellon."));
    lay39->addWidget(excellon_gen_group_excellon_zeros_label, 3, 0, 1, 1);
    excellon_gen_group_excellon_zeros_radio = new RadioSet({{QString::fromUtf8("LZ"), QVariant(QString::fromUtf8("L"))}, {QString::fromUtf8("TZ"), QVariant(QString::fromUtf8("T"))}}, Qt::Horizontal, true, nullptr);
    excellon_gen_group_excellon_zeros_radio->setValue(QVariant(QString::fromUtf8("T")));
    lay39->addWidget(excellon_gen_group_excellon_zeros_radio, 3, 1, 1, 1);
    excellon_gen_group_excellon_units_label = new QLabel(nullptr);
    excellon_gen_group_excellon_units_label->setProperty("alignment", 1);
    excellon_gen_group_excellon_units_label->setText(QString::fromUtf8("Unidades:"));
    excellon_gen_group_excellon_units_label->setToolTip(QString::fromUtf8("Esto establece las unidades predeterminadas de los archivos de Excellon.\nSi no se detecta en el archivo analizado el valor aquí\nserán utilizados. Algunos archivos de Excellon no tienen un encabezado\npor lo tanto este parámetro será utilizado."));
    lay39->addWidget(excellon_gen_group_excellon_units_label, 4, 0, 1, 1);
    excellon_gen_group_excellon_units_radio = new RadioSet({{QString::fromUtf8("Pulgada"), QVariant(QString::fromUtf8("INCH"))}, {QString::fromUtf8("mm"), QVariant(QString::fromUtf8("METRIC"))}}, Qt::Horizontal, true, nullptr);
    excellon_gen_group_excellon_units_radio->setToolTip(QString::fromUtf8("Esto establece las unidades de archivos de Excellon.\nAlgunos archivos de Excellon no tienen un encabezado\npor lo tanto este parámetro será utilizado."));
    excellon_gen_group_excellon_units_radio->setValue(QVariant(QString::fromUtf8("INCH")));
    lay39->addWidget(excellon_gen_group_excellon_units_radio, 4, 1, 1, 1);
    excellon_gen_group_update_excellon_cb = new FCCheckBox(nullptr);
    excellon_gen_group_update_excellon_cb->setChecked(true);
    excellon_gen_group_update_excellon_cb->setText(QString::fromUtf8("Actualizar configuración de exportación"));
    excellon_gen_group_update_excellon_cb->setToolTip(QString::fromUtf8("If checked, the Excellon Export settings will be updated with the ones above."));
    lay39->addWidget(excellon_gen_group_update_excellon_cb, 5, 0, 1, 2);
    excellon_gen_group_excellon_defaults_button = new QPushButton(nullptr);
    excellon_gen_group_excellon_defaults_button->setMinimumSize(QSize(80, 0));
    excellon_gen_group_excellon_defaults_button->setText(QString::fromUtf8("Restaurar los valores predeterminados"));
    lay39->addWidget(excellon_gen_group_excellon_defaults_button, 6, 0, 1, 2);
    auto* w44 = new QFrame(nullptr);
    w44->setProperty("frameShadow", 48);
    w44->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w44->setSizePolicy(sp); }
    lay39->addWidget(w44, 7, 0, 1, 2);
    excellon_gen_group_excellon_general_label = new QLabel(nullptr);
    excellon_gen_group_excellon_general_label->setText(QString::fromUtf8("<b>Optimización de ruta:</b>"));
    lay39->addWidget(excellon_gen_group_excellon_general_label, 8, 0, 1, 2);
    excellon_gen_group_excellon_optimization_label = new QLabel(nullptr);
    excellon_gen_group_excellon_optimization_label->setText(QString::fromUtf8("Algoritmo:"));
    excellon_gen_group_excellon_optimization_label->setToolTip(QString::fromUtf8("Esto establece el tipo de optimización para la ruta de perforación Excellon.\nSi <<MetaHeuristic>> está marcado, el algoritmo de Google OR-Tools con\nSe utiliza la ruta local guiada metaheurística. El tiempo de búsqueda predeterminado es de 3 segundos.\nSi <<Básico>> está marcado, se utiliza el algoritmo básico de Google OR-Tools.\nSi se marca <<TSA>>, se utiliza el algoritmo de vendedor ambulante para\nOptimización de la ruta de perforación.\n\nAlgunas opciones están deshabilitadas cuando la aplicación funciona en modo de 32 bits."));
    lay39->addWidget(excellon_gen_group_excellon_optimization_label, 9, 0, 1, 1);
    excellon_gen_group_excellon_optimization_radio = new RadioSet({{QString::fromUtf8("MetaHeuristic"), QVariant(QString::fromUtf8("M"))}, {QString::fromUtf8("BASIC"), QVariant(QString::fromUtf8("B"))}, {QString::fromUtf8("TSA"), QVariant(QString::fromUtf8("T"))}}, Qt::Vertical, false, nullptr);
    excellon_gen_group_excellon_optimization_radio->setValue(QVariant(QString::fromUtf8("B")));
    lay39->addWidget(excellon_gen_group_excellon_optimization_radio, 9, 1, 1, 1);
    excellon_gen_group_optimization_time_label = new QLabel(nullptr);
    excellon_gen_group_optimization_time_label->setProperty("alignment", 1);
    excellon_gen_group_optimization_time_label->setEnabled(false);
    excellon_gen_group_optimization_time_label->setText(QString::fromUtf8("Duración:"));
    excellon_gen_group_optimization_time_label->setToolTip(QString::fromUtf8("Cuando OR-Tools Metaheuristic (MH) está habilitado, hay un\numbral máximo de cuánto tiempo se dedica a hacer el\nOptimización del camino. Esta duración máxima se establece aquí.\nEn segundos."));
    lay39->addWidget(excellon_gen_group_optimization_time_label, 10, 0, 1, 1);
    excellon_gen_group_optimization_time_entry = new FCSpinner(nullptr);
    excellon_gen_group_optimization_time_entry->setEnabled(false);
    excellon_gen_group_optimization_time_entry->setProperty("maximum", 999);
    excellon_gen_group_optimization_time_entry->setProperty("value", 3);
    lay39->addWidget(excellon_gen_group_optimization_time_entry, 10, 1, 1, 1);
    auto* w45 = new QFrame(nullptr);
    w45->setProperty("frameShadow", 48);
    w45->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w45->setSizePolicy(sp); }
    lay39->addWidget(w45, 11, 0, 1, 2);
    excellon_gen_group_join_geo_label = new QLabel(nullptr);
    excellon_gen_group_join_geo_label->setText(QString::fromUtf8("<b>Opción de unirse</b>:"));
    lay39->addWidget(excellon_gen_group_join_geo_label, 12, 0, 1, 2);
    excellon_gen_group_fuse_tools_cb = new FCCheckBox(nullptr);
    excellon_gen_group_fuse_tools_cb->setChecked(true);
    excellon_gen_group_fuse_tools_cb->setText(QString::fromUtf8("Fusionar las herramientas"));
    excellon_gen_group_fuse_tools_cb->setToolTip(QString::fromUtf8("Cuando se marca, las herramientas se fusionarán\npero solo si comparten algunos de sus atributos."));
    lay39->addWidget(excellon_gen_group_fuse_tools_cb, 13, 0, 1, 2);
    auto* w46 = new QFrame(nullptr);
    w46->setProperty("frameShadow", 48);
    w46->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w46->setSizePolicy(sp); }
    lay39->addWidget(w46, 14, 0, 1, 2);
    excellon_gen_group_gerber_color_label = new QLabel(nullptr);
    excellon_gen_group_gerber_color_label->setText(QString::fromUtf8("<b>Color del objeto</b>"));
    lay39->addWidget(excellon_gen_group_gerber_color_label, 17, 0, 1, 2);
    excellon_gen_group_line_color_label = new QLabel(nullptr);
    excellon_gen_group_line_color_label->setText(QString::fromUtf8("Contorno:"));
    excellon_gen_group_line_color_label->setToolTip(QString::fromUtf8("Establecer el color de la línea para los objetos trazados."));
    lay39->addWidget(excellon_gen_group_line_color_label, 19, 0, 1, 1);
    excellon_gen_group_line_color_entry = new FCColorEntry(nullptr);
    excellon_gen_group_line_color_entry->setValue(QString::fromUtf8("#750000bf"));
    lay39->addWidget(excellon_gen_group_line_color_entry, 19, 1, 1, 1);
    excellon_gen_group_fill_color_label = new QLabel(nullptr);
    excellon_gen_group_fill_color_label->setText(QString::fromUtf8("Llenado:"));
    excellon_gen_group_fill_color_label->setToolTip(QString::fromUtf8("Establecer el color de relleno para los objetos trazados.\nLos primeros 6 dígitos son el color y los 2 últimos.\nLos dígitos son para el nivel alfa (transparencia)."));
    lay39->addWidget(excellon_gen_group_fill_color_label, 22, 0, 1, 1);
    excellon_gen_group_fill_color_entry = new FCColorEntry(nullptr);
    excellon_gen_group_fill_color_entry->setValue(QString::fromUtf8("#C40000bf"));
    lay39->addWidget(excellon_gen_group_fill_color_entry, 22, 1, 1, 1);
    excellon_gen_group_excellon_alpha_label = new QLabel(nullptr);
    excellon_gen_group_excellon_alpha_label->setText(QString::fromUtf8("Alfa:"));
    excellon_gen_group_excellon_alpha_label->setToolTip(QString::fromUtf8("Establecer la transparencia de relleno para los objetos trazados."));
    lay39->addWidget(excellon_gen_group_excellon_alpha_label, 24, 0, 1, 1);
    excellon_gen_group_excellon_alpha_entry = new FCSliderWithSpinner(0, 255, 1, nullptr);
    excellon_gen_group_excellon_alpha_entry->setValue(191);
    lay39->addWidget(excellon_gen_group_excellon_alpha_entry, 24, 1, 1, 1);
    lay39->setColumnStretch(1, 1);
    excellon_gen_group_layout->addLayout(lay39, 0);
    excellon_gen_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    excellon_gen_group->setLayout(excellon_gen_group_layout);
    layout->addWidget(excellon_gen_group, 0);
    vlay = new QVBoxLayout();
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(6);
    excellon_opt_group = new QGroupBox(nullptr);
    { QFont f = excellon_opt_group->font(); f.setBold(true); f.setPixelSize(16); excellon_opt_group->setFont(f); }
    excellon_opt_group->setMinimumSize(QSize(290, 0));
    excellon_opt_group_layout = new QVBoxLayout();
    excellon_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    excellon_opt_group_layout->setSpacing(6);
    excellon_opt_group_cncjob_label = new QLabel(nullptr);
    excellon_opt_group_cncjob_label->setText(QString::fromUtf8("<b>Crear CNCJob</b>"));
    excellon_opt_group_cncjob_label->setToolTip(QString::fromUtf8("Parámetros utilizados para crear un objeto de trabajo CNC\npara este objeto taladro."));
    excellon_opt_group_layout->addWidget(excellon_opt_group_cncjob_label, 0);
    auto* lay47 = new QGridLayout();
    lay47->setContentsMargins(0, 0, 0, 0);
    lay47->setHorizontalSpacing(6);
    lay47->setVerticalSpacing(6);
    excellon_opt_group_operation_label = new QLabel(nullptr);
    excellon_opt_group_operation_label->setText(QString::fromUtf8("<b>Operación:</b>"));
    excellon_opt_group_operation_label->setToolTip(QString::fromUtf8("Tipo de operación:\n- Perforación -> perforará las perforaciones / ranuras asociadas con esta herramienta\n- Fresado -> fresará los taladros / ranuras"));
    lay47->addWidget(excellon_opt_group_operation_label, 0, 0, 1, 1);
    excellon_opt_group_operation_radio = new RadioSet({{QString::fromUtf8("Perforación"), QVariant(QString::fromUtf8("drill"))}, {QString::fromUtf8("Fresado"), QVariant(QString::fromUtf8("mill"))}}, Qt::Horizontal, true, nullptr);
    excellon_opt_group_operation_radio->setValue(QVariant(QString::fromUtf8("drill")));
    lay47->addWidget(excellon_opt_group_operation_radio, 0, 1, 1, 1);
    excellon_opt_group_mill_type_label = new QLabel(nullptr);
    excellon_opt_group_mill_type_label->setText(QString::fromUtf8("Tipo de fresado:"));
    excellon_opt_group_mill_type_label->setToolTip(QString::fromUtf8("Tipo de fresado:\n- Taladros -> fresará los taladros asociados con esta herramienta\n- Ranuras -> fresará las ranuras asociadas con esta herramienta\n- Ambos -> fresarán taladros y molinos o lo que esté disponible"));
    lay47->addWidget(excellon_opt_group_mill_type_label, 1, 0, 1, 1);
    excellon_opt_group_milling_type_radio = new RadioSet({{QString::fromUtf8("Taladros"), QVariant(QString::fromUtf8("drills"))}, {QString::fromUtf8("Muesca"), QVariant(QString::fromUtf8("slots"))}, {QString::fromUtf8("Ambas"), QVariant(QString::fromUtf8("both"))}}, Qt::Horizontal, true, nullptr);
    excellon_opt_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("drills")));
    lay47->addWidget(excellon_opt_group_milling_type_radio, 1, 1, 1, 1);
    excellon_opt_group_mill_dia_label = new QLabel(nullptr);
    excellon_opt_group_mill_dia_label->setText(QString::fromUtf8("Diá. de fresado:"));
    excellon_opt_group_mill_dia_label->setToolTip(QString::fromUtf8("El diámetro de la herramienta que hará el fresado"));
    lay47->addWidget(excellon_opt_group_mill_dia_label, 2, 0, 1, 1);
    excellon_opt_group_mill_dia_entry = new FCDoubleSpinner(nullptr);
    excellon_opt_group_mill_dia_entry->setProperty("decimals", 4);
    excellon_opt_group_mill_dia_entry->setProperty("maximum", 10000.0);
    excellon_opt_group_mill_dia_entry->setProperty("value", 0.8);
    lay47->addWidget(excellon_opt_group_mill_dia_entry, 2, 1, 1, 1);
    excellon_opt_group_mill_hole_label = new QLabel(nullptr);
    excellon_opt_group_mill_hole_label->setText(QString::fromUtf8("<b>Agujeros de molino</b>"));
    excellon_opt_group_mill_hole_label->setToolTip(QString::fromUtf8("Crear geometría para fresar agujeros."));
    lay47->addWidget(excellon_opt_group_mill_hole_label, 16, 0, 1, 2);
    auto* w48 = new QLabel(nullptr);
    w48->setText(QString::fromUtf8("Diá de la herra. de Perfor:"));
    w48->setToolTip(QString::fromUtf8("Diá. de la herramienta de corte."));
    lay47->addWidget(w48, 18, 0, 1, 1);
    excellon_opt_group_tooldia_entry = new FCDoubleSpinner(nullptr);
    excellon_opt_group_tooldia_entry->setProperty("decimals", 4);
    excellon_opt_group_tooldia_entry->setProperty("maximum", 999.9999);
    excellon_opt_group_tooldia_entry->setProperty("value", 0.8);
    lay47->addWidget(excellon_opt_group_tooldia_entry, 18, 1, 1, 1);
    auto* w49 = new QLabel(nullptr);
    w49->setText(QString::fromUtf8("Diá. de la herra. de ranura:"));
    w49->setToolTip(QString::fromUtf8("Diámetro de la herramienta de corte\nAl fresar ranuras."));
    lay47->addWidget(w49, 21, 0, 1, 1);
    excellon_opt_group_slot_tooldia_entry = new FCDoubleSpinner(nullptr);
    excellon_opt_group_slot_tooldia_entry->setProperty("decimals", 4);
    excellon_opt_group_slot_tooldia_entry->setProperty("maximum", 999.9999);
    excellon_opt_group_slot_tooldia_entry->setProperty("value", 1.8);
    lay47->addWidget(excellon_opt_group_slot_tooldia_entry, 21, 1, 1, 1);
    lay47->setColumnStretch(1, 1);
    excellon_opt_group_layout->addLayout(lay47, 0);
    excellon_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    excellon_opt_group->setLayout(excellon_opt_group_layout);
    vlay->addWidget(excellon_opt_group, 0);
    excellon_adv_opt_group = new QGroupBox(nullptr);
    { QFont f = excellon_adv_opt_group->font(); f.setBold(true); f.setPixelSize(16); excellon_adv_opt_group->setFont(f); }
    excellon_adv_opt_group->setMinimumSize(QSize(250, 0));
    excellon_adv_opt_group_layout = new QVBoxLayout();
    excellon_adv_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    excellon_adv_opt_group_layout->setSpacing(6);
    excellon_adv_opt_group_exc_label = new QLabel(nullptr);
    excellon_adv_opt_group_exc_label->setText(QString::fromUtf8("<b>Opciones avanzadas:</b>"));
    excellon_adv_opt_group_exc_label->setToolTip(QString::fromUtf8("Una lista de los parámetros avanzados.\nEsos parámetros están disponibles sólo para\nAplicación avanzada Nivel."));
    excellon_adv_opt_group_layout->addWidget(excellon_adv_opt_group_exc_label, 0);
    auto* lay50 = new QGridLayout();
    lay50->setContentsMargins(0, 0, 0, 0);
    lay50->setHorizontalSpacing(6);
    lay50->setVerticalSpacing(6);
    excellon_adv_opt_group_table_visibility_cb = new FCCheckBox(nullptr);
    excellon_adv_opt_group_table_visibility_cb->setChecked(true);
    excellon_adv_opt_group_table_visibility_cb->setText(QString::fromUtf8("Mostrar / ocultar tabla"));
    excellon_adv_opt_group_table_visibility_cb->setToolTip(QString::fromUtf8("Alternar la visualización de la tabla de herramientas."));
    lay50->addWidget(excellon_adv_opt_group_table_visibility_cb, 0, 0, 1, 2);
    excellon_adv_opt_group_autoload_db_cb = new FCCheckBox(nullptr);
    excellon_adv_opt_group_autoload_db_cb->setText(QString::fromUtf8("Carga auto. desde DB"));
    excellon_adv_opt_group_autoload_db_cb->setToolTip(QString::fromUtf8("Reemplazo automático de las herramientas de las herramientas de aplicación relacionadas\ncon herramientas de DB que tienen un valor de diámetro cercano."));
    lay50->addWidget(excellon_adv_opt_group_autoload_db_cb, 1, 0, 1, 2);
    lay50->setColumnStretch(1, 1);
    excellon_adv_opt_group_layout->addLayout(lay50, 0);
    excellon_adv_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    excellon_adv_opt_group->setLayout(excellon_adv_opt_group_layout);
    vlay->addWidget(excellon_adv_opt_group, 0);
    excellon_exp_group = new QGroupBox(nullptr);
    { QFont f = excellon_exp_group->font(); f.setBold(true); f.setPixelSize(16); excellon_exp_group->setFont(f); }
    excellon_exp_group->setMinimumSize(QSize(250, 0));
    excellon_exp_group_layout = new QVBoxLayout();
    excellon_exp_group_layout->setContentsMargins(9, 9, 9, 9);
    excellon_exp_group_layout->setSpacing(6);
    excellon_exp_group_export_options_label = new QLabel(nullptr);
    excellon_exp_group_export_options_label->setText(QString::fromUtf8("<b>Opciones de export:</b>"));
    excellon_exp_group_export_options_label->setToolTip(QString::fromUtf8("Los parámetros establecidos aquí se utilizan en el archivo exportado.\ncuando se utiliza la entrada de menú Archivo -> Exportar -> Exportar Excellon."));
    excellon_exp_group_layout->addWidget(excellon_exp_group_export_options_label, 0);
    auto* lay51 = new QFormLayout();
    lay51->setContentsMargins(0, 0, 0, 0);
    lay51->setHorizontalSpacing(6);
    lay51->setVerticalSpacing(6);
    excellon_exp_group_excellon_units_label = new QLabel(nullptr);
    excellon_exp_group_excellon_units_label->setText(QString::fromUtf8("Unidades:"));
    excellon_exp_group_excellon_units_label->setToolTip(QString::fromUtf8("Las unidades utilizadas en el archivo Excellon."));
    lay51->setWidget(0, QFormLayout::ItemRole(0), excellon_exp_group_excellon_units_label);
    excellon_exp_group_excellon_units_radio = new RadioSet({{QString::fromUtf8("Pulgada"), QVariant(QString::fromUtf8("INCH"))}, {QString::fromUtf8("mm"), QVariant(QString::fromUtf8("METRIC"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_excellon_units_radio->setToolTip(QString::fromUtf8("Las unidades utilizadas en el archivo Excellon."));
    excellon_exp_group_excellon_units_radio->setValue(QVariant(QString::fromUtf8("INCH")));
    lay51->setWidget(0, QFormLayout::ItemRole(1), excellon_exp_group_excellon_units_radio);
    excellon_exp_group_digits_label = new QLabel(nullptr);
    excellon_exp_group_digits_label->setText(QString::fromUtf8("Entero/Decimales:"));
    excellon_exp_group_digits_label->setToolTip(QString::fromUtf8("Los archivos de exploración NC, normalmente denominados archivos Excellon\nSon archivos que se pueden encontrar en diferentes formatos.\nAquí configuramos el formato utilizado cuando el proporcionado\nLas coordenadas no están usando el punto."));
    lay51->setWidget(1, QFormLayout::ItemRole(0), excellon_exp_group_digits_label);
    auto* lay52 = new QHBoxLayout();
    lay52->setContentsMargins(0, 0, 0, 0);
    lay52->setSpacing(6);
    excellon_exp_group_format_whole_entry = new FCSpinner(nullptr);
    excellon_exp_group_format_whole_entry->setProperty("maximum", 9);
    excellon_exp_group_format_whole_entry->setMinimumSize(QSize(30, 0));
    excellon_exp_group_format_whole_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nCoordina toda la parte de Excellon."));
    excellon_exp_group_format_whole_entry->setProperty("value", 2);
    lay52->addWidget(excellon_exp_group_format_whole_entry, 1);
    auto* w53 = new QLabel(nullptr);
    w53->setMaximumSize(QSize(5, 16777215));
    w53->setMinimumSize(QSize(5, 0));
    w53->setText(QString::fromUtf8(":"));
    lay52->addWidget(w53, 1);
    excellon_exp_group_format_dec_entry = new FCSpinner(nullptr);
    excellon_exp_group_format_dec_entry->setProperty("maximum", 9);
    excellon_exp_group_format_dec_entry->setMinimumSize(QSize(30, 0));
    excellon_exp_group_format_dec_entry->setToolTip(QString::fromUtf8("Estos números significan el número de dígitos en\nLa parte decimal de las coordenadas de Excellon."));
    excellon_exp_group_format_dec_entry->setProperty("value", 4);
    lay52->addWidget(excellon_exp_group_format_dec_entry, 1);
    lay52->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    lay51->setLayout(1, QFormLayout::ItemRole(1), lay52);
    excellon_exp_group_format_label = new QLabel(nullptr);
    excellon_exp_group_format_label->setText(QString::fromUtf8("Formato:"));
    excellon_exp_group_format_label->setToolTip(QString::fromUtf8("Seleccione el tipo de formato de coordenadas utilizado.\nLas coordenadas se pueden guardar con punto decimal o sin.\nCuando no hay un punto decimal, se requiere especificar\nel número de dígitos para la parte entera y el número de decimales.\nTambién deberá especificarse si LZ = ceros iniciales se mantienen\no TZ = ceros finales se mantienen."));
    lay51->setWidget(2, QFormLayout::ItemRole(0), excellon_exp_group_format_label);
    excellon_exp_group_format_radio = new RadioSet({{QString::fromUtf8("Decimal"), QVariant(QString::fromUtf8("dec"))}, {QString::fromUtf8("Sin-Decimal"), QVariant(QString::fromUtf8("ndec"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_format_radio->setToolTip(QString::fromUtf8("Seleccione el tipo de formato de coordenadas utilizado.\nLas coordenadas se pueden guardar con punto decimal o sin.\nCuando no hay un punto decimal, se requiere especificar\nel número de dígitos para la parte entera y el número de decimales.\nTambién deberá especificarse si LZ = ceros iniciales se mantienen\no TZ = ceros finales se mantienen."));
    excellon_exp_group_format_radio->setValue(QVariant(QString::fromUtf8("dec")));
    lay51->setWidget(2, QFormLayout::ItemRole(1), excellon_exp_group_format_radio);
    excellon_exp_group_zeros_label = new QLabel(nullptr);
    excellon_exp_group_zeros_label->setProperty("alignment", 1);
    excellon_exp_group_zeros_label->setEnabled(false);
    excellon_exp_group_zeros_label->setText(QString::fromUtf8("Ceros:"));
    excellon_exp_group_zeros_label->setToolTip(QString::fromUtf8("Esto establece el tipo de ceros Excellon.\nSi LZ entonces Leading Zeros se mantienen y\nSe eliminan los ceros finales.\nSi se comprueba TZ, se mantienen los ceros finales.\ny Leading Zeros se eliminan."));
    lay51->setWidget(3, QFormLayout::ItemRole(0), excellon_exp_group_zeros_label);
    excellon_exp_group_zeros_radio = new RadioSet({{QString::fromUtf8("LZ"), QVariant(QString::fromUtf8("LZ"))}, {QString::fromUtf8("TZ"), QVariant(QString::fromUtf8("TZ"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_zeros_radio->setEnabled(false);
    excellon_exp_group_zeros_radio->setToolTip(QString::fromUtf8("Esto establece el tipo predeterminado de ceros Excellon.\nSi LZ entonces los ceros iniciales se mantienen y\nSe eliminan los ceros finales.\nSi se comprueba TZ, se mantienen los ceros finales.\ny se eliminan los ceros iniciales."));
    excellon_exp_group_zeros_radio->setValue(QVariant(QString::fromUtf8("LZ")));
    lay51->setWidget(3, QFormLayout::ItemRole(1), excellon_exp_group_zeros_radio);
    excellon_exp_group_slot_type_label = new QLabel(nullptr);
    excellon_exp_group_slot_type_label->setProperty("alignment", 1);
    excellon_exp_group_slot_type_label->setText(QString::fromUtf8("Tipo de ranura:"));
    excellon_exp_group_slot_type_label->setToolTip(QString::fromUtf8("Esto establece cómo se exportarán las ranuras.\nSi se enruta, las ranuras se enrutarán\nutilizando los comandos M15 / M16.\nSi PERFORADO (G85), las ranuras se exportarán\nutilizando el comando Ranura perforada (G85)."));
    lay51->setWidget(4, QFormLayout::ItemRole(0), excellon_exp_group_slot_type_label);
    excellon_exp_group_slot_type_radio = new RadioSet({{QString::fromUtf8("Enrutado"), QVariant(QString::fromUtf8("routing"))}, {QString::fromUtf8("Perforado (G85)"), QVariant(QString::fromUtf8("drilling"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_slot_type_radio->setToolTip(QString::fromUtf8("Esto establece cómo se exportarán las ranuras.\nSi se enruta, las ranuras se enrutarán\nutilizando los comandos M15 / M16.\nSi PERFORADO (G85), las ranuras se exportarán\nutilizando el comando Ranura perforada (G85)."));
    excellon_exp_group_slot_type_radio->setValue(QVariant(QString::fromUtf8("routing")));
    lay51->setWidget(4, QFormLayout::ItemRole(1), excellon_exp_group_slot_type_radio);
    excellon_exp_group_layout->addLayout(lay51, 0);
    excellon_exp_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    excellon_exp_group->setLayout(excellon_exp_group_layout);
    vlay->addWidget(excellon_exp_group, 0);
    layout->addLayout(vlay, 0);
    excellon_editor_group = new QGroupBox(nullptr);
    { QFont f = excellon_editor_group->font(); f.setBold(true); f.setPixelSize(16); excellon_editor_group->setFont(f); }
    excellon_editor_group->setMinimumSize(QSize(260, 0));
    excellon_editor_group_layout = new QVBoxLayout();
    excellon_editor_group_layout->setContentsMargins(9, 9, 9, 9);
    excellon_editor_group_layout->setSpacing(6);
    excellon_editor_group_param_label = new QLabel(nullptr);
    excellon_editor_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    excellon_editor_group_param_label->setToolTip(QString::fromUtf8("Una lista de los parámetros de Excellon Editor."));
    excellon_editor_group_layout->addWidget(excellon_editor_group_param_label, 0);
    auto* lay54 = new QGridLayout();
    lay54->setContentsMargins(0, 0, 0, 0);
    lay54->setHorizontalSpacing(6);
    lay54->setVerticalSpacing(6);
    excellon_editor_group_sel_limit_label = new QLabel(nullptr);
    excellon_editor_group_sel_limit_label->setText(QString::fromUtf8("Límite de selección:"));
    excellon_editor_group_sel_limit_label->setToolTip(QString::fromUtf8("Establecer el número de geometría de Excellon seleccionada\nelementos por encima de los cuales la geometría de utilidad\nse convierte en sólo un rectángulo de selección.\nAumenta el rendimiento al mover un\nGran cantidad de elementos geométricos."));
    lay54->addWidget(excellon_editor_group_sel_limit_label, 0, 0, 1, 1);
    excellon_editor_group_sel_limit_entry = new FCSpinner(nullptr);
    excellon_editor_group_sel_limit_entry->setProperty("maximum", 99999);
    excellon_editor_group_sel_limit_entry->setProperty("value", 30);
    lay54->addWidget(excellon_editor_group_sel_limit_entry, 0, 1, 1, 1);
    excellon_editor_group_addtool_entry_lbl = new QLabel(nullptr);
    excellon_editor_group_addtool_entry_lbl->setText(QString::fromUtf8("Nuevo dia:"));
    excellon_editor_group_addtool_entry_lbl->setToolTip(QString::fromUtf8("Diámetro para la nueva herramienta"));
    lay54->addWidget(excellon_editor_group_addtool_entry_lbl, 1, 0, 1, 1);
    excellon_editor_group_addtool_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_addtool_entry->setProperty("decimals", 4);
    excellon_editor_group_addtool_entry->setProperty("maximum", 99.9999);
    excellon_editor_group_addtool_entry->setProperty("value", 1.0);
    lay54->addWidget(excellon_editor_group_addtool_entry, 1, 1, 1, 1);
    excellon_editor_group_drill_array_size_label = new QLabel(nullptr);
    excellon_editor_group_drill_array_size_label->setText(QString::fromUtf8("Nu. de ejercicios:"));
    excellon_editor_group_drill_array_size_label->setToolTip(QString::fromUtf8("Especifique cuántas ranuras debe haber en la matriz."));
    lay54->addWidget(excellon_editor_group_drill_array_size_label, 2, 0, 1, 1);
    excellon_editor_group_drill_array_size_entry = new FCSpinner(nullptr);
    excellon_editor_group_drill_array_size_entry->setProperty("maximum", 9999);
    excellon_editor_group_drill_array_size_entry->setProperty("value", 5);
    lay54->addWidget(excellon_editor_group_drill_array_size_entry, 2, 1, 1, 1);
    excellon_editor_group_drill_array_linear_label = new QLabel(nullptr);
    excellon_editor_group_drill_array_linear_label->setText(QString::fromUtf8("<b>Matriz de taladro lineal:</b>"));
    lay54->addWidget(excellon_editor_group_drill_array_linear_label, 3, 0, 1, 2);
    excellon_editor_group_drill_axis_label = new QLabel(nullptr);
    excellon_editor_group_drill_axis_label->setText(QString::fromUtf8("Direccion lineal:"));
    excellon_editor_group_drill_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la matriz lineal:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la matriz"));
    lay54->addWidget(excellon_editor_group_drill_axis_label, 4, 0, 1, 1);
    excellon_editor_group_drill_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_drill_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay54->addWidget(excellon_editor_group_drill_axis_radio, 4, 1, 1, 1);
    excellon_editor_group_drill_pitch_label = new QLabel(nullptr);
    excellon_editor_group_drill_pitch_label->setText(QString::fromUtf8("Paso:"));
    excellon_editor_group_drill_pitch_label->setToolTip(QString::fromUtf8("Paso = Distancia entre elementos de la matriz."));
    lay54->addWidget(excellon_editor_group_drill_pitch_label, 5, 0, 1, 1);
    excellon_editor_group_drill_pitch_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_drill_pitch_entry->setProperty("decimals", 4);
    excellon_editor_group_drill_pitch_entry->setProperty("maximum", 360.0);
    excellon_editor_group_drill_pitch_entry->setProperty("minimum", -360.0);
    excellon_editor_group_drill_pitch_entry->setProperty("value", 2.54);
    lay54->addWidget(excellon_editor_group_drill_pitch_entry, 5, 1, 1, 1);
    excellon_editor_group_drill_angle_label = new QLabel(nullptr);
    excellon_editor_group_drill_angle_label->setText(QString::fromUtf8("Ángulo:"));
    excellon_editor_group_drill_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    lay54->addWidget(excellon_editor_group_drill_angle_label, 6, 0, 1, 1);
    excellon_editor_group_drill_angle_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_drill_angle_entry->setProperty("singleStep", 5.0);
    excellon_editor_group_drill_angle_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_drill_angle_entry, 6, 1, 1, 1);
    auto* w55 = new QLabel(nullptr);
    w55->setText(QString::fromUtf8("<b>Matriz de Taladro Circ:</b>"));
    lay54->addWidget(w55, 7, 0, 1, 2);
    excellon_editor_group_drill_circular_direction_label = new QLabel(nullptr);
    excellon_editor_group_drill_circular_direction_label->setText(QString::fromUtf8("Dirección circular:"));
    excellon_editor_group_drill_circular_direction_label->setToolTip(QString::fromUtf8("Dirección para matriz circular.\nPuede ser CW = en sentido horario o CCW = en sentido antihorario."));
    lay54->addWidget(excellon_editor_group_drill_circular_direction_label, 8, 0, 1, 1);
    excellon_editor_group_drill_circular_dir_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_drill_circular_dir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay54->addWidget(excellon_editor_group_drill_circular_dir_radio, 8, 1, 1, 1);
    excellon_editor_group_drill_circular_angle_label = new QLabel(nullptr);
    excellon_editor_group_drill_circular_angle_label->setText(QString::fromUtf8("Ángulo circular:"));
    excellon_editor_group_drill_circular_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    lay54->addWidget(excellon_editor_group_drill_circular_angle_label, 9, 0, 1, 1);
    excellon_editor_group_drill_circular_angle_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_drill_circular_angle_entry->setProperty("decimals", 4);
    excellon_editor_group_drill_circular_angle_entry->setProperty("maximum", 360.0);
    excellon_editor_group_drill_circular_angle_entry->setProperty("minimum", -360.0);
    excellon_editor_group_drill_circular_angle_entry->setProperty("singleStep", 5.0);
    excellon_editor_group_drill_circular_angle_entry->setProperty("value", 12.0);
    excellon_editor_group_drill_circular_angle_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_drill_circular_angle_entry, 9, 1, 1, 1);
    excellon_editor_group_drill_array_circ_label = new QLabel(nullptr);
    excellon_editor_group_drill_array_circ_label->setText(QString::fromUtf8("<b>Muesca:</b>"));
    lay54->addWidget(excellon_editor_group_drill_array_circ_label, 10, 0, 1, 2);
    excellon_editor_group_slot_length_label = new QLabel(nullptr);
    excellon_editor_group_slot_length_label->setMinimumSize(QSize(100, 0));
    excellon_editor_group_slot_length_label->setText(QString::fromUtf8("Longitud:"));
    excellon_editor_group_slot_length_label->setToolTip(QString::fromUtf8("Longitud. La longitud de la ranura."));
    lay54->addWidget(excellon_editor_group_slot_length_label, 11, 0, 1, 1);
    excellon_editor_group_slot_length_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_length_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_length_entry->setProperty("maximum", 99999.0);
    excellon_editor_group_slot_length_entry->setProperty("value", 5.0);
    excellon_editor_group_slot_length_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_length_entry, 11, 1, 1, 1);
    excellon_editor_group_slot_axis_label = new QLabel(nullptr);
    excellon_editor_group_slot_axis_label->setMinimumSize(QSize(100, 0));
    excellon_editor_group_slot_axis_label->setText(QString::fromUtf8("Dirección:"));
    excellon_editor_group_slot_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la ranura:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la ranura"));
    lay54->addWidget(excellon_editor_group_slot_axis_label, 12, 0, 1, 1);
    excellon_editor_group_slot_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_slot_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay54->addWidget(excellon_editor_group_slot_axis_radio, 12, 1, 1, 1);
    excellon_editor_group_slot_angle_label = new QLabel(nullptr);
    excellon_editor_group_slot_angle_label->setMinimumSize(QSize(100, 0));
    excellon_editor_group_slot_angle_label->setText(QString::fromUtf8("Ángulo:"));
    excellon_editor_group_slot_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca la ranura.\nLa precisión es de un máximo de 2 decimales.\nEl valor mínimo es: -360.00 grados.\nEl valor máximo es: 360.00 grados."));
    lay54->addWidget(excellon_editor_group_slot_angle_label, 13, 0, 1, 1);
    excellon_editor_group_slot_angle_spinner = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_angle_spinner->setProperty("decimals", 4);
    excellon_editor_group_slot_angle_spinner->setProperty("maximum", 360.0);
    excellon_editor_group_slot_angle_spinner->setProperty("minimum", -359.99);
    excellon_editor_group_slot_angle_spinner->setProperty("singleStep", 5.0);
    excellon_editor_group_slot_angle_spinner->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_angle_spinner, 13, 1, 1, 1);
    excellon_editor_group_slot_array_linear_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_linear_label->setText(QString::fromUtf8("<b>Matriz Lin de Ranuras:</b>"));
    lay54->addWidget(excellon_editor_group_slot_array_linear_label, 14, 0, 1, 2);
    excellon_editor_group_slot_array_size_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_size_label->setText(QString::fromUtf8("Nro. De ranuras:"));
    lay54->addWidget(excellon_editor_group_slot_array_size_label, 15, 0, 1, 1);
    excellon_editor_group_slot_array_size_entry = new FCSpinner(nullptr);
    excellon_editor_group_slot_array_size_entry->setProperty("maximum", 999999);
    excellon_editor_group_slot_array_size_entry->setProperty("value", 5);
    lay54->addWidget(excellon_editor_group_slot_array_size_entry, 15, 1, 1, 1);
    excellon_editor_group_slot_array_axis_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_axis_label->setText(QString::fromUtf8("Direccion lineal:"));
    excellon_editor_group_slot_array_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la matriz lineal:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la matriz"));
    lay54->addWidget(excellon_editor_group_slot_array_axis_label, 16, 0, 1, 1);
    excellon_editor_group_slot_array_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_slot_array_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay54->addWidget(excellon_editor_group_slot_array_axis_radio, 16, 1, 1, 1);
    excellon_editor_group_slot_array_pitch_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_pitch_label->setText(QString::fromUtf8("Paso:"));
    excellon_editor_group_slot_array_pitch_label->setToolTip(QString::fromUtf8("Paso = Distancia entre elementos de la matriz."));
    lay54->addWidget(excellon_editor_group_slot_array_pitch_label, 17, 0, 1, 1);
    excellon_editor_group_slot_array_pitch_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_array_pitch_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_array_pitch_entry->setProperty("maximum", 999999.0);
    excellon_editor_group_slot_array_pitch_entry->setProperty("value", 2.54);
    excellon_editor_group_slot_array_pitch_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_array_pitch_entry, 17, 1, 1, 1);
    excellon_editor_group_slot_array_angle_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_angle_label->setText(QString::fromUtf8("Ángulo:"));
    excellon_editor_group_slot_array_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    lay54->addWidget(excellon_editor_group_slot_array_angle_label, 18, 0, 1, 1);
    excellon_editor_group_slot_array_angle_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_array_angle_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_array_angle_entry->setProperty("maximum", 360.0);
    excellon_editor_group_slot_array_angle_entry->setProperty("minimum", -360.0);
    excellon_editor_group_slot_array_angle_entry->setProperty("singleStep", 5.0);
    excellon_editor_group_slot_array_angle_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_array_angle_entry, 18, 1, 1, 1);
    excellon_editor_group_slot_array_circ_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_circ_label->setText(QString::fromUtf8("<b>Matriz Circ de Ranura:</b>"));
    lay54->addWidget(excellon_editor_group_slot_array_circ_label, 19, 0, 1, 2);
    excellon_editor_group_slot_array_circular_direction_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_circular_direction_label->setText(QString::fromUtf8("Dirección circular:"));
    excellon_editor_group_slot_array_circular_direction_label->setToolTip(QString::fromUtf8("Dirección para matriz circular.\nPuede ser CW = en sentido horario o CCW = en sentido antihorario."));
    lay54->addWidget(excellon_editor_group_slot_array_circular_direction_label, 20, 0, 1, 1);
    excellon_editor_group_slot_array_circular_dir_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_slot_array_circular_dir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay54->addWidget(excellon_editor_group_slot_array_circular_dir_radio, 20, 1, 1, 1);
    excellon_editor_group_slot_array_circular_angle_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_circular_angle_label->setText(QString::fromUtf8("Ángulo circular:"));
    excellon_editor_group_slot_array_circular_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    lay54->addWidget(excellon_editor_group_slot_array_circular_angle_label, 21, 0, 1, 1);
    excellon_editor_group_slot_array_circular_angle_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_array_circular_angle_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_array_circular_angle_entry->setProperty("maximum", 360.0);
    excellon_editor_group_slot_array_circular_angle_entry->setProperty("minimum", -360.0);
    excellon_editor_group_slot_array_circular_angle_entry->setProperty("singleStep", 5.0);
    excellon_editor_group_slot_array_circular_angle_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_array_circular_angle_entry, 21, 1, 1, 1);
    excellon_editor_group_layout->addLayout(lay54, 0);
    excellon_editor_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    excellon_editor_group->setLayout(excellon_editor_group_layout);
    layout->addWidget(excellon_editor_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    excellon_defaults_form->setLayout(layout);
    excellon_scroll_area->setWidget(excellon_defaults_form);
    excellon_tab_lay->addWidget(excellon_scroll_area, 0);
    excellon_tab->setLayout(excellon_tab_lay);
    pref_tab_area->addTab(excellon_tab, QString::fromUtf8("EXCELLON"));
    geometry_tab = new QWidget(nullptr);
    geometry_tab->setProperty("objectName", QString::fromUtf8("geometry_tab"));
    geometry_tab->setHidden(true);
    geometry_tab_lay = new QVBoxLayout();
    geometry_tab_lay->setContentsMargins(2, 2, 2, 2);
    geometry_tab_lay->setSpacing(6);
    geometry_scroll_area = new QScrollArea(nullptr);
    geometry_defaults_form = new QWidget(nullptr);
    geometry_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    geometry_gen_group = new QGroupBox(nullptr);
    { QFont f = geometry_gen_group->font(); f.setBold(true); f.setPixelSize(16); geometry_gen_group->setFont(f); }
    geometry_gen_group->setMinimumSize(QSize(220, 0));
    geometry_gen_group_layout = new QVBoxLayout();
    geometry_gen_group_layout->setContentsMargins(9, 9, 9, 9);
    geometry_gen_group_layout->setSpacing(6);
    geometry_gen_group_plot_options_label = new QLabel(nullptr);
    geometry_gen_group_plot_options_label->setText(QString::fromUtf8("<b>Opciones de parcela:</b>"));
    geometry_gen_group_layout->addWidget(geometry_gen_group_plot_options_label, 0);
    auto* lay56 = new QHBoxLayout();
    lay56->setContentsMargins(0, 0, 0, 0);
    lay56->setSpacing(6);
    geometry_gen_group_plot_cb = new FCCheckBox(nullptr);
    geometry_gen_group_plot_cb->setChecked(true);
    geometry_gen_group_plot_cb->setText(QString::fromUtf8("Gráfico"));
    geometry_gen_group_plot_cb->setToolTip(QString::fromUtf8("Trazar (mostrar) este objeto."));
    lay56->addWidget(geometry_gen_group_plot_cb, 0);
    geometry_gen_group_multicolored_cb = new FCCheckBox(nullptr);
    geometry_gen_group_multicolored_cb->setText(QString::fromUtf8("M-Color"));
    geometry_gen_group_multicolored_cb->setToolTip(QString::fromUtf8("Dibuja polígonos en diferentes colores."));
    lay56->addWidget(geometry_gen_group_multicolored_cb, 0);
    geometry_gen_group_layout->addLayout(lay56, 0);
    auto* lay57 = new QGridLayout();
    lay57->setContentsMargins(0, 0, 0, 0);
    lay57->setHorizontalSpacing(6);
    lay57->setVerticalSpacing(6);
    geometry_gen_group_circle_steps_label = new QLabel(nullptr);
    geometry_gen_group_circle_steps_label->setText(QString::fromUtf8("Pasos del círculo:"));
    geometry_gen_group_circle_steps_label->setToolTip(QString::fromUtf8("El número de pasos de círculo para <b>Geometría</b>\nCírculo y arcos de aproximación lineal."));
    lay57->addWidget(geometry_gen_group_circle_steps_label, 1, 0, 1, 1);
    geometry_gen_group_circle_steps_entry = new FCSpinner(nullptr);
    geometry_gen_group_circle_steps_entry->setProperty("maximum", 999);
    geometry_gen_group_circle_steps_entry->setProperty("value", 64);
    lay57->addWidget(geometry_gen_group_circle_steps_entry, 1, 1, 1, 1);
    geometry_gen_group_tools_label = new QLabel(nullptr);
    geometry_gen_group_tools_label->setText(QString::fromUtf8("<b>Herramientas:</b>"));
    lay57->addWidget(geometry_gen_group_tools_label, 2, 0, 1, 2);
    auto* w58 = new QLabel(nullptr);
    w58->setText(QString::fromUtf8("<b><font color=\"green\">Diá. de Herram:</font></b>"));
    w58->setToolTip(QString::fromUtf8("Diámetros de las herramientas, separados por comas.\nEl valor del diámetro tiene que usar el separador de decimales de punto.\nValores válidos: 0.3, 1.0"));
    lay57->addWidget(w58, 3, 0, 1, 1);
    geometry_gen_group_cnctooldia_entry = new FCEntry(nullptr);
    geometry_gen_group_cnctooldia_entry->setText(QString::fromUtf8("2.4"));
    lay57->addWidget(geometry_gen_group_cnctooldia_entry, 3, 1, 1, 1);
    auto* w59 = new QFrame(nullptr);
    w59->setProperty("frameShadow", 48);
    w59->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w59->setSizePolicy(sp); }
    lay57->addWidget(w59, 9, 0, 1, 2);
    geometry_gen_group_opt_label = new QLabel(nullptr);
    geometry_gen_group_opt_label->setText(QString::fromUtf8("<b>Optimización de ruta:</b>"));
    lay57->addWidget(geometry_gen_group_opt_label, 10, 0, 1, 2);
    geometry_gen_group_opt_algorithm_label = new QLabel(nullptr);
    geometry_gen_group_opt_algorithm_label->setText(QString::fromUtf8("Algoritmo:"));
    geometry_gen_group_opt_algorithm_label->setToolTip(QString::fromUtf8("Esto establece el tipo de optimización para la ruta de perforación Excellon.\nSi <<MetaHeuristic>> está marcado, el algoritmo de Google OR-Tools con\nSe utiliza la ruta local guiada metaheurística. El tiempo de búsqueda predeterminado es de 3 segundos.\nSi <<Básico>> está marcado, se utiliza el algoritmo básico de Google OR-Tools.\nSi se marca <<TSA>>, se utiliza el algoritmo de vendedor ambulante para\nOptimización de la ruta de perforación.\n\nAlgunas opciones están deshabilitadas cuando la aplicación funciona en modo de 32 bits."));
    lay57->addWidget(geometry_gen_group_opt_algorithm_label, 12, 0, 1, 1);
    geometry_gen_group_opt_algorithm_radio = new RadioSet({{QString::fromUtf8("Rtree"), QVariant(QString::fromUtf8("R"))}, {QString::fromUtf8("MetaHeuristic"), QVariant(QString::fromUtf8("M"))}, {QString::fromUtf8("BASIC"), QVariant(QString::fromUtf8("B"))}, {QString::fromUtf8("TSA"), QVariant(QString::fromUtf8("T"))}}, Qt::Vertical, false, nullptr);
    geometry_gen_group_opt_algorithm_radio->setValue(QVariant(QString::fromUtf8("R")));
    lay57->addWidget(geometry_gen_group_opt_algorithm_radio, 12, 1, 1, 1);
    geometry_gen_group_optimization_time_label = new QLabel(nullptr);
    geometry_gen_group_optimization_time_label->setEnabled(false);
    geometry_gen_group_optimization_time_label->setText(QString::fromUtf8("Duración:"));
    geometry_gen_group_optimization_time_label->setToolTip(QString::fromUtf8("Cuando OR-Tools Metaheuristic (MH) está habilitado, hay un\numbral máximo de cuánto tiempo se dedica a hacer el\nOptimización del camino. Esta duración máxima se establece aquí.\nEn segundos."));
    lay57->addWidget(geometry_gen_group_optimization_time_label, 14, 0, 1, 1);
    geometry_gen_group_optimization_time_entry = new FCSpinner(nullptr);
    geometry_gen_group_optimization_time_entry->setEnabled(false);
    geometry_gen_group_optimization_time_entry->setProperty("maximum", 999);
    geometry_gen_group_optimization_time_entry->setProperty("value", 3);
    lay57->addWidget(geometry_gen_group_optimization_time_entry, 14, 1, 1, 1);
    auto* w60 = new QFrame(nullptr);
    w60->setProperty("frameShadow", 48);
    w60->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w60->setSizePolicy(sp); }
    lay57->addWidget(w60, 16, 0, 1, 2);
    geometry_gen_group_join_geo_label = new QLabel(nullptr);
    geometry_gen_group_join_geo_label->setText(QString::fromUtf8("<b>Opción de unirse</b>:"));
    lay57->addWidget(geometry_gen_group_join_geo_label, 18, 0, 1, 2);
    geometry_gen_group_fuse_tools_cb = new FCCheckBox(nullptr);
    geometry_gen_group_fuse_tools_cb->setChecked(true);
    geometry_gen_group_fuse_tools_cb->setText(QString::fromUtf8("Fusionar las herramientas"));
    geometry_gen_group_fuse_tools_cb->setToolTip(QString::fromUtf8("Cuando se marca, las herramientas se fusionarán\npero solo si comparten algunos de sus atributos."));
    lay57->addWidget(geometry_gen_group_fuse_tools_cb, 20, 0, 1, 2);
    auto* w61 = new QFrame(nullptr);
    w61->setProperty("frameShadow", 48);
    w61->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w61->setSizePolicy(sp); }
    lay57->addWidget(w61, 22, 0, 1, 2);
    geometry_gen_group_gerber_color_label = new QLabel(nullptr);
    geometry_gen_group_gerber_color_label->setText(QString::fromUtf8("<b>Color del objeto</b>:"));
    lay57->addWidget(geometry_gen_group_gerber_color_label, 24, 0, 1, 2);
    geometry_gen_group_line_color_label = new QLabel(nullptr);
    geometry_gen_group_line_color_label->setText(QString::fromUtf8("Contorno:"));
    geometry_gen_group_line_color_label->setToolTip(QString::fromUtf8("Establecer el color de la línea para los objetos trazados."));
    lay57->addWidget(geometry_gen_group_line_color_label, 26, 0, 1, 1);
    geometry_gen_group_line_color_entry = new FCColorEntry(nullptr);
    geometry_gen_group_line_color_entry->setValue(QString::fromUtf8("#FFAAAA"));
    lay57->addWidget(geometry_gen_group_line_color_entry, 26, 1, 1, 1);
    lay57->setColumnStretch(1, 1);
    geometry_gen_group_layout->addLayout(lay57, 0);
    geometry_gen_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    geometry_gen_group->setLayout(geometry_gen_group_layout);
    layout->addWidget(geometry_gen_group, 0);
    geometry_opt_group = new QGroupBox(nullptr);
    { QFont f = geometry_opt_group->font(); f.setBold(true); f.setPixelSize(16); geometry_opt_group->setFont(f); }
    geometry_opt_group->setMinimumSize(QSize(300, 0));
    geometry_opt_group_layout = new QVBoxLayout();
    geometry_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    geometry_opt_group_layout->setSpacing(6);
    geometry_opt_group_cncjob_label = new QLabel(nullptr);
    geometry_opt_group_cncjob_label->setText(QString::fromUtf8("<b>Crear CNCJob:</b>"));
    geometry_opt_group_cncjob_label->setToolTip(QString::fromUtf8("Crear un objeto de trabajo CNC\ntrazando los contornos de este\nObjeto de geometría."));
    geometry_opt_group_layout->addWidget(geometry_opt_group_cncjob_label, 0);
    auto* lay62 = new QGridLayout();
    lay62->setContentsMargins(0, 0, 0, 0);
    lay62->setHorizontalSpacing(6);
    lay62->setVerticalSpacing(6);
    auto* w63 = new QLabel(nullptr);
    w63->setText(QString::fromUtf8("Corte Z:"));
    w63->setToolTip(QString::fromUtf8("Profundidad de corte (negativo)\ndebajo de la superficie de cobre."));
    lay62->addWidget(w63, 0, 0, 1, 1);
    geometry_opt_group_cutz_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_cutz_entry->setProperty("decimals", 4);
    geometry_opt_group_cutz_entry->setProperty("maximum", 0.0);
    geometry_opt_group_cutz_entry->setProperty("minimum", -10000.0);
    geometry_opt_group_cutz_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_cutz_entry->setProperty("value", -2.4);
    geometry_opt_group_cutz_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_cutz_entry, 0, 1, 1, 1);
    geometry_opt_group_multidepth_cb = new FCCheckBox(nullptr);
    geometry_opt_group_multidepth_cb->setText(QString::fromUtf8("Profund. Múlti"));
    geometry_opt_group_multidepth_cb->setToolTip(QString::fromUtf8("Usa múltiples pases para limitar\nLa profundidad de corte en cada pasada. Será\ncortar varias veces hasta que el Corte Z sea\nalcanzado."));
    lay62->addWidget(geometry_opt_group_multidepth_cb, 1, 0, 1, 1);
    auto* w64 = new QLabel(nullptr);
    w64->setText(QString::fromUtf8("Profund. / Pase:"));
    w64->setToolTip(QString::fromUtf8("La profundidad a cortar en cada pasada,\ncuando está habilitado multidepto.\nTiene valor positivo aunque\nEs una fracción de la profundidad.\nque tiene valor negativo."));
    lay62->addWidget(w64, 2, 0, 1, 1);
    geometry_opt_group_depthperpass_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_depthperpass_entry->setProperty("decimals", 4);
    geometry_opt_group_depthperpass_entry->setEnabled(false);
    geometry_opt_group_depthperpass_entry->setProperty("maximum", 99999.0);
    geometry_opt_group_depthperpass_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_depthperpass_entry->setProperty("value", 0.8);
    geometry_opt_group_depthperpass_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_depthperpass_entry, 2, 1, 1, 1);
    auto* w65 = new QLabel(nullptr);
    w65->setText(QString::fromUtf8("Viaje Z:"));
    w65->setToolTip(QString::fromUtf8("Altura de la herramienta cuando\nMoviéndose sin cortar."));
    lay62->addWidget(w65, 3, 0, 1, 1);
    geometry_opt_group_travelz_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_travelz_entry->setProperty("decimals", 4);
    geometry_opt_group_travelz_entry->setProperty("maximum", 10000.0);
    geometry_opt_group_travelz_entry->setProperty("minimum", 0.0001);
    geometry_opt_group_travelz_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_travelz_entry->setProperty("value", 2.0);
    geometry_opt_group_travelz_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_travelz_entry, 3, 1, 1, 1);
    geometry_opt_group_toolchange_cb = new FCCheckBox(nullptr);
    geometry_opt_group_toolchange_cb->setText(QString::fromUtf8("Cambio de herram"));
    geometry_opt_group_toolchange_cb->setToolTip(QString::fromUtf8("Incluir secuencia de cambio de herramienta\nen el código de máquina (pausa para cambio de herramienta)."));
    lay62->addWidget(geometry_opt_group_toolchange_cb, 4, 0, 1, 2);
    auto* w66 = new QLabel(nullptr);
    w66->setText(QString::fromUtf8("Cambio de herramienta Z:"));
    w66->setToolTip(QString::fromUtf8("Posición del eje Z (altura) para\ncambio de herramienta."));
    lay62->addWidget(w66, 5, 0, 1, 1);
    geometry_opt_group_toolchangez_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_toolchangez_entry->setProperty("decimals", 4);
    geometry_opt_group_toolchangez_entry->setProperty("maximum", 10000.0);
    geometry_opt_group_toolchangez_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_toolchangez_entry->setProperty("value", 15.0);
    geometry_opt_group_toolchangez_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_toolchangez_entry, 5, 1, 1, 1);
    auto* w67 = new QLabel(nullptr);
    w67->setText(QString::fromUtf8("Fin del movi. Z:"));
    w67->setToolTip(QString::fromUtf8("Altura de la herramienta después de\nEl último movimiento al final del trabajo."));
    lay62->addWidget(w67, 6, 0, 1, 1);
    geometry_opt_group_endz_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_endz_entry->setProperty("decimals", 4);
    geometry_opt_group_endz_entry->setProperty("maximum", 10000.0);
    geometry_opt_group_endz_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_endz_entry->setProperty("value", 15.0);
    geometry_opt_group_endz_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_endz_entry, 6, 1, 1, 1);
    auto* w68 = new QLabel(nullptr);
    w68->setText(QString::fromUtf8("X, Y Fin del movimiento:"));
    w68->setToolTip(QString::fromUtf8("Fin movimiento X, Y posición. En formato (x, y).\nSi no se ingresa ningún valor, entonces no hay movimiento\nen el plano X, Y al final del trabajo."));
    lay62->addWidget(w68, 7, 0, 1, 1);
    geometry_opt_group_endxy_entry = new NumericalEvalTupleEntry(nullptr);
    geometry_opt_group_endxy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    geometry_opt_group_endxy_entry->setText(QString::fromUtf8("None"));
    lay62->addWidget(geometry_opt_group_endxy_entry, 7, 1, 1, 1);
    auto* w69 = new QLabel(nullptr);
    w69->setText(QString::fromUtf8("Avance X-Y:"));
    w69->setToolTip(QString::fromUtf8("Velocidad de corte en el XY.\nAvion en unidades por minuto"));
    lay62->addWidget(w69, 8, 0, 1, 1);
    geometry_opt_group_cncfeedrate_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_cncfeedrate_entry->setProperty("decimals", 4);
    geometry_opt_group_cncfeedrate_entry->setProperty("maximum", 910000.0);
    geometry_opt_group_cncfeedrate_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_cncfeedrate_entry->setProperty("value", 120.0);
    geometry_opt_group_cncfeedrate_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_cncfeedrate_entry, 8, 1, 1, 1);
    auto* w70 = new QLabel(nullptr);
    w70->setText(QString::fromUtf8("Avance Z:"));
    w70->setToolTip(QString::fromUtf8("Velocidad de corte en el XY.\nPlano en unidades por minuto.\nSe llama también Plunge."));
    lay62->addWidget(w70, 9, 0, 1, 1);
    geometry_opt_group_feedrate_z_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_feedrate_z_entry->setProperty("decimals", 4);
    geometry_opt_group_feedrate_z_entry->setProperty("maximum", 910000.0);
    geometry_opt_group_feedrate_z_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_feedrate_z_entry->setProperty("value", 60.0);
    geometry_opt_group_feedrate_z_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_feedrate_z_entry, 9, 1, 1, 1);
    auto* w71 = new QLabel(nullptr);
    w71->setText(QString::fromUtf8("Eje de velocidad:"));
    w71->setToolTip(QString::fromUtf8("Velocidad del husillo en RPM (opcional).\nSi se utiliza el postprocesador LÁSER,\nEste valor es el poder del láser."));
    lay62->addWidget(w71, 10, 0, 1, 1);
    geometry_opt_group_cncspindlespeed_entry = new FCSpinner(nullptr);
    geometry_opt_group_cncspindlespeed_entry->setProperty("maximum", 1000000);
    geometry_opt_group_cncspindlespeed_entry->setProperty("singleStep", 100);
    lay62->addWidget(geometry_opt_group_cncspindlespeed_entry, 10, 1, 1, 1);
    geometry_opt_group_dwell_cb = new FCCheckBox(nullptr);
    geometry_opt_group_dwell_cb->setText(QString::fromUtf8("Habilitar Permanencia"));
    geometry_opt_group_dwell_cb->setToolTip(QString::fromUtf8("Pausa para permitir que el husillo alcance su\nVelocidad antes del corte."));
    lay62->addWidget(geometry_opt_group_dwell_cb, 11, 0, 1, 1);
    auto* w72 = new QLabel(nullptr);
    w72->setText(QString::fromUtf8("Duración:"));
    w72->setToolTip(QString::fromUtf8("Número de unidades de tiempo para que el husillo permanezca."));
    lay62->addWidget(w72, 12, 0, 1, 1);
    geometry_opt_group_dwelltime_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_dwelltime_entry->setProperty("decimals", 4);
    geometry_opt_group_dwelltime_entry->setEnabled(false);
    geometry_opt_group_dwelltime_entry->setProperty("maximum", 99999.0);
    geometry_opt_group_dwelltime_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_dwelltime_entry->setProperty("value", 1.0);
    geometry_opt_group_dwelltime_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_dwelltime_entry, 12, 1, 1, 1);
    auto* w73 = new QLabel(nullptr);
    w73->setText(QString::fromUtf8("Postprocesador:"));
    w73->setToolTip(QString::fromUtf8("El archivo de postprocesador que dicta\nla salida del código de máquina (como GCode, RML, HPGL)."));
    lay62->addWidget(w73, 13, 0, 1, 1);
    geometry_opt_group_pp_geometry_name_cb = new FCComboBox(nullptr);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); geometry_opt_group_pp_geometry_name_cb->setSizePolicy(sp); }
    lay62->addWidget(geometry_opt_group_pp_geometry_name_cb, 13, 1, 1, 1);
    lay62->setColumnStretch(1, 1);
    geometry_opt_group_layout->addLayout(lay62, 0);
    geometry_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    geometry_opt_group->setLayout(geometry_opt_group_layout);
    layout->addWidget(geometry_opt_group, 0);
    geometry_adv_opt_group = new QGroupBox(nullptr);
    { QFont f = geometry_adv_opt_group->font(); f.setBold(true); f.setPixelSize(16); geometry_adv_opt_group->setFont(f); }
    geometry_adv_opt_group->setMinimumSize(QSize(270, 0));
    geometry_adv_opt_group_layout = new QVBoxLayout();
    geometry_adv_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    geometry_adv_opt_group_layout->setSpacing(6);
    geometry_adv_opt_group_geo_label = new FCLabel(nullptr);
    geometry_adv_opt_group_geo_label->setText(QString::fromUtf8("<b>Opciones avanzadas:</b>"));
    geometry_adv_opt_group_geo_label->setToolTip(QString::fromUtf8("Una lista de los parámetros avanzados.\nEsos parámetros están disponibles sólo para\nAplicación avanzada Nivel."));
    geometry_adv_opt_group_layout->addWidget(geometry_adv_opt_group_geo_label, 0);
    auto* lay74 = new QGridLayout();
    lay74->setContentsMargins(0, 0, 0, 0);
    lay74->setHorizontalSpacing(6);
    lay74->setVerticalSpacing(6);
    auto* w75 = new FCLabel(nullptr);
    w75->setText(QString::fromUtf8("Cambio de herra X, Y:"));
    w75->setToolTip(QString::fromUtf8("Cambio de herra X, posición Y."));
    lay74->addWidget(w75, 1, 0, 1, 1);
    geometry_adv_opt_group_toolchangexy_entry = new NumericalEvalTupleEntry(nullptr);
    geometry_adv_opt_group_toolchangexy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    geometry_adv_opt_group_toolchangexy_entry->setText(QString::fromUtf8("0.0, 0.0"));
    lay74->addWidget(geometry_adv_opt_group_toolchangexy_entry, 1, 1, 1, 1);
    auto* w76 = new FCLabel(nullptr);
    w76->setText(QString::fromUtf8("Comience Z:"));
    w76->setToolTip(QString::fromUtf8("Altura de la herramienta justo después de comenzar el trabajo.\nElimine el valor si no necesita esta característica."));
    lay74->addWidget(w76, 2, 0, 1, 1);
    geometry_adv_opt_group_gstartz_entry = new NumericalEvalEntry(nullptr);
    geometry_adv_opt_group_gstartz_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay74->addWidget(geometry_adv_opt_group_gstartz_entry, 2, 1, 1, 1);
    auto* w77 = new FCLabel(nullptr);
    w77->setText(QString::fromUtf8("Rápidos de avance:"));
    w77->setToolTip(QString::fromUtf8("Velocidad de corte en el plano XY.\n(en unidades por minuto).\nEsto es para el movimiento rápido G00.\nEs útil solo para Marlin,\nIgnorar para cualquier otro caso."));
    lay74->addWidget(w77, 4, 0, 1, 1);
    geometry_adv_opt_group_feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("maximum", 910000.0);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("value", 1500.0);
    geometry_adv_opt_group_feedrate_rapid_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_feedrate_rapid_entry, 4, 1, 1, 1);
    geometry_adv_opt_group_extracut_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_extracut_cb->setText(QString::fromUtf8("Recortar"));
    geometry_adv_opt_group_extracut_cb->setToolTip(QString::fromUtf8("Para eliminar posibles\nsobras de cobre donde el primer corte\nNos reunimos con el último corte, generamos un\nCorte extendido sobre la primera sección de corte."));
    lay74->addWidget(geometry_adv_opt_group_extracut_cb, 5, 0, 1, 1);
    geometry_adv_opt_group_e_cut_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_e_cut_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_e_cut_entry->setProperty("maximum", 99999.0);
    geometry_adv_opt_group_e_cut_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_e_cut_entry->setToolTip(QString::fromUtf8("Para eliminar posibles\nsobras de cobre donde el primer corte\nNos reunimos con el último corte, generamos un\nCorte extendido sobre la primera sección de corte."));
    geometry_adv_opt_group_e_cut_entry->setProperty("value", 0.1);
    geometry_adv_opt_group_e_cut_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_e_cut_entry, 5, 1, 1, 1);
    geometry_adv_opt_group_pdepth_label = new FCLabel(nullptr);
    geometry_adv_opt_group_pdepth_label->setText(QString::fromUtf8("Profundidad de la sonda Z:"));
    geometry_adv_opt_group_pdepth_label->setToolTip(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units."));
    lay74->addWidget(geometry_adv_opt_group_pdepth_label, 6, 0, 1, 1);
    geometry_adv_opt_group_pdepth_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_pdepth_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_pdepth_entry->setProperty("maximum", 0.0);
    geometry_adv_opt_group_pdepth_entry->setProperty("minimum", -99999.0);
    geometry_adv_opt_group_pdepth_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_pdepth_entry->setProperty("value", -0.02);
    geometry_adv_opt_group_pdepth_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_pdepth_entry, 6, 1, 1, 1);
    geometry_adv_opt_group_feedrate_probe_label = new FCLabel(nullptr);
    geometry_adv_opt_group_feedrate_probe_label->setText(QString::fromUtf8("Sonda de avance:"));
    geometry_adv_opt_group_feedrate_probe_label->setToolTip(QString::fromUtf8("La velocidad de avance utilizada mientras la sonda está sondeando."));
    lay74->addWidget(geometry_adv_opt_group_feedrate_probe_label, 7, 0, 1, 1);
    geometry_adv_opt_group_feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("maximum", 910000.0);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("value", 75.0);
    geometry_adv_opt_group_feedrate_probe_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_feedrate_probe_entry, 7, 1, 1, 1);
    auto* w78 = new FCLabel(nullptr);
    w78->setText(QString::fromUtf8("Dirección del motor:"));
    w78->setToolTip(QString::fromUtf8("Esto establece la dirección en que gira el husillo.\nPuede ser:\n- CW = en el sentido de las agujas del reloj o\n- CCW = a la izquierda"));
    lay74->addWidget(w78, 8, 0, 1, 1);
    geometry_adv_opt_group_spindledir_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    geometry_adv_opt_group_spindledir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay74->addWidget(geometry_adv_opt_group_spindledir_radio, 8, 1, 1, 1);
    geometry_adv_opt_group_fplunge_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_fplunge_cb->setText(QString::fromUtf8("Salto rápido"));
    geometry_adv_opt_group_fplunge_cb->setToolTip(QString::fromUtf8("Al comprobar esto, el movimiento vertical de\nZ_Toolchange a Z_move se hace con G0,\nes decir, la velocidad más rápida disponible.\nADVERTENCIA: el movimiento se realiza en Toolchange X, Y coords."));
    lay74->addWidget(geometry_adv_opt_group_fplunge_cb, 9, 0, 1, 2);
    auto* w79 = new FCLabel(nullptr);
    w79->setText(QString::fromUtf8("Tamaño del Seg. X:"));
    w79->setToolTip(QString::fromUtf8("El tamaño del segmento traza en el eje X.\nÚtil para la autonivelación.\nUn valor de 0 significa que no hay segmentación en el eje X."));
    lay74->addWidget(w79, 10, 0, 1, 1);
    geometry_adv_opt_group_segx_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_segx_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_segx_entry->setProperty("maximum", 99999.0);
    geometry_adv_opt_group_segx_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_segx_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_segx_entry, 10, 1, 1, 1);
    auto* w80 = new FCLabel(nullptr);
    w80->setText(QString::fromUtf8("Tamaño del Seg. Y:"));
    w80->setToolTip(QString::fromUtf8("El tamaño del segmento traza en el eje Y.\nÚtil para la autonivelación.\nUn valor de 0 significa que no hay segmentación en el eje Y."));
    lay74->addWidget(w80, 11, 0, 1, 1);
    geometry_adv_opt_group_segy_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_segy_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_segy_entry->setProperty("maximum", 99999.0);
    geometry_adv_opt_group_segy_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_segy_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_segy_entry, 11, 1, 1, 1);
    auto* w81 = new QFrame(nullptr);
    w81->setProperty("frameShadow", 48);
    w81->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w81->setSizePolicy(sp); }
    lay74->addWidget(w81, 12, 0, 1, 2);
    geometry_adv_opt_group_area_exc_label = new FCLabel(nullptr);
    geometry_adv_opt_group_area_exc_label->setText(QString::fromUtf8("<b>Exclusión de áreaSelección de área:</b>"));
    geometry_adv_opt_group_area_exc_label->setToolTip(QString::fromUtf8("Parámetros de exclusión de áreas."));
    lay74->addWidget(geometry_adv_opt_group_area_exc_label, 13, 0, 1, 2);
    geometry_adv_opt_group_exclusion_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_exclusion_cb->setText(QString::fromUtf8("Zonas de exclusión"));
    geometry_adv_opt_group_exclusion_cb->setToolTip(QString::fromUtf8("Incluir áreas de exclusión.\nEn esas áreas el recorrido de las herramientas.\nestá prohibido."));
    lay74->addWidget(geometry_adv_opt_group_exclusion_cb, 14, 0, 1, 2);
    geometry_adv_opt_group_area_shape_label = new FCLabel(nullptr);
    geometry_adv_opt_group_area_shape_label->setText(QString::fromUtf8("Forma:"));
    geometry_adv_opt_group_area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    lay74->addWidget(geometry_adv_opt_group_area_shape_label, 15, 0, 1, 1);
    geometry_adv_opt_group_area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    geometry_adv_opt_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("polygon")));
    lay74->addWidget(geometry_adv_opt_group_area_shape_radio, 15, 1, 1, 1);
    geometry_adv_opt_group_strategy_label = new FCLabel(nullptr);
    geometry_adv_opt_group_strategy_label->setText(QString::fromUtf8("Estrategia:"));
    geometry_adv_opt_group_strategy_label->setToolTip(QString::fromUtf8("La estrategia seguida al encontrar un área de exclusión.\nPuede ser:\n- Sobre -> al encontrar el área, la herramienta irá a una altura establecida\n- Alrededor -> evitará el área de exclusión recorriendo el área"));
    lay74->addWidget(geometry_adv_opt_group_strategy_label, 16, 0, 1, 1);
    geometry_adv_opt_group_strategy_radio = new RadioSet({{QString::fromUtf8("Sobre"), QVariant(QString::fromUtf8("over"))}, {QString::fromUtf8("AlrededorRedondo"), QVariant(QString::fromUtf8("around"))}}, Qt::Horizontal, true, nullptr);
    geometry_adv_opt_group_strategy_radio->setValue(QVariant(QString::fromUtf8("over")));
    lay74->addWidget(geometry_adv_opt_group_strategy_radio, 16, 1, 1, 1);
    geometry_adv_opt_group_over_z_label = new FCLabel(nullptr);
    geometry_adv_opt_group_over_z_label->setText(QString::fromUtf8("Sobre ZSuperposición:"));
    geometry_adv_opt_group_over_z_label->setToolTip(QString::fromUtf8("La altura Z a la que se elevará la herramienta para evitar\nUn área de interdicción."));
    lay74->addWidget(geometry_adv_opt_group_over_z_label, 18, 0, 1, 1);
    geometry_adv_opt_group_over_z_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_over_z_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_over_z_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_over_z_entry->setProperty("value", 1.0);
    lay74->addWidget(geometry_adv_opt_group_over_z_entry, 18, 1, 1, 1);
    auto* w82 = new QFrame(nullptr);
    w82->setProperty("frameShadow", 48);
    w82->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w82->setSizePolicy(sp); }
    lay74->addWidget(w82, 20, 0, 1, 2);
    geometry_adv_opt_group_polish_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_polish_cb->setText(QString::fromUtf8("Agregar acabado"));
    geometry_adv_opt_group_polish_cb->setToolTip(QString::fromUtf8("Agregará una sección de pintura al final del GCode.\nUn cepillo metálico limpiará el material después del fresado."));
    lay74->addWidget(geometry_adv_opt_group_polish_cb, 22, 0, 1, 2);
    geometry_adv_opt_group_polish_dia_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_dia_lbl->setText(QString::fromUtf8("Diá. de Herram:"));
    geometry_adv_opt_group_polish_dia_lbl->setToolTip(QString::fromUtf8("Diámetro de la herramienta de pulido."));
    lay74->addWidget(geometry_adv_opt_group_polish_dia_lbl, 24, 0, 1, 1);
    geometry_adv_opt_group_polish_dia_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_dia_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_dia_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_dia_entry->setProperty("value", 10.0);
    lay74->addWidget(geometry_adv_opt_group_polish_dia_entry, 24, 1, 1, 1);
    geometry_adv_opt_group_polish_travelz_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_travelz_lbl->setText(QString::fromUtf8("Viaje Z:"));
    geometry_adv_opt_group_polish_travelz_lbl->setToolTip(QString::fromUtf8("Altura de la herramienta cuando\nMoviéndose sin cortar."));
    lay74->addWidget(geometry_adv_opt_group_polish_travelz_lbl, 26, 0, 1, 1);
    geometry_adv_opt_group_polish_travelz_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("value", 2.0);
    lay74->addWidget(geometry_adv_opt_group_polish_travelz_entry, 26, 1, 1, 1);
    geometry_adv_opt_group_polish_pressure_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_pressure_lbl->setText(QString::fromUtf8("Presión:"));
    geometry_adv_opt_group_polish_pressure_lbl->setToolTip(QString::fromUtf8("Valor negativo. Cuanto mayor sea el valor absoluto\ncuanto más fuerte sea la presión del cepillo sobre el material."));
    lay74->addWidget(geometry_adv_opt_group_polish_pressure_lbl, 28, 0, 1, 1);
    geometry_adv_opt_group_polish_pressure_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("minimum", -10000.0);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("value", -1.0);
    lay74->addWidget(geometry_adv_opt_group_polish_pressure_entry, 28, 1, 1, 1);
    geometry_adv_opt_group_polish_margin_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_margin_lbl->setText(QString::fromUtf8("Margen:"));
    geometry_adv_opt_group_polish_margin_lbl->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay74->addWidget(geometry_adv_opt_group_polish_margin_lbl, 30, 0, 1, 1);
    geometry_adv_opt_group_polish_margin_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_margin_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_margin_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_margin_entry->setProperty("minimum", -10000.0);
    lay74->addWidget(geometry_adv_opt_group_polish_margin_entry, 30, 1, 1, 1);
    geometry_adv_opt_group_polish_over_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_over_lbl->setText(QString::fromUtf8("Superposición:"));
    geometry_adv_opt_group_polish_over_lbl->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta."));
    lay74->addWidget(geometry_adv_opt_group_polish_over_lbl, 32, 0, 1, 1);
    geometry_adv_opt_group_polish_over_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_over_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_over_entry->setProperty("maximum", 99.9999);
    geometry_adv_opt_group_polish_over_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_polish_over_entry->setProperty("suffix", QString::fromUtf8(" %"));
    geometry_adv_opt_group_polish_over_entry->setProperty("value", 5.0);
    geometry_adv_opt_group_polish_over_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_polish_over_entry, 32, 1, 1, 1);
    geometry_adv_opt_group_polish_method_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_method_lbl->setText(QString::fromUtf8("Método:"));
    geometry_adv_opt_group_polish_method_lbl->setToolTip(QString::fromUtf8("Algoritmo de pulido:\n- Estándar: Paso fijo hacia adentro.\n- Basado en semillas: hacia el exterior de la semilla.\n- Basado en líneas: líneas paralelas."));
    lay74->addWidget(geometry_adv_opt_group_polish_method_lbl, 34, 0, 1, 1);
    geometry_adv_opt_group_polish_method_combo = new FCComboBox2(nullptr);
    geometry_adv_opt_group_polish_method_combo->setProperty("currentText", QString::fromUtf8("Estándar"));
    geometry_adv_opt_group_polish_method_combo->addItem(QString::fromUtf8("Estándar"));
    geometry_adv_opt_group_polish_method_combo->addItem(QString::fromUtf8("Semilla"));
    geometry_adv_opt_group_polish_method_combo->addItem(QString::fromUtf8("Líneas"));
    lay74->addWidget(geometry_adv_opt_group_polish_method_combo, 34, 1, 1, 1);
    geometry_adv_opt_group_layout->addLayout(lay74, 0);
    geometry_adv_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    geometry_adv_opt_group->setLayout(geometry_adv_opt_group_layout);
    layout->addWidget(geometry_adv_opt_group, 0);
    geometry_editor_group = new QGroupBox(nullptr);
    { QFont f = geometry_editor_group->font(); f.setBold(true); f.setPixelSize(16); geometry_editor_group->setFont(f); }
    geometry_editor_group->setMinimumSize(QSize(250, 0));
    geometry_editor_group_layout = new QVBoxLayout();
    geometry_editor_group_layout->setContentsMargins(9, 9, 9, 9);
    geometry_editor_group_layout->setSpacing(6);
    geometry_editor_group_param_label = new QLabel(nullptr);
    geometry_editor_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    geometry_editor_group_param_label->setToolTip(QString::fromUtf8("Una lista de parámetros del editor."));
    geometry_editor_group_layout->addWidget(geometry_editor_group_param_label, 0);
    auto* lay83 = new QGridLayout();
    lay83->setContentsMargins(0, 0, 0, 0);
    lay83->setHorizontalSpacing(6);
    lay83->setVerticalSpacing(6);
    geometry_editor_group_sel_limit_label = new QLabel(nullptr);
    geometry_editor_group_sel_limit_label->setText(QString::fromUtf8("Límite de selección:"));
    geometry_editor_group_sel_limit_label->setToolTip(QString::fromUtf8("Establecer el número de geometría seleccionada\nelementos por encima de los cuales la geometría de utilidad\nse convierte en sólo un rectángulo de selección.\nAumenta el rendimiento al mover un\nGran cantidad de elementos geométricos."));
    lay83->addWidget(geometry_editor_group_sel_limit_label, 0, 0, 1, 1);
    geometry_editor_group_sel_limit_entry = new FCSpinner(nullptr);
    geometry_editor_group_sel_limit_entry->setProperty("maximum", 9999);
    geometry_editor_group_sel_limit_entry->setProperty("value", 30);
    lay83->addWidget(geometry_editor_group_sel_limit_entry, 0, 1, 1, 1);
    auto* w84 = new QLabel(nullptr);
    w84->setText(QString::fromUtf8("Tipo de fresado:"));
    w84->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    lay83->addWidget(w84, 1, 0, 1, 1);
    geometry_editor_group_milling_type_radio = new RadioSet({{QString::fromUtf8("Subida"), QVariant(QString::fromUtf8("cl"))}, {QString::fromUtf8("Convencional"), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    geometry_editor_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    lay83->addWidget(geometry_editor_group_milling_type_radio, 1, 1, 1, 1);
    geometry_editor_group_layout->addLayout(lay83, 0);
    geometry_editor_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    geometry_editor_group->setLayout(geometry_editor_group_layout);
    layout->addWidget(geometry_editor_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    geometry_defaults_form->setLayout(layout);
    geometry_scroll_area->setWidget(geometry_defaults_form);
    geometry_tab_lay->addWidget(geometry_scroll_area, 0);
    geometry_tab->setLayout(geometry_tab_lay);
    pref_tab_area->addTab(geometry_tab, QString::fromUtf8("GEOMETRÍA"));
    text_editor_tab = new QWidget(nullptr);
    text_editor_tab->setProperty("objectName", QString::fromUtf8("text_editor_tab"));
    text_editor_tab->setHidden(true);
    cncjob_tab_lay = new QVBoxLayout();
    cncjob_tab_lay->setContentsMargins(2, 2, 2, 2);
    cncjob_tab_lay->setSpacing(6);
    cncjob_scroll_area = new QScrollArea(nullptr);
    cncjob_defaults_form = new QWidget(nullptr);
    cncjob_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    cncjob_gen_group = new QGroupBox(nullptr);
    { QFont f = cncjob_gen_group->font(); f.setBold(true); f.setPixelSize(16); cncjob_gen_group->setFont(f); }
    cncjob_gen_group->setMinimumSize(QSize(260, 0));
    cncjob_gen_group_layout = new QVBoxLayout();
    cncjob_gen_group_layout->setContentsMargins(9, 9, 9, 9);
    cncjob_gen_group_layout->setSpacing(6);
    cncjob_gen_group_plot_options_label = new QLabel(nullptr);
    cncjob_gen_group_plot_options_label->setText(QString::fromUtf8("<b>Opciones de parcela:</b>"));
    cncjob_gen_group_layout->addWidget(cncjob_gen_group_plot_options_label, 0);
    auto* lay85 = new QGridLayout();
    lay85->setContentsMargins(0, 0, 0, 0);
    lay85->setHorizontalSpacing(6);
    lay85->setVerticalSpacing(6);
    cncjob_gen_group_plot_cb = new FCCheckBox(nullptr);
    cncjob_gen_group_plot_cb->setChecked(true);
    cncjob_gen_group_plot_cb->setText(QString::fromUtf8("Trazar objeto"));
    cncjob_gen_group_plot_cb->setToolTip(QString::fromUtf8("Trazar (mostrar) este objeto."));
    lay85->addWidget(cncjob_gen_group_plot_cb, 0, 0, 1, 2);
    cncjob_gen_group_steps_per_circle_label = new QLabel(nullptr);
    cncjob_gen_group_steps_per_circle_label->setText(QString::fromUtf8("Pasos del círculo:"));
    cncjob_gen_group_steps_per_circle_label->setToolTip(QString::fromUtf8("El número de pasos de círculo para <b> GCode </b>\nCírculo y arcos de aproximación lineal."));
    lay85->addWidget(cncjob_gen_group_steps_per_circle_label, 3, 0, 1, 1);
    cncjob_gen_group_steps_per_circle_entry = new FCSpinner(nullptr);
    cncjob_gen_group_steps_per_circle_entry->setProperty("maximum", 99999);
    cncjob_gen_group_steps_per_circle_entry->setProperty("value", 64);
    lay85->addWidget(cncjob_gen_group_steps_per_circle_entry, 3, 1, 1, 1);
    auto* w86 = new QLabel(nullptr);
    w86->setText(QString::fromUtf8("Dia de Viaje:"));
    w86->setToolTip(QString::fromUtf8("El ancho de las líneas de viaje a ser\nprestados en la trama."));
    lay85->addWidget(w86, 4, 0, 1, 1);
    cncjob_gen_group_tooldia_entry = new FCDoubleSpinner(nullptr);
    cncjob_gen_group_tooldia_entry->setProperty("decimals", 4);
    cncjob_gen_group_tooldia_entry->setProperty("maximum", 99999.0);
    cncjob_gen_group_tooldia_entry->setProperty("singleStep", 0.1);
    cncjob_gen_group_tooldia_entry->setProperty("value", 1.0);
    cncjob_gen_group_tooldia_entry->setWrapping(true);
    lay85->addWidget(cncjob_gen_group_tooldia_entry, 4, 1, 1, 1);
    auto* w87 = new QLabel(nullptr);
    w87->setText(QString::fromUtf8("<b>Decimales del código G:</b>"));
    lay85->addWidget(w87, 5, 0, 1, 2);
    auto* w88 = new QLabel(nullptr);
    w88->setText(QString::fromUtf8("Coordenadas:"));
    w88->setToolTip(QString::fromUtf8("El número de decimales a utilizar para\nLas coordenadas X, Y, Z en código CNC (GCODE, etc.)"));
    lay85->addWidget(w88, 6, 0, 1, 1);
    cncjob_gen_group_coords_dec_entry = new FCSpinner(nullptr);
    cncjob_gen_group_coords_dec_entry->setProperty("maximum", 9);
    cncjob_gen_group_coords_dec_entry->setProperty("value", 4);
    cncjob_gen_group_coords_dec_entry->setWrapping(true);
    lay85->addWidget(cncjob_gen_group_coords_dec_entry, 6, 1, 1, 1);
    auto* w89 = new QLabel(nullptr);
    w89->setText(QString::fromUtf8("Avance:"));
    w89->setToolTip(QString::fromUtf8("El número de decimales a utilizar para\nEl parámetro de avance en código CNC (GCODE, etc.)"));
    lay85->addWidget(w89, 7, 0, 1, 1);
    cncjob_gen_group_fr_dec_entry = new FCSpinner(nullptr);
    cncjob_gen_group_fr_dec_entry->setProperty("maximum", 9);
    cncjob_gen_group_fr_dec_entry->setProperty("value", 2);
    cncjob_gen_group_fr_dec_entry->setWrapping(true);
    lay85->addWidget(cncjob_gen_group_fr_dec_entry, 7, 1, 1, 1);
    auto* w90 = new QLabel(nullptr);
    w90->setText(QString::fromUtf8("Tipo de coordenadas:"));
    w90->setToolTip(QString::fromUtf8("El tipo de coordenadas que se utilizarán en Gcode.\nPuede ser:\n- G90 absoluto -> la referencia es el origen x = 0, y = 0\n- Incremental G91 -> la referencia es la posición anterior"));
    w90->setHidden(true);
    lay85->addWidget(w90, 8, 0, 1, 1);
    cncjob_gen_group_coords_type_radio = new RadioSet({{QString::fromUtf8("Absoluto"), QVariant(QString::fromUtf8("G90"))}, {QString::fromUtf8("Incremental"), QVariant(QString::fromUtf8("G91"))}}, Qt::Vertical, false, nullptr);
    cncjob_gen_group_coords_type_radio->setHidden(true);
    cncjob_gen_group_coords_type_radio->setValue(QVariant(QString::fromUtf8("G90")));
    lay85->addWidget(cncjob_gen_group_coords_type_radio, 8, 1, 1, 1);
    cncjob_gen_group_line_ending_cb = new FCCheckBox(nullptr);
    cncjob_gen_group_line_ending_cb->setText(QString::fromUtf8("Forzar el final de línea al estilo de Windows"));
    cncjob_gen_group_line_ending_cb->setToolTip(QString::fromUtf8("Cuando está marcado, forzará un final de línea de estilo Windows\n(\\r \\n) en sistemas operativos que no sean de Windows."));
    lay85->addWidget(cncjob_gen_group_line_ending_cb, 9, 0, 1, 3);
    auto* w91 = new QFrame(nullptr);
    w91->setProperty("frameShadow", 48);
    w91->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w91->setSizePolicy(sp); }
    lay85->addWidget(w91, 12, 0, 1, 2);
    cncjob_gen_group_travel_color_label = new QLabel(nullptr);
    cncjob_gen_group_travel_color_label->setText(QString::fromUtf8("<b>Color de Línea de Viaje</b>"));
    lay85->addWidget(cncjob_gen_group_travel_color_label, 13, 0, 1, 2);
    cncjob_gen_group_tline_color_label = new QLabel(nullptr);
    cncjob_gen_group_tline_color_label->setText(QString::fromUtf8("Contorno:"));
    cncjob_gen_group_tline_color_label->setToolTip(QString::fromUtf8("Establezca el color de la línea de viaje para los objetos trazados."));
    lay85->addWidget(cncjob_gen_group_tline_color_label, 14, 0, 1, 1);
    cncjob_gen_group_tline_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_tline_color_entry->setValue(QString::fromUtf8("#B5AB3A4c"));
    lay85->addWidget(cncjob_gen_group_tline_color_entry, 14, 1, 1, 1);
    cncjob_gen_group_tfill_color_label = new QLabel(nullptr);
    cncjob_gen_group_tfill_color_label->setText(QString::fromUtf8("Llenado:"));
    cncjob_gen_group_tfill_color_label->setToolTip(QString::fromUtf8("Establecer el color de relleno para los objetos trazados.\nLos primeros 6 dígitos son el color y los 2 últimos.\nLos dígitos son para el nivel alfa (transparencia)."));
    lay85->addWidget(cncjob_gen_group_tfill_color_label, 15, 0, 1, 1);
    cncjob_gen_group_tfill_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_tfill_color_entry->setValue(QString::fromUtf8("#F0E24D4c"));
    lay85->addWidget(cncjob_gen_group_tfill_color_entry, 15, 1, 1, 1);
    cncjob_gen_group_cncjob_alpha_label = new QLabel(nullptr);
    cncjob_gen_group_cncjob_alpha_label->setText(QString::fromUtf8("Alfa:"));
    cncjob_gen_group_cncjob_alpha_label->setToolTip(QString::fromUtf8("Establecer la transparencia de relleno para los objetos trazados."));
    lay85->addWidget(cncjob_gen_group_cncjob_alpha_label, 16, 0, 1, 1);
    cncjob_gen_group_cncjob_alpha_entry = new FCSliderWithSpinner(0, 255, 1, nullptr);
    cncjob_gen_group_cncjob_alpha_entry->setValue(76);
    lay85->addWidget(cncjob_gen_group_cncjob_alpha_entry, 16, 1, 1, 1);
    auto* w92 = new QFrame(nullptr);
    w92->setProperty("frameShadow", 48);
    w92->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w92->setSizePolicy(sp); }
    lay85->addWidget(w92, 17, 0, 1, 2);
    cncjob_gen_group_cnc_color_label = new QLabel(nullptr);
    cncjob_gen_group_cnc_color_label->setText(QString::fromUtf8("<b>Color del objeto</b>"));
    lay85->addWidget(cncjob_gen_group_cnc_color_label, 18, 0, 1, 2);
    cncjob_gen_group_line_color_label = new QLabel(nullptr);
    cncjob_gen_group_line_color_label->setText(QString::fromUtf8("Contorno:"));
    cncjob_gen_group_line_color_label->setToolTip(QString::fromUtf8("Establecer el color para los objetos trazados."));
    lay85->addWidget(cncjob_gen_group_line_color_label, 19, 0, 1, 1);
    cncjob_gen_group_line_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_line_color_entry->setValue(QString::fromUtf8("#4650BDFF"));
    lay85->addWidget(cncjob_gen_group_line_color_entry, 19, 1, 1, 1);
    cncjob_gen_group_fill_color_label = new QLabel(nullptr);
    cncjob_gen_group_fill_color_label->setText(QString::fromUtf8("Llenado:"));
    cncjob_gen_group_fill_color_label->setToolTip(QString::fromUtf8("Establecer el color de relleno para los objetos trazados.\nLos primeros 6 dígitos son el color y los 2 últimos.\nLos dígitos son para el nivel alfa (transparencia)."));
    lay85->addWidget(cncjob_gen_group_fill_color_label, 20, 0, 1, 1);
    cncjob_gen_group_fill_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_fill_color_entry->setValue(QString::fromUtf8("#5E6CFFFF"));
    lay85->addWidget(cncjob_gen_group_fill_color_entry, 20, 1, 1, 1);
    lay85->setColumnStretch(1, 1);
    cncjob_gen_group_layout->addLayout(lay85, 0);
    cncjob_gen_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    cncjob_gen_group->setLayout(cncjob_gen_group_layout);
    layout->addWidget(cncjob_gen_group, 0);
    auto* lay93 = new QVBoxLayout();
    lay93->setContentsMargins(0, 0, 0, 0);
    lay93->setSpacing(6);
    cncjob_opt_group = new QGroupBox(nullptr);
    { QFont f = cncjob_opt_group->font(); f.setBold(true); f.setPixelSize(16); cncjob_opt_group->setFont(f); }
    cncjob_opt_group->setMinimumSize(QSize(260, 0));
    cncjob_opt_group_layout = new QVBoxLayout();
    cncjob_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    cncjob_opt_group_layout->setSpacing(6);
    cncjob_opt_group_export_gcode_label = new QLabel(nullptr);
    cncjob_opt_group_export_gcode_label->setText(QString::fromUtf8("<b>Exportar G-Code:</b>"));
    cncjob_opt_group_export_gcode_label->setToolTip(QString::fromUtf8("Exportar y guardar código G a\nHacer este objeto a un archivo."));
    cncjob_opt_group_layout->addWidget(cncjob_opt_group_export_gcode_label, 0);
    auto* lay94 = new QGridLayout();
    lay94->setContentsMargins(0, 0, 0, 0);
    lay94->setHorizontalSpacing(6);
    lay94->setVerticalSpacing(6);
    cncjob_opt_group_cncplot_method_label = new QLabel(nullptr);
    cncjob_opt_group_cncplot_method_label->setText(QString::fromUtf8("Tipo de trazado:"));
    cncjob_opt_group_cncplot_method_label->setToolTip(QString::fromUtf8("Esto selecciona el tipo de geometrías en el lienzo para trazar.\nEsos pueden ser de tipo 'Viajes' lo que significa que los movimientos\nPor encima de la pieza de trabajo o puede ser de tipo 'Corte',\nLo que significa los movimientos que cortan en el material."));
    lay94->addWidget(cncjob_opt_group_cncplot_method_label, 1, 0, 1, 1);
    cncjob_opt_group_cncplot_method_radio = new RadioSet({{QString::fromUtf8("Todos"), QVariant(QString::fromUtf8("all"))}, {QString::fromUtf8("Viajar"), QVariant(QString::fromUtf8("travel"))}, {QString::fromUtf8("Cortar"), QVariant(QString::fromUtf8("cut"))}}, Qt::Vertical, false, nullptr);
    cncjob_opt_group_cncplot_method_radio->setValue(QVariant(QString::fromUtf8("all")));
    lay94->addWidget(cncjob_opt_group_cncplot_method_radio, 1, 1, 1, 1);
    auto* w95 = new QLabel(nullptr);
    lay94->addWidget(w95, 1, 2, 1, 1);
    cncjob_opt_group_annotation_cb = new FCCheckBox(nullptr);
    cncjob_opt_group_annotation_cb->setChecked(true);
    cncjob_opt_group_annotation_cb->setText(QString::fromUtf8("Mostrar anotación"));
    cncjob_opt_group_annotation_cb->setToolTip(QString::fromUtf8("Esto selecciona si mostrar la anotación de texto en el gráfico.\nCuando está marcado, mostrará números en orden para cada final.\nde una linea de viaje."));
    lay94->addWidget(cncjob_opt_group_annotation_cb, 2, 0, 1, 3);
    lay94->setColumnStretch(1, 1);
    cncjob_opt_group_layout->addLayout(lay94, 0);
    cncjob_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    cncjob_opt_group->setLayout(cncjob_opt_group_layout);
    lay93->addWidget(cncjob_opt_group, 0);
    cncjob_adv_opt_group = new QGroupBox(nullptr);
    { QFont f = cncjob_adv_opt_group->font(); f.setBold(true); f.setPixelSize(16); cncjob_adv_opt_group->setFont(f); }
    cncjob_adv_opt_group->setMinimumSize(QSize(260, 0));
    cncjob_adv_opt_group_layout = new QVBoxLayout();
    cncjob_adv_opt_group_layout->setContentsMargins(9, 9, 9, 9);
    cncjob_adv_opt_group_layout->setSpacing(6);
    auto* lay96 = new QGridLayout();
    lay96->setContentsMargins(0, 0, 0, 0);
    lay96->setHorizontalSpacing(6);
    lay96->setVerticalSpacing(6);
    cncjob_adv_opt_group_export_gcode_label = new QLabel(nullptr);
    cncjob_adv_opt_group_export_gcode_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    cncjob_adv_opt_group_export_gcode_label->setToolTip(QString::fromUtf8("Exportar y guardar código G a\nHacer este objeto a un archivo."));
    lay96->addWidget(cncjob_adv_opt_group_export_gcode_label, 0, 0, 1, 2);
    cncjob_adv_opt_group_annotation_fontsize_label = new QLabel(nullptr);
    cncjob_adv_opt_group_annotation_fontsize_label->setText(QString::fromUtf8("Tamaño de la anotación:"));
    cncjob_adv_opt_group_annotation_fontsize_label->setToolTip(QString::fromUtf8("El tamaño de fuente del texto de anotación. En píxeles."));
    lay96->addWidget(cncjob_adv_opt_group_annotation_fontsize_label, 2, 0, 1, 1);
    cncjob_adv_opt_group_annotation_fontsize_sp = new FCSpinner(nullptr);
    cncjob_adv_opt_group_annotation_fontsize_sp->setProperty("maximum", 9999);
    cncjob_adv_opt_group_annotation_fontsize_sp->setProperty("value", 9);
    lay96->addWidget(cncjob_adv_opt_group_annotation_fontsize_sp, 2, 1, 1, 1);
    cncjob_adv_opt_group_annotation_color_label = new QLabel(nullptr);
    cncjob_adv_opt_group_annotation_color_label->setText(QString::fromUtf8("Color de anotación:"));
    cncjob_adv_opt_group_annotation_color_label->setToolTip(QString::fromUtf8("Establecer el color de fuente para los textos de anotación."));
    lay96->addWidget(cncjob_adv_opt_group_annotation_color_label, 4, 0, 1, 1);
    cncjob_adv_opt_group_annotation_fontcolor_entry = new FCColorEntry(nullptr);
    cncjob_adv_opt_group_annotation_fontcolor_entry->setValue(QString::fromUtf8("#990000"));
    lay96->addWidget(cncjob_adv_opt_group_annotation_fontcolor_entry, 4, 1, 1, 1);
    cncjob_adv_opt_group_autolevelling_gcode_label = new QLabel(nullptr);
    cncjob_adv_opt_group_autolevelling_gcode_label->setText(QString::fromUtf8("<b>Nivelación automática</b>"));
    cncjob_adv_opt_group_autolevelling_gcode_label->setToolTip(QString::fromUtf8("Parámetros para la autonivelación."));
    lay96->addWidget(cncjob_adv_opt_group_autolevelling_gcode_label, 6, 0, 1, 2);
    auto* w97 = new FCLabel(nullptr);
    w97->setText(QString::fromUtf8("Modo:"));
    w97->setToolTip(QString::fromUtf8("Elija un modo para la generación de mapas de altura.\n- Manual: seleccionará una selección de puntos de sonda haciendo clic en el lienzo\n- Cuadrícula: generará automáticamente una cuadrícula de puntos de sonda"));
    lay96->addWidget(w97, 8, 0, 1, 1);
    cncjob_adv_opt_group_al_mode_radio = new RadioSet({{QString::fromUtf8("Manual"), QVariant(QString::fromUtf8("manual"))}, {QString::fromUtf8("Cuadrícula"), QVariant(QString::fromUtf8("grid"))}}, Qt::Horizontal, true, nullptr);
    cncjob_adv_opt_group_al_mode_radio->setValue(QVariant(QString::fromUtf8("grid")));
    lay96->addWidget(cncjob_adv_opt_group_al_mode_radio, 8, 1, 1, 1);
    cncjob_adv_opt_group_al_method_lbl = new FCLabel(nullptr);
    cncjob_adv_opt_group_al_method_lbl->setText(QString::fromUtf8("Método:"));
    cncjob_adv_opt_group_al_method_lbl->setToolTip(QString::fromUtf8("Elija un método para la aproximación de alturas a partir de datos de autonivelación.\n- Voronoi: generará un diagrama de Voronoi\n- Bilineal: utilizará interpolación bilineal. Solo se puede utilizar para el modo de cuadrícula."));
    lay96->addWidget(cncjob_adv_opt_group_al_method_lbl, 9, 0, 1, 1);
    cncjob_adv_opt_group_al_method_radio = new RadioSet({{QString::fromUtf8("Voronoi"), QVariant(QString::fromUtf8("v"))}, {QString::fromUtf8("Bilineal"), QVariant(QString::fromUtf8("b"))}}, Qt::Horizontal, true, nullptr);
    cncjob_adv_opt_group_al_method_radio->setValue(QVariant(QString::fromUtf8("v")));
    lay96->addWidget(cncjob_adv_opt_group_al_method_radio, 9, 1, 1, 1);
    cncjob_adv_opt_group_al_columns_label = new QLabel(nullptr);
    cncjob_adv_opt_group_al_columns_label->setText(QString::fromUtf8("Columnas:"));
    cncjob_adv_opt_group_al_columns_label->setToolTip(QString::fromUtf8("El número de columnas de la cuadrícula."));
    lay96->addWidget(cncjob_adv_opt_group_al_columns_label, 10, 0, 1, 1);
    cncjob_adv_opt_group_al_columns_entry = new FCSpinner(nullptr);
    cncjob_adv_opt_group_al_columns_entry->setProperty("value", 4);
    lay96->addWidget(cncjob_adv_opt_group_al_columns_entry, 10, 1, 1, 1);
    cncjob_adv_opt_group_al_rows_label = new QLabel(nullptr);
    cncjob_adv_opt_group_al_rows_label->setText(QString::fromUtf8("Filas:"));
    cncjob_adv_opt_group_al_rows_label->setToolTip(QString::fromUtf8("El número de filas de la cuadrícula."));
    lay96->addWidget(cncjob_adv_opt_group_al_rows_label, 12, 0, 1, 1);
    cncjob_adv_opt_group_al_rows_entry = new FCSpinner(nullptr);
    cncjob_adv_opt_group_al_rows_entry->setProperty("value", 4);
    lay96->addWidget(cncjob_adv_opt_group_al_rows_entry, 12, 1, 1, 1);
    cncjob_adv_opt_group_ptravelz_label = new QLabel(nullptr);
    cncjob_adv_opt_group_ptravelz_label->setText(QString::fromUtf8("Espacio de la sonda Z:"));
    cncjob_adv_opt_group_ptravelz_label->setToolTip(QString::fromUtf8("La Z segura para la sonda que viaja entre puntos de sonda."));
    lay96->addWidget(cncjob_adv_opt_group_ptravelz_label, 14, 0, 1, 1);
    cncjob_adv_opt_group_ptravelz_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_ptravelz_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_ptravelz_entry->setProperty("maximum", 10000.0);
    cncjob_adv_opt_group_ptravelz_entry->setProperty("value", 2.0);
    lay96->addWidget(cncjob_adv_opt_group_ptravelz_entry, 14, 1, 1, 1);
    cncjob_adv_opt_group_pdepth_label = new QLabel(nullptr);
    cncjob_adv_opt_group_pdepth_label->setText(QString::fromUtf8("Profundidad de la sonda Z:"));
    cncjob_adv_opt_group_pdepth_label->setToolTip(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units."));
    lay96->addWidget(cncjob_adv_opt_group_pdepth_label, 16, 0, 1, 1);
    cncjob_adv_opt_group_pdepth_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_pdepth_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_pdepth_entry->setProperty("maximum", 0.0);
    cncjob_adv_opt_group_pdepth_entry->setProperty("minimum", -910000.0);
    cncjob_adv_opt_group_pdepth_entry->setProperty("value", -1.0);
    lay96->addWidget(cncjob_adv_opt_group_pdepth_entry, 16, 1, 1, 1);
    cncjob_adv_opt_group_feedrate_probe_label = new QLabel(nullptr);
    cncjob_adv_opt_group_feedrate_probe_label->setText(QString::fromUtf8("Avance de la Sonda:"));
    cncjob_adv_opt_group_feedrate_probe_label->setToolTip(QString::fromUtf8("La velocidad de avance utilizada mientras la sonda está sondeando."));
    lay96->addWidget(cncjob_adv_opt_group_feedrate_probe_label, 18, 0, 1, 1);
    cncjob_adv_opt_group_feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_feedrate_probe_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_feedrate_probe_entry->setProperty("maximum", 910000.0);
    cncjob_adv_opt_group_feedrate_probe_entry->setProperty("value", 120.0);
    lay96->addWidget(cncjob_adv_opt_group_feedrate_probe_entry, 18, 1, 1, 1);
    auto* w98 = new QFrame(nullptr);
    w98->setProperty("frameShadow", 48);
    w98->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w98->setSizePolicy(sp); }
    lay96->addWidget(w98, 20, 0, 1, 2);
    cncjob_adv_opt_group_al_controller_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_al_controller_label->setText(QString::fromUtf8("Controlador:"));
    cncjob_adv_opt_group_al_controller_label->setToolTip(QString::fromUtf8("El tipo de controlador para el que generar\nmapa de altura gcode."));
    lay96->addWidget(cncjob_adv_opt_group_al_controller_label, 22, 0, 1, 1);
    cncjob_adv_opt_group_al_controller_combo = new FCComboBox(nullptr);
    cncjob_adv_opt_group_al_controller_combo->setProperty("currentText", QString::fromUtf8("MACH3"));
    cncjob_adv_opt_group_al_controller_combo->addItem(QString::fromUtf8("MACH3"));
    cncjob_adv_opt_group_al_controller_combo->addItem(QString::fromUtf8("MACH4"));
    cncjob_adv_opt_group_al_controller_combo->addItem(QString::fromUtf8("LinuxCNC"));
    cncjob_adv_opt_group_al_controller_combo->addItem(QString::fromUtf8("GRBL"));
    lay96->addWidget(cncjob_adv_opt_group_al_controller_combo, 22, 1, 1, 1);
    cncjob_adv_opt_group_jog_step_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_jog_step_label->setText(QString::fromUtf8("Paso:"));
    cncjob_adv_opt_group_jog_step_label->setToolTip(QString::fromUtf8("Cada acción de jog moverá los ejes con este valor."));
    lay96->addWidget(cncjob_adv_opt_group_jog_step_label, 24, 0, 1, 1);
    cncjob_adv_opt_group_jog_step_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_jog_step_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_jog_step_entry->setProperty("maximum", 910000.0);
    cncjob_adv_opt_group_jog_step_entry->setProperty("value", 5.0);
    lay96->addWidget(cncjob_adv_opt_group_jog_step_entry, 24, 1, 1, 1);
    cncjob_adv_opt_group_jog_fr_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_jog_fr_label->setText(QString::fromUtf8("Avance:"));
    cncjob_adv_opt_group_jog_fr_label->setToolTip(QString::fromUtf8("Avance al trotar."));
    lay96->addWidget(cncjob_adv_opt_group_jog_fr_label, 26, 0, 1, 1);
    cncjob_adv_opt_group_jog_fr_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_jog_fr_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_jog_fr_entry->setProperty("maximum", 910000.0);
    cncjob_adv_opt_group_jog_fr_entry->setProperty("value", 1500.0);
    lay96->addWidget(cncjob_adv_opt_group_jog_fr_entry, 26, 1, 1, 1);
    cncjob_adv_opt_group_jog_travelz_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_jog_travelz_label->setText(QString::fromUtf8("Viaje Z:"));
    cncjob_adv_opt_group_jog_travelz_label->setToolTip(QString::fromUtf8("Distancia de altura segura (Z) al trotar hasta el origen."));
    lay96->addWidget(cncjob_adv_opt_group_jog_travelz_label, 28, 0, 1, 1);
    cncjob_adv_opt_group_jog_travelz_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_jog_travelz_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_jog_travelz_entry->setProperty("maximum", 910000.0);
    cncjob_adv_opt_group_jog_travelz_entry->setProperty("value", 15.0);
    lay96->addWidget(cncjob_adv_opt_group_jog_travelz_entry, 28, 1, 1, 1);
    lay96->setColumnStretch(1, 1);
    cncjob_adv_opt_group_layout->addLayout(lay96, 0);
    cncjob_adv_opt_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    cncjob_adv_opt_group->setLayout(cncjob_adv_opt_group_layout);
    lay93->addWidget(cncjob_adv_opt_group, 0);
    layout->addLayout(lay93, 0);
    cncjob_editor_group = new QGroupBox(nullptr);
    { QFont f = cncjob_editor_group->font(); f.setBold(true); f.setPixelSize(16); cncjob_editor_group->setFont(f); }
    cncjob_editor_group->setMinimumSize(QSize(260, 0));
    cncjob_editor_group_layout = new QVBoxLayout();
    cncjob_editor_group_layout->setContentsMargins(9, 9, 9, 9);
    cncjob_editor_group_layout->setSpacing(6);
    cncjob_editor_group_param_label = new QLabel(nullptr);
    cncjob_editor_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    cncjob_editor_group_param_label->setToolTip(QString::fromUtf8("Una lista de parámetros del editor."));
    cncjob_editor_group_layout->addWidget(cncjob_editor_group_param_label, 0);
    auto* w99 = new QLabel(nullptr);
    w99->setText(QString::fromUtf8("Prefijo al código G:"));
    w99->setToolTip(QString::fromUtf8("Escribe aquí cualquier comando de G-Code que quieras\nMe gusta agregar al principio del archivo G-Code."));
    cncjob_editor_group_layout->addWidget(w99, 0);
    cncjob_editor_group_prepend_text = new FCTextArea(nullptr);
    { QFont f = cncjob_editor_group_prepend_text->font(); f.setPointSizeF(10.0); cncjob_editor_group_prepend_text->setFont(f); }
    cncjob_editor_group_prepend_text->setProperty("placeholderText", QString::fromUtf8("Escriba aquí los comandos de G-Code que le gustaría agregar al comienzo del archivo de G-Code."));
    cncjob_editor_group_layout->addWidget(cncjob_editor_group_prepend_text, 0);
    auto* w100 = new QLabel(nullptr);
    w100->setText(QString::fromUtf8("Adjuntar al código G:"));
    w100->setToolTip(QString::fromUtf8("Escribe aquí cualquier comando de código G que quieras\nMe gusta adjuntar al archivo generado.\nEs decir: M2 (Fin del programa)"));
    cncjob_editor_group_layout->addWidget(w100, 0);
    cncjob_editor_group_append_text = new FCTextArea(nullptr);
    { QFont f = cncjob_editor_group_append_text->font(); f.setPointSizeF(10.0); cncjob_editor_group_append_text->setFont(f); }
    cncjob_editor_group_append_text->setProperty("placeholderText", QString::fromUtf8("Escriba aquí los comandos de G-Code que le gustaría agregar al archivo generado.\nPor ejemplo: M2 (Fin del programa)"));
    cncjob_editor_group_layout->addWidget(cncjob_editor_group_append_text, 0);
    cncjob_editor_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    cncjob_editor_group->setLayout(cncjob_editor_group_layout);
    layout->addWidget(cncjob_editor_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    cncjob_defaults_form->setLayout(layout);
    cncjob_scroll_area->setWidget(cncjob_defaults_form);
    cncjob_tab_lay->addWidget(cncjob_scroll_area, 0);
    text_editor_tab->setLayout(cncjob_tab_lay);
    pref_tab_area->addTab(text_editor_tab, QString::fromUtf8("CNC-JOB"));
    tools_tab = new QWidget(nullptr);
    tools_tab->setHidden(true);
    tools_tab_lay = new QVBoxLayout();
    tools_tab_lay->setContentsMargins(2, 2, 2, 2);
    tools_tab_lay->setSpacing(6);
    tools_scroll_area = new QScrollArea(nullptr);
    tools_defaults_form = new QWidget(nullptr);
    tools_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    vlay = new QVBoxLayout();
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(6);
    tools_iso_group = new QGroupBox(nullptr);
    { QFont f = tools_iso_group->font(); f.setBold(true); f.setPixelSize(16); tools_iso_group->setFont(f); }
    tools_iso_group->setMinimumSize(QSize(220, 0));
    tools_iso_group_layout = new QVBoxLayout();
    tools_iso_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_iso_group_layout->setSpacing(6);
    tools_iso_group_iso_label = new QLabel(nullptr);
    tools_iso_group_iso_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_iso_group_iso_label->setToolTip(QString::fromUtf8("Crear un objeto de geometría con\nTrayectorias para cortar alrededor de polígonos."));
    tools_iso_group_layout->addWidget(tools_iso_group_iso_label, 0);
    auto* lay101 = new QGridLayout();
    lay101->setContentsMargins(0, 0, 0, 0);
    lay101->setHorizontalSpacing(6);
    lay101->setVerticalSpacing(6);
    auto* w102 = new QLabel(nullptr);
    w102->setText(QString::fromUtf8("<b><font color=\"green\">Diá. de Herram:</font></b>"));
    w102->setToolTip(QString::fromUtf8("Diámetros de las herramientas, separados por comas.\nEl valor del diámetro tiene que usar el separador de decimales de punto.\nValores válidos: 0.3, 1.0"));
    lay101->addWidget(w102, 0, 0, 1, 1);
    tools_iso_group_tool_dia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_iso_group_tool_dia_entry->setProperty("placeholderText", QString::fromUtf8("Valores Separados por Comas"));
    tools_iso_group_tool_dia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_iso_group_tool_dia_entry->setText(QString::fromUtf8("0.1"));
    lay101->addWidget(tools_iso_group_tool_dia_entry, 0, 1, 1, 2);
    tools_iso_group_order_label = new QLabel(nullptr);
    tools_iso_group_order_label->setText(QString::fromUtf8("Orden de la Herram:"));
    tools_iso_group_order_label->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    lay101->addWidget(tools_iso_group_order_label, 1, 0, 1, 1);
    tools_iso_group_order_radio = new RadioSet({{QString::fromUtf8("No"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Adelante"), QVariant(QString::fromUtf8("fwd"))}, {QString::fromUtf8("Atras"), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay101->addWidget(tools_iso_group_order_radio, 1, 1, 1, 2);
    tools_iso_group_tool_type_label = new QLabel(nullptr);
    tools_iso_group_tool_type_label->setText(QString::fromUtf8("Tipo de herram:"));
    tools_iso_group_tool_type_label->setToolTip(QString::fromUtf8("Tipo de herramienta predeterminada:\n- 'Forma V'\n- circular"));
    lay101->addWidget(tools_iso_group_tool_type_label, 2, 0, 1, 1);
    tools_iso_group_tool_type_radio = new RadioSet({{QString::fromUtf8("Forma V"), QVariant(QString::fromUtf8("V"))}, {QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("C1"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_tool_type_radio->setToolTip(QString::fromUtf8("Tipo de herramienta predeterminada:\n- 'Forma V'\n- circular"));
    tools_iso_group_tool_type_radio->setValue(QVariant(QString::fromUtf8("C1")));
    lay101->addWidget(tools_iso_group_tool_type_radio, 2, 1, 1, 2);
    tools_iso_group_tipdialabel = new QLabel(nullptr);
    tools_iso_group_tipdialabel->setText(QString::fromUtf8("V-Tipo Dia:"));
    tools_iso_group_tipdialabel->setToolTip(QString::fromUtf8("El diámetro de la punta para la herramienta en forma de V"));
    lay101->addWidget(tools_iso_group_tipdialabel, 3, 0, 1, 1);
    tools_iso_group_tipdia_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_tipdia_entry->setProperty("decimals", 4);
    tools_iso_group_tipdia_entry->setProperty("maximum", 1000.0);
    tools_iso_group_tipdia_entry->setProperty("singleStep", 0.1);
    tools_iso_group_tipdia_entry->setProperty("value", 0.1);
    lay101->addWidget(tools_iso_group_tipdia_entry, 3, 1, 1, 2);
    tools_iso_group_tipanglelabel = new QLabel(nullptr);
    tools_iso_group_tipanglelabel->setText(QString::fromUtf8("V-Tipo Ángulo:"));
    tools_iso_group_tipanglelabel->setToolTip(QString::fromUtf8("El ángulo de punta para la herramienta en forma de V.\nEn grados."));
    lay101->addWidget(tools_iso_group_tipanglelabel, 4, 0, 1, 1);
    tools_iso_group_tipangle_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_tipangle_entry->setProperty("decimals", 4);
    tools_iso_group_tipangle_entry->setProperty("maximum", 180.0);
    tools_iso_group_tipangle_entry->setProperty("minimum", 1.0);
    tools_iso_group_tipangle_entry->setProperty("singleStep", 5.0);
    tools_iso_group_tipangle_entry->setProperty("value", 30.0);
    tools_iso_group_tipangle_entry->setWrapping(true);
    lay101->addWidget(tools_iso_group_tipangle_entry, 4, 1, 1, 2);
    auto* w103 = new QLabel(nullptr);
    w103->setText(QString::fromUtf8("Corte Z:"));
    w103->setToolTip(QString::fromUtf8("Profundidad de corte en el material. Valor negativo.\nEn unidades de aplicación."));
    lay101->addWidget(w103, 5, 0, 1, 1);
    tools_iso_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_cutz_entry->setProperty("decimals", 4);
    tools_iso_group_cutz_entry->setProperty("maximum", 0.0);
    tools_iso_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_iso_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_iso_group_cutz_entry->setToolTip(QString::fromUtf8("Profundidad de corte en el material. Valor negativo.\nEn unidades de aplicación."));
    tools_iso_group_cutz_entry->setProperty("value", -0.05);
    lay101->addWidget(tools_iso_group_cutz_entry, 5, 1, 1, 2);
    tools_iso_group_newdialabel = new QLabel(nullptr);
    tools_iso_group_newdialabel->setText(QString::fromUtf8("Nuevo dia:"));
    tools_iso_group_newdialabel->setToolTip(QString::fromUtf8("Diámetro de la nueva herramienta para agregar en la Tabla de herramientas.\nSi la herramienta es de tipo V, este valor es automáticamente\ncalculado a partir de los otros parámetros."));
    lay101->addWidget(tools_iso_group_newdialabel, 6, 0, 1, 1);
    tools_iso_group_newdia_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_newdia_entry->setProperty("decimals", 4);
    tools_iso_group_newdia_entry->setProperty("maximum", 10000.0);
    tools_iso_group_newdia_entry->setProperty("minimum", 0.0001);
    tools_iso_group_newdia_entry->setProperty("singleStep", 0.1);
    tools_iso_group_newdia_entry->setProperty("value", 0.1);
    lay101->addWidget(tools_iso_group_newdia_entry, 6, 1, 1, 2);
    auto* w104 = new QFrame(nullptr);
    w104->setProperty("frameShadow", 48);
    w104->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w104->setSizePolicy(sp); }
    lay101->addWidget(w104, 7, 0, 1, 3);
    auto* w105 = new QLabel(nullptr);
    w105->setText(QString::fromUtf8("Pases:"));
    w105->setToolTip(QString::fromUtf8("Ancho de la brecha de aislamiento en\nNúmero (entero) de anchos de herramienta."));
    lay101->addWidget(w105, 8, 0, 1, 1);
    tools_iso_group_passes_entry = new FCSpinner(nullptr);
    tools_iso_group_passes_entry->setProperty("maximum", 999);
    tools_iso_group_passes_entry->setProperty("minimum", 1);
    tools_iso_group_passes_entry->setProperty("objectName", QString::fromUtf8("i_passes"));
    tools_iso_group_passes_entry->setProperty("value", 1);
    lay101->addWidget(tools_iso_group_passes_entry, 8, 1, 1, 2);
    auto* w106 = new QLabel(nullptr);
    w106->setText(QString::fromUtf8("Superposición:"));
    w106->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta."));
    lay101->addWidget(w106, 9, 0, 1, 1);
    tools_iso_group_overlap_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_overlap_entry->setProperty("decimals", 4);
    tools_iso_group_overlap_entry->setProperty("maximum", 99.9999);
    tools_iso_group_overlap_entry->setProperty("objectName", QString::fromUtf8("i_overlap"));
    tools_iso_group_overlap_entry->setProperty("singleStep", 0.1);
    tools_iso_group_overlap_entry->setProperty("suffix", QString::fromUtf8(" %"));
    tools_iso_group_overlap_entry->setProperty("value", 10.0);
    tools_iso_group_overlap_entry->setWrapping(true);
    lay101->addWidget(tools_iso_group_overlap_entry, 9, 1, 1, 2);
    tools_iso_group_milling_type_label = new QLabel(nullptr);
    tools_iso_group_milling_type_label->setText(QString::fromUtf8("Tipo de fresado:"));
    tools_iso_group_milling_type_label->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    lay101->addWidget(tools_iso_group_milling_type_label, 10, 0, 1, 1);
    tools_iso_group_milling_type_radio = new RadioSet({{QString::fromUtf8("Subida"), QVariant(QString::fromUtf8("cl"))}, {QString::fromUtf8("Convencional"), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_milling_type_radio->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    tools_iso_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    lay101->addWidget(tools_iso_group_milling_type_radio, 10, 1, 1, 2);
    tools_iso_group_follow_label = new QLabel(nullptr);
    tools_iso_group_follow_label->setText(QString::fromUtf8("Seguir:"));
    tools_iso_group_follow_label->setToolTip(QString::fromUtf8("Generar una geometría 'Seguir'.\nEsto significa que cortará a través\nEl medio de la traza."));
    lay101->addWidget(tools_iso_group_follow_label, 11, 0, 1, 1);
    tools_iso_group_follow_cb = new FCCheckBox(nullptr);
    tools_iso_group_follow_cb->setProperty("objectName", QString::fromUtf8("i_follow"));
    tools_iso_group_follow_cb->setToolTip(QString::fromUtf8("Generar una geometría 'Seguir'.\nEsto significa que cortará a través\nEl medio de la traza."));
    lay101->addWidget(tools_iso_group_follow_cb, 11, 1, 1, 2);
    tools_iso_group_iso_type_label = new QLabel(nullptr);
    tools_iso_group_iso_type_label->setText(QString::fromUtf8("Tipo de aislamiento:"));
    tools_iso_group_iso_type_label->setToolTip(QString::fromUtf8("Elija cómo se ejecutará el aislamiento:\n- 'Completo' -> aislamiento completo de polígonos\n- 'Ext' -> aislará solo en el exterior\n- 'Int' -> aislará solo en el interior\nEl aislamiento 'exterior' es casi siempre posible\n(con la herramienta adecuada) pero 'Interior'\nel aislamiento solo se puede hacer cuando hay una abertura\ndentro del polígono (por ejemplo, el polígono tiene forma de 'rosquilla')."));
    lay101->addWidget(tools_iso_group_iso_type_label, 12, 0, 1, 1);
    tools_iso_group_iso_type_radio = new RadioSet({{QString::fromUtf8("Completo"), QVariant(QString::fromUtf8("full"))}, {QString::fromUtf8("Exterior"), QVariant(QString::fromUtf8("ext"))}, {QString::fromUtf8("Interior"), QVariant(QString::fromUtf8("int"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_iso_type_radio->setProperty("objectName", QString::fromUtf8("i_type"));
    tools_iso_group_iso_type_radio->setValue(QVariant(QString::fromUtf8("full")));
    lay101->addWidget(tools_iso_group_iso_type_radio, 12, 1, 1, 2);
    auto* w107 = new QFrame(nullptr);
    w107->setProperty("frameShadow", 48);
    w107->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w107->setSizePolicy(sp); }
    lay101->addWidget(w107, 13, 0, 1, 3);
    tools_iso_group_rest_cb = new FCCheckBox(nullptr);
    tools_iso_group_rest_cb->setProperty("objectName", QString::fromUtf8("i_rest_machining"));
    tools_iso_group_rest_cb->setText(QString::fromUtf8("Resto"));
    tools_iso_group_rest_cb->setToolTip(QString::fromUtf8("Si está marcado, use 'mecanizado en resto'.\nBásicamente procesará cobre fuera de las características de la PCB,\nusando la herramienta más grande y continúe con las siguientes herramientas,\nde mayor a menor, para procesar las características de cobre que\nno pudo ser procesado por la herramienta anterior, hasta que haya\nno queda nada por procesar o no hay más herramientas.\n\nSi no está marcado, use el algoritmo estándar."));
    lay101->addWidget(tools_iso_group_rest_cb, 17, 0, 1, 1);
    tools_iso_group_combine_passes_cb = new FCCheckBox(nullptr);
    tools_iso_group_combine_passes_cb->setChecked(true);
    tools_iso_group_combine_passes_cb->setProperty("objectName", QString::fromUtf8("i_combine"));
    tools_iso_group_combine_passes_cb->setText(QString::fromUtf8("Combinar"));
    tools_iso_group_combine_passes_cb->setToolTip(QString::fromUtf8("Combina todos los pases en un objeto"));
    lay101->addWidget(tools_iso_group_combine_passes_cb, 17, 1, 1, 1);
    tools_iso_group_except_cb = new FCCheckBox(nullptr);
    tools_iso_group_except_cb->setProperty("objectName", QString::fromUtf8("i_except"));
    tools_iso_group_except_cb->setText(QString::fromUtf8("Excepto"));
    tools_iso_group_except_cb->setToolTip(QString::fromUtf8("Cuando se genera la geometría de Aislamiento,\nmarcando esto, el área del objeto a continuación\nserá restado de la geometría de aislamiento."));
    lay101->addWidget(tools_iso_group_except_cb, 17, 2, 1, 1);
    tools_iso_group_valid_cb = new FCCheckBox(nullptr);
    tools_iso_group_valid_cb->setProperty("objectName", QString::fromUtf8("i_check"));
    tools_iso_group_valid_cb->setText(QString::fromUtf8("Verificar validez"));
    tools_iso_group_valid_cb->setToolTip(QString::fromUtf8("Si se marca, se verifican los diámetros de las herramientas.\nsi proporcionarán un aislamiento completo."));
    lay101->addWidget(tools_iso_group_valid_cb, 18, 0, 1, 3);
    tools_iso_group_select_label = new QLabel(nullptr);
    tools_iso_group_select_label->setText(QString::fromUtf8("Selección:"));
    tools_iso_group_select_label->setToolTip(QString::fromUtf8("Alcance de aislamiento. Elija qué aislar:\n- 'Todos' -> Aislar todos los polígonos en el objeto\n- 'Selección de área' -> Aislar polígonos dentro de un área de selección.\n- 'Selección de polígonos' -> Aislar una selección de polígonos.\n- 'Objeto de referencia': procesará el área especificada por otro objeto."));
    lay101->addWidget(tools_iso_group_select_label, 20, 0, 1, 1);
    tools_iso_group_select_combo = new FCComboBox2(nullptr);
    tools_iso_group_select_combo->setProperty("currentText", QString::fromUtf8("Todos"));
    tools_iso_group_select_combo->setProperty("objectName", QString::fromUtf8("i_selection"));
    tools_iso_group_select_combo->addItem(QString::fromUtf8("Todos"));
    tools_iso_group_select_combo->addItem(QString::fromUtf8("Selección de área"));
    tools_iso_group_select_combo->addItem(QString::fromUtf8("Selección de polígono"));
    tools_iso_group_select_combo->addItem(QString::fromUtf8("Objeto de referencia"));
    lay101->addWidget(tools_iso_group_select_combo, 20, 1, 1, 2);
    tools_iso_group_area_shape_label = new QLabel(nullptr);
    tools_iso_group_area_shape_label->setText(QString::fromUtf8("Forma:"));
    tools_iso_group_area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    lay101->addWidget(tools_iso_group_area_shape_label, 21, 0, 1, 1);
    tools_iso_group_area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay101->addWidget(tools_iso_group_area_shape_radio, 21, 1, 1, 2);
    tools_iso_group_poly_int_cb = new FCCheckBox(nullptr);
    tools_iso_group_poly_int_cb->setText(QString::fromUtf8("Interiores"));
    tools_iso_group_poly_int_cb->setToolTip(QString::fromUtf8("Cuando está marcada, el usuario puede seleccionar interiores de un polígono.\n(agujeros en el polígono)."));
    lay101->addWidget(tools_iso_group_poly_int_cb, 22, 0, 1, 1);
    tools_iso_group_force_iso_cb = new FCCheckBox(nullptr);
    tools_iso_group_force_iso_cb->setChecked(true);
    tools_iso_group_force_iso_cb->setText(QString::fromUtf8("Forzado Resto"));
    tools_iso_group_force_iso_cb->setToolTip(QString::fromUtf8("Cuando se marca, el aislamiento se realizará con la herramienta actual, incluso si\nNo se pudieron aislar los interiores de un polígono (agujeros en el polígono).\nFunciona cuando se usa 'mecanizado en reposo'."));
    lay101->addWidget(tools_iso_group_force_iso_cb, 22, 1, 1, 1);
    auto* w108 = new QFrame(nullptr);
    w108->setProperty("frameShadow", 48);
    w108->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w108->setSizePolicy(sp); }
    lay101->addWidget(w108, 24, 0, 1, 3);
    auto* w109 = new QLabel(nullptr);
    w109->setText(QString::fromUtf8("Trazado:"));
    w109->setToolTip(QString::fromUtf8("- 'Normal': trazado normal, realizado al final del trabajo\n- 'Progresivo': cada forma se traza después de generarse"));
    lay101->addWidget(w109, 25, 0, 1, 1);
    tools_iso_group_plotting_radio = new RadioSet({{QString::fromUtf8("Normal"), QVariant(QString::fromUtf8("normal"))}, {QString::fromUtf8("Progresivo"), QVariant(QString::fromUtf8("progressive"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_plotting_radio->setValue(QVariant(QString::fromUtf8("normal")));
    lay101->addWidget(tools_iso_group_plotting_radio, 25, 1, 1, 2);
    tools_iso_group_layout->addLayout(lay101, 0);
    tools_iso_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_iso_group->setLayout(tools_iso_group_layout);
    vlay->addWidget(tools_iso_group, 0);
    tools_2sided_group = new QGroupBox(nullptr);
    { QFont f = tools_2sided_group->font(); f.setBold(true); f.setPixelSize(16); tools_2sided_group->setFont(f); }
    tools_2sided_group->setMinimumSize(QSize(220, 0));
    tools_2sided_group_layout = new QVBoxLayout();
    tools_2sided_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_2sided_group_layout->setSpacing(6);
    tools_2sided_group_dblsided_label = new QLabel(nullptr);
    tools_2sided_group_dblsided_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_2sided_group_dblsided_label->setToolTip(QString::fromUtf8("Una herramienta para ayudar en la creación de una doble cara.\nPCB utilizando orificios de alineación."));
    tools_2sided_group_layout->addWidget(tools_2sided_group_dblsided_label, 0);
    auto* lay110 = new QGridLayout();
    lay110->setContentsMargins(0, 0, 0, 0);
    lay110->setHorizontalSpacing(6);
    lay110->setVerticalSpacing(6);
    tools_2sided_group_dd_label = new QLabel(nullptr);
    tools_2sided_group_dd_label->setText(QString::fromUtf8("Diá. del Taladro:"));
    tools_2sided_group_dd_label->setToolTip(QString::fromUtf8("Diámetro del taladro para los orificios de alineación."));
    lay110->addWidget(tools_2sided_group_dd_label, 0, 0, 1, 1);
    tools_2sided_group_drill_dia_entry = new FCDoubleSpinner(nullptr);
    tools_2sided_group_drill_dia_entry->setProperty("decimals", 4);
    tools_2sided_group_drill_dia_entry->setProperty("maximum", 10000.0);
    tools_2sided_group_drill_dia_entry->setProperty("singleStep", 0.1);
    tools_2sided_group_drill_dia_entry->setProperty("value", 3.125);
    lay110->addWidget(tools_2sided_group_drill_dia_entry, 0, 1, 1, 1);
    tools_2sided_group_align_ax_label = new QLabel(nullptr);
    tools_2sided_group_align_ax_label->setText(QString::fromUtf8("Alinear eje:"));
    tools_2sided_group_align_ax_label->setToolTip(QString::fromUtf8("Espejo verticalmente (X) u horizontal (Y)."));
    lay110->addWidget(tools_2sided_group_align_ax_label, 1, 0, 1, 1);
    tools_2sided_group_align_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}}, Qt::Horizontal, true, nullptr);
    tools_2sided_group_align_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay110->addWidget(tools_2sided_group_align_axis_radio, 1, 1, 1, 1);
    auto* w111 = new QFrame(nullptr);
    w111->setProperty("frameShadow", 48);
    w111->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w111->setSizePolicy(sp); }
    lay110->addWidget(w111, 2, 0, 1, 2);
    tools_2sided_group_mirax_label = new QLabel(nullptr);
    tools_2sided_group_mirax_label->setText(QString::fromUtf8("Eje espejo:"));
    tools_2sided_group_mirax_label->setToolTip(QString::fromUtf8("Espejo verticalmente (X) u horizontal (Y)."));
    lay110->addWidget(tools_2sided_group_mirax_label, 3, 0, 1, 1);
    tools_2sided_group_mirror_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}}, Qt::Horizontal, true, nullptr);
    tools_2sided_group_mirror_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay110->addWidget(tools_2sided_group_mirror_axis_radio, 3, 1, 1, 1);
    tools_2sided_group_axloc_label = new QLabel(nullptr);
    tools_2sided_group_axloc_label->setText(QString::fromUtf8("Ref. del eje:"));
    tools_2sided_group_axloc_label->setToolTip(QString::fromUtf8("Las coordenadas utilizadas como referencia para la operación de espejo.\nPuede ser:\n- Punto -> un conjunto de coordenadas (x, y) alrededor del cual se refleja el objeto\n- Caja -> un conjunto de coordenadas (x, y) obtenidas del centro del\ncuadro delimitador de otro objeto seleccionado a continuación\n- Agujero Snap-> un punto definido por el centro de un taladro en un objeto Excellon"));
    lay110->addWidget(tools_2sided_group_axloc_label, 4, 0, 1, 1);
    tools_2sided_group_axis_location_radio = new RadioSet({{QString::fromUtf8("Punto"), QVariant(QString::fromUtf8("point"))}, {QString::fromUtf8("Caja"), QVariant(QString::fromUtf8("box"))}, {QString::fromUtf8("Encajar en el agujero"), QVariant(QString::fromUtf8("hole"))}}, Qt::Horizontal, true, nullptr);
    tools_2sided_group_axis_location_radio->setValue(QVariant(QString::fromUtf8("point")));
    lay110->addWidget(tools_2sided_group_axis_location_radio, 4, 1, 1, 1);
    tools_2sided_group_layout->addLayout(lay110, 0);
    tools_2sided_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_2sided_group->setLayout(tools_2sided_group_layout);
    vlay->addWidget(tools_2sided_group, 0);
    tools_cutout_group = new QGroupBox(nullptr);
    { QFont f = tools_cutout_group->font(); f.setBold(true); f.setPixelSize(16); tools_cutout_group->setFont(f); }
    tools_cutout_group->setMinimumSize(QSize(220, 0));
    tools_cutout_group_layout = new QVBoxLayout();
    tools_cutout_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_cutout_group_layout->setSpacing(6);
    tools_cutout_group_board_cutout_label = new FCLabel(nullptr);
    tools_cutout_group_board_cutout_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_cutout_group_board_cutout_label->setToolTip(QString::fromUtf8("Crear caminos de herramientas para cortar alrededor\nEl PCB y lo separa de\nEl tablero original."));
    tools_cutout_group_layout->addWidget(tools_cutout_group_board_cutout_label, 0);
    auto* lay112 = new QGridLayout();
    lay112->setContentsMargins(0, 0, 0, 0);
    lay112->setHorizontalSpacing(6);
    lay112->setVerticalSpacing(6);
    auto* w113 = new FCLabel(nullptr);
    w113->setText(QString::fromUtf8("Diá. de Herram:"));
    w113->setToolTip(QString::fromUtf8("Diámetro de la herramienta utilizada para cortar\nLa forma de PCB fuera del material circundante."));
    lay112->addWidget(w113, 0, 0, 1, 1);
    tools_cutout_group_cutout_tooldia_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutout_tooldia_entry->setProperty("decimals", 4);
    tools_cutout_group_cutout_tooldia_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_cutout_tooldia_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutout_tooldia_entry->setProperty("value", 2.4);
    lay112->addWidget(tools_cutout_group_cutout_tooldia_entry, 0, 1, 1, 1);
    auto* w114 = new FCLabel(nullptr);
    w114->setText(QString::fromUtf8("Corte Z:"));
    w114->setToolTip(QString::fromUtf8("Profundidad de corte (negativo)\ndebajo de la superficie de cobre."));
    lay112->addWidget(w114, 1, 0, 1, 1);
    tools_cutout_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutz_entry->setProperty("decimals", 4);
    tools_cutout_group_cutz_entry->setProperty("maximum", 0.0);
    tools_cutout_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_cutout_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutz_entry->setProperty("value", -1.8);
    lay112->addWidget(tools_cutout_group_cutz_entry, 1, 1, 1, 1);
    tools_cutout_group_mpass_cb = new FCCheckBox(nullptr);
    tools_cutout_group_mpass_cb->setChecked(true);
    tools_cutout_group_mpass_cb->setText(QString::fromUtf8("Profund. Múlti:"));
    tools_cutout_group_mpass_cb->setToolTip(QString::fromUtf8("Usa múltiples pases para limitar\nLa profundidad de corte en cada pasada. Será\ncortar varias veces hasta que el Corte Z sea\nalcanzado."));
    lay112->addWidget(tools_cutout_group_mpass_cb, 2, 0, 1, 1);
    tools_cutout_group_maxdepth_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_maxdepth_entry->setProperty("decimals", 4);
    tools_cutout_group_maxdepth_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_maxdepth_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_maxdepth_entry->setToolTip(QString::fromUtf8("Profundidad de cada pase (positivo)."));
    tools_cutout_group_maxdepth_entry->setProperty("value", 0.6);
    lay112->addWidget(tools_cutout_group_maxdepth_entry, 2, 1, 1, 1);
    auto* w115 = new FCLabel(nullptr);
    w115->setText(QString::fromUtf8("Tipo:"));
    w115->setToolTip(QString::fromUtf8("Elección del tipo de objeto que queremos recortar.\n- Único: contiene un único objeto de contorno de PCB Gerber.\n- Panel: un objeto de panel PCB Gerber, que se hace\nde muchos esquemas de PCB individuales."));
    lay112->addWidget(w115, 3, 0, 1, 1);
    tools_cutout_group_obj_kind_combo = new RadioSet({{QString::fromUtf8("Soltero"), QVariant(QString::fromUtf8("single"))}, {QString::fromUtf8("Panel"), QVariant(QString::fromUtf8("panel"))}}, Qt::Horizontal, true, nullptr);
    tools_cutout_group_obj_kind_combo->setValue(QVariant(QString::fromUtf8("single")));
    lay112->addWidget(tools_cutout_group_obj_kind_combo, 3, 1, 1, 1);
    auto* w116 = new FCLabel(nullptr);
    w116->setText(QString::fromUtf8("Margen:"));
    w116->setToolTip(QString::fromUtf8("Margen sobre los límites. Un valor positivo aquí\nhará que el corte de la PCB esté más alejado de\nel borde real de PCB"));
    lay112->addWidget(w116, 4, 0, 1, 1);
    tools_cutout_group_cutout_margin_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutout_margin_entry->setProperty("decimals", 4);
    tools_cutout_group_cutout_margin_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_cutout_margin_entry->setProperty("minimum", -10000.0);
    tools_cutout_group_cutout_margin_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutout_margin_entry->setProperty("value", 0.1);
    lay112->addWidget(tools_cutout_group_cutout_margin_entry, 4, 1, 1, 1);
    auto* w117 = new FCLabel(nullptr);
    w117->setText(QString::fromUtf8("Tamaño de la brecha:"));
    w117->setToolTip(QString::fromUtf8("El tamaño de los huecos del puente en el recorte\nsolía mantener la placa conectada a\nel material circundante (el\nde la cual se corta el PCB)."));
    lay112->addWidget(w117, 5, 0, 1, 1);
    tools_cutout_group_cutout_gap_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutout_gap_entry->setProperty("decimals", 4);
    tools_cutout_group_cutout_gap_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_cutout_gap_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutout_gap_entry->setProperty("value", 4.0);
    lay112->addWidget(tools_cutout_group_cutout_gap_entry, 5, 1, 1, 1);
    tools_cutout_group_gaptype_label = new FCLabel(nullptr);
    tools_cutout_group_gaptype_label->setText(QString::fromUtf8("Tipo de brecha:"));
    tools_cutout_group_gaptype_label->setToolTip(QString::fromUtf8("El tipo de brecha:\n- Puente -> el corte será interrumpido por puentes\n- Delgado -> igual que 'puente' pero será más delgado al fresar parcialmente el espacio\n- M-Bites -> 'Mouse Bites' - igual que el 'puente' pero cubierto con agujeros"));
    lay112->addWidget(tools_cutout_group_gaptype_label, 7, 0, 1, 1);
    tools_cutout_group_gaptype_radio = new RadioSet({{QString::fromUtf8("Puente"), QVariant(QString::fromUtf8("b"))}, {QString::fromUtf8("Delgado"), QVariant(QString::fromUtf8("bt"))}, {QString::fromUtf8("M-Bites"), QVariant(QString::fromUtf8("mb"))}}, Qt::Horizontal, true, nullptr);
    tools_cutout_group_gaptype_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay112->addWidget(tools_cutout_group_gaptype_radio, 7, 1, 1, 1);
    tools_cutout_group_thin_depth_label = new FCLabel(nullptr);
    tools_cutout_group_thin_depth_label->setText(QString::fromUtf8("Profundidad:"));
    tools_cutout_group_thin_depth_label->setToolTip(QString::fromUtf8("La profundidad hasta que se hace el fresado\npara adelgazar los huecos."));
    lay112->addWidget(tools_cutout_group_thin_depth_label, 9, 0, 1, 1);
    tools_cutout_group_thin_depth_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_thin_depth_entry->setProperty("decimals", 4);
    tools_cutout_group_thin_depth_entry->setProperty("maximum", -0.0);
    tools_cutout_group_thin_depth_entry->setProperty("minimum", -10000.0);
    tools_cutout_group_thin_depth_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_thin_depth_entry->setProperty("value", -1.0);
    lay112->addWidget(tools_cutout_group_thin_depth_entry, 9, 1, 1, 1);
    tools_cutout_group_mb_dia_label = new FCLabel(nullptr);
    tools_cutout_group_mb_dia_label->setText(QString::fromUtf8("Diá. de Herram:"));
    tools_cutout_group_mb_dia_label->setToolTip(QString::fromUtf8("El diámetro del taladro al hacer 'mouse bytes'."));
    lay112->addWidget(tools_cutout_group_mb_dia_label, 11, 0, 1, 1);
    tools_cutout_group_mb_dia_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_mb_dia_entry->setProperty("decimals", 4);
    tools_cutout_group_mb_dia_entry->setProperty("maximum", 100.0);
    tools_cutout_group_mb_dia_entry->setProperty("value", 0.6);
    lay112->addWidget(tools_cutout_group_mb_dia_entry, 11, 1, 1, 1);
    tools_cutout_group_mb_spacing_label = new FCLabel(nullptr);
    tools_cutout_group_mb_spacing_label->setText(QString::fromUtf8("Spacing:"));
    tools_cutout_group_mb_spacing_label->setToolTip(QString::fromUtf8("El espacio entre los taladros al hacer 'mouse bites'."));
    lay112->addWidget(tools_cutout_group_mb_spacing_label, 13, 0, 1, 1);
    tools_cutout_group_mb_spacing_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_mb_spacing_entry->setProperty("decimals", 4);
    tools_cutout_group_mb_spacing_entry->setProperty("maximum", 100.0);
    tools_cutout_group_mb_spacing_entry->setProperty("value", 0.3);
    lay112->addWidget(tools_cutout_group_mb_spacing_entry, 13, 1, 1, 1);
    auto* w118 = new FCLabel(nullptr);
    w118->setText(QString::fromUtf8("Brechas:"));
    w118->setToolTip(QString::fromUtf8("Número de huecos de puente utilizados para el recorte.\nPuede haber un máximo de 8 puentes / huecos.\nLas opciones son:\n- Ninguno - sin espacios\n- lr - izquierda + derecha\n- tb - arriba + abajo\n- 4 - izquierda + derecha + arriba + abajo\n- 2lr - 2 * izquierda + 2 * derecha\n- 2tb - 2 * top + 2 * bottom\n- 8 - 2 * izquierda + 2 * derecha + 2 * arriba + 2 * abajo"));
    lay112->addWidget(w118, 15, 0, 1, 1);
    tools_cutout_group_gaps_combo = new FCComboBox(nullptr);
    tools_cutout_group_gaps_combo->setProperty("currentText", QString::fromUtf8("4"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("None"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("LR"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("TB"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("4"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("2LR"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("2TB"));
    tools_cutout_group_gaps_combo->addItem(QString::fromUtf8("8"));
    tools_cutout_group_gaps_combo->setCurrentIndex(3);
    lay112->addWidget(tools_cutout_group_gaps_combo, 15, 1, 1, 1);
    tools_cutout_group_convex_box = new FCCheckBox(nullptr);
    tools_cutout_group_convex_box->setText(QString::fromUtf8("Forma convexa"));
    tools_cutout_group_convex_box->setToolTip(QString::fromUtf8("Crea una forma convexa que rodea toda la PCB.\nSe usa solo si el tipo de objeto de origen es Gerber."));
    lay112->addWidget(tools_cutout_group_convex_box, 17, 0, 1, 2);
    tools_cutout_group_big_cursor_cb = new FCCheckBox(nullptr);
    tools_cutout_group_big_cursor_cb->setChecked(true);
    tools_cutout_group_big_cursor_cb->setText(QString::fromUtf8("Cursor grande"));
    tools_cutout_group_big_cursor_cb->setToolTip(QString::fromUtf8("Use un cursor grande cuando agregue espacios manuales."));
    lay112->addWidget(tools_cutout_group_big_cursor_cb, 19, 0, 1, 2);
    tools_cutout_group_layout->addLayout(lay112, 0);
    tools_cutout_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_cutout_group->setLayout(tools_cutout_group_layout);
    vlay->addWidget(tools_cutout_group, 0);
    layout->addLayout(vlay, 0);
    vlay1 = new QVBoxLayout();
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->setSpacing(6);
    tools_drill_group = new QGroupBox(nullptr);
    { QFont f = tools_drill_group->font(); f.setBold(true); f.setPixelSize(16); tools_drill_group->setFont(f); }
    tools_drill_group->setMinimumSize(QSize(220, 0));
    tools_drill_group_layout = new QVBoxLayout();
    tools_drill_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_drill_group_layout->setSpacing(6);
    tools_drill_group_drill_label = new FCLabel(nullptr);
    tools_drill_group_drill_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_drill_group_drill_label->setToolTip(QString::fromUtf8("Cree CNCJob con trayectorias de herramientas para taladrar o fresar agujeros."));
    tools_drill_group_layout->addWidget(tools_drill_group_drill_label, 0);
    auto* lay119 = new QGridLayout();
    lay119->setContentsMargins(0, 0, 0, 0);
    lay119->setHorizontalSpacing(6);
    lay119->setVerticalSpacing(6);
    tools_drill_group_order_label = new FCLabel(nullptr);
    tools_drill_group_order_label->setText(QString::fromUtf8("Orden de la Herram:"));
    tools_drill_group_order_label->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    lay119->addWidget(tools_drill_group_order_label, 1, 0, 1, 1);
    tools_drill_group_order_radio = new RadioSet({{QString::fromUtf8("No"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Adelante"), QVariant(QString::fromUtf8("fwd"))}, {QString::fromUtf8("Atras"), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_order_radio->setValue(QVariant(QString::fromUtf8("no")));
    lay119->addWidget(tools_drill_group_order_radio, 1, 1, 1, 2);
    auto* w120 = new FCLabel(nullptr);
    w120->setText(QString::fromUtf8("Corte Z:"));
    w120->setToolTip(QString::fromUtf8("Profundidad de perforación (negativo)\ndebajo de la superficie de cobre."));
    lay119->addWidget(w120, 3, 0, 1, 1);
    tools_drill_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_cutz_entry->setProperty("decimals", 4);
    tools_drill_group_cutz_entry->setProperty("maximum", 0.0);
    tools_drill_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_drill_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_drill_group_cutz_entry->setProperty("value", -1.7);
    lay119->addWidget(tools_drill_group_cutz_entry, 3, 1, 1, 2);
    tools_drill_group_mpass_cb = new FCCheckBox(nullptr);
    tools_drill_group_mpass_cb->setText(QString::fromUtf8("Profund. Múlti:"));
    tools_drill_group_mpass_cb->setToolTip(QString::fromUtf8("Usa múltiples pases para limitar\nLa profundidad de corte en cada pasada. Será\ncortar varias veces hasta que el Corte Z sea\nalcanzado."));
    lay119->addWidget(tools_drill_group_mpass_cb, 4, 0, 1, 1);
    tools_drill_group_maxdepth_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_maxdepth_entry->setProperty("decimals", 4);
    tools_drill_group_maxdepth_entry->setProperty("maximum", 10000.0);
    tools_drill_group_maxdepth_entry->setProperty("singleStep", 0.1);
    tools_drill_group_maxdepth_entry->setToolTip(QString::fromUtf8("Profundidad de cada pase (positivo)."));
    tools_drill_group_maxdepth_entry->setProperty("value", 0.7);
    lay119->addWidget(tools_drill_group_maxdepth_entry, 4, 1, 1, 2);
    auto* w121 = new FCLabel(nullptr);
    w121->setText(QString::fromUtf8("Viaje Z:"));
    w121->setToolTip(QString::fromUtf8("Altura de herramienta al viajar\nA través del plano XY."));
    lay119->addWidget(w121, 5, 0, 1, 1);
    tools_drill_group_travelz_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_travelz_entry->setProperty("decimals", 4);
    tools_drill_group_travelz_entry->setProperty("maximum", 10000.0);
    tools_drill_group_travelz_entry->setProperty("minimum", 0.0001);
    tools_drill_group_travelz_entry->setProperty("value", 2.0);
    lay119->addWidget(tools_drill_group_travelz_entry, 5, 1, 1, 2);
    tools_drill_group_toolchange_cb = new FCCheckBox(nullptr);
    tools_drill_group_toolchange_cb->setText(QString::fromUtf8("Cambio de herram"));
    tools_drill_group_toolchange_cb->setToolTip(QString::fromUtf8("Incluir secuencia de cambio de herramienta\nen G-Code (Pausa para cambio de herramienta)."));
    lay119->addWidget(tools_drill_group_toolchange_cb, 6, 0, 1, 3);
    auto* w122 = new FCLabel(nullptr);
    w122->setText(QString::fromUtf8("Cambio de herramienta Z:"));
    w122->setToolTip(QString::fromUtf8("Posición del eje Z (altura) para\ncambio de herramienta."));
    lay119->addWidget(w122, 7, 0, 1, 1);
    tools_drill_group_toolchangez_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_toolchangez_entry->setProperty("decimals", 4);
    tools_drill_group_toolchangez_entry->setProperty("maximum", 10000.0);
    tools_drill_group_toolchangez_entry->setProperty("minimum", 0.0001);
    tools_drill_group_toolchangez_entry->setProperty("value", 15.0);
    lay119->addWidget(tools_drill_group_toolchangez_entry, 7, 1, 1, 2);
    auto* w123 = new FCLabel(nullptr);
    w123->setText(QString::fromUtf8("Fin del movi. Z:"));
    w123->setToolTip(QString::fromUtf8("Altura de la herramienta después de\nEl último movimiento al final del trabajo."));
    lay119->addWidget(w123, 8, 0, 1, 1);
    tools_drill_group_endz_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_endz_entry->setProperty("decimals", 4);
    tools_drill_group_endz_entry->setProperty("maximum", 10000.0);
    tools_drill_group_endz_entry->setProperty("value", 0.5);
    lay119->addWidget(tools_drill_group_endz_entry, 8, 1, 1, 2);
    auto* w124 = new FCLabel(nullptr);
    w124->setText(QString::fromUtf8("X, Y Fin del movimiento:"));
    w124->setToolTip(QString::fromUtf8("Fin movimiento X, Y posición. En formato (x, y).\nSi no se ingresa ningún valor, entonces no hay movimiento\nen el plano X, Y al final del trabajo."));
    lay119->addWidget(w124, 9, 0, 1, 1);
    tools_drill_group_endxy_entry = new NumericalEvalTupleEntry(nullptr);
    tools_drill_group_endxy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_drill_group_endxy_entry->setText(QString::fromUtf8("None"));
    lay119->addWidget(tools_drill_group_endxy_entry, 9, 1, 1, 2);
    auto* w125 = new FCLabel(nullptr);
    w125->setText(QString::fromUtf8("Avance Z:"));
    w125->setToolTip(QString::fromUtf8("Velocidad de herramienta durante la perforación\n(en unidades por minuto).\nLa llamada velocidad de avance 'Plunge'.\nEsto es para el movimiento lineal G01."));
    lay119->addWidget(w125, 10, 0, 1, 1);
    tools_drill_group_feedrate_z_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_feedrate_z_entry->setProperty("decimals", 4);
    tools_drill_group_feedrate_z_entry->setProperty("maximum", 910000.0);
    tools_drill_group_feedrate_z_entry->setProperty("value", 300.0);
    lay119->addWidget(tools_drill_group_feedrate_z_entry, 10, 1, 1, 2);
    auto* w126 = new FCLabel(nullptr);
    w126->setText(QString::fromUtf8("Eje de velocidad:"));
    w126->setToolTip(QString::fromUtf8("Velocidad del husillo\nen RPM (opcional)"));
    lay119->addWidget(w126, 11, 0, 1, 1);
    tools_drill_group_spindlespeed_entry = new FCSpinner(nullptr);
    tools_drill_group_spindlespeed_entry->setProperty("maximum", 1000000);
    tools_drill_group_spindlespeed_entry->setProperty("singleStep", 100);
    lay119->addWidget(tools_drill_group_spindlespeed_entry, 11, 1, 1, 2);
    tools_drill_group_dwell_cb = new FCCheckBox(nullptr);
    tools_drill_group_dwell_cb->setText(QString::fromUtf8("Habilitar Permanencia"));
    tools_drill_group_dwell_cb->setToolTip(QString::fromUtf8("Pausa para permitir que el husillo alcance su\nVelocidad antes del corte."));
    lay119->addWidget(tools_drill_group_dwell_cb, 12, 0, 1, 3);
    auto* w127 = new FCLabel(nullptr);
    w127->setText(QString::fromUtf8("Duración:"));
    w127->setToolTip(QString::fromUtf8("Número de unidades de tiempo para que el husillo permanezca."));
    lay119->addWidget(w127, 13, 0, 1, 1);
    tools_drill_group_dwelltime_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_dwelltime_entry->setProperty("decimals", 4);
    tools_drill_group_dwelltime_entry->setEnabled(false);
    tools_drill_group_dwelltime_entry->setProperty("maximum", 910000.0);
    tools_drill_group_dwelltime_entry->setProperty("value", 1.0);
    lay119->addWidget(tools_drill_group_dwelltime_entry, 13, 1, 1, 2);
    auto* w128 = new FCLabel(nullptr);
    w128->setText(QString::fromUtf8("Postprocesador:"));
    w128->setToolTip(QString::fromUtf8("El archivo JSON del postprocesador que dicta\nSalida de Gcode."));
    lay119->addWidget(w128, 14, 0, 1, 1);
    tools_drill_group_pp_excellon_name_cb = new FCComboBox(nullptr);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); tools_drill_group_pp_excellon_name_cb->setSizePolicy(sp); }
    lay119->addWidget(tools_drill_group_pp_excellon_name_cb, 14, 1, 1, 2);
    auto* w129 = new QFrame(nullptr);
    w129->setProperty("frameShadow", 48);
    w129->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w129->setSizePolicy(sp); }
    lay119->addWidget(w129, 16, 0, 1, 3);
    tools_drill_group_dslots_label = new FCLabel(nullptr);
    tools_drill_group_dslots_label->setText(QString::fromUtf8("<b>Perforando las ranuras:</b>"));
    lay119->addWidget(tools_drill_group_dslots_label, 18, 0, 1, 3);
    tools_drill_group_drill_slots_cb = new FCCheckBox(nullptr);
    tools_drill_group_drill_slots_cb->setText(QString::fromUtf8("Perforar las Ranuras"));
    tools_drill_group_drill_slots_cb->setToolTip(QString::fromUtf8("Si la herramienta seleccionada tiene ranuras, se perforarán."));
    lay119->addWidget(tools_drill_group_drill_slots_cb, 20, 0, 1, 3);
    tools_drill_group_drill_overlap_label = new FCLabel(nullptr);
    tools_drill_group_drill_overlap_label->setText(QString::fromUtf8("Superposición:"));
    tools_drill_group_drill_overlap_label->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del diámetro de la herramienta debe superponerse al taladro anterior."));
    lay119->addWidget(tools_drill_group_drill_overlap_label, 22, 0, 1, 1);
    tools_drill_group_drill_overlap_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_drill_overlap_entry->setProperty("decimals", 4);
    tools_drill_group_drill_overlap_entry->setProperty("maximum", 10000.0);
    tools_drill_group_drill_overlap_entry->setProperty("singleStep", 0.1);
    lay119->addWidget(tools_drill_group_drill_overlap_entry, 22, 1, 1, 2);
    tools_drill_group_last_drill_cb = new FCCheckBox(nullptr);
    tools_drill_group_last_drill_cb->setChecked(true);
    tools_drill_group_last_drill_cb->setText(QString::fromUtf8("Último ejercicio"));
    tools_drill_group_last_drill_cb->setToolTip(QString::fromUtf8("Si la longitud de la ranura no está completamente cubierta por taladros,\nagregue un taladro en el punto final de la ranura."));
    lay119->addWidget(tools_drill_group_last_drill_cb, 24, 0, 1, 3);
    auto* w130 = new QFrame(nullptr);
    w130->setProperty("frameShadow", 48);
    w130->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w130->setSizePolicy(sp); }
    lay119->addWidget(w130, 26, 0, 1, 3);
    tools_drill_group_exc_label = new FCLabel(nullptr);
    tools_drill_group_exc_label->setText(QString::fromUtf8("<b>Opciones avanzadas:</b>"));
    tools_drill_group_exc_label->setToolTip(QString::fromUtf8("Una lista de parámetros avanzados."));
    lay119->addWidget(tools_drill_group_exc_label, 28, 0, 1, 3);
    auto* w131 = new FCLabel(nullptr);
    w131->setText(QString::fromUtf8("Offset Z:"));
    w131->setToolTip(QString::fromUtf8("Algunas brocas (las más grandes) necesitan profundizar más\npara crear el diámetro del orificio de salida deseado debido a la forma de la punta.\nEl valor aquí puede compensar el parámetro Z de corte."));
    lay119->addWidget(w131, 29, 0, 1, 1);
    tools_drill_group_offset_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_offset_entry->setProperty("decimals", 4);
    tools_drill_group_offset_entry->setProperty("maximum", 999.9999);
    tools_drill_group_offset_entry->setProperty("minimum", -999.9999);
    lay119->addWidget(tools_drill_group_offset_entry, 29, 1, 1, 2);
    auto* w132 = new FCLabel(nullptr);
    w132->setText(QString::fromUtf8("Cambio de herra X, Y:"));
    w132->setToolTip(QString::fromUtf8("Cambio de herra X, posición Y."));
    lay119->addWidget(w132, 31, 0, 1, 1);
    tools_drill_group_toolchangexy_entry = new NumericalEvalTupleEntry(nullptr);
    tools_drill_group_toolchangexy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_drill_group_toolchangexy_entry->setText(QString::fromUtf8("0.0, 0.0"));
    lay119->addWidget(tools_drill_group_toolchangexy_entry, 31, 1, 1, 2);
    auto* w133 = new FCLabel(nullptr);
    w133->setText(QString::fromUtf8("Comience Z:"));
    w133->setToolTip(QString::fromUtf8("Altura de la herramienta justo después de comenzar el trabajo.\nElimine el valor si no necesita esta característica."));
    lay119->addWidget(w133, 33, 0, 1, 1);
    tools_drill_group_estartz_entry = new NumericalEvalEntry(nullptr);
    tools_drill_group_estartz_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay119->addWidget(tools_drill_group_estartz_entry, 33, 1, 1, 2);
    auto* w134 = new FCLabel(nullptr);
    w134->setText(QString::fromUtf8("Rápidos de avance:"));
    w134->setToolTip(QString::fromUtf8("Velocidad de la herramienta durante la perforación\n(en unidades por minuto).\nEsto es para el movimiento rápido G00.\nEs útil solo para Marlin,\nIgnorar para cualquier otro caso."));
    lay119->addWidget(w134, 35, 0, 1, 1);
    tools_drill_group_feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_feedrate_rapid_entry->setProperty("decimals", 4);
    tools_drill_group_feedrate_rapid_entry->setProperty("maximum", 910000.0);
    tools_drill_group_feedrate_rapid_entry->setProperty("value", 1500.0);
    lay119->addWidget(tools_drill_group_feedrate_rapid_entry, 35, 1, 1, 2);
    tools_drill_group_pdepth_label = new FCLabel(nullptr);
    tools_drill_group_pdepth_label->setText(QString::fromUtf8("Profundidad de la sonda Z:"));
    tools_drill_group_pdepth_label->setToolTip(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units."));
    lay119->addWidget(tools_drill_group_pdepth_label, 37, 0, 1, 1);
    tools_drill_group_pdepth_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_pdepth_entry->setProperty("decimals", 4);
    tools_drill_group_pdepth_entry->setProperty("maximum", 0.0);
    tools_drill_group_pdepth_entry->setProperty("minimum", -910000.0);
    tools_drill_group_pdepth_entry->setProperty("value", -0.02);
    lay119->addWidget(tools_drill_group_pdepth_entry, 37, 1, 1, 2);
    tools_drill_group_feedrate_probe_label = new FCLabel(nullptr);
    tools_drill_group_feedrate_probe_label->setText(QString::fromUtf8("Sonda de avance:"));
    tools_drill_group_feedrate_probe_label->setToolTip(QString::fromUtf8("La velocidad de avance utilizada mientras la sonda está sondeando."));
    lay119->addWidget(tools_drill_group_feedrate_probe_label, 38, 0, 1, 1);
    tools_drill_group_feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_feedrate_probe_entry->setProperty("decimals", 4);
    tools_drill_group_feedrate_probe_entry->setProperty("maximum", 910000.0);
    tools_drill_group_feedrate_probe_entry->setProperty("value", 75.0);
    lay119->addWidget(tools_drill_group_feedrate_probe_entry, 38, 1, 1, 2);
    auto* w135 = new FCLabel(nullptr);
    w135->setText(QString::fromUtf8("Dirección del motor:"));
    w135->setToolTip(QString::fromUtf8("Esto establece la dirección en que gira el husillo.\nPuede ser:\n- CW = en el sentido de las agujas del reloj o\n- CCW = a la izquierda"));
    lay119->addWidget(w135, 40, 0, 1, 1);
    tools_drill_group_spindledir_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_spindledir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay119->addWidget(tools_drill_group_spindledir_radio, 40, 1, 1, 2);
    tools_drill_group_fplunge_cb = new FCCheckBox(nullptr);
    tools_drill_group_fplunge_cb->setText(QString::fromUtf8("Salto rápido"));
    tools_drill_group_fplunge_cb->setToolTip(QString::fromUtf8("Al comprobar esto, el movimiento vertical de\nZ_Toolchange a Z_move se hace con G0,\nes decir, la velocidad más rápida disponible.\nADVERTENCIA: el movimiento se realiza en Toolchange X, Y coords."));
    lay119->addWidget(tools_drill_group_fplunge_cb, 42, 0, 1, 3);
    tools_drill_group_fretract_cb = new FCCheckBox(nullptr);
    tools_drill_group_fretract_cb->setText(QString::fromUtf8("Retracción rápida"));
    tools_drill_group_fretract_cb->setToolTip(QString::fromUtf8("Estrategia de salida del agujero.\n  - Cuando no esté enganchado, al salir del orificio perforado, la broca\nviajará lento, con velocidad de avance establecida (G1), hasta una profundidad de cero y luego\nviaje lo más rápido posible (G0) al Z Move (altura de desplazamiento).\n  - Cuando se verifica el recorrido desde Z corte (profundidad de corte) a Z_move\n(altura de recorrido) se realiza lo más rápido posible (G0) en un movimiento."));
    lay119->addWidget(tools_drill_group_fretract_cb, 45, 0, 1, 3);
    auto* w136 = new QFrame(nullptr);
    w136->setProperty("frameShadow", 48);
    w136->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w136->setSizePolicy(sp); }
    lay119->addWidget(w136, 46, 0, 1, 3);
    tools_drill_group_area_exc_label = new FCLabel(nullptr);
    tools_drill_group_area_exc_label->setText(QString::fromUtf8("<b>Exclusión de áreaSelección de área:</b>"));
    tools_drill_group_area_exc_label->setToolTip(QString::fromUtf8("Parámetros de exclusión de áreas."));
    lay119->addWidget(tools_drill_group_area_exc_label, 47, 0, 1, 2);
    tools_drill_group_exclusion_cb = new FCCheckBox(nullptr);
    tools_drill_group_exclusion_cb->setText(QString::fromUtf8("Zonas de exclusión"));
    tools_drill_group_exclusion_cb->setToolTip(QString::fromUtf8("Incluir áreas de exclusión.\nEn esas áreas el recorrido de las herramientas.\nestá prohibido."));
    lay119->addWidget(tools_drill_group_exclusion_cb, 49, 0, 1, 2);
    tools_drill_group_area_shape_label = new FCLabel(nullptr);
    tools_drill_group_area_shape_label->setText(QString::fromUtf8("Forma:"));
    tools_drill_group_area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    lay119->addWidget(tools_drill_group_area_shape_label, 51, 0, 1, 1);
    tools_drill_group_area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("polygon")));
    lay119->addWidget(tools_drill_group_area_shape_radio, 51, 1, 1, 1);
    tools_drill_group_strategy_label = new FCLabel(nullptr);
    tools_drill_group_strategy_label->setText(QString::fromUtf8("Estrategia:"));
    tools_drill_group_strategy_label->setToolTip(QString::fromUtf8("La estrategia seguida al encontrar un área de exclusión.\nPuede ser:\n- Sobre -> al encontrar el área, la herramienta irá a una altura establecida\n- Alrededor -> evitará el área de exclusión recorriendo el área"));
    lay119->addWidget(tools_drill_group_strategy_label, 53, 0, 1, 1);
    tools_drill_group_strategy_radio = new RadioSet({{QString::fromUtf8("Sobre"), QVariant(QString::fromUtf8("over"))}, {QString::fromUtf8("AlrededorRedondo"), QVariant(QString::fromUtf8("around"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_strategy_radio->setValue(QVariant(QString::fromUtf8("over")));
    lay119->addWidget(tools_drill_group_strategy_radio, 53, 1, 1, 1);
    tools_drill_group_over_z_label = new FCLabel(nullptr);
    tools_drill_group_over_z_label->setText(QString::fromUtf8("Sobre ZSuperposición:"));
    tools_drill_group_over_z_label->setToolTip(QString::fromUtf8("La altura Z a la que se elevará la herramienta para evitar\nUn área de interdicción."));
    lay119->addWidget(tools_drill_group_over_z_label, 55, 0, 1, 1);
    tools_drill_group_over_z_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_over_z_entry->setProperty("decimals", 4);
    tools_drill_group_over_z_entry->setProperty("maximum", 10000.0);
    tools_drill_group_over_z_entry->setProperty("value", 1.0);
    lay119->addWidget(tools_drill_group_over_z_entry, 55, 1, 1, 1);
    tools_drill_group_layout->addLayout(lay119, 0);
    tools_drill_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_drill_group->setLayout(tools_drill_group_layout);
    vlay1->addWidget(tools_drill_group, 0);
    tools_panelize_group = new QGroupBox(nullptr);
    { QFont f = tools_panelize_group->font(); f.setBold(true); f.setPixelSize(16); tools_panelize_group->setFont(f); }
    tools_panelize_group->setMinimumSize(QSize(220, 0));
    tools_panelize_group_layout = new QVBoxLayout();
    tools_panelize_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_panelize_group_layout->setSpacing(6);
    tools_panelize_group_panelize_label = new QLabel(nullptr);
    tools_panelize_group_panelize_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_panelize_group_panelize_label->setToolTip(QString::fromUtf8("Cree un objeto que contenga una matriz de (x, y) elementos,\nCada elemento es una copia del objeto fuente espaciado.\na una distancia X, distancia Y entre sí."));
    tools_panelize_group_layout->addWidget(tools_panelize_group_panelize_label, 0);
    auto* lay137 = new QGridLayout();
    lay137->setContentsMargins(0, 0, 0, 0);
    lay137->setHorizontalSpacing(6);
    lay137->setVerticalSpacing(6);
    tools_panelize_group_spacing_columns_label = new QLabel(nullptr);
    tools_panelize_group_spacing_columns_label->setText(QString::fromUtf8("Col. de espaciado:"));
    tools_panelize_group_spacing_columns_label->setToolTip(QString::fromUtf8("Espaciado entre columnas del panel deseado.\nEn unidades actuales."));
    lay137->addWidget(tools_panelize_group_spacing_columns_label, 0, 0, 1, 1);
    tools_panelize_group_pspacing_columns = new FCDoubleSpinner(nullptr);
    tools_panelize_group_pspacing_columns->setProperty("decimals", 4);
    tools_panelize_group_pspacing_columns->setProperty("maximum", 10000.0);
    tools_panelize_group_pspacing_columns->setProperty("singleStep", 0.1);
    lay137->addWidget(tools_panelize_group_pspacing_columns, 0, 1, 1, 1);
    tools_panelize_group_spacing_rows_label = new QLabel(nullptr);
    tools_panelize_group_spacing_rows_label->setText(QString::fromUtf8("Separación de filas:"));
    tools_panelize_group_spacing_rows_label->setToolTip(QString::fromUtf8("Espaciado entre filas del panel deseado.\nEn unidades actuales."));
    lay137->addWidget(tools_panelize_group_spacing_rows_label, 1, 0, 1, 1);
    tools_panelize_group_pspacing_rows = new FCDoubleSpinner(nullptr);
    tools_panelize_group_pspacing_rows->setProperty("decimals", 4);
    tools_panelize_group_pspacing_rows->setProperty("maximum", 10000.0);
    tools_panelize_group_pspacing_rows->setProperty("singleStep", 0.1);
    lay137->addWidget(tools_panelize_group_pspacing_rows, 1, 1, 1, 1);
    tools_panelize_group_columns_label = new QLabel(nullptr);
    tools_panelize_group_columns_label->setText(QString::fromUtf8("Columnas:"));
    tools_panelize_group_columns_label->setToolTip(QString::fromUtf8("Número de columnas del panel deseado"));
    lay137->addWidget(tools_panelize_group_columns_label, 2, 0, 1, 1);
    tools_panelize_group_pcolumns = new FCSpinner(nullptr);
    tools_panelize_group_pcolumns->setProperty("maximum", 1000);
    tools_panelize_group_pcolumns->setProperty("minimum", 1);
    tools_panelize_group_pcolumns->setProperty("value", 1);
    lay137->addWidget(tools_panelize_group_pcolumns, 2, 1, 1, 1);
    tools_panelize_group_rows_label = new QLabel(nullptr);
    tools_panelize_group_rows_label->setText(QString::fromUtf8("Filas:"));
    tools_panelize_group_rows_label->setToolTip(QString::fromUtf8("Número de filas del panel deseado"));
    lay137->addWidget(tools_panelize_group_rows_label, 3, 0, 1, 1);
    tools_panelize_group_prows = new FCSpinner(nullptr);
    tools_panelize_group_prows->setProperty("maximum", 1000);
    tools_panelize_group_prows->setProperty("minimum", 1);
    tools_panelize_group_prows->setProperty("value", 1);
    lay137->addWidget(tools_panelize_group_prows, 3, 1, 1, 1);
    tools_panelize_group_panel_type_label = new QLabel(nullptr);
    tools_panelize_group_panel_type_label->setText(QString::fromUtf8("Tipo de panel:"));
    tools_panelize_group_panel_type_label->setToolTip(QString::fromUtf8("Elija el tipo de objeto para el objeto del panel:\n- Gerber\n- Geometría"));
    lay137->addWidget(tools_panelize_group_panel_type_label, 4, 0, 1, 1);
    tools_panelize_group_panel_type_radio = new RadioSet({{QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("gerber"))}, {QString::fromUtf8("Geo"), QVariant(QString::fromUtf8("geometry"))}}, Qt::Horizontal, true, nullptr);
    tools_panelize_group_panel_type_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    lay137->addWidget(tools_panelize_group_panel_type_radio, 4, 1, 1, 1);
    tools_panelize_group_poptimization_cb = new FCCheckBox(nullptr);
    tools_panelize_group_poptimization_cb->setChecked(true);
    tools_panelize_group_poptimization_cb->setText(QString::fromUtf8("Optimización de ruta"));
    tools_panelize_group_poptimization_cb->setToolTip(QString::fromUtf8("Activo solo para el tipo de panel Geometría.\nCuando se marca, la aplicación encontrará\ncualesquiera dos elementos de línea superpuestos en el panel\ny eliminará las partes superpuestas, conservando solo una de ellas."));
    lay137->addWidget(tools_panelize_group_poptimization_cb, 5, 0, 1, 2);
    tools_panelize_group_pconstrain_cb = new FCCheckBox(nullptr);
    tools_panelize_group_pconstrain_cb->setText(QString::fromUtf8("Restringir dentro de:"));
    tools_panelize_group_pconstrain_cb->setToolTip(QString::fromUtf8("Área definida por DX y DY dentro para restringir el panel.\nLos valores DX y DY están en unidades actuales.\nIndependientemente de cuántas columnas y filas se deseen,\nEl panel final tendrá tantas columnas y filas como\nencajan completamente dentro del área seleccionada."));
    lay137->addWidget(tools_panelize_group_pconstrain_cb, 10, 0, 1, 2);
    tools_panelize_group_x_width_lbl = new QLabel(nullptr);
    tools_panelize_group_x_width_lbl->setText(QString::fromUtf8("Ancho (DX):"));
    tools_panelize_group_x_width_lbl->setToolTip(QString::fromUtf8("El ancho (DX) dentro del cual debe caber el panel.\nEn unidades actuales."));
    lay137->addWidget(tools_panelize_group_x_width_lbl, 12, 0, 1, 1);
    tools_panelize_group_px_width_entry = new FCDoubleSpinner(nullptr);
    tools_panelize_group_px_width_entry->setProperty("decimals", 4);
    tools_panelize_group_px_width_entry->setProperty("maximum", 10000.0);
    tools_panelize_group_px_width_entry->setProperty("singleStep", 0.1);
    tools_panelize_group_px_width_entry->setProperty("value", 200.0);
    lay137->addWidget(tools_panelize_group_px_width_entry, 12, 1, 1, 1);
    tools_panelize_group_y_height_lbl = new QLabel(nullptr);
    tools_panelize_group_y_height_lbl->setText(QString::fromUtf8("Altura (DY):"));
    tools_panelize_group_y_height_lbl->setToolTip(QString::fromUtf8("La altura (DY) dentro de la cual debe caber el panel.\nEn unidades actuales."));
    lay137->addWidget(tools_panelize_group_y_height_lbl, 17, 0, 1, 1);
    tools_panelize_group_py_height_entry = new FCDoubleSpinner(nullptr);
    tools_panelize_group_py_height_entry->setProperty("decimals", 4);
    tools_panelize_group_py_height_entry->setProperty("maximum", 10000.0);
    tools_panelize_group_py_height_entry->setProperty("singleStep", 0.1);
    tools_panelize_group_py_height_entry->setProperty("value", 290.0);
    lay137->addWidget(tools_panelize_group_py_height_entry, 17, 1, 1, 1);
    lay137->setColumnStretch(1, 1);
    tools_panelize_group_layout->addLayout(lay137, 0);
    tools_panelize_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_panelize_group->setLayout(tools_panelize_group_layout);
    vlay1->addWidget(tools_panelize_group, 0);
    layout->addLayout(vlay1, 0);
    vlay2 = new QVBoxLayout();
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->setSpacing(6);
    tools_ncc_group = new QGroupBox(nullptr);
    { QFont f = tools_ncc_group->font(); f.setBold(true); f.setPixelSize(16); tools_ncc_group->setFont(f); }
    tools_ncc_group->setMinimumSize(QSize(220, 0));
    tools_ncc_group_layout = new QVBoxLayout();
    tools_ncc_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_ncc_group_layout->setSpacing(6);
    tools_ncc_group_clearcopper_label = new QLabel(nullptr);
    tools_ncc_group_clearcopper_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_ncc_group_clearcopper_label->setToolTip(QString::fromUtf8("Crear un objeto de geometría con\nTrayectorias para cortar todas las regiones sin cobre."));
    tools_ncc_group_layout->addWidget(tools_ncc_group_clearcopper_label, 0);
    auto* lay138 = new QGridLayout();
    lay138->setContentsMargins(0, 0, 0, 0);
    lay138->setHorizontalSpacing(6);
    lay138->setVerticalSpacing(6);
    auto* w139 = new QLabel(nullptr);
    w139->setText(QString::fromUtf8("<b><font color=\"green\">Diá. de Herram:</font></b>"));
    w139->setToolTip(QString::fromUtf8("Diámetros de las herramientas, separados por comas.\nEl valor del diámetro tiene que usar el separador de decimales de punto.\nValores válidos: 0.3, 1.0"));
    lay138->addWidget(w139, 0, 0, 1, 1);
    tools_ncc_group_ncc_tool_dia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_ncc_group_ncc_tool_dia_entry->setProperty("placeholderText", QString::fromUtf8("Valores Separados por Comas"));
    tools_ncc_group_ncc_tool_dia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_ncc_group_ncc_tool_dia_entry->setText(QString::fromUtf8("1.0, 0.5"));
    lay138->addWidget(tools_ncc_group_ncc_tool_dia_entry, 0, 1, 1, 1);
    tools_ncc_group_tool_type_label = new QLabel(nullptr);
    tools_ncc_group_tool_type_label->setText(QString::fromUtf8("Tipo de herram:"));
    tools_ncc_group_tool_type_label->setToolTip(QString::fromUtf8("Tipo de herramienta predeterminada:\n- 'Forma V'\n- circular"));
    lay138->addWidget(tools_ncc_group_tool_type_label, 1, 0, 1, 1);
    tools_ncc_group_tool_type_radio = new RadioSet({{QString::fromUtf8("Forma V"), QVariant(QString::fromUtf8("V"))}, {QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("C1"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_tool_type_radio->setToolTip(QString::fromUtf8("Tipo de herramienta predeterminada:\n- 'Forma V'\n- circular"));
    tools_ncc_group_tool_type_radio->setValue(QVariant(QString::fromUtf8("C1")));
    lay138->addWidget(tools_ncc_group_tool_type_radio, 1, 1, 1, 1);
    tools_ncc_group_tipdialabel = new QLabel(nullptr);
    tools_ncc_group_tipdialabel->setText(QString::fromUtf8("V-Tipo Dia:"));
    tools_ncc_group_tipdialabel->setToolTip(QString::fromUtf8("El diámetro de la punta para la herramienta en forma de V"));
    lay138->addWidget(tools_ncc_group_tipdialabel, 2, 0, 1, 1);
    tools_ncc_group_tipdia_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_tipdia_entry->setProperty("decimals", 4);
    tools_ncc_group_tipdia_entry->setProperty("maximum", 1000.0);
    tools_ncc_group_tipdia_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_tipdia_entry->setProperty("value", 0.1);
    lay138->addWidget(tools_ncc_group_tipdia_entry, 2, 1, 1, 1);
    tools_ncc_group_tipanglelabel = new QLabel(nullptr);
    tools_ncc_group_tipanglelabel->setText(QString::fromUtf8("V-Tipo Ángulo:"));
    tools_ncc_group_tipanglelabel->setToolTip(QString::fromUtf8("El ángulo de punta para la herramienta en forma de V.\nEn grado."));
    lay138->addWidget(tools_ncc_group_tipanglelabel, 3, 0, 1, 1);
    tools_ncc_group_tipangle_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_tipangle_entry->setProperty("decimals", 4);
    tools_ncc_group_tipangle_entry->setProperty("maximum", 180.0);
    tools_ncc_group_tipangle_entry->setProperty("minimum", 1.0);
    tools_ncc_group_tipangle_entry->setProperty("singleStep", 5.0);
    tools_ncc_group_tipangle_entry->setProperty("value", 30.0);
    tools_ncc_group_tipangle_entry->setWrapping(true);
    lay138->addWidget(tools_ncc_group_tipangle_entry, 3, 1, 1, 1);
    auto* w140 = new QLabel(nullptr);
    w140->setText(QString::fromUtf8("Corte Z:"));
    w140->setToolTip(QString::fromUtf8("Profundidad de corte en el material. Valor negativo.\nEn unidades de aplicación."));
    lay138->addWidget(w140, 4, 0, 1, 1);
    tools_ncc_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_cutz_entry->setProperty("decimals", 4);
    tools_ncc_group_cutz_entry->setProperty("maximum", 0.0);
    tools_ncc_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_ncc_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_cutz_entry->setToolTip(QString::fromUtf8("Profundidad de corte en el material. Valor negativo.\nEn unidades de aplicación."));
    tools_ncc_group_cutz_entry->setProperty("value", -0.05);
    lay138->addWidget(tools_ncc_group_cutz_entry, 4, 1, 1, 1);
    tools_ncc_group_newdialabel = new QLabel(nullptr);
    tools_ncc_group_newdialabel->setText(QString::fromUtf8("Nuevo dia:"));
    tools_ncc_group_newdialabel->setToolTip(QString::fromUtf8("Diámetro de la nueva herramienta para agregar en la Tabla de herramientas.\nSi la herramienta es de tipo V, este valor es automáticamente\ncalculado a partir de los otros parámetros."));
    lay138->addWidget(tools_ncc_group_newdialabel, 5, 0, 1, 1);
    tools_ncc_group_newdia_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_newdia_entry->setProperty("decimals", 4);
    tools_ncc_group_newdia_entry->setProperty("maximum", 10000.0);
    tools_ncc_group_newdia_entry->setProperty("minimum", 0.0001);
    tools_ncc_group_newdia_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_newdia_entry->setProperty("value", 0.1);
    lay138->addWidget(tools_ncc_group_newdia_entry, 5, 1, 1, 1);
    auto* w141 = new QFrame(nullptr);
    w141->setProperty("frameShadow", 48);
    w141->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w141->setSizePolicy(sp); }
    lay138->addWidget(w141, 6, 0, 1, 2);
    tools_ncc_group_milling_type_label = new QLabel(nullptr);
    tools_ncc_group_milling_type_label->setText(QString::fromUtf8("Tipo de fresado:"));
    tools_ncc_group_milling_type_label->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    lay138->addWidget(tools_ncc_group_milling_type_label, 7, 0, 1, 1);
    tools_ncc_group_milling_type_radio = new RadioSet({{QString::fromUtf8("Subida"), QVariant(QString::fromUtf8("cl"))}, {QString::fromUtf8("Convencional"), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_milling_type_radio->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    tools_ncc_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    lay138->addWidget(tools_ncc_group_milling_type_radio, 7, 1, 1, 1);
    tools_ncc_group_ncc_order_label = new QLabel(nullptr);
    tools_ncc_group_ncc_order_label->setText(QString::fromUtf8("Orden de la Herram:"));
    tools_ncc_group_ncc_order_label->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    lay138->addWidget(tools_ncc_group_ncc_order_label, 8, 0, 1, 1);
    tools_ncc_group_ncc_order_radio = new RadioSet({{QString::fromUtf8("No"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Adelante"), QVariant(QString::fromUtf8("fwd"))}, {QString::fromUtf8("Atras"), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_ncc_order_radio->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    tools_ncc_group_ncc_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay138->addWidget(tools_ncc_group_ncc_order_radio, 8, 1, 1, 1);
    auto* w142 = new QFrame(nullptr);
    w142->setProperty("frameShadow", 48);
    w142->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w142->setSizePolicy(sp); }
    lay138->addWidget(w142, 9, 0, 1, 2);
    auto* w143 = new QLabel(nullptr);
    w143->setText(QString::fromUtf8("Superposición:"));
    w143->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta.\nAjustar el valor comenzando con valores más bajos\ny aumentarlo si las áreas que deben procesarse aún están\nno procesado.Valores más bajos = procesamiento más rápido, ejecución más rápida en CNC.\nValores más altos = procesamiento lento y ejecución lenta en CNC\ndebido a demasiados caminos."));
    lay138->addWidget(w143, 10, 0, 1, 1);
    tools_ncc_group_ncc_overlap_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_ncc_overlap_entry->setProperty("decimals", 4);
    tools_ncc_group_ncc_overlap_entry->setProperty("maximum", 99.9999);
    tools_ncc_group_ncc_overlap_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_ncc_overlap_entry->setProperty("suffix", QString::fromUtf8(" %"));
    tools_ncc_group_ncc_overlap_entry->setProperty("value", 40.0);
    tools_ncc_group_ncc_overlap_entry->setWrapping(true);
    lay138->addWidget(tools_ncc_group_ncc_overlap_entry, 10, 1, 1, 1);
    auto* w144 = new QLabel(nullptr);
    w144->setText(QString::fromUtf8("Margen:"));
    w144->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay138->addWidget(w144, 11, 0, 1, 1);
    tools_ncc_group_ncc_margin_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_ncc_margin_entry->setProperty("decimals", 4);
    tools_ncc_group_ncc_margin_entry->setProperty("maximum", 10000.0);
    tools_ncc_group_ncc_margin_entry->setProperty("minimum", -10000.0);
    tools_ncc_group_ncc_margin_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_ncc_margin_entry->setProperty("value", 1.0);
    lay138->addWidget(tools_ncc_group_ncc_margin_entry, 11, 1, 1, 1);
    auto* w145 = new QLabel(nullptr);
    w145->setText(QString::fromUtf8("Método:"));
    w145->setToolTip(QString::fromUtf8("Algoritmo para la limpieza de cobre:\n- Estándar: paso fijo hacia adentro.\n- Basado en semillas: hacia afuera de la semilla.\n- Basado en líneas: líneas paralelas."));
    lay138->addWidget(w145, 12, 0, 1, 1);
    tools_ncc_group_ncc_method_combo = new FCComboBox2(nullptr);
    tools_ncc_group_ncc_method_combo->setProperty("currentText", QString::fromUtf8("Semilla"));
    tools_ncc_group_ncc_method_combo->addItem(QString::fromUtf8("Estándar"));
    tools_ncc_group_ncc_method_combo->addItem(QString::fromUtf8("Semilla"));
    tools_ncc_group_ncc_method_combo->addItem(QString::fromUtf8("Líneas"));
    tools_ncc_group_ncc_method_combo->addItem(QString::fromUtf8("Combo"));
    tools_ncc_group_ncc_method_combo->setCurrentIndex(1);
    lay138->addWidget(tools_ncc_group_ncc_method_combo, 12, 1, 1, 1);
    tools_ncc_group_ncc_connect_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_connect_cb->setChecked(true);
    tools_ncc_group_ncc_connect_cb->setText(QString::fromUtf8("Conectar"));
    tools_ncc_group_ncc_connect_cb->setToolTip(QString::fromUtf8("Dibuja líneas entre el resultado\nSegmentos para minimizar elevaciones de herramientas."));
    lay138->addWidget(tools_ncc_group_ncc_connect_cb, 13, 0, 1, 1);
    tools_ncc_group_ncc_contour_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_contour_cb->setChecked(true);
    tools_ncc_group_ncc_contour_cb->setText(QString::fromUtf8("Contorno"));
    tools_ncc_group_ncc_contour_cb->setToolTip(QString::fromUtf8("Corta todo el perímetro del polígono.\nPara recortar los bordes ásperos."));
    lay138->addWidget(tools_ncc_group_ncc_contour_cb, 13, 1, 1, 1);
    tools_ncc_group_ncc_choice_offset_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_choice_offset_cb->setText(QString::fromUtf8("Compensar"));
    tools_ncc_group_ncc_choice_offset_cb->setToolTip(QString::fromUtf8("Si se usa, agregará una compensación a las características de cobre.\nEl claro de cobre terminará a distancia\nde las características de cobre."));
    lay138->addWidget(tools_ncc_group_ncc_choice_offset_cb, 14, 0, 1, 2);
    tools_ncc_group_ncc_offset_label = new QLabel(nullptr);
    tools_ncc_group_ncc_offset_label->setText(QString::fromUtf8("Valor de Comp:"));
    tools_ncc_group_ncc_offset_label->setToolTip(QString::fromUtf8("Si se usa, agregará una compensación a las características de cobre.\nEl claro de cobre terminará a distancia\nde las características de cobre."));
    lay138->addWidget(tools_ncc_group_ncc_offset_label, 15, 0, 1, 1);
    tools_ncc_group_ncc_offset_spinner = new FCDoubleSpinner(nullptr);
    tools_ncc_group_ncc_offset_spinner->setProperty("decimals", 4);
    tools_ncc_group_ncc_offset_spinner->setProperty("maximum", 10000.0);
    tools_ncc_group_ncc_offset_spinner->setProperty("singleStep", 0.1);
    tools_ncc_group_ncc_offset_spinner->setWrapping(true);
    lay138->addWidget(tools_ncc_group_ncc_offset_spinner, 15, 1, 1, 1);
    auto* w146 = new QFrame(nullptr);
    w146->setProperty("frameShadow", 48);
    w146->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w146->setSizePolicy(sp); }
    lay138->addWidget(w146, 16, 0, 1, 2);
    tools_ncc_group_ncc_rest_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_rest_cb->setText(QString::fromUtf8("Resto"));
    tools_ncc_group_ncc_rest_cb->setToolTip(QString::fromUtf8("Si está marcado, use 'mecanizado en resto'.\nBásicamente procesará cobre fuera de las características de la PCB,\nusando la herramienta más grande y continúe con las siguientes herramientas,\nde mayor a menor, para procesar las características de cobre que\nno pudo ser procesado por la herramienta anterior, hasta que haya\nno queda nada por procesar o no hay más herramientas.\n\nSi no está marcado, use el algoritmo estándar."));
    lay138->addWidget(tools_ncc_group_ncc_rest_cb, 17, 0, 1, 2);
    auto* w147 = new QLabel(nullptr);
    w147->setText(QString::fromUtf8("Selección:"));
    w147->setToolTip(QString::fromUtf8("Selección del área a procesar.\n- 'Sí mismo': la extensión del procesamiento se basa en el objeto que se procesa.\n- 'Selección de área': haga clic con el botón izquierdo del mouse para iniciar la selección del área a procesar.\n- 'Objeto de referencia': procesará el área especificada por otro objeto."));
    lay138->addWidget(w147, 18, 0, 1, 1);
    tools_ncc_group_select_combo = new FCComboBox2(nullptr);
    tools_ncc_group_select_combo->setProperty("currentText", QString::fromUtf8("Sí mismo"));
    tools_ncc_group_select_combo->addItem(QString::fromUtf8("Sí mismo"));
    tools_ncc_group_select_combo->addItem(QString::fromUtf8("Selección de área"));
    tools_ncc_group_select_combo->addItem(QString::fromUtf8("Objeto de referencia"));
    lay138->addWidget(tools_ncc_group_select_combo, 18, 1, 1, 1);
    tools_ncc_group_area_shape_label = new QLabel(nullptr);
    tools_ncc_group_area_shape_label->setText(QString::fromUtf8("Forma:"));
    tools_ncc_group_area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    lay138->addWidget(tools_ncc_group_area_shape_label, 19, 0, 1, 1);
    tools_ncc_group_area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay138->addWidget(tools_ncc_group_area_shape_radio, 19, 1, 1, 1);
    auto* w148 = new QFrame(nullptr);
    w148->setProperty("frameShadow", 48);
    w148->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w148->setSizePolicy(sp); }
    lay138->addWidget(w148, 20, 0, 1, 2);
    auto* w149 = new QLabel(nullptr);
    w149->setText(QString::fromUtf8("Trazado:"));
    w149->setToolTip(QString::fromUtf8("- 'Normal': trazado normal, realizado al final del trabajo\n- 'Progresivo': cada forma se traza después de generarse"));
    lay138->addWidget(w149, 21, 0, 1, 1);
    tools_ncc_group_plotting_radio = new RadioSet({{QString::fromUtf8("Normal"), QVariant(QString::fromUtf8("normal"))}, {QString::fromUtf8("Progresivo"), QVariant(QString::fromUtf8("progressive"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_plotting_radio->setValue(QVariant(QString::fromUtf8("normal")));
    lay138->addWidget(tools_ncc_group_plotting_radio, 21, 1, 1, 1);
    tools_ncc_group_valid_cb = new FCCheckBox(nullptr);
    tools_ncc_group_valid_cb->setChecked(true);
    tools_ncc_group_valid_cb->setProperty("objectName", QString::fromUtf8("n_check"));
    tools_ncc_group_valid_cb->setText(QString::fromUtf8("Verificar validez"));
    tools_ncc_group_valid_cb->setToolTip(QString::fromUtf8("Si se marca, se verifican los diámetros de las herramientas.\nsi proporcionarán un aislamiento completo."));
    lay138->addWidget(tools_ncc_group_valid_cb, 23, 0, 1, 2);
    tools_ncc_group_layout->addLayout(lay138, 0);
    tools_ncc_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_ncc_group->setLayout(tools_ncc_group_layout);
    vlay2->addWidget(tools_ncc_group, 0);
    tools_paint_group = new QGroupBox(nullptr);
    { QFont f = tools_paint_group->font(); f.setBold(true); f.setPixelSize(16); tools_paint_group->setFont(f); }
    tools_paint_group->setMinimumSize(QSize(220, 0));
    tools_paint_group_layout = new QVBoxLayout();
    tools_paint_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_paint_group_layout->setSpacing(6);
    tools_paint_group_paint_label = new QLabel(nullptr);
    tools_paint_group_paint_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_paint_group_paint_label->setToolTip(QString::fromUtf8("Cree trayectorias de herramientas para cubrir\ntoda el área de un polígono."));
    tools_paint_group_layout->addWidget(tools_paint_group_paint_label, 0);
    auto* lay150 = new QGridLayout();
    lay150->setContentsMargins(0, 0, 0, 0);
    lay150->setHorizontalSpacing(6);
    lay150->setVerticalSpacing(6);
    auto* w151 = new QLabel(nullptr);
    w151->setText(QString::fromUtf8("<b><font color=\"green\">Diá. de Herram:</font></b>"));
    w151->setToolTip(QString::fromUtf8("Diámetros de las herramientas, separados por comas.\nEl valor del diámetro tiene que usar el separador de decimales de punto.\nValores válidos: 0.3, 1.0"));
    lay150->addWidget(w151, 0, 0, 1, 1);
    tools_paint_group_painttooldia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_paint_group_painttooldia_entry->setProperty("placeholderText", QString::fromUtf8("Valores Separados por Comas"));
    tools_paint_group_painttooldia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_paint_group_painttooldia_entry->setText(QString::fromUtf8("0.3"));
    lay150->addWidget(tools_paint_group_painttooldia_entry, 0, 1, 1, 1);
    tools_paint_group_tool_type_label = new QLabel(nullptr);
    tools_paint_group_tool_type_label->setText(QString::fromUtf8("Tipo de herram:"));
    tools_paint_group_tool_type_label->setToolTip(QString::fromUtf8("Tipo de herramienta predeterminada:\n- 'Forma V'\n- circular"));
    lay150->addWidget(tools_paint_group_tool_type_label, 1, 0, 1, 1);
    tools_paint_group_tool_type_radio = new RadioSet({{QString::fromUtf8("Forma V"), QVariant(QString::fromUtf8("V"))}, {QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("C1"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_tool_type_radio->setProperty("objectName", QString::fromUtf8("Tipo de herram"));
    tools_paint_group_tool_type_radio->setValue(QVariant(QString::fromUtf8("C1")));
    lay150->addWidget(tools_paint_group_tool_type_radio, 1, 1, 1, 1);
    tools_paint_group_tipdialabel = new QLabel(nullptr);
    tools_paint_group_tipdialabel->setText(QString::fromUtf8("V-Tipo Dia:"));
    tools_paint_group_tipdialabel->setToolTip(QString::fromUtf8("El diámetro de la punta para la herramienta en forma de V"));
    lay150->addWidget(tools_paint_group_tipdialabel, 2, 0, 1, 1);
    tools_paint_group_tipdia_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_tipdia_entry->setProperty("decimals", 4);
    tools_paint_group_tipdia_entry->setProperty("maximum", 10000.0);
    tools_paint_group_tipdia_entry->setProperty("objectName", QString::fromUtf8("V-Tipo Dia"));
    tools_paint_group_tipdia_entry->setProperty("singleStep", 0.1);
    tools_paint_group_tipdia_entry->setProperty("value", 0.1);
    lay150->addWidget(tools_paint_group_tipdia_entry, 2, 1, 1, 1);
    tools_paint_group_tipanglelabel = new QLabel(nullptr);
    tools_paint_group_tipanglelabel->setText(QString::fromUtf8("V-Tipo Ángulo:"));
    tools_paint_group_tipanglelabel->setToolTip(QString::fromUtf8("El ángulo de punta para la herramienta en forma de V.\nEn grado."));
    lay150->addWidget(tools_paint_group_tipanglelabel, 3, 0, 1, 1);
    tools_paint_group_tipangle_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_tipangle_entry->setProperty("decimals", 4);
    tools_paint_group_tipangle_entry->setProperty("maximum", 180.0);
    tools_paint_group_tipangle_entry->setProperty("minimum", 1.0);
    tools_paint_group_tipangle_entry->setProperty("objectName", QString::fromUtf8("V-Tipo Ángulo"));
    tools_paint_group_tipangle_entry->setProperty("singleStep", 5.0);
    tools_paint_group_tipangle_entry->setProperty("value", 30.0);
    lay150->addWidget(tools_paint_group_tipangle_entry, 3, 1, 1, 1);
    auto* w152 = new QLabel(nullptr);
    w152->setText(QString::fromUtf8("Corte Z:"));
    w152->setToolTip(QString::fromUtf8("Profundidad de corte en el material. Valor negativo.\nEn unidades de aplicación."));
    lay150->addWidget(w152, 4, 0, 1, 1);
    tools_paint_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_cutz_entry->setProperty("decimals", 4);
    tools_paint_group_cutz_entry->setProperty("maximum", 0.0);
    tools_paint_group_cutz_entry->setProperty("minimum", -910000.0);
    tools_paint_group_cutz_entry->setProperty("objectName", QString::fromUtf8("Corte Z"));
    tools_paint_group_cutz_entry->setToolTip(QString::fromUtf8("Profundidad de corte en el material. Valor negativo.\nEn unidades de aplicación."));
    tools_paint_group_cutz_entry->setProperty("value", -0.05);
    lay150->addWidget(tools_paint_group_cutz_entry, 4, 1, 1, 1);
    tools_paint_group_newdialabel = new QLabel(nullptr);
    tools_paint_group_newdialabel->setText(QString::fromUtf8("Nuevo dia:"));
    tools_paint_group_newdialabel->setToolTip(QString::fromUtf8("Diámetro de la nueva herramienta para agregar en la Tabla de herramientas.\nSi la herramienta es de tipo V, este valor es automáticamente\ncalculado a partir de los otros parámetros."));
    lay150->addWidget(tools_paint_group_newdialabel, 5, 0, 1, 1);
    tools_paint_group_newdia_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_newdia_entry->setProperty("decimals", 4);
    tools_paint_group_newdia_entry->setProperty("maximum", 10000.0);
    tools_paint_group_newdia_entry->setProperty("objectName", QString::fromUtf8("Diá. de Herram"));
    tools_paint_group_newdia_entry->setProperty("value", 0.1);
    lay150->addWidget(tools_paint_group_newdia_entry, 5, 1, 1, 1);
    auto* w153 = new QFrame(nullptr);
    w153->setProperty("frameShadow", 48);
    w153->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w153->setSizePolicy(sp); }
    lay150->addWidget(w153, 6, 0, 1, 2);
    tools_paint_group_paint_order_label = new QLabel(nullptr);
    tools_paint_group_paint_order_label->setText(QString::fromUtf8("Orden de la Herram:"));
    tools_paint_group_paint_order_label->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    lay150->addWidget(tools_paint_group_paint_order_label, 7, 0, 1, 1);
    tools_paint_group_paint_order_radio = new RadioSet({{QString::fromUtf8("No"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Adelante"), QVariant(QString::fromUtf8("fwd"))}, {QString::fromUtf8("Atras"), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_paint_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay150->addWidget(tools_paint_group_paint_order_radio, 7, 1, 1, 1);
    auto* w154 = new QFrame(nullptr);
    w154->setProperty("frameShadow", 48);
    w154->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w154->setSizePolicy(sp); }
    lay150->addWidget(w154, 8, 0, 1, 2);
    auto* w155 = new QLabel(nullptr);
    w155->setText(QString::fromUtf8("Superposición:"));
    w155->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta.\nAjustar el valor comenzando con valores más bajos\ny aumentarlo si las áreas que deben procesarse aún están\nno procesado.Valores más bajos = procesamiento más rápido, ejecución más rápida en CNC.\nValores más altos = procesamiento lento y ejecución lenta en CNC\ndebido a demasiados caminos."));
    lay150->addWidget(w155, 9, 0, 1, 1);
    tools_paint_group_paintoverlap_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_paintoverlap_entry->setProperty("decimals", 4);
    tools_paint_group_paintoverlap_entry->setProperty("maximum", 99.9999);
    tools_paint_group_paintoverlap_entry->setProperty("singleStep", 0.1);
    tools_paint_group_paintoverlap_entry->setProperty("suffix", QString::fromUtf8(" %"));
    tools_paint_group_paintoverlap_entry->setProperty("value", 20.0);
    tools_paint_group_paintoverlap_entry->setWrapping(true);
    lay150->addWidget(tools_paint_group_paintoverlap_entry, 9, 1, 1, 1);
    auto* w156 = new QLabel(nullptr);
    w156->setText(QString::fromUtf8("Margen:"));
    w156->setToolTip(QString::fromUtf8("Distancia por la cual evitar\nlos bordes del polígono a\nser pintado."));
    lay150->addWidget(w156, 10, 0, 1, 1);
    tools_paint_group_paintmargin_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_paintmargin_entry->setProperty("decimals", 4);
    tools_paint_group_paintmargin_entry->setProperty("maximum", 10000.0);
    tools_paint_group_paintmargin_entry->setProperty("minimum", -10000.0);
    tools_paint_group_paintmargin_entry->setProperty("singleStep", 0.1);
    lay150->addWidget(tools_paint_group_paintmargin_entry, 10, 1, 1, 1);
    auto* w157 = new QLabel(nullptr);
    w157->setText(QString::fromUtf8("Método:"));
    w157->setToolTip(QString::fromUtf8("Algoritmo para pintar:\n- Estándar: paso fijo hacia adentro.\n- Basado en semillas: hacia afuera de la semilla.\n- Basado en líneas: líneas paralelas.\n- Líneas láser: activas solo para objetos Gerber.\nCreará líneas que siguen los rastros.\n- Combo: en caso de falla, se elegirá un nuevo método de los anteriores\nen el orden especificado."));
    lay150->addWidget(w157, 11, 0, 1, 1);
    tools_paint_group_paintmethod_combo = new FCComboBox2(nullptr);
    tools_paint_group_paintmethod_combo->setProperty("currentText", QString::fromUtf8("Estándar"));
    tools_paint_group_paintmethod_combo->addItem(QString::fromUtf8("Estándar"));
    tools_paint_group_paintmethod_combo->addItem(QString::fromUtf8("Semilla"));
    tools_paint_group_paintmethod_combo->addItem(QString::fromUtf8("Líneas"));
    tools_paint_group_paintmethod_combo->addItem(QString::fromUtf8("Lineas laser"));
    tools_paint_group_paintmethod_combo->addItem(QString::fromUtf8("Combo"));
    lay150->addWidget(tools_paint_group_paintmethod_combo, 11, 1, 1, 1);
    tools_paint_group_pathconnect_cb = new FCCheckBox(nullptr);
    tools_paint_group_pathconnect_cb->setChecked(true);
    tools_paint_group_pathconnect_cb->setText(QString::fromUtf8("Conectar"));
    tools_paint_group_pathconnect_cb->setToolTip(QString::fromUtf8("Dibuja líneas entre el resultado\nSegmentos para minimizar elevaciones de herramientas."));
    lay150->addWidget(tools_paint_group_pathconnect_cb, 12, 0, 1, 1);
    tools_paint_group_contour_cb = new FCCheckBox(nullptr);
    tools_paint_group_contour_cb->setChecked(true);
    tools_paint_group_contour_cb->setText(QString::fromUtf8("Contorno"));
    tools_paint_group_contour_cb->setToolTip(QString::fromUtf8("Corta todo el perímetro del polígono.\nPara recortar los bordes ásperos."));
    lay150->addWidget(tools_paint_group_contour_cb, 12, 1, 1, 1);
    auto* w158 = new QFrame(nullptr);
    w158->setProperty("frameShadow", 48);
    w158->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w158->setSizePolicy(sp); }
    lay150->addWidget(w158, 13, 0, 1, 2);
    tools_paint_group_rest_cb = new FCCheckBox(nullptr);
    tools_paint_group_rest_cb->setProperty("objectName", QString::fromUtf8("Resto"));
    tools_paint_group_rest_cb->setText(QString::fromUtf8("Resto"));
    tools_paint_group_rest_cb->setToolTip(QString::fromUtf8("Si está marcado, use 'mecanizado en resto'.\nBásicamente procesará cobre fuera de las características de la PCB,\nusando la herramienta más grande y continúe con las siguientes herramientas,\nde mayor a menor, para procesar las características de cobre que\nno pudo ser procesado por la herramienta anterior, hasta que haya\nno queda nada por procesar o no hay más herramientas.\n\nSi no está marcado, use el algoritmo estándar."));
    lay150->addWidget(tools_paint_group_rest_cb, 14, 0, 1, 2);
    auto* w159 = new QLabel(nullptr);
    w159->setText(QString::fromUtf8("Selección:"));
    w159->setToolTip(QString::fromUtf8("Selección del área a procesar.\n- 'Selección de polígonos': haga clic con el botón izquierdo del mouse para agregar / eliminar polígonos que se procesarán.\n- 'Selección de área': haga clic con el botón izquierdo del mouse para iniciar la selección del área a procesar.\nMantener presionada una tecla modificadora (CTRL o SHIFT) permitirá agregar múltiples áreas.\n- 'Todos los polígonos': el proceso comenzará después de hacer clic.\n- 'Objeto de referencia': procesará el área especificada por otro objeto."));
    lay150->addWidget(w159, 15, 0, 1, 1);
    tools_paint_group_selectmethod_combo = new FCComboBox2(nullptr);
    tools_paint_group_selectmethod_combo->setProperty("currentText", QString::fromUtf8("Todos"));
    tools_paint_group_selectmethod_combo->addItem(QString::fromUtf8("Todos"));
    tools_paint_group_selectmethod_combo->addItem(QString::fromUtf8("Selección de polígono"));
    tools_paint_group_selectmethod_combo->addItem(QString::fromUtf8("Selección de área"));
    tools_paint_group_selectmethod_combo->addItem(QString::fromUtf8("Objeto de referencia"));
    lay150->addWidget(tools_paint_group_selectmethod_combo, 15, 1, 1, 1);
    tools_paint_group_area_shape_label = new QLabel(nullptr);
    tools_paint_group_area_shape_label->setText(QString::fromUtf8("Forma:"));
    tools_paint_group_area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    lay150->addWidget(tools_paint_group_area_shape_label, 18, 0, 1, 1);
    tools_paint_group_area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay150->addWidget(tools_paint_group_area_shape_radio, 18, 1, 1, 1);
    auto* w160 = new QFrame(nullptr);
    w160->setProperty("frameShadow", 48);
    w160->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w160->setSizePolicy(sp); }
    lay150->addWidget(w160, 19, 0, 1, 2);
    auto* w161 = new QLabel(nullptr);
    w161->setText(QString::fromUtf8("Trazado:"));
    w161->setToolTip(QString::fromUtf8("- 'Normal': trazado normal, realizado al final del trabajo\n- 'Progresivo': cada forma se traza después de generarse"));
    lay150->addWidget(w161, 20, 0, 1, 1);
    tools_paint_group_paint_plotting_radio = new RadioSet({{QString::fromUtf8("Normal"), QVariant(QString::fromUtf8("normal"))}, {QString::fromUtf8("Progresivo"), QVariant(QString::fromUtf8("progressive"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_paint_plotting_radio->setValue(QVariant(QString::fromUtf8("normal")));
    lay150->addWidget(tools_paint_group_paint_plotting_radio, 20, 1, 1, 1);
    lay150->setColumnStretch(1, 1);
    tools_paint_group_layout->addLayout(lay150, 0);
    tools_paint_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_paint_group->setLayout(tools_paint_group_layout);
    vlay2->addWidget(tools_paint_group, 0);
    layout->addLayout(vlay2, 0);
    vlay3 = new QVBoxLayout();
    vlay3->setContentsMargins(0, 0, 0, 0);
    vlay3->setSpacing(6);
    tools_film_group = new QGroupBox(nullptr);
    { QFont f = tools_film_group->font(); f.setBold(true); f.setPixelSize(16); tools_film_group->setFont(f); }
    tools_film_group->setMinimumSize(QSize(220, 0));
    tools_film_group_layout = new QVBoxLayout();
    tools_film_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_film_group_layout->setSpacing(6);
    tools_film_group_film_label = new FCLabel(nullptr);
    tools_film_group_film_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_film_group_film_label->setToolTip(QString::fromUtf8("Cree una película de PCB a partir de un objeto Gerber o Geometry.\nEl archivo se guarda en formato SVG."));
    tools_film_group_layout->addWidget(tools_film_group_film_label, 0);
    auto* lay162 = new QGridLayout();
    lay162->setContentsMargins(0, 0, 0, 0);
    lay162->setHorizontalSpacing(6);
    lay162->setVerticalSpacing(6);
    auto* w163 = new FCLabel(nullptr);
    w163->setText(QString::fromUtf8("Tipo de Filme:"));
    w163->setToolTip(QString::fromUtf8("Genera una película negra positiva o una película negativa.\nPositivo significa que imprimirá las características.\nCon negro sobre un lienzo blanco.\nNegativo significa que imprimirá las características.\nCon blanco sobre un lienzo negro.\nEl formato de la película es SVG."));
    lay162->addWidget(w163, 0, 0, 1, 1);
    tools_film_group_film_type_radio = new RadioSet({{QString::fromUtf8("Pos"), QVariant(QString::fromUtf8("pos"))}, {QString::fromUtf8("Neg"), QVariant(QString::fromUtf8("neg"))}}, Qt::Horizontal, true, nullptr);
    tools_film_group_film_type_radio->setValue(QVariant(QString::fromUtf8("neg")));
    lay162->addWidget(tools_film_group_film_type_radio, 0, 1, 1, 1);
    tools_film_group_film_color_label = new FCLabel(nullptr);
    tools_film_group_film_color_label->setText(QString::fromUtf8("Color de la película:"));
    tools_film_group_film_color_label->setToolTip(QString::fromUtf8("Establezca el color de la película cuando se selecciona película positiva."));
    lay162->addWidget(tools_film_group_film_color_label, 1, 0, 1, 1);
    tools_film_group_film_color_entry = new FCColorEntry(nullptr);
    tools_film_group_film_color_entry->setValue(QString::fromUtf8("#000000"));
    lay162->addWidget(tools_film_group_film_color_entry, 1, 1, 1, 1);
    tools_film_group_film_boundary_label = new FCLabel(nullptr);
    tools_film_group_film_boundary_label->setText(QString::fromUtf8("Frontera:"));
    tools_film_group_film_boundary_label->setToolTip(QString::fromUtf8("Especifique un borde alrededor del objeto.\nSólo para película negativa.\nAyuda si usamos como objeto de caja lo mismo\nobjeto como en el objeto de la película. Se creará una gruesa\nbarra negra alrededor de la impresión real que permite una\nmejor delimitación de las características del esquema que son de\nColor blanco como el resto y que puede confundir con el\nEntorno si no fuera por esta frontera."));
    lay162->addWidget(tools_film_group_film_boundary_label, 2, 0, 1, 1);
    tools_film_group_film_boundary_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_boundary_entry->setProperty("decimals", 4);
    tools_film_group_film_boundary_entry->setProperty("maximum", 10000.0);
    tools_film_group_film_boundary_entry->setProperty("singleStep", 0.1);
    tools_film_group_film_boundary_entry->setProperty("value", 1.0);
    lay162->addWidget(tools_film_group_film_boundary_entry, 2, 1, 1, 1);
    tools_film_group_film_scale_stroke_label = new FCLabel(nullptr);
    tools_film_group_film_scale_stroke_label->setText(QString::fromUtf8("Trazo de escala:"));
    tools_film_group_film_scale_stroke_label->setToolTip(QString::fromUtf8("Escale el grosor de trazo de línea de cada entidad en el archivo SVG.\nSignifica que la línea que envuelve cada característica SVG será más gruesa o más delgada,\npor lo tanto, las características finas pueden verse más afectadas por este parámetro."));
    lay162->addWidget(tools_film_group_film_scale_stroke_label, 3, 0, 1, 1);
    tools_film_group_film_scale_stroke_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_scale_stroke_entry->setProperty("decimals", 4);
    tools_film_group_film_scale_stroke_entry->setProperty("maximum", 10000.0);
    tools_film_group_film_scale_stroke_entry->setProperty("singleStep", 0.1);
    lay162->addWidget(tools_film_group_film_scale_stroke_entry, 3, 1, 1, 1);
    tools_film_group_film_adj_label = new FCLabel(nullptr);
    tools_film_group_film_adj_label->setText(QString::fromUtf8("<b>Ajustes de la película</b>"));
    tools_film_group_film_adj_label->setToolTip(QString::fromUtf8("En algún momento, las impresoras distorsionarán la forma de impresión, especialmente los tipos de láser.\nEsta sección proporciona las herramientas para compensar las distorsiones de impresión."));
    lay162->addWidget(tools_film_group_film_adj_label, 4, 0, 1, 2);
    tools_film_group_film_scale_cb = new FCCheckBox(nullptr);
    { QFont f = tools_film_group_film_scale_cb->font(); f.setBold(true); tools_film_group_film_scale_cb->setFont(f); }
    tools_film_group_film_scale_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    tools_film_group_film_scale_cb->setText(QString::fromUtf8("Escalar la Geo de la Película"));
    tools_film_group_film_scale_cb->setToolTip(QString::fromUtf8("Un valor mayor que 1 estirará la película\nmientras que un valor menor que 1 lo sacudirá."));
    lay162->addWidget(tools_film_group_film_scale_cb, 5, 0, 1, 2);
    tools_film_group_film_scalex_label = new FCLabel(nullptr);
    tools_film_group_film_scalex_label->setText(QString::fromUtf8("Factor X:"));
    lay162->addWidget(tools_film_group_film_scalex_label, 6, 0, 1, 1);
    tools_film_group_film_scalex_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_scalex_entry->setProperty("decimals", 4);
    tools_film_group_film_scalex_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_scalex_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_scalex_entry->setProperty("singleStep", 0.01);
    tools_film_group_film_scalex_entry->setProperty("value", 1.0);
    lay162->addWidget(tools_film_group_film_scalex_entry, 6, 1, 1, 1);
    tools_film_group_film_scaley_label = new FCLabel(nullptr);
    tools_film_group_film_scaley_label->setText(QString::fromUtf8("Factor Y:"));
    lay162->addWidget(tools_film_group_film_scaley_label, 7, 0, 1, 1);
    tools_film_group_film_scaley_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_scaley_entry->setProperty("decimals", 4);
    tools_film_group_film_scaley_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_scaley_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_scaley_entry->setProperty("singleStep", 0.01);
    tools_film_group_film_scaley_entry->setProperty("value", 1.0);
    lay162->addWidget(tools_film_group_film_scaley_entry, 7, 1, 1, 1);
    tools_film_group_film_skew_cb = new FCCheckBox(nullptr);
    { QFont f = tools_film_group_film_skew_cb->font(); f.setBold(true); tools_film_group_film_skew_cb->setFont(f); }
    tools_film_group_film_skew_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    tools_film_group_film_skew_cb->setText(QString::fromUtf8("Inclina la Geo de la Película"));
    tools_film_group_film_skew_cb->setToolTip(QString::fromUtf8("Los valores positivos se sesgarán a la derecha.\nmientras que los valores negativos se desviarán a la izquierda."));
    lay162->addWidget(tools_film_group_film_skew_cb, 8, 0, 1, 2);
    tools_film_group_film_skewx_label = new FCLabel(nullptr);
    tools_film_group_film_skewx_label->setText(QString::fromUtf8("Ángulo X:"));
    lay162->addWidget(tools_film_group_film_skewx_label, 9, 0, 1, 1);
    tools_film_group_film_skewx_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_skewx_entry->setProperty("decimals", 4);
    tools_film_group_film_skewx_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_skewx_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_skewx_entry->setProperty("singleStep", 0.01);
    lay162->addWidget(tools_film_group_film_skewx_entry, 9, 1, 1, 1);
    tools_film_group_film_skewy_label = new FCLabel(nullptr);
    tools_film_group_film_skewy_label->setText(QString::fromUtf8("Ángulo Y:"));
    lay162->addWidget(tools_film_group_film_skewy_label, 10, 0, 1, 1);
    tools_film_group_film_skewy_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_skewy_entry->setProperty("decimals", 4);
    tools_film_group_film_skewy_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_skewy_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_skewy_entry->setProperty("singleStep", 0.01);
    lay162->addWidget(tools_film_group_film_skewy_entry, 10, 1, 1, 1);
    tools_film_group_film_skew_ref_label = new FCLabel(nullptr);
    tools_film_group_film_skew_ref_label->setText(QString::fromUtf8("Referencia:"));
    tools_film_group_film_skew_ref_label->setToolTip(QString::fromUtf8("El punto de referencia que se utilizará como origen para el sesgo.\nPuede ser uno de los cuatro puntos del cuadro delimitador de geometría."));
    lay162->addWidget(tools_film_group_film_skew_ref_label, 11, 0, 1, 1);
    tools_film_group_film_skew_reference = new RadioSet({{QString::fromUtf8("Abajo a la izquierda"), QVariant(QString::fromUtf8("bottomleft"))}, {QString::fromUtf8("Arriba a la izquierda"), QVariant(QString::fromUtf8("topleft"))}, {QString::fromUtf8("Abajo a la derecha"), QVariant(QString::fromUtf8("bottomright"))}, {QString::fromUtf8("Arriba a la derecha"), QVariant(QString::fromUtf8("topright"))}}, Qt::Vertical, false, nullptr);
    tools_film_group_film_skew_reference->setValue(QVariant(QString::fromUtf8("bottomleft")));
    lay162->addWidget(tools_film_group_film_skew_reference, 11, 1, 1, 1);
    tools_film_group_film_mirror_cb = new FCCheckBox(nullptr);
    { QFont f = tools_film_group_film_mirror_cb->font(); f.setBold(true); tools_film_group_film_mirror_cb->setFont(f); }
    tools_film_group_film_mirror_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    tools_film_group_film_mirror_cb->setText(QString::fromUtf8("Refleja la Geo de la Película"));
    tools_film_group_film_mirror_cb->setToolTip(QString::fromUtf8("Refleje la geometría de la película en el eje seleccionado o en ambos."));
    lay162->addWidget(tools_film_group_film_mirror_cb, 12, 0, 1, 2);
    tools_film_group_film_mirror_axis_label = new FCLabel(nullptr);
    tools_film_group_film_mirror_axis_label->setText(QString::fromUtf8("Eje espejo:"));
    lay162->addWidget(tools_film_group_film_mirror_axis_label, 13, 0, 1, 1);
    tools_film_group_film_mirror_axis = new RadioSet({{QString::fromUtf8("Ninguno"), QVariant(QString::fromUtf8("none"))}, {QString::fromUtf8("X"), QVariant(QString::fromUtf8("x"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("y"))}, {QString::fromUtf8("Ambas"), QVariant(QString::fromUtf8("both"))}}, Qt::Horizontal, false, nullptr);
    tools_film_group_film_mirror_axis->setValue(QVariant(QString::fromUtf8("none")));
    lay162->addWidget(tools_film_group_film_mirror_axis, 13, 1, 1, 1);
    auto* w164 = new QFrame(nullptr);
    w164->setProperty("frameShadow", 48);
    w164->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w164->setSizePolicy(sp); }
    lay162->addWidget(w164, 14, 0, 1, 2);
    tools_film_group_file_type_label = new FCLabel(nullptr);
    tools_film_group_file_type_label->setText(QString::fromUtf8("Tipo de Filme:"));
    tools_film_group_file_type_label->setToolTip(QString::fromUtf8("El tipo de archivo de la película guardada. Puede ser:\n- 'SVG' -> formato vectorial de código abierto\n- 'PNG' -> imagen de trama\n- 'PDF' -> formato de documento portátil"));
    lay162->addWidget(tools_film_group_file_type_label, 15, 0, 1, 1);
    tools_film_group_file_type_radio = new RadioSet({{QString::fromUtf8("SVG"), QVariant(QString::fromUtf8("svg"))}, {QString::fromUtf8("PNG"), QVariant(QString::fromUtf8("png"))}, {QString::fromUtf8("PDF"), QVariant(QString::fromUtf8("pdf"))}}, Qt::Horizontal, false, nullptr);
    tools_film_group_file_type_radio->setValue(QVariant(QString::fromUtf8("svg")));
    lay162->addWidget(tools_film_group_file_type_radio, 15, 1, 1, 1);
    tools_film_group_orientation_label = new FCLabel(nullptr);
    tools_film_group_orientation_label->setText(QString::fromUtf8("Orient. de la página:"));
    tools_film_group_orientation_label->setToolTip(QString::fromUtf8("Puede ser:\n- retrato\n- paisaje"));
    lay162->addWidget(tools_film_group_orientation_label, 16, 0, 1, 1);
    tools_film_group_orientation_radio = new RadioSet({{QString::fromUtf8("Retrato"), QVariant(QString::fromUtf8("p"))}, {QString::fromUtf8("Paisaje"), QVariant(QString::fromUtf8("l"))}}, Qt::Horizontal, false, nullptr);
    tools_film_group_orientation_radio->setValue(QVariant(QString::fromUtf8("p")));
    lay162->addWidget(tools_film_group_orientation_radio, 16, 1, 1, 1);
    tools_film_group_pagesize_label = new FCLabel(nullptr);
    tools_film_group_pagesize_label->setText(QString::fromUtf8("Tamaño de página:"));
    tools_film_group_pagesize_label->setToolTip(QString::fromUtf8("Una selección de tamaños de página estándar ISO 216."));
    lay162->addWidget(tools_film_group_pagesize_label, 17, 0, 1, 1);
    tools_film_group_pagesize_combo = new FCComboBox(nullptr);
    tools_film_group_pagesize_combo->setProperty("currentText", QString::fromUtf8("A4"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("Bounds"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A0"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A1"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A2"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A3"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A4"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A5"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A6"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A7"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A8"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A9"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("A10"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B0"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B1"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B2"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B3"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B4"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B5"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B6"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B7"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B8"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B9"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("B10"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C0"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C1"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C2"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C3"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C4"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C5"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C6"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C7"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C8"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C9"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("C10"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("LETTER"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("LEGAL"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("ELEVENSEVENTEEN"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("JUNIOR_LEGAL"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("HALF_LETTER"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("GOV_LETTER"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("GOV_LEGAL"));
    tools_film_group_pagesize_combo->addItem(QString::fromUtf8("LEDGER"));
    tools_film_group_pagesize_combo->setCurrentIndex(5);
    lay162->addWidget(tools_film_group_pagesize_combo, 17, 1, 1, 1);
    tools_film_group_png_dpi_label = new FCLabel(nullptr);
    tools_film_group_png_dpi_label->setText(QString::fromUtf8("PNG DPI:"));
    tools_film_group_png_dpi_label->setToolTip(QString::fromUtf8("El valor predeterminado es 96 DPI. Cambie este valor para escalar el archivo PNG."));
    lay162->addWidget(tools_film_group_png_dpi_label, 19, 0, 1, 1);
    tools_film_group_png_dpi_spinner = new FCSpinner(nullptr);
    tools_film_group_png_dpi_spinner->setProperty("maximum", 100000);
    tools_film_group_png_dpi_spinner->setProperty("value", 96);
    lay162->addWidget(tools_film_group_png_dpi_spinner, 19, 1, 1, 1);
    tools_film_group_layout->addLayout(lay162, 0);
    tools_film_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_film_group->setLayout(tools_film_group_layout);
    vlay3->addWidget(tools_film_group, 0);
    tools_transform_group = new QGroupBox(nullptr);
    { QFont f = tools_transform_group->font(); f.setBold(true); f.setPixelSize(16); tools_transform_group->setFont(f); }
    tools_transform_group->setMinimumSize(QSize(200, 0));
    tools_transform_group_layout = new QVBoxLayout();
    tools_transform_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_transform_group_layout->setSpacing(6);
    tools_transform_group_transform_label = new QLabel(nullptr);
    tools_transform_group_transform_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_transform_group_transform_label->setToolTip(QString::fromUtf8("Diversas transformaciones que se pueden aplicar.\nen un objeto de aplicación."));
    tools_transform_group_layout->addWidget(tools_transform_group_transform_label, 0);
    auto* lay165 = new QGridLayout();
    lay165->setContentsMargins(0, 0, 0, 0);
    lay165->setHorizontalSpacing(6);
    lay165->setVerticalSpacing(6);
    auto* w166 = new QLabel(nullptr);
    w166->setText(QString::fromUtf8("Referencia:"));
    w166->setToolTip(QString::fromUtf8("El punto de referencia para Rotate, Skew, Scale, Mirror.\nPuede ser:\n- Origen -> es el punto 0, 0\n- Selección -> el centro del cuadro delimitador de los objetos seleccionados\n- Punto -> un punto personalizado definido por coordenadas X, Y\n- Objeto -> el centro del cuadro delimitador de un objeto específico"));
    lay165->addWidget(w166, 0, 0, 1, 1);
    tools_transform_group_ref_combo = new FCComboBox(nullptr);
    tools_transform_group_ref_combo->setProperty("currentText", QString::fromUtf8("Selección"));
    tools_transform_group_ref_combo->addItem(QString::fromUtf8("Origen"));
    tools_transform_group_ref_combo->addItem(QString::fromUtf8("Selección"));
    tools_transform_group_ref_combo->addItem(QString::fromUtf8("Punto"));
    tools_transform_group_ref_combo->addItem(QString::fromUtf8("Objeto"));
    tools_transform_group_ref_combo->setCurrentIndex(1);
    lay165->addWidget(tools_transform_group_ref_combo, 0, 1, 1, 1);
    tools_transform_group_point_label = new QLabel(nullptr);
    tools_transform_group_point_label->setText(QString::fromUtf8("Punto:"));
    tools_transform_group_point_label->setToolTip(QString::fromUtf8("Un punto de referencia en formato X, Y."));
    lay165->addWidget(tools_transform_group_point_label, 1, 0, 1, 1);
    tools_transform_group_point_entry = new NumericalEvalTupleEntry(nullptr);
    tools_transform_group_point_entry->setText(QString::fromUtf8("0, 0"));
    lay165->addWidget(tools_transform_group_point_entry, 1, 1, 1, 1);
    tools_transform_group_type_object_label = new QLabel(nullptr);
    tools_transform_group_type_object_label->setText(QString::fromUtf8("Objeto:"));
    tools_transform_group_type_object_label->setToolTip(QString::fromUtf8("El tipo de objeto utilizado como referencia."));
    lay165->addWidget(tools_transform_group_type_object_label, 3, 0, 1, 1);
    tools_transform_group_type_obj_combo = new FCComboBox(nullptr);
    tools_transform_group_type_obj_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    tools_transform_group_type_obj_combo->addItem(QString::fromUtf8("Gerber"));
    tools_transform_group_type_obj_combo->addItem(QString::fromUtf8("Excellon"));
    tools_transform_group_type_obj_combo->addItem(QString::fromUtf8("Geometría"));
    lay165->addWidget(tools_transform_group_type_obj_combo, 3, 1, 1, 1);
    auto* w167 = new QLabel(nullptr);
    w167->setText(QString::fromUtf8("<b>Girar</b>"));
    lay165->addWidget(w167, 4, 0, 1, 2);
    tools_transform_group_rotate_label = new QLabel(nullptr);
    tools_transform_group_rotate_label->setText(QString::fromUtf8("Ángulo:"));
    tools_transform_group_rotate_label->setToolTip(QString::fromUtf8("Ángulo, en grados.\nNúmero Real entre -360 y 359.\nNúmeros positivos para el movimiento CW.\nNúmeros negativos para movimiento CCW."));
    lay165->addWidget(tools_transform_group_rotate_label, 6, 0, 1, 1);
    tools_transform_group_rotate_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_rotate_entry->setProperty("decimals", 4);
    tools_transform_group_rotate_entry->setProperty("maximum", 360.0);
    tools_transform_group_rotate_entry->setProperty("minimum", -360.0);
    tools_transform_group_rotate_entry->setProperty("singleStep", 15.0);
    tools_transform_group_rotate_entry->setProperty("value", 90.0);
    lay165->addWidget(tools_transform_group_rotate_entry, 6, 1, 1, 1);
    auto* w168 = new QLabel(nullptr);
    w168->setText(QString::fromUtf8("<b>Inclinar</b>"));
    lay165->addWidget(w168, 8, 0, 1, 1);
    tools_transform_group_skew_link_cb = new FCCheckBox(nullptr);
    tools_transform_group_skew_link_cb->setChecked(true);
    tools_transform_group_skew_link_cb->setText(QString::fromUtf8("Enlazar"));
    tools_transform_group_skew_link_cb->setToolTip(QString::fromUtf8("Enlace la entrada Y a la entrada X y copie su contenido."));
    lay165->addWidget(tools_transform_group_skew_link_cb, 8, 1, 1, 1);
    tools_transform_group_skewx_label = new QLabel(nullptr);
    tools_transform_group_skewx_label->setText(QString::fromUtf8("Ángulo X:"));
    tools_transform_group_skewx_label->setToolTip(QString::fromUtf8("Ángulo, en grados.\nNúmero Real entre -360 y 359."));
    lay165->addWidget(tools_transform_group_skewx_label, 9, 0, 1, 1);
    tools_transform_group_skewx_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_skewx_entry->setProperty("decimals", 4);
    tools_transform_group_skewx_entry->setProperty("maximum", 360.0);
    tools_transform_group_skewx_entry->setProperty("minimum", -360.0);
    tools_transform_group_skewx_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_skewx_entry, 9, 1, 1, 1);
    tools_transform_group_skewy_label = new QLabel(nullptr);
    tools_transform_group_skewy_label->setText(QString::fromUtf8("Ángulo Y:"));
    tools_transform_group_skewy_label->setToolTip(QString::fromUtf8("Ángulo, en grados.\nNúmero Real entre -360 y 359."));
    lay165->addWidget(tools_transform_group_skewy_label, 10, 0, 1, 1);
    tools_transform_group_skewy_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_skewy_entry->setProperty("decimals", 4);
    tools_transform_group_skewy_entry->setProperty("maximum", 360.0);
    tools_transform_group_skewy_entry->setProperty("minimum", -360.0);
    tools_transform_group_skewy_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_skewy_entry, 10, 1, 1, 1);
    auto* w169 = new QLabel(nullptr);
    w169->setText(QString::fromUtf8("<b>Escala</b>"));
    lay165->addWidget(w169, 12, 0, 1, 1);
    tools_transform_group_scale_link_cb = new FCCheckBox(nullptr);
    tools_transform_group_scale_link_cb->setChecked(true);
    tools_transform_group_scale_link_cb->setText(QString::fromUtf8("Enlazar"));
    tools_transform_group_scale_link_cb->setToolTip(QString::fromUtf8("Enlace la entrada Y a la entrada X y copie su contenido."));
    lay165->addWidget(tools_transform_group_scale_link_cb, 12, 1, 1, 1);
    tools_transform_group_scalex_label = new QLabel(nullptr);
    tools_transform_group_scalex_label->setText(QString::fromUtf8("Factor X:"));
    tools_transform_group_scalex_label->setToolTip(QString::fromUtf8("Factor de escalado en eje X."));
    lay165->addWidget(tools_transform_group_scalex_label, 14, 0, 1, 1);
    tools_transform_group_scalex_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_scalex_entry->setProperty("decimals", 4);
    tools_transform_group_scalex_entry->setProperty("maximum", 10000.0);
    tools_transform_group_scalex_entry->setProperty("singleStep", 0.1);
    tools_transform_group_scalex_entry->setProperty("value", 1.0);
    lay165->addWidget(tools_transform_group_scalex_entry, 14, 1, 1, 1);
    tools_transform_group_scaley_label = new QLabel(nullptr);
    tools_transform_group_scaley_label->setText(QString::fromUtf8("Factor Y:"));
    tools_transform_group_scaley_label->setToolTip(QString::fromUtf8("Factor de escalado en eje Y."));
    lay165->addWidget(tools_transform_group_scaley_label, 16, 0, 1, 1);
    tools_transform_group_scaley_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_scaley_entry->setProperty("decimals", 4);
    tools_transform_group_scaley_entry->setProperty("maximum", 10000.0);
    tools_transform_group_scaley_entry->setProperty("singleStep", 0.1);
    tools_transform_group_scaley_entry->setProperty("value", 1.0);
    lay165->addWidget(tools_transform_group_scaley_entry, 16, 1, 1, 1);
    auto* w170 = new QLabel(nullptr);
    w170->setText(QString::fromUtf8("<b>Compensar</b>"));
    lay165->addWidget(w170, 20, 0, 1, 2);
    tools_transform_group_offx_label = new QLabel(nullptr);
    tools_transform_group_offx_label->setText(QString::fromUtf8("Valor X:"));
    tools_transform_group_offx_label->setToolTip(QString::fromUtf8("Distancia a desplazamiento en el eje X. En unidades actuales."));
    lay165->addWidget(tools_transform_group_offx_label, 22, 0, 1, 1);
    tools_transform_group_offx_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_offx_entry->setProperty("decimals", 4);
    tools_transform_group_offx_entry->setProperty("maximum", 10000.0);
    tools_transform_group_offx_entry->setProperty("minimum", -10000.0);
    tools_transform_group_offx_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_offx_entry, 22, 1, 1, 1);
    tools_transform_group_offy_label = new QLabel(nullptr);
    tools_transform_group_offy_label->setText(QString::fromUtf8("Valor Y:"));
    tools_transform_group_offy_label->setToolTip(QString::fromUtf8("Distancia a desplazamiento en el eje Y. En unidades actuales."));
    lay165->addWidget(tools_transform_group_offy_label, 24, 0, 1, 1);
    tools_transform_group_offy_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_offy_entry->setProperty("decimals", 4);
    tools_transform_group_offy_entry->setProperty("maximum", 10000.0);
    tools_transform_group_offy_entry->setProperty("minimum", -10000.0);
    tools_transform_group_offy_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_offy_entry, 24, 1, 1, 1);
    auto* w171 = new QLabel(nullptr);
    w171->setText(QString::fromUtf8("<b>Buffer</b>"));
    lay165->addWidget(w171, 26, 0, 1, 1);
    tools_transform_group_buffer_rounded_cb = new FCCheckBox(nullptr);
    tools_transform_group_buffer_rounded_cb->setChecked(true);
    tools_transform_group_buffer_rounded_cb->setText(QString::fromUtf8("Redondeado"));
    tools_transform_group_buffer_rounded_cb->setToolTip(QString::fromUtf8("Si se marca, el búfer rodeará la forma tamponada,\nCada rincón será redondeado.\nSi no está marcado, el búfer seguirá la geometría exacta\nde la forma amortiguada."));
    lay165->addWidget(tools_transform_group_buffer_rounded_cb, 26, 1, 1, 1);
    tools_transform_group_buffer_label = new QLabel(nullptr);
    tools_transform_group_buffer_label->setText(QString::fromUtf8("Distancia:"));
    tools_transform_group_buffer_label->setToolTip(QString::fromUtf8("Un valor positivo creará el efecto de dilatación,\nmientras que un valor negativo creará el efecto de la erosión.\nCada elemento de geometría del objeto se incrementará\no disminuido con la 'distancia'."));
    lay165->addWidget(tools_transform_group_buffer_label, 28, 0, 1, 1);
    tools_transform_group_buffer_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_buffer_entry->setProperty("decimals", 4);
    tools_transform_group_buffer_entry->setProperty("maximum", 10000.0);
    tools_transform_group_buffer_entry->setProperty("minimum", -10000.0);
    tools_transform_group_buffer_entry->setProperty("singleStep", 0.1);
    tools_transform_group_buffer_entry->setWrapping(true);
    lay165->addWidget(tools_transform_group_buffer_entry, 28, 1, 1, 1);
    tools_transform_group_buffer_factor_label = new QLabel(nullptr);
    tools_transform_group_buffer_factor_label->setText(QString::fromUtf8("Valor:"));
    tools_transform_group_buffer_factor_label->setToolTip(QString::fromUtf8("Un valor positivo creará el efecto de dilatación,\nmientras que un valor negativo creará el efecto de la erosión.\nCada elemento de geometría del objeto se incrementará\no disminuido para ajustarse al 'Valor'. El Valor es un porcentaje\nde la dimensión inicial."));
    lay165->addWidget(tools_transform_group_buffer_factor_label, 30, 0, 1, 1);
    tools_transform_group_buffer_factor_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_buffer_factor_entry->setProperty("decimals", 4);
    tools_transform_group_buffer_factor_entry->setProperty("maximum", 1000.0);
    tools_transform_group_buffer_factor_entry->setProperty("minimum", -100.0);
    tools_transform_group_buffer_factor_entry->setProperty("suffix", QString::fromUtf8(" %"));
    tools_transform_group_buffer_factor_entry->setProperty("value", 100.0);
    tools_transform_group_buffer_factor_entry->setWrapping(true);
    lay165->addWidget(tools_transform_group_buffer_factor_entry, 30, 1, 1, 1);
    lay165->setColumnStretch(1, 1);
    tools_transform_group_layout->addLayout(lay165, 0);
    tools_transform_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_transform_group->setLayout(tools_transform_group_layout);
    vlay3->addWidget(tools_transform_group, 0);
    layout->addLayout(vlay3, 0);
    vlay4 = new QVBoxLayout();
    vlay4->setContentsMargins(0, 0, 0, 0);
    vlay4->setSpacing(6);
    tools_solderpaste_group = new QGroupBox(nullptr);
    { QFont f = tools_solderpaste_group->font(); f.setBold(true); f.setPixelSize(16); tools_solderpaste_group->setFont(f); }
    tools_solderpaste_group->setMinimumSize(QSize(200, 0));
    tools_solderpaste_group_layout = new QVBoxLayout();
    tools_solderpaste_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_solderpaste_group_layout->setSpacing(6);
    tools_solderpaste_group_solderpastelabel = new QLabel(nullptr);
    tools_solderpaste_group_solderpastelabel->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_solderpaste_group_solderpastelabel->setToolTip(QString::fromUtf8("Una herramienta para crear GCode para dispensar\npasta de soldadura en una PCB."));
    tools_solderpaste_group_layout->addWidget(tools_solderpaste_group_solderpastelabel, 0);
    auto* lay172 = new QGridLayout();
    lay172->setContentsMargins(0, 0, 0, 0);
    lay172->setHorizontalSpacing(6);
    lay172->setVerticalSpacing(6);
    auto* w173 = new QLabel(nullptr);
    w173->setText(QString::fromUtf8("<b><font color=\"green\">Diá. de Herram:</font></b>"));
    w173->setToolTip(QString::fromUtf8("Diámetros de las herramientas, separados por comas.\nEl valor del diámetro tiene que usar el separador de decimales de punto.\nValores válidos: 0.3, 1.0"));
    lay172->addWidget(w173, 0, 0, 1, 1);
    tools_solderpaste_group_nozzle_tool_dia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_solderpaste_group_nozzle_tool_dia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_solderpaste_group_nozzle_tool_dia_entry->setText(QString::fromUtf8("1.0, 0.3"));
    lay172->addWidget(tools_solderpaste_group_nozzle_tool_dia_entry, 0, 1, 1, 1);
    tools_solderpaste_group_addtool_entry_lbl = new QLabel(nullptr);
    tools_solderpaste_group_addtool_entry_lbl->setText(QString::fromUtf8("<b>Nuevo diá de boquilla:</b>"));
    tools_solderpaste_group_addtool_entry_lbl->setToolTip(QString::fromUtf8("Diámetro de la nueva herramienta para agregar en la tabla de herramientas"));
    lay172->addWidget(tools_solderpaste_group_addtool_entry_lbl, 1, 0, 1, 1);
    tools_solderpaste_group_addtool_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_addtool_entry->setProperty("decimals", 4);
    tools_solderpaste_group_addtool_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_addtool_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_addtool_entry->setProperty("value", 0.3);
    lay172->addWidget(tools_solderpaste_group_addtool_entry, 1, 1, 1, 1);
    tools_solderpaste_group_z_start_label = new QLabel(nullptr);
    tools_solderpaste_group_z_start_label->setText(QString::fromUtf8("Inicio de dispen. Z:"));
    tools_solderpaste_group_z_start_label->setToolTip(QString::fromUtf8("La altura (Z) cuando comienza la dispensación de pasta de soldadura."));
    lay172->addWidget(tools_solderpaste_group_z_start_label, 2, 0, 1, 1);
    tools_solderpaste_group_z_start_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_start_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_start_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_start_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_start_entry->setProperty("value", 0.05);
    lay172->addWidget(tools_solderpaste_group_z_start_entry, 2, 1, 1, 1);
    tools_solderpaste_group_z_dispense_label = new QLabel(nullptr);
    tools_solderpaste_group_z_dispense_label->setText(QString::fromUtf8("Dispensación Z:"));
    tools_solderpaste_group_z_dispense_label->setToolTip(QString::fromUtf8("La altura (Z) al dispensar pasta de soldadura."));
    lay172->addWidget(tools_solderpaste_group_z_dispense_label, 3, 0, 1, 1);
    tools_solderpaste_group_z_dispense_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_dispense_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_dispense_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_dispense_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_dispense_entry->setProperty("value", 0.1);
    lay172->addWidget(tools_solderpaste_group_z_dispense_entry, 3, 1, 1, 1);
    tools_solderpaste_group_z_stop_label = new QLabel(nullptr);
    tools_solderpaste_group_z_stop_label->setText(QString::fromUtf8("Parada de dispen. Z:"));
    tools_solderpaste_group_z_stop_label->setToolTip(QString::fromUtf8("La altura (Z) cuando se detiene la dispensación de pasta de soldadura."));
    lay172->addWidget(tools_solderpaste_group_z_stop_label, 4, 0, 1, 1);
    tools_solderpaste_group_z_stop_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_stop_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_stop_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_stop_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_stop_entry->setProperty("value", 0.05);
    lay172->addWidget(tools_solderpaste_group_z_stop_entry, 4, 1, 1, 1);
    tools_solderpaste_group_z_travel_label = new QLabel(nullptr);
    tools_solderpaste_group_z_travel_label->setText(QString::fromUtf8("Viajar Z:"));
    tools_solderpaste_group_z_travel_label->setToolTip(QString::fromUtf8("La altura (Z) para viajar entre almohadillas\n(sin dispensar pasta de soldadura)."));
    lay172->addWidget(tools_solderpaste_group_z_travel_label, 5, 0, 1, 1);
    tools_solderpaste_group_z_travel_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_travel_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_travel_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_travel_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_travel_entry->setProperty("value", 0.1);
    lay172->addWidget(tools_solderpaste_group_z_travel_entry, 5, 1, 1, 1);
    tools_solderpaste_group_z_toolchange_label = new QLabel(nullptr);
    tools_solderpaste_group_z_toolchange_label->setText(QString::fromUtf8("Cambio de herra. Z:"));
    tools_solderpaste_group_z_toolchange_label->setToolTip(QString::fromUtf8("La altura (Z) para el cambio de herramienta (boquilla)."));
    lay172->addWidget(tools_solderpaste_group_z_toolchange_label, 6, 0, 1, 1);
    tools_solderpaste_group_z_toolchange_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_toolchange_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_toolchange_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_toolchange_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_toolchange_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_z_toolchange_entry, 6, 1, 1, 1);
    tools_solderpaste_group_xy_toolchange_label = new QLabel(nullptr);
    tools_solderpaste_group_xy_toolchange_label->setText(QString::fromUtf8("Cambio de herra X, Y:"));
    tools_solderpaste_group_xy_toolchange_label->setToolTip(QString::fromUtf8("La ubicación X, Y para el cambio de herramienta (boquilla).\nEl formato es (x, y) donde x e y son números reales."));
    lay172->addWidget(tools_solderpaste_group_xy_toolchange_label, 7, 0, 1, 1);
    tools_solderpaste_group_xy_toolchange_entry = new NumericalEvalTupleEntry(nullptr);
    tools_solderpaste_group_xy_toolchange_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_solderpaste_group_xy_toolchange_entry->setText(QString::fromUtf8("0.0, 0.0"));
    lay172->addWidget(tools_solderpaste_group_xy_toolchange_entry, 7, 1, 1, 1);
    tools_solderpaste_group_frxy_label = new QLabel(nullptr);
    tools_solderpaste_group_frxy_label->setText(QString::fromUtf8("Avance X-Y:"));
    tools_solderpaste_group_frxy_label->setToolTip(QString::fromUtf8("Avance (velocidad) mientras se mueve en el plano X-Y."));
    lay172->addWidget(tools_solderpaste_group_frxy_label, 8, 0, 1, 1);
    tools_solderpaste_group_frxy_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_frxy_entry->setProperty("decimals", 4);
    tools_solderpaste_group_frxy_entry->setProperty("maximum", 910000.0);
    tools_solderpaste_group_frxy_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_frxy_entry->setProperty("value", 150.0);
    lay172->addWidget(tools_solderpaste_group_frxy_entry, 8, 1, 1, 1);
    tools_solderpaste_group_frz_label = new QLabel(nullptr);
    tools_solderpaste_group_frz_label->setText(QString::fromUtf8("Avance Z:"));
    tools_solderpaste_group_frz_label->setToolTip(QString::fromUtf8("Avance (velocidad) mientras se mueve verticalmente\n(en el plano Z)."));
    lay172->addWidget(tools_solderpaste_group_frz_label, 9, 0, 1, 1);
    tools_solderpaste_group_frz_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_frz_entry->setProperty("decimals", 4);
    tools_solderpaste_group_frz_entry->setProperty("maximum", 910000.0);
    tools_solderpaste_group_frz_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_frz_entry->setProperty("value", 150.0);
    lay172->addWidget(tools_solderpaste_group_frz_entry, 9, 1, 1, 1);
    tools_solderpaste_group_frz_dispense_label = new QLabel(nullptr);
    tools_solderpaste_group_frz_dispense_label->setText(QString::fromUtf8("Avance de Dispens. Z:"));
    tools_solderpaste_group_frz_dispense_label->setToolTip(QString::fromUtf8("Avance (velocidad) mientras se mueve verticalmente\npara dispensar la posición (en el plano Z)."));
    lay172->addWidget(tools_solderpaste_group_frz_dispense_label, 10, 0, 1, 1);
    tools_solderpaste_group_frz_dispense_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_frz_dispense_entry->setProperty("decimals", 4);
    tools_solderpaste_group_frz_dispense_entry->setProperty("maximum", 910000.0);
    tools_solderpaste_group_frz_dispense_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_frz_dispense_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_frz_dispense_entry, 10, 1, 1, 1);
    tools_solderpaste_group_speedfwd_label = new QLabel(nullptr);
    tools_solderpaste_group_speedfwd_label->setText(QString::fromUtf8("Veloc. del husillo FWD:"));
    tools_solderpaste_group_speedfwd_label->setToolTip(QString::fromUtf8("La velocidad del dispensador mientras empuja la pasta de soldadura\na través de la boquilla dispensadora."));
    lay172->addWidget(tools_solderpaste_group_speedfwd_label, 11, 0, 1, 1);
    tools_solderpaste_group_speedfwd_entry = new FCSpinner(nullptr);
    tools_solderpaste_group_speedfwd_entry->setProperty("maximum", 99999);
    tools_solderpaste_group_speedfwd_entry->setProperty("singleStep", 1000);
    tools_solderpaste_group_speedfwd_entry->setProperty("value", 300);
    lay172->addWidget(tools_solderpaste_group_speedfwd_entry, 11, 1, 1, 1);
    tools_solderpaste_group_dwellfwd_label = new QLabel(nullptr);
    tools_solderpaste_group_dwellfwd_label->setText(QString::fromUtf8("Morar FWD:"));
    tools_solderpaste_group_dwellfwd_label->setToolTip(QString::fromUtf8("Pausa después de la dispensación de soldadura."));
    lay172->addWidget(tools_solderpaste_group_dwellfwd_label, 12, 0, 1, 1);
    tools_solderpaste_group_dwellfwd_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_dwellfwd_entry->setProperty("decimals", 4);
    tools_solderpaste_group_dwellfwd_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_dwellfwd_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_dwellfwd_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_dwellfwd_entry, 12, 1, 1, 1);
    tools_solderpaste_group_speedrev_label = new QLabel(nullptr);
    tools_solderpaste_group_speedrev_label->setText(QString::fromUtf8("Veloc. del husillo REV:"));
    tools_solderpaste_group_speedrev_label->setToolTip(QString::fromUtf8("La velocidad del dispensador mientras se retrae la pasta de soldadura\na través de la boquilla dispensadora."));
    lay172->addWidget(tools_solderpaste_group_speedrev_label, 13, 0, 1, 1);
    tools_solderpaste_group_speedrev_entry = new FCSpinner(nullptr);
    tools_solderpaste_group_speedrev_entry->setProperty("maximum", 999999);
    tools_solderpaste_group_speedrev_entry->setProperty("singleStep", 1000);
    tools_solderpaste_group_speedrev_entry->setProperty("value", 200);
    lay172->addWidget(tools_solderpaste_group_speedrev_entry, 13, 1, 1, 1);
    tools_solderpaste_group_dwellrev_label = new QLabel(nullptr);
    tools_solderpaste_group_dwellrev_label->setText(QString::fromUtf8("Morar REV:"));
    tools_solderpaste_group_dwellrev_label->setToolTip(QString::fromUtf8("Pausa después de que el dispensador de pasta de soldadura se retraiga,\npara permitir el equilibrio de presión."));
    lay172->addWidget(tools_solderpaste_group_dwellrev_label, 14, 0, 1, 1);
    tools_solderpaste_group_dwellrev_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_dwellrev_entry->setProperty("decimals", 4);
    tools_solderpaste_group_dwellrev_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_dwellrev_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_dwellrev_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_dwellrev_entry, 14, 1, 1, 1);
    auto* w174 = new QLabel(nullptr);
    w174->setText(QString::fromUtf8("Postprocesador:"));
    w174->setToolTip(QString::fromUtf8("Archivos que controlan la generación de GCode."));
    lay172->addWidget(w174, 15, 0, 1, 1);
    tools_solderpaste_group_pp_combo = new FCComboBox(nullptr);
    lay172->addWidget(tools_solderpaste_group_pp_combo, 15, 1, 1, 1);
    tools_solderpaste_group_layout->addLayout(lay172, 0);
    tools_solderpaste_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_solderpaste_group->setLayout(tools_solderpaste_group_layout);
    vlay4->addWidget(tools_solderpaste_group, 0);
    tools_corners_group = new QGroupBox(nullptr);
    { QFont f = tools_corners_group->font(); f.setBold(true); f.setPixelSize(16); tools_corners_group->setFont(f); }
    tools_corners_group->setMinimumSize(QSize(200, 0));
    tools_corners_group_layout = new QVBoxLayout();
    tools_corners_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_corners_group_layout->setSpacing(6);
    auto* lay175 = new QGridLayout();
    lay175->setContentsMargins(0, 0, 0, 0);
    lay175->setHorizontalSpacing(6);
    lay175->setVerticalSpacing(6);
    tools_corners_group_param_label = new FCLabel(nullptr);
    tools_corners_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_corners_group_param_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay175->addWidget(tools_corners_group_param_label, 0, 0, 1, 2);
    tools_corners_group_type_label = new FCLabel(nullptr);
    tools_corners_group_type_label->setText(QString::fromUtf8("Tipo:"));
    tools_corners_group_type_label->setToolTip(QString::fromUtf8("Forma del marcador."));
    lay175->addWidget(tools_corners_group_type_label, 2, 0, 1, 1);
    tools_corners_group_type_radio = new RadioSet({{QString::fromUtf8("Semi-Cruz"), QVariant(QString::fromUtf8("s"))}, {QString::fromUtf8("Cruce"), QVariant(QString::fromUtf8("c"))}}, Qt::Horizontal, true, nullptr);
    tools_corners_group_type_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay175->addWidget(tools_corners_group_type_radio, 2, 1, 1, 1);
    tools_corners_group_thick_label = new FCLabel(nullptr);
    tools_corners_group_thick_label->setText(QString::fromUtf8("Espesor:"));
    tools_corners_group_thick_label->setToolTip(QString::fromUtf8("El grosor de la línea que hace el marcador de esquina."));
    lay175->addWidget(tools_corners_group_thick_label, 4, 0, 1, 1);
    tools_corners_group_thick_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_thick_entry->setProperty("decimals", 4);
    tools_corners_group_thick_entry->setProperty("maximum", 9.9999);
    tools_corners_group_thick_entry->setProperty("singleStep", 0.0001);
    tools_corners_group_thick_entry->setProperty("value", 0.1);
    tools_corners_group_thick_entry->setWrapping(true);
    lay175->addWidget(tools_corners_group_thick_entry, 4, 1, 1, 1);
    tools_corners_group_margin_label = new FCLabel(nullptr);
    tools_corners_group_margin_label->setText(QString::fromUtf8("Margen:"));
    tools_corners_group_margin_label->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay175->addWidget(tools_corners_group_margin_label, 6, 0, 1, 1);
    tools_corners_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_margin_entry->setProperty("decimals", 4);
    tools_corners_group_margin_entry->setProperty("maximum", 10000.0);
    tools_corners_group_margin_entry->setProperty("minimum", -10000.0);
    tools_corners_group_margin_entry->setProperty("singleStep", 0.1);
    lay175->addWidget(tools_corners_group_margin_entry, 6, 1, 1, 1);
    tools_corners_group_l_label = new FCLabel(nullptr);
    tools_corners_group_l_label->setText(QString::fromUtf8("Longitud:"));
    tools_corners_group_l_label->setToolTip(QString::fromUtf8("La longitud de la línea que hace el marcador de esquina."));
    lay175->addWidget(tools_corners_group_l_label, 8, 0, 1, 1);
    tools_corners_group_l_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_l_entry->setProperty("decimals", 4);
    tools_corners_group_l_entry->setProperty("maximum", 10000.0);
    tools_corners_group_l_entry->setProperty("minimum", -10000.0);
    tools_corners_group_l_entry->setProperty("singleStep", 0.0001);
    tools_corners_group_l_entry->setProperty("value", 3.0);
    lay175->addWidget(tools_corners_group_l_entry, 8, 1, 1, 1);
    tools_corners_group_drill_dia_label = new FCLabel(nullptr);
    tools_corners_group_drill_dia_label->setText(QString::fromUtf8("Diá. del Taladro:"));
    tools_corners_group_drill_dia_label->setToolTip(QString::fromUtf8("Diá del Taladro."));
    lay175->addWidget(tools_corners_group_drill_dia_label, 10, 0, 1, 1);
    tools_corners_group_drill_dia_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_drill_dia_entry->setProperty("decimals", 4);
    tools_corners_group_drill_dia_entry->setProperty("maximum", 100.0);
    tools_corners_group_drill_dia_entry->setProperty("value", 0.5);
    tools_corners_group_drill_dia_entry->setWrapping(true);
    lay175->addWidget(tools_corners_group_drill_dia_entry, 10, 1, 1, 1);
    lay175->setColumnStretch(1, 1);
    tools_corners_group_layout->addLayout(lay175, 0);
    tools_corners_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_corners_group->setLayout(tools_corners_group_layout);
    vlay4->addWidget(tools_corners_group, 0);
    tools_calculators_group = new QGroupBox(nullptr);
    { QFont f = tools_calculators_group->font(); f.setBold(true); f.setPixelSize(16); tools_calculators_group->setFont(f); }
    tools_calculators_group->setMinimumSize(QSize(220, 0));
    tools_calculators_group_layout = new QVBoxLayout();
    tools_calculators_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_calculators_group_layout->setSpacing(6);
    tools_calculators_group_vshape_tool_label = new FCLabel(nullptr);
    tools_calculators_group_vshape_tool_label->setText(QString::fromUtf8("<b>Calc. de herra. en forma de V:</b>"));
    tools_calculators_group_vshape_tool_label->setToolTip(QString::fromUtf8("Calcule el diámetro de la herramienta para una herramienta de forma de V dada,\nteniendo el diámetro de la punta, el ángulo de la punta y\nProfundidad de corte como parámetros."));
    tools_calculators_group_layout->addWidget(tools_calculators_group_vshape_tool_label, 0);
    auto* lay176 = new QGridLayout();
    lay176->setContentsMargins(0, 0, 0, 0);
    lay176->setHorizontalSpacing(6);
    lay176->setVerticalSpacing(6);
    tools_calculators_group_tip_dia_label = new FCLabel(nullptr);
    tools_calculators_group_tip_dia_label->setText(QString::fromUtf8("Diá. de la punta:"));
    tools_calculators_group_tip_dia_label->setToolTip(QString::fromUtf8("Este es el diámetro de la punta de la herramienta.\nEstá especificado por el fabricante."));
    lay176->addWidget(tools_calculators_group_tip_dia_label, 0, 0, 1, 1);
    tools_calculators_group_tip_dia_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_tip_dia_entry->setProperty("decimals", 4);
    tools_calculators_group_tip_dia_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_tip_dia_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_tip_dia_entry->setProperty("value", 0.2);
    lay176->addWidget(tools_calculators_group_tip_dia_entry, 0, 1, 1, 1);
    tools_calculators_group_tip_angle_label = new FCLabel(nullptr);
    tools_calculators_group_tip_angle_label->setText(QString::fromUtf8("Ángulo de la punta:"));
    tools_calculators_group_tip_angle_label->setToolTip(QString::fromUtf8("Este es el ángulo en la punta de la herramienta.\nEstá especificado por el fabricante."));
    lay176->addWidget(tools_calculators_group_tip_angle_label, 2, 0, 1, 1);
    tools_calculators_group_tip_angle_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_tip_angle_entry->setProperty("decimals", 4);
    tools_calculators_group_tip_angle_entry->setProperty("maximum", 180.0);
    tools_calculators_group_tip_angle_entry->setProperty("singleStep", 5.0);
    tools_calculators_group_tip_angle_entry->setProperty("value", 30.0);
    lay176->addWidget(tools_calculators_group_tip_angle_entry, 2, 1, 1, 1);
    tools_calculators_group_cut_z_label = new FCLabel(nullptr);
    tools_calculators_group_cut_z_label->setText(QString::fromUtf8("Corte Z:"));
    tools_calculators_group_cut_z_label->setToolTip(QString::fromUtf8("Esta es la profundidad para cortar en material.\nEn el objeto de trabajo CNC es el parámetro CutZ."));
    lay176->addWidget(tools_calculators_group_cut_z_label, 4, 0, 1, 1);
    tools_calculators_group_cut_z_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_cut_z_entry->setProperty("decimals", 4);
    tools_calculators_group_cut_z_entry->setProperty("maximum", 0.0);
    tools_calculators_group_cut_z_entry->setProperty("minimum", -10000.0);
    tools_calculators_group_cut_z_entry->setProperty("singleStep", 0.01);
    lay176->addWidget(tools_calculators_group_cut_z_entry, 4, 1, 1, 1);
    tools_calculators_group_plate_title_label = new FCLabel(nullptr);
    tools_calculators_group_plate_title_label->setText(QString::fromUtf8("<b>Calculadora de electrochapado:</b>"));
    tools_calculators_group_plate_title_label->setToolTip(QString::fromUtf8("Esta calculadora es útil para aquellos que platican la vía / la almohadilla / los agujeros de perforación,\nUtilizando un método como tinta de graphite o tinta de hipofosfito de calcio o cloruro de paladio."));
    lay176->addWidget(tools_calculators_group_plate_title_label, 3, 0, 1, 2);
    tools_calculators_group_pcblengthlabel = new FCLabel(nullptr);
    tools_calculators_group_pcblengthlabel->setText(QString::fromUtf8("Longitud del tablero:"));
    tools_calculators_group_pcblengthlabel->setToolTip(QString::fromUtf8("Esta es la longitud del tablero. En centímetros."));
    lay176->addWidget(tools_calculators_group_pcblengthlabel, 6, 0, 1, 1);
    tools_calculators_group_pcblength_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_pcblength_entry->setProperty("decimals", 4);
    tools_calculators_group_pcblength_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_pcblength_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_pcblength_entry->setProperty("value", 10.0);
    lay176->addWidget(tools_calculators_group_pcblength_entry, 6, 1, 1, 1);
    tools_calculators_group_pcbwidthlabel = new FCLabel(nullptr);
    tools_calculators_group_pcbwidthlabel->setText(QString::fromUtf8("Ancho del tablero:"));
    tools_calculators_group_pcbwidthlabel->setToolTip(QString::fromUtf8("Este es el ancho de la tabla. En centímetros."));
    lay176->addWidget(tools_calculators_group_pcbwidthlabel, 8, 0, 1, 1);
    tools_calculators_group_pcbwidth_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_pcbwidth_entry->setProperty("decimals", 4);
    tools_calculators_group_pcbwidth_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_pcbwidth_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_pcbwidth_entry->setProperty("value", 10.0);
    lay176->addWidget(tools_calculators_group_pcbwidth_entry, 8, 1, 1, 1);
    tools_calculators_group_area_label = new FCLabel(nullptr);
    tools_calculators_group_area_label->setText(QString::fromUtf8("Zona:"));
    tools_calculators_group_area_label->setToolTip(QString::fromUtf8("Esta es el área del PCB."));
    lay176->addWidget(tools_calculators_group_area_label, 10, 0, 1, 1);
    tools_calculators_group_area_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_area_entry->setProperty("decimals", 4);
    tools_calculators_group_area_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); tools_calculators_group_area_entry->setSizePolicy(sp); }
    tools_calculators_group_area_entry->setProperty("value", 100.0);
    lay176->addWidget(tools_calculators_group_area_entry, 10, 1, 1, 1);
    tools_calculators_group_cdensity_label = new FCLabel(nullptr);
    tools_calculators_group_cdensity_label->setText(QString::fromUtf8("Densidad actual:"));
    tools_calculators_group_cdensity_label->setToolTip(QString::fromUtf8("Densidad de corriente para pasar por el tablero.\nEn amperios por pies cuadrados ASF."));
    lay176->addWidget(tools_calculators_group_cdensity_label, 12, 0, 1, 1);
    tools_calculators_group_cdensity_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_cdensity_entry->setProperty("decimals", 4);
    tools_calculators_group_cdensity_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_cdensity_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_cdensity_entry->setProperty("value", 13.0);
    lay176->addWidget(tools_calculators_group_cdensity_entry, 12, 1, 1, 1);
    tools_calculators_group_growth_label = new FCLabel(nullptr);
    tools_calculators_group_growth_label->setText(QString::fromUtf8("Crecimiento de cobre:"));
    tools_calculators_group_growth_label->setToolTip(QString::fromUtf8("Qué tan grueso pretende ser el crecimiento del cobre.\nEn micras."));
    lay176->addWidget(tools_calculators_group_growth_label, 14, 0, 1, 1);
    tools_calculators_group_growth_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_growth_entry->setProperty("decimals", 4);
    tools_calculators_group_growth_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_growth_entry->setProperty("singleStep", 0.01);
    tools_calculators_group_growth_entry->setProperty("value", 10.0);
    lay176->addWidget(tools_calculators_group_growth_entry, 14, 1, 1, 1);
    lay176->setColumnStretch(1, 1);
    tools_calculators_group_layout->addLayout(lay176, 0);
    tools_calculators_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_calculators_group->setLayout(tools_calculators_group_layout);
    vlay4->addWidget(tools_calculators_group, 0);
    tools_sub_group = new QGroupBox(nullptr);
    { QFont f = tools_sub_group->font(); f.setBold(true); f.setPixelSize(16); tools_sub_group->setFont(f); }
    tools_sub_group->setMinimumSize(QSize(200, 0));
    tools_sub_group_layout = new QVBoxLayout();
    tools_sub_group_layout->setContentsMargins(9, 9, 9, 9);
    tools_sub_group_layout->setSpacing(6);
    tools_sub_group_sublabel = new QLabel(nullptr);
    tools_sub_group_sublabel->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools_sub_group_sublabel->setToolTip(QString::fromUtf8("Una herramienta para restar un objeto Gerber o Geometry\nde otro del mismo tipo."));
    tools_sub_group_layout->addWidget(tools_sub_group_sublabel, 0);
    tools_sub_group_close_paths_cb = new FCCheckBox(nullptr);
    tools_sub_group_close_paths_cb->setChecked(true);
    tools_sub_group_close_paths_cb->setText(QString::fromUtf8("Caminos cercanos"));
    tools_sub_group_close_paths_cb->setToolTip(QString::fromUtf8("Marcar esto cerrará los caminos cortados por el objeto sustractor."));
    tools_sub_group_layout->addWidget(tools_sub_group_close_paths_cb, 0);
    tools_sub_group_delete_sources_cb = new FCCheckBox(nullptr);
    tools_sub_group_delete_sources_cb->setText(QString::fromUtf8("Eliminar fuente"));
    tools_sub_group_delete_sources_cb->setToolTip(QString::fromUtf8("Cuando está marcado, eliminará los objetos de origen\ndespués de una operación exitosa."));
    tools_sub_group_layout->addWidget(tools_sub_group_delete_sources_cb, 0);
    tools_sub_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools_sub_group->setLayout(tools_sub_group_layout);
    vlay4->addWidget(tools_sub_group, 0);
    layout->addLayout(vlay4, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    tools_defaults_form->setLayout(layout);
    tools_scroll_area->setWidget(tools_defaults_form);
    tools_tab_lay->addWidget(tools_scroll_area, 0);
    tools_tab->setLayout(tools_tab_lay);
    pref_tab_area->addTab(tools_tab, QString::fromUtf8("HERRAMIENTAS"));
    tools2_tab = new QWidget(nullptr);
    tools2_tab->setHidden(true);
    tools2_tab_lay = new QVBoxLayout();
    tools2_tab_lay->setContentsMargins(2, 2, 2, 2);
    tools2_tab_lay->setSpacing(6);
    tools2_scroll_area = new QScrollArea(nullptr);
    tools2_defaults_form = new QWidget(nullptr);
    tools2_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    vlay = new QVBoxLayout();
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(6);
    tools2_checkrules_group = new QGroupBox(nullptr);
    { QFont f = tools2_checkrules_group->font(); f.setBold(true); f.setPixelSize(16); tools2_checkrules_group->setFont(f); }
    tools2_checkrules_group->setMinimumSize(QSize(220, 0));
    tools2_checkrules_group_layout = new QVBoxLayout();
    tools2_checkrules_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_checkrules_group_layout->setSpacing(6);
    tools2_checkrules_group_crlabel = new QLabel(nullptr);
    tools2_checkrules_group_crlabel->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_checkrules_group_crlabel->setToolTip(QString::fromUtf8("Una herramienta para verificar si los archivos de Gerber están dentro de un conjunto\nde las normas de fabricación."));
    tools2_checkrules_group_layout->addWidget(tools2_checkrules_group_crlabel, 0);
    tools2_checkrules_group_form_layout_1 = new QFormLayout();
    tools2_checkrules_group_form_layout_1->setContentsMargins(0, 0, 0, 0);
    tools2_checkrules_group_form_layout_1->setHorizontalSpacing(6);
    tools2_checkrules_group_form_layout_1->setVerticalSpacing(6);
    tools2_checkrules_group_trace_size_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_trace_size_cb->setChecked(true);
    tools2_checkrules_group_trace_size_cb->setText(QString::fromUtf8("Tamaño de traza:"));
    tools2_checkrules_group_trace_size_cb->setToolTip(QString::fromUtf8("Esto comprueba si se cumple el tamaño mínimo para las trazas."));
    tools2_checkrules_group_form_layout_1->setWidget(0, QFormLayout::ItemRole(2), tools2_checkrules_group_trace_size_cb);
    tools2_checkrules_group_trace_size_lbl = new QLabel(nullptr);
    tools2_checkrules_group_trace_size_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_trace_size_lbl->setToolTip(QString::fromUtf8("Tamaño de traza mínimo aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(1, QFormLayout::ItemRole(0), tools2_checkrules_group_trace_size_lbl);
    tools2_checkrules_group_trace_size_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_trace_size_entry->setProperty("decimals", 4);
    tools2_checkrules_group_trace_size_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_trace_size_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_trace_size_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(1, QFormLayout::ItemRole(1), tools2_checkrules_group_trace_size_entry);
    tools2_checkrules_group_clearance_copper2copper_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_copper2copper_cb->setChecked(true);
    tools2_checkrules_group_clearance_copper2copper_cb->setText(QString::fromUtf8("Distancia de Cobre a Cobre:"));
    tools2_checkrules_group_clearance_copper2copper_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre cobre\nhuellas se cumplen."));
    tools2_checkrules_group_form_layout_1->setWidget(2, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_copper2copper_cb);
    tools2_checkrules_group_clearance_copper2copper_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_copper2copper_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_copper2copper_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(3, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_copper2copper_lbl);
    tools2_checkrules_group_clearance_copper2copper_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(3, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_copper2copper_entry);
    tools2_checkrules_group_clearance_copper2ol_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_copper2ol_cb->setChecked(true);
    tools2_checkrules_group_clearance_copper2ol_cb->setText(QString::fromUtf8("Distancia de Cobre a Contorno:"));
    tools2_checkrules_group_clearance_copper2ol_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre cobre\nhuellas y el esquema se cumple."));
    tools2_checkrules_group_form_layout_1->setWidget(4, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_copper2ol_cb);
    tools2_checkrules_group_clearance_copper2ol_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_copper2ol_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_copper2ol_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(5, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_copper2ol_lbl);
    tools2_checkrules_group_clearance_copper2ol_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("value", 1.0);
    tools2_checkrules_group_form_layout_1->setWidget(5, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_copper2ol_entry);
    tools2_checkrules_group_clearance_silk2silk_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_silk2silk_cb->setChecked(true);
    tools2_checkrules_group_clearance_silk2silk_cb->setText(QString::fromUtf8("Distancia de Serigrafía a Serigrafía:"));
    tools2_checkrules_group_clearance_silk2silk_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre serigrafía\nhuellas y huellas de serigrafía se cumplen."));
    tools2_checkrules_group_form_layout_1->setWidget(6, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_silk2silk_cb);
    tools2_checkrules_group_clearance_silk2silk_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_silk2silk_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_silk2silk_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(7, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_silk2silk_lbl);
    tools2_checkrules_group_clearance_silk2silk_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(7, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_silk2silk_entry);
    tools2_checkrules_group_clearance_silk2sm_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_silk2sm_cb->setChecked(true);
    tools2_checkrules_group_clearance_silk2sm_cb->setText(QString::fromUtf8("Serigrafía para Soldar Máscara Distancia:"));
    tools2_checkrules_group_clearance_silk2sm_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre serigrafía\nTraces y soldermask traces se cumplen."));
    tools2_checkrules_group_form_layout_1->setWidget(8, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_silk2sm_cb);
    tools2_checkrules_group_clearance_silk2sm_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_silk2sm_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_silk2sm_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(9, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_silk2sm_lbl);
    tools2_checkrules_group_clearance_silk2sm_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(9, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_silk2sm_entry);
    tools2_checkrules_group_clearance_silk2ol_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_silk2ol_cb->setChecked(true);
    tools2_checkrules_group_clearance_silk2ol_cb->setText(QString::fromUtf8("Serigrafía para Contorno Distancia:"));
    tools2_checkrules_group_clearance_silk2ol_cb->setToolTip(QString::fromUtf8("Esto verifica si el espacio libre mínimo entre la serigrafía\nhuellas y el contorno se cumple."));
    tools2_checkrules_group_form_layout_1->setWidget(10, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_silk2ol_cb);
    tools2_checkrules_group_clearance_silk2ol_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_silk2ol_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_silk2ol_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(11, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_silk2ol_lbl);
    tools2_checkrules_group_clearance_silk2ol_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("value", 1.0);
    tools2_checkrules_group_form_layout_1->setWidget(11, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_silk2ol_entry);
    tools2_checkrules_group_clearance_sm2sm_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_sm2sm_cb->setChecked(true);
    tools2_checkrules_group_clearance_sm2sm_cb->setText(QString::fromUtf8("Astilla de máscara de soldadura mínima:"));
    tools2_checkrules_group_clearance_sm2sm_cb->setToolTip(QString::fromUtf8("Esto verifica si la distancia mínima entre la máscara de soldadura\nrastros y rastros de máscara de soldadura se cumplen."));
    tools2_checkrules_group_form_layout_1->setWidget(12, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_sm2sm_cb);
    tools2_checkrules_group_clearance_sm2sm_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_sm2sm_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_sm2sm_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(13, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_sm2sm_lbl);
    tools2_checkrules_group_clearance_sm2sm_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(13, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_sm2sm_entry);
    tools2_checkrules_group_ring_integrity_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_ring_integrity_cb->setChecked(true);
    tools2_checkrules_group_ring_integrity_cb->setText(QString::fromUtf8("Anillo anular mínimo:"));
    tools2_checkrules_group_ring_integrity_cb->setToolTip(QString::fromUtf8("Esto verifica si queda el anillo de cobre mínimo al perforar\nSe encuentra un agujero en una almohadilla."));
    tools2_checkrules_group_form_layout_1->setWidget(14, QFormLayout::ItemRole(2), tools2_checkrules_group_ring_integrity_cb);
    tools2_checkrules_group_ring_integrity_lbl = new QLabel(nullptr);
    tools2_checkrules_group_ring_integrity_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_ring_integrity_lbl->setToolTip(QString::fromUtf8("Valor mínimo aceptable del anillo."));
    tools2_checkrules_group_form_layout_1->setWidget(15, QFormLayout::ItemRole(0), tools2_checkrules_group_ring_integrity_lbl);
    tools2_checkrules_group_ring_integrity_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_ring_integrity_entry->setProperty("decimals", 4);
    tools2_checkrules_group_ring_integrity_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_ring_integrity_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_ring_integrity_entry->setProperty("value", 0.3);
    tools2_checkrules_group_form_layout_1->setWidget(15, QFormLayout::ItemRole(1), tools2_checkrules_group_ring_integrity_entry);
    auto* w177 = new QLabel(nullptr);
    tools2_checkrules_group_form_layout_1->setWidget(16, QFormLayout::ItemRole(2), w177);
    tools2_checkrules_group_clearance_d2d_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_d2d_cb->setChecked(true);
    tools2_checkrules_group_clearance_d2d_cb->setText(QString::fromUtf8("Distancia entre Agujeros:"));
    tools2_checkrules_group_clearance_d2d_cb->setToolTip(QString::fromUtf8("Esto verifica si la distancia mínima entre un taladro\ny se encuentra otro taladro."));
    tools2_checkrules_group_form_layout_1->setWidget(17, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_d2d_cb);
    tools2_checkrules_group_clearance_d2d_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_d2d_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_clearance_d2d_lbl->setToolTip(QString::fromUtf8("Tamaño mínimo aceptable de perforación."));
    tools2_checkrules_group_form_layout_1->setWidget(18, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_d2d_lbl);
    tools2_checkrules_group_clearance_d2d_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("value", 0.3);
    tools2_checkrules_group_form_layout_1->setWidget(18, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_d2d_entry);
    tools2_checkrules_group_drill_size_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_drill_size_cb->setChecked(true);
    tools2_checkrules_group_drill_size_cb->setText(QString::fromUtf8("Tamaño del Agujero:"));
    tools2_checkrules_group_drill_size_cb->setToolTip(QString::fromUtf8("Esto comprueba si los agujeros de perforación\nLos tamaños están por encima del umbral."));
    tools2_checkrules_group_form_layout_1->setWidget(19, QFormLayout::ItemRole(2), tools2_checkrules_group_drill_size_cb);
    tools2_checkrules_group_drill_size_lbl = new QLabel(nullptr);
    tools2_checkrules_group_drill_size_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    tools2_checkrules_group_drill_size_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    tools2_checkrules_group_form_layout_1->setWidget(20, QFormLayout::ItemRole(0), tools2_checkrules_group_drill_size_lbl);
    tools2_checkrules_group_drill_size_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_drill_size_entry->setProperty("decimals", 4);
    tools2_checkrules_group_drill_size_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_drill_size_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_drill_size_entry->setProperty("value", 0.3);
    tools2_checkrules_group_form_layout_1->setWidget(20, QFormLayout::ItemRole(1), tools2_checkrules_group_drill_size_entry);
    tools2_checkrules_group_layout->addLayout(tools2_checkrules_group_form_layout_1, 0);
    tools2_checkrules_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_checkrules_group->setLayout(tools2_checkrules_group_layout);
    vlay->addWidget(tools2_checkrules_group, 0);
    tools2_optimal_group = new QGroupBox(nullptr);
    { QFont f = tools2_optimal_group->font(); f.setBold(true); f.setPixelSize(16); tools2_optimal_group->setFont(f); }
    tools2_optimal_group->setMinimumSize(QSize(220, 0));
    tools2_optimal_group_layout = new QVBoxLayout();
    tools2_optimal_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_optimal_group_layout->setSpacing(6);
    tools2_optimal_group_optlabel = new QLabel(nullptr);
    tools2_optimal_group_optlabel->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_optimal_group_optlabel->setToolTip(QString::fromUtf8("Una herramienta para encontrar la distancia mínima entre\ncada dos elementos geométricos de Gerber"));
    tools2_optimal_group_layout->addWidget(tools2_optimal_group_optlabel, 0);
    auto* lay178 = new QGridLayout();
    lay178->setContentsMargins(0, 0, 0, 0);
    lay178->setHorizontalSpacing(6);
    lay178->setVerticalSpacing(6);
    tools2_optimal_group_precision_lbl = new QLabel(nullptr);
    tools2_optimal_group_precision_lbl->setText(QString::fromUtf8("Precisión:"));
    tools2_optimal_group_precision_lbl->setToolTip(QString::fromUtf8("Número de decimales para las distancias y coordenadas en esta herramienta."));
    lay178->addWidget(tools2_optimal_group_precision_lbl, 0, 0, 1, 1);
    tools2_optimal_group_precision_sp = new FCSpinner(nullptr);
    tools2_optimal_group_precision_sp->setProperty("maximum", 10);
    tools2_optimal_group_precision_sp->setProperty("minimum", 2);
    tools2_optimal_group_precision_sp->setProperty("value", 4);
    tools2_optimal_group_precision_sp->setWrapping(true);
    lay178->addWidget(tools2_optimal_group_precision_sp, 0, 1, 1, 1);
    lay178->setColumnStretch(1, 1);
    tools2_optimal_group_layout->addLayout(lay178, 0);
    tools2_optimal_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_optimal_group->setLayout(tools2_optimal_group_layout);
    vlay->addWidget(tools2_optimal_group, 0);
    layout->addLayout(vlay, 0);
    vlay1 = new QVBoxLayout();
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->setSpacing(6);
    tools2_qrcode_group = new QGroupBox(nullptr);
    { QFont f = tools2_qrcode_group->font(); f.setBold(true); f.setPixelSize(16); tools2_qrcode_group->setFont(f); }
    tools2_qrcode_group->setMinimumSize(QSize(220, 0));
    tools2_qrcode_group_layout = new QVBoxLayout();
    tools2_qrcode_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_qrcode_group_layout->setSpacing(6);
    tools2_qrcode_group_qrlabel = new QLabel(nullptr);
    tools2_qrcode_group_qrlabel->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_qrcode_group_qrlabel->setToolTip(QString::fromUtf8("Una herramienta para crear un QRCode que se puede insertar\nen un archivo Gerber seleccionado, o puede exportarse como un archivo."));
    tools2_qrcode_group_layout->addWidget(tools2_qrcode_group_qrlabel, 0);
    auto* lay179 = new QGridLayout();
    lay179->setContentsMargins(0, 0, 0, 0);
    lay179->setHorizontalSpacing(6);
    lay179->setVerticalSpacing(6);
    tools2_qrcode_group_version_label = new QLabel(nullptr);
    tools2_qrcode_group_version_label->setText(QString::fromUtf8("Versión:"));
    tools2_qrcode_group_version_label->setToolTip(QString::fromUtf8("La versión de QRCode puede tener valores de 1 (21x21 elementos)\na 40 (177x177 elementos)."));
    lay179->addWidget(tools2_qrcode_group_version_label, 1, 0, 1, 1);
    tools2_qrcode_group_version_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_version_entry->setProperty("maximum", 40);
    tools2_qrcode_group_version_entry->setProperty("minimum", 1);
    tools2_qrcode_group_version_entry->setProperty("value", 1);
    tools2_qrcode_group_version_entry->setWrapping(true);
    lay179->addWidget(tools2_qrcode_group_version_entry, 1, 1, 1, 1);
    tools2_qrcode_group_error_label = new QLabel(nullptr);
    tools2_qrcode_group_error_label->setText(QString::fromUtf8("Corrección de error:"));
    tools2_qrcode_group_error_label->setToolTip(QString::fromUtf8("Parámetro que controla la corrección de errores utilizada para el código QR.\nL = máximo 7 %% de errores pueden ser corregidos\nM = máximo 15%% de errores pueden ser corregidos\nQ = se puede corregir un máximo de 25%% de errores\nH = máximo 30 %% de errores pueden ser corregidos."));
    lay179->addWidget(tools2_qrcode_group_error_label, 2, 0, 1, 1);
    tools2_qrcode_group_error_radio = new RadioSet({{QString::fromUtf8("L"), QVariant(QString::fromUtf8("L"))}, {QString::fromUtf8("M"), QVariant(QString::fromUtf8("M"))}, {QString::fromUtf8("Q"), QVariant(QString::fromUtf8("Q"))}, {QString::fromUtf8("H"), QVariant(QString::fromUtf8("H"))}}, Qt::Horizontal, true, nullptr);
    tools2_qrcode_group_error_radio->setToolTip(QString::fromUtf8("Parámetro que controla la corrección de errores utilizada para el código QR.\nL = máximo 7 %% de errores pueden ser corregidos\nM = máximo 15%% de errores pueden ser corregidos\nQ = se puede corregir un máximo de 25%% de errores\nH = máximo 30 %% de errores pueden ser corregidos."));
    tools2_qrcode_group_error_radio->setValue(QVariant(QString::fromUtf8("L")));
    lay179->addWidget(tools2_qrcode_group_error_radio, 2, 1, 1, 1);
    tools2_qrcode_group_bsize_label = new QLabel(nullptr);
    tools2_qrcode_group_bsize_label->setText(QString::fromUtf8("Tamaño de Elementos:"));
    tools2_qrcode_group_bsize_label->setToolTip(QString::fromUtf8("El tamaño del elemento controla el tamaño general del código QR\najustando el tamaño de cada cuadro en el código."));
    lay179->addWidget(tools2_qrcode_group_bsize_label, 3, 0, 1, 1);
    tools2_qrcode_group_bsize_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_bsize_entry->setProperty("maximum", 9999);
    tools2_qrcode_group_bsize_entry->setProperty("minimum", 1);
    tools2_qrcode_group_bsize_entry->setProperty("value", 3);
    tools2_qrcode_group_bsize_entry->setWrapping(true);
    lay179->addWidget(tools2_qrcode_group_bsize_entry, 3, 1, 1, 1);
    tools2_qrcode_group_border_size_label = new QLabel(nullptr);
    tools2_qrcode_group_border_size_label->setText(QString::fromUtf8("Tamaño de borde:"));
    tools2_qrcode_group_border_size_label->setToolTip(QString::fromUtf8("Tamaño del borde del código QR. Cuántos elementos tiene el borde.\nEl valor predeterminado es 4. El ancho del espacio libre alrededor del Código QR."));
    lay179->addWidget(tools2_qrcode_group_border_size_label, 4, 0, 1, 1);
    tools2_qrcode_group_border_size_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_border_size_entry->setProperty("maximum", 9999);
    tools2_qrcode_group_border_size_entry->setProperty("minimum", 1);
    tools2_qrcode_group_border_size_entry->setProperty("value", 4);
    tools2_qrcode_group_border_size_entry->setWrapping(true);
    lay179->addWidget(tools2_qrcode_group_border_size_entry, 4, 1, 1, 1);
    tools2_qrcode_group_text_label = new QLabel(nullptr);
    tools2_qrcode_group_text_label->setText(QString::fromUtf8("Datos de QRCode:"));
    tools2_qrcode_group_text_label->setToolTip(QString::fromUtf8("Datos de QRCode. Texto alfanumérico a codificar en el Código QR."));
    lay179->addWidget(tools2_qrcode_group_text_label, 5, 0, 1, 1);
    tools2_qrcode_group_text_data = new FCTextArea(nullptr);
    tools2_qrcode_group_text_data->setProperty("placeholderText", QString::fromUtf8("Agregue aquí el texto que se incluirá en el QRCode ..."));
    lay179->addWidget(tools2_qrcode_group_text_data, 6, 0, 1, 2);
    tools2_qrcode_group_pol_label = new QLabel(nullptr);
    tools2_qrcode_group_pol_label->setText(QString::fromUtf8("Polaridad:"));
    tools2_qrcode_group_pol_label->setToolTip(QString::fromUtf8("Elija la polaridad del código QR.\nSe puede dibujar de forma negativa (los cuadrados son claros)\no de manera positiva (los cuadrados son opacos)."));
    lay179->addWidget(tools2_qrcode_group_pol_label, 7, 0, 1, 1);
    tools2_qrcode_group_pol_radio = new RadioSet({{QString::fromUtf8("Negativa"), QVariant(QString::fromUtf8("neg"))}, {QString::fromUtf8("Positivo"), QVariant(QString::fromUtf8("pos"))}}, Qt::Horizontal, true, nullptr);
    tools2_qrcode_group_pol_radio->setToolTip(QString::fromUtf8("Elija el tipo de QRCode que se creará.\nSi se agrega en un archivo de Silkscreen Gerber, el QRCode puede\nser agregado como positivo Si se agrega a un cobre Gerber\nentonces quizás el QRCode se pueda agregar como negativo."));
    tools2_qrcode_group_pol_radio->setValue(QVariant(QString::fromUtf8("pos")));
    lay179->addWidget(tools2_qrcode_group_pol_radio, 7, 1, 1, 1);
    tools2_qrcode_group_bb_label = new QLabel(nullptr);
    tools2_qrcode_group_bb_label->setText(QString::fromUtf8("Cuadro delimitador:"));
    tools2_qrcode_group_bb_label->setToolTip(QString::fromUtf8("El cuadro delimitador, que significa el espacio vacío que rodea\nLa geometría QRCode, puede tener una forma redondeada o cuadrada."));
    lay179->addWidget(tools2_qrcode_group_bb_label, 8, 0, 1, 1);
    tools2_qrcode_group_bb_radio = new RadioSet({{QString::fromUtf8("Redondeado"), QVariant(QString::fromUtf8("r"))}, {QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("s"))}}, Qt::Horizontal, true, nullptr);
    tools2_qrcode_group_bb_radio->setToolTip(QString::fromUtf8("El cuadro delimitador, que significa el espacio vacío que rodea\nLa geometría QRCode, puede tener una forma redondeada o cuadrada."));
    tools2_qrcode_group_bb_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay179->addWidget(tools2_qrcode_group_bb_radio, 8, 1, 1, 1);
    tools2_qrcode_group_fill_color_label = new QLabel(nullptr);
    tools2_qrcode_group_fill_color_label->setText(QString::fromUtf8("Color de relleno:"));
    tools2_qrcode_group_fill_color_label->setToolTip(QString::fromUtf8("Establezca el color de relleno del código QR (color de cuadrados / elementos)."));
    lay179->addWidget(tools2_qrcode_group_fill_color_label, 9, 0, 1, 1);
    tools2_qrcode_group_fill_color_entry = new FCColorEntry(nullptr);
    tools2_qrcode_group_fill_color_entry->setValue(QString::fromUtf8("#000000"));
    lay179->addWidget(tools2_qrcode_group_fill_color_entry, 9, 1, 1, 1);
    tools2_qrcode_group_back_color_label = new QLabel(nullptr);
    tools2_qrcode_group_back_color_label->setText(QString::fromUtf8("Color de fondo:"));
    tools2_qrcode_group_back_color_label->setToolTip(QString::fromUtf8("Establece el color de fondo del QRCode."));
    lay179->addWidget(tools2_qrcode_group_back_color_label, 10, 0, 1, 1);
    tools2_qrcode_group_back_color_entry = new FCColorEntry(nullptr);
    tools2_qrcode_group_back_color_entry->setValue(QString::fromUtf8("#FFFFFF"));
    lay179->addWidget(tools2_qrcode_group_back_color_entry, 10, 1, 1, 1);
    tools2_qrcode_group_sel_limit_label = new QLabel(nullptr);
    tools2_qrcode_group_sel_limit_label->setText(QString::fromUtf8("Límite de selección:"));
    tools2_qrcode_group_sel_limit_label->setToolTip(QString::fromUtf8("Establecer el número de geometría seleccionada\nelementos por encima de los cuales la geometría de utilidad\nse convierte en sólo un rectángulo de selección.\nAumenta el rendimiento al mover un\nGran cantidad de elementos geométricos."));
    lay179->addWidget(tools2_qrcode_group_sel_limit_label, 11, 0, 1, 1);
    tools2_qrcode_group_sel_limit_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_sel_limit_entry->setProperty("maximum", 9999);
    tools2_qrcode_group_sel_limit_entry->setProperty("value", 330);
    lay179->addWidget(tools2_qrcode_group_sel_limit_entry, 11, 1, 1, 1);
    lay179->setColumnStretch(1, 1);
    tools2_qrcode_group_layout->addLayout(lay179, 0);
    tools2_qrcode_group->setLayout(tools2_qrcode_group_layout);
    vlay1->addWidget(tools2_qrcode_group, 0);
    tools2_fiducials_group = new QGroupBox(nullptr);
    { QFont f = tools2_fiducials_group->font(); f.setBold(true); f.setPixelSize(16); tools2_fiducials_group->setFont(f); }
    tools2_fiducials_group->setMinimumSize(QSize(220, 0));
    tools2_fiducials_group_layout = new QVBoxLayout();
    tools2_fiducials_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_fiducials_group_layout->setSpacing(6);
    auto* lay180 = new QGridLayout();
    lay180->setContentsMargins(0, 0, 0, 0);
    lay180->setHorizontalSpacing(6);
    lay180->setVerticalSpacing(6);
    tools2_fiducials_group_param_label = new QLabel(nullptr);
    tools2_fiducials_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_fiducials_group_param_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay180->addWidget(tools2_fiducials_group_param_label, 0, 0, 1, 2);
    tools2_fiducials_group_dia_label = new QLabel(nullptr);
    tools2_fiducials_group_dia_label->setText(QString::fromUtf8("Tamaño:"));
    tools2_fiducials_group_dia_label->setToolTip(QString::fromUtf8("Esto establece el diámetro fiducial si el tipo fiducial es circular,\nde lo contrario es el tamaño del fiducial.\nLa apertura de la máscara de soldadura es el doble que eso."));
    lay180->addWidget(tools2_fiducials_group_dia_label, 1, 0, 1, 1);
    tools2_fiducials_group_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_fiducials_group_dia_entry->setProperty("decimals", 4);
    tools2_fiducials_group_dia_entry->setProperty("maximum", 3.0);
    tools2_fiducials_group_dia_entry->setProperty("minimum", 1.0);
    tools2_fiducials_group_dia_entry->setProperty("singleStep", 0.1);
    tools2_fiducials_group_dia_entry->setProperty("value", 1.0);
    tools2_fiducials_group_dia_entry->setWrapping(true);
    lay180->addWidget(tools2_fiducials_group_dia_entry, 1, 1, 1, 1);
    tools2_fiducials_group_margin_label = new QLabel(nullptr);
    tools2_fiducials_group_margin_label->setText(QString::fromUtf8("Margen:"));
    tools2_fiducials_group_margin_label->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay180->addWidget(tools2_fiducials_group_margin_label, 2, 0, 1, 1);
    tools2_fiducials_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_fiducials_group_margin_entry->setProperty("decimals", 4);
    tools2_fiducials_group_margin_entry->setProperty("maximum", 10000.0);
    tools2_fiducials_group_margin_entry->setProperty("minimum", -10000.0);
    tools2_fiducials_group_margin_entry->setProperty("singleStep", 0.1);
    tools2_fiducials_group_margin_entry->setProperty("value", 1.0);
    lay180->addWidget(tools2_fiducials_group_margin_entry, 2, 1, 1, 1);
    tools2_fiducials_group_mode_label = new QLabel(nullptr);
    tools2_fiducials_group_mode_label->setText(QString::fromUtf8("Modo:"));
    tools2_fiducials_group_mode_label->setToolTip(QString::fromUtf8("- 'Auto' - colocación automática de fiduciales en las esquinas del cuadro delimitador.\n- 'Manual' - colocación manual de fiduciales."));
    lay180->addWidget(tools2_fiducials_group_mode_label, 3, 0, 1, 1);
    tools2_fiducials_group_mode_radio = new RadioSet({{QString::fromUtf8("Auto"), QVariant(QString::fromUtf8("auto"))}, {QString::fromUtf8("Manual"), QVariant(QString::fromUtf8("manual"))}}, Qt::Horizontal, false, nullptr);
    tools2_fiducials_group_mode_radio->setValue(QVariant(QString::fromUtf8("auto")));
    lay180->addWidget(tools2_fiducials_group_mode_radio, 3, 1, 1, 1);
    tools2_fiducials_group_pos_label = new QLabel(nullptr);
    tools2_fiducials_group_pos_label->setText(QString::fromUtf8("Segundo fiducial:"));
    tools2_fiducials_group_pos_label->setToolTip(QString::fromUtf8("La posición para el segundo fiducial.\n- 'Arriba' - el orden es: abajo a la izquierda, arriba a la izquierda, arriba a la derecha.\n- 'Abajo' - el orden es: abajo a la izquierda, abajo a la derecha, arriba a la derecha.\n- 'Ninguno' - no hay un segundo fiducial. El orden es: abajo a la izquierda, arriba a la derecha."));
    lay180->addWidget(tools2_fiducials_group_pos_label, 4, 0, 1, 1);
    tools2_fiducials_group_pos_radio = new RadioSet({{QString::fromUtf8("Arriba"), QVariant(QString::fromUtf8("up"))}, {QString::fromUtf8("Abajo"), QVariant(QString::fromUtf8("down"))}, {QString::fromUtf8("Ninguno"), QVariant(QString::fromUtf8("no"))}}, Qt::Horizontal, false, nullptr);
    tools2_fiducials_group_pos_radio->setValue(QVariant(QString::fromUtf8("up")));
    lay180->addWidget(tools2_fiducials_group_pos_radio, 4, 1, 1, 1);
    auto* w181 = new QFrame(nullptr);
    w181->setProperty("frameShadow", 48);
    w181->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w181->setSizePolicy(sp); }
    lay180->addWidget(w181, 5, 0, 1, 2);
    tools2_fiducials_group_fid_type_label = new QLabel(nullptr);
    tools2_fiducials_group_fid_type_label->setText(QString::fromUtf8("Tipo fiducial:"));
    tools2_fiducials_group_fid_type_label->setToolTip(QString::fromUtf8("El tipo de fiducial.\n- 'Circular': este es el fiducial regular.\n- 'Cruce' - líneas cruzadas fiduciales.\n- 'Ajedrez' - patrón de ajedrez fiducial."));
    lay180->addWidget(tools2_fiducials_group_fid_type_label, 6, 0, 1, 1);
    tools2_fiducials_group_fid_type_radio = new RadioSet({{QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("circular"))}, {QString::fromUtf8("Cruce"), QVariant(QString::fromUtf8("cross"))}, {QString::fromUtf8("Ajedrez"), QVariant(QString::fromUtf8("chess"))}}, Qt::Horizontal, false, nullptr);
    tools2_fiducials_group_fid_type_radio->setValue(QVariant(QString::fromUtf8("circular")));
    lay180->addWidget(tools2_fiducials_group_fid_type_radio, 6, 1, 1, 1);
    tools2_fiducials_group_line_thickness_label = new QLabel(nullptr);
    tools2_fiducials_group_line_thickness_label->setText(QString::fromUtf8("Grosor de la línea:"));
    tools2_fiducials_group_line_thickness_label->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay180->addWidget(tools2_fiducials_group_line_thickness_label, 7, 0, 1, 1);
    tools2_fiducials_group_line_thickness_entry = new FCDoubleSpinner(nullptr);
    tools2_fiducials_group_line_thickness_entry->setProperty("decimals", 4);
    tools2_fiducials_group_line_thickness_entry->setProperty("maximum", 10000.0);
    tools2_fiducials_group_line_thickness_entry->setProperty("singleStep", 0.1);
    tools2_fiducials_group_line_thickness_entry->setProperty("value", 0.25);
    lay180->addWidget(tools2_fiducials_group_line_thickness_entry, 7, 1, 1, 1);
    lay180->setColumnStretch(1, 1);
    tools2_fiducials_group_layout->addLayout(lay180, 0);
    tools2_fiducials_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_fiducials_group->setLayout(tools2_fiducials_group_layout);
    vlay1->addWidget(tools2_fiducials_group, 0);
    layout->addLayout(vlay1, 0);
    vlay2 = new QVBoxLayout();
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->setSpacing(6);
    tools2_cfill_group = new QGroupBox(nullptr);
    { QFont f = tools2_cfill_group->font(); f.setBold(true); f.setPixelSize(16); tools2_cfill_group->setFont(f); }
    tools2_cfill_group->setMinimumSize(QSize(220, 0));
    tools2_cfill_group_layout = new QVBoxLayout();
    tools2_cfill_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_cfill_group_layout->setSpacing(6);
    auto* lay182 = new QGridLayout();
    lay182->setContentsMargins(0, 0, 0, 0);
    lay182->setHorizontalSpacing(6);
    lay182->setVerticalSpacing(6);
    tools2_cfill_group_cflabel = new FCLabel(nullptr);
    tools2_cfill_group_cflabel->setText(QString::fromUtf8("<b>Parámetros</b>"));
    tools2_cfill_group_cflabel->setToolTip(QString::fromUtf8("Una herramienta para generar un ladrón de cobre que se puede agregar\na un archivo Gerber seleccionado."));
    lay182->addWidget(tools2_cfill_group_cflabel, 0, 0, 1, 2);
    tools2_cfill_group_circle_steps_lbl = new FCLabel(nullptr);
    tools2_cfill_group_circle_steps_lbl->setText(QString::fromUtf8("Pasos del círculo:"));
    tools2_cfill_group_circle_steps_lbl->setToolTip(QString::fromUtf8("Número de pasos (líneas) utilizados para interpolar círculos."));
    lay182->addWidget(tools2_cfill_group_circle_steps_lbl, 2, 0, 1, 1);
    tools2_cfill_group_circlesteps_entry = new FCSpinner(nullptr);
    tools2_cfill_group_circlesteps_entry->setProperty("maximum", 9999);
    tools2_cfill_group_circlesteps_entry->setProperty("minimum", 1);
    tools2_cfill_group_circlesteps_entry->setProperty("value", 64);
    lay182->addWidget(tools2_cfill_group_circlesteps_entry, 2, 1, 1, 1);
    tools2_cfill_group_clearance_label = new FCLabel(nullptr);
    tools2_cfill_group_clearance_label->setText(QString::fromUtf8("Despeje:"));
    tools2_cfill_group_clearance_label->setToolTip(QString::fromUtf8("Esto establece la distancia entre los componentes de Copper Thieving\n(el relleno de polígono puede dividirse en múltiples polígonos)\ny las huellas de cobre en el archivo Gerber."));
    lay182->addWidget(tools2_cfill_group_clearance_label, 4, 0, 1, 1);
    tools2_cfill_group_clearance_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_clearance_entry->setProperty("decimals", 4);
    tools2_cfill_group_clearance_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_clearance_entry->setProperty("value", 0.25);
    lay182->addWidget(tools2_cfill_group_clearance_entry, 4, 1, 1, 1);
    tools2_cfill_group_margin_label = new FCLabel(nullptr);
    tools2_cfill_group_margin_label->setText(QString::fromUtf8("Margen:"));
    tools2_cfill_group_margin_label->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay182->addWidget(tools2_cfill_group_margin_label, 6, 0, 1, 1);
    tools2_cfill_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_margin_entry->setProperty("decimals", 4);
    tools2_cfill_group_margin_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_margin_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_margin_entry, 6, 1, 1, 1);
    tools2_cfill_group_area_label = new FCLabel(nullptr);
    tools2_cfill_group_area_label->setText(QString::fromUtf8("Zona:"));
    tools2_cfill_group_area_label->setToolTip(QString::fromUtf8("Robar áreas con un área menor a este valor no se agregará."));
    lay182->addWidget(tools2_cfill_group_area_label, 8, 0, 1, 1);
    tools2_cfill_group_area_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_area_entry->setProperty("decimals", 4);
    tools2_cfill_group_area_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_area_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_area_entry->setProperty("value", 0.1);
    lay182->addWidget(tools2_cfill_group_area_entry, 8, 1, 1, 1);
    tools2_cfill_group_reference_label = new FCLabel(nullptr);
    tools2_cfill_group_reference_label->setText(QString::fromUtf8("Referencia:"));
    tools2_cfill_group_reference_label->setToolTip(QString::fromUtf8("- 'Sí mismo': la extensión de Copper Thieving se basa en la extensión del objeto.\n- 'Selección de área': haga clic con el botón izquierdo del mouse para iniciar la selección del área a rellenar.\n- 'Objeto de referencia': 'Copper Thieving' dentro del área especificada por otro objeto."));
    lay182->addWidget(tools2_cfill_group_reference_label, 10, 0, 1, 1);
    tools2_cfill_group_reference_radio = new RadioSet({{QString::fromUtf8("Sí mismo"), QVariant(QString::fromUtf8("itself"))}, {QString::fromUtf8("Selección de área"), QVariant(QString::fromUtf8("area"))}, {QString::fromUtf8("Objeto de referencia"), QVariant(QString::fromUtf8("box"))}}, Qt::Vertical, false, nullptr);
    tools2_cfill_group_reference_radio->setValue(QVariant(QString::fromUtf8("itself")));
    lay182->addWidget(tools2_cfill_group_reference_radio, 10, 1, 1, 1);
    tools2_cfill_group_bbox_type_label = new FCLabel(nullptr);
    tools2_cfill_group_bbox_type_label->setText(QString::fromUtf8("Tipo de caja:"));
    tools2_cfill_group_bbox_type_label->setToolTip(QString::fromUtf8("- 'Rectangular': el cuadro delimitador tendrá forma rectangular.\n- 'Mínimo': el cuadro delimitador tendrá forma de casco convexo."));
    lay182->addWidget(tools2_cfill_group_bbox_type_label, 12, 0, 1, 1);
    tools2_cfill_group_bbox_type_radio = new RadioSet({{QString::fromUtf8("Rectangular"), QVariant(QString::fromUtf8("rect"))}, {QString::fromUtf8("Mínimo"), QVariant(QString::fromUtf8("min"))}}, Qt::Horizontal, false, nullptr);
    tools2_cfill_group_bbox_type_radio->setValue(QVariant(QString::fromUtf8("rect")));
    lay182->addWidget(tools2_cfill_group_bbox_type_radio, 12, 1, 1, 1);
    auto* w183 = new QFrame(nullptr);
    w183->setProperty("frameShadow", 48);
    w183->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w183->setSizePolicy(sp); }
    lay182->addWidget(w183, 14, 0, 1, 2);
    tools2_cfill_group_fill_type_label = new FCLabel(nullptr);
    tools2_cfill_group_fill_type_label->setText(QString::fromUtf8("Tipo de relleno:"));
    tools2_cfill_group_fill_type_label->setToolTip(QString::fromUtf8("- 'Sólido': el robo de cobre será un polígono sólido.\n- 'Cuadrícula de puntos': el área vacía se rellenará con un patrón de puntos.\n- 'Cuadrícula de cuadrados': el área vacía se rellenará con un patrón de cuadrados.\n- 'Cuadrícula de líneas': el área vacía se rellenará con un patrón de líneas."));
    lay182->addWidget(tools2_cfill_group_fill_type_label, 16, 0, 1, 1);
    tools2_cfill_group_fill_type_radio = new RadioSet({{QString::fromUtf8("Sólido"), QVariant(QString::fromUtf8("solid"))}, {QString::fromUtf8("Cuadrícula de puntos"), QVariant(QString::fromUtf8("dot"))}, {QString::fromUtf8("Cuadrícula de cuadrados"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Cuadrícula de líneas"), QVariant(QString::fromUtf8("line"))}}, Qt::Vertical, false, nullptr);
    tools2_cfill_group_fill_type_radio->setValue(QVariant(QString::fromUtf8("solid")));
    lay182->addWidget(tools2_cfill_group_fill_type_radio, 16, 1, 1, 1);
    tools2_cfill_group_dots_label = new FCLabel(nullptr);
    tools2_cfill_group_dots_label->setText(QString::fromUtf8("<b>Parámetros de cuadrícula de puntos</b>:"));
    lay182->addWidget(tools2_cfill_group_dots_label, 18, 0, 1, 2);
    tools2_cfill_group_dotdia_label = new FCLabel(nullptr);
    tools2_cfill_group_dotdia_label->setText(QString::fromUtf8("Dia:"));
    tools2_cfill_group_dotdia_label->setToolTip(QString::fromUtf8("Diámetro de punto en cuadrícula de puntos."));
    lay182->addWidget(tools2_cfill_group_dotdia_label, 20, 0, 1, 1);
    tools2_cfill_group_dot_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_dot_dia_entry->setProperty("decimals", 4);
    tools2_cfill_group_dot_dia_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_dot_dia_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_dot_dia_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_dot_dia_entry, 20, 1, 1, 1);
    tools2_cfill_group_dotspacing_label = new FCLabel(nullptr);
    tools2_cfill_group_dotspacing_label->setText(QString::fromUtf8("Spacing:"));
    tools2_cfill_group_dotspacing_label->setToolTip(QString::fromUtf8("Distancia entre cada dos puntos en la cuadrícula de puntos."));
    lay182->addWidget(tools2_cfill_group_dotspacing_label, 22, 0, 1, 1);
    tools2_cfill_group_dot_spacing_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_dot_spacing_entry->setProperty("decimals", 4);
    tools2_cfill_group_dot_spacing_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_dot_spacing_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_dot_spacing_entry->setProperty("value", 2.0);
    lay182->addWidget(tools2_cfill_group_dot_spacing_entry, 22, 1, 1, 1);
    tools2_cfill_group_squares_label = new FCLabel(nullptr);
    tools2_cfill_group_squares_label->setText(QString::fromUtf8("<b>Parámetros de la cuadrícula de cuadrados</b>:"));
    lay182->addWidget(tools2_cfill_group_squares_label, 24, 0, 1, 2);
    tools2_cfill_group_square_size_label = new FCLabel(nullptr);
    tools2_cfill_group_square_size_label->setText(QString::fromUtf8("Tamaño:"));
    tools2_cfill_group_square_size_label->setToolTip(QString::fromUtf8("Tamaño del lado cuadrado en cuadrícula de cuadrados."));
    lay182->addWidget(tools2_cfill_group_square_size_label, 26, 0, 1, 1);
    tools2_cfill_group_square_size_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_square_size_entry->setProperty("decimals", 4);
    tools2_cfill_group_square_size_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_square_size_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_square_size_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_square_size_entry, 26, 1, 1, 1);
    tools2_cfill_group_squares_spacing_label = new FCLabel(nullptr);
    tools2_cfill_group_squares_spacing_label->setText(QString::fromUtf8("Spacing:"));
    tools2_cfill_group_squares_spacing_label->setToolTip(QString::fromUtf8("Distancia entre cada dos cuadrados en la cuadrícula de cuadrados."));
    lay182->addWidget(tools2_cfill_group_squares_spacing_label, 28, 0, 1, 1);
    tools2_cfill_group_squares_spacing_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_squares_spacing_entry->setProperty("decimals", 4);
    tools2_cfill_group_squares_spacing_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_squares_spacing_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_squares_spacing_entry->setProperty("value", 2.0);
    lay182->addWidget(tools2_cfill_group_squares_spacing_entry, 28, 1, 1, 1);
    tools2_cfill_group_lines_label = new FCLabel(nullptr);
    tools2_cfill_group_lines_label->setText(QString::fromUtf8("<b>Parámetros de cuadrícula de líneas</b>:"));
    lay182->addWidget(tools2_cfill_group_lines_label, 30, 0, 1, 2);
    tools2_cfill_group_line_size_label = new FCLabel(nullptr);
    tools2_cfill_group_line_size_label->setText(QString::fromUtf8("Tamaño:"));
    tools2_cfill_group_line_size_label->setToolTip(QString::fromUtf8("Tamaño del grosor de línea en la cuadrícula de líneas."));
    lay182->addWidget(tools2_cfill_group_line_size_label, 32, 0, 1, 1);
    tools2_cfill_group_line_size_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_line_size_entry->setProperty("decimals", 4);
    tools2_cfill_group_line_size_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_line_size_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_line_size_entry->setProperty("value", 0.25);
    lay182->addWidget(tools2_cfill_group_line_size_entry, 32, 1, 1, 1);
    tools2_cfill_group_lines_spacing_label = new FCLabel(nullptr);
    tools2_cfill_group_lines_spacing_label->setText(QString::fromUtf8("Spacing:"));
    tools2_cfill_group_lines_spacing_label->setToolTip(QString::fromUtf8("Distancia entre cada dos líneas en la cuadrícula de líneas."));
    lay182->addWidget(tools2_cfill_group_lines_spacing_label, 34, 0, 1, 1);
    tools2_cfill_group_lines_spacing_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_lines_spacing_entry->setProperty("decimals", 4);
    tools2_cfill_group_lines_spacing_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_lines_spacing_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_lines_spacing_entry->setProperty("value", 2.0);
    lay182->addWidget(tools2_cfill_group_lines_spacing_entry, 34, 1, 1, 1);
    tools2_cfill_group_robber_bar_label = new FCLabel(nullptr);
    tools2_cfill_group_robber_bar_label->setText(QString::fromUtf8("<b>Parámetros de la Robber Bar</b>"));
    tools2_cfill_group_robber_bar_label->setToolTip(QString::fromUtf8("Parámetros utilizados para la Robber Bar.\nRobber Bar = borde de cobre para ayudar en el enchapado de agujeros."));
    lay182->addWidget(tools2_cfill_group_robber_bar_label, 36, 0, 1, 2);
    tools2_cfill_group_rb_margin_label = new FCLabel(nullptr);
    tools2_cfill_group_rb_margin_label->setText(QString::fromUtf8("Margen:"));
    tools2_cfill_group_rb_margin_label->setToolTip(QString::fromUtf8("Margen límite del recinto para Robber Bar."));
    lay182->addWidget(tools2_cfill_group_rb_margin_label, 38, 0, 1, 1);
    tools2_cfill_group_rb_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_rb_margin_entry->setProperty("decimals", 4);
    tools2_cfill_group_rb_margin_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_rb_margin_entry->setProperty("minimum", -10000.0);
    tools2_cfill_group_rb_margin_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_rb_margin_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_rb_margin_entry, 38, 1, 1, 1);
    tools2_cfill_group_rb_thickness_label = new FCLabel(nullptr);
    tools2_cfill_group_rb_thickness_label->setText(QString::fromUtf8("Espesor:"));
    tools2_cfill_group_rb_thickness_label->setToolTip(QString::fromUtf8("El grosor de la Robber Bar."));
    lay182->addWidget(tools2_cfill_group_rb_thickness_label, 40, 0, 1, 1);
    tools2_cfill_group_rb_thickness_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_rb_thickness_entry->setProperty("decimals", 4);
    tools2_cfill_group_rb_thickness_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_rb_thickness_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_rb_thickness_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_rb_thickness_entry, 40, 1, 1, 1);
    tools2_cfill_group_patern_mask_label = new FCLabel(nullptr);
    tools2_cfill_group_patern_mask_label->setText(QString::fromUtf8("<b>Máscara de baño de patrones</b>"));
    tools2_cfill_group_patern_mask_label->setToolTip(QString::fromUtf8("Genere una máscara para el enchapado de patrones."));
    lay182->addWidget(tools2_cfill_group_patern_mask_label, 42, 0, 1, 2);
    tools2_cfill_group_clearance_ppm_label = new FCLabel(nullptr);
    tools2_cfill_group_clearance_ppm_label->setText(QString::fromUtf8("Despeje:"));
    tools2_cfill_group_clearance_ppm_label->setToolTip(QString::fromUtf8("La distancia entre los posibles elementos de Copper Thieving.\ny / o Robber Bar y las aberturas reales en la máscara."));
    lay182->addWidget(tools2_cfill_group_clearance_ppm_label, 44, 0, 1, 1);
    tools2_cfill_group_clearance_ppm_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_clearance_ppm_entry->setProperty("decimals", 4);
    tools2_cfill_group_clearance_ppm_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_clearance_ppm_entry->setProperty("minimum", -10000.0);
    tools2_cfill_group_clearance_ppm_entry->setProperty("singleStep", 0.1);
    lay182->addWidget(tools2_cfill_group_clearance_ppm_entry, 44, 1, 1, 1);
    tools2_cfill_group_ppm_choice_label = new FCLabel(nullptr);
    tools2_cfill_group_ppm_choice_label->setText(QString::fromUtf8("Añadir:"));
    tools2_cfill_group_ppm_choice_label->setToolTip(QString::fromUtf8("Elija qué geometría adicional incluir, si está disponible."));
    lay182->addWidget(tools2_cfill_group_ppm_choice_label, 46, 0, 1, 1);
    tools2_cfill_group_ppm_choice_radio = new RadioSet({{QString::fromUtf8("Ambas"), QVariant(QString::fromUtf8("b"))}, {QString::fromUtf8("Ladrón"), QVariant(QString::fromUtf8("t"))}, {QString::fromUtf8("Barra de Ladrón"), QVariant(QString::fromUtf8("r"))}, {QString::fromUtf8("Ninguno"), QVariant(QString::fromUtf8("n"))}}, Qt::Vertical, false, nullptr);
    tools2_cfill_group_ppm_choice_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay182->addWidget(tools2_cfill_group_ppm_choice_radio, 46, 1, 1, 1);
    lay182->setColumnStretch(1, 1);
    tools2_cfill_group_layout->addLayout(lay182, 0);
    tools2_cfill_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_cfill_group->setLayout(tools2_cfill_group_layout);
    vlay2->addWidget(tools2_cfill_group, 0);
    layout->addLayout(vlay2, 0);
    vlay3 = new QVBoxLayout();
    vlay3->setContentsMargins(0, 0, 0, 0);
    vlay3->setSpacing(6);
    tools2_cal_group = new QGroupBox(nullptr);
    { QFont f = tools2_cal_group->font(); f.setBold(true); f.setPixelSize(16); tools2_cal_group->setFont(f); }
    tools2_cal_group->setMinimumSize(QSize(220, 0));
    tools2_cal_group_layout = new QVBoxLayout();
    tools2_cal_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_cal_group_layout->setSpacing(6);
    auto* lay184 = new QGridLayout();
    lay184->setContentsMargins(0, 0, 0, 0);
    lay184->setHorizontalSpacing(6);
    lay184->setVerticalSpacing(6);
    tools2_cal_group_param_label = new QLabel(nullptr);
    tools2_cal_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_cal_group_param_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay184->addWidget(tools2_cal_group_param_label, 0, 0, 1, 2);
    tools2_cal_group_cal_source_lbl = new QLabel(nullptr);
    tools2_cal_group_cal_source_lbl->setText(QString::fromUtf8("<b>Tipo de Fuente:</b>"));
    tools2_cal_group_cal_source_lbl->setToolTip(QString::fromUtf8("La fuente de los puntos de calibración.\nPuede ser:\n- Objeto -> haga clic en un agujero geo para Excellon o una almohadilla para Gerber\n- Libre -> haga clic libremente en el lienzo para adquirir los puntos de calibración"));
    lay184->addWidget(tools2_cal_group_cal_source_lbl, 1, 0, 1, 1);
    tools2_cal_group_cal_source_radio = new RadioSet({{QString::fromUtf8("Objeto"), QVariant(QString::fromUtf8("object"))}, {QString::fromUtf8("Libre"), QVariant(QString::fromUtf8("free"))}}, Qt::Horizontal, false, nullptr);
    tools2_cal_group_cal_source_radio->setValue(QVariant(QString::fromUtf8("object")));
    lay184->addWidget(tools2_cal_group_cal_source_radio, 1, 1, 1, 2);
    auto* w185 = new QFrame(nullptr);
    w185->setProperty("frameShadow", 48);
    w185->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w185->setSizePolicy(sp); }
    lay184->addWidget(w185, 2, 0, 1, 2);
    auto* w186 = new QLabel(nullptr);
    w186->setText(QString::fromUtf8("Viaje Z:"));
    w186->setToolTip(QString::fromUtf8("Altura (Z) para viajar entre los puntos."));
    lay184->addWidget(w186, 3, 0, 1, 1);
    tools2_cal_group_travelz_entry = new FCDoubleSpinner(nullptr);
    tools2_cal_group_travelz_entry->setProperty("decimals", 4);
    tools2_cal_group_travelz_entry->setProperty("maximum", 10000.0);
    tools2_cal_group_travelz_entry->setProperty("minimum", -10000.0);
    tools2_cal_group_travelz_entry->setProperty("singleStep", 0.1);
    tools2_cal_group_travelz_entry->setProperty("value", 2.0);
    lay184->addWidget(tools2_cal_group_travelz_entry, 3, 1, 1, 2);
    auto* w187 = new QLabel(nullptr);
    w187->setText(QString::fromUtf8("Verificación Z:"));
    w187->setToolTip(QString::fromUtf8("Altura (Z) para verificar el punto."));
    lay184->addWidget(w187, 4, 0, 1, 1);
    tools2_cal_group_verz_entry = new FCDoubleSpinner(nullptr);
    tools2_cal_group_verz_entry->setProperty("decimals", 4);
    tools2_cal_group_verz_entry->setProperty("maximum", 10000.0);
    tools2_cal_group_verz_entry->setProperty("minimum", -10000.0);
    tools2_cal_group_verz_entry->setProperty("singleStep", 0.1);
    tools2_cal_group_verz_entry->setProperty("value", 0.1);
    lay184->addWidget(tools2_cal_group_verz_entry, 4, 1, 1, 2);
    tools2_cal_group_zeroz_cb = new FCCheckBox(nullptr);
    tools2_cal_group_zeroz_cb->setText(QString::fromUtf8("Cero la Z para Herram."));
    tools2_cal_group_zeroz_cb->setToolTip(QString::fromUtf8("Incluya una secuencia para poner a cero la altura (Z)\nde la herramienta de verificación."));
    lay184->addWidget(tools2_cal_group_zeroz_cb, 5, 0, 1, 3);
    auto* w188 = new QLabel(nullptr);
    w188->setText(QString::fromUtf8("Cambio de herramienta Z:"));
    w188->setToolTip(QString::fromUtf8("Altura (Z) para montar la sonda de verificación."));
    lay184->addWidget(w188, 6, 0, 1, 1);
    tools2_cal_group_toolchangez_entry = new FCDoubleSpinner(nullptr);
    tools2_cal_group_toolchangez_entry->setProperty("decimals", 4);
    tools2_cal_group_toolchangez_entry->setProperty("maximum", 10000.0);
    tools2_cal_group_toolchangez_entry->setProperty("singleStep", 0.1);
    tools2_cal_group_toolchangez_entry->setProperty("value", 15.0);
    lay184->addWidget(tools2_cal_group_toolchangez_entry, 6, 1, 1, 2);
    auto* w189 = new QLabel(nullptr);
    w189->setText(QString::fromUtf8("Cambio de herra X, Y:"));
    w189->setToolTip(QString::fromUtf8("Posición de cambio de herramienta X, Y.\nSi no se ingresa ningún valor, entonces el actual\n(x, y) se utilizará el punto,"));
    lay184->addWidget(w189, 7, 0, 1, 1);
    tools2_cal_group_toolchange_xy_entry = new NumericalEvalTupleEntry(nullptr);
    tools2_cal_group_toolchange_xy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay184->addWidget(tools2_cal_group_toolchange_xy_entry, 7, 1, 1, 2);
    auto* w190 = new QLabel(nullptr);
    w190->setText(QString::fromUtf8("Segundo punto:"));
    w190->setToolTip(QString::fromUtf8("El segundo punto en la verificación de Gcode puede ser:\n- arriba a la izquierda -> el usuario alineará la PCB verticalmente\n- abajo a la derecha -> el usuario alineará la PCB horizontalmente"));
    lay184->addWidget(w190, 8, 0, 1, 1);
    tools2_cal_group_second_point_radio = new RadioSet({{QString::fromUtf8("Arriba a la izquierda"), QVariant(QString::fromUtf8("tl"))}, {QString::fromUtf8("Abajo a la derecha"), QVariant(QString::fromUtf8("br"))}}, Qt::Vertical, true, nullptr);
    tools2_cal_group_second_point_radio->setValue(QVariant(QString::fromUtf8("tl")));
    lay184->addWidget(tools2_cal_group_second_point_radio, 8, 1, 1, 2);
    lay184->setColumnStretch(1, 1);
    tools2_cal_group_layout->addLayout(lay184, 0);
    tools2_cal_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_cal_group->setLayout(tools2_cal_group_layout);
    vlay3->addWidget(tools2_cal_group, 0);
    tools2_edrills_group = new QGroupBox(nullptr);
    { QFont f = tools2_edrills_group->font(); f.setBold(true); f.setPixelSize(16); tools2_edrills_group->setFont(f); }
    tools2_edrills_group->setMinimumSize(QSize(220, 0));
    tools2_edrills_group_layout = new QVBoxLayout();
    tools2_edrills_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_edrills_group_layout->setSpacing(6);
    auto* lay191 = new QGridLayout();
    lay191->setContentsMargins(0, 0, 0, 0);
    lay191->setHorizontalSpacing(6);
    lay191->setVerticalSpacing(6);
    tools2_edrills_group_param_label = new QLabel(nullptr);
    tools2_edrills_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_edrills_group_param_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay191->addWidget(tools2_edrills_group_param_label, 0, 0, 1, 2);
    tools2_edrills_group_padt_label = new QLabel(nullptr);
    tools2_edrills_group_padt_label->setText(QString::fromUtf8("<b>Tipo de almohadillas procesadas:</b>"));
    tools2_edrills_group_padt_label->setToolTip(QString::fromUtf8("El tipo de forma de almohadillas que se procesará.\nSi la PCB tiene muchas almohadillas SMD con almohadillas rectangulares,\ndeshabilitar la apertura rectangular."));
    lay191->addWidget(tools2_edrills_group_padt_label, 2, 0, 1, 2);
    tools2_edrills_group_circular_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_circular_cb->setChecked(true);
    tools2_edrills_group_circular_cb->setText(QString::fromUtf8("Circular"));
    tools2_edrills_group_circular_cb->setToolTip(QString::fromUtf8("Proceso de Almohadillas Circulares."));
    lay191->addWidget(tools2_edrills_group_circular_cb, 3, 0, 1, 2);
    tools2_edrills_group_oblong_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_oblong_cb->setText(QString::fromUtf8("Oblongo"));
    tools2_edrills_group_oblong_cb->setToolTip(QString::fromUtf8("Procesar almohadillas oblongas."));
    lay191->addWidget(tools2_edrills_group_oblong_cb, 4, 0, 1, 2);
    tools2_edrills_group_square_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_square_cb->setText(QString::fromUtf8("Cuadrado"));
    tools2_edrills_group_square_cb->setToolTip(QString::fromUtf8("Procesar almohadillas cuadradas."));
    lay191->addWidget(tools2_edrills_group_square_cb, 5, 0, 1, 2);
    tools2_edrills_group_rectangular_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_rectangular_cb->setText(QString::fromUtf8("Rectangular"));
    tools2_edrills_group_rectangular_cb->setToolTip(QString::fromUtf8("Proceso Almohadillas Rectangulares."));
    lay191->addWidget(tools2_edrills_group_rectangular_cb, 6, 0, 1, 2);
    tools2_edrills_group_other_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_other_cb->setText(QString::fromUtf8("Otros"));
    tools2_edrills_group_other_cb->setToolTip(QString::fromUtf8("Procese los pads no en las categorías anteriores."));
    lay191->addWidget(tools2_edrills_group_other_cb, 7, 0, 1, 2);
    auto* w192 = new QFrame(nullptr);
    w192->setProperty("frameShadow", 48);
    w192->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w192->setSizePolicy(sp); }
    lay191->addWidget(w192, 8, 0, 1, 2);
    tools2_edrills_group_hole_size_label = new QLabel(nullptr);
    tools2_edrills_group_hole_size_label->setText(QString::fromUtf8("<b>Método:</b>"));
    tools2_edrills_group_hole_size_label->setToolTip(QString::fromUtf8("El método para procesar almohadillas. Puede ser:\n- Diámetro fijo -> todos los agujeros tendrán un tamaño establecido\n- Anillo anular fijo -> todos los agujeros tendrán un anillo anular establecido\n- Proporcional -> cada tamaño de agujero será una fracción del tamaño de la almohadilla"));
    lay191->addWidget(tools2_edrills_group_hole_size_label, 9, 0, 1, 1);
    tools2_edrills_group_hole_size_radio = new RadioSet({{QString::fromUtf8("Diámetro fijo"), QVariant(QString::fromUtf8("fixed"))}, {QString::fromUtf8("Anillo anular fijo"), QVariant(QString::fromUtf8("ring"))}, {QString::fromUtf8("Proporcional"), QVariant(QString::fromUtf8("prop"))}}, Qt::Vertical, false, nullptr);
    tools2_edrills_group_hole_size_radio->setValue(QVariant(QString::fromUtf8("fixed")));
    lay191->addWidget(tools2_edrills_group_hole_size_radio, 9, 1, 1, 1);
    auto* w193 = new QFrame(nullptr);
    w193->setProperty("frameShadow", 48);
    w193->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w193->setSizePolicy(sp); }
    lay191->addWidget(w193, 10, 0, 1, 2);
    tools2_edrills_group_fixed_label = new QLabel(nullptr);
    tools2_edrills_group_fixed_label->setText(QString::fromUtf8("<b>Diámetro fijo</b>"));
    lay191->addWidget(tools2_edrills_group_fixed_label, 11, 0, 1, 2);
    tools2_edrills_group_dia_label = new QLabel(nullptr);
    tools2_edrills_group_dia_label->setText(QString::fromUtf8("Valor:"));
    tools2_edrills_group_dia_label->setToolTip(QString::fromUtf8("Diámetro fijo del agujero."));
    lay191->addWidget(tools2_edrills_group_dia_label, 12, 0, 1, 1);
    tools2_edrills_group_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_dia_entry->setProperty("decimals", 4);
    tools2_edrills_group_dia_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_dia_entry->setProperty("value", 0.5);
    lay191->addWidget(tools2_edrills_group_dia_entry, 12, 1, 1, 1);
    tools2_edrills_group_ring_label = new QLabel(nullptr);
    tools2_edrills_group_ring_label->setText(QString::fromUtf8("<b>Anillo anular fijo</b>"));
    tools2_edrills_group_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular.\nLa astilla de cobre entre el exterior del agujero\ny el margen de la almohadilla de cobre."));
    lay191->addWidget(tools2_edrills_group_ring_label, 13, 0, 1, 2);
    tools2_edrills_group_circular_ring_label = new QLabel(nullptr);
    tools2_edrills_group_circular_ring_label->setText(QString::fromUtf8("Circular:"));
    tools2_edrills_group_circular_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas circulares."));
    lay191->addWidget(tools2_edrills_group_circular_ring_label, 14, 0, 1, 1);
    tools2_edrills_group_circular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_circular_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_circular_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_circular_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_circular_ring_entry, 14, 1, 1, 1);
    tools2_edrills_group_oblong_ring_label = new QLabel(nullptr);
    tools2_edrills_group_oblong_ring_label->setText(QString::fromUtf8("Oblongo:"));
    tools2_edrills_group_oblong_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas oblongas."));
    lay191->addWidget(tools2_edrills_group_oblong_ring_label, 15, 0, 1, 1);
    tools2_edrills_group_oblong_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_oblong_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_oblong_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_oblong_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_oblong_ring_entry, 15, 1, 1, 1);
    tools2_edrills_group_square_ring_label = new QLabel(nullptr);
    tools2_edrills_group_square_ring_label->setText(QString::fromUtf8("Cuadrado:"));
    tools2_edrills_group_square_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas cuadradas."));
    lay191->addWidget(tools2_edrills_group_square_ring_label, 16, 0, 1, 1);
    tools2_edrills_group_square_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_square_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_square_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_square_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_square_ring_entry, 16, 1, 1, 1);
    tools2_edrills_group_rectangular_ring_label = new QLabel(nullptr);
    tools2_edrills_group_rectangular_ring_label->setText(QString::fromUtf8("Rectangular:"));
    tools2_edrills_group_rectangular_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas rectangulares."));
    lay191->addWidget(tools2_edrills_group_rectangular_ring_label, 17, 0, 1, 1);
    tools2_edrills_group_rectangular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_rectangular_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_rectangular_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_rectangular_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_rectangular_ring_entry, 17, 1, 1, 1);
    tools2_edrills_group_other_ring_label = new QLabel(nullptr);
    tools2_edrills_group_other_ring_label->setText(QString::fromUtf8("Otros:"));
    tools2_edrills_group_other_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para otras almohadillas."));
    lay191->addWidget(tools2_edrills_group_other_ring_label, 18, 0, 1, 1);
    tools2_edrills_group_other_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_other_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_other_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_other_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_other_ring_entry, 18, 1, 1, 1);
    tools2_edrills_group_prop_label = new QLabel(nullptr);
    tools2_edrills_group_prop_label->setText(QString::fromUtf8("<b>Diá. proporcional</b>"));
    lay191->addWidget(tools2_edrills_group_prop_label, 19, 0, 1, 2);
    tools2_edrills_group_factor_label = new QLabel(nullptr);
    tools2_edrills_group_factor_label->setText(QString::fromUtf8("Factor:"));
    tools2_edrills_group_factor_label->setToolTip(QString::fromUtf8("Diámetro proporcional.\nEl diámetro del agujero será una fracción del tamaño de la almohadilla."));
    lay191->addWidget(tools2_edrills_group_factor_label, 20, 0, 1, 1);
    tools2_edrills_group_factor_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_factor_entry->setProperty("decimals", 4);
    tools2_edrills_group_factor_entry->setProperty("maximum", 100.0);
    tools2_edrills_group_factor_entry->setProperty("singleStep", 0.1);
    tools2_edrills_group_factor_entry->setProperty("suffix", QString::fromUtf8(" %"));
    tools2_edrills_group_factor_entry->setProperty("value", 80.0);
    lay191->addWidget(tools2_edrills_group_factor_entry, 20, 1, 1, 1);
    lay191->setColumnStretch(1, 1);
    tools2_edrills_group_layout->addLayout(lay191, 0);
    tools2_edrills_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_edrills_group->setLayout(tools2_edrills_group_layout);
    vlay3->addWidget(tools2_edrills_group, 0);
    layout->addLayout(vlay3, 0);
    vlay4 = new QVBoxLayout();
    vlay4->setContentsMargins(0, 0, 0, 0);
    vlay4->setSpacing(6);
    tools2_punch_group = new QGroupBox(nullptr);
    { QFont f = tools2_punch_group->font(); f.setBold(true); f.setPixelSize(16); tools2_punch_group->setFont(f); }
    tools2_punch_group->setMinimumSize(QSize(220, 0));
    tools2_punch_group_layout = new QVBoxLayout();
    tools2_punch_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_punch_group_layout->setSpacing(6);
    auto* lay194 = new QGridLayout();
    lay194->setContentsMargins(0, 0, 0, 0);
    lay194->setHorizontalSpacing(6);
    lay194->setVerticalSpacing(6);
    tools2_punch_group_param_label = new QLabel(nullptr);
    tools2_punch_group_param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_punch_group_param_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay194->addWidget(tools2_punch_group_param_label, 0, 0, 1, 2);
    tools2_punch_group_padt_label = new QLabel(nullptr);
    tools2_punch_group_padt_label->setText(QString::fromUtf8("<b>Tipo de almohadillas procesadas:</b>"));
    tools2_punch_group_padt_label->setToolTip(QString::fromUtf8("El tipo de forma de almohadillas que se procesará.\nSi la PCB tiene muchas almohadillas SMD con almohadillas rectangulares,\ndeshabilitar la apertura rectangular."));
    lay194->addWidget(tools2_punch_group_padt_label, 2, 0, 1, 2);
    tools2_punch_group_circular_cb = new FCCheckBox(nullptr);
    tools2_punch_group_circular_cb->setChecked(true);
    tools2_punch_group_circular_cb->setText(QString::fromUtf8("Circular"));
    tools2_punch_group_circular_cb->setToolTip(QString::fromUtf8("Proceso de Almohadillas Circulares."));
    lay194->addWidget(tools2_punch_group_circular_cb, 3, 0, 1, 2);
    tools2_punch_group_oblong_cb = new FCCheckBox(nullptr);
    tools2_punch_group_oblong_cb->setText(QString::fromUtf8("Oblongo"));
    tools2_punch_group_oblong_cb->setToolTip(QString::fromUtf8("Procesar almohadillas oblongas."));
    lay194->addWidget(tools2_punch_group_oblong_cb, 4, 0, 1, 2);
    tools2_punch_group_square_cb = new FCCheckBox(nullptr);
    tools2_punch_group_square_cb->setChecked(true);
    tools2_punch_group_square_cb->setText(QString::fromUtf8("Cuadrado"));
    tools2_punch_group_square_cb->setToolTip(QString::fromUtf8("Procesar almohadillas cuadradas."));
    lay194->addWidget(tools2_punch_group_square_cb, 5, 0, 1, 2);
    tools2_punch_group_rectangular_cb = new FCCheckBox(nullptr);
    tools2_punch_group_rectangular_cb->setText(QString::fromUtf8("Rectangular"));
    tools2_punch_group_rectangular_cb->setToolTip(QString::fromUtf8("Proceso Almohadillas Rectangulares."));
    lay194->addWidget(tools2_punch_group_rectangular_cb, 6, 0, 1, 2);
    tools2_punch_group_other_cb = new FCCheckBox(nullptr);
    tools2_punch_group_other_cb->setText(QString::fromUtf8("Otros"));
    tools2_punch_group_other_cb->setToolTip(QString::fromUtf8("Procese los pads no en las categorías anteriores."));
    lay194->addWidget(tools2_punch_group_other_cb, 7, 0, 1, 2);
    auto* w195 = new QFrame(nullptr);
    w195->setProperty("frameShadow", 48);
    w195->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w195->setSizePolicy(sp); }
    lay194->addWidget(w195, 8, 0, 1, 2);
    tools2_punch_group_hole_size_label = new QLabel(nullptr);
    tools2_punch_group_hole_size_label->setText(QString::fromUtf8("<b>Método:</b>"));
    tools2_punch_group_hole_size_label->setToolTip(QString::fromUtf8("La fuente del orificio de perforación puede ser:\n- Objeto Excellon-> el centro de perforación de objetos Excellon servirá como referencia.\n- Diámetro fijo -> intentará usar el centro de las almohadillas como referencia agregando agujeros de diámetro fijo.\n- Anillo anular fijo -> intentará mantener un anillo anular establecido.\n- Proporcional -> hará un orificio de perforación Gerber con un diámetro del porcentaje del diámetro de la almohadilla."));
    lay194->addWidget(tools2_punch_group_hole_size_label, 9, 0, 1, 1);
    tools2_punch_group_hole_size_radio = new RadioSet({{QString::fromUtf8("Excellon"), QVariant(QString::fromUtf8("exc"))}, {QString::fromUtf8("Diámetro fijo"), QVariant(QString::fromUtf8("fixed"))}, {QString::fromUtf8("Anillo anular fijo"), QVariant(QString::fromUtf8("ring"))}, {QString::fromUtf8("Proporcional"), QVariant(QString::fromUtf8("prop"))}}, Qt::Vertical, false, nullptr);
    tools2_punch_group_hole_size_radio->setValue(QVariant(QString::fromUtf8("exc")));
    lay194->addWidget(tools2_punch_group_hole_size_radio, 9, 1, 1, 1);
    auto* w196 = new QFrame(nullptr);
    w196->setProperty("frameShadow", 48);
    w196->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w196->setSizePolicy(sp); }
    lay194->addWidget(w196, 10, 0, 1, 2);
    tools2_punch_group_fixed_label = new QLabel(nullptr);
    tools2_punch_group_fixed_label->setText(QString::fromUtf8("<b>Diámetro fijo</b>"));
    lay194->addWidget(tools2_punch_group_fixed_label, 11, 0, 1, 2);
    tools2_punch_group_dia_label = new QLabel(nullptr);
    tools2_punch_group_dia_label->setText(QString::fromUtf8("Valor:"));
    tools2_punch_group_dia_label->setToolTip(QString::fromUtf8("Diámetro fijo del agujero."));
    lay194->addWidget(tools2_punch_group_dia_label, 12, 0, 1, 1);
    tools2_punch_group_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_dia_entry->setProperty("decimals", 4);
    tools2_punch_group_dia_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_dia_entry->setProperty("value", 0.5);
    lay194->addWidget(tools2_punch_group_dia_entry, 12, 1, 1, 1);
    tools2_punch_group_ring_label = new QLabel(nullptr);
    tools2_punch_group_ring_label->setText(QString::fromUtf8("<b>Anillo anular fijo</b>"));
    tools2_punch_group_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular.\nLa astilla de cobre entre el exterior del agujero\ny el margen de la almohadilla de cobre."));
    lay194->addWidget(tools2_punch_group_ring_label, 13, 0, 1, 2);
    tools2_punch_group_circular_ring_label = new QLabel(nullptr);
    tools2_punch_group_circular_ring_label->setText(QString::fromUtf8("Circular:"));
    tools2_punch_group_circular_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas circulares."));
    lay194->addWidget(tools2_punch_group_circular_ring_label, 14, 0, 1, 1);
    tools2_punch_group_circular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_circular_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_circular_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_circular_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_circular_ring_entry, 14, 1, 1, 1);
    tools2_punch_group_oblong_ring_label = new QLabel(nullptr);
    tools2_punch_group_oblong_ring_label->setText(QString::fromUtf8("Oblongo:"));
    tools2_punch_group_oblong_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas oblongas."));
    lay194->addWidget(tools2_punch_group_oblong_ring_label, 15, 0, 1, 1);
    tools2_punch_group_oblong_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_oblong_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_oblong_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_oblong_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_oblong_ring_entry, 15, 1, 1, 1);
    tools2_punch_group_square_ring_label = new QLabel(nullptr);
    tools2_punch_group_square_ring_label->setText(QString::fromUtf8("Cuadrado:"));
    tools2_punch_group_square_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas cuadradas."));
    lay194->addWidget(tools2_punch_group_square_ring_label, 16, 0, 1, 1);
    tools2_punch_group_square_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_square_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_square_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_square_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_square_ring_entry, 16, 1, 1, 1);
    tools2_punch_group_rectangular_ring_label = new QLabel(nullptr);
    tools2_punch_group_rectangular_ring_label->setText(QString::fromUtf8("Rectangular:"));
    tools2_punch_group_rectangular_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para almohadillas rectangulares."));
    lay194->addWidget(tools2_punch_group_rectangular_ring_label, 17, 0, 1, 1);
    tools2_punch_group_rectangular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_rectangular_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_rectangular_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_rectangular_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_rectangular_ring_entry, 17, 1, 1, 1);
    tools2_punch_group_other_ring_label = new QLabel(nullptr);
    tools2_punch_group_other_ring_label->setText(QString::fromUtf8("Otros:"));
    tools2_punch_group_other_ring_label->setToolTip(QString::fromUtf8("El tamaño del anillo anular para otras almohadillas."));
    lay194->addWidget(tools2_punch_group_other_ring_label, 18, 0, 1, 1);
    tools2_punch_group_other_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_other_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_other_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_other_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_other_ring_entry, 18, 1, 1, 1);
    tools2_punch_group_prop_label = new QLabel(nullptr);
    tools2_punch_group_prop_label->setText(QString::fromUtf8("<b>Diá. proporcional</b>"));
    lay194->addWidget(tools2_punch_group_prop_label, 19, 0, 1, 2);
    tools2_punch_group_factor_label = new QLabel(nullptr);
    tools2_punch_group_factor_label->setText(QString::fromUtf8("Factor:"));
    tools2_punch_group_factor_label->setToolTip(QString::fromUtf8("Diámetro proporcional.\nEl diámetro del agujero será una fracción del tamaño de la almohadilla."));
    lay194->addWidget(tools2_punch_group_factor_label, 20, 0, 1, 1);
    tools2_punch_group_factor_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_factor_entry->setProperty("decimals", 4);
    tools2_punch_group_factor_entry->setProperty("maximum", 100.0);
    tools2_punch_group_factor_entry->setProperty("singleStep", 0.1);
    tools2_punch_group_factor_entry->setProperty("suffix", QString::fromUtf8(" %"));
    tools2_punch_group_factor_entry->setProperty("value", 80.0);
    lay194->addWidget(tools2_punch_group_factor_entry, 20, 1, 1, 1);
    lay194->setColumnStretch(1, 1);
    tools2_punch_group_layout->addLayout(lay194, 0);
    tools2_punch_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_punch_group->setLayout(tools2_punch_group_layout);
    vlay4->addWidget(tools2_punch_group, 0);
    tools2_invert_group = new QGroupBox(nullptr);
    { QFont f = tools2_invert_group->font(); f.setBold(true); f.setPixelSize(16); tools2_invert_group->setFont(f); }
    tools2_invert_group->setMinimumSize(QSize(220, 0));
    tools2_invert_group_layout = new QVBoxLayout();
    tools2_invert_group_layout->setContentsMargins(9, 9, 9, 9);
    tools2_invert_group_layout->setSpacing(6);
    tools2_invert_group_sublabel = new QLabel(nullptr);
    tools2_invert_group_sublabel->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    tools2_invert_group_sublabel->setToolTip(QString::fromUtf8("Una herramienta para invertir la geometría de Gerber de positivo a negativo\ny a la inversa."));
    tools2_invert_group_layout->addWidget(tools2_invert_group_sublabel, 0);
    auto* lay197 = new QGridLayout();
    lay197->setContentsMargins(0, 0, 0, 0);
    lay197->setHorizontalSpacing(6);
    lay197->setVerticalSpacing(6);
    tools2_invert_group_margin_label = new QLabel(nullptr);
    tools2_invert_group_margin_label->setText(QString::fromUtf8("Margen:"));
    tools2_invert_group_margin_label->setToolTip(QString::fromUtf8("Distancia por la cual evitar\nLos bordes del objeto Gerber."));
    lay197->addWidget(tools2_invert_group_margin_label, 2, 0, 1, 2);
    tools2_invert_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_invert_group_margin_entry->setProperty("decimals", 4);
    tools2_invert_group_margin_entry->setProperty("maximum", 10000.0);
    tools2_invert_group_margin_entry->setProperty("objectName", QString::fromUtf8("Margen"));
    tools2_invert_group_margin_entry->setProperty("value", 0.1);
    lay197->addWidget(tools2_invert_group_margin_entry, 3, 0, 1, 2);
    tools2_invert_group_join_label = new QLabel(nullptr);
    tools2_invert_group_join_label->setText(QString::fromUtf8("Estilo de unión de líneas:"));
    tools2_invert_group_join_label->setToolTip(QString::fromUtf8("La forma en que se unirán las líneas en el contorno del objeto.\nPuede ser:\n- redondeado -> se agrega un arco entre dos líneas de unión\n- cuadrado -> las líneas se encuentran en un ángulo de 90 grados\n- bisel -> las líneas están unidas por una tercera línea"));
    lay197->addWidget(tools2_invert_group_join_label, 5, 0, 1, 2);
    tools2_invert_group_join_radio = new RadioSet({{QString::fromUtf8("Redondeado"), QVariant(QString::fromUtf8("r"))}, {QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("s"))}, {QString::fromUtf8("Bisel"), QVariant(QString::fromUtf8("b"))}}, Qt::Vertical, false, nullptr);
    tools2_invert_group_join_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay197->addWidget(tools2_invert_group_join_radio, 7, 0, 1, 2);
    lay197->setColumnStretch(1, 1);
    tools2_invert_group_layout->addLayout(lay197, 0);
    tools2_invert_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    tools2_invert_group->setLayout(tools2_invert_group_layout);
    vlay4->addWidget(tools2_invert_group, 0);
    layout->addLayout(vlay4, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    tools2_defaults_form->setLayout(layout);
    tools2_scroll_area->setWidget(tools2_defaults_form);
    tools2_tab_lay->addWidget(tools2_scroll_area, 0);
    tools2_tab->setLayout(tools2_tab_lay);
    pref_tab_area->addTab(tools2_tab, QString::fromUtf8("HERRAMIENTAS 2"));
    fa_tab = new QWidget(nullptr);
    fa_tab->setProperty("objectName", QString::fromUtf8("fa_tab"));
    fa_tab->setHidden(true);
    fa_tab_lay = new QVBoxLayout();
    fa_tab_lay->setContentsMargins(2, 2, 2, 2);
    fa_tab_lay->setSpacing(6);
    fa_scroll_area = new QScrollArea(nullptr);
    util_defaults_form = new QWidget(nullptr);
    util_defaults_form->setAutoFillBackground(true);
    layout = new QHBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    vlay = new QVBoxLayout();
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(6);
    fa_excellon_group = new QGroupBox(nullptr);
    { QFont f = fa_excellon_group->font(); f.setBold(true); f.setPixelSize(16); fa_excellon_group->setFont(f); }
    fa_excellon_group->setMinimumSize(QSize(260, 0));
    fa_excellon_group_layout = new QVBoxLayout();
    fa_excellon_group_layout->setContentsMargins(2, 2, 2, 2);
    fa_excellon_group_layout->setSpacing(6);
    auto* w198 = new VerticalScrollArea(nullptr);
    w198->setProperty("frameShape", 0);
    auto* w199 = new QWidget(nullptr);
    w199->setAutoFillBackground(true);
    fa_excellon_group_vertical_lay = new QVBoxLayout();
    fa_excellon_group_vertical_lay->setContentsMargins(9, 9, 9, 9);
    fa_excellon_group_vertical_lay->setSpacing(6);
    auto* lay200 = new QHBoxLayout();
    lay200->setContentsMargins(0, 0, 0, 0);
    lay200->setSpacing(6);
    fa_excellon_group_restore_btn = new FCButton(nullptr);
    fa_excellon_group_restore_btn->setText(QString::fromUtf8("Restaurar"));
    fa_excellon_group_restore_btn->setToolTip(QString::fromUtf8("Restaurar la lista de extensiones al estado predeterminado."));
    lay200->addWidget(fa_excellon_group_restore_btn, 0);
    fa_excellon_group_del_all_btn = new FCButton(nullptr);
    fa_excellon_group_del_all_btn->setText(QString::fromUtf8("Eliminar todosEliminar taladro"));
    fa_excellon_group_del_all_btn->setToolTip(QString::fromUtf8("Eliminar todas las extensiones de la lista."));
    lay200->addWidget(fa_excellon_group_del_all_btn, 0);
    fa_excellon_group_vertical_lay->addLayout(lay200, 0);
    auto* w201 = new QLabel(nullptr);
    w201->setText(QString::fromUtf8("<b>Lista de extensiones:</b>"));
    w201->setToolTip(QString::fromUtf8("Lista de extensiones de archivo para ser\nasociado con FlatCAM."));
    fa_excellon_group_vertical_lay->addWidget(w201, 0);
    fa_excellon_group_exc_list_text = new FCTextArea(nullptr);
    { QFont f = fa_excellon_group_exc_list_text->font(); f.setPointSizeF(10.0); fa_excellon_group_exc_list_text->setFont(f); }
    fa_excellon_group_exc_list_text->setProperty("plainText", QString::fromUtf8("drd, drill, drl, exc, ncd, tap, xln"));
    fa_excellon_group_exc_list_text->setReadOnly(true);
    fa_excellon_group_exc_list_text->setProperty("textInteractionFlags", 1);
    fa_excellon_group_vertical_lay->addWidget(fa_excellon_group_exc_list_text, 0);
    auto* lay202 = new QHBoxLayout();
    lay202->setContentsMargins(0, 0, 0, 0);
    lay202->setSpacing(6);
    fa_excellon_group_ext_label = new QLabel(nullptr);
    fa_excellon_group_ext_label->setText(QString::fromUtf8("ExtensiónLista de extensiones:"));
    fa_excellon_group_ext_label->setToolTip(QString::fromUtf8("Una extensión de archivo para agregar o eliminar a la lista."));
    lay202->addWidget(fa_excellon_group_ext_label, 0);
    fa_excellon_group_ext_entry = new FCEntry(nullptr);
    lay202->addWidget(fa_excellon_group_ext_entry, 0);
    fa_excellon_group_vertical_lay->addLayout(lay202, 0);
    auto* lay203 = new QHBoxLayout();
    lay203->setContentsMargins(0, 0, 0, 0);
    lay203->setSpacing(6);
    fa_excellon_group_add_btn = new FCButton(nullptr);
    fa_excellon_group_add_btn->setText(QString::fromUtf8("Agregar extensión"));
    fa_excellon_group_add_btn->setToolTip(QString::fromUtf8("Agregar una extensión de archivo a la lista"));
    lay203->addWidget(fa_excellon_group_add_btn, 0);
    fa_excellon_group_del_btn = new FCButton(nullptr);
    fa_excellon_group_del_btn->setText(QString::fromUtf8("Eliminar extensión"));
    fa_excellon_group_del_btn->setToolTip(QString::fromUtf8("Eliminar una extensión de archivo de la lista"));
    lay203->addWidget(fa_excellon_group_del_btn, 0);
    fa_excellon_group_vertical_lay->addLayout(lay203, 0);
    fa_excellon_group_exc_list_btn = new FCButton(nullptr);
    fa_excellon_group_exc_list_btn->setText(QString::fromUtf8("Aplicar asociación"));
    fa_excellon_group_exc_list_btn->setToolTip(QString::fromUtf8("Aplicar las asociaciones de archivos entre\nFlatCAM y los archivos con las extensiones anteriores.\nEstarán activos después del próximo inicio de sesión.\nEsto funciona solo en Windows."));
    fa_excellon_group_vertical_lay->addWidget(fa_excellon_group_exc_list_btn, 0);
    w199->setLayout(fa_excellon_group_vertical_lay);
    w198->setWidget(w199);
    fa_excellon_group_layout->addWidget(w198, 0);
    fa_excellon_group->setLayout(fa_excellon_group_layout);
    vlay->addWidget(fa_excellon_group, 0);
    fa_gcode_group = new QGroupBox(nullptr);
    { QFont f = fa_gcode_group->font(); f.setBold(true); f.setPixelSize(16); fa_gcode_group->setFont(f); }
    fa_gcode_group->setMinimumSize(QSize(260, 0));
    fa_gcode_group_layout = new QVBoxLayout();
    fa_gcode_group_layout->setContentsMargins(9, 9, 9, 9);
    fa_gcode_group_layout->setSpacing(6);
    auto* lay204 = new QHBoxLayout();
    lay204->setContentsMargins(0, 0, 0, 0);
    lay204->setSpacing(6);
    fa_gcode_group_restore_btn = new FCButton(nullptr);
    fa_gcode_group_restore_btn->setText(QString::fromUtf8("Restaurar"));
    fa_gcode_group_restore_btn->setToolTip(QString::fromUtf8("Restaurar la lista de extensiones al estado predeterminado."));
    lay204->addWidget(fa_gcode_group_restore_btn, 0);
    fa_gcode_group_del_all_btn = new FCButton(nullptr);
    fa_gcode_group_del_all_btn->setText(QString::fromUtf8("Eliminar todosEliminar taladro"));
    fa_gcode_group_del_all_btn->setToolTip(QString::fromUtf8("Eliminar todas las extensiones de la lista."));
    lay204->addWidget(fa_gcode_group_del_all_btn, 0);
    fa_gcode_group_layout->addLayout(lay204, 0);
    fa_gcode_group_gco_list_label = new QLabel(nullptr);
    fa_gcode_group_gco_list_label->setText(QString::fromUtf8("<b>Lista de extensiones:</b>"));
    fa_gcode_group_gco_list_label->setToolTip(QString::fromUtf8("Lista de extensiones de archivo para ser\nasociado con FlatCAM."));
    fa_gcode_group_layout->addWidget(fa_gcode_group_gco_list_label, 0);
    fa_gcode_group_gco_list_text = new FCTextArea(nullptr);
    { QFont f = fa_gcode_group_gco_list_text->font(); f.setPointSizeF(10.0); fa_gcode_group_gco_list_text->setFont(f); }
    fa_gcode_group_gco_list_text->setProperty("plainText", QString::fromUtf8("cnc, din, dnc, ecs, eia, fan, fgc, fnc, gc, gcd, gcode, h, hnc, i, min, mpf, mpr, nc, ncc, ncg, ncp, ngc, out, ply, rol, sbp, tap, xpi"));
    fa_gcode_group_gco_list_text->setReadOnly(true);
    fa_gcode_group_gco_list_text->setProperty("textInteractionFlags", 1);
    fa_gcode_group_layout->addWidget(fa_gcode_group_gco_list_text, 0);
    auto* lay205 = new QHBoxLayout();
    lay205->setContentsMargins(0, 0, 0, 0);
    lay205->setSpacing(6);
    fa_gcode_group_ext_label = new QLabel(nullptr);
    fa_gcode_group_ext_label->setText(QString::fromUtf8("ExtensiónLista de extensiones:"));
    fa_gcode_group_ext_label->setToolTip(QString::fromUtf8("Una extensión de archivo para agregar o eliminar a la lista."));
    lay205->addWidget(fa_gcode_group_ext_label, 0);
    fa_gcode_group_ext_entry = new FCEntry(nullptr);
    lay205->addWidget(fa_gcode_group_ext_entry, 0);
    fa_gcode_group_layout->addLayout(lay205, 0);
    auto* lay206 = new QHBoxLayout();
    lay206->setContentsMargins(0, 0, 0, 0);
    lay206->setSpacing(6);
    fa_gcode_group_add_btn = new FCButton(nullptr);
    fa_gcode_group_add_btn->setText(QString::fromUtf8("Agregar extensión"));
    fa_gcode_group_add_btn->setToolTip(QString::fromUtf8("Agregar una extensión de archivo a la lista"));
    lay206->addWidget(fa_gcode_group_add_btn, 0);
    fa_gcode_group_del_btn = new FCButton(nullptr);
    fa_gcode_group_del_btn->setText(QString::fromUtf8("Eliminar extensión"));
    fa_gcode_group_del_btn->setToolTip(QString::fromUtf8("Eliminar una extensión de archivo de la lista"));
    lay206->addWidget(fa_gcode_group_del_btn, 0);
    fa_gcode_group_layout->addLayout(lay206, 0);
    fa_gcode_group_gco_list_btn = new FCButton(nullptr);
    fa_gcode_group_gco_list_btn->setText(QString::fromUtf8("Aplicar asociación"));
    fa_gcode_group_gco_list_btn->setToolTip(QString::fromUtf8("Aplicar las asociaciones de archivos entre\nFlatCAM y los archivos con las extensiones anteriores.\nEstarán activos después del próximo inicio de sesión.\nEsto funciona solo en Windows."));
    fa_gcode_group_layout->addWidget(fa_gcode_group_gco_list_btn, 0);
    fa_gcode_group->setLayout(fa_gcode_group_layout);
    vlay->addWidget(fa_gcode_group, 0);
    layout->addLayout(vlay, 0);
    fa_gerber_group = new QGroupBox(nullptr);
    { QFont f = fa_gerber_group->font(); f.setBold(true); f.setPixelSize(16); fa_gerber_group->setFont(f); }
    fa_gerber_group->setMinimumSize(QSize(260, 0));
    fa_gerber_group_layout = new QVBoxLayout();
    fa_gerber_group_layout->setContentsMargins(9, 9, 9, 9);
    fa_gerber_group_layout->setSpacing(6);
    auto* lay207 = new QHBoxLayout();
    lay207->setContentsMargins(0, 0, 0, 0);
    lay207->setSpacing(6);
    fa_gerber_group_restore_btn = new FCButton(nullptr);
    fa_gerber_group_restore_btn->setText(QString::fromUtf8("Restaurar"));
    fa_gerber_group_restore_btn->setToolTip(QString::fromUtf8("Restaurar la lista de extensiones al estado predeterminado."));
    lay207->addWidget(fa_gerber_group_restore_btn, 0);
    fa_gerber_group_del_all_btn = new FCButton(nullptr);
    fa_gerber_group_del_all_btn->setText(QString::fromUtf8("Eliminar todosEliminar taladro"));
    fa_gerber_group_del_all_btn->setToolTip(QString::fromUtf8("Eliminar todas las extensiones de la lista."));
    lay207->addWidget(fa_gerber_group_del_all_btn, 0);
    fa_gerber_group_layout->addLayout(lay207, 0);
    fa_gerber_group_grb_list_label = new QLabel(nullptr);
    fa_gerber_group_grb_list_label->setText(QString::fromUtf8("<b>Lista de extensiones:</b>"));
    fa_gerber_group_grb_list_label->setToolTip(QString::fromUtf8("Lista de extensiones de archivo para ser\nasociado con FlatCAM."));
    fa_gerber_group_layout->addWidget(fa_gerber_group_grb_list_label, 0);
    fa_gerber_group_grb_list_text = new FCTextArea(nullptr);
    { QFont f = fa_gerber_group_grb_list_text->font(); f.setPointSizeF(10.0); fa_gerber_group_grb_list_text->setFont(f); }
    fa_gerber_group_grb_list_text->setProperty("plainText", QString::fromUtf8("art, bot, bsm, cmp, crc, crs, dim, gb0, gb1, gb2, gb3, gb4, gb5, gb6, gb7, gb8, gb9, gbd, gbl, gbo, gbp, gbr, gbs, gdo, ger, gko, gm1, gm2, gm3, grb, gtl, gto, gtp, gts, ly15, ly2, mil, outline, pho, plc, pls, smb, smt, sol, spb, spt, ssb, sst, stc, sts, top, tsm"));
    fa_gerber_group_grb_list_text->setReadOnly(true);
    fa_gerber_group_grb_list_text->setProperty("textInteractionFlags", 1);
    fa_gerber_group_layout->addWidget(fa_gerber_group_grb_list_text, 0);
    auto* lay208 = new QHBoxLayout();
    lay208->setContentsMargins(0, 0, 0, 0);
    lay208->setSpacing(6);
    fa_gerber_group_ext_label = new QLabel(nullptr);
    fa_gerber_group_ext_label->setText(QString::fromUtf8("ExtensiónLista de extensiones:"));
    fa_gerber_group_ext_label->setToolTip(QString::fromUtf8("Una extensión de archivo para agregar o eliminar a la lista."));
    lay208->addWidget(fa_gerber_group_ext_label, 0);
    fa_gerber_group_ext_entry = new FCEntry(nullptr);
    lay208->addWidget(fa_gerber_group_ext_entry, 0);
    fa_gerber_group_layout->addLayout(lay208, 0);
    auto* lay209 = new QHBoxLayout();
    lay209->setContentsMargins(0, 0, 0, 0);
    lay209->setSpacing(6);
    fa_gerber_group_add_btn = new FCButton(nullptr);
    fa_gerber_group_add_btn->setText(QString::fromUtf8("Agregar extensión"));
    fa_gerber_group_add_btn->setToolTip(QString::fromUtf8("Agregar una extensión de archivo a la lista"));
    lay209->addWidget(fa_gerber_group_add_btn, 0);
    fa_gerber_group_del_btn = new FCButton(nullptr);
    fa_gerber_group_del_btn->setText(QString::fromUtf8("Eliminar extensión"));
    fa_gerber_group_del_btn->setToolTip(QString::fromUtf8("Eliminar una extensión de archivo de la lista"));
    lay209->addWidget(fa_gerber_group_del_btn, 0);
    fa_gerber_group_layout->addLayout(lay209, 0);
    fa_gerber_group_grb_list_btn = new FCButton(nullptr);
    fa_gerber_group_grb_list_btn->setText(QString::fromUtf8("Aplicar asociación"));
    fa_gerber_group_grb_list_btn->setToolTip(QString::fromUtf8("Aplicar las asociaciones de archivos entre\nFlatCAM y los archivos con las extensiones anteriores.\nEstarán activos después del próximo inicio de sesión.\nEsto funciona solo en Windows."));
    fa_gerber_group_layout->addWidget(fa_gerber_group_grb_list_btn, 0);
    fa_gerber_group->setLayout(fa_gerber_group_layout);
    layout->addWidget(fa_gerber_group, 0);
    kw_group = new QGroupBox(nullptr);
    { QFont f = kw_group->font(); f.setBold(true); f.setPixelSize(16); kw_group->setFont(f); }
    kw_group->setMinimumSize(QSize(260, 0));
    kw_group_layout = new QVBoxLayout();
    kw_group_layout->setContentsMargins(9, 9, 9, 9);
    kw_group_layout->setSpacing(6);
    auto* lay210 = new QHBoxLayout();
    lay210->setContentsMargins(0, 0, 0, 0);
    lay210->setSpacing(6);
    kw_group_restore_btn = new FCButton(nullptr);
    kw_group_restore_btn->setText(QString::fromUtf8("Restaurar"));
    kw_group_restore_btn->setToolTip(QString::fromUtf8("Restaure la lista de palabras clave de autocompletador al estado predeterminado."));
    lay210->addWidget(kw_group_restore_btn, 0);
    kw_group_del_all_btn = new FCButton(nullptr);
    kw_group_del_all_btn->setText(QString::fromUtf8("Eliminar todosEliminar taladro"));
    kw_group_del_all_btn->setToolTip(QString::fromUtf8("Elimine todas las palabras clave de autocompletador de la lista."));
    lay210->addWidget(kw_group_del_all_btn, 0);
    kw_group_layout->addLayout(lay210, 0);
    kw_group_grb_list_label = new QLabel(nullptr);
    kw_group_grb_list_label->setText(QString::fromUtf8("<b>Lista de palabras clave:</b>"));
    kw_group_grb_list_label->setToolTip(QString::fromUtf8("Lista de palabras clave utilizadas por\nel autocompletador en FlatCAM.\nEl autocompletador está instalado\nen el Editor de Código y para el Tcl Shell."));
    kw_group_layout->addWidget(kw_group_grb_list_label, 0);
    kw_group_kw_list_text = new FCTextArea(nullptr);
    { QFont f = kw_group_kw_list_text->font(); f.setPointSizeF(10.0); kw_group_kw_list_text->setFont(f); }
    kw_group_kw_list_text->setProperty("plainText", QString::fromUtf8("Desktop, Documents, FlatConfig, FlatPrj, False, Marius, My Documents, Paste_1, Repetier, Roland_MDX_20, True, Users, Toolchange_Custom, Toolchange_Probe_MACH3, Toolchange_manual, Users, all, axis, auto, axisoffset, box, center_x, center_y, columns, combine, connect, contour, default, depthperpass, dia, diatol, dist, drilled_dias, drillz, dpp, dwelltime, endxy, endz, extracut_length, f, feedrate, feedrate_z, grbl_11, GRBL_laser, gridoffsety, gridx, gridy, has_offset, holes, hpgl, iso_type, line_xyz, margin, marlin, method, milled_dias, minoffset, name, offset, opt_type, order, outname, overlap, passes, postamble, pp, ppname_e, ppname_g, preamble, radius, ref, rest, rows, shellvar_, scale_factor, spacing_columns, spacing_rows, spindlespeed, startz, startxy, toolchange_xy, toolchangez, tooldia, travelz, use_threads, value, x, x0, x1, y, y0, y1, z_cut, z_move"));
    kw_group_kw_list_text->setReadOnly(true);
    kw_group_kw_list_text->setProperty("textInteractionFlags", 1);
    kw_group_layout->addWidget(kw_group_kw_list_text, 0);
    auto* lay211 = new QHBoxLayout();
    lay211->setContentsMargins(0, 0, 0, 0);
    lay211->setSpacing(6);
    kw_group_kw_label = new QLabel(nullptr);
    kw_group_kw_label->setText(QString::fromUtf8("ExtensiónLista de extensiones:"));
    kw_group_kw_label->setToolTip(QString::fromUtf8("Una palabra clave para agregar o eliminar a la lista."));
    lay211->addWidget(kw_group_kw_label, 0);
    kw_group_kw_entry = new FCEntry(nullptr);
    lay211->addWidget(kw_group_kw_entry, 0);
    kw_group_layout->addLayout(lay211, 0);
    auto* lay212 = new QHBoxLayout();
    lay212->setContentsMargins(0, 0, 0, 0);
    lay212->setSpacing(6);
    kw_group_add_btn = new FCButton(nullptr);
    kw_group_add_btn->setText(QString::fromUtf8("Agregar palabra clave"));
    kw_group_add_btn->setToolTip(QString::fromUtf8("Agregar una palabra clave a la lista"));
    lay212->addWidget(kw_group_add_btn, 0);
    kw_group_del_btn = new FCButton(nullptr);
    kw_group_del_btn->setText(QString::fromUtf8("Eliminar palabra clave"));
    kw_group_del_btn->setToolTip(QString::fromUtf8("Eliminar una palabra clave de la lista"));
    lay212->addWidget(kw_group_del_btn, 0);
    kw_group_layout->addLayout(lay212, 0);
    kw_group->setLayout(kw_group_layout);
    layout->addWidget(kw_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    util_defaults_form->setLayout(layout);
    fa_scroll_area->setWidget(util_defaults_form);
    fa_tab_lay->addWidget(fa_scroll_area, 0);
    fa_tab->setLayout(fa_tab_lay);
    pref_tab_area->addTab(fa_tab, QString::fromUtf8("UTILIDADES"));
    pref_tab_layout->addWidget(pref_tab_area, 0);
    pref_tab_bottom_layout = new QHBoxLayout();
    pref_tab_bottom_layout->setContentsMargins(0, 0, 0, 0);
    pref_tab_bottom_layout->setSpacing(6);
    pref_tab_bottom_layout_1 = new QHBoxLayout();
    pref_tab_bottom_layout_1->setContentsMargins(0, 0, 0, 0);
    pref_tab_bottom_layout_1->setSpacing(6);
    pref_defaults_button = new FCButton(nullptr);
    pref_defaults_button->setIcon(QIcon(QString::fromUtf8(":/images/restore32.png")));
    pref_defaults_button->setMinimumSize(QSize(130, 0));
    pref_defaults_button->setText(QString::fromUtf8("Restaurar los valores predeterminados"));
    pref_defaults_button->setToolTip(QString::fromUtf8("Restaurar todo el conjunto de valores predeterminados\na los valores iniciales cargados después del primer lanzamiento."));
    pref_tab_bottom_layout_1->addWidget(pref_defaults_button, 0);
    pref_open_button = new FCButton(nullptr);
    pref_open_button->setIcon(QIcon(QString::fromUtf8(":/images/pref.png")));
    pref_open_button->setMinimumSize(QSize(130, 0));
    pref_open_button->setText(QString::fromUtf8("Abrir Carpeta de Pref"));
    pref_open_button->setToolTip(QString::fromUtf8("Abra la carpeta donde FlatCAM guarda los archivos de preferencias."));
    pref_tab_bottom_layout_1->addWidget(pref_open_button, 0);
    clear_btn = new FCButton(nullptr);
    clear_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    clear_btn->setMinimumSize(QSize(130, 0));
    clear_btn->setText(QString::fromUtf8("Borrar la configuración de la GUI"));
    clear_btn->setToolTip(QString::fromUtf8("Borrar la configuración de la GUI para FlatCAM,\ntales como: diseño, estado gui, estilo, soporte hdpi etc."));
    pref_tab_bottom_layout_1->addWidget(clear_btn, 0);
    pref_tab_bottom_layout->addLayout(pref_tab_bottom_layout_1, 0);
    pref_tab_bottom_layout_2 = new QHBoxLayout();
    pref_tab_bottom_layout_2->setContentsMargins(0, 0, 0, 0);
    pref_tab_bottom_layout_2->setSpacing(6);
    pref_apply_button = new FCButton(nullptr);
    pref_apply_button->setIcon(QIcon(QString::fromUtf8(":/images/apply32.png")));
    pref_apply_button->setMinimumSize(QSize(130, 0));
    pref_apply_button->setText(QString::fromUtf8("Aplicar"));
    pref_apply_button->setToolTip(QString::fromUtf8("Aplique las preferencias actuales sin guardar en un archivo."));
    pref_tab_bottom_layout_2->addWidget(pref_apply_button, 0);
    pref_save_button = new FCButton(nullptr);
    pref_save_button->setIcon(QIcon(QString::fromUtf8(":/images/save_as.png")));
    pref_save_button->setMinimumSize(QSize(130, 0));
    pref_save_button->setText(QString::fromUtf8("Salvar"));
    pref_save_button->setToolTip(QString::fromUtf8("Guarde la configuración actual en el archivo 'current_defaults'\nque es el archivo que almacena las preferencias predeterminadas de trabajo."));
    pref_tab_bottom_layout_2->addWidget(pref_save_button, 0);
    pref_close_button = new FCButton(nullptr);
    pref_close_button->setMinimumSize(QSize(130, 0));
    pref_close_button->setText(QString::fromUtf8("Cancelar"));
    pref_close_button->setToolTip(QString::fromUtf8("No guardará los cambios y cerrará la ventana de preferencias."));
    pref_tab_bottom_layout_2->addWidget(pref_close_button, 0);
    pref_tab_bottom_layout->addLayout(pref_tab_bottom_layout_2, 0);
    pref_tab_layout->addLayout(pref_tab_bottom_layout, 0);
    setLayout(pref_tab_layout);
}

std::map<std::string, QObject*> PreferencesTabUIBase::membersByName() const {
    return {
        {"clear_btn", clear_btn},
        {"cncjob_adv_opt_group", cncjob_adv_opt_group},
        {"cncjob_adv_opt_group_al_columns_entry", cncjob_adv_opt_group_al_columns_entry},
        {"cncjob_adv_opt_group_al_columns_label", cncjob_adv_opt_group_al_columns_label},
        {"cncjob_adv_opt_group_al_controller_combo", cncjob_adv_opt_group_al_controller_combo},
        {"cncjob_adv_opt_group_al_controller_label", cncjob_adv_opt_group_al_controller_label},
        {"cncjob_adv_opt_group_al_method_lbl", cncjob_adv_opt_group_al_method_lbl},
        {"cncjob_adv_opt_group_al_method_radio", cncjob_adv_opt_group_al_method_radio},
        {"cncjob_adv_opt_group_al_mode_radio", cncjob_adv_opt_group_al_mode_radio},
        {"cncjob_adv_opt_group_al_rows_entry", cncjob_adv_opt_group_al_rows_entry},
        {"cncjob_adv_opt_group_al_rows_label", cncjob_adv_opt_group_al_rows_label},
        {"cncjob_adv_opt_group_annotation_color_label", cncjob_adv_opt_group_annotation_color_label},
        {"cncjob_adv_opt_group_annotation_fontcolor_entry", cncjob_adv_opt_group_annotation_fontcolor_entry},
        {"cncjob_adv_opt_group_annotation_fontsize_label", cncjob_adv_opt_group_annotation_fontsize_label},
        {"cncjob_adv_opt_group_annotation_fontsize_sp", cncjob_adv_opt_group_annotation_fontsize_sp},
        {"cncjob_adv_opt_group_autolevelling_gcode_label", cncjob_adv_opt_group_autolevelling_gcode_label},
        {"cncjob_adv_opt_group_export_gcode_label", cncjob_adv_opt_group_export_gcode_label},
        {"cncjob_adv_opt_group_feedrate_probe_entry", cncjob_adv_opt_group_feedrate_probe_entry},
        {"cncjob_adv_opt_group_feedrate_probe_label", cncjob_adv_opt_group_feedrate_probe_label},
        {"cncjob_adv_opt_group_jog_fr_entry", cncjob_adv_opt_group_jog_fr_entry},
        {"cncjob_adv_opt_group_jog_fr_label", cncjob_adv_opt_group_jog_fr_label},
        {"cncjob_adv_opt_group_jog_step_entry", cncjob_adv_opt_group_jog_step_entry},
        {"cncjob_adv_opt_group_jog_step_label", cncjob_adv_opt_group_jog_step_label},
        {"cncjob_adv_opt_group_jog_travelz_entry", cncjob_adv_opt_group_jog_travelz_entry},
        {"cncjob_adv_opt_group_jog_travelz_label", cncjob_adv_opt_group_jog_travelz_label},
        {"cncjob_adv_opt_group_layout", cncjob_adv_opt_group_layout},
        {"cncjob_adv_opt_group_pdepth_entry", cncjob_adv_opt_group_pdepth_entry},
        {"cncjob_adv_opt_group_pdepth_label", cncjob_adv_opt_group_pdepth_label},
        {"cncjob_adv_opt_group_ptravelz_entry", cncjob_adv_opt_group_ptravelz_entry},
        {"cncjob_adv_opt_group_ptravelz_label", cncjob_adv_opt_group_ptravelz_label},
        {"cncjob_defaults_form", cncjob_defaults_form},
        {"cncjob_editor_group", cncjob_editor_group},
        {"cncjob_editor_group_append_text", cncjob_editor_group_append_text},
        {"cncjob_editor_group_layout", cncjob_editor_group_layout},
        {"cncjob_editor_group_param_label", cncjob_editor_group_param_label},
        {"cncjob_editor_group_prepend_text", cncjob_editor_group_prepend_text},
        {"cncjob_gen_group", cncjob_gen_group},
        {"cncjob_gen_group_cnc_color_label", cncjob_gen_group_cnc_color_label},
        {"cncjob_gen_group_cncjob_alpha_entry", cncjob_gen_group_cncjob_alpha_entry},
        {"cncjob_gen_group_cncjob_alpha_label", cncjob_gen_group_cncjob_alpha_label},
        {"cncjob_gen_group_coords_dec_entry", cncjob_gen_group_coords_dec_entry},
        {"cncjob_gen_group_coords_type_radio", cncjob_gen_group_coords_type_radio},
        {"cncjob_gen_group_fill_color_entry", cncjob_gen_group_fill_color_entry},
        {"cncjob_gen_group_fill_color_label", cncjob_gen_group_fill_color_label},
        {"cncjob_gen_group_fr_dec_entry", cncjob_gen_group_fr_dec_entry},
        {"cncjob_gen_group_layout", cncjob_gen_group_layout},
        {"cncjob_gen_group_line_color_entry", cncjob_gen_group_line_color_entry},
        {"cncjob_gen_group_line_color_label", cncjob_gen_group_line_color_label},
        {"cncjob_gen_group_line_ending_cb", cncjob_gen_group_line_ending_cb},
        {"cncjob_gen_group_plot_cb", cncjob_gen_group_plot_cb},
        {"cncjob_gen_group_plot_options_label", cncjob_gen_group_plot_options_label},
        {"cncjob_gen_group_steps_per_circle_entry", cncjob_gen_group_steps_per_circle_entry},
        {"cncjob_gen_group_steps_per_circle_label", cncjob_gen_group_steps_per_circle_label},
        {"cncjob_gen_group_tfill_color_entry", cncjob_gen_group_tfill_color_entry},
        {"cncjob_gen_group_tfill_color_label", cncjob_gen_group_tfill_color_label},
        {"cncjob_gen_group_tline_color_entry", cncjob_gen_group_tline_color_entry},
        {"cncjob_gen_group_tline_color_label", cncjob_gen_group_tline_color_label},
        {"cncjob_gen_group_tooldia_entry", cncjob_gen_group_tooldia_entry},
        {"cncjob_gen_group_travel_color_label", cncjob_gen_group_travel_color_label},
        {"cncjob_opt_group", cncjob_opt_group},
        {"cncjob_opt_group_annotation_cb", cncjob_opt_group_annotation_cb},
        {"cncjob_opt_group_cncplot_method_label", cncjob_opt_group_cncplot_method_label},
        {"cncjob_opt_group_cncplot_method_radio", cncjob_opt_group_cncplot_method_radio},
        {"cncjob_opt_group_export_gcode_label", cncjob_opt_group_export_gcode_label},
        {"cncjob_opt_group_layout", cncjob_opt_group_layout},
        {"cncjob_scroll_area", cncjob_scroll_area},
        {"cncjob_tab_lay", cncjob_tab_lay},
        {"excellon_adv_opt_group", excellon_adv_opt_group},
        {"excellon_adv_opt_group_autoload_db_cb", excellon_adv_opt_group_autoload_db_cb},
        {"excellon_adv_opt_group_exc_label", excellon_adv_opt_group_exc_label},
        {"excellon_adv_opt_group_layout", excellon_adv_opt_group_layout},
        {"excellon_adv_opt_group_table_visibility_cb", excellon_adv_opt_group_table_visibility_cb},
        {"excellon_defaults_form", excellon_defaults_form},
        {"excellon_editor_group", excellon_editor_group},
        {"excellon_editor_group_addtool_entry", excellon_editor_group_addtool_entry},
        {"excellon_editor_group_addtool_entry_lbl", excellon_editor_group_addtool_entry_lbl},
        {"excellon_editor_group_drill_angle_entry", excellon_editor_group_drill_angle_entry},
        {"excellon_editor_group_drill_angle_label", excellon_editor_group_drill_angle_label},
        {"excellon_editor_group_drill_array_circ_label", excellon_editor_group_drill_array_circ_label},
        {"excellon_editor_group_drill_array_linear_label", excellon_editor_group_drill_array_linear_label},
        {"excellon_editor_group_drill_array_size_entry", excellon_editor_group_drill_array_size_entry},
        {"excellon_editor_group_drill_array_size_label", excellon_editor_group_drill_array_size_label},
        {"excellon_editor_group_drill_axis_label", excellon_editor_group_drill_axis_label},
        {"excellon_editor_group_drill_axis_radio", excellon_editor_group_drill_axis_radio},
        {"excellon_editor_group_drill_circular_angle_entry", excellon_editor_group_drill_circular_angle_entry},
        {"excellon_editor_group_drill_circular_angle_label", excellon_editor_group_drill_circular_angle_label},
        {"excellon_editor_group_drill_circular_dir_radio", excellon_editor_group_drill_circular_dir_radio},
        {"excellon_editor_group_drill_circular_direction_label", excellon_editor_group_drill_circular_direction_label},
        {"excellon_editor_group_drill_pitch_entry", excellon_editor_group_drill_pitch_entry},
        {"excellon_editor_group_drill_pitch_label", excellon_editor_group_drill_pitch_label},
        {"excellon_editor_group_layout", excellon_editor_group_layout},
        {"excellon_editor_group_param_label", excellon_editor_group_param_label},
        {"excellon_editor_group_sel_limit_entry", excellon_editor_group_sel_limit_entry},
        {"excellon_editor_group_sel_limit_label", excellon_editor_group_sel_limit_label},
        {"excellon_editor_group_slot_angle_label", excellon_editor_group_slot_angle_label},
        {"excellon_editor_group_slot_angle_spinner", excellon_editor_group_slot_angle_spinner},
        {"excellon_editor_group_slot_array_angle_entry", excellon_editor_group_slot_array_angle_entry},
        {"excellon_editor_group_slot_array_angle_label", excellon_editor_group_slot_array_angle_label},
        {"excellon_editor_group_slot_array_axis_label", excellon_editor_group_slot_array_axis_label},
        {"excellon_editor_group_slot_array_axis_radio", excellon_editor_group_slot_array_axis_radio},
        {"excellon_editor_group_slot_array_circ_label", excellon_editor_group_slot_array_circ_label},
        {"excellon_editor_group_slot_array_circular_angle_entry", excellon_editor_group_slot_array_circular_angle_entry},
        {"excellon_editor_group_slot_array_circular_angle_label", excellon_editor_group_slot_array_circular_angle_label},
        {"excellon_editor_group_slot_array_circular_dir_radio", excellon_editor_group_slot_array_circular_dir_radio},
        {"excellon_editor_group_slot_array_circular_direction_label", excellon_editor_group_slot_array_circular_direction_label},
        {"excellon_editor_group_slot_array_linear_label", excellon_editor_group_slot_array_linear_label},
        {"excellon_editor_group_slot_array_pitch_entry", excellon_editor_group_slot_array_pitch_entry},
        {"excellon_editor_group_slot_array_pitch_label", excellon_editor_group_slot_array_pitch_label},
        {"excellon_editor_group_slot_array_size_entry", excellon_editor_group_slot_array_size_entry},
        {"excellon_editor_group_slot_array_size_label", excellon_editor_group_slot_array_size_label},
        {"excellon_editor_group_slot_axis_label", excellon_editor_group_slot_axis_label},
        {"excellon_editor_group_slot_axis_radio", excellon_editor_group_slot_axis_radio},
        {"excellon_editor_group_slot_length_entry", excellon_editor_group_slot_length_entry},
        {"excellon_editor_group_slot_length_label", excellon_editor_group_slot_length_label},
        {"excellon_exp_group", excellon_exp_group},
        {"excellon_exp_group_digits_label", excellon_exp_group_digits_label},
        {"excellon_exp_group_excellon_units_label", excellon_exp_group_excellon_units_label},
        {"excellon_exp_group_excellon_units_radio", excellon_exp_group_excellon_units_radio},
        {"excellon_exp_group_export_options_label", excellon_exp_group_export_options_label},
        {"excellon_exp_group_format_dec_entry", excellon_exp_group_format_dec_entry},
        {"excellon_exp_group_format_label", excellon_exp_group_format_label},
        {"excellon_exp_group_format_radio", excellon_exp_group_format_radio},
        {"excellon_exp_group_format_whole_entry", excellon_exp_group_format_whole_entry},
        {"excellon_exp_group_layout", excellon_exp_group_layout},
        {"excellon_exp_group_slot_type_label", excellon_exp_group_slot_type_label},
        {"excellon_exp_group_slot_type_radio", excellon_exp_group_slot_type_radio},
        {"excellon_exp_group_zeros_label", excellon_exp_group_zeros_label},
        {"excellon_exp_group_zeros_radio", excellon_exp_group_zeros_radio},
        {"excellon_gen_group", excellon_gen_group},
        {"excellon_gen_group_excellon_alpha_entry", excellon_gen_group_excellon_alpha_entry},
        {"excellon_gen_group_excellon_alpha_label", excellon_gen_group_excellon_alpha_label},
        {"excellon_gen_group_excellon_defaults_button", excellon_gen_group_excellon_defaults_button},
        {"excellon_gen_group_excellon_format_in_label", excellon_gen_group_excellon_format_in_label},
        {"excellon_gen_group_excellon_format_label", excellon_gen_group_excellon_format_label},
        {"excellon_gen_group_excellon_format_lower_in_entry", excellon_gen_group_excellon_format_lower_in_entry},
        {"excellon_gen_group_excellon_format_lower_mm_entry", excellon_gen_group_excellon_format_lower_mm_entry},
        {"excellon_gen_group_excellon_format_mm_label", excellon_gen_group_excellon_format_mm_label},
        {"excellon_gen_group_excellon_format_upper_in_entry", excellon_gen_group_excellon_format_upper_in_entry},
        {"excellon_gen_group_excellon_format_upper_mm_entry", excellon_gen_group_excellon_format_upper_mm_entry},
        {"excellon_gen_group_excellon_general_label", excellon_gen_group_excellon_general_label},
        {"excellon_gen_group_excellon_optimization_label", excellon_gen_group_excellon_optimization_label},
        {"excellon_gen_group_excellon_optimization_radio", excellon_gen_group_excellon_optimization_radio},
        {"excellon_gen_group_excellon_units_label", excellon_gen_group_excellon_units_label},
        {"excellon_gen_group_excellon_units_radio", excellon_gen_group_excellon_units_radio},
        {"excellon_gen_group_excellon_zeros_label", excellon_gen_group_excellon_zeros_label},
        {"excellon_gen_group_excellon_zeros_radio", excellon_gen_group_excellon_zeros_radio},
        {"excellon_gen_group_fill_color_entry", excellon_gen_group_fill_color_entry},
        {"excellon_gen_group_fill_color_label", excellon_gen_group_fill_color_label},
        {"excellon_gen_group_fuse_tools_cb", excellon_gen_group_fuse_tools_cb},
        {"excellon_gen_group_gerber_color_label", excellon_gen_group_gerber_color_label},
        {"excellon_gen_group_join_geo_label", excellon_gen_group_join_geo_label},
        {"excellon_gen_group_layout", excellon_gen_group_layout},
        {"excellon_gen_group_line_color_entry", excellon_gen_group_line_color_entry},
        {"excellon_gen_group_line_color_label", excellon_gen_group_line_color_label},
        {"excellon_gen_group_multicolored_cb", excellon_gen_group_multicolored_cb},
        {"excellon_gen_group_optimization_time_entry", excellon_gen_group_optimization_time_entry},
        {"excellon_gen_group_optimization_time_label", excellon_gen_group_optimization_time_label},
        {"excellon_gen_group_plot_cb", excellon_gen_group_plot_cb},
        {"excellon_gen_group_plot_options_label", excellon_gen_group_plot_options_label},
        {"excellon_gen_group_solid_cb", excellon_gen_group_solid_cb},
        {"excellon_gen_group_update_excellon_cb", excellon_gen_group_update_excellon_cb},
        {"excellon_opt_group", excellon_opt_group},
        {"excellon_opt_group_cncjob_label", excellon_opt_group_cncjob_label},
        {"excellon_opt_group_layout", excellon_opt_group_layout},
        {"excellon_opt_group_mill_dia_entry", excellon_opt_group_mill_dia_entry},
        {"excellon_opt_group_mill_dia_label", excellon_opt_group_mill_dia_label},
        {"excellon_opt_group_mill_hole_label", excellon_opt_group_mill_hole_label},
        {"excellon_opt_group_mill_type_label", excellon_opt_group_mill_type_label},
        {"excellon_opt_group_milling_type_radio", excellon_opt_group_milling_type_radio},
        {"excellon_opt_group_operation_label", excellon_opt_group_operation_label},
        {"excellon_opt_group_operation_radio", excellon_opt_group_operation_radio},
        {"excellon_opt_group_slot_tooldia_entry", excellon_opt_group_slot_tooldia_entry},
        {"excellon_opt_group_tooldia_entry", excellon_opt_group_tooldia_entry},
        {"excellon_scroll_area", excellon_scroll_area},
        {"excellon_tab", excellon_tab},
        {"excellon_tab_lay", excellon_tab_lay},
        {"fa_excellon_group", fa_excellon_group},
        {"fa_excellon_group_add_btn", fa_excellon_group_add_btn},
        {"fa_excellon_group_del_all_btn", fa_excellon_group_del_all_btn},
        {"fa_excellon_group_del_btn", fa_excellon_group_del_btn},
        {"fa_excellon_group_exc_list_btn", fa_excellon_group_exc_list_btn},
        {"fa_excellon_group_exc_list_text", fa_excellon_group_exc_list_text},
        {"fa_excellon_group_ext_entry", fa_excellon_group_ext_entry},
        {"fa_excellon_group_ext_label", fa_excellon_group_ext_label},
        {"fa_excellon_group_layout", fa_excellon_group_layout},
        {"fa_excellon_group_restore_btn", fa_excellon_group_restore_btn},
        {"fa_excellon_group_vertical_lay", fa_excellon_group_vertical_lay},
        {"fa_gcode_group", fa_gcode_group},
        {"fa_gcode_group_add_btn", fa_gcode_group_add_btn},
        {"fa_gcode_group_del_all_btn", fa_gcode_group_del_all_btn},
        {"fa_gcode_group_del_btn", fa_gcode_group_del_btn},
        {"fa_gcode_group_ext_entry", fa_gcode_group_ext_entry},
        {"fa_gcode_group_ext_label", fa_gcode_group_ext_label},
        {"fa_gcode_group_gco_list_btn", fa_gcode_group_gco_list_btn},
        {"fa_gcode_group_gco_list_label", fa_gcode_group_gco_list_label},
        {"fa_gcode_group_gco_list_text", fa_gcode_group_gco_list_text},
        {"fa_gcode_group_layout", fa_gcode_group_layout},
        {"fa_gcode_group_restore_btn", fa_gcode_group_restore_btn},
        {"fa_gerber_group", fa_gerber_group},
        {"fa_gerber_group_add_btn", fa_gerber_group_add_btn},
        {"fa_gerber_group_del_all_btn", fa_gerber_group_del_all_btn},
        {"fa_gerber_group_del_btn", fa_gerber_group_del_btn},
        {"fa_gerber_group_ext_entry", fa_gerber_group_ext_entry},
        {"fa_gerber_group_ext_label", fa_gerber_group_ext_label},
        {"fa_gerber_group_grb_list_btn", fa_gerber_group_grb_list_btn},
        {"fa_gerber_group_grb_list_label", fa_gerber_group_grb_list_label},
        {"fa_gerber_group_grb_list_text", fa_gerber_group_grb_list_text},
        {"fa_gerber_group_layout", fa_gerber_group_layout},
        {"fa_gerber_group_restore_btn", fa_gerber_group_restore_btn},
        {"fa_scroll_area", fa_scroll_area},
        {"fa_tab", fa_tab},
        {"fa_tab_lay", fa_tab_lay},
        {"general_app_group", general_app_group},
        {"general_app_group_app_level_label", general_app_group_app_level_label},
        {"general_app_group_app_level_radio", general_app_group_app_level_radio},
        {"general_app_group_autosave_cb", general_app_group_autosave_cb},
        {"general_app_group_autosave_entry", general_app_group_autosave_entry},
        {"general_app_group_autosave_label", general_app_group_autosave_label},
        {"general_app_group_bmargin_entry", general_app_group_bmargin_entry},
        {"general_app_group_bmargin_label", general_app_group_bmargin_label},
        {"general_app_group_compress_label", general_app_group_compress_label},
        {"general_app_group_compress_spinner", general_app_group_compress_spinner},
        {"general_app_group_ge_label", general_app_group_ge_label},
        {"general_app_group_ge_radio", general_app_group_ge_radio},
        {"general_app_group_language_apply_btn", general_app_group_language_apply_btn},
        {"general_app_group_language_cb", general_app_group_language_cb},
        {"general_app_group_languagelabel", general_app_group_languagelabel},
        {"general_app_group_layout", general_app_group_layout},
        {"general_app_group_lmargin_entry", general_app_group_lmargin_entry},
        {"general_app_group_lmargin_label", general_app_group_lmargin_label},
        {"general_app_group_pdf_param_label", general_app_group_pdf_param_label},
        {"general_app_group_portability_cb", general_app_group_portability_cb},
        {"general_app_group_precision_inch_entry", general_app_group_precision_inch_entry},
        {"general_app_group_precision_inch_label", general_app_group_precision_inch_label},
        {"general_app_group_precision_metric_entry", general_app_group_precision_metric_entry},
        {"general_app_group_precision_metric_label", general_app_group_precision_metric_label},
        {"general_app_group_project_startup_cb", general_app_group_project_startup_cb},
        {"general_app_group_rmargin_entry", general_app_group_rmargin_entry},
        {"general_app_group_rmargin_label", general_app_group_rmargin_label},
        {"general_app_group_save_label", general_app_group_save_label},
        {"general_app_group_save_type_cb", general_app_group_save_type_cb},
        {"general_app_group_send_stats_cb", general_app_group_send_stats_cb},
        {"general_app_group_shell_startup_cb", general_app_group_shell_startup_cb},
        {"general_app_group_splash_cb", general_app_group_splash_cb},
        {"general_app_group_startup_label", general_app_group_startup_label},
        {"general_app_group_systray_cb", general_app_group_systray_cb},
        {"general_app_group_tmargin_entry", general_app_group_tmargin_entry},
        {"general_app_group_tmargin_label", general_app_group_tmargin_label},
        {"general_app_group_tol_entry", general_app_group_tol_entry},
        {"general_app_group_units_radio", general_app_group_units_radio},
        {"general_app_group_unitslabel", general_app_group_unitslabel},
        {"general_app_group_version_check_cb", general_app_group_version_check_cb},
        {"general_app_group_worker_number_label", general_app_group_worker_number_label},
        {"general_app_group_worker_number_sb", general_app_group_worker_number_sb},
        {"general_app_set_group", general_app_set_group},
        {"general_app_set_group_activity_combo", general_app_set_group_activity_combo},
        {"general_app_set_group_activity_label", general_app_set_group_activity_label},
        {"general_app_set_group_allow_edit_cb", general_app_set_group_allow_edit_cb},
        {"general_app_set_group_axis_font_size_label", general_app_set_group_axis_font_size_label},
        {"general_app_set_group_axis_font_size_spinner", general_app_set_group_axis_font_size_spinner},
        {"general_app_set_group_bm_limit_label", general_app_set_group_bm_limit_label},
        {"general_app_set_group_bm_limit_spinner", general_app_set_group_bm_limit_spinner},
        {"general_app_set_group_cursor_lbl", general_app_set_group_cursor_lbl},
        {"general_app_set_group_cursor_radio", general_app_set_group_cursor_radio},
        {"general_app_set_group_cursor_size_entry", general_app_set_group_cursor_size_entry},
        {"general_app_set_group_cursor_size_lbl", general_app_set_group_cursor_size_lbl},
        {"general_app_set_group_cursor_width_entry", general_app_set_group_cursor_width_entry},
        {"general_app_set_group_cursor_width_lbl", general_app_set_group_cursor_width_lbl},
        {"general_app_set_group_delete_conf_cb", general_app_set_group_delete_conf_cb},
        {"general_app_set_group_font_size_label", general_app_set_group_font_size_label},
        {"general_app_set_group_grid_label", general_app_set_group_grid_label},
        {"general_app_set_group_gridx_entry", general_app_set_group_gridx_entry},
        {"general_app_set_group_gridx_label", general_app_set_group_gridx_label},
        {"general_app_set_group_gridy_entry", general_app_set_group_gridy_entry},
        {"general_app_set_group_gridy_label", general_app_set_group_gridy_label},
        {"general_app_set_group_hud_font_size_label", general_app_set_group_hud_font_size_label},
        {"general_app_set_group_hud_font_size_spinner", general_app_set_group_hud_font_size_spinner},
        {"general_app_set_group_layout", general_app_set_group_layout},
        {"general_app_set_group_machinist_cb", general_app_set_group_machinist_cb},
        {"general_app_set_group_mouse_color_label", general_app_set_group_mouse_color_label},
        {"general_app_set_group_mouse_cursor_color_cb", general_app_set_group_mouse_cursor_color_cb},
        {"general_app_set_group_mouse_cursor_entry", general_app_set_group_mouse_cursor_entry},
        {"general_app_set_group_mouse_lbl", general_app_set_group_mouse_lbl},
        {"general_app_set_group_mselect_radio", general_app_set_group_mselect_radio},
        {"general_app_set_group_mselectlabel", general_app_set_group_mselectlabel},
        {"general_app_set_group_notebook_font_size_label", general_app_set_group_notebook_font_size_label},
        {"general_app_set_group_notebook_font_size_spinner", general_app_set_group_notebook_font_size_spinner},
        {"general_app_set_group_open_style_cb", general_app_set_group_open_style_cb},
        {"general_app_set_group_pan_button_radio", general_app_set_group_pan_button_radio},
        {"general_app_set_group_panbuttonlabel", general_app_set_group_panbuttonlabel},
        {"general_app_set_group_snap_max_dist_entry", general_app_set_group_snap_max_dist_entry},
        {"general_app_set_group_snap_max_label", general_app_set_group_snap_max_label},
        {"general_app_set_group_textbox_font_size_label", general_app_set_group_textbox_font_size_label},
        {"general_app_set_group_textbox_font_size_spinner", general_app_set_group_textbox_font_size_spinner},
        {"general_app_set_group_toggle_tooltips_cb", general_app_set_group_toggle_tooltips_cb},
        {"general_app_set_group_wk_cb", general_app_set_group_wk_cb},
        {"general_app_set_group_wk_orientation_label", general_app_set_group_wk_orientation_label},
        {"general_app_set_group_wk_orientation_radio", general_app_set_group_wk_orientation_radio},
        {"general_app_set_group_workspace_cb", general_app_set_group_workspace_cb},
        {"general_app_set_group_workspace_label", general_app_set_group_workspace_label},
        {"general_app_set_group_workspace_type_lbl", general_app_set_group_workspace_type_lbl},
        {"general_defaults_form", general_defaults_form},
        {"general_gui_group", general_gui_group},
        {"general_gui_group_alt_sf_color_entry", general_gui_group_alt_sf_color_entry},
        {"general_gui_group_alt_sf_color_label", general_gui_group_alt_sf_color_label},
        {"general_gui_group_alt_sl_color_entry", general_gui_group_alt_sl_color_entry},
        {"general_gui_group_alt_sl_color_label", general_gui_group_alt_sl_color_label},
        {"general_gui_group_draw_color_entry", general_gui_group_draw_color_entry},
        {"general_gui_group_draw_color_label", general_gui_group_draw_color_label},
        {"general_gui_group_editor_color_label", general_gui_group_editor_color_label},
        {"general_gui_group_gray_icons_cb", general_gui_group_gray_icons_cb},
        {"general_gui_group_hdpi_cb", general_gui_group_hdpi_cb},
        {"general_gui_group_hover_cb", general_gui_group_hover_cb},
        {"general_gui_group_layout", general_gui_group_layout},
        {"general_gui_group_layout_combo", general_gui_group_layout_combo},
        {"general_gui_group_layout_label", general_gui_group_layout_label},
        {"general_gui_group_left_right_alpha_entry", general_gui_group_left_right_alpha_entry},
        {"general_gui_group_left_right_alpha_label", general_gui_group_left_right_alpha_label},
        {"general_gui_group_proj_color_dis_entry", general_gui_group_proj_color_dis_entry},
        {"general_gui_group_proj_color_dis_label", general_gui_group_proj_color_dis_label},
        {"general_gui_group_proj_color_entry", general_gui_group_proj_color_entry},
        {"general_gui_group_proj_color_label", general_gui_group_proj_color_label},
        {"general_gui_group_proj_settings_label", general_gui_group_proj_settings_label},
        {"general_gui_group_project_autohide_cb", general_gui_group_project_autohide_cb},
        {"general_gui_group_right_left_alpha_entry", general_gui_group_right_left_alpha_entry},
        {"general_gui_group_right_left_alpha_label", general_gui_group_right_left_alpha_label},
        {"general_gui_group_sel_draw_color_entry", general_gui_group_sel_draw_color_entry},
        {"general_gui_group_sel_draw_color_label", general_gui_group_sel_draw_color_label},
        {"general_gui_group_sel_lr_label", general_gui_group_sel_lr_label},
        {"general_gui_group_sel_rl_label", general_gui_group_sel_rl_label},
        {"general_gui_group_selection_cb", general_gui_group_selection_cb},
        {"general_gui_group_sf_color_entry", general_gui_group_sf_color_entry},
        {"general_gui_group_sf_color_label", general_gui_group_sf_color_label},
        {"general_gui_group_sl_color_entry", general_gui_group_sl_color_entry},
        {"general_gui_group_sl_color_label", general_gui_group_sl_color_label},
        {"general_gui_group_style_combo", general_gui_group_style_combo},
        {"general_gui_group_style_label", general_gui_group_style_label},
        {"general_gui_group_theme_label", general_gui_group_theme_label},
        {"general_gui_group_theme_radio", general_gui_group_theme_radio},
        {"general_scroll_area", general_scroll_area},
        {"general_tab", general_tab},
        {"general_tab_lay", general_tab_lay},
        {"geometry_adv_opt_group", geometry_adv_opt_group},
        {"geometry_adv_opt_group_area_exc_label", geometry_adv_opt_group_area_exc_label},
        {"geometry_adv_opt_group_area_shape_label", geometry_adv_opt_group_area_shape_label},
        {"geometry_adv_opt_group_area_shape_radio", geometry_adv_opt_group_area_shape_radio},
        {"geometry_adv_opt_group_e_cut_entry", geometry_adv_opt_group_e_cut_entry},
        {"geometry_adv_opt_group_exclusion_cb", geometry_adv_opt_group_exclusion_cb},
        {"geometry_adv_opt_group_extracut_cb", geometry_adv_opt_group_extracut_cb},
        {"geometry_adv_opt_group_feedrate_probe_entry", geometry_adv_opt_group_feedrate_probe_entry},
        {"geometry_adv_opt_group_feedrate_probe_label", geometry_adv_opt_group_feedrate_probe_label},
        {"geometry_adv_opt_group_feedrate_rapid_entry", geometry_adv_opt_group_feedrate_rapid_entry},
        {"geometry_adv_opt_group_fplunge_cb", geometry_adv_opt_group_fplunge_cb},
        {"geometry_adv_opt_group_geo_label", geometry_adv_opt_group_geo_label},
        {"geometry_adv_opt_group_gstartz_entry", geometry_adv_opt_group_gstartz_entry},
        {"geometry_adv_opt_group_layout", geometry_adv_opt_group_layout},
        {"geometry_adv_opt_group_over_z_entry", geometry_adv_opt_group_over_z_entry},
        {"geometry_adv_opt_group_over_z_label", geometry_adv_opt_group_over_z_label},
        {"geometry_adv_opt_group_pdepth_entry", geometry_adv_opt_group_pdepth_entry},
        {"geometry_adv_opt_group_pdepth_label", geometry_adv_opt_group_pdepth_label},
        {"geometry_adv_opt_group_polish_cb", geometry_adv_opt_group_polish_cb},
        {"geometry_adv_opt_group_polish_dia_entry", geometry_adv_opt_group_polish_dia_entry},
        {"geometry_adv_opt_group_polish_dia_lbl", geometry_adv_opt_group_polish_dia_lbl},
        {"geometry_adv_opt_group_polish_margin_entry", geometry_adv_opt_group_polish_margin_entry},
        {"geometry_adv_opt_group_polish_margin_lbl", geometry_adv_opt_group_polish_margin_lbl},
        {"geometry_adv_opt_group_polish_method_combo", geometry_adv_opt_group_polish_method_combo},
        {"geometry_adv_opt_group_polish_method_lbl", geometry_adv_opt_group_polish_method_lbl},
        {"geometry_adv_opt_group_polish_over_entry", geometry_adv_opt_group_polish_over_entry},
        {"geometry_adv_opt_group_polish_over_lbl", geometry_adv_opt_group_polish_over_lbl},
        {"geometry_adv_opt_group_polish_pressure_entry", geometry_adv_opt_group_polish_pressure_entry},
        {"geometry_adv_opt_group_polish_pressure_lbl", geometry_adv_opt_group_polish_pressure_lbl},
        {"geometry_adv_opt_group_polish_travelz_entry", geometry_adv_opt_group_polish_travelz_entry},
        {"geometry_adv_opt_group_polish_travelz_lbl", geometry_adv_opt_group_polish_travelz_lbl},
        {"geometry_adv_opt_group_segx_entry", geometry_adv_opt_group_segx_entry},
        {"geometry_adv_opt_group_segy_entry", geometry_adv_opt_group_segy_entry},
        {"geometry_adv_opt_group_spindledir_radio", geometry_adv_opt_group_spindledir_radio},
        {"geometry_adv_opt_group_strategy_label", geometry_adv_opt_group_strategy_label},
        {"geometry_adv_opt_group_strategy_radio", geometry_adv_opt_group_strategy_radio},
        {"geometry_adv_opt_group_toolchangexy_entry", geometry_adv_opt_group_toolchangexy_entry},
        {"geometry_defaults_form", geometry_defaults_form},
        {"geometry_editor_group", geometry_editor_group},
        {"geometry_editor_group_layout", geometry_editor_group_layout},
        {"geometry_editor_group_milling_type_radio", geometry_editor_group_milling_type_radio},
        {"geometry_editor_group_param_label", geometry_editor_group_param_label},
        {"geometry_editor_group_sel_limit_entry", geometry_editor_group_sel_limit_entry},
        {"geometry_editor_group_sel_limit_label", geometry_editor_group_sel_limit_label},
        {"geometry_gen_group", geometry_gen_group},
        {"geometry_gen_group_circle_steps_entry", geometry_gen_group_circle_steps_entry},
        {"geometry_gen_group_circle_steps_label", geometry_gen_group_circle_steps_label},
        {"geometry_gen_group_cnctooldia_entry", geometry_gen_group_cnctooldia_entry},
        {"geometry_gen_group_fuse_tools_cb", geometry_gen_group_fuse_tools_cb},
        {"geometry_gen_group_gerber_color_label", geometry_gen_group_gerber_color_label},
        {"geometry_gen_group_join_geo_label", geometry_gen_group_join_geo_label},
        {"geometry_gen_group_layout", geometry_gen_group_layout},
        {"geometry_gen_group_line_color_entry", geometry_gen_group_line_color_entry},
        {"geometry_gen_group_line_color_label", geometry_gen_group_line_color_label},
        {"geometry_gen_group_multicolored_cb", geometry_gen_group_multicolored_cb},
        {"geometry_gen_group_opt_algorithm_label", geometry_gen_group_opt_algorithm_label},
        {"geometry_gen_group_opt_algorithm_radio", geometry_gen_group_opt_algorithm_radio},
        {"geometry_gen_group_opt_label", geometry_gen_group_opt_label},
        {"geometry_gen_group_optimization_time_entry", geometry_gen_group_optimization_time_entry},
        {"geometry_gen_group_optimization_time_label", geometry_gen_group_optimization_time_label},
        {"geometry_gen_group_plot_cb", geometry_gen_group_plot_cb},
        {"geometry_gen_group_plot_options_label", geometry_gen_group_plot_options_label},
        {"geometry_gen_group_tools_label", geometry_gen_group_tools_label},
        {"geometry_opt_group", geometry_opt_group},
        {"geometry_opt_group_cncfeedrate_entry", geometry_opt_group_cncfeedrate_entry},
        {"geometry_opt_group_cncjob_label", geometry_opt_group_cncjob_label},
        {"geometry_opt_group_cncspindlespeed_entry", geometry_opt_group_cncspindlespeed_entry},
        {"geometry_opt_group_cutz_entry", geometry_opt_group_cutz_entry},
        {"geometry_opt_group_depthperpass_entry", geometry_opt_group_depthperpass_entry},
        {"geometry_opt_group_dwell_cb", geometry_opt_group_dwell_cb},
        {"geometry_opt_group_dwelltime_entry", geometry_opt_group_dwelltime_entry},
        {"geometry_opt_group_endxy_entry", geometry_opt_group_endxy_entry},
        {"geometry_opt_group_endz_entry", geometry_opt_group_endz_entry},
        {"geometry_opt_group_feedrate_z_entry", geometry_opt_group_feedrate_z_entry},
        {"geometry_opt_group_layout", geometry_opt_group_layout},
        {"geometry_opt_group_multidepth_cb", geometry_opt_group_multidepth_cb},
        {"geometry_opt_group_pp_geometry_name_cb", geometry_opt_group_pp_geometry_name_cb},
        {"geometry_opt_group_toolchange_cb", geometry_opt_group_toolchange_cb},
        {"geometry_opt_group_toolchangez_entry", geometry_opt_group_toolchangez_entry},
        {"geometry_opt_group_travelz_entry", geometry_opt_group_travelz_entry},
        {"geometry_scroll_area", geometry_scroll_area},
        {"geometry_tab", geometry_tab},
        {"geometry_tab_lay", geometry_tab_lay},
        {"gerber_adv_opt_group", gerber_adv_opt_group},
        {"gerber_adv_opt_group_adv_param_label", gerber_adv_opt_group_adv_param_label},
        {"gerber_adv_opt_group_aperture_table_visibility_cb", gerber_adv_opt_group_aperture_table_visibility_cb},
        {"gerber_adv_opt_group_buffering_radio", gerber_adv_opt_group_buffering_radio},
        {"gerber_adv_opt_group_delayed_buffer_cb", gerber_adv_opt_group_delayed_buffer_cb},
        {"gerber_adv_opt_group_follow_cb", gerber_adv_opt_group_follow_cb},
        {"gerber_adv_opt_group_layout", gerber_adv_opt_group_layout},
        {"gerber_adv_opt_group_simplification_tol_label", gerber_adv_opt_group_simplification_tol_label},
        {"gerber_adv_opt_group_simplification_tol_spinner", gerber_adv_opt_group_simplification_tol_spinner},
        {"gerber_adv_opt_group_simplify_cb", gerber_adv_opt_group_simplify_cb},
        {"gerber_defaults_form", gerber_defaults_form},
        {"gerber_editor_group", gerber_editor_group},
        {"gerber_editor_group_addcode_entry", gerber_editor_group_addcode_entry},
        {"gerber_editor_group_addcode_entry_lbl", gerber_editor_group_addcode_entry_lbl},
        {"gerber_editor_group_adddim_entry", gerber_editor_group_adddim_entry},
        {"gerber_editor_group_adddim_label", gerber_editor_group_adddim_label},
        {"gerber_editor_group_addsize_entry", gerber_editor_group_addsize_entry},
        {"gerber_editor_group_addsize_entry_lbl", gerber_editor_group_addsize_entry_lbl},
        {"gerber_editor_group_addtype_combo", gerber_editor_group_addtype_combo},
        {"gerber_editor_group_addtype_combo_lbl", gerber_editor_group_addtype_combo_lbl},
        {"gerber_editor_group_grb_angle_entry", gerber_editor_group_grb_angle_entry},
        {"gerber_editor_group_grb_angle_label", gerber_editor_group_grb_angle_label},
        {"gerber_editor_group_grb_array_circ_label", gerber_editor_group_grb_array_circ_label},
        {"gerber_editor_group_grb_array_linear_label", gerber_editor_group_grb_array_linear_label},
        {"gerber_editor_group_grb_array_size_entry", gerber_editor_group_grb_array_size_entry},
        {"gerber_editor_group_grb_array_size_label", gerber_editor_group_grb_array_size_label},
        {"gerber_editor_group_grb_array_tools_b_label", gerber_editor_group_grb_array_tools_b_label},
        {"gerber_editor_group_grb_array_tools_ma_label", gerber_editor_group_grb_array_tools_ma_label},
        {"gerber_editor_group_grb_array_tools_s_label", gerber_editor_group_grb_array_tools_s_label},
        {"gerber_editor_group_grb_axis_label", gerber_editor_group_grb_axis_label},
        {"gerber_editor_group_grb_axis_radio", gerber_editor_group_grb_axis_radio},
        {"gerber_editor_group_grb_buff_entry", gerber_editor_group_grb_buff_entry},
        {"gerber_editor_group_grb_buff_label", gerber_editor_group_grb_buff_label},
        {"gerber_editor_group_grb_circular_angle_entry", gerber_editor_group_grb_circular_angle_entry},
        {"gerber_editor_group_grb_circular_angle_label", gerber_editor_group_grb_circular_angle_label},
        {"gerber_editor_group_grb_circular_dir_radio", gerber_editor_group_grb_circular_dir_radio},
        {"gerber_editor_group_grb_circular_direction_label", gerber_editor_group_grb_circular_direction_label},
        {"gerber_editor_group_grb_ma_high_entry", gerber_editor_group_grb_ma_high_entry},
        {"gerber_editor_group_grb_ma_high_label", gerber_editor_group_grb_ma_high_label},
        {"gerber_editor_group_grb_ma_low_entry", gerber_editor_group_grb_ma_low_entry},
        {"gerber_editor_group_grb_ma_low_label", gerber_editor_group_grb_ma_low_label},
        {"gerber_editor_group_grb_pitch_entry", gerber_editor_group_grb_pitch_entry},
        {"gerber_editor_group_grb_pitch_label", gerber_editor_group_grb_pitch_label},
        {"gerber_editor_group_grb_scale_entry", gerber_editor_group_grb_scale_entry},
        {"gerber_editor_group_grb_scale_label", gerber_editor_group_grb_scale_label},
        {"gerber_editor_group_layout", gerber_editor_group_layout},
        {"gerber_editor_group_param_label", gerber_editor_group_param_label},
        {"gerber_editor_group_sel_limit_entry", gerber_editor_group_sel_limit_entry},
        {"gerber_editor_group_sel_limit_label", gerber_editor_group_sel_limit_label},
        {"gerber_exp_group", gerber_exp_group},
        {"gerber_exp_group_digits_label", gerber_exp_group_digits_label},
        {"gerber_exp_group_export_options_label", gerber_exp_group_export_options_label},
        {"gerber_exp_group_format_dec_entry", gerber_exp_group_format_dec_entry},
        {"gerber_exp_group_format_whole_entry", gerber_exp_group_format_whole_entry},
        {"gerber_exp_group_gerber_units_label", gerber_exp_group_gerber_units_label},
        {"gerber_exp_group_gerber_units_radio", gerber_exp_group_gerber_units_radio},
        {"gerber_exp_group_layout", gerber_exp_group_layout},
        {"gerber_exp_group_zeros_label", gerber_exp_group_zeros_label},
        {"gerber_exp_group_zeros_radio", gerber_exp_group_zeros_radio},
        {"gerber_gen_group", gerber_gen_group},
        {"gerber_gen_group_circle_steps_entry", gerber_gen_group_circle_steps_entry},
        {"gerber_gen_group_circle_steps_label", gerber_gen_group_circle_steps_label},
        {"gerber_gen_group_clear_colors_button", gerber_gen_group_clear_colors_button},
        {"gerber_gen_group_fill_color_entry", gerber_gen_group_fill_color_entry},
        {"gerber_gen_group_fill_color_label", gerber_gen_group_fill_color_label},
        {"gerber_gen_group_gerber_alpha_entry", gerber_gen_group_gerber_alpha_entry},
        {"gerber_gen_group_gerber_alpha_label", gerber_gen_group_gerber_alpha_label},
        {"gerber_gen_group_gerber_clean_cb", gerber_gen_group_gerber_clean_cb},
        {"gerber_gen_group_gerber_color_label", gerber_gen_group_gerber_color_label},
        {"gerber_gen_group_gerber_default_label", gerber_gen_group_gerber_default_label},
        {"gerber_gen_group_gerber_extra_buffering", gerber_gen_group_gerber_extra_buffering},
        {"gerber_gen_group_gerber_units_label", gerber_gen_group_gerber_units_label},
        {"gerber_gen_group_gerber_units_radio", gerber_gen_group_gerber_units_radio},
        {"gerber_gen_group_gerber_zeros_label", gerber_gen_group_gerber_zeros_label},
        {"gerber_gen_group_gerber_zeros_radio", gerber_gen_group_gerber_zeros_radio},
        {"gerber_gen_group_layout", gerber_gen_group_layout},
        {"gerber_gen_group_line_color_entry", gerber_gen_group_line_color_entry},
        {"gerber_gen_group_line_color_label", gerber_gen_group_line_color_label},
        {"gerber_gen_group_multicolored_cb", gerber_gen_group_multicolored_cb},
        {"gerber_gen_group_plot_cb", gerber_gen_group_plot_cb},
        {"gerber_gen_group_plot_options_label", gerber_gen_group_plot_options_label},
        {"gerber_gen_group_solid_cb", gerber_gen_group_solid_cb},
        {"gerber_gen_group_store_colors_cb", gerber_gen_group_store_colors_cb},
        {"gerber_opt_group", gerber_opt_group},
        {"gerber_opt_group_bbmargin_entry", gerber_opt_group_bbmargin_entry},
        {"gerber_opt_group_bbrounded_cb", gerber_opt_group_bbrounded_cb},
        {"gerber_opt_group_boundingbox_label", gerber_opt_group_boundingbox_label},
        {"gerber_opt_group_clearcopper_label", gerber_opt_group_clearcopper_label},
        {"gerber_opt_group_layout", gerber_opt_group_layout},
        {"gerber_opt_group_noncopper_margin_entry", gerber_opt_group_noncopper_margin_entry},
        {"gerber_opt_group_noncopper_rounded_cb", gerber_opt_group_noncopper_rounded_cb},
        {"gerber_scroll_area", gerber_scroll_area},
        {"gerber_tab", gerber_tab},
        {"gerber_tab_lay", gerber_tab_lay},
        {"hlay1", hlay1},
        {"kw_group", kw_group},
        {"kw_group_add_btn", kw_group_add_btn},
        {"kw_group_del_all_btn", kw_group_del_all_btn},
        {"kw_group_del_btn", kw_group_del_btn},
        {"kw_group_grb_list_label", kw_group_grb_list_label},
        {"kw_group_kw_entry", kw_group_kw_entry},
        {"kw_group_kw_label", kw_group_kw_label},
        {"kw_group_kw_list_text", kw_group_kw_list_text},
        {"kw_group_layout", kw_group_layout},
        {"kw_group_restore_btn", kw_group_restore_btn},
        {"layout", layout},
        {"pref_apply_button", pref_apply_button},
        {"pref_close_button", pref_close_button},
        {"pref_defaults_button", pref_defaults_button},
        {"pref_open_button", pref_open_button},
        {"pref_save_button", pref_save_button},
        {"pref_tab_area", pref_tab_area},
        {"pref_tab_bottom_layout", pref_tab_bottom_layout},
        {"pref_tab_bottom_layout_1", pref_tab_bottom_layout_1},
        {"pref_tab_bottom_layout_2", pref_tab_bottom_layout_2},
        {"pref_tab_layout", pref_tab_layout},
        {"text_editor_tab", text_editor_tab},
        {"tools2_cal_group", tools2_cal_group},
        {"tools2_cal_group_cal_source_lbl", tools2_cal_group_cal_source_lbl},
        {"tools2_cal_group_cal_source_radio", tools2_cal_group_cal_source_radio},
        {"tools2_cal_group_layout", tools2_cal_group_layout},
        {"tools2_cal_group_param_label", tools2_cal_group_param_label},
        {"tools2_cal_group_second_point_radio", tools2_cal_group_second_point_radio},
        {"tools2_cal_group_toolchange_xy_entry", tools2_cal_group_toolchange_xy_entry},
        {"tools2_cal_group_toolchangez_entry", tools2_cal_group_toolchangez_entry},
        {"tools2_cal_group_travelz_entry", tools2_cal_group_travelz_entry},
        {"tools2_cal_group_verz_entry", tools2_cal_group_verz_entry},
        {"tools2_cal_group_zeroz_cb", tools2_cal_group_zeroz_cb},
        {"tools2_cfill_group", tools2_cfill_group},
        {"tools2_cfill_group_area_entry", tools2_cfill_group_area_entry},
        {"tools2_cfill_group_area_label", tools2_cfill_group_area_label},
        {"tools2_cfill_group_bbox_type_label", tools2_cfill_group_bbox_type_label},
        {"tools2_cfill_group_bbox_type_radio", tools2_cfill_group_bbox_type_radio},
        {"tools2_cfill_group_cflabel", tools2_cfill_group_cflabel},
        {"tools2_cfill_group_circle_steps_lbl", tools2_cfill_group_circle_steps_lbl},
        {"tools2_cfill_group_circlesteps_entry", tools2_cfill_group_circlesteps_entry},
        {"tools2_cfill_group_clearance_entry", tools2_cfill_group_clearance_entry},
        {"tools2_cfill_group_clearance_label", tools2_cfill_group_clearance_label},
        {"tools2_cfill_group_clearance_ppm_entry", tools2_cfill_group_clearance_ppm_entry},
        {"tools2_cfill_group_clearance_ppm_label", tools2_cfill_group_clearance_ppm_label},
        {"tools2_cfill_group_dot_dia_entry", tools2_cfill_group_dot_dia_entry},
        {"tools2_cfill_group_dot_spacing_entry", tools2_cfill_group_dot_spacing_entry},
        {"tools2_cfill_group_dotdia_label", tools2_cfill_group_dotdia_label},
        {"tools2_cfill_group_dots_label", tools2_cfill_group_dots_label},
        {"tools2_cfill_group_dotspacing_label", tools2_cfill_group_dotspacing_label},
        {"tools2_cfill_group_fill_type_label", tools2_cfill_group_fill_type_label},
        {"tools2_cfill_group_fill_type_radio", tools2_cfill_group_fill_type_radio},
        {"tools2_cfill_group_layout", tools2_cfill_group_layout},
        {"tools2_cfill_group_line_size_entry", tools2_cfill_group_line_size_entry},
        {"tools2_cfill_group_line_size_label", tools2_cfill_group_line_size_label},
        {"tools2_cfill_group_lines_label", tools2_cfill_group_lines_label},
        {"tools2_cfill_group_lines_spacing_entry", tools2_cfill_group_lines_spacing_entry},
        {"tools2_cfill_group_lines_spacing_label", tools2_cfill_group_lines_spacing_label},
        {"tools2_cfill_group_margin_entry", tools2_cfill_group_margin_entry},
        {"tools2_cfill_group_margin_label", tools2_cfill_group_margin_label},
        {"tools2_cfill_group_patern_mask_label", tools2_cfill_group_patern_mask_label},
        {"tools2_cfill_group_ppm_choice_label", tools2_cfill_group_ppm_choice_label},
        {"tools2_cfill_group_ppm_choice_radio", tools2_cfill_group_ppm_choice_radio},
        {"tools2_cfill_group_rb_margin_entry", tools2_cfill_group_rb_margin_entry},
        {"tools2_cfill_group_rb_margin_label", tools2_cfill_group_rb_margin_label},
        {"tools2_cfill_group_rb_thickness_entry", tools2_cfill_group_rb_thickness_entry},
        {"tools2_cfill_group_rb_thickness_label", tools2_cfill_group_rb_thickness_label},
        {"tools2_cfill_group_reference_label", tools2_cfill_group_reference_label},
        {"tools2_cfill_group_reference_radio", tools2_cfill_group_reference_radio},
        {"tools2_cfill_group_robber_bar_label", tools2_cfill_group_robber_bar_label},
        {"tools2_cfill_group_square_size_entry", tools2_cfill_group_square_size_entry},
        {"tools2_cfill_group_square_size_label", tools2_cfill_group_square_size_label},
        {"tools2_cfill_group_squares_label", tools2_cfill_group_squares_label},
        {"tools2_cfill_group_squares_spacing_entry", tools2_cfill_group_squares_spacing_entry},
        {"tools2_cfill_group_squares_spacing_label", tools2_cfill_group_squares_spacing_label},
        {"tools2_checkrules_group", tools2_checkrules_group},
        {"tools2_checkrules_group_clearance_copper2copper_cb", tools2_checkrules_group_clearance_copper2copper_cb},
        {"tools2_checkrules_group_clearance_copper2copper_entry", tools2_checkrules_group_clearance_copper2copper_entry},
        {"tools2_checkrules_group_clearance_copper2copper_lbl", tools2_checkrules_group_clearance_copper2copper_lbl},
        {"tools2_checkrules_group_clearance_copper2ol_cb", tools2_checkrules_group_clearance_copper2ol_cb},
        {"tools2_checkrules_group_clearance_copper2ol_entry", tools2_checkrules_group_clearance_copper2ol_entry},
        {"tools2_checkrules_group_clearance_copper2ol_lbl", tools2_checkrules_group_clearance_copper2ol_lbl},
        {"tools2_checkrules_group_clearance_d2d_cb", tools2_checkrules_group_clearance_d2d_cb},
        {"tools2_checkrules_group_clearance_d2d_entry", tools2_checkrules_group_clearance_d2d_entry},
        {"tools2_checkrules_group_clearance_d2d_lbl", tools2_checkrules_group_clearance_d2d_lbl},
        {"tools2_checkrules_group_clearance_silk2ol_cb", tools2_checkrules_group_clearance_silk2ol_cb},
        {"tools2_checkrules_group_clearance_silk2ol_entry", tools2_checkrules_group_clearance_silk2ol_entry},
        {"tools2_checkrules_group_clearance_silk2ol_lbl", tools2_checkrules_group_clearance_silk2ol_lbl},
        {"tools2_checkrules_group_clearance_silk2silk_cb", tools2_checkrules_group_clearance_silk2silk_cb},
        {"tools2_checkrules_group_clearance_silk2silk_entry", tools2_checkrules_group_clearance_silk2silk_entry},
        {"tools2_checkrules_group_clearance_silk2silk_lbl", tools2_checkrules_group_clearance_silk2silk_lbl},
        {"tools2_checkrules_group_clearance_silk2sm_cb", tools2_checkrules_group_clearance_silk2sm_cb},
        {"tools2_checkrules_group_clearance_silk2sm_entry", tools2_checkrules_group_clearance_silk2sm_entry},
        {"tools2_checkrules_group_clearance_silk2sm_lbl", tools2_checkrules_group_clearance_silk2sm_lbl},
        {"tools2_checkrules_group_clearance_sm2sm_cb", tools2_checkrules_group_clearance_sm2sm_cb},
        {"tools2_checkrules_group_clearance_sm2sm_entry", tools2_checkrules_group_clearance_sm2sm_entry},
        {"tools2_checkrules_group_clearance_sm2sm_lbl", tools2_checkrules_group_clearance_sm2sm_lbl},
        {"tools2_checkrules_group_crlabel", tools2_checkrules_group_crlabel},
        {"tools2_checkrules_group_drill_size_cb", tools2_checkrules_group_drill_size_cb},
        {"tools2_checkrules_group_drill_size_entry", tools2_checkrules_group_drill_size_entry},
        {"tools2_checkrules_group_drill_size_lbl", tools2_checkrules_group_drill_size_lbl},
        {"tools2_checkrules_group_form_layout_1", tools2_checkrules_group_form_layout_1},
        {"tools2_checkrules_group_layout", tools2_checkrules_group_layout},
        {"tools2_checkrules_group_ring_integrity_cb", tools2_checkrules_group_ring_integrity_cb},
        {"tools2_checkrules_group_ring_integrity_entry", tools2_checkrules_group_ring_integrity_entry},
        {"tools2_checkrules_group_ring_integrity_lbl", tools2_checkrules_group_ring_integrity_lbl},
        {"tools2_checkrules_group_trace_size_cb", tools2_checkrules_group_trace_size_cb},
        {"tools2_checkrules_group_trace_size_entry", tools2_checkrules_group_trace_size_entry},
        {"tools2_checkrules_group_trace_size_lbl", tools2_checkrules_group_trace_size_lbl},
        {"tools2_defaults_form", tools2_defaults_form},
        {"tools2_edrills_group", tools2_edrills_group},
        {"tools2_edrills_group_circular_cb", tools2_edrills_group_circular_cb},
        {"tools2_edrills_group_circular_ring_entry", tools2_edrills_group_circular_ring_entry},
        {"tools2_edrills_group_circular_ring_label", tools2_edrills_group_circular_ring_label},
        {"tools2_edrills_group_dia_entry", tools2_edrills_group_dia_entry},
        {"tools2_edrills_group_dia_label", tools2_edrills_group_dia_label},
        {"tools2_edrills_group_factor_entry", tools2_edrills_group_factor_entry},
        {"tools2_edrills_group_factor_label", tools2_edrills_group_factor_label},
        {"tools2_edrills_group_fixed_label", tools2_edrills_group_fixed_label},
        {"tools2_edrills_group_hole_size_label", tools2_edrills_group_hole_size_label},
        {"tools2_edrills_group_hole_size_radio", tools2_edrills_group_hole_size_radio},
        {"tools2_edrills_group_layout", tools2_edrills_group_layout},
        {"tools2_edrills_group_oblong_cb", tools2_edrills_group_oblong_cb},
        {"tools2_edrills_group_oblong_ring_entry", tools2_edrills_group_oblong_ring_entry},
        {"tools2_edrills_group_oblong_ring_label", tools2_edrills_group_oblong_ring_label},
        {"tools2_edrills_group_other_cb", tools2_edrills_group_other_cb},
        {"tools2_edrills_group_other_ring_entry", tools2_edrills_group_other_ring_entry},
        {"tools2_edrills_group_other_ring_label", tools2_edrills_group_other_ring_label},
        {"tools2_edrills_group_padt_label", tools2_edrills_group_padt_label},
        {"tools2_edrills_group_param_label", tools2_edrills_group_param_label},
        {"tools2_edrills_group_prop_label", tools2_edrills_group_prop_label},
        {"tools2_edrills_group_rectangular_cb", tools2_edrills_group_rectangular_cb},
        {"tools2_edrills_group_rectangular_ring_entry", tools2_edrills_group_rectangular_ring_entry},
        {"tools2_edrills_group_rectangular_ring_label", tools2_edrills_group_rectangular_ring_label},
        {"tools2_edrills_group_ring_label", tools2_edrills_group_ring_label},
        {"tools2_edrills_group_square_cb", tools2_edrills_group_square_cb},
        {"tools2_edrills_group_square_ring_entry", tools2_edrills_group_square_ring_entry},
        {"tools2_edrills_group_square_ring_label", tools2_edrills_group_square_ring_label},
        {"tools2_fiducials_group", tools2_fiducials_group},
        {"tools2_fiducials_group_dia_entry", tools2_fiducials_group_dia_entry},
        {"tools2_fiducials_group_dia_label", tools2_fiducials_group_dia_label},
        {"tools2_fiducials_group_fid_type_label", tools2_fiducials_group_fid_type_label},
        {"tools2_fiducials_group_fid_type_radio", tools2_fiducials_group_fid_type_radio},
        {"tools2_fiducials_group_layout", tools2_fiducials_group_layout},
        {"tools2_fiducials_group_line_thickness_entry", tools2_fiducials_group_line_thickness_entry},
        {"tools2_fiducials_group_line_thickness_label", tools2_fiducials_group_line_thickness_label},
        {"tools2_fiducials_group_margin_entry", tools2_fiducials_group_margin_entry},
        {"tools2_fiducials_group_margin_label", tools2_fiducials_group_margin_label},
        {"tools2_fiducials_group_mode_label", tools2_fiducials_group_mode_label},
        {"tools2_fiducials_group_mode_radio", tools2_fiducials_group_mode_radio},
        {"tools2_fiducials_group_param_label", tools2_fiducials_group_param_label},
        {"tools2_fiducials_group_pos_label", tools2_fiducials_group_pos_label},
        {"tools2_fiducials_group_pos_radio", tools2_fiducials_group_pos_radio},
        {"tools2_invert_group", tools2_invert_group},
        {"tools2_invert_group_join_label", tools2_invert_group_join_label},
        {"tools2_invert_group_join_radio", tools2_invert_group_join_radio},
        {"tools2_invert_group_layout", tools2_invert_group_layout},
        {"tools2_invert_group_margin_entry", tools2_invert_group_margin_entry},
        {"tools2_invert_group_margin_label", tools2_invert_group_margin_label},
        {"tools2_invert_group_sublabel", tools2_invert_group_sublabel},
        {"tools2_optimal_group", tools2_optimal_group},
        {"tools2_optimal_group_layout", tools2_optimal_group_layout},
        {"tools2_optimal_group_optlabel", tools2_optimal_group_optlabel},
        {"tools2_optimal_group_precision_lbl", tools2_optimal_group_precision_lbl},
        {"tools2_optimal_group_precision_sp", tools2_optimal_group_precision_sp},
        {"tools2_punch_group", tools2_punch_group},
        {"tools2_punch_group_circular_cb", tools2_punch_group_circular_cb},
        {"tools2_punch_group_circular_ring_entry", tools2_punch_group_circular_ring_entry},
        {"tools2_punch_group_circular_ring_label", tools2_punch_group_circular_ring_label},
        {"tools2_punch_group_dia_entry", tools2_punch_group_dia_entry},
        {"tools2_punch_group_dia_label", tools2_punch_group_dia_label},
        {"tools2_punch_group_factor_entry", tools2_punch_group_factor_entry},
        {"tools2_punch_group_factor_label", tools2_punch_group_factor_label},
        {"tools2_punch_group_fixed_label", tools2_punch_group_fixed_label},
        {"tools2_punch_group_hole_size_label", tools2_punch_group_hole_size_label},
        {"tools2_punch_group_hole_size_radio", tools2_punch_group_hole_size_radio},
        {"tools2_punch_group_layout", tools2_punch_group_layout},
        {"tools2_punch_group_oblong_cb", tools2_punch_group_oblong_cb},
        {"tools2_punch_group_oblong_ring_entry", tools2_punch_group_oblong_ring_entry},
        {"tools2_punch_group_oblong_ring_label", tools2_punch_group_oblong_ring_label},
        {"tools2_punch_group_other_cb", tools2_punch_group_other_cb},
        {"tools2_punch_group_other_ring_entry", tools2_punch_group_other_ring_entry},
        {"tools2_punch_group_other_ring_label", tools2_punch_group_other_ring_label},
        {"tools2_punch_group_padt_label", tools2_punch_group_padt_label},
        {"tools2_punch_group_param_label", tools2_punch_group_param_label},
        {"tools2_punch_group_prop_label", tools2_punch_group_prop_label},
        {"tools2_punch_group_rectangular_cb", tools2_punch_group_rectangular_cb},
        {"tools2_punch_group_rectangular_ring_entry", tools2_punch_group_rectangular_ring_entry},
        {"tools2_punch_group_rectangular_ring_label", tools2_punch_group_rectangular_ring_label},
        {"tools2_punch_group_ring_label", tools2_punch_group_ring_label},
        {"tools2_punch_group_square_cb", tools2_punch_group_square_cb},
        {"tools2_punch_group_square_ring_entry", tools2_punch_group_square_ring_entry},
        {"tools2_punch_group_square_ring_label", tools2_punch_group_square_ring_label},
        {"tools2_qrcode_group", tools2_qrcode_group},
        {"tools2_qrcode_group_back_color_entry", tools2_qrcode_group_back_color_entry},
        {"tools2_qrcode_group_back_color_label", tools2_qrcode_group_back_color_label},
        {"tools2_qrcode_group_bb_label", tools2_qrcode_group_bb_label},
        {"tools2_qrcode_group_bb_radio", tools2_qrcode_group_bb_radio},
        {"tools2_qrcode_group_border_size_entry", tools2_qrcode_group_border_size_entry},
        {"tools2_qrcode_group_border_size_label", tools2_qrcode_group_border_size_label},
        {"tools2_qrcode_group_bsize_entry", tools2_qrcode_group_bsize_entry},
        {"tools2_qrcode_group_bsize_label", tools2_qrcode_group_bsize_label},
        {"tools2_qrcode_group_error_label", tools2_qrcode_group_error_label},
        {"tools2_qrcode_group_error_radio", tools2_qrcode_group_error_radio},
        {"tools2_qrcode_group_fill_color_entry", tools2_qrcode_group_fill_color_entry},
        {"tools2_qrcode_group_fill_color_label", tools2_qrcode_group_fill_color_label},
        {"tools2_qrcode_group_layout", tools2_qrcode_group_layout},
        {"tools2_qrcode_group_pol_label", tools2_qrcode_group_pol_label},
        {"tools2_qrcode_group_pol_radio", tools2_qrcode_group_pol_radio},
        {"tools2_qrcode_group_qrlabel", tools2_qrcode_group_qrlabel},
        {"tools2_qrcode_group_sel_limit_entry", tools2_qrcode_group_sel_limit_entry},
        {"tools2_qrcode_group_sel_limit_label", tools2_qrcode_group_sel_limit_label},
        {"tools2_qrcode_group_text_data", tools2_qrcode_group_text_data},
        {"tools2_qrcode_group_text_label", tools2_qrcode_group_text_label},
        {"tools2_qrcode_group_version_entry", tools2_qrcode_group_version_entry},
        {"tools2_qrcode_group_version_label", tools2_qrcode_group_version_label},
        {"tools2_scroll_area", tools2_scroll_area},
        {"tools2_tab", tools2_tab},
        {"tools2_tab_lay", tools2_tab_lay},
        {"tools_2sided_group", tools_2sided_group},
        {"tools_2sided_group_align_ax_label", tools_2sided_group_align_ax_label},
        {"tools_2sided_group_align_axis_radio", tools_2sided_group_align_axis_radio},
        {"tools_2sided_group_axis_location_radio", tools_2sided_group_axis_location_radio},
        {"tools_2sided_group_axloc_label", tools_2sided_group_axloc_label},
        {"tools_2sided_group_dblsided_label", tools_2sided_group_dblsided_label},
        {"tools_2sided_group_dd_label", tools_2sided_group_dd_label},
        {"tools_2sided_group_drill_dia_entry", tools_2sided_group_drill_dia_entry},
        {"tools_2sided_group_layout", tools_2sided_group_layout},
        {"tools_2sided_group_mirax_label", tools_2sided_group_mirax_label},
        {"tools_2sided_group_mirror_axis_radio", tools_2sided_group_mirror_axis_radio},
        {"tools_calculators_group", tools_calculators_group},
        {"tools_calculators_group_area_entry", tools_calculators_group_area_entry},
        {"tools_calculators_group_area_label", tools_calculators_group_area_label},
        {"tools_calculators_group_cdensity_entry", tools_calculators_group_cdensity_entry},
        {"tools_calculators_group_cdensity_label", tools_calculators_group_cdensity_label},
        {"tools_calculators_group_cut_z_entry", tools_calculators_group_cut_z_entry},
        {"tools_calculators_group_cut_z_label", tools_calculators_group_cut_z_label},
        {"tools_calculators_group_growth_entry", tools_calculators_group_growth_entry},
        {"tools_calculators_group_growth_label", tools_calculators_group_growth_label},
        {"tools_calculators_group_layout", tools_calculators_group_layout},
        {"tools_calculators_group_pcblength_entry", tools_calculators_group_pcblength_entry},
        {"tools_calculators_group_pcblengthlabel", tools_calculators_group_pcblengthlabel},
        {"tools_calculators_group_pcbwidth_entry", tools_calculators_group_pcbwidth_entry},
        {"tools_calculators_group_pcbwidthlabel", tools_calculators_group_pcbwidthlabel},
        {"tools_calculators_group_plate_title_label", tools_calculators_group_plate_title_label},
        {"tools_calculators_group_tip_angle_entry", tools_calculators_group_tip_angle_entry},
        {"tools_calculators_group_tip_angle_label", tools_calculators_group_tip_angle_label},
        {"tools_calculators_group_tip_dia_entry", tools_calculators_group_tip_dia_entry},
        {"tools_calculators_group_tip_dia_label", tools_calculators_group_tip_dia_label},
        {"tools_calculators_group_vshape_tool_label", tools_calculators_group_vshape_tool_label},
        {"tools_corners_group", tools_corners_group},
        {"tools_corners_group_drill_dia_entry", tools_corners_group_drill_dia_entry},
        {"tools_corners_group_drill_dia_label", tools_corners_group_drill_dia_label},
        {"tools_corners_group_l_entry", tools_corners_group_l_entry},
        {"tools_corners_group_l_label", tools_corners_group_l_label},
        {"tools_corners_group_layout", tools_corners_group_layout},
        {"tools_corners_group_margin_entry", tools_corners_group_margin_entry},
        {"tools_corners_group_margin_label", tools_corners_group_margin_label},
        {"tools_corners_group_param_label", tools_corners_group_param_label},
        {"tools_corners_group_thick_entry", tools_corners_group_thick_entry},
        {"tools_corners_group_thick_label", tools_corners_group_thick_label},
        {"tools_corners_group_type_label", tools_corners_group_type_label},
        {"tools_corners_group_type_radio", tools_corners_group_type_radio},
        {"tools_cutout_group", tools_cutout_group},
        {"tools_cutout_group_big_cursor_cb", tools_cutout_group_big_cursor_cb},
        {"tools_cutout_group_board_cutout_label", tools_cutout_group_board_cutout_label},
        {"tools_cutout_group_convex_box", tools_cutout_group_convex_box},
        {"tools_cutout_group_cutout_gap_entry", tools_cutout_group_cutout_gap_entry},
        {"tools_cutout_group_cutout_margin_entry", tools_cutout_group_cutout_margin_entry},
        {"tools_cutout_group_cutout_tooldia_entry", tools_cutout_group_cutout_tooldia_entry},
        {"tools_cutout_group_cutz_entry", tools_cutout_group_cutz_entry},
        {"tools_cutout_group_gaps_combo", tools_cutout_group_gaps_combo},
        {"tools_cutout_group_gaptype_label", tools_cutout_group_gaptype_label},
        {"tools_cutout_group_gaptype_radio", tools_cutout_group_gaptype_radio},
        {"tools_cutout_group_layout", tools_cutout_group_layout},
        {"tools_cutout_group_maxdepth_entry", tools_cutout_group_maxdepth_entry},
        {"tools_cutout_group_mb_dia_entry", tools_cutout_group_mb_dia_entry},
        {"tools_cutout_group_mb_dia_label", tools_cutout_group_mb_dia_label},
        {"tools_cutout_group_mb_spacing_entry", tools_cutout_group_mb_spacing_entry},
        {"tools_cutout_group_mb_spacing_label", tools_cutout_group_mb_spacing_label},
        {"tools_cutout_group_mpass_cb", tools_cutout_group_mpass_cb},
        {"tools_cutout_group_obj_kind_combo", tools_cutout_group_obj_kind_combo},
        {"tools_cutout_group_thin_depth_entry", tools_cutout_group_thin_depth_entry},
        {"tools_cutout_group_thin_depth_label", tools_cutout_group_thin_depth_label},
        {"tools_defaults_form", tools_defaults_form},
        {"tools_drill_group", tools_drill_group},
        {"tools_drill_group_area_exc_label", tools_drill_group_area_exc_label},
        {"tools_drill_group_area_shape_label", tools_drill_group_area_shape_label},
        {"tools_drill_group_area_shape_radio", tools_drill_group_area_shape_radio},
        {"tools_drill_group_cutz_entry", tools_drill_group_cutz_entry},
        {"tools_drill_group_drill_label", tools_drill_group_drill_label},
        {"tools_drill_group_drill_overlap_entry", tools_drill_group_drill_overlap_entry},
        {"tools_drill_group_drill_overlap_label", tools_drill_group_drill_overlap_label},
        {"tools_drill_group_drill_slots_cb", tools_drill_group_drill_slots_cb},
        {"tools_drill_group_dslots_label", tools_drill_group_dslots_label},
        {"tools_drill_group_dwell_cb", tools_drill_group_dwell_cb},
        {"tools_drill_group_dwelltime_entry", tools_drill_group_dwelltime_entry},
        {"tools_drill_group_endxy_entry", tools_drill_group_endxy_entry},
        {"tools_drill_group_endz_entry", tools_drill_group_endz_entry},
        {"tools_drill_group_estartz_entry", tools_drill_group_estartz_entry},
        {"tools_drill_group_exc_label", tools_drill_group_exc_label},
        {"tools_drill_group_exclusion_cb", tools_drill_group_exclusion_cb},
        {"tools_drill_group_feedrate_probe_entry", tools_drill_group_feedrate_probe_entry},
        {"tools_drill_group_feedrate_probe_label", tools_drill_group_feedrate_probe_label},
        {"tools_drill_group_feedrate_rapid_entry", tools_drill_group_feedrate_rapid_entry},
        {"tools_drill_group_feedrate_z_entry", tools_drill_group_feedrate_z_entry},
        {"tools_drill_group_fplunge_cb", tools_drill_group_fplunge_cb},
        {"tools_drill_group_fretract_cb", tools_drill_group_fretract_cb},
        {"tools_drill_group_last_drill_cb", tools_drill_group_last_drill_cb},
        {"tools_drill_group_layout", tools_drill_group_layout},
        {"tools_drill_group_maxdepth_entry", tools_drill_group_maxdepth_entry},
        {"tools_drill_group_mpass_cb", tools_drill_group_mpass_cb},
        {"tools_drill_group_offset_entry", tools_drill_group_offset_entry},
        {"tools_drill_group_order_label", tools_drill_group_order_label},
        {"tools_drill_group_order_radio", tools_drill_group_order_radio},
        {"tools_drill_group_over_z_entry", tools_drill_group_over_z_entry},
        {"tools_drill_group_over_z_label", tools_drill_group_over_z_label},
        {"tools_drill_group_pdepth_entry", tools_drill_group_pdepth_entry},
        {"tools_drill_group_pdepth_label", tools_drill_group_pdepth_label},
        {"tools_drill_group_pp_excellon_name_cb", tools_drill_group_pp_excellon_name_cb},
        {"tools_drill_group_spindledir_radio", tools_drill_group_spindledir_radio},
        {"tools_drill_group_spindlespeed_entry", tools_drill_group_spindlespeed_entry},
        {"tools_drill_group_strategy_label", tools_drill_group_strategy_label},
        {"tools_drill_group_strategy_radio", tools_drill_group_strategy_radio},
        {"tools_drill_group_toolchange_cb", tools_drill_group_toolchange_cb},
        {"tools_drill_group_toolchangexy_entry", tools_drill_group_toolchangexy_entry},
        {"tools_drill_group_toolchangez_entry", tools_drill_group_toolchangez_entry},
        {"tools_drill_group_travelz_entry", tools_drill_group_travelz_entry},
        {"tools_film_group", tools_film_group},
        {"tools_film_group_file_type_label", tools_film_group_file_type_label},
        {"tools_film_group_file_type_radio", tools_film_group_file_type_radio},
        {"tools_film_group_film_adj_label", tools_film_group_film_adj_label},
        {"tools_film_group_film_boundary_entry", tools_film_group_film_boundary_entry},
        {"tools_film_group_film_boundary_label", tools_film_group_film_boundary_label},
        {"tools_film_group_film_color_entry", tools_film_group_film_color_entry},
        {"tools_film_group_film_color_label", tools_film_group_film_color_label},
        {"tools_film_group_film_label", tools_film_group_film_label},
        {"tools_film_group_film_mirror_axis", tools_film_group_film_mirror_axis},
        {"tools_film_group_film_mirror_axis_label", tools_film_group_film_mirror_axis_label},
        {"tools_film_group_film_mirror_cb", tools_film_group_film_mirror_cb},
        {"tools_film_group_film_scale_cb", tools_film_group_film_scale_cb},
        {"tools_film_group_film_scale_stroke_entry", tools_film_group_film_scale_stroke_entry},
        {"tools_film_group_film_scale_stroke_label", tools_film_group_film_scale_stroke_label},
        {"tools_film_group_film_scalex_entry", tools_film_group_film_scalex_entry},
        {"tools_film_group_film_scalex_label", tools_film_group_film_scalex_label},
        {"tools_film_group_film_scaley_entry", tools_film_group_film_scaley_entry},
        {"tools_film_group_film_scaley_label", tools_film_group_film_scaley_label},
        {"tools_film_group_film_skew_cb", tools_film_group_film_skew_cb},
        {"tools_film_group_film_skew_ref_label", tools_film_group_film_skew_ref_label},
        {"tools_film_group_film_skew_reference", tools_film_group_film_skew_reference},
        {"tools_film_group_film_skewx_entry", tools_film_group_film_skewx_entry},
        {"tools_film_group_film_skewx_label", tools_film_group_film_skewx_label},
        {"tools_film_group_film_skewy_entry", tools_film_group_film_skewy_entry},
        {"tools_film_group_film_skewy_label", tools_film_group_film_skewy_label},
        {"tools_film_group_film_type_radio", tools_film_group_film_type_radio},
        {"tools_film_group_layout", tools_film_group_layout},
        {"tools_film_group_orientation_label", tools_film_group_orientation_label},
        {"tools_film_group_orientation_radio", tools_film_group_orientation_radio},
        {"tools_film_group_pagesize_combo", tools_film_group_pagesize_combo},
        {"tools_film_group_pagesize_label", tools_film_group_pagesize_label},
        {"tools_film_group_png_dpi_label", tools_film_group_png_dpi_label},
        {"tools_film_group_png_dpi_spinner", tools_film_group_png_dpi_spinner},
        {"tools_iso_group", tools_iso_group},
        {"tools_iso_group_area_shape_label", tools_iso_group_area_shape_label},
        {"tools_iso_group_area_shape_radio", tools_iso_group_area_shape_radio},
        {"tools_iso_group_combine_passes_cb", tools_iso_group_combine_passes_cb},
        {"tools_iso_group_cutz_entry", tools_iso_group_cutz_entry},
        {"tools_iso_group_except_cb", tools_iso_group_except_cb},
        {"tools_iso_group_follow_cb", tools_iso_group_follow_cb},
        {"tools_iso_group_follow_label", tools_iso_group_follow_label},
        {"tools_iso_group_force_iso_cb", tools_iso_group_force_iso_cb},
        {"tools_iso_group_iso_label", tools_iso_group_iso_label},
        {"tools_iso_group_iso_type_label", tools_iso_group_iso_type_label},
        {"tools_iso_group_iso_type_radio", tools_iso_group_iso_type_radio},
        {"tools_iso_group_layout", tools_iso_group_layout},
        {"tools_iso_group_milling_type_label", tools_iso_group_milling_type_label},
        {"tools_iso_group_milling_type_radio", tools_iso_group_milling_type_radio},
        {"tools_iso_group_newdia_entry", tools_iso_group_newdia_entry},
        {"tools_iso_group_newdialabel", tools_iso_group_newdialabel},
        {"tools_iso_group_order_label", tools_iso_group_order_label},
        {"tools_iso_group_order_radio", tools_iso_group_order_radio},
        {"tools_iso_group_overlap_entry", tools_iso_group_overlap_entry},
        {"tools_iso_group_passes_entry", tools_iso_group_passes_entry},
        {"tools_iso_group_plotting_radio", tools_iso_group_plotting_radio},
        {"tools_iso_group_poly_int_cb", tools_iso_group_poly_int_cb},
        {"tools_iso_group_rest_cb", tools_iso_group_rest_cb},
        {"tools_iso_group_select_combo", tools_iso_group_select_combo},
        {"tools_iso_group_select_label", tools_iso_group_select_label},
        {"tools_iso_group_tipangle_entry", tools_iso_group_tipangle_entry},
        {"tools_iso_group_tipanglelabel", tools_iso_group_tipanglelabel},
        {"tools_iso_group_tipdia_entry", tools_iso_group_tipdia_entry},
        {"tools_iso_group_tipdialabel", tools_iso_group_tipdialabel},
        {"tools_iso_group_tool_dia_entry", tools_iso_group_tool_dia_entry},
        {"tools_iso_group_tool_type_label", tools_iso_group_tool_type_label},
        {"tools_iso_group_tool_type_radio", tools_iso_group_tool_type_radio},
        {"tools_iso_group_valid_cb", tools_iso_group_valid_cb},
        {"tools_ncc_group", tools_ncc_group},
        {"tools_ncc_group_area_shape_label", tools_ncc_group_area_shape_label},
        {"tools_ncc_group_area_shape_radio", tools_ncc_group_area_shape_radio},
        {"tools_ncc_group_clearcopper_label", tools_ncc_group_clearcopper_label},
        {"tools_ncc_group_cutz_entry", tools_ncc_group_cutz_entry},
        {"tools_ncc_group_layout", tools_ncc_group_layout},
        {"tools_ncc_group_milling_type_label", tools_ncc_group_milling_type_label},
        {"tools_ncc_group_milling_type_radio", tools_ncc_group_milling_type_radio},
        {"tools_ncc_group_ncc_choice_offset_cb", tools_ncc_group_ncc_choice_offset_cb},
        {"tools_ncc_group_ncc_connect_cb", tools_ncc_group_ncc_connect_cb},
        {"tools_ncc_group_ncc_contour_cb", tools_ncc_group_ncc_contour_cb},
        {"tools_ncc_group_ncc_margin_entry", tools_ncc_group_ncc_margin_entry},
        {"tools_ncc_group_ncc_method_combo", tools_ncc_group_ncc_method_combo},
        {"tools_ncc_group_ncc_offset_label", tools_ncc_group_ncc_offset_label},
        {"tools_ncc_group_ncc_offset_spinner", tools_ncc_group_ncc_offset_spinner},
        {"tools_ncc_group_ncc_order_label", tools_ncc_group_ncc_order_label},
        {"tools_ncc_group_ncc_order_radio", tools_ncc_group_ncc_order_radio},
        {"tools_ncc_group_ncc_overlap_entry", tools_ncc_group_ncc_overlap_entry},
        {"tools_ncc_group_ncc_rest_cb", tools_ncc_group_ncc_rest_cb},
        {"tools_ncc_group_ncc_tool_dia_entry", tools_ncc_group_ncc_tool_dia_entry},
        {"tools_ncc_group_newdia_entry", tools_ncc_group_newdia_entry},
        {"tools_ncc_group_newdialabel", tools_ncc_group_newdialabel},
        {"tools_ncc_group_plotting_radio", tools_ncc_group_plotting_radio},
        {"tools_ncc_group_select_combo", tools_ncc_group_select_combo},
        {"tools_ncc_group_tipangle_entry", tools_ncc_group_tipangle_entry},
        {"tools_ncc_group_tipanglelabel", tools_ncc_group_tipanglelabel},
        {"tools_ncc_group_tipdia_entry", tools_ncc_group_tipdia_entry},
        {"tools_ncc_group_tipdialabel", tools_ncc_group_tipdialabel},
        {"tools_ncc_group_tool_type_label", tools_ncc_group_tool_type_label},
        {"tools_ncc_group_tool_type_radio", tools_ncc_group_tool_type_radio},
        {"tools_ncc_group_valid_cb", tools_ncc_group_valid_cb},
        {"tools_paint_group", tools_paint_group},
        {"tools_paint_group_area_shape_label", tools_paint_group_area_shape_label},
        {"tools_paint_group_area_shape_radio", tools_paint_group_area_shape_radio},
        {"tools_paint_group_contour_cb", tools_paint_group_contour_cb},
        {"tools_paint_group_cutz_entry", tools_paint_group_cutz_entry},
        {"tools_paint_group_layout", tools_paint_group_layout},
        {"tools_paint_group_newdia_entry", tools_paint_group_newdia_entry},
        {"tools_paint_group_newdialabel", tools_paint_group_newdialabel},
        {"tools_paint_group_paint_label", tools_paint_group_paint_label},
        {"tools_paint_group_paint_order_label", tools_paint_group_paint_order_label},
        {"tools_paint_group_paint_order_radio", tools_paint_group_paint_order_radio},
        {"tools_paint_group_paint_plotting_radio", tools_paint_group_paint_plotting_radio},
        {"tools_paint_group_paintmargin_entry", tools_paint_group_paintmargin_entry},
        {"tools_paint_group_paintmethod_combo", tools_paint_group_paintmethod_combo},
        {"tools_paint_group_paintoverlap_entry", tools_paint_group_paintoverlap_entry},
        {"tools_paint_group_painttooldia_entry", tools_paint_group_painttooldia_entry},
        {"tools_paint_group_pathconnect_cb", tools_paint_group_pathconnect_cb},
        {"tools_paint_group_rest_cb", tools_paint_group_rest_cb},
        {"tools_paint_group_selectmethod_combo", tools_paint_group_selectmethod_combo},
        {"tools_paint_group_tipangle_entry", tools_paint_group_tipangle_entry},
        {"tools_paint_group_tipanglelabel", tools_paint_group_tipanglelabel},
        {"tools_paint_group_tipdia_entry", tools_paint_group_tipdia_entry},
        {"tools_paint_group_tipdialabel", tools_paint_group_tipdialabel},
        {"tools_paint_group_tool_type_label", tools_paint_group_tool_type_label},
        {"tools_paint_group_tool_type_radio", tools_paint_group_tool_type_radio},
        {"tools_panelize_group", tools_panelize_group},
        {"tools_panelize_group_columns_label", tools_panelize_group_columns_label},
        {"tools_panelize_group_layout", tools_panelize_group_layout},
        {"tools_panelize_group_panel_type_label", tools_panelize_group_panel_type_label},
        {"tools_panelize_group_panel_type_radio", tools_panelize_group_panel_type_radio},
        {"tools_panelize_group_panelize_label", tools_panelize_group_panelize_label},
        {"tools_panelize_group_pcolumns", tools_panelize_group_pcolumns},
        {"tools_panelize_group_pconstrain_cb", tools_panelize_group_pconstrain_cb},
        {"tools_panelize_group_poptimization_cb", tools_panelize_group_poptimization_cb},
        {"tools_panelize_group_prows", tools_panelize_group_prows},
        {"tools_panelize_group_pspacing_columns", tools_panelize_group_pspacing_columns},
        {"tools_panelize_group_pspacing_rows", tools_panelize_group_pspacing_rows},
        {"tools_panelize_group_px_width_entry", tools_panelize_group_px_width_entry},
        {"tools_panelize_group_py_height_entry", tools_panelize_group_py_height_entry},
        {"tools_panelize_group_rows_label", tools_panelize_group_rows_label},
        {"tools_panelize_group_spacing_columns_label", tools_panelize_group_spacing_columns_label},
        {"tools_panelize_group_spacing_rows_label", tools_panelize_group_spacing_rows_label},
        {"tools_panelize_group_x_width_lbl", tools_panelize_group_x_width_lbl},
        {"tools_panelize_group_y_height_lbl", tools_panelize_group_y_height_lbl},
        {"tools_scroll_area", tools_scroll_area},
        {"tools_solderpaste_group", tools_solderpaste_group},
        {"tools_solderpaste_group_addtool_entry", tools_solderpaste_group_addtool_entry},
        {"tools_solderpaste_group_addtool_entry_lbl", tools_solderpaste_group_addtool_entry_lbl},
        {"tools_solderpaste_group_dwellfwd_entry", tools_solderpaste_group_dwellfwd_entry},
        {"tools_solderpaste_group_dwellfwd_label", tools_solderpaste_group_dwellfwd_label},
        {"tools_solderpaste_group_dwellrev_entry", tools_solderpaste_group_dwellrev_entry},
        {"tools_solderpaste_group_dwellrev_label", tools_solderpaste_group_dwellrev_label},
        {"tools_solderpaste_group_frxy_entry", tools_solderpaste_group_frxy_entry},
        {"tools_solderpaste_group_frxy_label", tools_solderpaste_group_frxy_label},
        {"tools_solderpaste_group_frz_dispense_entry", tools_solderpaste_group_frz_dispense_entry},
        {"tools_solderpaste_group_frz_dispense_label", tools_solderpaste_group_frz_dispense_label},
        {"tools_solderpaste_group_frz_entry", tools_solderpaste_group_frz_entry},
        {"tools_solderpaste_group_frz_label", tools_solderpaste_group_frz_label},
        {"tools_solderpaste_group_layout", tools_solderpaste_group_layout},
        {"tools_solderpaste_group_nozzle_tool_dia_entry", tools_solderpaste_group_nozzle_tool_dia_entry},
        {"tools_solderpaste_group_pp_combo", tools_solderpaste_group_pp_combo},
        {"tools_solderpaste_group_solderpastelabel", tools_solderpaste_group_solderpastelabel},
        {"tools_solderpaste_group_speedfwd_entry", tools_solderpaste_group_speedfwd_entry},
        {"tools_solderpaste_group_speedfwd_label", tools_solderpaste_group_speedfwd_label},
        {"tools_solderpaste_group_speedrev_entry", tools_solderpaste_group_speedrev_entry},
        {"tools_solderpaste_group_speedrev_label", tools_solderpaste_group_speedrev_label},
        {"tools_solderpaste_group_xy_toolchange_entry", tools_solderpaste_group_xy_toolchange_entry},
        {"tools_solderpaste_group_xy_toolchange_label", tools_solderpaste_group_xy_toolchange_label},
        {"tools_solderpaste_group_z_dispense_entry", tools_solderpaste_group_z_dispense_entry},
        {"tools_solderpaste_group_z_dispense_label", tools_solderpaste_group_z_dispense_label},
        {"tools_solderpaste_group_z_start_entry", tools_solderpaste_group_z_start_entry},
        {"tools_solderpaste_group_z_start_label", tools_solderpaste_group_z_start_label},
        {"tools_solderpaste_group_z_stop_entry", tools_solderpaste_group_z_stop_entry},
        {"tools_solderpaste_group_z_stop_label", tools_solderpaste_group_z_stop_label},
        {"tools_solderpaste_group_z_toolchange_entry", tools_solderpaste_group_z_toolchange_entry},
        {"tools_solderpaste_group_z_toolchange_label", tools_solderpaste_group_z_toolchange_label},
        {"tools_solderpaste_group_z_travel_entry", tools_solderpaste_group_z_travel_entry},
        {"tools_solderpaste_group_z_travel_label", tools_solderpaste_group_z_travel_label},
        {"tools_sub_group", tools_sub_group},
        {"tools_sub_group_close_paths_cb", tools_sub_group_close_paths_cb},
        {"tools_sub_group_delete_sources_cb", tools_sub_group_delete_sources_cb},
        {"tools_sub_group_layout", tools_sub_group_layout},
        {"tools_sub_group_sublabel", tools_sub_group_sublabel},
        {"tools_tab", tools_tab},
        {"tools_tab_lay", tools_tab_lay},
        {"tools_transform_group", tools_transform_group},
        {"tools_transform_group_buffer_entry", tools_transform_group_buffer_entry},
        {"tools_transform_group_buffer_factor_entry", tools_transform_group_buffer_factor_entry},
        {"tools_transform_group_buffer_factor_label", tools_transform_group_buffer_factor_label},
        {"tools_transform_group_buffer_label", tools_transform_group_buffer_label},
        {"tools_transform_group_buffer_rounded_cb", tools_transform_group_buffer_rounded_cb},
        {"tools_transform_group_layout", tools_transform_group_layout},
        {"tools_transform_group_offx_entry", tools_transform_group_offx_entry},
        {"tools_transform_group_offx_label", tools_transform_group_offx_label},
        {"tools_transform_group_offy_entry", tools_transform_group_offy_entry},
        {"tools_transform_group_offy_label", tools_transform_group_offy_label},
        {"tools_transform_group_point_entry", tools_transform_group_point_entry},
        {"tools_transform_group_point_label", tools_transform_group_point_label},
        {"tools_transform_group_ref_combo", tools_transform_group_ref_combo},
        {"tools_transform_group_rotate_entry", tools_transform_group_rotate_entry},
        {"tools_transform_group_rotate_label", tools_transform_group_rotate_label},
        {"tools_transform_group_scale_link_cb", tools_transform_group_scale_link_cb},
        {"tools_transform_group_scalex_entry", tools_transform_group_scalex_entry},
        {"tools_transform_group_scalex_label", tools_transform_group_scalex_label},
        {"tools_transform_group_scaley_entry", tools_transform_group_scaley_entry},
        {"tools_transform_group_scaley_label", tools_transform_group_scaley_label},
        {"tools_transform_group_skew_link_cb", tools_transform_group_skew_link_cb},
        {"tools_transform_group_skewx_entry", tools_transform_group_skewx_entry},
        {"tools_transform_group_skewx_label", tools_transform_group_skewx_label},
        {"tools_transform_group_skewy_entry", tools_transform_group_skewy_entry},
        {"tools_transform_group_skewy_label", tools_transform_group_skewy_label},
        {"tools_transform_group_transform_label", tools_transform_group_transform_label},
        {"tools_transform_group_type_obj_combo", tools_transform_group_type_obj_combo},
        {"tools_transform_group_type_object_label", tools_transform_group_type_object_label},
        {"util_defaults_form", util_defaults_form},
        {"vlay", vlay},
        {"vlay1", vlay1},
        {"vlay2", vlay2},
        {"vlay3", vlay3},
        {"vlay4", vlay4},
    };
}

}  // namespace flatcam::gui
