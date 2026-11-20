// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PreferencesTabUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

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
    general_app_group_unitslabel->setText(trUi_(QString::fromUtf8("<span style=\"color:red;\"><b>Units:</b></span>")));
    general_app_group_unitslabel->setToolTip(trUi_(QString::fromUtf8("The default value for FlatCAM units.\nWhatever is selected here is set every time\nFlatCAM is started.")));
    lay1->addWidget(general_app_group_unitslabel, 0, 0, 1, 1);
    general_app_group_units_radio = new RadioSet({{trUi_(QString::fromUtf8("MM")), QVariant(QString::fromUtf8("MM"))}, {trUi_(QString::fromUtf8("IN")), QVariant(QString::fromUtf8("IN"))}}, Qt::Horizontal, true, nullptr);
    general_app_group_units_radio->setValue(QVariant(QString::fromUtf8("MM")));
    lay1->addWidget(general_app_group_units_radio, 0, 1, 1, 1);
    general_app_group_precision_metric_label = new QLabel(nullptr);
    general_app_group_precision_metric_label->setText(trUi_(QString::fromUtf8("Precision MM:")));
    general_app_group_precision_metric_label->setToolTip(trUi_(QString::fromUtf8("The number of decimals used throughout the application\nwhen the set units are in METRIC system.\nAny change here require an application restart.")));
    lay1->addWidget(general_app_group_precision_metric_label, 1, 0, 1, 1);
    general_app_group_precision_metric_entry = new FCSpinner(nullptr);
    general_app_group_precision_metric_entry->setProperty("maximum", 16);
    general_app_group_precision_metric_entry->setProperty("minimum", 2);
    general_app_group_precision_metric_entry->setProperty("value", 4);
    general_app_group_precision_metric_entry->setWrapping(true);
    lay1->addWidget(general_app_group_precision_metric_entry, 1, 1, 1, 1);
    general_app_group_precision_inch_label = new QLabel(nullptr);
    general_app_group_precision_inch_label->setText(trUi_(QString::fromUtf8("Precision Inch:")));
    general_app_group_precision_inch_label->setToolTip(trUi_(QString::fromUtf8("The number of decimals used throughout the application\nwhen the set units are in INCH system.\nAny change here require an application restart.")));
    lay1->addWidget(general_app_group_precision_inch_label, 2, 0, 1, 1);
    general_app_group_precision_inch_entry = new FCSpinner(nullptr);
    general_app_group_precision_inch_entry->setProperty("maximum", 16);
    general_app_group_precision_inch_entry->setProperty("minimum", 2);
    general_app_group_precision_inch_entry->setProperty("value", 4);
    general_app_group_precision_inch_entry->setWrapping(true);
    lay1->addWidget(general_app_group_precision_inch_entry, 2, 1, 1, 1);
    general_app_group_ge_label = new QLabel(nullptr);
    general_app_group_ge_label->setText(trUi_(QString::fromUtf8("<b>Graphic Engine:</b>")));
    general_app_group_ge_label->setToolTip(trUi_(QString::fromUtf8("Choose what graphic engine to use in FlatCAM.\nLegacy(2D) -> reduced functionality, slow performance but enhanced compatibility.\nOpenGL(3D) -> full functionality, high performance\nSome graphic cards are too old and do not work in OpenGL(3D) mode, like:\nIntel HD3000 or older. In this case the plot area will be black therefore\nuse the Legacy(2D) mode.")));
    lay1->addWidget(general_app_group_ge_label, 3, 0, 1, 1);
    general_app_group_ge_radio = new RadioSet({{trUi_(QString::fromUtf8("Legacy(2D)")), QVariant(QString::fromUtf8("2D"))}, {trUi_(QString::fromUtf8("OpenGL(3D)")), QVariant(QString::fromUtf8("3D"))}}, Qt::Vertical, true, nullptr);
    general_app_group_ge_radio->setValue(QVariant(QString::fromUtf8("3D")));
    lay1->addWidget(general_app_group_ge_radio, 3, 1, 1, 1);
    auto* w2 = new QFrame(nullptr);
    w2->setProperty("frameShadow", 48);
    w2->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w2->setSizePolicy(sp); }
    lay1->addWidget(w2, 4, 0, 1, 2);
    general_app_group_app_level_label = new QLabel(nullptr);
    general_app_group_app_level_label->setText(trUi_(QString::fromUtf8("<span style=\"color:red;\"><b>APPLICATION LEVEL:</b></span>")));
    general_app_group_app_level_label->setToolTip(trUi_(QString::fromUtf8("Choose the default level of usage for FlatCAM.\nBASIC level -> reduced functionality, best for beginner's.\nADVANCED level -> full functionality.\n\nThe choice here will influence the parameters in\nthe Selected Tab for all kinds of FlatCAM objects.")));
    lay1->addWidget(general_app_group_app_level_label, 5, 0, 1, 2);
    general_app_group_app_level_radio = new RadioSet({{trUi_(QString::fromUtf8("Basic")), QVariant(QString::fromUtf8("b"))}, {trUi_(QString::fromUtf8("Advanced")), QVariant(QString::fromUtf8("a"))}}, Qt::Horizontal, true, nullptr);
    general_app_group_app_level_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay1->addWidget(general_app_group_app_level_radio, 6, 0, 1, 2);
    general_app_group_portability_cb = new FCCheckBox(nullptr);
    general_app_group_portability_cb->setText(trUi_(QString::fromUtf8("Portable app")));
    general_app_group_portability_cb->setToolTip(trUi_(QString::fromUtf8("Choose if the application should run as portable.\n\nIf Checked the application will run portable,\nwhich means that the preferences files will be saved\nin the application folder, in the lib\\config subfolder.")));
    general_app_group_portability_cb->setHidden(true);
    lay1->addWidget(general_app_group_portability_cb, 7, 0, 1, 2);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay1->addWidget(w3, 8, 0, 1, 2);
    general_app_group_languagelabel = new QLabel(nullptr);
    general_app_group_languagelabel->setText(trUi_(QString::fromUtf8("<b>Languages</b>")));
    general_app_group_languagelabel->setToolTip(trUi_(QString::fromUtf8("Set the language used throughout FlatCAM.")));
    lay1->addWidget(general_app_group_languagelabel, 9, 0, 1, 2);
    general_app_group_language_cb = new FCComboBox(nullptr);
    lay1->addWidget(general_app_group_language_cb, 10, 0, 1, 2);
    general_app_group_language_apply_btn = new FCButton(nullptr);
    general_app_group_language_apply_btn->setText(trUi_(QString::fromUtf8("Apply Language")));
    general_app_group_language_apply_btn->setToolTip(trUi_(QString::fromUtf8("Set the language used throughout FlatCAM.\nThe app will restart after click.")));
    lay1->addWidget(general_app_group_language_apply_btn, 15, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay1->addWidget(w4, 16, 0, 1, 2);
    general_app_group_startup_label = new QLabel(nullptr);
    general_app_group_startup_label->setText(trUi_(QString::fromUtf8("<b>Startup Settings</b>")));
    lay1->addWidget(general_app_group_startup_label, 17, 0, 1, 2);
    general_app_group_splash_cb = new FCCheckBox(nullptr);
    general_app_group_splash_cb->setText(trUi_(QString::fromUtf8("Splash Screen")));
    general_app_group_splash_cb->setToolTip(trUi_(QString::fromUtf8("Enable display of the splash screen at application startup.")));
    lay1->addWidget(general_app_group_splash_cb, 18, 0, 1, 2);
    general_app_group_systray_cb = new FCCheckBox(nullptr);
    general_app_group_systray_cb->setChecked(true);
    general_app_group_systray_cb->setText(trUi_(QString::fromUtf8("Sys Tray Icon")));
    general_app_group_systray_cb->setToolTip(trUi_(QString::fromUtf8("Enable display of FlatCAM icon in Sys Tray.")));
    lay1->addWidget(general_app_group_systray_cb, 19, 0, 1, 2);
    general_app_group_shell_startup_cb = new FCCheckBox(nullptr);
    general_app_group_shell_startup_cb->setText(trUi_(QString::fromUtf8("Show Shell")));
    general_app_group_shell_startup_cb->setToolTip(trUi_(QString::fromUtf8("Check this box if you want the shell to\nstart automatically at startup.")));
    lay1->addWidget(general_app_group_shell_startup_cb, 20, 0, 1, 2);
    general_app_group_project_startup_cb = new FCCheckBox(nullptr);
    general_app_group_project_startup_cb->setText(trUi_(QString::fromUtf8("Show Project")));
    general_app_group_project_startup_cb->setToolTip(trUi_(QString::fromUtf8("Check this box if you want the project/selected/tool tab area to\nto be shown automatically at startup.")));
    lay1->addWidget(general_app_group_project_startup_cb, 21, 0, 1, 2);
    general_app_group_version_check_cb = new FCCheckBox(nullptr);
    general_app_group_version_check_cb->setText(trUi_(QString::fromUtf8("Version Check")));
    general_app_group_version_check_cb->setToolTip(trUi_(QString::fromUtf8("Check this box if you want to check\nfor a new version automatically at startup.")));
    lay1->addWidget(general_app_group_version_check_cb, 22, 0, 1, 2);
    general_app_group_send_stats_cb = new FCCheckBox(nullptr);
    general_app_group_send_stats_cb->setEnabled(false);
    general_app_group_send_stats_cb->setText(trUi_(QString::fromUtf8("Send Statistics")));
    general_app_group_send_stats_cb->setToolTip(trUi_(QString::fromUtf8("Check this box if you agree to send anonymous\nstats automatically at startup, to help improve FlatCAM.")));
    lay1->addWidget(general_app_group_send_stats_cb, 23, 0, 1, 2);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay1->addWidget(w5, 24, 0, 1, 2);
    general_app_group_worker_number_label = new QLabel(nullptr);
    general_app_group_worker_number_label->setText(trUi_(QString::fromUtf8("Workers number:")));
    general_app_group_worker_number_label->setToolTip(trUi_(QString::fromUtf8("The number of Qthreads made available to the App.\nA bigger number may finish the jobs more quickly but\ndepending on your computer speed, may make the App\nunresponsive. Can have a value between 2 and 16.\nDefault value is 2.\nAfter change, it will be applied at next App start.")));
    lay1->addWidget(general_app_group_worker_number_label, 25, 0, 1, 1);
    general_app_group_worker_number_sb = new FCSpinner(nullptr);
    general_app_group_worker_number_sb->setProperty("maximum", 16);
    general_app_group_worker_number_sb->setProperty("minimum", 2);
    general_app_group_worker_number_sb->setProperty("value", 5);
    lay1->addWidget(general_app_group_worker_number_sb, 25, 1, 1, 1);
    auto* w6 = new QLabel(nullptr);
    w6->setText(trUi_(QString::fromUtf8("Geo Tolerance:")));
    w6->setToolTip(trUi_(QString::fromUtf8("This value can counter the effect of the Circle Steps\nparameter. Default value is 0.005.\nA lower value will increase the detail both in image\nand in Gcode for the circles, with a higher cost in\nperformance. Higher value will provide more\nperformance at the expense of level of detail.")));
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
    general_app_group_save_label->setText(trUi_(QString::fromUtf8("<b>Save Settings</b>")));
    lay1->addWidget(general_app_group_save_label, 28, 0, 1, 2);
    general_app_group_save_type_cb = new FCCheckBox(nullptr);
    general_app_group_save_type_cb->setChecked(true);
    general_app_group_save_type_cb->setText(trUi_(QString::fromUtf8("Save Compressed Project")));
    general_app_group_save_type_cb->setToolTip(trUi_(QString::fromUtf8("Whether to save a compressed or uncompressed project.\nWhen checked it will save a compressed FlatCAM project.")));
    lay1->addWidget(general_app_group_save_type_cb, 29, 0, 1, 2);
    general_app_group_compress_label = new QLabel(nullptr);
    general_app_group_compress_label->setText(trUi_(QString::fromUtf8("Compression:")));
    general_app_group_compress_label->setToolTip(trUi_(QString::fromUtf8("The level of compression used when saving\na FlatCAM project. Higher value means better compression\nbut require more RAM usage and more processing time.")));
    lay1->addWidget(general_app_group_compress_label, 30, 0, 1, 1);
    general_app_group_compress_spinner = new FCSpinner(nullptr);
    general_app_group_compress_spinner->setProperty("maximum", 9);
    general_app_group_compress_spinner->setProperty("value", 3);
    lay1->addWidget(general_app_group_compress_spinner, 30, 1, 1, 1);
    general_app_group_autosave_cb = new FCCheckBox(nullptr);
    general_app_group_autosave_cb->setText(trUi_(QString::fromUtf8("Enable Auto Save")));
    general_app_group_autosave_cb->setToolTip(trUi_(QString::fromUtf8("Check to enable the autosave feature.\nWhen enabled, the application will try to save a project\nat the set interval.")));
    lay1->addWidget(general_app_group_autosave_cb, 31, 0, 1, 2);
    general_app_group_autosave_label = new QLabel(nullptr);
    general_app_group_autosave_label->setText(trUi_(QString::fromUtf8("Interval:")));
    general_app_group_autosave_label->setToolTip(trUi_(QString::fromUtf8("Time interval for autosaving. In milliseconds.\nThe application will try to save periodically but only\nif the project was saved manually at least once.\nWhile active, some operations may block this feature.")));
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
    general_app_group_pdf_param_label->setText(trUi_(QString::fromUtf8("<B>Text to PDF parameters:</b>")));
    general_app_group_pdf_param_label->setToolTip(trUi_(QString::fromUtf8("Used when saving text in Code Editor or in FlatCAM Document objects.")));
    lay1->addWidget(general_app_group_pdf_param_label, 34, 0, 1, 2);
    general_app_group_tmargin_label = new QLabel(nullptr);
    general_app_group_tmargin_label->setText(trUi_(QString::fromUtf8("Top Margin:")));
    general_app_group_tmargin_label->setToolTip(trUi_(QString::fromUtf8("Distance between text body and the top of the PDF file.")));
    lay1->addWidget(general_app_group_tmargin_label, 35, 0, 1, 1);
    general_app_group_tmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_tmargin_entry->setProperty("decimals", 4);
    general_app_group_tmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_tmargin_entry->setProperty("value", 15.0);
    lay1->addWidget(general_app_group_tmargin_entry, 35, 1, 1, 1);
    general_app_group_bmargin_label = new QLabel(nullptr);
    general_app_group_bmargin_label->setText(trUi_(QString::fromUtf8("Bottom Margin:")));
    general_app_group_bmargin_label->setToolTip(trUi_(QString::fromUtf8("Distance between text body and the bottom of the PDF file.")));
    lay1->addWidget(general_app_group_bmargin_label, 36, 0, 1, 1);
    general_app_group_bmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_bmargin_entry->setProperty("decimals", 4);
    general_app_group_bmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_bmargin_entry->setProperty("value", 10.0);
    lay1->addWidget(general_app_group_bmargin_entry, 36, 1, 1, 1);
    general_app_group_lmargin_label = new QLabel(nullptr);
    general_app_group_lmargin_label->setText(trUi_(QString::fromUtf8("Left Margin:")));
    general_app_group_lmargin_label->setToolTip(trUi_(QString::fromUtf8("Distance between text body and the left of the PDF file.")));
    lay1->addWidget(general_app_group_lmargin_label, 37, 0, 1, 1);
    general_app_group_lmargin_entry = new FCDoubleSpinner(nullptr);
    general_app_group_lmargin_entry->setProperty("decimals", 4);
    general_app_group_lmargin_entry->setProperty("maximum", 10000.0);
    general_app_group_lmargin_entry->setProperty("value", 20.0);
    lay1->addWidget(general_app_group_lmargin_entry, 37, 1, 1, 1);
    general_app_group_rmargin_label = new QLabel(nullptr);
    general_app_group_rmargin_label->setText(trUi_(QString::fromUtf8("Right Margin:")));
    general_app_group_rmargin_label->setToolTip(trUi_(QString::fromUtf8("Distance between text body and the right of the PDF file.")));
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
    general_gui_group_theme_label->setText(trUi_(QString::fromUtf8("Theme:")));
    general_gui_group_theme_label->setToolTip(trUi_(QString::fromUtf8("Select a theme for the application.\nIt will theme the plot area.")));
    lay9->addWidget(general_gui_group_theme_label, 0, 0, 1, 1);
    general_gui_group_theme_radio = new RadioSet({{trUi_(QString::fromUtf8("Light")), QVariant(QString::fromUtf8("white"))}, {trUi_(QString::fromUtf8("Dark")), QVariant(QString::fromUtf8("black"))}}, Qt::Vertical, true, nullptr);
    general_gui_group_theme_radio->setValue(QVariant(QString::fromUtf8("black")));
    lay9->addWidget(general_gui_group_theme_radio, 0, 1, 1, 1);
    general_gui_group_gray_icons_cb = new FCCheckBox(nullptr);
    general_gui_group_gray_icons_cb->setChecked(true);
    general_gui_group_gray_icons_cb->setText(trUi_(QString::fromUtf8("Use Gray Icons")));
    general_gui_group_gray_icons_cb->setToolTip(trUi_(QString::fromUtf8("Check this box to use a set of icons with\na lighter (gray) color. To be used when a\nfull dark theme is applied.")));
    lay9->addWidget(general_gui_group_gray_icons_cb, 1, 0, 1, 3);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay9->addWidget(w10, 3, 0, 1, 2);
    general_gui_group_layout_label = new QLabel(nullptr);
    general_gui_group_layout_label->setText(trUi_(QString::fromUtf8("Layout:")));
    general_gui_group_layout_label->setToolTip(trUi_(QString::fromUtf8("Select a layout for the application.\nIt is applied immediately.")));
    lay9->addWidget(general_gui_group_layout_label, 4, 0, 1, 1);
    general_gui_group_layout_combo = new FCComboBox(nullptr);
    general_gui_group_layout_combo->setProperty("currentText", trUi_(QString::fromUtf8("compact")));
    general_gui_group_layout_combo->addItem(trUi_(QString::fromUtf8("standard")));
    general_gui_group_layout_combo->addItem(trUi_(QString::fromUtf8("compact")));
    general_gui_group_layout_combo->addItem(trUi_(QString::fromUtf8("minimal")));
    general_gui_group_layout_combo->setCurrentIndex(1);
    lay9->addWidget(general_gui_group_layout_combo, 4, 1, 1, 1);
    general_gui_group_style_label = new QLabel(nullptr);
    general_gui_group_style_label->setText(trUi_(QString::fromUtf8("Style:")));
    general_gui_group_style_label->setToolTip(trUi_(QString::fromUtf8("Select a style for the application.\nIt will be applied at the next app start.")));
    lay9->addWidget(general_gui_group_style_label, 5, 0, 1, 1);
    general_gui_group_style_combo = new FCComboBox(nullptr);
    general_gui_group_style_combo->setProperty("currentText", trUi_(QString::fromUtf8("Fusion")));
    general_gui_group_style_combo->addItem(trUi_(QString::fromUtf8("macintosh")));
    general_gui_group_style_combo->addItem(trUi_(QString::fromUtf8("Windows")));
    general_gui_group_style_combo->addItem(trUi_(QString::fromUtf8("Fusion")));
    general_gui_group_style_combo->setCurrentIndex(2);
    lay9->addWidget(general_gui_group_style_combo, 5, 1, 1, 1);
    general_gui_group_hdpi_cb = new FCCheckBox(nullptr);
    general_gui_group_hdpi_cb->setText(trUi_(QString::fromUtf8("HDPI Support")));
    general_gui_group_hdpi_cb->setToolTip(trUi_(QString::fromUtf8("Enable High DPI support for the application.\nIt will be applied at the next app start.")));
    lay9->addWidget(general_gui_group_hdpi_cb, 6, 0, 1, 3);
    general_gui_group_hover_cb = new FCCheckBox(nullptr);
    general_gui_group_hover_cb->setText(trUi_(QString::fromUtf8("Hover Shape")));
    general_gui_group_hover_cb->setToolTip(trUi_(QString::fromUtf8("Enable display of a hover shape for the application objects.\nIt is displayed whenever the mouse cursor is hovering\nover any kind of not-selected object.")));
    lay9->addWidget(general_gui_group_hover_cb, 8, 0, 1, 3);
    general_gui_group_selection_cb = new FCCheckBox(nullptr);
    general_gui_group_selection_cb->setChecked(true);
    general_gui_group_selection_cb->setText(trUi_(QString::fromUtf8("Selection Shape")));
    general_gui_group_selection_cb->setToolTip(trUi_(QString::fromUtf8("Enable the display of a selection shape for the application objects.\nIt is displayed whenever the mouse selects an object\neither by clicking or dragging mouse from left to right or\nright to left.")));
    lay9->addWidget(general_gui_group_selection_cb, 9, 0, 1, 3);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    lay9->addWidget(w11, 14, 0, 1, 2);
    general_gui_group_sel_lr_label = new QLabel(nullptr);
    general_gui_group_sel_lr_label->setText(trUi_(QString::fromUtf8("<b>Left-Right Selection Color</b>")));
    lay9->addWidget(general_gui_group_sel_lr_label, 15, 0, 1, 2);
    general_gui_group_sl_color_label = new QLabel(nullptr);
    general_gui_group_sl_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    general_gui_group_sl_color_label->setToolTip(trUi_(QString::fromUtf8("Set the line color for the 'left to right' selection box.")));
    lay9->addWidget(general_gui_group_sl_color_label, 16, 0, 1, 1);
    general_gui_group_sl_color_entry = new FCColorEntry(nullptr);
    general_gui_group_sl_color_entry->setValue(QString::fromUtf8("#0000ffbf"));
    lay9->addWidget(general_gui_group_sl_color_entry, 16, 1, 1, 1);
    general_gui_group_sf_color_label = new QLabel(nullptr);
    general_gui_group_sf_color_label->setText(trUi_(QString::fromUtf8("Fill:")));
    general_gui_group_sf_color_label->setToolTip(trUi_(QString::fromUtf8("Set the fill color for the selection box\nin case that the selection is done from left to right.\nFirst 6 digits are the color and the last 2\ndigits are for alpha (transparency) level.")));
    lay9->addWidget(general_gui_group_sf_color_label, 17, 0, 1, 1);
    general_gui_group_sf_color_entry = new FCColorEntry(nullptr);
    general_gui_group_sf_color_entry->setValue(QString::fromUtf8("#a5a5ffbf"));
    lay9->addWidget(general_gui_group_sf_color_entry, 17, 1, 1, 1);
    general_gui_group_left_right_alpha_label = new QLabel(nullptr);
    general_gui_group_left_right_alpha_label->setText(trUi_(QString::fromUtf8("Alpha:")));
    general_gui_group_left_right_alpha_label->setToolTip(trUi_(QString::fromUtf8("Set the fill transparency for the 'left to right' selection box.")));
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
    general_gui_group_sel_rl_label->setText(trUi_(QString::fromUtf8("<b>Right-Left Selection Color</b>")));
    lay9->addWidget(general_gui_group_sel_rl_label, 20, 0, 1, 2);
    general_gui_group_alt_sl_color_label = new QLabel(nullptr);
    general_gui_group_alt_sl_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    general_gui_group_alt_sl_color_label->setToolTip(trUi_(QString::fromUtf8("Set the line color for the 'right to left' selection box.")));
    lay9->addWidget(general_gui_group_alt_sl_color_label, 21, 0, 1, 1);
    general_gui_group_alt_sl_color_entry = new FCColorEntry(nullptr);
    general_gui_group_alt_sl_color_entry->setValue(QString::fromUtf8("#006E20bf"));
    lay9->addWidget(general_gui_group_alt_sl_color_entry, 21, 1, 1, 1);
    general_gui_group_alt_sf_color_label = new QLabel(nullptr);
    general_gui_group_alt_sf_color_label->setText(trUi_(QString::fromUtf8("Fill:")));
    general_gui_group_alt_sf_color_label->setToolTip(trUi_(QString::fromUtf8("Set the color for the shape.")));
    lay9->addWidget(general_gui_group_alt_sf_color_label, 22, 0, 1, 1);
    general_gui_group_alt_sf_color_entry = new FCColorEntry(nullptr);
    general_gui_group_alt_sf_color_entry->setValue(QString::fromUtf8("#BBF268bf"));
    lay9->addWidget(general_gui_group_alt_sf_color_entry, 22, 1, 1, 1);
    general_gui_group_right_left_alpha_label = new QLabel(nullptr);
    general_gui_group_right_left_alpha_label->setText(trUi_(QString::fromUtf8("Alpha:")));
    general_gui_group_right_left_alpha_label->setToolTip(trUi_(QString::fromUtf8("Set the fill transparency for selection 'right to left' box.")));
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
    general_gui_group_editor_color_label->setText(trUi_(QString::fromUtf8("<b>Editor Color</b>")));
    lay9->addWidget(general_gui_group_editor_color_label, 25, 0, 1, 2);
    general_gui_group_draw_color_label = new QLabel(nullptr);
    general_gui_group_draw_color_label->setText(trUi_(QString::fromUtf8("Drawing:")));
    lay9->addWidget(general_gui_group_draw_color_label, 26, 0, 1, 1);
    general_gui_group_draw_color_entry = new FCColorEntry(nullptr);
    general_gui_group_draw_color_entry->setValue(QString::fromUtf8("#FF0000"));
    lay9->addWidget(general_gui_group_draw_color_entry, 26, 1, 1, 1);
    general_gui_group_sel_draw_color_label = new QLabel(nullptr);
    general_gui_group_sel_draw_color_label->setText(trUi_(QString::fromUtf8("Selection:")));
    general_gui_group_sel_draw_color_label->setToolTip(trUi_(QString::fromUtf8("Set the color of the shape when selected.")));
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
    general_gui_group_proj_settings_label->setText(trUi_(QString::fromUtf8("<b>Project Items Color</b>")));
    lay9->addWidget(general_gui_group_proj_settings_label, 29, 0, 1, 2);
    general_gui_group_proj_color_label = new QLabel(nullptr);
    general_gui_group_proj_color_label->setText(trUi_(QString::fromUtf8("Enabled:")));
    general_gui_group_proj_color_label->setToolTip(trUi_(QString::fromUtf8("Set the color of the items in Project Tab Tree.")));
    lay9->addWidget(general_gui_group_proj_color_label, 30, 0, 1, 1);
    general_gui_group_proj_color_entry = new FCColorEntry(nullptr);
    lay9->addWidget(general_gui_group_proj_color_entry, 30, 1, 1, 1);
    general_gui_group_proj_color_dis_label = new QLabel(nullptr);
    general_gui_group_proj_color_dis_label->setText(trUi_(QString::fromUtf8("Disabled:")));
    general_gui_group_proj_color_dis_label->setToolTip(trUi_(QString::fromUtf8("Set the color of the items in Project Tab Tree,\nfor the case when the items are disabled.")));
    lay9->addWidget(general_gui_group_proj_color_dis_label, 31, 0, 1, 1);
    general_gui_group_proj_color_dis_entry = new FCColorEntry(nullptr);
    lay9->addWidget(general_gui_group_proj_color_dis_entry, 31, 1, 1, 1);
    general_gui_group_project_autohide_cb = new FCCheckBox(nullptr);
    general_gui_group_project_autohide_cb->setChecked(true);
    general_gui_group_project_autohide_cb->setText(trUi_(QString::fromUtf8("Project AutoHide")));
    general_gui_group_project_autohide_cb->setToolTip(trUi_(QString::fromUtf8("Check this box if you want the project/selected/tool tab area to\nhide automatically when there are no objects loaded and\nto show whenever a new object is created.")));
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
    general_app_set_group_grid_label->setText(trUi_(QString::fromUtf8("<b>Grid Settings</b>")));
    lay16->addWidget(general_app_set_group_grid_label, 0, 0, 1, 2);
    general_app_set_group_gridx_label = new QLabel(nullptr);
    general_app_set_group_gridx_label->setText(trUi_(QString::fromUtf8("X value:")));
    general_app_set_group_gridx_label->setToolTip(trUi_(QString::fromUtf8("This is the Grid snap value on X axis.")));
    lay16->addWidget(general_app_set_group_gridx_label, 1, 0, 1, 1);
    general_app_set_group_gridx_entry = new FCDoubleSpinner(nullptr);
    general_app_set_group_gridx_entry->setProperty("decimals", 4);
    general_app_set_group_gridx_entry->setProperty("singleStep", 0.1);
    general_app_set_group_gridx_entry->setProperty("value", 1.0);
    lay16->addWidget(general_app_set_group_gridx_entry, 1, 1, 1, 1);
    general_app_set_group_gridy_label = new QLabel(nullptr);
    general_app_set_group_gridy_label->setText(trUi_(QString::fromUtf8("Y value:")));
    general_app_set_group_gridy_label->setToolTip(trUi_(QString::fromUtf8("This is the Grid snap value on Y axis.")));
    lay16->addWidget(general_app_set_group_gridy_label, 2, 0, 1, 1);
    general_app_set_group_gridy_entry = new FCDoubleSpinner(nullptr);
    general_app_set_group_gridy_entry->setProperty("decimals", 4);
    general_app_set_group_gridy_entry->setProperty("singleStep", 0.1);
    general_app_set_group_gridy_entry->setProperty("value", 1.0);
    lay16->addWidget(general_app_set_group_gridy_entry, 2, 1, 1, 1);
    general_app_set_group_snap_max_label = new QLabel(nullptr);
    general_app_set_group_snap_max_label->setText(trUi_(QString::fromUtf8("Snap Max:")));
    general_app_set_group_snap_max_label->setToolTip(trUi_(QString::fromUtf8("Max. magnet distance")));
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
    general_app_set_group_workspace_label->setText(trUi_(QString::fromUtf8("<b>Workspace Settings</b>")));
    lay16->addWidget(general_app_set_group_workspace_label, 5, 0, 1, 2);
    general_app_set_group_workspace_cb = new FCCheckBox(nullptr);
    general_app_set_group_workspace_cb->setText(trUi_(QString::fromUtf8("Active")));
    general_app_set_group_workspace_cb->setToolTip(trUi_(QString::fromUtf8("Draw a delimiting rectangle on canvas.\nThe purpose is to illustrate the limits for our work.")));
    lay16->addWidget(general_app_set_group_workspace_cb, 6, 0, 1, 2);
    general_app_set_group_workspace_type_lbl = new QLabel(nullptr);
    general_app_set_group_workspace_type_lbl->setText(trUi_(QString::fromUtf8("Size:")));
    general_app_set_group_workspace_type_lbl->setToolTip(trUi_(QString::fromUtf8("Select the type of rectangle to be used on canvas,\nas valid workspace.")));
    lay16->addWidget(general_app_set_group_workspace_type_lbl, 7, 0, 1, 1);
    general_app_set_group_wk_cb = new FCComboBox(nullptr);
    general_app_set_group_wk_cb->setProperty("currentText", trUi_(QString::fromUtf8("A4")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A0")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A1")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A2")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A3")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A4")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A5")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A6")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A7")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A8")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A9")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("A10")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B0")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B1")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B2")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B3")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B4")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B5")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B6")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B7")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B8")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B9")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("B10")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C0")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C1")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C2")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C3")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C4")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C5")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C6")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C7")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C8")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C9")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("C10")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("LETTER")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("LEGAL")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("ELEVENSEVENTEEN")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("JUNIOR_LEGAL")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("HALF_LETTER")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("GOV_LETTER")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("GOV_LEGAL")));
    general_app_set_group_wk_cb->addItem(trUi_(QString::fromUtf8("LEDGER")));
    general_app_set_group_wk_cb->setCurrentIndex(4);
    lay16->addWidget(general_app_set_group_wk_cb, 7, 1, 1, 1);
    general_app_set_group_wk_orientation_label = new QLabel(nullptr);
    general_app_set_group_wk_orientation_label->setText(trUi_(QString::fromUtf8("Orientation:")));
    general_app_set_group_wk_orientation_label->setToolTip(trUi_(QString::fromUtf8("Can be:\n- Portrait\n- Landscape")));
    lay16->addWidget(general_app_set_group_wk_orientation_label, 8, 0, 1, 1);
    general_app_set_group_wk_orientation_radio = new RadioSet({{trUi_(QString::fromUtf8("Portrait")), QVariant(QString::fromUtf8("p"))}, {trUi_(QString::fromUtf8("Landscape")), QVariant(QString::fromUtf8("l"))}}, Qt::Horizontal, false, nullptr);
    general_app_set_group_wk_orientation_radio->setValue(QVariant(QString::fromUtf8("p")));
    lay16->addWidget(general_app_set_group_wk_orientation_radio, 8, 1, 1, 1);
    auto* w18 = new QFrame(nullptr);
    w18->setProperty("frameShadow", 48);
    w18->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w18->setSizePolicy(sp); }
    lay16->addWidget(w18, 9, 0, 1, 2);
    general_app_set_group_font_size_label = new QLabel(nullptr);
    general_app_set_group_font_size_label->setText(trUi_(QString::fromUtf8("<b>Font Size</b>")));
    lay16->addWidget(general_app_set_group_font_size_label, 10, 0, 1, 2);
    general_app_set_group_notebook_font_size_label = new QLabel(nullptr);
    general_app_set_group_notebook_font_size_label->setText(trUi_(QString::fromUtf8("Notebook:")));
    general_app_set_group_notebook_font_size_label->setToolTip(trUi_(QString::fromUtf8("This sets the font size for the elements found in the Notebook.\nThe notebook is the collapsible area in the left side of the GUI,\nand include the Project, Selected and Tool tabs.")));
    lay16->addWidget(general_app_set_group_notebook_font_size_label, 11, 0, 1, 1);
    general_app_set_group_notebook_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_notebook_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_notebook_font_size_spinner->setProperty("minimum", 8);
    general_app_set_group_notebook_font_size_spinner->setProperty("value", 12);
    general_app_set_group_notebook_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_notebook_font_size_spinner, 11, 1, 1, 1);
    general_app_set_group_axis_font_size_label = new QLabel(nullptr);
    general_app_set_group_axis_font_size_label->setText(trUi_(QString::fromUtf8("Axis:")));
    general_app_set_group_axis_font_size_label->setToolTip(trUi_(QString::fromUtf8("This sets the font size for canvas axis.")));
    lay16->addWidget(general_app_set_group_axis_font_size_label, 12, 0, 1, 1);
    general_app_set_group_axis_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_axis_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_axis_font_size_spinner->setProperty("value", 8);
    general_app_set_group_axis_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_axis_font_size_spinner, 12, 1, 1, 1);
    general_app_set_group_textbox_font_size_label = new QLabel(nullptr);
    general_app_set_group_textbox_font_size_label->setText(trUi_(QString::fromUtf8("Textbox:")));
    general_app_set_group_textbox_font_size_label->setToolTip(trUi_(QString::fromUtf8("This sets the font size for the Textbox GUI\nelements that are used in the application.")));
    lay16->addWidget(general_app_set_group_textbox_font_size_label, 13, 0, 1, 1);
    general_app_set_group_textbox_font_size_spinner = new FCSpinner(nullptr);
    general_app_set_group_textbox_font_size_spinner->setProperty("maximum", 40);
    general_app_set_group_textbox_font_size_spinner->setProperty("minimum", 8);
    general_app_set_group_textbox_font_size_spinner->setProperty("value", 10);
    general_app_set_group_textbox_font_size_spinner->setWrapping(true);
    lay16->addWidget(general_app_set_group_textbox_font_size_spinner, 13, 1, 1, 1);
    general_app_set_group_hud_font_size_label = new QLabel(nullptr);
    general_app_set_group_hud_font_size_label->setText(trUi_(QString::fromUtf8("HUD:")));
    general_app_set_group_hud_font_size_label->setToolTip(trUi_(QString::fromUtf8("This sets the font size for the Heads Up Display.")));
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
    general_app_set_group_mouse_lbl->setText(trUi_(QString::fromUtf8("<b>Mouse Settings</b>")));
    lay16->addWidget(general_app_set_group_mouse_lbl, 21, 0, 1, 2);
    general_app_set_group_cursor_lbl = new QLabel(nullptr);
    general_app_set_group_cursor_lbl->setText(trUi_(QString::fromUtf8("Cursor Shape:")));
    general_app_set_group_cursor_lbl->setToolTip(trUi_(QString::fromUtf8("Choose a mouse cursor shape.\n- Small -> with a customizable size.\n- Big -> Infinite lines")));
    lay16->addWidget(general_app_set_group_cursor_lbl, 22, 0, 1, 1);
    general_app_set_group_cursor_radio = new RadioSet({{trUi_(QString::fromUtf8("Small")), QVariant(QString::fromUtf8("small"))}, {trUi_(QString::fromUtf8("Big")), QVariant(QString::fromUtf8("big"))}}, Qt::Horizontal, false, nullptr);
    general_app_set_group_cursor_radio->setValue(QVariant(QString::fromUtf8("small")));
    lay16->addWidget(general_app_set_group_cursor_radio, 22, 1, 1, 1);
    general_app_set_group_cursor_size_lbl = new QLabel(nullptr);
    general_app_set_group_cursor_size_lbl->setText(trUi_(QString::fromUtf8("Cursor Size:")));
    general_app_set_group_cursor_size_lbl->setToolTip(trUi_(QString::fromUtf8("Set the size of the mouse cursor, in pixels.")));
    lay16->addWidget(general_app_set_group_cursor_size_lbl, 23, 0, 1, 1);
    general_app_set_group_cursor_size_entry = new FCSpinner(nullptr);
    general_app_set_group_cursor_size_entry->setProperty("maximum", 70);
    general_app_set_group_cursor_size_entry->setProperty("minimum", 10);
    general_app_set_group_cursor_size_entry->setProperty("value", 20);
    general_app_set_group_cursor_size_entry->setWrapping(true);
    lay16->addWidget(general_app_set_group_cursor_size_entry, 23, 1, 1, 1);
    general_app_set_group_cursor_width_lbl = new QLabel(nullptr);
    general_app_set_group_cursor_width_lbl->setText(trUi_(QString::fromUtf8("Cursor Width:")));
    general_app_set_group_cursor_width_lbl->setToolTip(trUi_(QString::fromUtf8("Set the line width of the mouse cursor, in pixels.")));
    lay16->addWidget(general_app_set_group_cursor_width_lbl, 24, 0, 1, 1);
    general_app_set_group_cursor_width_entry = new FCSpinner(nullptr);
    general_app_set_group_cursor_width_entry->setProperty("maximum", 10);
    general_app_set_group_cursor_width_entry->setProperty("minimum", 1);
    general_app_set_group_cursor_width_entry->setProperty("value", 2);
    general_app_set_group_cursor_width_entry->setWrapping(true);
    lay16->addWidget(general_app_set_group_cursor_width_entry, 24, 1, 1, 1);
    general_app_set_group_mouse_cursor_color_cb = new FCCheckBox(nullptr);
    general_app_set_group_mouse_cursor_color_cb->setChecked(true);
    general_app_set_group_mouse_cursor_color_cb->setText(trUi_(QString::fromUtf8("Cursor Color")));
    general_app_set_group_mouse_cursor_color_cb->setToolTip(trUi_(QString::fromUtf8("Check this box to color mouse cursor.")));
    lay16->addWidget(general_app_set_group_mouse_cursor_color_cb, 25, 0, 1, 2);
    general_app_set_group_mouse_color_label = new QLabel(nullptr);
    general_app_set_group_mouse_color_label->setText(trUi_(QString::fromUtf8("Cursor Color:")));
    general_app_set_group_mouse_color_label->setToolTip(trUi_(QString::fromUtf8("Set the color of the mouse cursor.")));
    lay16->addWidget(general_app_set_group_mouse_color_label, 26, 0, 1, 1);
    general_app_set_group_mouse_cursor_entry = new FCColorEntry(nullptr);
    general_app_set_group_mouse_cursor_entry->setValue(QString::fromUtf8("#FF0000"));
    lay16->addWidget(general_app_set_group_mouse_cursor_entry, 26, 1, 1, 1);
    general_app_set_group_panbuttonlabel = new QLabel(nullptr);
    general_app_set_group_panbuttonlabel->setText(trUi_(QString::fromUtf8("Pan Button:")));
    general_app_set_group_panbuttonlabel->setToolTip(trUi_(QString::fromUtf8("Select the mouse button to use for panning:\n- MMB --> Middle Mouse Button\n- RMB --> Right Mouse Button")));
    lay16->addWidget(general_app_set_group_panbuttonlabel, 27, 0, 1, 1);
    general_app_set_group_pan_button_radio = new RadioSet({{trUi_(QString::fromUtf8("MMB")), QVariant(QString::fromUtf8("3"))}, {trUi_(QString::fromUtf8("RMB")), QVariant(QString::fromUtf8("2"))}}, Qt::Horizontal, true, nullptr);
    general_app_set_group_pan_button_radio->setValue(QVariant(QString::fromUtf8("2")));
    lay16->addWidget(general_app_set_group_pan_button_radio, 27, 1, 1, 1);
    general_app_set_group_mselectlabel = new QLabel(nullptr);
    general_app_set_group_mselectlabel->setText(trUi_(QString::fromUtf8("Multiple Selection:")));
    general_app_set_group_mselectlabel->setToolTip(trUi_(QString::fromUtf8("Select the key used for multiple selection.")));
    lay16->addWidget(general_app_set_group_mselectlabel, 28, 0, 1, 1);
    general_app_set_group_mselect_radio = new RadioSet({{trUi_(QString::fromUtf8("CTRL")), QVariant(QString::fromUtf8("Control"))}, {trUi_(QString::fromUtf8("SHIFT")), QVariant(QString::fromUtf8("Shift"))}}, Qt::Horizontal, true, nullptr);
    general_app_set_group_mselect_radio->setValue(QVariant(QString::fromUtf8("Control")));
    lay16->addWidget(general_app_set_group_mselect_radio, 28, 1, 1, 1);
    auto* w20 = new QFrame(nullptr);
    w20->setProperty("frameShadow", 48);
    w20->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w20->setSizePolicy(sp); }
    lay16->addWidget(w20, 29, 0, 1, 2);
    general_app_set_group_delete_conf_cb = new FCCheckBox(nullptr);
    general_app_set_group_delete_conf_cb->setChecked(true);
    general_app_set_group_delete_conf_cb->setText(trUi_(QString::fromUtf8("Delete object confirmation")));
    general_app_set_group_delete_conf_cb->setToolTip(trUi_(QString::fromUtf8("When checked the application will ask for user confirmation\nwhenever the Delete object(s) event is triggered, either by\nmenu shortcut or key shortcut.")));
    lay16->addWidget(general_app_set_group_delete_conf_cb, 30, 0, 1, 2);
    general_app_set_group_allow_edit_cb = new FCCheckBox(nullptr);
    general_app_set_group_allow_edit_cb->setText(trUi_(QString::fromUtf8("Allow Edit")));
    general_app_set_group_allow_edit_cb->setToolTip(trUi_(QString::fromUtf8("When checked, the user can edit the object names in the Project Tab\nby clicking on the object name. Active after restart.")));
    lay16->addWidget(general_app_set_group_allow_edit_cb, 31, 0, 1, 2);
    general_app_set_group_open_style_cb = new FCCheckBox(nullptr);
    general_app_set_group_open_style_cb->setChecked(true);
    general_app_set_group_open_style_cb->setText(trUi_(QString::fromUtf8("\"Open\" behavior")));
    general_app_set_group_open_style_cb->setToolTip(trUi_(QString::fromUtf8("When checked the path for the last saved file is used when saving files,\nand the path for the last opened file is used when opening files.\n\nWhen unchecked the path for opening files is the one used last: either the\npath for saving files or the path for opening files.")));
    lay16->addWidget(general_app_set_group_open_style_cb, 32, 0, 1, 2);
    general_app_set_group_toggle_tooltips_cb = new FCCheckBox(nullptr);
    general_app_set_group_toggle_tooltips_cb->setChecked(true);
    general_app_set_group_toggle_tooltips_cb->setText(trUi_(QString::fromUtf8("Enable ToolTips")));
    general_app_set_group_toggle_tooltips_cb->setToolTip(trUi_(QString::fromUtf8("Check this box if you want to have toolTips displayed\nwhen hovering with mouse over items throughout the App.")));
    lay16->addWidget(general_app_set_group_toggle_tooltips_cb, 33, 0, 1, 2);
    general_app_set_group_machinist_cb = new FCCheckBox(nullptr);
    general_app_set_group_machinist_cb->setText(trUi_(QString::fromUtf8("Allow Machinist Unsafe Settings")));
    general_app_set_group_machinist_cb->setToolTip(trUi_(QString::fromUtf8("If checked, some of the application settings will be allowed\nto have values that are usually unsafe to use.\nLike Z travel negative values or Z Cut positive values.\nIt will applied at the next application start.\n<<WARNING>>: Don't change this unless you know what you are doing !!!")));
    lay16->addWidget(general_app_set_group_machinist_cb, 34, 0, 1, 2);
    general_app_set_group_bm_limit_label = new QLabel(nullptr);
    general_app_set_group_bm_limit_label->setText(trUi_(QString::fromUtf8("Bookmarks limit:")));
    general_app_set_group_bm_limit_label->setToolTip(trUi_(QString::fromUtf8("The maximum number of bookmarks that may be installed in the menu.\nThe number of bookmarks in the bookmark manager may be greater\nbut the menu will hold only so much.")));
    lay16->addWidget(general_app_set_group_bm_limit_label, 35, 0, 1, 1);
    general_app_set_group_bm_limit_spinner = new FCSpinner(nullptr);
    general_app_set_group_bm_limit_spinner->setProperty("maximum", 9999);
    general_app_set_group_bm_limit_spinner->setProperty("value", 10);
    lay16->addWidget(general_app_set_group_bm_limit_spinner, 35, 1, 1, 1);
    general_app_set_group_activity_label = new QLabel(nullptr);
    general_app_set_group_activity_label->setText(trUi_(QString::fromUtf8("Activity Icon:")));
    general_app_set_group_activity_label->setToolTip(trUi_(QString::fromUtf8("Select the GIF that show activity when FlatCAM is active.")));
    lay16->addWidget(general_app_set_group_activity_label, 36, 0, 1, 1);
    general_app_set_group_activity_combo = new FCComboBox(nullptr);
    general_app_set_group_activity_combo->setProperty("currentText", trUi_(QString::fromUtf8("Ball green")));
    general_app_set_group_activity_combo->addItem(trUi_(QString::fromUtf8("Ball black")));
    general_app_set_group_activity_combo->addItem(trUi_(QString::fromUtf8("Ball green")));
    general_app_set_group_activity_combo->addItem(trUi_(QString::fromUtf8("Arrow green")));
    general_app_set_group_activity_combo->addItem(trUi_(QString::fromUtf8("Eclipse green")));
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
    pref_tab_area->addTab(general_tab, trUi_(QString::fromUtf8("General")));
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
    gerber_gen_group_plot_options_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    gerber_gen_group_plot_options_label->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    gerber_gen_group_layout->addWidget(gerber_gen_group_plot_options_label, 0);
    auto* lay21 = new QGridLayout();
    lay21->setContentsMargins(0, 0, 0, 0);
    lay21->setHorizontalSpacing(6);
    lay21->setVerticalSpacing(6);
    gerber_gen_group_plot_cb = new FCCheckBox(nullptr);
    gerber_gen_group_plot_cb->setChecked(true);
    gerber_gen_group_plot_cb->setText(trUi_(QString::fromUtf8("Plot")));
    lay21->addWidget(gerber_gen_group_plot_cb, 0, 0, 1, 1);
    gerber_gen_group_solid_cb = new FCCheckBox(nullptr);
    gerber_gen_group_solid_cb->setChecked(true);
    gerber_gen_group_solid_cb->setText(trUi_(QString::fromUtf8("Solid")));
    gerber_gen_group_solid_cb->setToolTip(trUi_(QString::fromUtf8("Solid color polygons.")));
    lay21->addWidget(gerber_gen_group_solid_cb, 0, 1, 1, 1);
    gerber_gen_group_multicolored_cb = new FCCheckBox(nullptr);
    gerber_gen_group_multicolored_cb->setText(trUi_(QString::fromUtf8("M-Color")));
    gerber_gen_group_multicolored_cb->setToolTip(trUi_(QString::fromUtf8("Draw polygons in different colors.")));
    lay21->addWidget(gerber_gen_group_multicolored_cb, 0, 2, 1, 1);
    gerber_gen_group_circle_steps_label = new QLabel(nullptr);
    gerber_gen_group_circle_steps_label->setText(trUi_(QString::fromUtf8("Circle Steps:")));
    gerber_gen_group_circle_steps_label->setToolTip(trUi_(QString::fromUtf8("The number of circle steps for Gerber \ncircular aperture linear approximation.")));
    lay21->addWidget(gerber_gen_group_circle_steps_label, 1, 0, 1, 1);
    gerber_gen_group_circle_steps_entry = new FCSpinner(nullptr);
    gerber_gen_group_circle_steps_entry->setProperty("maximum", 9999);
    gerber_gen_group_circle_steps_entry->setProperty("value", 64);
    lay21->addWidget(gerber_gen_group_circle_steps_entry, 1, 1, 1, 2);
    auto* w22 = new QLabel(nullptr);
    lay21->addWidget(w22, 2, 0, 1, 3);
    gerber_gen_group_gerber_default_label = new QLabel(nullptr);
    gerber_gen_group_gerber_default_label->setText(trUi_(QString::fromUtf8("<b>Default Values:</b>")));
    gerber_gen_group_gerber_default_label->setToolTip(trUi_(QString::fromUtf8("Those values will be used as fallback values\nin case that they are not found in the Gerber file.")));
    lay21->addWidget(gerber_gen_group_gerber_default_label, 3, 0, 1, 3);
    gerber_gen_group_gerber_units_label = new QLabel(nullptr);
    gerber_gen_group_gerber_units_label->setText(trUi_(QString::fromUtf8("Units:")));
    gerber_gen_group_gerber_units_label->setToolTip(trUi_(QString::fromUtf8("The units used in the Gerber file.")));
    lay21->addWidget(gerber_gen_group_gerber_units_label, 4, 0, 1, 1);
    gerber_gen_group_gerber_units_radio = new RadioSet({{trUi_(QString::fromUtf8("Inch")), QVariant(QString::fromUtf8("IN"))}, {trUi_(QString::fromUtf8("mm")), QVariant(QString::fromUtf8("MM"))}}, Qt::Horizontal, true, nullptr);
    gerber_gen_group_gerber_units_radio->setToolTip(trUi_(QString::fromUtf8("The units used in the Gerber file.")));
    gerber_gen_group_gerber_units_radio->setValue(QVariant(QString::fromUtf8("IN")));
    lay21->addWidget(gerber_gen_group_gerber_units_radio, 4, 1, 1, 2);
    gerber_gen_group_gerber_zeros_label = new QLabel(nullptr);
    gerber_gen_group_gerber_zeros_label->setProperty("alignment", 1);
    gerber_gen_group_gerber_zeros_label->setText(trUi_(QString::fromUtf8("Zeros:")));
    gerber_gen_group_gerber_zeros_label->setToolTip(trUi_(QString::fromUtf8("This sets the type of Gerber zeros.\nIf LZ then Leading Zeros are removed and\nTrailing Zeros are kept.\nIf TZ is checked then Trailing Zeros are removed\nand Leading Zeros are kept.")));
    lay21->addWidget(gerber_gen_group_gerber_zeros_label, 5, 0, 1, 1);
    gerber_gen_group_gerber_zeros_radio = new RadioSet({{trUi_(QString::fromUtf8("LZ")), QVariant(QString::fromUtf8("L"))}, {trUi_(QString::fromUtf8("TZ")), QVariant(QString::fromUtf8("T"))}}, Qt::Horizontal, true, nullptr);
    gerber_gen_group_gerber_zeros_radio->setToolTip(trUi_(QString::fromUtf8("This sets the type of Gerber zeros.\nIf LZ then Leading Zeros are removed and\nTrailing Zeros are kept.\nIf TZ is checked then Trailing Zeros are removed\nand Leading Zeros are kept.")));
    gerber_gen_group_gerber_zeros_radio->setValue(QVariant(QString::fromUtf8("L")));
    lay21->addWidget(gerber_gen_group_gerber_zeros_radio, 5, 1, 1, 2);
    auto* w23 = new QFrame(nullptr);
    w23->setProperty("frameShadow", 48);
    w23->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w23->setSizePolicy(sp); }
    lay21->addWidget(w23, 6, 0, 1, 3);
    gerber_gen_group_gerber_clean_cb = new FCCheckBox(nullptr);
    gerber_gen_group_gerber_clean_cb->setChecked(true);
    gerber_gen_group_gerber_clean_cb->setText(trUi_(QString::fromUtf8("Clean Apertures")));
    gerber_gen_group_gerber_clean_cb->setToolTip(trUi_(QString::fromUtf8("Will remove apertures that do not have geometry\nthus lowering the number of apertures in the Gerber object.")));
    lay21->addWidget(gerber_gen_group_gerber_clean_cb, 7, 0, 1, 3);
    gerber_gen_group_gerber_extra_buffering = new FCCheckBox(nullptr);
    gerber_gen_group_gerber_extra_buffering->setChecked(true);
    gerber_gen_group_gerber_extra_buffering->setText(trUi_(QString::fromUtf8("Polarity change buffer")));
    gerber_gen_group_gerber_extra_buffering->setToolTip(trUi_(QString::fromUtf8("Will apply extra buffering for the\nsolid geometry when we have polarity changes.\nMay help loading Gerber files that otherwise\ndo not load correctly.")));
    lay21->addWidget(gerber_gen_group_gerber_extra_buffering, 8, 0, 1, 3);
    gerber_gen_group_store_colors_cb = new FCCheckBox(nullptr);
    gerber_gen_group_store_colors_cb->setChecked(true);
    gerber_gen_group_store_colors_cb->setText(trUi_(QString::fromUtf8("Store colors")));
    gerber_gen_group_store_colors_cb->setToolTip(trUi_(QString::fromUtf8("It will store the set colors for Gerber objects.\nThose will be used each time the application is started.")));
    lay21->addWidget(gerber_gen_group_store_colors_cb, 11, 0, 1, 1);
    gerber_gen_group_clear_colors_button = new FCButton(nullptr);
    gerber_gen_group_clear_colors_button->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    gerber_gen_group_clear_colors_button->setText(trUi_(QString::fromUtf8("Clear Colors")));
    gerber_gen_group_clear_colors_button->setToolTip(trUi_(QString::fromUtf8("Reset the colors associated with Gerber objects.")));
    lay21->addWidget(gerber_gen_group_clear_colors_button, 11, 1, 1, 2);
    auto* w24 = new QFrame(nullptr);
    w24->setProperty("frameShadow", 48);
    w24->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w24->setSizePolicy(sp); }
    lay21->addWidget(w24, 13, 0, 1, 3);
    gerber_gen_group_gerber_color_label = new QLabel(nullptr);
    gerber_gen_group_gerber_color_label->setText(trUi_(QString::fromUtf8("<b>Object Color</b>")));
    lay21->addWidget(gerber_gen_group_gerber_color_label, 15, 0, 1, 3);
    gerber_gen_group_line_color_label = new QLabel(nullptr);
    gerber_gen_group_line_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    gerber_gen_group_line_color_label->setToolTip(trUi_(QString::fromUtf8("Set the line color for plotted objects.")));
    lay21->addWidget(gerber_gen_group_line_color_label, 17, 0, 1, 1);
    gerber_gen_group_line_color_entry = new FCColorEntry(nullptr);
    gerber_gen_group_line_color_entry->setValue(QString::fromUtf8("#006E20bf"));
    lay21->addWidget(gerber_gen_group_line_color_entry, 17, 1, 1, 2);
    gerber_gen_group_fill_color_label = new QLabel(nullptr);
    gerber_gen_group_fill_color_label->setText(trUi_(QString::fromUtf8("Fill:")));
    gerber_gen_group_fill_color_label->setToolTip(trUi_(QString::fromUtf8("Set the fill color for plotted objects.\nFirst 6 digits are the color and the last 2\ndigits are for alpha (transparency) level.")));
    lay21->addWidget(gerber_gen_group_fill_color_label, 20, 0, 1, 1);
    gerber_gen_group_fill_color_entry = new FCColorEntry(nullptr);
    gerber_gen_group_fill_color_entry->setValue(QString::fromUtf8("#BBF268bf"));
    lay21->addWidget(gerber_gen_group_fill_color_entry, 20, 1, 1, 2);
    gerber_gen_group_gerber_alpha_label = new QLabel(nullptr);
    gerber_gen_group_gerber_alpha_label->setText(trUi_(QString::fromUtf8("Alpha:")));
    gerber_gen_group_gerber_alpha_label->setToolTip(trUi_(QString::fromUtf8("Set the fill transparency for plotted objects.")));
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
    gerber_opt_group_clearcopper_label->setText(trUi_(QString::fromUtf8("<b>Non-copper regions:</b>")));
    gerber_opt_group_clearcopper_label->setToolTip(trUi_(QString::fromUtf8("Create polygons covering the\nareas without copper on the PCB.\nEquivalent to the inverse of this\nobject. Can be used to remove all\ncopper from a specified region.")));
    gerber_opt_group_layout->addWidget(gerber_opt_group_clearcopper_label, 0);
    auto* lay25 = new QGridLayout();
    lay25->setContentsMargins(0, 0, 0, 0);
    lay25->setHorizontalSpacing(6);
    lay25->setVerticalSpacing(6);
    auto* w26 = new QLabel(nullptr);
    w26->setText(trUi_(QString::fromUtf8("Boundary Margin:")));
    w26->setToolTip(trUi_(QString::fromUtf8("Specify the edge of the PCB\nby drawing a box around all\nobjects with this minimum\ndistance.")));
    lay25->addWidget(w26, 0, 0, 1, 1);
    gerber_opt_group_noncopper_margin_entry = new FCDoubleSpinner(nullptr);
    gerber_opt_group_noncopper_margin_entry->setProperty("decimals", 4);
    gerber_opt_group_noncopper_margin_entry->setProperty("maximum", 9999.0);
    gerber_opt_group_noncopper_margin_entry->setProperty("minimum", -9999.0);
    gerber_opt_group_noncopper_margin_entry->setProperty("singleStep", 0.1);
    gerber_opt_group_noncopper_margin_entry->setProperty("value", 0.1);
    lay25->addWidget(gerber_opt_group_noncopper_margin_entry, 0, 1, 1, 1);
    gerber_opt_group_noncopper_rounded_cb = new FCCheckBox(nullptr);
    gerber_opt_group_noncopper_rounded_cb->setText(trUi_(QString::fromUtf8("Rounded Geo")));
    gerber_opt_group_noncopper_rounded_cb->setToolTip(trUi_(QString::fromUtf8("Resulting geometry will have rounded corners.")));
    lay25->addWidget(gerber_opt_group_noncopper_rounded_cb, 1, 0, 1, 2);
    auto* w27 = new QFrame(nullptr);
    w27->setProperty("frameShadow", 48);
    w27->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w27->setSizePolicy(sp); }
    lay25->addWidget(w27, 2, 0, 1, 2);
    gerber_opt_group_layout->addLayout(lay25, 0);
    gerber_opt_group_boundingbox_label = new QLabel(nullptr);
    gerber_opt_group_boundingbox_label->setText(trUi_(QString::fromUtf8("<b>Bounding Box:</b>")));
    gerber_opt_group_layout->addWidget(gerber_opt_group_boundingbox_label, 0);
    auto* lay28 = new QGridLayout();
    lay28->setContentsMargins(0, 0, 0, 0);
    lay28->setHorizontalSpacing(6);
    lay28->setVerticalSpacing(6);
    auto* w29 = new QLabel(nullptr);
    w29->setText(trUi_(QString::fromUtf8("Boundary Margin:")));
    w29->setToolTip(trUi_(QString::fromUtf8("Distance of the edges of the box\nto the nearest polygon.")));
    lay28->addWidget(w29, 0, 0, 1, 1);
    gerber_opt_group_bbmargin_entry = new FCDoubleSpinner(nullptr);
    gerber_opt_group_bbmargin_entry->setProperty("decimals", 4);
    gerber_opt_group_bbmargin_entry->setProperty("maximum", 9999.0);
    gerber_opt_group_bbmargin_entry->setProperty("minimum", -9999.0);
    gerber_opt_group_bbmargin_entry->setProperty("singleStep", 0.1);
    gerber_opt_group_bbmargin_entry->setProperty("value", 0.1);
    lay28->addWidget(gerber_opt_group_bbmargin_entry, 0, 1, 1, 1);
    gerber_opt_group_bbrounded_cb = new FCCheckBox(nullptr);
    gerber_opt_group_bbrounded_cb->setText(trUi_(QString::fromUtf8("Rounded Geo")));
    gerber_opt_group_bbrounded_cb->setToolTip(trUi_(QString::fromUtf8("If the bounding box is \nto have rounded corners\ntheir radius is equal to\nthe margin.")));
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
    gerber_adv_opt_group_adv_param_label->setText(trUi_(QString::fromUtf8("<b>Advanced Options:</b>")));
    gerber_adv_opt_group_adv_param_label->setToolTip(trUi_(QString::fromUtf8("A list of advanced parameters.\nThose parameters are available only for\nAdvanced App. Level.")));
    gerber_adv_opt_group_layout->addWidget(gerber_adv_opt_group_adv_param_label, 0);
    auto* lay30 = new QGridLayout();
    lay30->setContentsMargins(0, 0, 0, 0);
    lay30->setHorizontalSpacing(6);
    lay30->setVerticalSpacing(6);
    gerber_adv_opt_group_follow_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_follow_cb->setText(trUi_(QString::fromUtf8("\"Follow\"")));
    gerber_adv_opt_group_follow_cb->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    lay30->addWidget(gerber_adv_opt_group_follow_cb, 0, 0, 1, 2);
    gerber_adv_opt_group_aperture_table_visibility_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_aperture_table_visibility_cb->setText(trUi_(QString::fromUtf8("Table Show/Hide")));
    gerber_adv_opt_group_aperture_table_visibility_cb->setToolTip(trUi_(QString::fromUtf8("Toggle the display of the Tools Table.")));
    lay30->addWidget(gerber_adv_opt_group_aperture_table_visibility_cb, 1, 0, 1, 2);
    auto* w31 = new QFrame(nullptr);
    w31->setProperty("frameShadow", 48);
    w31->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w31->setSizePolicy(sp); }
    lay30->addWidget(w31, 2, 0, 1, 2);
    auto* w32 = new QLabel(nullptr);
    w32->setText(trUi_(QString::fromUtf8("Buffering:")));
    w32->setToolTip(trUi_(QString::fromUtf8("Buffering type:\n- None --> best performance, fast file loading but no so good display\n- Full --> slow file loading but good visuals. This is the default.\n<<WARNING>>: Don't change this unless you know what you are doing !!!")));
    lay30->addWidget(w32, 9, 0, 1, 1);
    gerber_adv_opt_group_buffering_radio = new RadioSet({{trUi_(QString::fromUtf8("None")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Full")), QVariant(QString::fromUtf8("full"))}}, Qt::Horizontal, true, nullptr);
    gerber_adv_opt_group_buffering_radio->setValue(QVariant(QString::fromUtf8("full")));
    lay30->addWidget(gerber_adv_opt_group_buffering_radio, 9, 1, 1, 1);
    gerber_adv_opt_group_delayed_buffer_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_delayed_buffer_cb->setChecked(true);
    gerber_adv_opt_group_delayed_buffer_cb->setEnabled(false);
    gerber_adv_opt_group_delayed_buffer_cb->setText(trUi_(QString::fromUtf8("Delayed Buffering")));
    gerber_adv_opt_group_delayed_buffer_cb->setToolTip(trUi_(QString::fromUtf8("When checked it will do the buffering in background.")));
    lay30->addWidget(gerber_adv_opt_group_delayed_buffer_cb, 10, 0, 1, 2);
    gerber_adv_opt_group_simplify_cb = new FCCheckBox(nullptr);
    gerber_adv_opt_group_simplify_cb->setText(trUi_(QString::fromUtf8("Simplify")));
    gerber_adv_opt_group_simplify_cb->setToolTip(trUi_(QString::fromUtf8("When checked all the Gerber polygons will be\nloaded with simplification having a set tolerance.\n<<WARNING>>: Don't change this unless you know what you are doing !!!")));
    lay30->addWidget(gerber_adv_opt_group_simplify_cb, 11, 0, 1, 2);
    gerber_adv_opt_group_simplification_tol_label = new QLabel(nullptr);
    gerber_adv_opt_group_simplification_tol_label->setEnabled(false);
    gerber_adv_opt_group_simplification_tol_label->setText(trUi_(QString::fromUtf8("Tolerance")));
    gerber_adv_opt_group_simplification_tol_label->setToolTip(trUi_(QString::fromUtf8("Tolerance for polygon simplification.")));
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
    gerber_exp_group_export_options_label->setText(trUi_(QString::fromUtf8("<b>Export Options:</b>")));
    gerber_exp_group_export_options_label->setToolTip(trUi_(QString::fromUtf8("The parameters set here are used in the file exported\nwhen using the File -> Export -> Export Gerber menu entry.")));
    gerber_exp_group_layout->addWidget(gerber_exp_group_export_options_label, 0);
    auto* lay33 = new QFormLayout();
    lay33->setContentsMargins(0, 0, 0, 0);
    lay33->setHorizontalSpacing(6);
    lay33->setVerticalSpacing(6);
    gerber_exp_group_gerber_units_label = new QLabel(nullptr);
    gerber_exp_group_gerber_units_label->setText(trUi_(QString::fromUtf8("Units:")));
    gerber_exp_group_gerber_units_label->setToolTip(trUi_(QString::fromUtf8("The units used in the Gerber file.")));
    lay33->setWidget(0, QFormLayout::ItemRole(0), gerber_exp_group_gerber_units_label);
    gerber_exp_group_gerber_units_radio = new RadioSet({{trUi_(QString::fromUtf8("Inch")), QVariant(QString::fromUtf8("IN"))}, {trUi_(QString::fromUtf8("mm")), QVariant(QString::fromUtf8("MM"))}}, Qt::Horizontal, true, nullptr);
    gerber_exp_group_gerber_units_radio->setToolTip(trUi_(QString::fromUtf8("The units used in the Gerber file.")));
    gerber_exp_group_gerber_units_radio->setValue(QVariant(QString::fromUtf8("IN")));
    lay33->setWidget(0, QFormLayout::ItemRole(1), gerber_exp_group_gerber_units_radio);
    gerber_exp_group_digits_label = new QLabel(nullptr);
    gerber_exp_group_digits_label->setText(trUi_(QString::fromUtf8("Int/Decimals:")));
    gerber_exp_group_digits_label->setToolTip(trUi_(QString::fromUtf8("The number of digits in the whole part of the number\nand in the fractional part of the number.")));
    lay33->setWidget(1, QFormLayout::ItemRole(0), gerber_exp_group_digits_label);
    auto* lay34 = new QHBoxLayout();
    lay34->setContentsMargins(0, 0, 0, 0);
    lay34->setSpacing(6);
    gerber_exp_group_format_whole_entry = new FCSpinner(nullptr);
    gerber_exp_group_format_whole_entry->setProperty("maximum", 9);
    gerber_exp_group_format_whole_entry->setMinimumSize(QSize(30, 0));
    gerber_exp_group_format_whole_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe whole part of Gerber coordinates.")));
    gerber_exp_group_format_whole_entry->setProperty("value", 2);
    gerber_exp_group_format_whole_entry->setWrapping(true);
    lay34->addWidget(gerber_exp_group_format_whole_entry, 1);
    auto* w35 = new QLabel(nullptr);
    w35->setMaximumSize(QSize(5, 16777215));
    w35->setMinimumSize(QSize(5, 0));
    w35->setText(trUi_(QString::fromUtf8(":")));
    lay34->addWidget(w35, 1);
    gerber_exp_group_format_dec_entry = new FCSpinner(nullptr);
    gerber_exp_group_format_dec_entry->setProperty("maximum", 9);
    gerber_exp_group_format_dec_entry->setMinimumSize(QSize(30, 0));
    gerber_exp_group_format_dec_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe decimal part of Gerber coordinates.")));
    gerber_exp_group_format_dec_entry->setProperty("value", 4);
    gerber_exp_group_format_dec_entry->setWrapping(true);
    lay34->addWidget(gerber_exp_group_format_dec_entry, 1);
    lay34->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    lay33->setLayout(1, QFormLayout::ItemRole(1), lay34);
    gerber_exp_group_zeros_label = new QLabel(nullptr);
    gerber_exp_group_zeros_label->setProperty("alignment", 1);
    gerber_exp_group_zeros_label->setText(trUi_(QString::fromUtf8("Zeros:")));
    gerber_exp_group_zeros_label->setToolTip(trUi_(QString::fromUtf8("This sets the type of Gerber zeros.\nIf LZ then Leading Zeros are removed and\nTrailing Zeros are kept.\nIf TZ is checked then Trailing Zeros are removed\nand Leading Zeros are kept.")));
    lay33->setWidget(2, QFormLayout::ItemRole(0), gerber_exp_group_zeros_label);
    gerber_exp_group_zeros_radio = new RadioSet({{trUi_(QString::fromUtf8("LZ")), QVariant(QString::fromUtf8("L"))}, {trUi_(QString::fromUtf8("TZ")), QVariant(QString::fromUtf8("T"))}}, Qt::Horizontal, true, nullptr);
    gerber_exp_group_zeros_radio->setToolTip(trUi_(QString::fromUtf8("This sets the type of Gerber zeros.\nIf LZ then Leading Zeros are removed and\nTrailing Zeros are kept.\nIf TZ is checked then Trailing Zeros are removed\nand Leading Zeros are kept.")));
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
    gerber_editor_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    gerber_editor_group_param_label->setToolTip(trUi_(QString::fromUtf8("A list of Gerber Editor parameters.")));
    gerber_editor_group_layout->addWidget(gerber_editor_group_param_label, 0);
    auto* lay36 = new QGridLayout();
    lay36->setContentsMargins(0, 0, 0, 0);
    lay36->setHorizontalSpacing(6);
    lay36->setVerticalSpacing(6);
    gerber_editor_group_sel_limit_label = new QLabel(nullptr);
    gerber_editor_group_sel_limit_label->setText(trUi_(QString::fromUtf8("Selection limit:")));
    gerber_editor_group_sel_limit_label->setToolTip(trUi_(QString::fromUtf8("Set the number of selected Gerber geometry\nitems above which the utility geometry\nbecomes just a selection rectangle.\nIncreases the performance when moving a\nlarge number of geometric elements.")));
    lay36->addWidget(gerber_editor_group_sel_limit_label, 0, 0, 1, 1);
    gerber_editor_group_sel_limit_entry = new FCSpinner(nullptr);
    gerber_editor_group_sel_limit_entry->setProperty("maximum", 9999);
    gerber_editor_group_sel_limit_entry->setProperty("value", 30);
    lay36->addWidget(gerber_editor_group_sel_limit_entry, 0, 1, 1, 1);
    gerber_editor_group_addcode_entry_lbl = new QLabel(nullptr);
    gerber_editor_group_addcode_entry_lbl->setText(trUi_(QString::fromUtf8("New Aperture code:")));
    gerber_editor_group_addcode_entry_lbl->setToolTip(trUi_(QString::fromUtf8("Code for the new aperture")));
    lay36->addWidget(gerber_editor_group_addcode_entry_lbl, 1, 0, 1, 1);
    gerber_editor_group_addcode_entry = new FCSpinner(nullptr);
    gerber_editor_group_addcode_entry->setProperty("minimum", 10);
    gerber_editor_group_addcode_entry->setProperty("value", 10);
    gerber_editor_group_addcode_entry->setWrapping(true);
    lay36->addWidget(gerber_editor_group_addcode_entry, 1, 1, 1, 1);
    gerber_editor_group_addsize_entry_lbl = new QLabel(nullptr);
    gerber_editor_group_addsize_entry_lbl->setText(trUi_(QString::fromUtf8("New Aperture size:")));
    gerber_editor_group_addsize_entry_lbl->setToolTip(trUi_(QString::fromUtf8("Size for the new aperture")));
    lay36->addWidget(gerber_editor_group_addsize_entry_lbl, 2, 0, 1, 1);
    gerber_editor_group_addsize_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_addsize_entry->setProperty("decimals", 4);
    gerber_editor_group_addsize_entry->setProperty("maximum", 100.0);
    gerber_editor_group_addsize_entry->setProperty("value", 0.8);
    lay36->addWidget(gerber_editor_group_addsize_entry, 2, 1, 1, 1);
    gerber_editor_group_addtype_combo_lbl = new QLabel(nullptr);
    gerber_editor_group_addtype_combo_lbl->setText(trUi_(QString::fromUtf8("New Aperture type:")));
    gerber_editor_group_addtype_combo_lbl->setToolTip(trUi_(QString::fromUtf8("Type for the new aperture.\nCan be 'C', 'R' or 'O'.")));
    lay36->addWidget(gerber_editor_group_addtype_combo_lbl, 3, 0, 1, 1);
    gerber_editor_group_addtype_combo = new FCComboBox(nullptr);
    gerber_editor_group_addtype_combo->setProperty("currentText", trUi_(QString::fromUtf8("C")));
    gerber_editor_group_addtype_combo->addItem(trUi_(QString::fromUtf8("C")));
    gerber_editor_group_addtype_combo->addItem(trUi_(QString::fromUtf8("R")));
    gerber_editor_group_addtype_combo->addItem(trUi_(QString::fromUtf8("O")));
    lay36->addWidget(gerber_editor_group_addtype_combo, 3, 1, 1, 1);
    gerber_editor_group_grb_array_size_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_size_label->setText(trUi_(QString::fromUtf8("Nr of pads:")));
    gerber_editor_group_grb_array_size_label->setToolTip(trUi_(QString::fromUtf8("Specify how many pads to be in the array.")));
    lay36->addWidget(gerber_editor_group_grb_array_size_label, 4, 0, 1, 1);
    gerber_editor_group_grb_array_size_entry = new FCSpinner(nullptr);
    gerber_editor_group_grb_array_size_entry->setProperty("maximum", 9999);
    gerber_editor_group_grb_array_size_entry->setProperty("value", 5);
    lay36->addWidget(gerber_editor_group_grb_array_size_entry, 4, 1, 1, 1);
    gerber_editor_group_adddim_label = new QLabel(nullptr);
    gerber_editor_group_adddim_label->setText(trUi_(QString::fromUtf8("Aperture Dimensions:")));
    gerber_editor_group_adddim_label->setToolTip(trUi_(QString::fromUtf8("Diameters of the tools, separated by comma.\nThe value of the diameter has to use the dot decimals separator.\nValid values: 0.3, 1.0")));
    lay36->addWidget(gerber_editor_group_adddim_label, 5, 0, 1, 1);
    gerber_editor_group_adddim_entry = new NumericalEvalTupleEntry(nullptr);
    gerber_editor_group_adddim_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    gerber_editor_group_adddim_entry->setText(trUi_(QString::fromUtf8("0.5, 0.5")));
    lay36->addWidget(gerber_editor_group_adddim_entry, 5, 1, 1, 1);
    gerber_editor_group_grb_array_linear_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_linear_label->setText(trUi_(QString::fromUtf8("<b>Linear Pad Array:</b>")));
    lay36->addWidget(gerber_editor_group_grb_array_linear_label, 6, 0, 1, 2);
    gerber_editor_group_grb_axis_label = new QLabel(nullptr);
    gerber_editor_group_grb_axis_label->setText(trUi_(QString::fromUtf8("Linear Direction:")));
    gerber_editor_group_grb_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the linear array is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the array inclination")));
    lay36->addWidget(gerber_editor_group_grb_axis_label, 7, 0, 1, 1);
    gerber_editor_group_grb_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    gerber_editor_group_grb_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay36->addWidget(gerber_editor_group_grb_axis_radio, 7, 1, 1, 1);
    gerber_editor_group_grb_pitch_label = new QLabel(nullptr);
    gerber_editor_group_grb_pitch_label->setText(trUi_(QString::fromUtf8("Pitch:")));
    gerber_editor_group_grb_pitch_label->setToolTip(trUi_(QString::fromUtf8("Pitch = Distance between elements of the array.")));
    lay36->addWidget(gerber_editor_group_grb_pitch_label, 8, 0, 1, 1);
    gerber_editor_group_grb_pitch_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_pitch_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_pitch_entry->setProperty("value", 0.1);
    lay36->addWidget(gerber_editor_group_grb_pitch_entry, 8, 1, 1, 1);
    gerber_editor_group_grb_angle_label = new QLabel(nullptr);
    gerber_editor_group_grb_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    gerber_editor_group_grb_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    lay36->addWidget(gerber_editor_group_grb_angle_label, 9, 0, 1, 1);
    gerber_editor_group_grb_angle_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_angle_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_angle_entry->setProperty("maximum", 360.0);
    gerber_editor_group_grb_angle_entry->setProperty("minimum", -360.0);
    gerber_editor_group_grb_angle_entry->setProperty("singleStep", 5.0);
    lay36->addWidget(gerber_editor_group_grb_angle_entry, 9, 1, 1, 1);
    gerber_editor_group_grb_array_circ_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_circ_label->setText(trUi_(QString::fromUtf8("<b>Circular Pad Array:</b>")));
    lay36->addWidget(gerber_editor_group_grb_array_circ_label, 10, 0, 1, 2);
    gerber_editor_group_grb_circular_direction_label = new QLabel(nullptr);
    gerber_editor_group_grb_circular_direction_label->setText(trUi_(QString::fromUtf8("Circular Direction:")));
    gerber_editor_group_grb_circular_direction_label->setToolTip(trUi_(QString::fromUtf8("Direction for circular array.\nCan be CW = clockwise or CCW = counter clockwise.")));
    lay36->addWidget(gerber_editor_group_grb_circular_direction_label, 11, 0, 1, 1);
    gerber_editor_group_grb_circular_dir_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    gerber_editor_group_grb_circular_dir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay36->addWidget(gerber_editor_group_grb_circular_dir_radio, 11, 1, 1, 1);
    gerber_editor_group_grb_circular_angle_label = new QLabel(nullptr);
    gerber_editor_group_grb_circular_angle_label->setText(trUi_(QString::fromUtf8("Circular Angle:")));
    gerber_editor_group_grb_circular_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    lay36->addWidget(gerber_editor_group_grb_circular_angle_label, 12, 0, 1, 1);
    gerber_editor_group_grb_circular_angle_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_circular_angle_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_circular_angle_entry->setProperty("maximum", 360.0);
    gerber_editor_group_grb_circular_angle_entry->setProperty("minimum", -360.0);
    gerber_editor_group_grb_circular_angle_entry->setProperty("singleStep", 5.0);
    lay36->addWidget(gerber_editor_group_grb_circular_angle_entry, 12, 1, 1, 1);
    gerber_editor_group_grb_array_tools_b_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_tools_b_label->setText(trUi_(QString::fromUtf8("<b>Buffer Tool:</b>")));
    lay36->addWidget(gerber_editor_group_grb_array_tools_b_label, 13, 0, 1, 2);
    gerber_editor_group_grb_buff_label = new QLabel(nullptr);
    gerber_editor_group_grb_buff_label->setText(trUi_(QString::fromUtf8("Buffer distance:")));
    gerber_editor_group_grb_buff_label->setToolTip(trUi_(QString::fromUtf8("Distance at which to buffer the Gerber element.")));
    lay36->addWidget(gerber_editor_group_grb_buff_label, 14, 0, 1, 1);
    gerber_editor_group_grb_buff_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_buff_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_buff_entry->setProperty("maximum", 9999.0);
    gerber_editor_group_grb_buff_entry->setProperty("minimum", -9999.0);
    gerber_editor_group_grb_buff_entry->setProperty("value", 0.1);
    lay36->addWidget(gerber_editor_group_grb_buff_entry, 14, 1, 1, 1);
    gerber_editor_group_grb_array_tools_s_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_tools_s_label->setText(trUi_(QString::fromUtf8("<b>Scale Tool:</b>")));
    lay36->addWidget(gerber_editor_group_grb_array_tools_s_label, 15, 0, 1, 2);
    gerber_editor_group_grb_scale_label = new QLabel(nullptr);
    gerber_editor_group_grb_scale_label->setText(trUi_(QString::fromUtf8("Scale factor:")));
    gerber_editor_group_grb_scale_label->setToolTip(trUi_(QString::fromUtf8("Factor to scale the Gerber element.")));
    lay36->addWidget(gerber_editor_group_grb_scale_label, 16, 0, 1, 1);
    gerber_editor_group_grb_scale_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_scale_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_scale_entry->setProperty("maximum", 9999.0);
    gerber_editor_group_grb_scale_entry->setProperty("value", 1.0);
    lay36->addWidget(gerber_editor_group_grb_scale_entry, 16, 1, 1, 1);
    gerber_editor_group_grb_array_tools_ma_label = new QLabel(nullptr);
    gerber_editor_group_grb_array_tools_ma_label->setText(trUi_(QString::fromUtf8("<b>Mark Area Tool:</b>")));
    lay36->addWidget(gerber_editor_group_grb_array_tools_ma_label, 17, 0, 1, 2);
    gerber_editor_group_grb_ma_low_label = new QLabel(nullptr);
    gerber_editor_group_grb_ma_low_label->setText(trUi_(QString::fromUtf8("Threshold low:")));
    gerber_editor_group_grb_ma_low_label->setToolTip(trUi_(QString::fromUtf8("Threshold value under which the apertures are not marked.")));
    lay36->addWidget(gerber_editor_group_grb_ma_low_label, 18, 0, 1, 1);
    gerber_editor_group_grb_ma_low_entry = new FCDoubleSpinner(nullptr);
    gerber_editor_group_grb_ma_low_entry->setProperty("decimals", 4);
    gerber_editor_group_grb_ma_low_entry->setProperty("maximum", 9999.0);
    lay36->addWidget(gerber_editor_group_grb_ma_low_entry, 18, 1, 1, 1);
    gerber_editor_group_grb_ma_high_label = new QLabel(nullptr);
    gerber_editor_group_grb_ma_high_label->setText(trUi_(QString::fromUtf8("Threshold high:")));
    gerber_editor_group_grb_ma_high_label->setToolTip(trUi_(QString::fromUtf8("Threshold value over which the apertures are not marked.")));
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
    pref_tab_area->addTab(gerber_tab, trUi_(QString::fromUtf8("GERBER")));
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
    excellon_gen_group_plot_options_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    excellon_gen_group_layout->addWidget(excellon_gen_group_plot_options_label, 0);
    auto* lay37 = new QGridLayout();
    lay37->setContentsMargins(0, 0, 0, 0);
    lay37->setHorizontalSpacing(6);
    lay37->setVerticalSpacing(6);
    excellon_gen_group_plot_cb = new FCCheckBox(nullptr);
    excellon_gen_group_plot_cb->setChecked(true);
    excellon_gen_group_plot_cb->setText(trUi_(QString::fromUtf8("Plot")));
    excellon_gen_group_plot_cb->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay37->addWidget(excellon_gen_group_plot_cb, 0, 0, 1, 1);
    excellon_gen_group_solid_cb = new FCCheckBox(nullptr);
    excellon_gen_group_solid_cb->setChecked(true);
    excellon_gen_group_solid_cb->setText(trUi_(QString::fromUtf8("Solid")));
    excellon_gen_group_solid_cb->setToolTip(trUi_(QString::fromUtf8("Plot as solid circles.")));
    lay37->addWidget(excellon_gen_group_solid_cb, 0, 1, 1, 1);
    excellon_gen_group_multicolored_cb = new FCCheckBox(nullptr);
    excellon_gen_group_multicolored_cb->setText(trUi_(QString::fromUtf8("M-Color")));
    excellon_gen_group_multicolored_cb->setToolTip(trUi_(QString::fromUtf8("Draw polygons in different colors.")));
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
    excellon_gen_group_excellon_format_label->setText(trUi_(QString::fromUtf8("<b>Excellon Format:</b>")));
    excellon_gen_group_excellon_format_label->setToolTip(trUi_(QString::fromUtf8("The NC drill files, usually named Excellon files\nare files that can be found in different formats.\nHere we set the format used when the provided\ncoordinates are not using period.\n\nPossible presets:\n\nPROTEUS 3:3 MM LZ\nDipTrace 5:2 MM TZ\nDipTrace 4:3 MM LZ\n\nEAGLE 3:3 MM TZ\nEAGLE 4:3 MM TZ\nEAGLE 2:5 INCH TZ\nEAGLE 3:5 INCH TZ\n\nALTIUM 2:4 INCH LZ\nSprint Layout 2:4 INCH LZ\nKiCAD 3:5 INCH TZ")));
    lay39->addWidget(excellon_gen_group_excellon_format_label, 0, 0, 1, 2);
    excellon_gen_group_excellon_format_in_label = new QLabel(nullptr);
    excellon_gen_group_excellon_format_in_label->setText(trUi_(QString::fromUtf8("INCH:")));
    excellon_gen_group_excellon_format_in_label->setToolTip(trUi_(QString::fromUtf8("Default values for INCH are 2:4")));
    lay39->addWidget(excellon_gen_group_excellon_format_in_label, 1, 0, 1, 1);
    auto* lay40 = new QHBoxLayout();
    lay40->setContentsMargins(0, 0, 0, 0);
    lay40->setSpacing(6);
    excellon_gen_group_excellon_format_upper_in_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_upper_in_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_upper_in_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_upper_in_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe whole part of Excellon coordinates.")));
    excellon_gen_group_excellon_format_upper_in_entry->setProperty("value", 2);
    lay40->addWidget(excellon_gen_group_excellon_format_upper_in_entry, 0);
    auto* w41 = new QLabel(nullptr);
    w41->setMaximumSize(QSize(5, 16777215));
    w41->setMinimumSize(QSize(5, 0));
    w41->setText(trUi_(QString::fromUtf8(":")));
    lay40->addWidget(w41, 0);
    excellon_gen_group_excellon_format_lower_in_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_lower_in_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_lower_in_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_lower_in_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe decimal part of Excellon coordinates.")));
    excellon_gen_group_excellon_format_lower_in_entry->setProperty("value", 4);
    lay40->addWidget(excellon_gen_group_excellon_format_lower_in_entry, 0);
    lay39->addLayout(lay40, 1, 1, 1, 1);
    excellon_gen_group_excellon_format_mm_label = new QLabel(nullptr);
    excellon_gen_group_excellon_format_mm_label->setText(trUi_(QString::fromUtf8("METRIC:")));
    excellon_gen_group_excellon_format_mm_label->setToolTip(trUi_(QString::fromUtf8("Default values for METRIC are 3:3")));
    lay39->addWidget(excellon_gen_group_excellon_format_mm_label, 2, 0, 1, 1);
    auto* lay42 = new QHBoxLayout();
    lay42->setContentsMargins(0, 0, 0, 0);
    lay42->setSpacing(6);
    excellon_gen_group_excellon_format_upper_mm_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_upper_mm_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_upper_mm_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_upper_mm_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe whole part of Excellon coordinates.")));
    excellon_gen_group_excellon_format_upper_mm_entry->setProperty("value", 3);
    lay42->addWidget(excellon_gen_group_excellon_format_upper_mm_entry, 0);
    auto* w43 = new QLabel(nullptr);
    w43->setMaximumSize(QSize(5, 16777215));
    w43->setMinimumSize(QSize(5, 0));
    w43->setText(trUi_(QString::fromUtf8(":")));
    lay42->addWidget(w43, 1);
    excellon_gen_group_excellon_format_lower_mm_entry = new FCSpinner(nullptr);
    excellon_gen_group_excellon_format_lower_mm_entry->setProperty("maximum", 9);
    excellon_gen_group_excellon_format_lower_mm_entry->setMinimumSize(QSize(30, 0));
    excellon_gen_group_excellon_format_lower_mm_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe decimal part of Excellon coordinates.")));
    excellon_gen_group_excellon_format_lower_mm_entry->setProperty("value", 3);
    lay42->addWidget(excellon_gen_group_excellon_format_lower_mm_entry, 0);
    lay39->addLayout(lay42, 2, 1, 1, 1);
    excellon_gen_group_excellon_zeros_label = new QLabel(nullptr);
    excellon_gen_group_excellon_zeros_label->setProperty("alignment", 1);
    excellon_gen_group_excellon_zeros_label->setText(trUi_(QString::fromUtf8("Zeros:")));
    excellon_gen_group_excellon_zeros_label->setToolTip(trUi_(QString::fromUtf8("This sets the type of Excellon zeros.\nIf LZ then Leading Zeros are kept and\nTrailing Zeros are removed.\nIf TZ is checked then Trailing Zeros are kept\nand Leading Zeros are removed.\n\nThis is used when there is no information\nstored in the Excellon file.")));
    lay39->addWidget(excellon_gen_group_excellon_zeros_label, 3, 0, 1, 1);
    excellon_gen_group_excellon_zeros_radio = new RadioSet({{trUi_(QString::fromUtf8("LZ")), QVariant(QString::fromUtf8("L"))}, {trUi_(QString::fromUtf8("TZ")), QVariant(QString::fromUtf8("T"))}}, Qt::Horizontal, true, nullptr);
    excellon_gen_group_excellon_zeros_radio->setValue(QVariant(QString::fromUtf8("T")));
    lay39->addWidget(excellon_gen_group_excellon_zeros_radio, 3, 1, 1, 1);
    excellon_gen_group_excellon_units_label = new QLabel(nullptr);
    excellon_gen_group_excellon_units_label->setProperty("alignment", 1);
    excellon_gen_group_excellon_units_label->setText(trUi_(QString::fromUtf8("Units:")));
    excellon_gen_group_excellon_units_label->setToolTip(trUi_(QString::fromUtf8("This sets the default units of Excellon files.\nIf it is not detected in the parsed file the value here\nwill be used.Some Excellon files don't have an header\ntherefore this parameter will be used.")));
    lay39->addWidget(excellon_gen_group_excellon_units_label, 4, 0, 1, 1);
    excellon_gen_group_excellon_units_radio = new RadioSet({{trUi_(QString::fromUtf8("Inch")), QVariant(QString::fromUtf8("INCH"))}, {trUi_(QString::fromUtf8("mm")), QVariant(QString::fromUtf8("METRIC"))}}, Qt::Horizontal, true, nullptr);
    excellon_gen_group_excellon_units_radio->setToolTip(trUi_(QString::fromUtf8("This sets the units of Excellon files.\nSome Excellon files don't have an header\ntherefore this parameter will be used.")));
    excellon_gen_group_excellon_units_radio->setValue(QVariant(QString::fromUtf8("INCH")));
    lay39->addWidget(excellon_gen_group_excellon_units_radio, 4, 1, 1, 1);
    excellon_gen_group_update_excellon_cb = new FCCheckBox(nullptr);
    excellon_gen_group_update_excellon_cb->setChecked(true);
    excellon_gen_group_update_excellon_cb->setText(trUi_(QString::fromUtf8("Update Export settings")));
    excellon_gen_group_update_excellon_cb->setToolTip(trUi_(QString::fromUtf8("If checked, the Excellon Export settings will be updated with the ones above.")));
    lay39->addWidget(excellon_gen_group_update_excellon_cb, 5, 0, 1, 2);
    excellon_gen_group_excellon_defaults_button = new QPushButton(nullptr);
    excellon_gen_group_excellon_defaults_button->setMinimumSize(QSize(80, 0));
    excellon_gen_group_excellon_defaults_button->setText(trUi_(QString::fromUtf8("Restore Defaults")));
    lay39->addWidget(excellon_gen_group_excellon_defaults_button, 6, 0, 1, 2);
    auto* w44 = new QFrame(nullptr);
    w44->setProperty("frameShadow", 48);
    w44->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w44->setSizePolicy(sp); }
    lay39->addWidget(w44, 7, 0, 1, 2);
    excellon_gen_group_excellon_general_label = new QLabel(nullptr);
    excellon_gen_group_excellon_general_label->setText(trUi_(QString::fromUtf8("<b>Path Optimization:</b>")));
    lay39->addWidget(excellon_gen_group_excellon_general_label, 8, 0, 1, 2);
    excellon_gen_group_excellon_optimization_label = new QLabel(nullptr);
    excellon_gen_group_excellon_optimization_label->setText(trUi_(QString::fromUtf8("Algorithm:")));
    excellon_gen_group_excellon_optimization_label->setToolTip(trUi_(QString::fromUtf8("This sets the optimization type for the Excellon drill path.\nIf <<MetaHeuristic>> is checked then Google OR-Tools algorithm with\nMetaHeuristic Guided Local Path is used. Default search time is 3sec.\nIf <<Basic>> is checked then Google OR-Tools Basic algorithm is used.\nIf <<TSA>> is checked then Travelling Salesman algorithm is used for\ndrill path optimization.\n\nSome options are disabled when the application works in 32bit mode.")));
    lay39->addWidget(excellon_gen_group_excellon_optimization_label, 9, 0, 1, 1);
    excellon_gen_group_excellon_optimization_radio = new RadioSet({{trUi_(QString::fromUtf8("MetaHeuristic")), QVariant(QString::fromUtf8("M"))}, {trUi_(QString::fromUtf8("Basic")), QVariant(QString::fromUtf8("B"))}, {trUi_(QString::fromUtf8("TSA")), QVariant(QString::fromUtf8("T"))}}, Qt::Vertical, false, nullptr);
    excellon_gen_group_excellon_optimization_radio->setValue(QVariant(QString::fromUtf8("B")));
    lay39->addWidget(excellon_gen_group_excellon_optimization_radio, 9, 1, 1, 1);
    excellon_gen_group_optimization_time_label = new QLabel(nullptr);
    excellon_gen_group_optimization_time_label->setProperty("alignment", 1);
    excellon_gen_group_optimization_time_label->setEnabled(false);
    excellon_gen_group_optimization_time_label->setText(trUi_(QString::fromUtf8("Duration:")));
    excellon_gen_group_optimization_time_label->setToolTip(trUi_(QString::fromUtf8("When OR-Tools Metaheuristic (MH) is enabled there is a\nmaximum threshold for how much time is spent doing the\npath optimization. This max duration is set here.\nIn seconds.")));
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
    excellon_gen_group_join_geo_label->setText(trUi_(QString::fromUtf8("<b>Join Option</b>:")));
    lay39->addWidget(excellon_gen_group_join_geo_label, 12, 0, 1, 2);
    excellon_gen_group_fuse_tools_cb = new FCCheckBox(nullptr);
    excellon_gen_group_fuse_tools_cb->setChecked(true);
    excellon_gen_group_fuse_tools_cb->setText(trUi_(QString::fromUtf8("Fuse Tools")));
    excellon_gen_group_fuse_tools_cb->setToolTip(trUi_(QString::fromUtf8("When checked, the tools will be merged\nbut only if they share some of their attributes.")));
    lay39->addWidget(excellon_gen_group_fuse_tools_cb, 13, 0, 1, 2);
    auto* w46 = new QFrame(nullptr);
    w46->setProperty("frameShadow", 48);
    w46->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w46->setSizePolicy(sp); }
    lay39->addWidget(w46, 14, 0, 1, 2);
    excellon_gen_group_gerber_color_label = new QLabel(nullptr);
    excellon_gen_group_gerber_color_label->setText(trUi_(QString::fromUtf8("<b>Object Color</b>")));
    lay39->addWidget(excellon_gen_group_gerber_color_label, 17, 0, 1, 2);
    excellon_gen_group_line_color_label = new QLabel(nullptr);
    excellon_gen_group_line_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    excellon_gen_group_line_color_label->setToolTip(trUi_(QString::fromUtf8("Set the line color for plotted objects.")));
    lay39->addWidget(excellon_gen_group_line_color_label, 19, 0, 1, 1);
    excellon_gen_group_line_color_entry = new FCColorEntry(nullptr);
    excellon_gen_group_line_color_entry->setValue(QString::fromUtf8("#750000bf"));
    lay39->addWidget(excellon_gen_group_line_color_entry, 19, 1, 1, 1);
    excellon_gen_group_fill_color_label = new QLabel(nullptr);
    excellon_gen_group_fill_color_label->setText(trUi_(QString::fromUtf8("Fill:")));
    excellon_gen_group_fill_color_label->setToolTip(trUi_(QString::fromUtf8("Set the fill color for plotted objects.\nFirst 6 digits are the color and the last 2\ndigits are for alpha (transparency) level.")));
    lay39->addWidget(excellon_gen_group_fill_color_label, 22, 0, 1, 1);
    excellon_gen_group_fill_color_entry = new FCColorEntry(nullptr);
    excellon_gen_group_fill_color_entry->setValue(QString::fromUtf8("#C40000bf"));
    lay39->addWidget(excellon_gen_group_fill_color_entry, 22, 1, 1, 1);
    excellon_gen_group_excellon_alpha_label = new QLabel(nullptr);
    excellon_gen_group_excellon_alpha_label->setText(trUi_(QString::fromUtf8("Alpha:")));
    excellon_gen_group_excellon_alpha_label->setToolTip(trUi_(QString::fromUtf8("Set the fill transparency for plotted objects.")));
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
    excellon_opt_group_cncjob_label->setText(trUi_(QString::fromUtf8("<b>Create CNCJob</b>")));
    excellon_opt_group_cncjob_label->setToolTip(trUi_(QString::fromUtf8("Parameters used to create a CNC Job object\nfor this drill object.")));
    excellon_opt_group_layout->addWidget(excellon_opt_group_cncjob_label, 0);
    auto* lay47 = new QGridLayout();
    lay47->setContentsMargins(0, 0, 0, 0);
    lay47->setHorizontalSpacing(6);
    lay47->setVerticalSpacing(6);
    excellon_opt_group_operation_label = new QLabel(nullptr);
    excellon_opt_group_operation_label->setText(trUi_(QString::fromUtf8("<b>Operation:</b>")));
    excellon_opt_group_operation_label->setToolTip(trUi_(QString::fromUtf8("Operation type:\n- Drilling -> will drill the drills/slots associated with this tool\n- Milling -> will mill the drills/slots")));
    lay47->addWidget(excellon_opt_group_operation_label, 0, 0, 1, 1);
    excellon_opt_group_operation_radio = new RadioSet({{trUi_(QString::fromUtf8("Drilling")), QVariant(QString::fromUtf8("drill"))}, {trUi_(QString::fromUtf8("Milling")), QVariant(QString::fromUtf8("mill"))}}, Qt::Horizontal, true, nullptr);
    excellon_opt_group_operation_radio->setValue(QVariant(QString::fromUtf8("drill")));
    lay47->addWidget(excellon_opt_group_operation_radio, 0, 1, 1, 1);
    excellon_opt_group_mill_type_label = new QLabel(nullptr);
    excellon_opt_group_mill_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    excellon_opt_group_mill_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- Drills -> will mill the drills associated with this tool\n- Slots -> will mill the slots associated with this tool\n- Both -> will mill both drills and mills or whatever is available")));
    lay47->addWidget(excellon_opt_group_mill_type_label, 1, 0, 1, 1);
    excellon_opt_group_milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Drills")), QVariant(QString::fromUtf8("drills"))}, {trUi_(QString::fromUtf8("Slots")), QVariant(QString::fromUtf8("slots"))}, {trUi_(QString::fromUtf8("Both")), QVariant(QString::fromUtf8("both"))}}, Qt::Horizontal, true, nullptr);
    excellon_opt_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("drills")));
    lay47->addWidget(excellon_opt_group_milling_type_radio, 1, 1, 1, 1);
    excellon_opt_group_mill_dia_label = new QLabel(nullptr);
    excellon_opt_group_mill_dia_label->setText(trUi_(QString::fromUtf8("Milling Diameter:")));
    excellon_opt_group_mill_dia_label->setToolTip(trUi_(QString::fromUtf8("The diameter of the tool who will do the milling")));
    lay47->addWidget(excellon_opt_group_mill_dia_label, 2, 0, 1, 1);
    excellon_opt_group_mill_dia_entry = new FCDoubleSpinner(nullptr);
    excellon_opt_group_mill_dia_entry->setProperty("decimals", 4);
    excellon_opt_group_mill_dia_entry->setProperty("maximum", 10000.0);
    excellon_opt_group_mill_dia_entry->setProperty("value", 0.8);
    lay47->addWidget(excellon_opt_group_mill_dia_entry, 2, 1, 1, 1);
    excellon_opt_group_mill_hole_label = new QLabel(nullptr);
    excellon_opt_group_mill_hole_label->setText(trUi_(QString::fromUtf8("<b>Mill Holes</b>")));
    excellon_opt_group_mill_hole_label->setToolTip(trUi_(QString::fromUtf8("Create Geometry for milling holes.")));
    lay47->addWidget(excellon_opt_group_mill_hole_label, 16, 0, 1, 2);
    auto* w48 = new QLabel(nullptr);
    w48->setText(trUi_(QString::fromUtf8("Drill Tool dia:")));
    w48->setToolTip(trUi_(QString::fromUtf8("Diameter of the cutting tool.")));
    lay47->addWidget(w48, 18, 0, 1, 1);
    excellon_opt_group_tooldia_entry = new FCDoubleSpinner(nullptr);
    excellon_opt_group_tooldia_entry->setProperty("decimals", 4);
    excellon_opt_group_tooldia_entry->setProperty("maximum", 999.9999);
    excellon_opt_group_tooldia_entry->setProperty("value", 0.8);
    lay47->addWidget(excellon_opt_group_tooldia_entry, 18, 1, 1, 1);
    auto* w49 = new QLabel(nullptr);
    w49->setText(trUi_(QString::fromUtf8("Slot Tool dia:")));
    w49->setToolTip(trUi_(QString::fromUtf8("Diameter of the cutting tool\nwhen milling slots.")));
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
    excellon_adv_opt_group_exc_label->setText(trUi_(QString::fromUtf8("<b>Advanced Options:</b>")));
    excellon_adv_opt_group_exc_label->setToolTip(trUi_(QString::fromUtf8("A list of advanced parameters.\nThose parameters are available only for\nAdvanced App. Level.")));
    excellon_adv_opt_group_layout->addWidget(excellon_adv_opt_group_exc_label, 0);
    auto* lay50 = new QGridLayout();
    lay50->setContentsMargins(0, 0, 0, 0);
    lay50->setHorizontalSpacing(6);
    lay50->setVerticalSpacing(6);
    excellon_adv_opt_group_table_visibility_cb = new FCCheckBox(nullptr);
    excellon_adv_opt_group_table_visibility_cb->setChecked(true);
    excellon_adv_opt_group_table_visibility_cb->setText(trUi_(QString::fromUtf8("Table Show/Hide")));
    excellon_adv_opt_group_table_visibility_cb->setToolTip(trUi_(QString::fromUtf8("Toggle the display of the Tools Table.")));
    lay50->addWidget(excellon_adv_opt_group_table_visibility_cb, 0, 0, 1, 2);
    excellon_adv_opt_group_autoload_db_cb = new FCCheckBox(nullptr);
    excellon_adv_opt_group_autoload_db_cb->setText(trUi_(QString::fromUtf8("Auto load from DB")));
    excellon_adv_opt_group_autoload_db_cb->setToolTip(trUi_(QString::fromUtf8("Automatic replacement of the tools from related application tools\nwith tools from DB that have a close diameter value.")));
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
    excellon_exp_group_export_options_label->setText(trUi_(QString::fromUtf8("<b>Export Options:</b>")));
    excellon_exp_group_export_options_label->setToolTip(trUi_(QString::fromUtf8("The parameters set here are used in the file exported\nwhen using the File -> Export -> Export Excellon menu entry.")));
    excellon_exp_group_layout->addWidget(excellon_exp_group_export_options_label, 0);
    auto* lay51 = new QFormLayout();
    lay51->setContentsMargins(0, 0, 0, 0);
    lay51->setHorizontalSpacing(6);
    lay51->setVerticalSpacing(6);
    excellon_exp_group_excellon_units_label = new QLabel(nullptr);
    excellon_exp_group_excellon_units_label->setText(trUi_(QString::fromUtf8("Units:")));
    excellon_exp_group_excellon_units_label->setToolTip(trUi_(QString::fromUtf8("The units used in the Excellon file.")));
    lay51->setWidget(0, QFormLayout::ItemRole(0), excellon_exp_group_excellon_units_label);
    excellon_exp_group_excellon_units_radio = new RadioSet({{trUi_(QString::fromUtf8("Inch")), QVariant(QString::fromUtf8("INCH"))}, {trUi_(QString::fromUtf8("mm")), QVariant(QString::fromUtf8("METRIC"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_excellon_units_radio->setToolTip(trUi_(QString::fromUtf8("The units used in the Excellon file.")));
    excellon_exp_group_excellon_units_radio->setValue(QVariant(QString::fromUtf8("INCH")));
    lay51->setWidget(0, QFormLayout::ItemRole(1), excellon_exp_group_excellon_units_radio);
    excellon_exp_group_digits_label = new QLabel(nullptr);
    excellon_exp_group_digits_label->setText(trUi_(QString::fromUtf8("Int/Decimals:")));
    excellon_exp_group_digits_label->setToolTip(trUi_(QString::fromUtf8("The NC drill files, usually named Excellon files\nare files that can be found in different formats.\nHere we set the format used when the provided\ncoordinates are not using period.")));
    lay51->setWidget(1, QFormLayout::ItemRole(0), excellon_exp_group_digits_label);
    auto* lay52 = new QHBoxLayout();
    lay52->setContentsMargins(0, 0, 0, 0);
    lay52->setSpacing(6);
    excellon_exp_group_format_whole_entry = new FCSpinner(nullptr);
    excellon_exp_group_format_whole_entry->setProperty("maximum", 9);
    excellon_exp_group_format_whole_entry->setMinimumSize(QSize(30, 0));
    excellon_exp_group_format_whole_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe whole part of Excellon coordinates.")));
    excellon_exp_group_format_whole_entry->setProperty("value", 2);
    lay52->addWidget(excellon_exp_group_format_whole_entry, 1);
    auto* w53 = new QLabel(nullptr);
    w53->setMaximumSize(QSize(5, 16777215));
    w53->setMinimumSize(QSize(5, 0));
    w53->setText(trUi_(QString::fromUtf8(":")));
    lay52->addWidget(w53, 1);
    excellon_exp_group_format_dec_entry = new FCSpinner(nullptr);
    excellon_exp_group_format_dec_entry->setProperty("maximum", 9);
    excellon_exp_group_format_dec_entry->setMinimumSize(QSize(30, 0));
    excellon_exp_group_format_dec_entry->setToolTip(trUi_(QString::fromUtf8("This numbers signify the number of digits in\nthe decimal part of Excellon coordinates.")));
    excellon_exp_group_format_dec_entry->setProperty("value", 4);
    lay52->addWidget(excellon_exp_group_format_dec_entry, 1);
    lay52->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    lay51->setLayout(1, QFormLayout::ItemRole(1), lay52);
    excellon_exp_group_format_label = new QLabel(nullptr);
    excellon_exp_group_format_label->setText(trUi_(QString::fromUtf8("Format:")));
    excellon_exp_group_format_label->setToolTip(trUi_(QString::fromUtf8("Select the kind of coordinates format used.\nCoordinates can be saved with decimal point or without.\nWhen there is no decimal point, it is required to specify\nthe number of digits for integer part and the number of decimals.\nAlso it will have to be specified if LZ = leading zeros are kept\nor TZ = trailing zeros are kept.")));
    lay51->setWidget(2, QFormLayout::ItemRole(0), excellon_exp_group_format_label);
    excellon_exp_group_format_radio = new RadioSet({{trUi_(QString::fromUtf8("Decimal")), QVariant(QString::fromUtf8("dec"))}, {trUi_(QString::fromUtf8("No-Decimal")), QVariant(QString::fromUtf8("ndec"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_format_radio->setToolTip(trUi_(QString::fromUtf8("Select the kind of coordinates format used.\nCoordinates can be saved with decimal point or without.\nWhen there is no decimal point, it is required to specify\nthe number of digits for integer part and the number of decimals.\nAlso it will have to be specified if LZ = leading zeros are kept\nor TZ = trailing zeros are kept.")));
    excellon_exp_group_format_radio->setValue(QVariant(QString::fromUtf8("dec")));
    lay51->setWidget(2, QFormLayout::ItemRole(1), excellon_exp_group_format_radio);
    excellon_exp_group_zeros_label = new QLabel(nullptr);
    excellon_exp_group_zeros_label->setProperty("alignment", 1);
    excellon_exp_group_zeros_label->setEnabled(false);
    excellon_exp_group_zeros_label->setText(trUi_(QString::fromUtf8("Zeros:")));
    excellon_exp_group_zeros_label->setToolTip(trUi_(QString::fromUtf8("This sets the type of Excellon zeros.\nIf LZ then Leading Zeros are kept and\nTrailing Zeros are removed.\nIf TZ is checked then Trailing Zeros are kept\nand Leading Zeros are removed.")));
    lay51->setWidget(3, QFormLayout::ItemRole(0), excellon_exp_group_zeros_label);
    excellon_exp_group_zeros_radio = new RadioSet({{trUi_(QString::fromUtf8("LZ")), QVariant(QString::fromUtf8("LZ"))}, {trUi_(QString::fromUtf8("TZ")), QVariant(QString::fromUtf8("TZ"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_zeros_radio->setEnabled(false);
    excellon_exp_group_zeros_radio->setToolTip(trUi_(QString::fromUtf8("This sets the default type of Excellon zeros.\nIf LZ then Leading Zeros are kept and\nTrailing Zeros are removed.\nIf TZ is checked then Trailing Zeros are kept\nand Leading Zeros are removed.")));
    excellon_exp_group_zeros_radio->setValue(QVariant(QString::fromUtf8("LZ")));
    lay51->setWidget(3, QFormLayout::ItemRole(1), excellon_exp_group_zeros_radio);
    excellon_exp_group_slot_type_label = new QLabel(nullptr);
    excellon_exp_group_slot_type_label->setProperty("alignment", 1);
    excellon_exp_group_slot_type_label->setText(trUi_(QString::fromUtf8("Slot type:")));
    excellon_exp_group_slot_type_label->setToolTip(trUi_(QString::fromUtf8("This sets how the slots will be exported.\nIf ROUTED then the slots will be routed\nusing M15/M16 commands.\nIf DRILLED(G85) the slots will be exported\nusing the Drilled slot command (G85).")));
    lay51->setWidget(4, QFormLayout::ItemRole(0), excellon_exp_group_slot_type_label);
    excellon_exp_group_slot_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Routed")), QVariant(QString::fromUtf8("routing"))}, {trUi_(QString::fromUtf8("Drilled(G85)")), QVariant(QString::fromUtf8("drilling"))}}, Qt::Horizontal, true, nullptr);
    excellon_exp_group_slot_type_radio->setToolTip(trUi_(QString::fromUtf8("This sets how the slots will be exported.\nIf ROUTED then the slots will be routed\nusing M15/M16 commands.\nIf DRILLED(G85) the slots will be exported\nusing the Drilled slot command (G85).")));
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
    excellon_editor_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    excellon_editor_group_param_label->setToolTip(trUi_(QString::fromUtf8("A list of Excellon Editor parameters.")));
    excellon_editor_group_layout->addWidget(excellon_editor_group_param_label, 0);
    auto* lay54 = new QGridLayout();
    lay54->setContentsMargins(0, 0, 0, 0);
    lay54->setHorizontalSpacing(6);
    lay54->setVerticalSpacing(6);
    excellon_editor_group_sel_limit_label = new QLabel(nullptr);
    excellon_editor_group_sel_limit_label->setText(trUi_(QString::fromUtf8("Selection limit:")));
    excellon_editor_group_sel_limit_label->setToolTip(trUi_(QString::fromUtf8("Set the number of selected Excellon geometry\nitems above which the utility geometry\nbecomes just a selection rectangle.\nIncreases the performance when moving a\nlarge number of geometric elements.")));
    lay54->addWidget(excellon_editor_group_sel_limit_label, 0, 0, 1, 1);
    excellon_editor_group_sel_limit_entry = new FCSpinner(nullptr);
    excellon_editor_group_sel_limit_entry->setProperty("maximum", 99999);
    excellon_editor_group_sel_limit_entry->setProperty("value", 30);
    lay54->addWidget(excellon_editor_group_sel_limit_entry, 0, 1, 1, 1);
    excellon_editor_group_addtool_entry_lbl = new QLabel(nullptr);
    excellon_editor_group_addtool_entry_lbl->setText(trUi_(QString::fromUtf8("New Dia:")));
    excellon_editor_group_addtool_entry_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool")));
    lay54->addWidget(excellon_editor_group_addtool_entry_lbl, 1, 0, 1, 1);
    excellon_editor_group_addtool_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_addtool_entry->setProperty("decimals", 4);
    excellon_editor_group_addtool_entry->setProperty("maximum", 99.9999);
    excellon_editor_group_addtool_entry->setProperty("value", 1.0);
    lay54->addWidget(excellon_editor_group_addtool_entry, 1, 1, 1, 1);
    excellon_editor_group_drill_array_size_label = new QLabel(nullptr);
    excellon_editor_group_drill_array_size_label->setText(trUi_(QString::fromUtf8("Nr of drills:")));
    excellon_editor_group_drill_array_size_label->setToolTip(trUi_(QString::fromUtf8("Specify how many slots to be in the array.")));
    lay54->addWidget(excellon_editor_group_drill_array_size_label, 2, 0, 1, 1);
    excellon_editor_group_drill_array_size_entry = new FCSpinner(nullptr);
    excellon_editor_group_drill_array_size_entry->setProperty("maximum", 9999);
    excellon_editor_group_drill_array_size_entry->setProperty("value", 5);
    lay54->addWidget(excellon_editor_group_drill_array_size_entry, 2, 1, 1, 1);
    excellon_editor_group_drill_array_linear_label = new QLabel(nullptr);
    excellon_editor_group_drill_array_linear_label->setText(trUi_(QString::fromUtf8("<b>Linear Drill Array:</b>")));
    lay54->addWidget(excellon_editor_group_drill_array_linear_label, 3, 0, 1, 2);
    excellon_editor_group_drill_axis_label = new QLabel(nullptr);
    excellon_editor_group_drill_axis_label->setText(trUi_(QString::fromUtf8("Linear Direction:")));
    excellon_editor_group_drill_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the linear array is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the array inclination")));
    lay54->addWidget(excellon_editor_group_drill_axis_label, 4, 0, 1, 1);
    excellon_editor_group_drill_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_drill_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay54->addWidget(excellon_editor_group_drill_axis_radio, 4, 1, 1, 1);
    excellon_editor_group_drill_pitch_label = new QLabel(nullptr);
    excellon_editor_group_drill_pitch_label->setText(trUi_(QString::fromUtf8("Pitch:")));
    excellon_editor_group_drill_pitch_label->setToolTip(trUi_(QString::fromUtf8("Pitch = Distance between elements of the array.")));
    lay54->addWidget(excellon_editor_group_drill_pitch_label, 5, 0, 1, 1);
    excellon_editor_group_drill_pitch_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_drill_pitch_entry->setProperty("decimals", 4);
    excellon_editor_group_drill_pitch_entry->setProperty("maximum", 360.0);
    excellon_editor_group_drill_pitch_entry->setProperty("minimum", -360.0);
    excellon_editor_group_drill_pitch_entry->setProperty("value", 2.54);
    lay54->addWidget(excellon_editor_group_drill_pitch_entry, 5, 1, 1, 1);
    excellon_editor_group_drill_angle_label = new QLabel(nullptr);
    excellon_editor_group_drill_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    excellon_editor_group_drill_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    lay54->addWidget(excellon_editor_group_drill_angle_label, 6, 0, 1, 1);
    excellon_editor_group_drill_angle_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_drill_angle_entry->setProperty("singleStep", 5.0);
    excellon_editor_group_drill_angle_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_drill_angle_entry, 6, 1, 1, 1);
    auto* w55 = new QLabel(nullptr);
    w55->setText(trUi_(QString::fromUtf8("<b>Circular Drill Array:</b>")));
    lay54->addWidget(w55, 7, 0, 1, 2);
    excellon_editor_group_drill_circular_direction_label = new QLabel(nullptr);
    excellon_editor_group_drill_circular_direction_label->setText(trUi_(QString::fromUtf8("Circular Direction:")));
    excellon_editor_group_drill_circular_direction_label->setToolTip(trUi_(QString::fromUtf8("Direction for circular array.\nCan be CW = clockwise or CCW = counter clockwise.")));
    lay54->addWidget(excellon_editor_group_drill_circular_direction_label, 8, 0, 1, 1);
    excellon_editor_group_drill_circular_dir_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_drill_circular_dir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay54->addWidget(excellon_editor_group_drill_circular_dir_radio, 8, 1, 1, 1);
    excellon_editor_group_drill_circular_angle_label = new QLabel(nullptr);
    excellon_editor_group_drill_circular_angle_label->setText(trUi_(QString::fromUtf8("Circular Angle:")));
    excellon_editor_group_drill_circular_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
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
    excellon_editor_group_drill_array_circ_label->setText(trUi_(QString::fromUtf8("<b>Slots:</b>")));
    lay54->addWidget(excellon_editor_group_drill_array_circ_label, 10, 0, 1, 2);
    excellon_editor_group_slot_length_label = new QLabel(nullptr);
    excellon_editor_group_slot_length_label->setMinimumSize(QSize(100, 0));
    excellon_editor_group_slot_length_label->setText(trUi_(QString::fromUtf8("Length:")));
    excellon_editor_group_slot_length_label->setToolTip(trUi_(QString::fromUtf8("Length. The length of the slot.")));
    lay54->addWidget(excellon_editor_group_slot_length_label, 11, 0, 1, 1);
    excellon_editor_group_slot_length_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_length_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_length_entry->setProperty("maximum", 99999.0);
    excellon_editor_group_slot_length_entry->setProperty("value", 5.0);
    excellon_editor_group_slot_length_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_length_entry, 11, 1, 1, 1);
    excellon_editor_group_slot_axis_label = new QLabel(nullptr);
    excellon_editor_group_slot_axis_label->setMinimumSize(QSize(100, 0));
    excellon_editor_group_slot_axis_label->setText(trUi_(QString::fromUtf8("Direction:")));
    excellon_editor_group_slot_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the slot is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the slot inclination")));
    lay54->addWidget(excellon_editor_group_slot_axis_label, 12, 0, 1, 1);
    excellon_editor_group_slot_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_slot_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay54->addWidget(excellon_editor_group_slot_axis_radio, 12, 1, 1, 1);
    excellon_editor_group_slot_angle_label = new QLabel(nullptr);
    excellon_editor_group_slot_angle_label->setMinimumSize(QSize(100, 0));
    excellon_editor_group_slot_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    excellon_editor_group_slot_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which the slot is placed.\nThe precision is of max 2 decimals.\nMin value is: -360.00 degrees.\nMax value is: 360.00 degrees.")));
    lay54->addWidget(excellon_editor_group_slot_angle_label, 13, 0, 1, 1);
    excellon_editor_group_slot_angle_spinner = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_angle_spinner->setProperty("decimals", 4);
    excellon_editor_group_slot_angle_spinner->setProperty("maximum", 360.0);
    excellon_editor_group_slot_angle_spinner->setProperty("minimum", -359.99);
    excellon_editor_group_slot_angle_spinner->setProperty("singleStep", 5.0);
    excellon_editor_group_slot_angle_spinner->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_angle_spinner, 13, 1, 1, 1);
    excellon_editor_group_slot_array_linear_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_linear_label->setText(trUi_(QString::fromUtf8("<b>Linear Slot Array:</b>")));
    lay54->addWidget(excellon_editor_group_slot_array_linear_label, 14, 0, 1, 2);
    excellon_editor_group_slot_array_size_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_size_label->setText(trUi_(QString::fromUtf8("Nr of slots:")));
    lay54->addWidget(excellon_editor_group_slot_array_size_label, 15, 0, 1, 1);
    excellon_editor_group_slot_array_size_entry = new FCSpinner(nullptr);
    excellon_editor_group_slot_array_size_entry->setProperty("maximum", 999999);
    excellon_editor_group_slot_array_size_entry->setProperty("value", 5);
    lay54->addWidget(excellon_editor_group_slot_array_size_entry, 15, 1, 1, 1);
    excellon_editor_group_slot_array_axis_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_axis_label->setText(trUi_(QString::fromUtf8("Linear Direction:")));
    excellon_editor_group_slot_array_axis_label->setToolTip(trUi_(QString::fromUtf8("Direction on which the linear array is oriented:\n- 'X' - horizontal axis \n- 'Y' - vertical axis or \n- 'Angle' - a custom angle for the array inclination")));
    lay54->addWidget(excellon_editor_group_slot_array_axis_label, 16, 0, 1, 1);
    excellon_editor_group_slot_array_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}, {trUi_(QString::fromUtf8("Angle")), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_slot_array_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay54->addWidget(excellon_editor_group_slot_array_axis_radio, 16, 1, 1, 1);
    excellon_editor_group_slot_array_pitch_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_pitch_label->setText(trUi_(QString::fromUtf8("Pitch:")));
    excellon_editor_group_slot_array_pitch_label->setToolTip(trUi_(QString::fromUtf8("Pitch = Distance between elements of the array.")));
    lay54->addWidget(excellon_editor_group_slot_array_pitch_label, 17, 0, 1, 1);
    excellon_editor_group_slot_array_pitch_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_array_pitch_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_array_pitch_entry->setProperty("maximum", 999999.0);
    excellon_editor_group_slot_array_pitch_entry->setProperty("value", 2.54);
    excellon_editor_group_slot_array_pitch_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_array_pitch_entry, 17, 1, 1, 1);
    excellon_editor_group_slot_array_angle_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_angle_label->setText(trUi_(QString::fromUtf8("Angle:")));
    excellon_editor_group_slot_array_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
    lay54->addWidget(excellon_editor_group_slot_array_angle_label, 18, 0, 1, 1);
    excellon_editor_group_slot_array_angle_entry = new FCDoubleSpinner(nullptr);
    excellon_editor_group_slot_array_angle_entry->setProperty("decimals", 4);
    excellon_editor_group_slot_array_angle_entry->setProperty("maximum", 360.0);
    excellon_editor_group_slot_array_angle_entry->setProperty("minimum", -360.0);
    excellon_editor_group_slot_array_angle_entry->setProperty("singleStep", 5.0);
    excellon_editor_group_slot_array_angle_entry->setWrapping(true);
    lay54->addWidget(excellon_editor_group_slot_array_angle_entry, 18, 1, 1, 1);
    excellon_editor_group_slot_array_circ_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_circ_label->setText(trUi_(QString::fromUtf8("<b>Circular Slot Array:</b>")));
    lay54->addWidget(excellon_editor_group_slot_array_circ_label, 19, 0, 1, 2);
    excellon_editor_group_slot_array_circular_direction_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_circular_direction_label->setText(trUi_(QString::fromUtf8("Circular Direction:")));
    excellon_editor_group_slot_array_circular_direction_label->setToolTip(trUi_(QString::fromUtf8("Direction for circular array.\nCan be CW = clockwise or CCW = counter clockwise.")));
    lay54->addWidget(excellon_editor_group_slot_array_circular_direction_label, 20, 0, 1, 1);
    excellon_editor_group_slot_array_circular_dir_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    excellon_editor_group_slot_array_circular_dir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay54->addWidget(excellon_editor_group_slot_array_circular_dir_radio, 20, 1, 1, 1);
    excellon_editor_group_slot_array_circular_angle_label = new QLabel(nullptr);
    excellon_editor_group_slot_array_circular_angle_label->setText(trUi_(QString::fromUtf8("Circular Angle:")));
    excellon_editor_group_slot_array_circular_angle_label->setToolTip(trUi_(QString::fromUtf8("Angle at which each element in circular array is placed.")));
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
    pref_tab_area->addTab(excellon_tab, trUi_(QString::fromUtf8("EXCELLON")));
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
    geometry_gen_group_plot_options_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    geometry_gen_group_layout->addWidget(geometry_gen_group_plot_options_label, 0);
    auto* lay56 = new QHBoxLayout();
    lay56->setContentsMargins(0, 0, 0, 0);
    lay56->setSpacing(6);
    geometry_gen_group_plot_cb = new FCCheckBox(nullptr);
    geometry_gen_group_plot_cb->setChecked(true);
    geometry_gen_group_plot_cb->setText(trUi_(QString::fromUtf8("Plot")));
    geometry_gen_group_plot_cb->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay56->addWidget(geometry_gen_group_plot_cb, 0);
    geometry_gen_group_multicolored_cb = new FCCheckBox(nullptr);
    geometry_gen_group_multicolored_cb->setText(trUi_(QString::fromUtf8("M-Color")));
    geometry_gen_group_multicolored_cb->setToolTip(trUi_(QString::fromUtf8("Draw polygons in different colors.")));
    lay56->addWidget(geometry_gen_group_multicolored_cb, 0);
    geometry_gen_group_layout->addLayout(lay56, 0);
    auto* lay57 = new QGridLayout();
    lay57->setContentsMargins(0, 0, 0, 0);
    lay57->setHorizontalSpacing(6);
    lay57->setVerticalSpacing(6);
    geometry_gen_group_circle_steps_label = new QLabel(nullptr);
    geometry_gen_group_circle_steps_label->setText(trUi_(QString::fromUtf8("Circle Steps:")));
    geometry_gen_group_circle_steps_label->setToolTip(trUi_(QString::fromUtf8("The number of circle steps for <b>Geometry</b> \ncircle and arc shapes linear approximation.")));
    lay57->addWidget(geometry_gen_group_circle_steps_label, 1, 0, 1, 1);
    geometry_gen_group_circle_steps_entry = new FCSpinner(nullptr);
    geometry_gen_group_circle_steps_entry->setProperty("maximum", 999);
    geometry_gen_group_circle_steps_entry->setProperty("value", 64);
    lay57->addWidget(geometry_gen_group_circle_steps_entry, 1, 1, 1, 1);
    geometry_gen_group_tools_label = new QLabel(nullptr);
    geometry_gen_group_tools_label->setText(trUi_(QString::fromUtf8("<b>Tools:</b>")));
    lay57->addWidget(geometry_gen_group_tools_label, 2, 0, 1, 2);
    auto* w58 = new QLabel(nullptr);
    w58->setText(trUi_(QString::fromUtf8("<b><font color=\"green\">Tools Dia:</font></b>")));
    w58->setToolTip(trUi_(QString::fromUtf8("Diameters of the tools, separated by comma.\nThe value of the diameter has to use the dot decimals separator.\nValid values: 0.3, 1.0")));
    lay57->addWidget(w58, 3, 0, 1, 1);
    geometry_gen_group_cnctooldia_entry = new FCEntry(nullptr);
    geometry_gen_group_cnctooldia_entry->setText(trUi_(QString::fromUtf8("2.4")));
    lay57->addWidget(geometry_gen_group_cnctooldia_entry, 3, 1, 1, 1);
    auto* w59 = new QFrame(nullptr);
    w59->setProperty("frameShadow", 48);
    w59->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w59->setSizePolicy(sp); }
    lay57->addWidget(w59, 9, 0, 1, 2);
    geometry_gen_group_opt_label = new QLabel(nullptr);
    geometry_gen_group_opt_label->setText(trUi_(QString::fromUtf8("<b>Path Optimization:</b>")));
    lay57->addWidget(geometry_gen_group_opt_label, 10, 0, 1, 2);
    geometry_gen_group_opt_algorithm_label = new QLabel(nullptr);
    geometry_gen_group_opt_algorithm_label->setText(trUi_(QString::fromUtf8("Algorithm:")));
    geometry_gen_group_opt_algorithm_label->setToolTip(trUi_(QString::fromUtf8("This sets the path optimization algorithm.\n- Rtre -> Rtree algorithm\n- MetaHeuristic -> Google OR-Tools algorithm with\nMetaHeuristic Guided Local Path is used. Default search time is 3sec.\n- Basic -> Using Google OR-Tools Basic algorithm\n- TSA -> Using Travelling Salesman algorithm\n\nSome options are disabled when the application works in 32bit mode.")));
    lay57->addWidget(geometry_gen_group_opt_algorithm_label, 12, 0, 1, 1);
    geometry_gen_group_opt_algorithm_radio = new RadioSet({{trUi_(QString::fromUtf8("Rtree")), QVariant(QString::fromUtf8("R"))}, {trUi_(QString::fromUtf8("MetaHeuristic")), QVariant(QString::fromUtf8("M"))}, {trUi_(QString::fromUtf8("Basic")), QVariant(QString::fromUtf8("B"))}, {trUi_(QString::fromUtf8("TSA")), QVariant(QString::fromUtf8("T"))}}, Qt::Vertical, false, nullptr);
    geometry_gen_group_opt_algorithm_radio->setValue(QVariant(QString::fromUtf8("R")));
    lay57->addWidget(geometry_gen_group_opt_algorithm_radio, 12, 1, 1, 1);
    geometry_gen_group_optimization_time_label = new QLabel(nullptr);
    geometry_gen_group_optimization_time_label->setEnabled(false);
    geometry_gen_group_optimization_time_label->setText(trUi_(QString::fromUtf8("Duration:")));
    geometry_gen_group_optimization_time_label->setToolTip(trUi_(QString::fromUtf8("When OR-Tools Metaheuristic (MH) is enabled there is a\nmaximum threshold for how much time is spent doing the\npath optimization. This max duration is set here.\nIn seconds.")));
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
    geometry_gen_group_join_geo_label->setText(trUi_(QString::fromUtf8("<b>Join Option</b>:")));
    lay57->addWidget(geometry_gen_group_join_geo_label, 18, 0, 1, 2);
    geometry_gen_group_fuse_tools_cb = new FCCheckBox(nullptr);
    geometry_gen_group_fuse_tools_cb->setChecked(true);
    geometry_gen_group_fuse_tools_cb->setText(trUi_(QString::fromUtf8("Fuse Tools")));
    geometry_gen_group_fuse_tools_cb->setToolTip(trUi_(QString::fromUtf8("When checked, the tools will be merged\nbut only if they share some of their attributes.")));
    lay57->addWidget(geometry_gen_group_fuse_tools_cb, 20, 0, 1, 2);
    auto* w61 = new QFrame(nullptr);
    w61->setProperty("frameShadow", 48);
    w61->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w61->setSizePolicy(sp); }
    lay57->addWidget(w61, 22, 0, 1, 2);
    geometry_gen_group_gerber_color_label = new QLabel(nullptr);
    geometry_gen_group_gerber_color_label->setText(trUi_(QString::fromUtf8("<b>Object Color</b>:")));
    lay57->addWidget(geometry_gen_group_gerber_color_label, 24, 0, 1, 2);
    geometry_gen_group_line_color_label = new QLabel(nullptr);
    geometry_gen_group_line_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    geometry_gen_group_line_color_label->setToolTip(trUi_(QString::fromUtf8("Set the line color for plotted objects.")));
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
    geometry_opt_group_cncjob_label->setText(trUi_(QString::fromUtf8("<b>Create CNCJob:</b>")));
    geometry_opt_group_cncjob_label->setToolTip(trUi_(QString::fromUtf8("Create a CNC Job object\ntracing the contours of this\nGeometry object.")));
    geometry_opt_group_layout->addWidget(geometry_opt_group_cncjob_label, 0);
    auto* lay62 = new QGridLayout();
    lay62->setContentsMargins(0, 0, 0, 0);
    lay62->setHorizontalSpacing(6);
    lay62->setVerticalSpacing(6);
    auto* w63 = new QLabel(nullptr);
    w63->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w63->setToolTip(trUi_(QString::fromUtf8("Cutting depth (negative)\nbelow the copper surface.")));
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
    geometry_opt_group_multidepth_cb->setText(trUi_(QString::fromUtf8("Multi-Depth")));
    geometry_opt_group_multidepth_cb->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    lay62->addWidget(geometry_opt_group_multidepth_cb, 1, 0, 1, 1);
    auto* w64 = new QLabel(nullptr);
    w64->setText(trUi_(QString::fromUtf8("Depth/Pass:")));
    w64->setToolTip(trUi_(QString::fromUtf8("The depth to cut on each pass,\nwhen multidepth is enabled.\nIt has positive value although\nit is a fraction from the depth\nwhich has negative value.")));
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
    w65->setText(trUi_(QString::fromUtf8("Travel Z:")));
    w65->setToolTip(trUi_(QString::fromUtf8("Height of the tool when\nmoving without cutting.")));
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
    geometry_opt_group_toolchange_cb->setText(trUi_(QString::fromUtf8("Tool change")));
    geometry_opt_group_toolchange_cb->setToolTip(trUi_(QString::fromUtf8("Include tool-change sequence\nin the Machine Code (Pause for tool change).")));
    lay62->addWidget(geometry_opt_group_toolchange_cb, 4, 0, 1, 2);
    auto* w66 = new QLabel(nullptr);
    w66->setText(trUi_(QString::fromUtf8("Toolchange Z:")));
    w66->setToolTip(trUi_(QString::fromUtf8("Z-axis position (height) for\ntool change.")));
    lay62->addWidget(w66, 5, 0, 1, 1);
    geometry_opt_group_toolchangez_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_toolchangez_entry->setProperty("decimals", 4);
    geometry_opt_group_toolchangez_entry->setProperty("maximum", 10000.0);
    geometry_opt_group_toolchangez_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_toolchangez_entry->setProperty("value", 15.0);
    geometry_opt_group_toolchangez_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_toolchangez_entry, 5, 1, 1, 1);
    auto* w67 = new QLabel(nullptr);
    w67->setText(trUi_(QString::fromUtf8("End move Z:")));
    w67->setToolTip(trUi_(QString::fromUtf8("Height of the tool after\nthe last move at the end of the job.")));
    lay62->addWidget(w67, 6, 0, 1, 1);
    geometry_opt_group_endz_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_endz_entry->setProperty("decimals", 4);
    geometry_opt_group_endz_entry->setProperty("maximum", 10000.0);
    geometry_opt_group_endz_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_endz_entry->setProperty("value", 15.0);
    geometry_opt_group_endz_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_endz_entry, 6, 1, 1, 1);
    auto* w68 = new QLabel(nullptr);
    w68->setText(trUi_(QString::fromUtf8("End move X,Y:")));
    w68->setToolTip(trUi_(QString::fromUtf8("End move X,Y position. In format (x,y).\nIf no value is entered then there is no move\non X,Y plane at the end of the job.")));
    lay62->addWidget(w68, 7, 0, 1, 1);
    geometry_opt_group_endxy_entry = new NumericalEvalTupleEntry(nullptr);
    geometry_opt_group_endxy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    geometry_opt_group_endxy_entry->setText(trUi_(QString::fromUtf8("None")));
    lay62->addWidget(geometry_opt_group_endxy_entry, 7, 1, 1, 1);
    auto* w69 = new QLabel(nullptr);
    w69->setText(trUi_(QString::fromUtf8("Feedrate X-Y:")));
    w69->setToolTip(trUi_(QString::fromUtf8("Cutting speed in the XY\nplane in units per minute")));
    lay62->addWidget(w69, 8, 0, 1, 1);
    geometry_opt_group_cncfeedrate_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_cncfeedrate_entry->setProperty("decimals", 4);
    geometry_opt_group_cncfeedrate_entry->setProperty("maximum", 910000.0);
    geometry_opt_group_cncfeedrate_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_cncfeedrate_entry->setProperty("value", 120.0);
    geometry_opt_group_cncfeedrate_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_cncfeedrate_entry, 8, 1, 1, 1);
    auto* w70 = new QLabel(nullptr);
    w70->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    w70->setToolTip(trUi_(QString::fromUtf8("Cutting speed in the XY\nplane in units per minute.\nIt is called also Plunge.")));
    lay62->addWidget(w70, 9, 0, 1, 1);
    geometry_opt_group_feedrate_z_entry = new FCDoubleSpinner(nullptr);
    geometry_opt_group_feedrate_z_entry->setProperty("decimals", 4);
    geometry_opt_group_feedrate_z_entry->setProperty("maximum", 910000.0);
    geometry_opt_group_feedrate_z_entry->setProperty("singleStep", 0.1);
    geometry_opt_group_feedrate_z_entry->setProperty("value", 60.0);
    geometry_opt_group_feedrate_z_entry->setWrapping(true);
    lay62->addWidget(geometry_opt_group_feedrate_z_entry, 9, 1, 1, 1);
    auto* w71 = new QLabel(nullptr);
    w71->setText(trUi_(QString::fromUtf8("Spindle speed:")));
    w71->setToolTip(trUi_(QString::fromUtf8("Speed of the spindle in RPM (optional).\nIf LASER preprocessor is used,\nthis value is the power of laser.")));
    lay62->addWidget(w71, 10, 0, 1, 1);
    geometry_opt_group_cncspindlespeed_entry = new FCSpinner(nullptr);
    geometry_opt_group_cncspindlespeed_entry->setProperty("maximum", 1000000);
    geometry_opt_group_cncspindlespeed_entry->setProperty("singleStep", 100);
    lay62->addWidget(geometry_opt_group_cncspindlespeed_entry, 10, 1, 1, 1);
    geometry_opt_group_dwell_cb = new FCCheckBox(nullptr);
    geometry_opt_group_dwell_cb->setText(trUi_(QString::fromUtf8("Enable Dwell")));
    geometry_opt_group_dwell_cb->setToolTip(trUi_(QString::fromUtf8("Pause to allow the spindle to reach its\nspeed before cutting.")));
    lay62->addWidget(geometry_opt_group_dwell_cb, 11, 0, 1, 1);
    auto* w72 = new QLabel(nullptr);
    w72->setText(trUi_(QString::fromUtf8("Duration:")));
    w72->setToolTip(trUi_(QString::fromUtf8("Number of time units for spindle to dwell.")));
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
    w73->setText(trUi_(QString::fromUtf8("Preprocessor:")));
    w73->setToolTip(trUi_(QString::fromUtf8("The Preprocessor file that dictates\nthe Machine Code (like GCode, RML, HPGL) output.")));
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
    geometry_adv_opt_group_geo_label->setText(trUi_(QString::fromUtf8("<b>Advanced Options:</b>")));
    geometry_adv_opt_group_geo_label->setToolTip(trUi_(QString::fromUtf8("A list of advanced parameters.\nThose parameters are available only for\nAdvanced App. Level.")));
    geometry_adv_opt_group_layout->addWidget(geometry_adv_opt_group_geo_label, 0);
    auto* lay74 = new QGridLayout();
    lay74->setContentsMargins(0, 0, 0, 0);
    lay74->setHorizontalSpacing(6);
    lay74->setVerticalSpacing(6);
    auto* w75 = new FCLabel(nullptr);
    w75->setText(trUi_(QString::fromUtf8("Toolchange X-Y:")));
    w75->setToolTip(trUi_(QString::fromUtf8("Toolchange X,Y position.")));
    lay74->addWidget(w75, 1, 0, 1, 1);
    geometry_adv_opt_group_toolchangexy_entry = new NumericalEvalTupleEntry(nullptr);
    geometry_adv_opt_group_toolchangexy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    geometry_adv_opt_group_toolchangexy_entry->setText(trUi_(QString::fromUtf8("0.0, 0.0")));
    lay74->addWidget(geometry_adv_opt_group_toolchangexy_entry, 1, 1, 1, 1);
    auto* w76 = new FCLabel(nullptr);
    w76->setText(trUi_(QString::fromUtf8("Start Z:")));
    w76->setToolTip(trUi_(QString::fromUtf8("Height of the tool just after starting the work.\nDelete the value if you don't need this feature.")));
    lay74->addWidget(w76, 2, 0, 1, 1);
    geometry_adv_opt_group_gstartz_entry = new NumericalEvalEntry(nullptr);
    geometry_adv_opt_group_gstartz_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay74->addWidget(geometry_adv_opt_group_gstartz_entry, 2, 1, 1, 1);
    auto* w77 = new FCLabel(nullptr);
    w77->setText(trUi_(QString::fromUtf8("Feedrate Rapids:")));
    w77->setToolTip(trUi_(QString::fromUtf8("Cutting speed in the XY plane\n(in units per minute).\nThis is for the rapid move G00.\nIt is useful only for Marlin,\nignore for any other cases.")));
    lay74->addWidget(w77, 4, 0, 1, 1);
    geometry_adv_opt_group_feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("maximum", 910000.0);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_feedrate_rapid_entry->setProperty("value", 1500.0);
    geometry_adv_opt_group_feedrate_rapid_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_feedrate_rapid_entry, 4, 1, 1, 1);
    geometry_adv_opt_group_extracut_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_extracut_cb->setText(trUi_(QString::fromUtf8("Re-cut")));
    geometry_adv_opt_group_extracut_cb->setToolTip(trUi_(QString::fromUtf8("In order to remove possible\ncopper leftovers where first cut\nmeet with last cut, we generate an\nextended cut over the first cut section.")));
    lay74->addWidget(geometry_adv_opt_group_extracut_cb, 5, 0, 1, 1);
    geometry_adv_opt_group_e_cut_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_e_cut_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_e_cut_entry->setProperty("maximum", 99999.0);
    geometry_adv_opt_group_e_cut_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_e_cut_entry->setToolTip(trUi_(QString::fromUtf8("In order to remove possible\ncopper leftovers where first cut\nmeet with last cut, we generate an\nextended cut over the first cut section.")));
    geometry_adv_opt_group_e_cut_entry->setProperty("value", 0.1);
    geometry_adv_opt_group_e_cut_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_e_cut_entry, 5, 1, 1, 1);
    geometry_adv_opt_group_pdepth_label = new FCLabel(nullptr);
    geometry_adv_opt_group_pdepth_label->setText(trUi_(QString::fromUtf8("Probe Z depth:")));
    geometry_adv_opt_group_pdepth_label->setToolTip(trUi_(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units.")));
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
    geometry_adv_opt_group_feedrate_probe_label->setText(trUi_(QString::fromUtf8("Feedrate Probe:")));
    geometry_adv_opt_group_feedrate_probe_label->setToolTip(trUi_(QString::fromUtf8("The feedrate used while the probe is probing.")));
    lay74->addWidget(geometry_adv_opt_group_feedrate_probe_label, 7, 0, 1, 1);
    geometry_adv_opt_group_feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("maximum", 910000.0);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_feedrate_probe_entry->setProperty("value", 75.0);
    geometry_adv_opt_group_feedrate_probe_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_feedrate_probe_entry, 7, 1, 1, 1);
    auto* w78 = new FCLabel(nullptr);
    w78->setText(trUi_(QString::fromUtf8("Spindle direction:")));
    w78->setToolTip(trUi_(QString::fromUtf8("This sets the direction that the spindle is rotating.\nIt can be either:\n- CW = clockwise or\n- CCW = counter clockwise")));
    lay74->addWidget(w78, 8, 0, 1, 1);
    geometry_adv_opt_group_spindledir_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    geometry_adv_opt_group_spindledir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay74->addWidget(geometry_adv_opt_group_spindledir_radio, 8, 1, 1, 1);
    geometry_adv_opt_group_fplunge_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_fplunge_cb->setText(trUi_(QString::fromUtf8("Fast Plunge")));
    geometry_adv_opt_group_fplunge_cb->setToolTip(trUi_(QString::fromUtf8("By checking this, the vertical move from\nZ_Toolchange to Z_move is done with G0,\nmeaning the fastest speed available.\nWARNING: the move is done at Toolchange X,Y coords.")));
    lay74->addWidget(geometry_adv_opt_group_fplunge_cb, 9, 0, 1, 2);
    auto* w79 = new FCLabel(nullptr);
    w79->setText(trUi_(QString::fromUtf8("Segment X size:")));
    w79->setToolTip(trUi_(QString::fromUtf8("The size of the trace segment on the X axis.\nUseful for auto-leveling.\nA value of 0 means no segmentation on the X axis.")));
    lay74->addWidget(w79, 10, 0, 1, 1);
    geometry_adv_opt_group_segx_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_segx_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_segx_entry->setProperty("maximum", 99999.0);
    geometry_adv_opt_group_segx_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_segx_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_segx_entry, 10, 1, 1, 1);
    auto* w80 = new FCLabel(nullptr);
    w80->setText(trUi_(QString::fromUtf8("Segment Y size:")));
    w80->setToolTip(trUi_(QString::fromUtf8("The size of the trace segment on the Y axis.\nUseful for auto-leveling.\nA value of 0 means no segmentation on the Y axis.")));
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
    geometry_adv_opt_group_area_exc_label->setText(trUi_(QString::fromUtf8("<b>Area Exclusion:</b>")));
    geometry_adv_opt_group_area_exc_label->setToolTip(trUi_(QString::fromUtf8("Area exclusion parameters.")));
    lay74->addWidget(geometry_adv_opt_group_area_exc_label, 13, 0, 1, 2);
    geometry_adv_opt_group_exclusion_cb = new FCCheckBox(nullptr);
    geometry_adv_opt_group_exclusion_cb->setText(trUi_(QString::fromUtf8("Exclusion areas")));
    geometry_adv_opt_group_exclusion_cb->setToolTip(trUi_(QString::fromUtf8("Include exclusion areas.\nIn those areas the travel of the tools\nis forbidden.")));
    lay74->addWidget(geometry_adv_opt_group_exclusion_cb, 14, 0, 1, 2);
    geometry_adv_opt_group_area_shape_label = new FCLabel(nullptr);
    geometry_adv_opt_group_area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    geometry_adv_opt_group_area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    lay74->addWidget(geometry_adv_opt_group_area_shape_label, 15, 0, 1, 1);
    geometry_adv_opt_group_area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    geometry_adv_opt_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("polygon")));
    lay74->addWidget(geometry_adv_opt_group_area_shape_radio, 15, 1, 1, 1);
    geometry_adv_opt_group_strategy_label = new FCLabel(nullptr);
    geometry_adv_opt_group_strategy_label->setText(trUi_(QString::fromUtf8("Strategy:")));
    geometry_adv_opt_group_strategy_label->setToolTip(trUi_(QString::fromUtf8("The strategy followed when encountering an exclusion area.\nCan be:\n- Over -> when encountering the area, the tool will go to a set height\n- Around -> will avoid the exclusion area by going around the area")));
    lay74->addWidget(geometry_adv_opt_group_strategy_label, 16, 0, 1, 1);
    geometry_adv_opt_group_strategy_radio = new RadioSet({{trUi_(QString::fromUtf8("Over")), QVariant(QString::fromUtf8("over"))}, {trUi_(QString::fromUtf8("Around")), QVariant(QString::fromUtf8("around"))}}, Qt::Horizontal, true, nullptr);
    geometry_adv_opt_group_strategy_radio->setValue(QVariant(QString::fromUtf8("over")));
    lay74->addWidget(geometry_adv_opt_group_strategy_radio, 16, 1, 1, 1);
    geometry_adv_opt_group_over_z_label = new FCLabel(nullptr);
    geometry_adv_opt_group_over_z_label->setText(trUi_(QString::fromUtf8("Over Z:")));
    geometry_adv_opt_group_over_z_label->setToolTip(trUi_(QString::fromUtf8("The height Z to which the tool will rise in order to avoid\nan interdiction area.")));
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
    geometry_adv_opt_group_polish_cb->setText(trUi_(QString::fromUtf8("Add Polish")));
    geometry_adv_opt_group_polish_cb->setToolTip(trUi_(QString::fromUtf8("Will add a Paint section at the end of the GCode.\nA metallic brush will clean the material after milling.")));
    lay74->addWidget(geometry_adv_opt_group_polish_cb, 22, 0, 1, 2);
    geometry_adv_opt_group_polish_dia_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_dia_lbl->setText(trUi_(QString::fromUtf8("Tool Dia:")));
    geometry_adv_opt_group_polish_dia_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the polishing tool.")));
    lay74->addWidget(geometry_adv_opt_group_polish_dia_lbl, 24, 0, 1, 1);
    geometry_adv_opt_group_polish_dia_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_dia_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_dia_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_dia_entry->setProperty("value", 10.0);
    lay74->addWidget(geometry_adv_opt_group_polish_dia_entry, 24, 1, 1, 1);
    geometry_adv_opt_group_polish_travelz_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_travelz_lbl->setText(trUi_(QString::fromUtf8("Travel Z:")));
    geometry_adv_opt_group_polish_travelz_lbl->setToolTip(trUi_(QString::fromUtf8("Height of the tool when\nmoving without cutting.")));
    lay74->addWidget(geometry_adv_opt_group_polish_travelz_lbl, 26, 0, 1, 1);
    geometry_adv_opt_group_polish_travelz_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_polish_travelz_entry->setProperty("value", 2.0);
    lay74->addWidget(geometry_adv_opt_group_polish_travelz_entry, 26, 1, 1, 1);
    geometry_adv_opt_group_polish_pressure_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_pressure_lbl->setText(trUi_(QString::fromUtf8("Pressure:")));
    geometry_adv_opt_group_polish_pressure_lbl->setToolTip(trUi_(QString::fromUtf8("Negative value. The higher the absolute value\nthe stronger the pressure of the brush on the material.")));
    lay74->addWidget(geometry_adv_opt_group_polish_pressure_lbl, 28, 0, 1, 1);
    geometry_adv_opt_group_polish_pressure_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("minimum", -10000.0);
    geometry_adv_opt_group_polish_pressure_entry->setProperty("value", -1.0);
    lay74->addWidget(geometry_adv_opt_group_polish_pressure_entry, 28, 1, 1, 1);
    geometry_adv_opt_group_polish_margin_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_margin_lbl->setText(trUi_(QString::fromUtf8("Margin:")));
    geometry_adv_opt_group_polish_margin_lbl->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay74->addWidget(geometry_adv_opt_group_polish_margin_lbl, 30, 0, 1, 1);
    geometry_adv_opt_group_polish_margin_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_margin_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_margin_entry->setProperty("maximum", 10000.0);
    geometry_adv_opt_group_polish_margin_entry->setProperty("minimum", -10000.0);
    lay74->addWidget(geometry_adv_opt_group_polish_margin_entry, 30, 1, 1, 1);
    geometry_adv_opt_group_polish_over_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_over_lbl->setText(trUi_(QString::fromUtf8("Overlap:")));
    geometry_adv_opt_group_polish_over_lbl->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.")));
    lay74->addWidget(geometry_adv_opt_group_polish_over_lbl, 32, 0, 1, 1);
    geometry_adv_opt_group_polish_over_entry = new FCDoubleSpinner(nullptr);
    geometry_adv_opt_group_polish_over_entry->setProperty("decimals", 4);
    geometry_adv_opt_group_polish_over_entry->setProperty("maximum", 99.9999);
    geometry_adv_opt_group_polish_over_entry->setProperty("singleStep", 0.1);
    geometry_adv_opt_group_polish_over_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    geometry_adv_opt_group_polish_over_entry->setProperty("value", 5.0);
    geometry_adv_opt_group_polish_over_entry->setWrapping(true);
    lay74->addWidget(geometry_adv_opt_group_polish_over_entry, 32, 1, 1, 1);
    geometry_adv_opt_group_polish_method_lbl = new FCLabel(nullptr);
    geometry_adv_opt_group_polish_method_lbl->setText(trUi_(QString::fromUtf8("Method:")));
    geometry_adv_opt_group_polish_method_lbl->setToolTip(trUi_(QString::fromUtf8("Algorithm for polishing:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.")));
    lay74->addWidget(geometry_adv_opt_group_polish_method_lbl, 34, 0, 1, 1);
    geometry_adv_opt_group_polish_method_combo = new FCComboBox2(nullptr);
    geometry_adv_opt_group_polish_method_combo->setProperty("currentText", trUi_(QString::fromUtf8("Standard")));
    geometry_adv_opt_group_polish_method_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    geometry_adv_opt_group_polish_method_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    geometry_adv_opt_group_polish_method_combo->addItem(trUi_(QString::fromUtf8("Lines")));
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
    geometry_editor_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    geometry_editor_group_param_label->setToolTip(trUi_(QString::fromUtf8("A list of Editor parameters.")));
    geometry_editor_group_layout->addWidget(geometry_editor_group_param_label, 0);
    auto* lay83 = new QGridLayout();
    lay83->setContentsMargins(0, 0, 0, 0);
    lay83->setHorizontalSpacing(6);
    lay83->setVerticalSpacing(6);
    geometry_editor_group_sel_limit_label = new QLabel(nullptr);
    geometry_editor_group_sel_limit_label->setText(trUi_(QString::fromUtf8("Selection limit:")));
    geometry_editor_group_sel_limit_label->setToolTip(trUi_(QString::fromUtf8("Set the number of selected geometry\nitems above which the utility geometry\nbecomes just a selection rectangle.\nIncreases the performance when moving a\nlarge number of geometric elements.")));
    lay83->addWidget(geometry_editor_group_sel_limit_label, 0, 0, 1, 1);
    geometry_editor_group_sel_limit_entry = new FCSpinner(nullptr);
    geometry_editor_group_sel_limit_entry->setProperty("maximum", 9999);
    geometry_editor_group_sel_limit_entry->setProperty("value", 30);
    lay83->addWidget(geometry_editor_group_sel_limit_entry, 0, 1, 1, 1);
    auto* w84 = new QLabel(nullptr);
    w84->setText(trUi_(QString::fromUtf8("Milling Type:")));
    w84->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    lay83->addWidget(w84, 1, 0, 1, 1);
    geometry_editor_group_milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
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
    pref_tab_area->addTab(geometry_tab, trUi_(QString::fromUtf8("GEOMETRY")));
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
    cncjob_gen_group_plot_options_label->setText(trUi_(QString::fromUtf8("<b>Plot Options:</b>")));
    cncjob_gen_group_layout->addWidget(cncjob_gen_group_plot_options_label, 0);
    auto* lay85 = new QGridLayout();
    lay85->setContentsMargins(0, 0, 0, 0);
    lay85->setHorizontalSpacing(6);
    lay85->setVerticalSpacing(6);
    cncjob_gen_group_plot_cb = new FCCheckBox(nullptr);
    cncjob_gen_group_plot_cb->setChecked(true);
    cncjob_gen_group_plot_cb->setText(trUi_(QString::fromUtf8("Plot Object")));
    cncjob_gen_group_plot_cb->setToolTip(trUi_(QString::fromUtf8("Plot (show) this object.")));
    lay85->addWidget(cncjob_gen_group_plot_cb, 0, 0, 1, 2);
    cncjob_gen_group_steps_per_circle_label = new QLabel(nullptr);
    cncjob_gen_group_steps_per_circle_label->setText(trUi_(QString::fromUtf8("Circle Steps:")));
    cncjob_gen_group_steps_per_circle_label->setToolTip(trUi_(QString::fromUtf8("The number of circle steps for <b>GCode</b> \ncircle and arc shapes linear approximation.")));
    lay85->addWidget(cncjob_gen_group_steps_per_circle_label, 3, 0, 1, 1);
    cncjob_gen_group_steps_per_circle_entry = new FCSpinner(nullptr);
    cncjob_gen_group_steps_per_circle_entry->setProperty("maximum", 99999);
    cncjob_gen_group_steps_per_circle_entry->setProperty("value", 64);
    lay85->addWidget(cncjob_gen_group_steps_per_circle_entry, 3, 1, 1, 1);
    auto* w86 = new QLabel(nullptr);
    w86->setText(trUi_(QString::fromUtf8("Travel dia:")));
    w86->setToolTip(trUi_(QString::fromUtf8("The width of the travel lines to be\nrendered in the plot.")));
    lay85->addWidget(w86, 4, 0, 1, 1);
    cncjob_gen_group_tooldia_entry = new FCDoubleSpinner(nullptr);
    cncjob_gen_group_tooldia_entry->setProperty("decimals", 4);
    cncjob_gen_group_tooldia_entry->setProperty("maximum", 99999.0);
    cncjob_gen_group_tooldia_entry->setProperty("singleStep", 0.1);
    cncjob_gen_group_tooldia_entry->setProperty("value", 1.0);
    cncjob_gen_group_tooldia_entry->setWrapping(true);
    lay85->addWidget(cncjob_gen_group_tooldia_entry, 4, 1, 1, 1);
    auto* w87 = new QLabel(nullptr);
    w87->setText(trUi_(QString::fromUtf8("<b>G-code Decimals:</b>")));
    lay85->addWidget(w87, 5, 0, 1, 2);
    auto* w88 = new QLabel(nullptr);
    w88->setText(trUi_(QString::fromUtf8("Coordinates:")));
    w88->setToolTip(trUi_(QString::fromUtf8("The number of decimals to be used for \nthe X, Y, Z coordinates in CNC code (GCODE, etc.)")));
    lay85->addWidget(w88, 6, 0, 1, 1);
    cncjob_gen_group_coords_dec_entry = new FCSpinner(nullptr);
    cncjob_gen_group_coords_dec_entry->setProperty("maximum", 9);
    cncjob_gen_group_coords_dec_entry->setProperty("value", 4);
    cncjob_gen_group_coords_dec_entry->setWrapping(true);
    lay85->addWidget(cncjob_gen_group_coords_dec_entry, 6, 1, 1, 1);
    auto* w89 = new QLabel(nullptr);
    w89->setText(trUi_(QString::fromUtf8("Feedrate:")));
    w89->setToolTip(trUi_(QString::fromUtf8("The number of decimals to be used for \nthe Feedrate parameter in CNC code (GCODE, etc.)")));
    lay85->addWidget(w89, 7, 0, 1, 1);
    cncjob_gen_group_fr_dec_entry = new FCSpinner(nullptr);
    cncjob_gen_group_fr_dec_entry->setProperty("maximum", 9);
    cncjob_gen_group_fr_dec_entry->setProperty("value", 2);
    cncjob_gen_group_fr_dec_entry->setWrapping(true);
    lay85->addWidget(cncjob_gen_group_fr_dec_entry, 7, 1, 1, 1);
    auto* w90 = new QLabel(nullptr);
    w90->setText(trUi_(QString::fromUtf8("Coordinates type:")));
    w90->setToolTip(trUi_(QString::fromUtf8("The type of coordinates to be used in Gcode.\nCan be:\n- Absolute G90 -> the reference is the origin x=0, y=0\n- Incremental G91 -> the reference is the previous position")));
    w90->setHidden(true);
    lay85->addWidget(w90, 8, 0, 1, 1);
    cncjob_gen_group_coords_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Absolute")), QVariant(QString::fromUtf8("G90"))}, {trUi_(QString::fromUtf8("Incremental")), QVariant(QString::fromUtf8("G91"))}}, Qt::Vertical, false, nullptr);
    cncjob_gen_group_coords_type_radio->setHidden(true);
    cncjob_gen_group_coords_type_radio->setValue(QVariant(QString::fromUtf8("G90")));
    lay85->addWidget(cncjob_gen_group_coords_type_radio, 8, 1, 1, 1);
    cncjob_gen_group_line_ending_cb = new FCCheckBox(nullptr);
    cncjob_gen_group_line_ending_cb->setText(trUi_(QString::fromUtf8("Force Windows style line-ending")));
    cncjob_gen_group_line_ending_cb->setToolTip(trUi_(QString::fromUtf8("When checked will force a Windows style line-ending\n(\\r\\n) on non-Windows OS's.")));
    lay85->addWidget(cncjob_gen_group_line_ending_cb, 9, 0, 1, 3);
    auto* w91 = new QFrame(nullptr);
    w91->setProperty("frameShadow", 48);
    w91->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w91->setSizePolicy(sp); }
    lay85->addWidget(w91, 12, 0, 1, 2);
    cncjob_gen_group_travel_color_label = new QLabel(nullptr);
    cncjob_gen_group_travel_color_label->setText(trUi_(QString::fromUtf8("<b>Travel Line Color</b>")));
    lay85->addWidget(cncjob_gen_group_travel_color_label, 13, 0, 1, 2);
    cncjob_gen_group_tline_color_label = new QLabel(nullptr);
    cncjob_gen_group_tline_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    cncjob_gen_group_tline_color_label->setToolTip(trUi_(QString::fromUtf8("Set the travel line color for plotted objects.")));
    lay85->addWidget(cncjob_gen_group_tline_color_label, 14, 0, 1, 1);
    cncjob_gen_group_tline_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_tline_color_entry->setValue(QString::fromUtf8("#B5AB3A4c"));
    lay85->addWidget(cncjob_gen_group_tline_color_entry, 14, 1, 1, 1);
    cncjob_gen_group_tfill_color_label = new QLabel(nullptr);
    cncjob_gen_group_tfill_color_label->setText(trUi_(QString::fromUtf8("Fill:")));
    cncjob_gen_group_tfill_color_label->setToolTip(trUi_(QString::fromUtf8("Set the fill color for plotted objects.\nFirst 6 digits are the color and the last 2\ndigits are for alpha (transparency) level.")));
    lay85->addWidget(cncjob_gen_group_tfill_color_label, 15, 0, 1, 1);
    cncjob_gen_group_tfill_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_tfill_color_entry->setValue(QString::fromUtf8("#F0E24D4c"));
    lay85->addWidget(cncjob_gen_group_tfill_color_entry, 15, 1, 1, 1);
    cncjob_gen_group_cncjob_alpha_label = new QLabel(nullptr);
    cncjob_gen_group_cncjob_alpha_label->setText(trUi_(QString::fromUtf8("Alpha:")));
    cncjob_gen_group_cncjob_alpha_label->setToolTip(trUi_(QString::fromUtf8("Set the fill transparency for plotted objects.")));
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
    cncjob_gen_group_cnc_color_label->setText(trUi_(QString::fromUtf8("<b>Object Color</b>")));
    lay85->addWidget(cncjob_gen_group_cnc_color_label, 18, 0, 1, 2);
    cncjob_gen_group_line_color_label = new QLabel(nullptr);
    cncjob_gen_group_line_color_label->setText(trUi_(QString::fromUtf8("Outline:")));
    cncjob_gen_group_line_color_label->setToolTip(trUi_(QString::fromUtf8("Set the color for plotted objects.")));
    lay85->addWidget(cncjob_gen_group_line_color_label, 19, 0, 1, 1);
    cncjob_gen_group_line_color_entry = new FCColorEntry(nullptr);
    cncjob_gen_group_line_color_entry->setValue(QString::fromUtf8("#4650BDFF"));
    lay85->addWidget(cncjob_gen_group_line_color_entry, 19, 1, 1, 1);
    cncjob_gen_group_fill_color_label = new QLabel(nullptr);
    cncjob_gen_group_fill_color_label->setText(trUi_(QString::fromUtf8("Fill:")));
    cncjob_gen_group_fill_color_label->setToolTip(trUi_(QString::fromUtf8("Set the fill color for plotted objects.\nFirst 6 digits are the color and the last 2\ndigits are for alpha (transparency) level.")));
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
    cncjob_opt_group_export_gcode_label->setText(trUi_(QString::fromUtf8("<b>Export G-Code:</b>")));
    cncjob_opt_group_export_gcode_label->setToolTip(trUi_(QString::fromUtf8("Export and save G-Code to\nmake this object to a file.")));
    cncjob_opt_group_layout->addWidget(cncjob_opt_group_export_gcode_label, 0);
    auto* lay94 = new QGridLayout();
    lay94->setContentsMargins(0, 0, 0, 0);
    lay94->setHorizontalSpacing(6);
    lay94->setVerticalSpacing(6);
    cncjob_opt_group_cncplot_method_label = new QLabel(nullptr);
    cncjob_opt_group_cncplot_method_label->setText(trUi_(QString::fromUtf8("Plot kind:")));
    cncjob_opt_group_cncplot_method_label->setToolTip(trUi_(QString::fromUtf8("This selects the kind of geometries on the canvas to plot.\nThose can be either of type 'Travel' which means the moves\nabove the work piece or it can be of type 'Cut',\nwhich means the moves that cut into the material.")));
    lay94->addWidget(cncjob_opt_group_cncplot_method_label, 1, 0, 1, 1);
    cncjob_opt_group_cncplot_method_radio = new RadioSet({{trUi_(QString::fromUtf8("All")), QVariant(QString::fromUtf8("all"))}, {trUi_(QString::fromUtf8("Travel")), QVariant(QString::fromUtf8("travel"))}, {trUi_(QString::fromUtf8("Cut")), QVariant(QString::fromUtf8("cut"))}}, Qt::Vertical, false, nullptr);
    cncjob_opt_group_cncplot_method_radio->setValue(QVariant(QString::fromUtf8("all")));
    lay94->addWidget(cncjob_opt_group_cncplot_method_radio, 1, 1, 1, 1);
    auto* w95 = new QLabel(nullptr);
    lay94->addWidget(w95, 1, 2, 1, 1);
    cncjob_opt_group_annotation_cb = new FCCheckBox(nullptr);
    cncjob_opt_group_annotation_cb->setChecked(true);
    cncjob_opt_group_annotation_cb->setText(trUi_(QString::fromUtf8("Display Annotation")));
    cncjob_opt_group_annotation_cb->setToolTip(trUi_(QString::fromUtf8("This selects if to display text annotation on the plot.\nWhen checked it will display numbers in order for each end\nof a travel line.")));
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
    cncjob_adv_opt_group_export_gcode_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    cncjob_adv_opt_group_export_gcode_label->setToolTip(trUi_(QString::fromUtf8("Export and save G-Code to\nmake this object to a file.")));
    lay96->addWidget(cncjob_adv_opt_group_export_gcode_label, 0, 0, 1, 2);
    cncjob_adv_opt_group_annotation_fontsize_label = new QLabel(nullptr);
    cncjob_adv_opt_group_annotation_fontsize_label->setText(trUi_(QString::fromUtf8("Annotation Size:")));
    cncjob_adv_opt_group_annotation_fontsize_label->setToolTip(trUi_(QString::fromUtf8("The font size of the annotation text. In pixels.")));
    lay96->addWidget(cncjob_adv_opt_group_annotation_fontsize_label, 2, 0, 1, 1);
    cncjob_adv_opt_group_annotation_fontsize_sp = new FCSpinner(nullptr);
    cncjob_adv_opt_group_annotation_fontsize_sp->setProperty("maximum", 9999);
    cncjob_adv_opt_group_annotation_fontsize_sp->setProperty("value", 9);
    lay96->addWidget(cncjob_adv_opt_group_annotation_fontsize_sp, 2, 1, 1, 1);
    cncjob_adv_opt_group_annotation_color_label = new QLabel(nullptr);
    cncjob_adv_opt_group_annotation_color_label->setText(trUi_(QString::fromUtf8("Annotation Color:")));
    cncjob_adv_opt_group_annotation_color_label->setToolTip(trUi_(QString::fromUtf8("Set the font color for the annotation texts.")));
    lay96->addWidget(cncjob_adv_opt_group_annotation_color_label, 4, 0, 1, 1);
    cncjob_adv_opt_group_annotation_fontcolor_entry = new FCColorEntry(nullptr);
    cncjob_adv_opt_group_annotation_fontcolor_entry->setValue(QString::fromUtf8("#990000"));
    lay96->addWidget(cncjob_adv_opt_group_annotation_fontcolor_entry, 4, 1, 1, 1);
    cncjob_adv_opt_group_autolevelling_gcode_label = new QLabel(nullptr);
    cncjob_adv_opt_group_autolevelling_gcode_label->setText(trUi_(QString::fromUtf8("<b>Autolevelling</b>")));
    cncjob_adv_opt_group_autolevelling_gcode_label->setToolTip(trUi_(QString::fromUtf8("Parameters for the autolevelling.")));
    lay96->addWidget(cncjob_adv_opt_group_autolevelling_gcode_label, 6, 0, 1, 2);
    auto* w97 = new FCLabel(nullptr);
    w97->setText(trUi_(QString::fromUtf8("Mode:")));
    w97->setToolTip(trUi_(QString::fromUtf8("Choose a mode for height map generation.\n- Manual: will pick a selection of probe points by clicking on canvas\n- Grid: will automatically generate a grid of probe points")));
    lay96->addWidget(w97, 8, 0, 1, 1);
    cncjob_adv_opt_group_al_mode_radio = new RadioSet({{trUi_(QString::fromUtf8("Manual")), QVariant(QString::fromUtf8("manual"))}, {trUi_(QString::fromUtf8("Grid")), QVariant(QString::fromUtf8("grid"))}}, Qt::Horizontal, true, nullptr);
    cncjob_adv_opt_group_al_mode_radio->setValue(QVariant(QString::fromUtf8("grid")));
    lay96->addWidget(cncjob_adv_opt_group_al_mode_radio, 8, 1, 1, 1);
    cncjob_adv_opt_group_al_method_lbl = new FCLabel(nullptr);
    cncjob_adv_opt_group_al_method_lbl->setText(trUi_(QString::fromUtf8("Method:")));
    cncjob_adv_opt_group_al_method_lbl->setToolTip(trUi_(QString::fromUtf8("Choose a method for approximation of heights from autolevelling data.\n- Voronoi: will generate a Voronoi diagram\n- Bilinear: will use bilinear interpolation. Usable only for grid mode.")));
    lay96->addWidget(cncjob_adv_opt_group_al_method_lbl, 9, 0, 1, 1);
    cncjob_adv_opt_group_al_method_radio = new RadioSet({{trUi_(QString::fromUtf8("Voronoi")), QVariant(QString::fromUtf8("v"))}, {trUi_(QString::fromUtf8("Bilinear")), QVariant(QString::fromUtf8("b"))}}, Qt::Horizontal, true, nullptr);
    cncjob_adv_opt_group_al_method_radio->setValue(QVariant(QString::fromUtf8("v")));
    lay96->addWidget(cncjob_adv_opt_group_al_method_radio, 9, 1, 1, 1);
    cncjob_adv_opt_group_al_columns_label = new QLabel(nullptr);
    cncjob_adv_opt_group_al_columns_label->setText(trUi_(QString::fromUtf8("Columns:")));
    cncjob_adv_opt_group_al_columns_label->setToolTip(trUi_(QString::fromUtf8("The number of grid columns.")));
    lay96->addWidget(cncjob_adv_opt_group_al_columns_label, 10, 0, 1, 1);
    cncjob_adv_opt_group_al_columns_entry = new FCSpinner(nullptr);
    cncjob_adv_opt_group_al_columns_entry->setProperty("value", 4);
    lay96->addWidget(cncjob_adv_opt_group_al_columns_entry, 10, 1, 1, 1);
    cncjob_adv_opt_group_al_rows_label = new QLabel(nullptr);
    cncjob_adv_opt_group_al_rows_label->setText(trUi_(QString::fromUtf8("Rows:")));
    cncjob_adv_opt_group_al_rows_label->setToolTip(trUi_(QString::fromUtf8("The number of grid rows.")));
    lay96->addWidget(cncjob_adv_opt_group_al_rows_label, 12, 0, 1, 1);
    cncjob_adv_opt_group_al_rows_entry = new FCSpinner(nullptr);
    cncjob_adv_opt_group_al_rows_entry->setProperty("value", 4);
    lay96->addWidget(cncjob_adv_opt_group_al_rows_entry, 12, 1, 1, 1);
    cncjob_adv_opt_group_ptravelz_label = new QLabel(nullptr);
    cncjob_adv_opt_group_ptravelz_label->setText(trUi_(QString::fromUtf8("Probe Z travel:")));
    cncjob_adv_opt_group_ptravelz_label->setToolTip(trUi_(QString::fromUtf8("The safe Z for probe travelling between probe points.")));
    lay96->addWidget(cncjob_adv_opt_group_ptravelz_label, 14, 0, 1, 1);
    cncjob_adv_opt_group_ptravelz_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_ptravelz_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_ptravelz_entry->setProperty("maximum", 10000.0);
    cncjob_adv_opt_group_ptravelz_entry->setProperty("value", 2.0);
    lay96->addWidget(cncjob_adv_opt_group_ptravelz_entry, 14, 1, 1, 1);
    cncjob_adv_opt_group_pdepth_label = new QLabel(nullptr);
    cncjob_adv_opt_group_pdepth_label->setText(trUi_(QString::fromUtf8("Probe Z depth:")));
    cncjob_adv_opt_group_pdepth_label->setToolTip(trUi_(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units.")));
    lay96->addWidget(cncjob_adv_opt_group_pdepth_label, 16, 0, 1, 1);
    cncjob_adv_opt_group_pdepth_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_pdepth_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_pdepth_entry->setProperty("maximum", 0.0);
    cncjob_adv_opt_group_pdepth_entry->setProperty("minimum", -910000.0);
    cncjob_adv_opt_group_pdepth_entry->setProperty("value", -1.0);
    lay96->addWidget(cncjob_adv_opt_group_pdepth_entry, 16, 1, 1, 1);
    cncjob_adv_opt_group_feedrate_probe_label = new QLabel(nullptr);
    cncjob_adv_opt_group_feedrate_probe_label->setText(trUi_(QString::fromUtf8("Probe Feedrate:")));
    cncjob_adv_opt_group_feedrate_probe_label->setToolTip(trUi_(QString::fromUtf8("The feedrate used while the probe is probing.")));
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
    cncjob_adv_opt_group_al_controller_label->setText(trUi_(QString::fromUtf8("Controller:")));
    cncjob_adv_opt_group_al_controller_label->setToolTip(trUi_(QString::fromUtf8("The kind of controller for which to generate\nheight map gcode.")));
    lay96->addWidget(cncjob_adv_opt_group_al_controller_label, 22, 0, 1, 1);
    cncjob_adv_opt_group_al_controller_combo = new FCComboBox(nullptr);
    cncjob_adv_opt_group_al_controller_combo->setProperty("currentText", trUi_(QString::fromUtf8("MACH3")));
    cncjob_adv_opt_group_al_controller_combo->addItem(trUi_(QString::fromUtf8("MACH3")));
    cncjob_adv_opt_group_al_controller_combo->addItem(trUi_(QString::fromUtf8("MACH4")));
    cncjob_adv_opt_group_al_controller_combo->addItem(trUi_(QString::fromUtf8("LinuxCNC")));
    cncjob_adv_opt_group_al_controller_combo->addItem(trUi_(QString::fromUtf8("GRBL")));
    lay96->addWidget(cncjob_adv_opt_group_al_controller_combo, 22, 1, 1, 1);
    cncjob_adv_opt_group_jog_step_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_jog_step_label->setText(trUi_(QString::fromUtf8("Step:")));
    cncjob_adv_opt_group_jog_step_label->setToolTip(trUi_(QString::fromUtf8("Each jog action will move the axes with this value.")));
    lay96->addWidget(cncjob_adv_opt_group_jog_step_label, 24, 0, 1, 1);
    cncjob_adv_opt_group_jog_step_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_jog_step_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_jog_step_entry->setProperty("maximum", 910000.0);
    cncjob_adv_opt_group_jog_step_entry->setProperty("value", 5.0);
    lay96->addWidget(cncjob_adv_opt_group_jog_step_entry, 24, 1, 1, 1);
    cncjob_adv_opt_group_jog_fr_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_jog_fr_label->setText(trUi_(QString::fromUtf8("Feedrate:")));
    cncjob_adv_opt_group_jog_fr_label->setToolTip(trUi_(QString::fromUtf8("Feedrate when jogging.")));
    lay96->addWidget(cncjob_adv_opt_group_jog_fr_label, 26, 0, 1, 1);
    cncjob_adv_opt_group_jog_fr_entry = new FCDoubleSpinner(nullptr);
    cncjob_adv_opt_group_jog_fr_entry->setProperty("decimals", 4);
    cncjob_adv_opt_group_jog_fr_entry->setProperty("maximum", 910000.0);
    cncjob_adv_opt_group_jog_fr_entry->setProperty("value", 1500.0);
    lay96->addWidget(cncjob_adv_opt_group_jog_fr_entry, 26, 1, 1, 1);
    cncjob_adv_opt_group_jog_travelz_label = new FCLabel(nullptr);
    cncjob_adv_opt_group_jog_travelz_label->setText(trUi_(QString::fromUtf8("Travel Z:")));
    cncjob_adv_opt_group_jog_travelz_label->setToolTip(trUi_(QString::fromUtf8("Safe height (Z) distance when jogging to origin.")));
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
    cncjob_editor_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    cncjob_editor_group_param_label->setToolTip(trUi_(QString::fromUtf8("A list of Editor parameters.")));
    cncjob_editor_group_layout->addWidget(cncjob_editor_group_param_label, 0);
    auto* w99 = new QLabel(nullptr);
    w99->setText(trUi_(QString::fromUtf8("Prepend to G-Code:")));
    w99->setToolTip(trUi_(QString::fromUtf8("Type here any G-Code commands you would\nlike to add at the beginning of the G-Code file.")));
    cncjob_editor_group_layout->addWidget(w99, 0);
    cncjob_editor_group_prepend_text = new FCTextArea(nullptr);
    { QFont f = cncjob_editor_group_prepend_text->font(); f.setPointSizeF(10.0); cncjob_editor_group_prepend_text->setFont(f); }
    cncjob_editor_group_prepend_text->setProperty("placeholderText", trUi_(QString::fromUtf8("Type here any G-Code commands you would like to add at the beginning of the G-Code file.")));
    cncjob_editor_group_layout->addWidget(cncjob_editor_group_prepend_text, 0);
    auto* w100 = new QLabel(nullptr);
    w100->setText(trUi_(QString::fromUtf8("Append to G-Code:")));
    w100->setToolTip(trUi_(QString::fromUtf8("Type here any G-Code commands you would\nlike to append to the generated file.\nI.e.: M2 (End of program)")));
    cncjob_editor_group_layout->addWidget(w100, 0);
    cncjob_editor_group_append_text = new FCTextArea(nullptr);
    { QFont f = cncjob_editor_group_append_text->font(); f.setPointSizeF(10.0); cncjob_editor_group_append_text->setFont(f); }
    cncjob_editor_group_append_text->setProperty("placeholderText", trUi_(QString::fromUtf8("Type here any G-Code commands you would like to append to the generated file.\nI.e.: M2 (End of program)")));
    cncjob_editor_group_layout->addWidget(cncjob_editor_group_append_text, 0);
    cncjob_editor_group_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    cncjob_editor_group->setLayout(cncjob_editor_group_layout);
    layout->addWidget(cncjob_editor_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    cncjob_defaults_form->setLayout(layout);
    cncjob_scroll_area->setWidget(cncjob_defaults_form);
    cncjob_tab_lay->addWidget(cncjob_scroll_area, 0);
    text_editor_tab->setLayout(cncjob_tab_lay);
    pref_tab_area->addTab(text_editor_tab, trUi_(QString::fromUtf8("CNC-JOB")));
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
    tools_iso_group_iso_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_iso_group_iso_label->setToolTip(trUi_(QString::fromUtf8("Create a Geometry object with\ntoolpaths to cut around polygons.")));
    tools_iso_group_layout->addWidget(tools_iso_group_iso_label, 0);
    auto* lay101 = new QGridLayout();
    lay101->setContentsMargins(0, 0, 0, 0);
    lay101->setHorizontalSpacing(6);
    lay101->setVerticalSpacing(6);
    auto* w102 = new QLabel(nullptr);
    w102->setText(trUi_(QString::fromUtf8("<b><font color=\"green\">Tools Dia:</font></b>")));
    w102->setToolTip(trUi_(QString::fromUtf8("Diameters of the tools, separated by comma.\nThe value of the diameter has to use the dot decimals separator.\nValid values: 0.3, 1.0")));
    lay101->addWidget(w102, 0, 0, 1, 1);
    tools_iso_group_tool_dia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_iso_group_tool_dia_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Comma separated values")));
    tools_iso_group_tool_dia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_iso_group_tool_dia_entry->setText(trUi_(QString::fromUtf8("0.1")));
    lay101->addWidget(tools_iso_group_tool_dia_entry, 0, 1, 1, 2);
    tools_iso_group_order_label = new QLabel(nullptr);
    tools_iso_group_order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    tools_iso_group_order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay101->addWidget(tools_iso_group_order_label, 1, 0, 1, 1);
    tools_iso_group_order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay101->addWidget(tools_iso_group_order_radio, 1, 1, 1, 2);
    tools_iso_group_tool_type_label = new QLabel(nullptr);
    tools_iso_group_tool_type_label->setText(trUi_(QString::fromUtf8("Tool Type:")));
    tools_iso_group_tool_type_label->setToolTip(trUi_(QString::fromUtf8("Default tool type:\n- 'V-shape'\n- Circular")));
    lay101->addWidget(tools_iso_group_tool_type_label, 2, 0, 1, 1);
    tools_iso_group_tool_type_radio = new RadioSet({{trUi_(QString::fromUtf8("V-shape")), QVariant(QString::fromUtf8("V"))}, {trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("C1"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_tool_type_radio->setToolTip(trUi_(QString::fromUtf8("Default tool type:\n- 'V-shape'\n- Circular")));
    tools_iso_group_tool_type_radio->setValue(QVariant(QString::fromUtf8("C1")));
    lay101->addWidget(tools_iso_group_tool_type_radio, 2, 1, 1, 2);
    tools_iso_group_tipdialabel = new QLabel(nullptr);
    tools_iso_group_tipdialabel->setText(trUi_(QString::fromUtf8("V-Tip Dia:")));
    tools_iso_group_tipdialabel->setToolTip(trUi_(QString::fromUtf8("The tip diameter for V-Shape Tool")));
    lay101->addWidget(tools_iso_group_tipdialabel, 3, 0, 1, 1);
    tools_iso_group_tipdia_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_tipdia_entry->setProperty("decimals", 4);
    tools_iso_group_tipdia_entry->setProperty("maximum", 1000.0);
    tools_iso_group_tipdia_entry->setProperty("singleStep", 0.1);
    tools_iso_group_tipdia_entry->setProperty("value", 0.1);
    lay101->addWidget(tools_iso_group_tipdia_entry, 3, 1, 1, 2);
    tools_iso_group_tipanglelabel = new QLabel(nullptr);
    tools_iso_group_tipanglelabel->setText(trUi_(QString::fromUtf8("V-Tip Angle:")));
    tools_iso_group_tipanglelabel->setToolTip(trUi_(QString::fromUtf8("The tip angle for V-Shape Tool.\nIn degrees.")));
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
    w103->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w103->setToolTip(trUi_(QString::fromUtf8("Depth of cut into material. Negative value.\nIn application units.")));
    lay101->addWidget(w103, 5, 0, 1, 1);
    tools_iso_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_cutz_entry->setProperty("decimals", 4);
    tools_iso_group_cutz_entry->setProperty("maximum", 0.0);
    tools_iso_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_iso_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_iso_group_cutz_entry->setToolTip(trUi_(QString::fromUtf8("Depth of cut into material. Negative value.\nIn application units.")));
    tools_iso_group_cutz_entry->setProperty("value", -0.05);
    lay101->addWidget(tools_iso_group_cutz_entry, 5, 1, 1, 2);
    tools_iso_group_newdialabel = new QLabel(nullptr);
    tools_iso_group_newdialabel->setText(trUi_(QString::fromUtf8("New Dia:")));
    tools_iso_group_newdialabel->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool to add in the Tool Table.\nIf the tool is V-shape type then this value is automatically\ncalculated from the other parameters.")));
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
    w105->setText(trUi_(QString::fromUtf8("Passes:")));
    w105->setToolTip(trUi_(QString::fromUtf8("Width of the isolation gap in\nnumber (integer) of tool widths.")));
    lay101->addWidget(w105, 8, 0, 1, 1);
    tools_iso_group_passes_entry = new FCSpinner(nullptr);
    tools_iso_group_passes_entry->setProperty("maximum", 999);
    tools_iso_group_passes_entry->setProperty("minimum", 1);
    tools_iso_group_passes_entry->setProperty("objectName", QString::fromUtf8("i_passes"));
    tools_iso_group_passes_entry->setProperty("value", 1);
    lay101->addWidget(tools_iso_group_passes_entry, 8, 1, 1, 2);
    auto* w106 = new QLabel(nullptr);
    w106->setText(trUi_(QString::fromUtf8("Overlap:")));
    w106->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.")));
    lay101->addWidget(w106, 9, 0, 1, 1);
    tools_iso_group_overlap_entry = new FCDoubleSpinner(nullptr);
    tools_iso_group_overlap_entry->setProperty("decimals", 4);
    tools_iso_group_overlap_entry->setProperty("maximum", 99.9999);
    tools_iso_group_overlap_entry->setProperty("objectName", QString::fromUtf8("i_overlap"));
    tools_iso_group_overlap_entry->setProperty("singleStep", 0.1);
    tools_iso_group_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    tools_iso_group_overlap_entry->setProperty("value", 10.0);
    tools_iso_group_overlap_entry->setWrapping(true);
    lay101->addWidget(tools_iso_group_overlap_entry, 9, 1, 1, 2);
    tools_iso_group_milling_type_label = new QLabel(nullptr);
    tools_iso_group_milling_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    tools_iso_group_milling_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    lay101->addWidget(tools_iso_group_milling_type_label, 10, 0, 1, 1);
    tools_iso_group_milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_milling_type_radio->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    tools_iso_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    lay101->addWidget(tools_iso_group_milling_type_radio, 10, 1, 1, 2);
    tools_iso_group_follow_label = new QLabel(nullptr);
    tools_iso_group_follow_label->setText(trUi_(QString::fromUtf8("Follow:")));
    tools_iso_group_follow_label->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    lay101->addWidget(tools_iso_group_follow_label, 11, 0, 1, 1);
    tools_iso_group_follow_cb = new FCCheckBox(nullptr);
    tools_iso_group_follow_cb->setProperty("objectName", QString::fromUtf8("i_follow"));
    tools_iso_group_follow_cb->setToolTip(trUi_(QString::fromUtf8("Generate a 'Follow' geometry.\nThis means that it will cut through\nthe middle of the trace.")));
    lay101->addWidget(tools_iso_group_follow_cb, 11, 1, 1, 2);
    tools_iso_group_iso_type_label = new QLabel(nullptr);
    tools_iso_group_iso_type_label->setText(trUi_(QString::fromUtf8("Isolation Type:")));
    tools_iso_group_iso_type_label->setToolTip(trUi_(QString::fromUtf8("Choose how the isolation will be executed:\n- 'Full' -> complete isolation of polygons\n- 'Ext' -> will isolate only on the outside\n- 'Int' -> will isolate only on the inside\n'Exterior' isolation is almost always possible\n(with the right tool) but 'Interior'\nisolation can be done only when there is an opening\ninside of the polygon (e.g polygon is a 'doughnut' shape).")));
    lay101->addWidget(tools_iso_group_iso_type_label, 12, 0, 1, 1);
    tools_iso_group_iso_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Full")), QVariant(QString::fromUtf8("full"))}, {trUi_(QString::fromUtf8("Ext")), QVariant(QString::fromUtf8("ext"))}, {trUi_(QString::fromUtf8("Int")), QVariant(QString::fromUtf8("int"))}}, Qt::Horizontal, true, nullptr);
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
    tools_iso_group_rest_cb->setText(trUi_(QString::fromUtf8("Rest")));
    tools_iso_group_rest_cb->setToolTip(trUi_(QString::fromUtf8("If checked, use 'rest machining'.\nBasically it will process copper outside PCB features,\nusing the biggest tool and continue with the next tools,\nfrom bigger to smaller, to process the copper features that\ncould not be processed by previous tool, until there is\nnothing left to process or there are no more tools.\n\nIf not checked, use the standard algorithm.")));
    lay101->addWidget(tools_iso_group_rest_cb, 17, 0, 1, 1);
    tools_iso_group_combine_passes_cb = new FCCheckBox(nullptr);
    tools_iso_group_combine_passes_cb->setChecked(true);
    tools_iso_group_combine_passes_cb->setProperty("objectName", QString::fromUtf8("i_combine"));
    tools_iso_group_combine_passes_cb->setText(trUi_(QString::fromUtf8("Combine")));
    tools_iso_group_combine_passes_cb->setToolTip(trUi_(QString::fromUtf8("Combine all passes into one object")));
    lay101->addWidget(tools_iso_group_combine_passes_cb, 17, 1, 1, 1);
    tools_iso_group_except_cb = new FCCheckBox(nullptr);
    tools_iso_group_except_cb->setProperty("objectName", QString::fromUtf8("i_except"));
    tools_iso_group_except_cb->setText(trUi_(QString::fromUtf8("Except")));
    tools_iso_group_except_cb->setToolTip(trUi_(QString::fromUtf8("When the isolation geometry is generated,\nby checking this, the area of the object below\nwill be subtracted from the isolation geometry.")));
    lay101->addWidget(tools_iso_group_except_cb, 17, 2, 1, 1);
    tools_iso_group_valid_cb = new FCCheckBox(nullptr);
    tools_iso_group_valid_cb->setProperty("objectName", QString::fromUtf8("i_check"));
    tools_iso_group_valid_cb->setText(trUi_(QString::fromUtf8("Check validity")));
    tools_iso_group_valid_cb->setToolTip(trUi_(QString::fromUtf8("If checked then the tools diameters are verified\nif they will provide a complete isolation.")));
    lay101->addWidget(tools_iso_group_valid_cb, 18, 0, 1, 3);
    tools_iso_group_select_label = new QLabel(nullptr);
    tools_iso_group_select_label->setText(trUi_(QString::fromUtf8("Selection:")));
    tools_iso_group_select_label->setToolTip(trUi_(QString::fromUtf8("Isolation scope. Choose what to isolate:\n- 'All' -> Isolate all the polygons in the object\n- 'Area Selection' -> Isolate polygons within a selection area.\n- 'Polygon Selection' -> Isolate a selection of polygons.\n- 'Reference Object' - will process the area specified by another object.")));
    lay101->addWidget(tools_iso_group_select_label, 20, 0, 1, 1);
    tools_iso_group_select_combo = new FCComboBox2(nullptr);
    tools_iso_group_select_combo->setProperty("currentText", trUi_(QString::fromUtf8("All")));
    tools_iso_group_select_combo->setProperty("objectName", QString::fromUtf8("i_selection"));
    tools_iso_group_select_combo->addItem(trUi_(QString::fromUtf8("All")));
    tools_iso_group_select_combo->addItem(trUi_(QString::fromUtf8("Area Selection")));
    tools_iso_group_select_combo->addItem(trUi_(QString::fromUtf8("Polygon Selection")));
    tools_iso_group_select_combo->addItem(trUi_(QString::fromUtf8("Reference Object")));
    lay101->addWidget(tools_iso_group_select_combo, 20, 1, 1, 2);
    tools_iso_group_area_shape_label = new QLabel(nullptr);
    tools_iso_group_area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    tools_iso_group_area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    lay101->addWidget(tools_iso_group_area_shape_label, 21, 0, 1, 1);
    tools_iso_group_area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_iso_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay101->addWidget(tools_iso_group_area_shape_radio, 21, 1, 1, 2);
    tools_iso_group_poly_int_cb = new FCCheckBox(nullptr);
    tools_iso_group_poly_int_cb->setText(trUi_(QString::fromUtf8("Interiors")));
    tools_iso_group_poly_int_cb->setToolTip(trUi_(QString::fromUtf8("When checked the user can select interiors of a polygon.\n(holes in the polygon).")));
    lay101->addWidget(tools_iso_group_poly_int_cb, 22, 0, 1, 1);
    tools_iso_group_force_iso_cb = new FCCheckBox(nullptr);
    tools_iso_group_force_iso_cb->setChecked(true);
    tools_iso_group_force_iso_cb->setText(trUi_(QString::fromUtf8("Forced Rest")));
    tools_iso_group_force_iso_cb->setToolTip(trUi_(QString::fromUtf8("When checked the isolation will be done with the current tool even if\ninteriors of a polygon (holes in the polygon) could not be isolated.\nWorks when 'rest machining' is used.")));
    lay101->addWidget(tools_iso_group_force_iso_cb, 22, 1, 1, 1);
    auto* w108 = new QFrame(nullptr);
    w108->setProperty("frameShadow", 48);
    w108->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w108->setSizePolicy(sp); }
    lay101->addWidget(w108, 24, 0, 1, 3);
    auto* w109 = new QLabel(nullptr);
    w109->setText(trUi_(QString::fromUtf8("Plotting:")));
    w109->setToolTip(trUi_(QString::fromUtf8("- 'Normal' - normal plotting, done at the end of the job\n- 'Progressive' - each shape is plotted after it is generated")));
    lay101->addWidget(w109, 25, 0, 1, 1);
    tools_iso_group_plotting_radio = new RadioSet({{trUi_(QString::fromUtf8("Normal")), QVariant(QString::fromUtf8("normal"))}, {trUi_(QString::fromUtf8("Progressive")), QVariant(QString::fromUtf8("progressive"))}}, Qt::Horizontal, true, nullptr);
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
    tools_2sided_group_dblsided_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_2sided_group_dblsided_label->setToolTip(trUi_(QString::fromUtf8("A tool to help in creating a double sided\nPCB using alignment holes.")));
    tools_2sided_group_layout->addWidget(tools_2sided_group_dblsided_label, 0);
    auto* lay110 = new QGridLayout();
    lay110->setContentsMargins(0, 0, 0, 0);
    lay110->setHorizontalSpacing(6);
    lay110->setVerticalSpacing(6);
    tools_2sided_group_dd_label = new QLabel(nullptr);
    tools_2sided_group_dd_label->setText(trUi_(QString::fromUtf8("Drill Dia:")));
    tools_2sided_group_dd_label->setToolTip(trUi_(QString::fromUtf8("Diameter of the drill for the alignment holes.")));
    lay110->addWidget(tools_2sided_group_dd_label, 0, 0, 1, 1);
    tools_2sided_group_drill_dia_entry = new FCDoubleSpinner(nullptr);
    tools_2sided_group_drill_dia_entry->setProperty("decimals", 4);
    tools_2sided_group_drill_dia_entry->setProperty("maximum", 10000.0);
    tools_2sided_group_drill_dia_entry->setProperty("singleStep", 0.1);
    tools_2sided_group_drill_dia_entry->setProperty("value", 3.125);
    lay110->addWidget(tools_2sided_group_drill_dia_entry, 0, 1, 1, 1);
    tools_2sided_group_align_ax_label = new QLabel(nullptr);
    tools_2sided_group_align_ax_label->setText(trUi_(QString::fromUtf8("Align Axis:")));
    tools_2sided_group_align_ax_label->setToolTip(trUi_(QString::fromUtf8("Mirror vertically (X) or horizontally (Y).")));
    lay110->addWidget(tools_2sided_group_align_ax_label, 1, 0, 1, 1);
    tools_2sided_group_align_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}}, Qt::Horizontal, true, nullptr);
    tools_2sided_group_align_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay110->addWidget(tools_2sided_group_align_axis_radio, 1, 1, 1, 1);
    auto* w111 = new QFrame(nullptr);
    w111->setProperty("frameShadow", 48);
    w111->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w111->setSizePolicy(sp); }
    lay110->addWidget(w111, 2, 0, 1, 2);
    tools_2sided_group_mirax_label = new QLabel(nullptr);
    tools_2sided_group_mirax_label->setText(trUi_(QString::fromUtf8("Mirror Axis:")));
    tools_2sided_group_mirax_label->setToolTip(trUi_(QString::fromUtf8("Mirror vertically (X) or horizontally (Y).")));
    lay110->addWidget(tools_2sided_group_mirax_label, 3, 0, 1, 1);
    tools_2sided_group_mirror_axis_radio = new RadioSet({{trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("X"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("Y"))}}, Qt::Horizontal, true, nullptr);
    tools_2sided_group_mirror_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay110->addWidget(tools_2sided_group_mirror_axis_radio, 3, 1, 1, 1);
    tools_2sided_group_axloc_label = new QLabel(nullptr);
    tools_2sided_group_axloc_label->setText(trUi_(QString::fromUtf8("Axis Ref:")));
    tools_2sided_group_axloc_label->setToolTip(trUi_(QString::fromUtf8("The coordinates used as reference for the mirror operation.\nCan be:\n- Point -> a set of coordinates (x,y) around which the object is mirrored\n- Box -> a set of coordinates (x, y) obtained from the center of the\nbounding box of another object selected below\n- Hole Snap-> a point defined by the center of a drill hone in a Excellon object")));
    lay110->addWidget(tools_2sided_group_axloc_label, 4, 0, 1, 1);
    tools_2sided_group_axis_location_radio = new RadioSet({{trUi_(QString::fromUtf8("Point")), QVariant(QString::fromUtf8("point"))}, {trUi_(QString::fromUtf8("Box")), QVariant(QString::fromUtf8("box"))}, {trUi_(QString::fromUtf8("Hole Snap")), QVariant(QString::fromUtf8("hole"))}}, Qt::Horizontal, true, nullptr);
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
    tools_cutout_group_board_cutout_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_cutout_group_board_cutout_label->setToolTip(trUi_(QString::fromUtf8("Create toolpaths to cut around\nthe PCB and separate it from\nthe original board.")));
    tools_cutout_group_layout->addWidget(tools_cutout_group_board_cutout_label, 0);
    auto* lay112 = new QGridLayout();
    lay112->setContentsMargins(0, 0, 0, 0);
    lay112->setHorizontalSpacing(6);
    lay112->setVerticalSpacing(6);
    auto* w113 = new FCLabel(nullptr);
    w113->setText(trUi_(QString::fromUtf8("Tool Diameter:")));
    w113->setToolTip(trUi_(QString::fromUtf8("Diameter of the tool used to cutout\nthe PCB shape out of the surrounding material.")));
    lay112->addWidget(w113, 0, 0, 1, 1);
    tools_cutout_group_cutout_tooldia_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutout_tooldia_entry->setProperty("decimals", 4);
    tools_cutout_group_cutout_tooldia_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_cutout_tooldia_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutout_tooldia_entry->setProperty("value", 2.4);
    lay112->addWidget(tools_cutout_group_cutout_tooldia_entry, 0, 1, 1, 1);
    auto* w114 = new FCLabel(nullptr);
    w114->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w114->setToolTip(trUi_(QString::fromUtf8("Cutting depth (negative)\nbelow the copper surface.")));
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
    tools_cutout_group_mpass_cb->setText(trUi_(QString::fromUtf8("Multi-Depth:")));
    tools_cutout_group_mpass_cb->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    lay112->addWidget(tools_cutout_group_mpass_cb, 2, 0, 1, 1);
    tools_cutout_group_maxdepth_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_maxdepth_entry->setProperty("decimals", 4);
    tools_cutout_group_maxdepth_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_maxdepth_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_maxdepth_entry->setToolTip(trUi_(QString::fromUtf8("Depth of each pass (positive).")));
    tools_cutout_group_maxdepth_entry->setProperty("value", 0.6);
    lay112->addWidget(tools_cutout_group_maxdepth_entry, 2, 1, 1, 1);
    auto* w115 = new FCLabel(nullptr);
    w115->setText(trUi_(QString::fromUtf8("Kind:")));
    w115->setToolTip(trUi_(QString::fromUtf8("Choice of what kind the object we want to cutout is.\n- Single: contain a single PCB Gerber outline object.\n- Panel: a panel PCB Gerber object, which is made\nout of many individual PCB outlines.")));
    lay112->addWidget(w115, 3, 0, 1, 1);
    tools_cutout_group_obj_kind_combo = new RadioSet({{trUi_(QString::fromUtf8("Single")), QVariant(QString::fromUtf8("single"))}, {trUi_(QString::fromUtf8("Panel")), QVariant(QString::fromUtf8("panel"))}}, Qt::Horizontal, true, nullptr);
    tools_cutout_group_obj_kind_combo->setValue(QVariant(QString::fromUtf8("single")));
    lay112->addWidget(tools_cutout_group_obj_kind_combo, 3, 1, 1, 1);
    auto* w116 = new FCLabel(nullptr);
    w116->setText(trUi_(QString::fromUtf8("Margin:")));
    w116->setToolTip(trUi_(QString::fromUtf8("Margin over bounds. A positive value here\nwill make the cutout of the PCB further from\nthe actual PCB border")));
    lay112->addWidget(w116, 4, 0, 1, 1);
    tools_cutout_group_cutout_margin_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutout_margin_entry->setProperty("decimals", 4);
    tools_cutout_group_cutout_margin_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_cutout_margin_entry->setProperty("minimum", -10000.0);
    tools_cutout_group_cutout_margin_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutout_margin_entry->setProperty("value", 0.1);
    lay112->addWidget(tools_cutout_group_cutout_margin_entry, 4, 1, 1, 1);
    auto* w117 = new FCLabel(nullptr);
    w117->setText(trUi_(QString::fromUtf8("Gap size:")));
    w117->setToolTip(trUi_(QString::fromUtf8("The size of the bridge gaps in the cutout\nused to keep the board connected to\nthe surrounding material (the one \nfrom which the PCB is cutout).")));
    lay112->addWidget(w117, 5, 0, 1, 1);
    tools_cutout_group_cutout_gap_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_cutout_gap_entry->setProperty("decimals", 4);
    tools_cutout_group_cutout_gap_entry->setProperty("maximum", 10000.0);
    tools_cutout_group_cutout_gap_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_cutout_gap_entry->setProperty("value", 4.0);
    lay112->addWidget(tools_cutout_group_cutout_gap_entry, 5, 1, 1, 1);
    tools_cutout_group_gaptype_label = new FCLabel(nullptr);
    tools_cutout_group_gaptype_label->setText(trUi_(QString::fromUtf8("Gap type:")));
    tools_cutout_group_gaptype_label->setToolTip(trUi_(QString::fromUtf8("The type of gap:\n- Bridge -> the cutout will be interrupted by bridges\n- Thin -> same as 'bridge' but it will be thinner by partially milling the gap\n- M-Bites -> 'Mouse Bites' - same as 'bridge' but covered with drill holes")));
    lay112->addWidget(tools_cutout_group_gaptype_label, 7, 0, 1, 1);
    tools_cutout_group_gaptype_radio = new RadioSet({{trUi_(QString::fromUtf8("Bridge")), QVariant(QString::fromUtf8("b"))}, {trUi_(QString::fromUtf8("Thin")), QVariant(QString::fromUtf8("bt"))}, {trUi_(QString::fromUtf8("M-Bites")), QVariant(QString::fromUtf8("mb"))}}, Qt::Horizontal, true, nullptr);
    tools_cutout_group_gaptype_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay112->addWidget(tools_cutout_group_gaptype_radio, 7, 1, 1, 1);
    tools_cutout_group_thin_depth_label = new FCLabel(nullptr);
    tools_cutout_group_thin_depth_label->setText(trUi_(QString::fromUtf8("Depth:")));
    tools_cutout_group_thin_depth_label->setToolTip(trUi_(QString::fromUtf8("The depth until the milling is done\nin order to thin the gaps.")));
    lay112->addWidget(tools_cutout_group_thin_depth_label, 9, 0, 1, 1);
    tools_cutout_group_thin_depth_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_thin_depth_entry->setProperty("decimals", 4);
    tools_cutout_group_thin_depth_entry->setProperty("maximum", -0.0);
    tools_cutout_group_thin_depth_entry->setProperty("minimum", -10000.0);
    tools_cutout_group_thin_depth_entry->setProperty("singleStep", 0.1);
    tools_cutout_group_thin_depth_entry->setProperty("value", -1.0);
    lay112->addWidget(tools_cutout_group_thin_depth_entry, 9, 1, 1, 1);
    tools_cutout_group_mb_dia_label = new FCLabel(nullptr);
    tools_cutout_group_mb_dia_label->setText(trUi_(QString::fromUtf8("Tool Diameter:")));
    tools_cutout_group_mb_dia_label->setToolTip(trUi_(QString::fromUtf8("The drill hole diameter when doing mouse bites.")));
    lay112->addWidget(tools_cutout_group_mb_dia_label, 11, 0, 1, 1);
    tools_cutout_group_mb_dia_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_mb_dia_entry->setProperty("decimals", 4);
    tools_cutout_group_mb_dia_entry->setProperty("maximum", 100.0);
    tools_cutout_group_mb_dia_entry->setProperty("value", 0.6);
    lay112->addWidget(tools_cutout_group_mb_dia_entry, 11, 1, 1, 1);
    tools_cutout_group_mb_spacing_label = new FCLabel(nullptr);
    tools_cutout_group_mb_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    tools_cutout_group_mb_spacing_label->setToolTip(trUi_(QString::fromUtf8("The spacing between drill holes when doing mouse bites.")));
    lay112->addWidget(tools_cutout_group_mb_spacing_label, 13, 0, 1, 1);
    tools_cutout_group_mb_spacing_entry = new FCDoubleSpinner(nullptr);
    tools_cutout_group_mb_spacing_entry->setProperty("decimals", 4);
    tools_cutout_group_mb_spacing_entry->setProperty("maximum", 100.0);
    tools_cutout_group_mb_spacing_entry->setProperty("value", 0.3);
    lay112->addWidget(tools_cutout_group_mb_spacing_entry, 13, 1, 1, 1);
    auto* w118 = new FCLabel(nullptr);
    w118->setText(trUi_(QString::fromUtf8("Gaps:")));
    w118->setToolTip(trUi_(QString::fromUtf8("Number of gaps used for the cutout.\nThere can be maximum 8 bridges/gaps.\nThe choices are:\n- None  - no gaps\n- lr    - left + right\n- tb    - top + bottom\n- 4     - left + right +top + bottom\n- 2lr   - 2*left + 2*right\n- 2tb  - 2*top + 2*bottom\n- 8     - 2*left + 2*right +2*top + 2*bottom")));
    lay112->addWidget(w118, 15, 0, 1, 1);
    tools_cutout_group_gaps_combo = new FCComboBox(nullptr);
    tools_cutout_group_gaps_combo->setProperty("currentText", trUi_(QString::fromUtf8("4")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("None")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("LR")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("TB")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("4")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("2LR")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("2TB")));
    tools_cutout_group_gaps_combo->addItem(trUi_(QString::fromUtf8("8")));
    tools_cutout_group_gaps_combo->setCurrentIndex(3);
    lay112->addWidget(tools_cutout_group_gaps_combo, 15, 1, 1, 1);
    tools_cutout_group_convex_box = new FCCheckBox(nullptr);
    tools_cutout_group_convex_box->setText(trUi_(QString::fromUtf8("Convex Shape")));
    tools_cutout_group_convex_box->setToolTip(trUi_(QString::fromUtf8("Create a convex shape surrounding the entire PCB.\nUsed only if the source object type is Gerber.")));
    lay112->addWidget(tools_cutout_group_convex_box, 17, 0, 1, 2);
    tools_cutout_group_big_cursor_cb = new FCCheckBox(nullptr);
    tools_cutout_group_big_cursor_cb->setChecked(true);
    tools_cutout_group_big_cursor_cb->setText(trUi_(QString::fromUtf8("Big cursor")));
    tools_cutout_group_big_cursor_cb->setToolTip(trUi_(QString::fromUtf8("Use a big cursor when adding manual gaps.")));
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
    tools_drill_group_drill_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_drill_group_drill_label->setToolTip(trUi_(QString::fromUtf8("Create CNCJob with toolpaths for drilling or milling holes.")));
    tools_drill_group_layout->addWidget(tools_drill_group_drill_label, 0);
    auto* lay119 = new QGridLayout();
    lay119->setContentsMargins(0, 0, 0, 0);
    lay119->setHorizontalSpacing(6);
    lay119->setVerticalSpacing(6);
    tools_drill_group_order_label = new FCLabel(nullptr);
    tools_drill_group_order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    tools_drill_group_order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay119->addWidget(tools_drill_group_order_label, 1, 0, 1, 1);
    tools_drill_group_order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_order_radio->setValue(QVariant(QString::fromUtf8("no")));
    lay119->addWidget(tools_drill_group_order_radio, 1, 1, 1, 2);
    auto* w120 = new FCLabel(nullptr);
    w120->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w120->setToolTip(trUi_(QString::fromUtf8("Drill depth (negative)\nbelow the copper surface.")));
    lay119->addWidget(w120, 3, 0, 1, 1);
    tools_drill_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_cutz_entry->setProperty("decimals", 4);
    tools_drill_group_cutz_entry->setProperty("maximum", 0.0);
    tools_drill_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_drill_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_drill_group_cutz_entry->setProperty("value", -1.7);
    lay119->addWidget(tools_drill_group_cutz_entry, 3, 1, 1, 2);
    tools_drill_group_mpass_cb = new FCCheckBox(nullptr);
    tools_drill_group_mpass_cb->setText(trUi_(QString::fromUtf8("Multi-Depth:")));
    tools_drill_group_mpass_cb->setToolTip(trUi_(QString::fromUtf8("Use multiple passes to limit\nthe cut depth in each pass. Will\ncut multiple times until Cut Z is\nreached.")));
    lay119->addWidget(tools_drill_group_mpass_cb, 4, 0, 1, 1);
    tools_drill_group_maxdepth_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_maxdepth_entry->setProperty("decimals", 4);
    tools_drill_group_maxdepth_entry->setProperty("maximum", 10000.0);
    tools_drill_group_maxdepth_entry->setProperty("singleStep", 0.1);
    tools_drill_group_maxdepth_entry->setToolTip(trUi_(QString::fromUtf8("Depth of each pass (positive).")));
    tools_drill_group_maxdepth_entry->setProperty("value", 0.7);
    lay119->addWidget(tools_drill_group_maxdepth_entry, 4, 1, 1, 2);
    auto* w121 = new FCLabel(nullptr);
    w121->setText(trUi_(QString::fromUtf8("Travel Z:")));
    w121->setToolTip(trUi_(QString::fromUtf8("Tool height when travelling\nacross the XY plane.")));
    lay119->addWidget(w121, 5, 0, 1, 1);
    tools_drill_group_travelz_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_travelz_entry->setProperty("decimals", 4);
    tools_drill_group_travelz_entry->setProperty("maximum", 10000.0);
    tools_drill_group_travelz_entry->setProperty("minimum", 0.0001);
    tools_drill_group_travelz_entry->setProperty("value", 2.0);
    lay119->addWidget(tools_drill_group_travelz_entry, 5, 1, 1, 2);
    tools_drill_group_toolchange_cb = new FCCheckBox(nullptr);
    tools_drill_group_toolchange_cb->setText(trUi_(QString::fromUtf8("Tool change")));
    tools_drill_group_toolchange_cb->setToolTip(trUi_(QString::fromUtf8("Include tool-change sequence\nin G-Code (Pause for tool change).")));
    lay119->addWidget(tools_drill_group_toolchange_cb, 6, 0, 1, 3);
    auto* w122 = new FCLabel(nullptr);
    w122->setText(trUi_(QString::fromUtf8("Toolchange Z:")));
    w122->setToolTip(trUi_(QString::fromUtf8("Z-axis position (height) for\ntool change.")));
    lay119->addWidget(w122, 7, 0, 1, 1);
    tools_drill_group_toolchangez_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_toolchangez_entry->setProperty("decimals", 4);
    tools_drill_group_toolchangez_entry->setProperty("maximum", 10000.0);
    tools_drill_group_toolchangez_entry->setProperty("minimum", 0.0001);
    tools_drill_group_toolchangez_entry->setProperty("value", 15.0);
    lay119->addWidget(tools_drill_group_toolchangez_entry, 7, 1, 1, 2);
    auto* w123 = new FCLabel(nullptr);
    w123->setText(trUi_(QString::fromUtf8("End move Z:")));
    w123->setToolTip(trUi_(QString::fromUtf8("Height of the tool after\nthe last move at the end of the job.")));
    lay119->addWidget(w123, 8, 0, 1, 1);
    tools_drill_group_endz_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_endz_entry->setProperty("decimals", 4);
    tools_drill_group_endz_entry->setProperty("maximum", 10000.0);
    tools_drill_group_endz_entry->setProperty("value", 0.5);
    lay119->addWidget(tools_drill_group_endz_entry, 8, 1, 1, 2);
    auto* w124 = new FCLabel(nullptr);
    w124->setText(trUi_(QString::fromUtf8("End move X,Y:")));
    w124->setToolTip(trUi_(QString::fromUtf8("End move X,Y position. In format (x,y).\nIf no value is entered then there is no move\non X,Y plane at the end of the job.")));
    lay119->addWidget(w124, 9, 0, 1, 1);
    tools_drill_group_endxy_entry = new NumericalEvalTupleEntry(nullptr);
    tools_drill_group_endxy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_drill_group_endxy_entry->setText(trUi_(QString::fromUtf8("None")));
    lay119->addWidget(tools_drill_group_endxy_entry, 9, 1, 1, 2);
    auto* w125 = new FCLabel(nullptr);
    w125->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    w125->setToolTip(trUi_(QString::fromUtf8("Tool speed while drilling\n(in units per minute).\nSo called 'Plunge' feedrate.\nThis is for linear move G01.")));
    lay119->addWidget(w125, 10, 0, 1, 1);
    tools_drill_group_feedrate_z_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_feedrate_z_entry->setProperty("decimals", 4);
    tools_drill_group_feedrate_z_entry->setProperty("maximum", 910000.0);
    tools_drill_group_feedrate_z_entry->setProperty("value", 300.0);
    lay119->addWidget(tools_drill_group_feedrate_z_entry, 10, 1, 1, 2);
    auto* w126 = new FCLabel(nullptr);
    w126->setText(trUi_(QString::fromUtf8("Spindle Speed:")));
    w126->setToolTip(trUi_(QString::fromUtf8("Speed of the spindle\nin RPM (optional)")));
    lay119->addWidget(w126, 11, 0, 1, 1);
    tools_drill_group_spindlespeed_entry = new FCSpinner(nullptr);
    tools_drill_group_spindlespeed_entry->setProperty("maximum", 1000000);
    tools_drill_group_spindlespeed_entry->setProperty("singleStep", 100);
    lay119->addWidget(tools_drill_group_spindlespeed_entry, 11, 1, 1, 2);
    tools_drill_group_dwell_cb = new FCCheckBox(nullptr);
    tools_drill_group_dwell_cb->setText(trUi_(QString::fromUtf8("Enable Dwell")));
    tools_drill_group_dwell_cb->setToolTip(trUi_(QString::fromUtf8("Pause to allow the spindle to reach its\nspeed before cutting.")));
    lay119->addWidget(tools_drill_group_dwell_cb, 12, 0, 1, 3);
    auto* w127 = new FCLabel(nullptr);
    w127->setText(trUi_(QString::fromUtf8("Duration:")));
    w127->setToolTip(trUi_(QString::fromUtf8("Number of time units for spindle to dwell.")));
    lay119->addWidget(w127, 13, 0, 1, 1);
    tools_drill_group_dwelltime_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_dwelltime_entry->setProperty("decimals", 4);
    tools_drill_group_dwelltime_entry->setEnabled(false);
    tools_drill_group_dwelltime_entry->setProperty("maximum", 910000.0);
    tools_drill_group_dwelltime_entry->setProperty("value", 1.0);
    lay119->addWidget(tools_drill_group_dwelltime_entry, 13, 1, 1, 2);
    auto* w128 = new FCLabel(nullptr);
    w128->setText(trUi_(QString::fromUtf8("Preprocessor:")));
    w128->setToolTip(trUi_(QString::fromUtf8("The preprocessor JSON file that dictates\nGcode output.")));
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
    tools_drill_group_dslots_label->setText(trUi_(QString::fromUtf8("<b>Drilling Slots:</b>")));
    lay119->addWidget(tools_drill_group_dslots_label, 18, 0, 1, 3);
    tools_drill_group_drill_slots_cb = new FCCheckBox(nullptr);
    tools_drill_group_drill_slots_cb->setText(trUi_(QString::fromUtf8("Drill slots")));
    tools_drill_group_drill_slots_cb->setToolTip(trUi_(QString::fromUtf8("If the selected tool has slots then they will be drilled.")));
    lay119->addWidget(tools_drill_group_drill_slots_cb, 20, 0, 1, 3);
    tools_drill_group_drill_overlap_label = new FCLabel(nullptr);
    tools_drill_group_drill_overlap_label->setText(trUi_(QString::fromUtf8("Overlap:")));
    tools_drill_group_drill_overlap_label->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool diameter to overlap previous drill hole.")));
    lay119->addWidget(tools_drill_group_drill_overlap_label, 22, 0, 1, 1);
    tools_drill_group_drill_overlap_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_drill_overlap_entry->setProperty("decimals", 4);
    tools_drill_group_drill_overlap_entry->setProperty("maximum", 10000.0);
    tools_drill_group_drill_overlap_entry->setProperty("singleStep", 0.1);
    lay119->addWidget(tools_drill_group_drill_overlap_entry, 22, 1, 1, 2);
    tools_drill_group_last_drill_cb = new FCCheckBox(nullptr);
    tools_drill_group_last_drill_cb->setChecked(true);
    tools_drill_group_last_drill_cb->setText(trUi_(QString::fromUtf8("Last drill")));
    tools_drill_group_last_drill_cb->setToolTip(trUi_(QString::fromUtf8("If the slot length is not completely covered by drill holes,\nadd a drill hole on the slot end point.")));
    lay119->addWidget(tools_drill_group_last_drill_cb, 24, 0, 1, 3);
    auto* w130 = new QFrame(nullptr);
    w130->setProperty("frameShadow", 48);
    w130->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w130->setSizePolicy(sp); }
    lay119->addWidget(w130, 26, 0, 1, 3);
    tools_drill_group_exc_label = new FCLabel(nullptr);
    tools_drill_group_exc_label->setText(trUi_(QString::fromUtf8("<b>Advanced Options:</b>")));
    tools_drill_group_exc_label->setToolTip(trUi_(QString::fromUtf8("A list of advanced parameters.")));
    lay119->addWidget(tools_drill_group_exc_label, 28, 0, 1, 3);
    auto* w131 = new FCLabel(nullptr);
    w131->setText(trUi_(QString::fromUtf8("Offset Z:")));
    w131->setToolTip(trUi_(QString::fromUtf8("Some drill bits (the larger ones) need to drill deeper\nto create the desired exit hole diameter due of the tip shape.\nThe value here can compensate the Cut Z parameter.")));
    lay119->addWidget(w131, 29, 0, 1, 1);
    tools_drill_group_offset_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_offset_entry->setProperty("decimals", 4);
    tools_drill_group_offset_entry->setProperty("maximum", 999.9999);
    tools_drill_group_offset_entry->setProperty("minimum", -999.9999);
    lay119->addWidget(tools_drill_group_offset_entry, 29, 1, 1, 2);
    auto* w132 = new FCLabel(nullptr);
    w132->setText(trUi_(QString::fromUtf8("Toolchange X,Y:")));
    w132->setToolTip(trUi_(QString::fromUtf8("Toolchange X,Y position.")));
    lay119->addWidget(w132, 31, 0, 1, 1);
    tools_drill_group_toolchangexy_entry = new NumericalEvalTupleEntry(nullptr);
    tools_drill_group_toolchangexy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_drill_group_toolchangexy_entry->setText(trUi_(QString::fromUtf8("0.0, 0.0")));
    lay119->addWidget(tools_drill_group_toolchangexy_entry, 31, 1, 1, 2);
    auto* w133 = new FCLabel(nullptr);
    w133->setText(trUi_(QString::fromUtf8("Start Z:")));
    w133->setToolTip(trUi_(QString::fromUtf8("Height of the tool just after starting the work.\nDelete the value if you don't need this feature.")));
    lay119->addWidget(w133, 33, 0, 1, 1);
    tools_drill_group_estartz_entry = new NumericalEvalEntry(nullptr);
    tools_drill_group_estartz_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay119->addWidget(tools_drill_group_estartz_entry, 33, 1, 1, 2);
    auto* w134 = new FCLabel(nullptr);
    w134->setText(trUi_(QString::fromUtf8("Feedrate Rapids:")));
    w134->setToolTip(trUi_(QString::fromUtf8("Tool speed while drilling\n(in units per minute).\nThis is for the rapid move G00.\nIt is useful only for Marlin,\nignore for any other cases.")));
    lay119->addWidget(w134, 35, 0, 1, 1);
    tools_drill_group_feedrate_rapid_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_feedrate_rapid_entry->setProperty("decimals", 4);
    tools_drill_group_feedrate_rapid_entry->setProperty("maximum", 910000.0);
    tools_drill_group_feedrate_rapid_entry->setProperty("value", 1500.0);
    lay119->addWidget(tools_drill_group_feedrate_rapid_entry, 35, 1, 1, 2);
    tools_drill_group_pdepth_label = new FCLabel(nullptr);
    tools_drill_group_pdepth_label->setText(trUi_(QString::fromUtf8("Probe Z depth:")));
    tools_drill_group_pdepth_label->setToolTip(trUi_(QString::fromUtf8("The maximum depth that the probe is allowed\nto probe. Negative value, in current units.")));
    lay119->addWidget(tools_drill_group_pdepth_label, 37, 0, 1, 1);
    tools_drill_group_pdepth_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_pdepth_entry->setProperty("decimals", 4);
    tools_drill_group_pdepth_entry->setProperty("maximum", 0.0);
    tools_drill_group_pdepth_entry->setProperty("minimum", -910000.0);
    tools_drill_group_pdepth_entry->setProperty("value", -0.02);
    lay119->addWidget(tools_drill_group_pdepth_entry, 37, 1, 1, 2);
    tools_drill_group_feedrate_probe_label = new FCLabel(nullptr);
    tools_drill_group_feedrate_probe_label->setText(trUi_(QString::fromUtf8("Feedrate Probe:")));
    tools_drill_group_feedrate_probe_label->setToolTip(trUi_(QString::fromUtf8("The feedrate used while the probe is probing.")));
    lay119->addWidget(tools_drill_group_feedrate_probe_label, 38, 0, 1, 1);
    tools_drill_group_feedrate_probe_entry = new FCDoubleSpinner(nullptr);
    tools_drill_group_feedrate_probe_entry->setProperty("decimals", 4);
    tools_drill_group_feedrate_probe_entry->setProperty("maximum", 910000.0);
    tools_drill_group_feedrate_probe_entry->setProperty("value", 75.0);
    lay119->addWidget(tools_drill_group_feedrate_probe_entry, 38, 1, 1, 2);
    auto* w135 = new FCLabel(nullptr);
    w135->setText(trUi_(QString::fromUtf8("Spindle direction:")));
    w135->setToolTip(trUi_(QString::fromUtf8("This sets the direction that the spindle is rotating.\nIt can be either:\n- CW = clockwise or\n- CCW = counter clockwise")));
    lay119->addWidget(w135, 40, 0, 1, 1);
    tools_drill_group_spindledir_radio = new RadioSet({{trUi_(QString::fromUtf8("CW")), QVariant(QString::fromUtf8("CW"))}, {trUi_(QString::fromUtf8("CCW")), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_spindledir_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay119->addWidget(tools_drill_group_spindledir_radio, 40, 1, 1, 2);
    tools_drill_group_fplunge_cb = new FCCheckBox(nullptr);
    tools_drill_group_fplunge_cb->setText(trUi_(QString::fromUtf8("Fast Plunge")));
    tools_drill_group_fplunge_cb->setToolTip(trUi_(QString::fromUtf8("By checking this, the vertical move from\nZ_Toolchange to Z_move is done with G0,\nmeaning the fastest speed available.\nWARNING: the move is done at Toolchange X,Y coords.")));
    lay119->addWidget(tools_drill_group_fplunge_cb, 42, 0, 1, 3);
    tools_drill_group_fretract_cb = new FCCheckBox(nullptr);
    tools_drill_group_fretract_cb->setText(trUi_(QString::fromUtf8("Fast Retract")));
    tools_drill_group_fretract_cb->setToolTip(trUi_(QString::fromUtf8("Exit hole strategy.\n - When uncheked, while exiting the drilled hole the drill bit\nwill travel slow, with set feedrate (G1), up to zero depth and then\ntravel as fast as possible (G0) to the Z Move (travel height).\n - When checked the travel from Z cut (cut depth) to Z_move\n(travel height) is done as fast as possible (G0) in one move.")));
    lay119->addWidget(tools_drill_group_fretract_cb, 45, 0, 1, 3);
    auto* w136 = new QFrame(nullptr);
    w136->setProperty("frameShadow", 48);
    w136->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w136->setSizePolicy(sp); }
    lay119->addWidget(w136, 46, 0, 1, 3);
    tools_drill_group_area_exc_label = new FCLabel(nullptr);
    tools_drill_group_area_exc_label->setText(trUi_(QString::fromUtf8("<b>Area Exclusion:</b>")));
    tools_drill_group_area_exc_label->setToolTip(trUi_(QString::fromUtf8("Area exclusion parameters.")));
    lay119->addWidget(tools_drill_group_area_exc_label, 47, 0, 1, 2);
    tools_drill_group_exclusion_cb = new FCCheckBox(nullptr);
    tools_drill_group_exclusion_cb->setText(trUi_(QString::fromUtf8("Exclusion areas")));
    tools_drill_group_exclusion_cb->setToolTip(trUi_(QString::fromUtf8("Include exclusion areas.\nIn those areas the travel of the tools\nis forbidden.")));
    lay119->addWidget(tools_drill_group_exclusion_cb, 49, 0, 1, 2);
    tools_drill_group_area_shape_label = new FCLabel(nullptr);
    tools_drill_group_area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    tools_drill_group_area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    lay119->addWidget(tools_drill_group_area_shape_label, 51, 0, 1, 1);
    tools_drill_group_area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("polygon")));
    lay119->addWidget(tools_drill_group_area_shape_radio, 51, 1, 1, 1);
    tools_drill_group_strategy_label = new FCLabel(nullptr);
    tools_drill_group_strategy_label->setText(trUi_(QString::fromUtf8("Strategy:")));
    tools_drill_group_strategy_label->setToolTip(trUi_(QString::fromUtf8("The strategy followed when encountering an exclusion area.\nCan be:\n- Over -> when encountering the area, the tool will go to a set height\n- Around -> will avoid the exclusion area by going around the area")));
    lay119->addWidget(tools_drill_group_strategy_label, 53, 0, 1, 1);
    tools_drill_group_strategy_radio = new RadioSet({{trUi_(QString::fromUtf8("Over")), QVariant(QString::fromUtf8("over"))}, {trUi_(QString::fromUtf8("Around")), QVariant(QString::fromUtf8("around"))}}, Qt::Horizontal, true, nullptr);
    tools_drill_group_strategy_radio->setValue(QVariant(QString::fromUtf8("over")));
    lay119->addWidget(tools_drill_group_strategy_radio, 53, 1, 1, 1);
    tools_drill_group_over_z_label = new FCLabel(nullptr);
    tools_drill_group_over_z_label->setText(trUi_(QString::fromUtf8("Over Z:")));
    tools_drill_group_over_z_label->setToolTip(trUi_(QString::fromUtf8("The height Z to which the tool will rise in order to avoid\nan interdiction area.")));
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
    tools_panelize_group_panelize_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_panelize_group_panelize_label->setToolTip(trUi_(QString::fromUtf8("Create an object that contains an array of (x, y) elements,\neach element is a copy of the source object spaced\nat a X distance, Y distance of each other.")));
    tools_panelize_group_layout->addWidget(tools_panelize_group_panelize_label, 0);
    auto* lay137 = new QGridLayout();
    lay137->setContentsMargins(0, 0, 0, 0);
    lay137->setHorizontalSpacing(6);
    lay137->setVerticalSpacing(6);
    tools_panelize_group_spacing_columns_label = new QLabel(nullptr);
    tools_panelize_group_spacing_columns_label->setText(trUi_(QString::fromUtf8("Spacing cols:")));
    tools_panelize_group_spacing_columns_label->setToolTip(trUi_(QString::fromUtf8("Spacing between columns of the desired panel.\nIn current units.")));
    lay137->addWidget(tools_panelize_group_spacing_columns_label, 0, 0, 1, 1);
    tools_panelize_group_pspacing_columns = new FCDoubleSpinner(nullptr);
    tools_panelize_group_pspacing_columns->setProperty("decimals", 4);
    tools_panelize_group_pspacing_columns->setProperty("maximum", 10000.0);
    tools_panelize_group_pspacing_columns->setProperty("singleStep", 0.1);
    lay137->addWidget(tools_panelize_group_pspacing_columns, 0, 1, 1, 1);
    tools_panelize_group_spacing_rows_label = new QLabel(nullptr);
    tools_panelize_group_spacing_rows_label->setText(trUi_(QString::fromUtf8("Spacing rows:")));
    tools_panelize_group_spacing_rows_label->setToolTip(trUi_(QString::fromUtf8("Spacing between rows of the desired panel.\nIn current units.")));
    lay137->addWidget(tools_panelize_group_spacing_rows_label, 1, 0, 1, 1);
    tools_panelize_group_pspacing_rows = new FCDoubleSpinner(nullptr);
    tools_panelize_group_pspacing_rows->setProperty("decimals", 4);
    tools_panelize_group_pspacing_rows->setProperty("maximum", 10000.0);
    tools_panelize_group_pspacing_rows->setProperty("singleStep", 0.1);
    lay137->addWidget(tools_panelize_group_pspacing_rows, 1, 1, 1, 1);
    tools_panelize_group_columns_label = new QLabel(nullptr);
    tools_panelize_group_columns_label->setText(trUi_(QString::fromUtf8("Columns:")));
    tools_panelize_group_columns_label->setToolTip(trUi_(QString::fromUtf8("Number of columns of the desired panel")));
    lay137->addWidget(tools_panelize_group_columns_label, 2, 0, 1, 1);
    tools_panelize_group_pcolumns = new FCSpinner(nullptr);
    tools_panelize_group_pcolumns->setProperty("maximum", 1000);
    tools_panelize_group_pcolumns->setProperty("minimum", 1);
    tools_panelize_group_pcolumns->setProperty("value", 1);
    lay137->addWidget(tools_panelize_group_pcolumns, 2, 1, 1, 1);
    tools_panelize_group_rows_label = new QLabel(nullptr);
    tools_panelize_group_rows_label->setText(trUi_(QString::fromUtf8("Rows:")));
    tools_panelize_group_rows_label->setToolTip(trUi_(QString::fromUtf8("Number of rows of the desired panel")));
    lay137->addWidget(tools_panelize_group_rows_label, 3, 0, 1, 1);
    tools_panelize_group_prows = new FCSpinner(nullptr);
    tools_panelize_group_prows->setProperty("maximum", 1000);
    tools_panelize_group_prows->setProperty("minimum", 1);
    tools_panelize_group_prows->setProperty("value", 1);
    lay137->addWidget(tools_panelize_group_prows, 3, 1, 1, 1);
    tools_panelize_group_panel_type_label = new QLabel(nullptr);
    tools_panelize_group_panel_type_label->setText(trUi_(QString::fromUtf8("Panel Type:")));
    tools_panelize_group_panel_type_label->setToolTip(trUi_(QString::fromUtf8("Choose the type of object for the panel object:\n- Gerber\n- Geometry")));
    lay137->addWidget(tools_panelize_group_panel_type_label, 4, 0, 1, 1);
    tools_panelize_group_panel_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("gerber"))}, {trUi_(QString::fromUtf8("Geo")), QVariant(QString::fromUtf8("geometry"))}}, Qt::Horizontal, true, nullptr);
    tools_panelize_group_panel_type_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    lay137->addWidget(tools_panelize_group_panel_type_radio, 4, 1, 1, 1);
    tools_panelize_group_poptimization_cb = new FCCheckBox(nullptr);
    tools_panelize_group_poptimization_cb->setChecked(true);
    tools_panelize_group_poptimization_cb->setText(trUi_(QString::fromUtf8("Path Optimization")));
    tools_panelize_group_poptimization_cb->setToolTip(trUi_(QString::fromUtf8("Active only for Geometry panel type.\nWhen checked the application will find\nany two overlapping Line elements in the panel\nand will remove the overlapping parts, keeping only one of them.")));
    lay137->addWidget(tools_panelize_group_poptimization_cb, 5, 0, 1, 2);
    tools_panelize_group_pconstrain_cb = new FCCheckBox(nullptr);
    tools_panelize_group_pconstrain_cb->setText(trUi_(QString::fromUtf8("Constrain within:")));
    tools_panelize_group_pconstrain_cb->setToolTip(trUi_(QString::fromUtf8("Area define by DX and DY within to constrain the panel.\nDX and DY values are in current units.\nRegardless of how many columns and rows are desired,\nthe final panel will have as many columns and rows as\nthey fit completely within selected area.")));
    lay137->addWidget(tools_panelize_group_pconstrain_cb, 10, 0, 1, 2);
    tools_panelize_group_x_width_lbl = new QLabel(nullptr);
    tools_panelize_group_x_width_lbl->setText(trUi_(QString::fromUtf8("Width (DX):")));
    tools_panelize_group_x_width_lbl->setToolTip(trUi_(QString::fromUtf8("The width (DX) within which the panel must fit.\nIn current units.")));
    lay137->addWidget(tools_panelize_group_x_width_lbl, 12, 0, 1, 1);
    tools_panelize_group_px_width_entry = new FCDoubleSpinner(nullptr);
    tools_panelize_group_px_width_entry->setProperty("decimals", 4);
    tools_panelize_group_px_width_entry->setProperty("maximum", 10000.0);
    tools_panelize_group_px_width_entry->setProperty("singleStep", 0.1);
    tools_panelize_group_px_width_entry->setProperty("value", 200.0);
    lay137->addWidget(tools_panelize_group_px_width_entry, 12, 1, 1, 1);
    tools_panelize_group_y_height_lbl = new QLabel(nullptr);
    tools_panelize_group_y_height_lbl->setText(trUi_(QString::fromUtf8("Height (DY):")));
    tools_panelize_group_y_height_lbl->setToolTip(trUi_(QString::fromUtf8("The height (DY)within which the panel must fit.\nIn current units.")));
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
    tools_ncc_group_clearcopper_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_ncc_group_clearcopper_label->setToolTip(trUi_(QString::fromUtf8("Create a Geometry object with\ntoolpaths to cut all non-copper regions.")));
    tools_ncc_group_layout->addWidget(tools_ncc_group_clearcopper_label, 0);
    auto* lay138 = new QGridLayout();
    lay138->setContentsMargins(0, 0, 0, 0);
    lay138->setHorizontalSpacing(6);
    lay138->setVerticalSpacing(6);
    auto* w139 = new QLabel(nullptr);
    w139->setText(trUi_(QString::fromUtf8("<b><font color=\"green\">Tools Dia:</font></b>")));
    w139->setToolTip(trUi_(QString::fromUtf8("Diameters of the tools, separated by comma.\nThe value of the diameter has to use the dot decimals separator.\nValid values: 0.3, 1.0")));
    lay138->addWidget(w139, 0, 0, 1, 1);
    tools_ncc_group_ncc_tool_dia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_ncc_group_ncc_tool_dia_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Comma separated values")));
    tools_ncc_group_ncc_tool_dia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_ncc_group_ncc_tool_dia_entry->setText(trUi_(QString::fromUtf8("1.0, 0.5")));
    lay138->addWidget(tools_ncc_group_ncc_tool_dia_entry, 0, 1, 1, 1);
    tools_ncc_group_tool_type_label = new QLabel(nullptr);
    tools_ncc_group_tool_type_label->setText(trUi_(QString::fromUtf8("Tool Type:")));
    tools_ncc_group_tool_type_label->setToolTip(trUi_(QString::fromUtf8("Default tool type:\n- 'V-shape'\n- Circular")));
    lay138->addWidget(tools_ncc_group_tool_type_label, 1, 0, 1, 1);
    tools_ncc_group_tool_type_radio = new RadioSet({{trUi_(QString::fromUtf8("V-shape")), QVariant(QString::fromUtf8("V"))}, {trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("C1"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_tool_type_radio->setToolTip(trUi_(QString::fromUtf8("Default tool type:\n- 'V-shape'\n- Circular")));
    tools_ncc_group_tool_type_radio->setValue(QVariant(QString::fromUtf8("C1")));
    lay138->addWidget(tools_ncc_group_tool_type_radio, 1, 1, 1, 1);
    tools_ncc_group_tipdialabel = new QLabel(nullptr);
    tools_ncc_group_tipdialabel->setText(trUi_(QString::fromUtf8("V-Tip Dia:")));
    tools_ncc_group_tipdialabel->setToolTip(trUi_(QString::fromUtf8("The tip diameter for V-Shape Tool")));
    lay138->addWidget(tools_ncc_group_tipdialabel, 2, 0, 1, 1);
    tools_ncc_group_tipdia_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_tipdia_entry->setProperty("decimals", 4);
    tools_ncc_group_tipdia_entry->setProperty("maximum", 1000.0);
    tools_ncc_group_tipdia_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_tipdia_entry->setProperty("value", 0.1);
    lay138->addWidget(tools_ncc_group_tipdia_entry, 2, 1, 1, 1);
    tools_ncc_group_tipanglelabel = new QLabel(nullptr);
    tools_ncc_group_tipanglelabel->setText(trUi_(QString::fromUtf8("V-Tip Angle:")));
    tools_ncc_group_tipanglelabel->setToolTip(trUi_(QString::fromUtf8("The tip angle for V-Shape Tool.\nIn degree.")));
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
    w140->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w140->setToolTip(trUi_(QString::fromUtf8("Depth of cut into material. Negative value.\nIn application units.")));
    lay138->addWidget(w140, 4, 0, 1, 1);
    tools_ncc_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_cutz_entry->setProperty("decimals", 4);
    tools_ncc_group_cutz_entry->setProperty("maximum", 0.0);
    tools_ncc_group_cutz_entry->setProperty("minimum", -10000.0);
    tools_ncc_group_cutz_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_cutz_entry->setToolTip(trUi_(QString::fromUtf8("Depth of cut into material. Negative value.\nIn application units.")));
    tools_ncc_group_cutz_entry->setProperty("value", -0.05);
    lay138->addWidget(tools_ncc_group_cutz_entry, 4, 1, 1, 1);
    tools_ncc_group_newdialabel = new QLabel(nullptr);
    tools_ncc_group_newdialabel->setText(trUi_(QString::fromUtf8("New Dia:")));
    tools_ncc_group_newdialabel->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool to add in the Tool Table.\nIf the tool is V-shape type then this value is automatically\ncalculated from the other parameters.")));
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
    tools_ncc_group_milling_type_label->setText(trUi_(QString::fromUtf8("Milling Type:")));
    tools_ncc_group_milling_type_label->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    lay138->addWidget(tools_ncc_group_milling_type_label, 7, 0, 1, 1);
    tools_ncc_group_milling_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Climb")), QVariant(QString::fromUtf8("cl"))}, {trUi_(QString::fromUtf8("Conventional")), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_milling_type_radio->setToolTip(trUi_(QString::fromUtf8("Milling type:\n- climb / best for precision milling and to reduce tool usage\n- conventional / useful when there is no backlash compensation")));
    tools_ncc_group_milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    lay138->addWidget(tools_ncc_group_milling_type_radio, 7, 1, 1, 1);
    tools_ncc_group_ncc_order_label = new QLabel(nullptr);
    tools_ncc_group_ncc_order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    tools_ncc_group_ncc_order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay138->addWidget(tools_ncc_group_ncc_order_label, 8, 0, 1, 1);
    tools_ncc_group_ncc_order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_ncc_order_radio->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    tools_ncc_group_ncc_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay138->addWidget(tools_ncc_group_ncc_order_radio, 8, 1, 1, 1);
    auto* w142 = new QFrame(nullptr);
    w142->setProperty("frameShadow", 48);
    w142->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w142->setSizePolicy(sp); }
    lay138->addWidget(w142, 9, 0, 1, 2);
    auto* w143 = new QLabel(nullptr);
    w143->setText(trUi_(QString::fromUtf8("Overlap:")));
    w143->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.\nAdjust the value starting with lower values\nand increasing it if areas that should be processed are still \nnot processed.\nLower values = faster processing, faster execution on CNC.\nHigher values = slow processing and slow execution on CNC\ndue of too many paths.")));
    lay138->addWidget(w143, 10, 0, 1, 1);
    tools_ncc_group_ncc_overlap_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_ncc_overlap_entry->setProperty("decimals", 4);
    tools_ncc_group_ncc_overlap_entry->setProperty("maximum", 99.9999);
    tools_ncc_group_ncc_overlap_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_ncc_overlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    tools_ncc_group_ncc_overlap_entry->setProperty("value", 40.0);
    tools_ncc_group_ncc_overlap_entry->setWrapping(true);
    lay138->addWidget(tools_ncc_group_ncc_overlap_entry, 10, 1, 1, 1);
    auto* w144 = new QLabel(nullptr);
    w144->setText(trUi_(QString::fromUtf8("Margin:")));
    w144->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay138->addWidget(w144, 11, 0, 1, 1);
    tools_ncc_group_ncc_margin_entry = new FCDoubleSpinner(nullptr);
    tools_ncc_group_ncc_margin_entry->setProperty("decimals", 4);
    tools_ncc_group_ncc_margin_entry->setProperty("maximum", 10000.0);
    tools_ncc_group_ncc_margin_entry->setProperty("minimum", -10000.0);
    tools_ncc_group_ncc_margin_entry->setProperty("singleStep", 0.1);
    tools_ncc_group_ncc_margin_entry->setProperty("value", 1.0);
    lay138->addWidget(tools_ncc_group_ncc_margin_entry, 11, 1, 1, 1);
    auto* w145 = new QLabel(nullptr);
    w145->setText(trUi_(QString::fromUtf8("Method:")));
    w145->setToolTip(trUi_(QString::fromUtf8("Algorithm for copper clearing:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.")));
    lay138->addWidget(w145, 12, 0, 1, 1);
    tools_ncc_group_ncc_method_combo = new FCComboBox2(nullptr);
    tools_ncc_group_ncc_method_combo->setProperty("currentText", trUi_(QString::fromUtf8("Seed")));
    tools_ncc_group_ncc_method_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    tools_ncc_group_ncc_method_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    tools_ncc_group_ncc_method_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    tools_ncc_group_ncc_method_combo->addItem(trUi_(QString::fromUtf8("Combo")));
    tools_ncc_group_ncc_method_combo->setCurrentIndex(1);
    lay138->addWidget(tools_ncc_group_ncc_method_combo, 12, 1, 1, 1);
    tools_ncc_group_ncc_connect_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_connect_cb->setChecked(true);
    tools_ncc_group_ncc_connect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    tools_ncc_group_ncc_connect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    lay138->addWidget(tools_ncc_group_ncc_connect_cb, 13, 0, 1, 1);
    tools_ncc_group_ncc_contour_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_contour_cb->setChecked(true);
    tools_ncc_group_ncc_contour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    tools_ncc_group_ncc_contour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    lay138->addWidget(tools_ncc_group_ncc_contour_cb, 13, 1, 1, 1);
    tools_ncc_group_ncc_choice_offset_cb = new FCCheckBox(nullptr);
    tools_ncc_group_ncc_choice_offset_cb->setText(trUi_(QString::fromUtf8("Offset")));
    tools_ncc_group_ncc_choice_offset_cb->setToolTip(trUi_(QString::fromUtf8("If used, it will add an offset to the copper features.\nThe copper clearing will finish to a distance\nfrom the copper features.")));
    lay138->addWidget(tools_ncc_group_ncc_choice_offset_cb, 14, 0, 1, 2);
    tools_ncc_group_ncc_offset_label = new QLabel(nullptr);
    tools_ncc_group_ncc_offset_label->setText(trUi_(QString::fromUtf8("Offset value:")));
    tools_ncc_group_ncc_offset_label->setToolTip(trUi_(QString::fromUtf8("If used, it will add an offset to the copper features.\nThe copper clearing will finish to a distance\nfrom the copper features.")));
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
    tools_ncc_group_ncc_rest_cb->setText(trUi_(QString::fromUtf8("Rest")));
    tools_ncc_group_ncc_rest_cb->setToolTip(trUi_(QString::fromUtf8("If checked, use 'rest machining'.\nBasically it will process copper outside PCB features,\nusing the biggest tool and continue with the next tools,\nfrom bigger to smaller, to process the copper features that\ncould not be processed by previous tool, until there is\nnothing left to process or there are no more tools.\n\nIf not checked, use the standard algorithm.")));
    lay138->addWidget(tools_ncc_group_ncc_rest_cb, 17, 0, 1, 2);
    auto* w147 = new QLabel(nullptr);
    w147->setText(trUi_(QString::fromUtf8("Selection:")));
    w147->setToolTip(trUi_(QString::fromUtf8("Selection of area to be processed.\n- 'Itself' - the processing extent is based on the object that is processed.\n - 'Area Selection' - left mouse click to start selection of the area to be processed.\n- 'Reference Object' - will process the area specified by another object.")));
    lay138->addWidget(w147, 18, 0, 1, 1);
    tools_ncc_group_select_combo = new FCComboBox2(nullptr);
    tools_ncc_group_select_combo->setProperty("currentText", trUi_(QString::fromUtf8("Itself")));
    tools_ncc_group_select_combo->addItem(trUi_(QString::fromUtf8("Itself")));
    tools_ncc_group_select_combo->addItem(trUi_(QString::fromUtf8("Area Selection")));
    tools_ncc_group_select_combo->addItem(trUi_(QString::fromUtf8("Reference Object")));
    lay138->addWidget(tools_ncc_group_select_combo, 18, 1, 1, 1);
    tools_ncc_group_area_shape_label = new QLabel(nullptr);
    tools_ncc_group_area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    tools_ncc_group_area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    lay138->addWidget(tools_ncc_group_area_shape_label, 19, 0, 1, 1);
    tools_ncc_group_area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay138->addWidget(tools_ncc_group_area_shape_radio, 19, 1, 1, 1);
    auto* w148 = new QFrame(nullptr);
    w148->setProperty("frameShadow", 48);
    w148->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w148->setSizePolicy(sp); }
    lay138->addWidget(w148, 20, 0, 1, 2);
    auto* w149 = new QLabel(nullptr);
    w149->setText(trUi_(QString::fromUtf8("Plotting:")));
    w149->setToolTip(trUi_(QString::fromUtf8("- 'Normal' - normal plotting, done at the end of the job\n- 'Progressive' - each shape is plotted after it is generated")));
    lay138->addWidget(w149, 21, 0, 1, 1);
    tools_ncc_group_plotting_radio = new RadioSet({{trUi_(QString::fromUtf8("Normal")), QVariant(QString::fromUtf8("normal"))}, {trUi_(QString::fromUtf8("Progressive")), QVariant(QString::fromUtf8("progressive"))}}, Qt::Horizontal, true, nullptr);
    tools_ncc_group_plotting_radio->setValue(QVariant(QString::fromUtf8("normal")));
    lay138->addWidget(tools_ncc_group_plotting_radio, 21, 1, 1, 1);
    tools_ncc_group_valid_cb = new FCCheckBox(nullptr);
    tools_ncc_group_valid_cb->setChecked(true);
    tools_ncc_group_valid_cb->setProperty("objectName", QString::fromUtf8("n_check"));
    tools_ncc_group_valid_cb->setText(trUi_(QString::fromUtf8("Check validity")));
    tools_ncc_group_valid_cb->setToolTip(trUi_(QString::fromUtf8("If checked then the tools diameters are verified\nif they will provide a complete isolation.")));
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
    tools_paint_group_paint_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_paint_group_paint_label->setToolTip(trUi_(QString::fromUtf8("Creates tool paths to cover the\nwhole area of a polygon.")));
    tools_paint_group_layout->addWidget(tools_paint_group_paint_label, 0);
    auto* lay150 = new QGridLayout();
    lay150->setContentsMargins(0, 0, 0, 0);
    lay150->setHorizontalSpacing(6);
    lay150->setVerticalSpacing(6);
    auto* w151 = new QLabel(nullptr);
    w151->setText(trUi_(QString::fromUtf8("<b><font color=\"green\">Tools Dia:</font></b>")));
    w151->setToolTip(trUi_(QString::fromUtf8("Diameters of the tools, separated by comma.\nThe value of the diameter has to use the dot decimals separator.\nValid values: 0.3, 1.0")));
    lay150->addWidget(w151, 0, 0, 1, 1);
    tools_paint_group_painttooldia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_paint_group_painttooldia_entry->setProperty("placeholderText", trUi_(QString::fromUtf8("Comma separated values")));
    tools_paint_group_painttooldia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_paint_group_painttooldia_entry->setText(trUi_(QString::fromUtf8("0.3")));
    lay150->addWidget(tools_paint_group_painttooldia_entry, 0, 1, 1, 1);
    tools_paint_group_tool_type_label = new QLabel(nullptr);
    tools_paint_group_tool_type_label->setText(trUi_(QString::fromUtf8("Tool Type:")));
    tools_paint_group_tool_type_label->setToolTip(trUi_(QString::fromUtf8("Default tool type:\n- 'V-shape'\n- Circular")));
    lay150->addWidget(tools_paint_group_tool_type_label, 1, 0, 1, 1);
    tools_paint_group_tool_type_radio = new RadioSet({{trUi_(QString::fromUtf8("V-shape")), QVariant(QString::fromUtf8("V"))}, {trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("C1"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_tool_type_radio->setProperty("objectName", QString::fromUtf8("Tool Type"));
    tools_paint_group_tool_type_radio->setValue(QVariant(QString::fromUtf8("C1")));
    lay150->addWidget(tools_paint_group_tool_type_radio, 1, 1, 1, 1);
    tools_paint_group_tipdialabel = new QLabel(nullptr);
    tools_paint_group_tipdialabel->setText(trUi_(QString::fromUtf8("V-Tip Dia:")));
    tools_paint_group_tipdialabel->setToolTip(trUi_(QString::fromUtf8("The tip diameter for V-Shape Tool")));
    lay150->addWidget(tools_paint_group_tipdialabel, 2, 0, 1, 1);
    tools_paint_group_tipdia_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_tipdia_entry->setProperty("decimals", 4);
    tools_paint_group_tipdia_entry->setProperty("maximum", 10000.0);
    tools_paint_group_tipdia_entry->setProperty("objectName", QString::fromUtf8("V-Tip Dia"));
    tools_paint_group_tipdia_entry->setProperty("singleStep", 0.1);
    tools_paint_group_tipdia_entry->setProperty("value", 0.1);
    lay150->addWidget(tools_paint_group_tipdia_entry, 2, 1, 1, 1);
    tools_paint_group_tipanglelabel = new QLabel(nullptr);
    tools_paint_group_tipanglelabel->setText(trUi_(QString::fromUtf8("V-Tip Angle:")));
    tools_paint_group_tipanglelabel->setToolTip(trUi_(QString::fromUtf8("The tip angle for V-Shape Tool.\nIn degree.")));
    lay150->addWidget(tools_paint_group_tipanglelabel, 3, 0, 1, 1);
    tools_paint_group_tipangle_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_tipangle_entry->setProperty("decimals", 4);
    tools_paint_group_tipangle_entry->setProperty("maximum", 180.0);
    tools_paint_group_tipangle_entry->setProperty("minimum", 1.0);
    tools_paint_group_tipangle_entry->setProperty("objectName", QString::fromUtf8("V-Tip Angle"));
    tools_paint_group_tipangle_entry->setProperty("singleStep", 5.0);
    tools_paint_group_tipangle_entry->setProperty("value", 30.0);
    lay150->addWidget(tools_paint_group_tipangle_entry, 3, 1, 1, 1);
    auto* w152 = new QLabel(nullptr);
    w152->setText(trUi_(QString::fromUtf8("Cut Z:")));
    w152->setToolTip(trUi_(QString::fromUtf8("Depth of cut into material. Negative value.\nIn application units.")));
    lay150->addWidget(w152, 4, 0, 1, 1);
    tools_paint_group_cutz_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_cutz_entry->setProperty("decimals", 4);
    tools_paint_group_cutz_entry->setProperty("maximum", 0.0);
    tools_paint_group_cutz_entry->setProperty("minimum", -910000.0);
    tools_paint_group_cutz_entry->setProperty("objectName", QString::fromUtf8("Cut Z"));
    tools_paint_group_cutz_entry->setToolTip(trUi_(QString::fromUtf8("Depth of cut into material. Negative value.\nIn application units.")));
    tools_paint_group_cutz_entry->setProperty("value", -0.05);
    lay150->addWidget(tools_paint_group_cutz_entry, 4, 1, 1, 1);
    tools_paint_group_newdialabel = new QLabel(nullptr);
    tools_paint_group_newdialabel->setText(trUi_(QString::fromUtf8("New Dia:")));
    tools_paint_group_newdialabel->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool to add in the Tool Table.\nIf the tool is V-shape type then this value is automatically\ncalculated from the other parameters.")));
    lay150->addWidget(tools_paint_group_newdialabel, 5, 0, 1, 1);
    tools_paint_group_newdia_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_newdia_entry->setProperty("decimals", 4);
    tools_paint_group_newdia_entry->setProperty("maximum", 10000.0);
    tools_paint_group_newdia_entry->setProperty("objectName", QString::fromUtf8("Tool Dia"));
    tools_paint_group_newdia_entry->setProperty("value", 0.1);
    lay150->addWidget(tools_paint_group_newdia_entry, 5, 1, 1, 1);
    auto* w153 = new QFrame(nullptr);
    w153->setProperty("frameShadow", 48);
    w153->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w153->setSizePolicy(sp); }
    lay150->addWidget(w153, 6, 0, 1, 2);
    tools_paint_group_paint_order_label = new QLabel(nullptr);
    tools_paint_group_paint_order_label->setText(trUi_(QString::fromUtf8("Tool order:")));
    tools_paint_group_paint_order_label->setToolTip(trUi_(QString::fromUtf8("This set the way that the tools in the tools table are used.\n'No' --> means that the used order is the one in the tool table\n'Forward' --> means that the tools will be ordered from small to big\n'Reverse' --> means that the tools will ordered from big to small\n\nWARNING: using rest machining will automatically set the order\nin reverse and disable this control.")));
    lay150->addWidget(tools_paint_group_paint_order_label, 7, 0, 1, 1);
    tools_paint_group_paint_order_radio = new RadioSet({{trUi_(QString::fromUtf8("No")), QVariant(QString::fromUtf8("no"))}, {trUi_(QString::fromUtf8("Forward")), QVariant(QString::fromUtf8("fwd"))}, {trUi_(QString::fromUtf8("Reverse")), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_paint_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay150->addWidget(tools_paint_group_paint_order_radio, 7, 1, 1, 1);
    auto* w154 = new QFrame(nullptr);
    w154->setProperty("frameShadow", 48);
    w154->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w154->setSizePolicy(sp); }
    lay150->addWidget(w154, 8, 0, 1, 2);
    auto* w155 = new QLabel(nullptr);
    w155->setText(trUi_(QString::fromUtf8("Overlap:")));
    w155->setToolTip(trUi_(QString::fromUtf8("How much (percentage) of the tool width to overlap each tool pass.\nAdjust the value starting with lower values\nand increasing it if areas that should be processed are still \nnot processed.\nLower values = faster processing, faster execution on CNC.\nHigher values = slow processing and slow execution on CNC\ndue of too many paths.")));
    lay150->addWidget(w155, 9, 0, 1, 1);
    tools_paint_group_paintoverlap_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_paintoverlap_entry->setProperty("decimals", 4);
    tools_paint_group_paintoverlap_entry->setProperty("maximum", 99.9999);
    tools_paint_group_paintoverlap_entry->setProperty("singleStep", 0.1);
    tools_paint_group_paintoverlap_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    tools_paint_group_paintoverlap_entry->setProperty("value", 20.0);
    tools_paint_group_paintoverlap_entry->setWrapping(true);
    lay150->addWidget(tools_paint_group_paintoverlap_entry, 9, 1, 1, 1);
    auto* w156 = new QLabel(nullptr);
    w156->setText(trUi_(QString::fromUtf8("Margin:")));
    w156->setToolTip(trUi_(QString::fromUtf8("Distance by which to avoid\nthe edges of the polygon to\nbe painted.")));
    lay150->addWidget(w156, 10, 0, 1, 1);
    tools_paint_group_paintmargin_entry = new FCDoubleSpinner(nullptr);
    tools_paint_group_paintmargin_entry->setProperty("decimals", 4);
    tools_paint_group_paintmargin_entry->setProperty("maximum", 10000.0);
    tools_paint_group_paintmargin_entry->setProperty("minimum", -10000.0);
    tools_paint_group_paintmargin_entry->setProperty("singleStep", 0.1);
    lay150->addWidget(tools_paint_group_paintmargin_entry, 10, 1, 1, 1);
    auto* w157 = new QLabel(nullptr);
    w157->setText(trUi_(QString::fromUtf8("Method:")));
    w157->setToolTip(trUi_(QString::fromUtf8("Algorithm for painting:\n- Standard: Fixed step inwards.\n- Seed-based: Outwards from seed.\n- Line-based: Parallel lines.\n- Laser-lines: Active only for Gerber objects.\nWill create lines that follow the traces.\n- Combo: In case of failure a new method will be picked from the above\nin the order specified.")));
    lay150->addWidget(w157, 11, 0, 1, 1);
    tools_paint_group_paintmethod_combo = new FCComboBox2(nullptr);
    tools_paint_group_paintmethod_combo->setProperty("currentText", trUi_(QString::fromUtf8("Standard")));
    tools_paint_group_paintmethod_combo->addItem(trUi_(QString::fromUtf8("Standard")));
    tools_paint_group_paintmethod_combo->addItem(trUi_(QString::fromUtf8("Seed")));
    tools_paint_group_paintmethod_combo->addItem(trUi_(QString::fromUtf8("Lines")));
    tools_paint_group_paintmethod_combo->addItem(trUi_(QString::fromUtf8("Laser_lines")));
    tools_paint_group_paintmethod_combo->addItem(trUi_(QString::fromUtf8("Combo")));
    lay150->addWidget(tools_paint_group_paintmethod_combo, 11, 1, 1, 1);
    tools_paint_group_pathconnect_cb = new FCCheckBox(nullptr);
    tools_paint_group_pathconnect_cb->setChecked(true);
    tools_paint_group_pathconnect_cb->setText(trUi_(QString::fromUtf8("Connect")));
    tools_paint_group_pathconnect_cb->setToolTip(trUi_(QString::fromUtf8("Draw lines between resulting\nsegments to minimize tool lifts.")));
    lay150->addWidget(tools_paint_group_pathconnect_cb, 12, 0, 1, 1);
    tools_paint_group_contour_cb = new FCCheckBox(nullptr);
    tools_paint_group_contour_cb->setChecked(true);
    tools_paint_group_contour_cb->setText(trUi_(QString::fromUtf8("Contour")));
    tools_paint_group_contour_cb->setToolTip(trUi_(QString::fromUtf8("Cut around the perimeter of the polygon\nto trim rough edges.")));
    lay150->addWidget(tools_paint_group_contour_cb, 12, 1, 1, 1);
    auto* w158 = new QFrame(nullptr);
    w158->setProperty("frameShadow", 48);
    w158->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w158->setSizePolicy(sp); }
    lay150->addWidget(w158, 13, 0, 1, 2);
    tools_paint_group_rest_cb = new FCCheckBox(nullptr);
    tools_paint_group_rest_cb->setProperty("objectName", QString::fromUtf8("Rest"));
    tools_paint_group_rest_cb->setText(trUi_(QString::fromUtf8("Rest")));
    tools_paint_group_rest_cb->setToolTip(trUi_(QString::fromUtf8("If checked, use 'rest machining'.\nBasically it will process copper outside PCB features,\nusing the biggest tool and continue with the next tools,\nfrom bigger to smaller, to process the copper features that\ncould not be processed by previous tool, until there is\nnothing left to process or there are no more tools.\n\nIf not checked, use the standard algorithm.")));
    lay150->addWidget(tools_paint_group_rest_cb, 14, 0, 1, 2);
    auto* w159 = new QLabel(nullptr);
    w159->setText(trUi_(QString::fromUtf8("Selection:")));
    w159->setToolTip(trUi_(QString::fromUtf8("Selection of area to be processed.\n- 'Polygon Selection' - left mouse click to add/remove polygons to be processed.\n- 'Area Selection' - left mouse click to start selection of the area to be processed.\nKeeping a modifier key pressed (CTRL or SHIFT) will allow to add multiple areas.\n- 'All Polygons' - the process will start after click.\n- 'Reference Object' - will process the area specified by another object.")));
    lay150->addWidget(w159, 15, 0, 1, 1);
    tools_paint_group_selectmethod_combo = new FCComboBox2(nullptr);
    tools_paint_group_selectmethod_combo->setProperty("currentText", trUi_(QString::fromUtf8("All")));
    tools_paint_group_selectmethod_combo->addItem(trUi_(QString::fromUtf8("All")));
    tools_paint_group_selectmethod_combo->addItem(trUi_(QString::fromUtf8("Polygon Selection")));
    tools_paint_group_selectmethod_combo->addItem(trUi_(QString::fromUtf8("Area Selection")));
    tools_paint_group_selectmethod_combo->addItem(trUi_(QString::fromUtf8("Reference Object")));
    lay150->addWidget(tools_paint_group_selectmethod_combo, 15, 1, 1, 1);
    tools_paint_group_area_shape_label = new QLabel(nullptr);
    tools_paint_group_area_shape_label->setText(trUi_(QString::fromUtf8("Shape:")));
    tools_paint_group_area_shape_label->setToolTip(trUi_(QString::fromUtf8("The kind of selection shape used for area selection.")));
    lay150->addWidget(tools_paint_group_area_shape_label, 18, 0, 1, 1);
    tools_paint_group_area_shape_radio = new RadioSet({{trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Polygon")), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    tools_paint_group_area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay150->addWidget(tools_paint_group_area_shape_radio, 18, 1, 1, 1);
    auto* w160 = new QFrame(nullptr);
    w160->setProperty("frameShadow", 48);
    w160->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w160->setSizePolicy(sp); }
    lay150->addWidget(w160, 19, 0, 1, 2);
    auto* w161 = new QLabel(nullptr);
    w161->setText(trUi_(QString::fromUtf8("Plotting:")));
    w161->setToolTip(trUi_(QString::fromUtf8("- 'Normal' - normal plotting, done at the end of the job\n- 'Progressive' - each shape is plotted after it is generated")));
    lay150->addWidget(w161, 20, 0, 1, 1);
    tools_paint_group_paint_plotting_radio = new RadioSet({{trUi_(QString::fromUtf8("Normal")), QVariant(QString::fromUtf8("normal"))}, {trUi_(QString::fromUtf8("Progressive")), QVariant(QString::fromUtf8("progressive"))}}, Qt::Horizontal, true, nullptr);
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
    tools_film_group_film_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_film_group_film_label->setToolTip(trUi_(QString::fromUtf8("Create a PCB film from a Gerber or Geometry object.\nThe file is saved in SVG format.")));
    tools_film_group_layout->addWidget(tools_film_group_film_label, 0);
    auto* lay162 = new QGridLayout();
    lay162->setContentsMargins(0, 0, 0, 0);
    lay162->setHorizontalSpacing(6);
    lay162->setVerticalSpacing(6);
    auto* w163 = new FCLabel(nullptr);
    w163->setText(trUi_(QString::fromUtf8("Film Type:")));
    w163->setToolTip(trUi_(QString::fromUtf8("Generate a Positive black film or a Negative film.\nPositive means that it will print the features\nwith black on a white canvas.\nNegative means that it will print the features\nwith white on a black canvas.\nThe Film format is SVG.")));
    lay162->addWidget(w163, 0, 0, 1, 1);
    tools_film_group_film_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Pos")), QVariant(QString::fromUtf8("pos"))}, {trUi_(QString::fromUtf8("Neg")), QVariant(QString::fromUtf8("neg"))}}, Qt::Horizontal, true, nullptr);
    tools_film_group_film_type_radio->setValue(QVariant(QString::fromUtf8("neg")));
    lay162->addWidget(tools_film_group_film_type_radio, 0, 1, 1, 1);
    tools_film_group_film_color_label = new FCLabel(nullptr);
    tools_film_group_film_color_label->setText(trUi_(QString::fromUtf8("Film Color:")));
    tools_film_group_film_color_label->setToolTip(trUi_(QString::fromUtf8("Set the film color when positive film is selected.")));
    lay162->addWidget(tools_film_group_film_color_label, 1, 0, 1, 1);
    tools_film_group_film_color_entry = new FCColorEntry(nullptr);
    tools_film_group_film_color_entry->setValue(QString::fromUtf8("#000000"));
    lay162->addWidget(tools_film_group_film_color_entry, 1, 1, 1, 1);
    tools_film_group_film_boundary_label = new FCLabel(nullptr);
    tools_film_group_film_boundary_label->setText(trUi_(QString::fromUtf8("Border:")));
    tools_film_group_film_boundary_label->setToolTip(trUi_(QString::fromUtf8("Specify a border around the object.\nOnly for negative film.\nIt helps if we use as a Box Object the same \nobject as in Film Object. It will create a thick\nblack bar around the actual print allowing for a\nbetter delimitation of the outline features which are of\nwhite color like the rest and which may confound with the\nsurroundings if not for this border.")));
    lay162->addWidget(tools_film_group_film_boundary_label, 2, 0, 1, 1);
    tools_film_group_film_boundary_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_boundary_entry->setProperty("decimals", 4);
    tools_film_group_film_boundary_entry->setProperty("maximum", 10000.0);
    tools_film_group_film_boundary_entry->setProperty("singleStep", 0.1);
    tools_film_group_film_boundary_entry->setProperty("value", 1.0);
    lay162->addWidget(tools_film_group_film_boundary_entry, 2, 1, 1, 1);
    tools_film_group_film_scale_stroke_label = new FCLabel(nullptr);
    tools_film_group_film_scale_stroke_label->setText(trUi_(QString::fromUtf8("Scale Stroke:")));
    tools_film_group_film_scale_stroke_label->setToolTip(trUi_(QString::fromUtf8("Scale the line stroke thickness of each feature in the SVG file.\nIt means that the line that envelope each SVG feature will be thicker or thinner,\ntherefore the fine features may be more affected by this parameter.")));
    lay162->addWidget(tools_film_group_film_scale_stroke_label, 3, 0, 1, 1);
    tools_film_group_film_scale_stroke_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_scale_stroke_entry->setProperty("decimals", 4);
    tools_film_group_film_scale_stroke_entry->setProperty("maximum", 10000.0);
    tools_film_group_film_scale_stroke_entry->setProperty("singleStep", 0.1);
    lay162->addWidget(tools_film_group_film_scale_stroke_entry, 3, 1, 1, 1);
    tools_film_group_film_adj_label = new FCLabel(nullptr);
    tools_film_group_film_adj_label->setText(trUi_(QString::fromUtf8("<b>Film Adjustments</b>")));
    tools_film_group_film_adj_label->setToolTip(trUi_(QString::fromUtf8("Sometime the printers will distort the print shape, especially the Laser types.\nThis section provide the tools to compensate for the print distortions.")));
    lay162->addWidget(tools_film_group_film_adj_label, 4, 0, 1, 2);
    tools_film_group_film_scale_cb = new FCCheckBox(nullptr);
    { QFont f = tools_film_group_film_scale_cb->font(); f.setBold(true); tools_film_group_film_scale_cb->setFont(f); }
    tools_film_group_film_scale_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    tools_film_group_film_scale_cb->setText(trUi_(QString::fromUtf8("Scale Film geometry")));
    tools_film_group_film_scale_cb->setToolTip(trUi_(QString::fromUtf8("A value greater than 1 will stretch the film\nwhile a value less than 1 will jolt it.")));
    lay162->addWidget(tools_film_group_film_scale_cb, 5, 0, 1, 2);
    tools_film_group_film_scalex_label = new FCLabel(nullptr);
    tools_film_group_film_scalex_label->setText(trUi_(QString::fromUtf8("X factor:")));
    lay162->addWidget(tools_film_group_film_scalex_label, 6, 0, 1, 1);
    tools_film_group_film_scalex_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_scalex_entry->setProperty("decimals", 4);
    tools_film_group_film_scalex_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_scalex_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_scalex_entry->setProperty("singleStep", 0.01);
    tools_film_group_film_scalex_entry->setProperty("value", 1.0);
    lay162->addWidget(tools_film_group_film_scalex_entry, 6, 1, 1, 1);
    tools_film_group_film_scaley_label = new FCLabel(nullptr);
    tools_film_group_film_scaley_label->setText(trUi_(QString::fromUtf8("Y factor:")));
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
    tools_film_group_film_skew_cb->setText(trUi_(QString::fromUtf8("Skew Film geometry")));
    tools_film_group_film_skew_cb->setToolTip(trUi_(QString::fromUtf8("Positive values will skew to the right\nwhile negative values will skew to the left.")));
    lay162->addWidget(tools_film_group_film_skew_cb, 8, 0, 1, 2);
    tools_film_group_film_skewx_label = new FCLabel(nullptr);
    tools_film_group_film_skewx_label->setText(trUi_(QString::fromUtf8("X angle:")));
    lay162->addWidget(tools_film_group_film_skewx_label, 9, 0, 1, 1);
    tools_film_group_film_skewx_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_skewx_entry->setProperty("decimals", 4);
    tools_film_group_film_skewx_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_skewx_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_skewx_entry->setProperty("singleStep", 0.01);
    lay162->addWidget(tools_film_group_film_skewx_entry, 9, 1, 1, 1);
    tools_film_group_film_skewy_label = new FCLabel(nullptr);
    tools_film_group_film_skewy_label->setText(trUi_(QString::fromUtf8("Y angle:")));
    lay162->addWidget(tools_film_group_film_skewy_label, 10, 0, 1, 1);
    tools_film_group_film_skewy_entry = new FCDoubleSpinner(nullptr);
    tools_film_group_film_skewy_entry->setProperty("decimals", 4);
    tools_film_group_film_skewy_entry->setProperty("maximum", 999.9999);
    tools_film_group_film_skewy_entry->setProperty("minimum", -999.9999);
    tools_film_group_film_skewy_entry->setProperty("singleStep", 0.01);
    lay162->addWidget(tools_film_group_film_skewy_entry, 10, 1, 1, 1);
    tools_film_group_film_skew_ref_label = new FCLabel(nullptr);
    tools_film_group_film_skew_ref_label->setText(trUi_(QString::fromUtf8("Reference:")));
    tools_film_group_film_skew_ref_label->setToolTip(trUi_(QString::fromUtf8("The reference point to be used as origin for the skew.\nIt can be one of the four points of the geometry bounding box.")));
    lay162->addWidget(tools_film_group_film_skew_ref_label, 11, 0, 1, 1);
    tools_film_group_film_skew_reference = new RadioSet({{trUi_(QString::fromUtf8("Bottom Left")), QVariant(QString::fromUtf8("bottomleft"))}, {trUi_(QString::fromUtf8("Top Left")), QVariant(QString::fromUtf8("topleft"))}, {trUi_(QString::fromUtf8("Bottom Right")), QVariant(QString::fromUtf8("bottomright"))}, {trUi_(QString::fromUtf8("Top right")), QVariant(QString::fromUtf8("topright"))}}, Qt::Vertical, false, nullptr);
    tools_film_group_film_skew_reference->setValue(QVariant(QString::fromUtf8("bottomleft")));
    lay162->addWidget(tools_film_group_film_skew_reference, 11, 1, 1, 1);
    tools_film_group_film_mirror_cb = new FCCheckBox(nullptr);
    { QFont f = tools_film_group_film_mirror_cb->font(); f.setBold(true); tools_film_group_film_mirror_cb->setFont(f); }
    tools_film_group_film_mirror_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: black}\n\t\t\t"));
    tools_film_group_film_mirror_cb->setText(trUi_(QString::fromUtf8("Mirror Film geometry")));
    tools_film_group_film_mirror_cb->setToolTip(trUi_(QString::fromUtf8("Mirror the film geometry on the selected axis or on both.")));
    lay162->addWidget(tools_film_group_film_mirror_cb, 12, 0, 1, 2);
    tools_film_group_film_mirror_axis_label = new FCLabel(nullptr);
    tools_film_group_film_mirror_axis_label->setText(trUi_(QString::fromUtf8("Mirror Axis:")));
    lay162->addWidget(tools_film_group_film_mirror_axis_label, 13, 0, 1, 1);
    tools_film_group_film_mirror_axis = new RadioSet({{trUi_(QString::fromUtf8("None")), QVariant(QString::fromUtf8("none"))}, {trUi_(QString::fromUtf8("X")), QVariant(QString::fromUtf8("x"))}, {trUi_(QString::fromUtf8("Y")), QVariant(QString::fromUtf8("y"))}, {trUi_(QString::fromUtf8("Both")), QVariant(QString::fromUtf8("both"))}}, Qt::Horizontal, false, nullptr);
    tools_film_group_film_mirror_axis->setValue(QVariant(QString::fromUtf8("none")));
    lay162->addWidget(tools_film_group_film_mirror_axis, 13, 1, 1, 1);
    auto* w164 = new QFrame(nullptr);
    w164->setProperty("frameShadow", 48);
    w164->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w164->setSizePolicy(sp); }
    lay162->addWidget(w164, 14, 0, 1, 2);
    tools_film_group_file_type_label = new FCLabel(nullptr);
    tools_film_group_file_type_label->setText(trUi_(QString::fromUtf8("Film Type:")));
    tools_film_group_file_type_label->setToolTip(trUi_(QString::fromUtf8("The file type of the saved film. Can be:\n- 'SVG' -> open-source vectorial format\n- 'PNG' -> raster image\n- 'PDF' -> portable document format")));
    lay162->addWidget(tools_film_group_file_type_label, 15, 0, 1, 1);
    tools_film_group_file_type_radio = new RadioSet({{trUi_(QString::fromUtf8("SVG")), QVariant(QString::fromUtf8("svg"))}, {trUi_(QString::fromUtf8("PNG")), QVariant(QString::fromUtf8("png"))}, {trUi_(QString::fromUtf8("PDF")), QVariant(QString::fromUtf8("pdf"))}}, Qt::Horizontal, false, nullptr);
    tools_film_group_file_type_radio->setValue(QVariant(QString::fromUtf8("svg")));
    lay162->addWidget(tools_film_group_file_type_radio, 15, 1, 1, 1);
    tools_film_group_orientation_label = new FCLabel(nullptr);
    tools_film_group_orientation_label->setText(trUi_(QString::fromUtf8("Page Orientation:")));
    tools_film_group_orientation_label->setToolTip(trUi_(QString::fromUtf8("Can be:\n- Portrait\n- Landscape")));
    lay162->addWidget(tools_film_group_orientation_label, 16, 0, 1, 1);
    tools_film_group_orientation_radio = new RadioSet({{trUi_(QString::fromUtf8("Portrait")), QVariant(QString::fromUtf8("p"))}, {trUi_(QString::fromUtf8("Landscape")), QVariant(QString::fromUtf8("l"))}}, Qt::Horizontal, false, nullptr);
    tools_film_group_orientation_radio->setValue(QVariant(QString::fromUtf8("p")));
    lay162->addWidget(tools_film_group_orientation_radio, 16, 1, 1, 1);
    tools_film_group_pagesize_label = new FCLabel(nullptr);
    tools_film_group_pagesize_label->setText(trUi_(QString::fromUtf8("Page Size:")));
    tools_film_group_pagesize_label->setToolTip(trUi_(QString::fromUtf8("A selection of standard ISO 216 page sizes.")));
    lay162->addWidget(tools_film_group_pagesize_label, 17, 0, 1, 1);
    tools_film_group_pagesize_combo = new FCComboBox(nullptr);
    tools_film_group_pagesize_combo->setProperty("currentText", trUi_(QString::fromUtf8("A4")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("Bounds")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A0")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A1")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A2")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A3")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A4")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A5")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A6")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A7")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A8")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A9")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("A10")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B0")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B1")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B2")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B3")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B4")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B5")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B6")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B7")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B8")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B9")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("B10")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C0")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C1")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C2")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C3")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C4")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C5")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C6")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C7")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C8")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C9")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("C10")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("LETTER")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("LEGAL")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("ELEVENSEVENTEEN")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("JUNIOR_LEGAL")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("HALF_LETTER")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("GOV_LETTER")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("GOV_LEGAL")));
    tools_film_group_pagesize_combo->addItem(trUi_(QString::fromUtf8("LEDGER")));
    tools_film_group_pagesize_combo->setCurrentIndex(5);
    lay162->addWidget(tools_film_group_pagesize_combo, 17, 1, 1, 1);
    tools_film_group_png_dpi_label = new FCLabel(nullptr);
    tools_film_group_png_dpi_label->setText(trUi_(QString::fromUtf8("PNG DPI:")));
    tools_film_group_png_dpi_label->setToolTip(trUi_(QString::fromUtf8("Default value is 96 DPI. Change this value to scale the PNG file.")));
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
    tools_transform_group_transform_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_transform_group_transform_label->setToolTip(trUi_(QString::fromUtf8("Various transformations that can be applied\non a application object.")));
    tools_transform_group_layout->addWidget(tools_transform_group_transform_label, 0);
    auto* lay165 = new QGridLayout();
    lay165->setContentsMargins(0, 0, 0, 0);
    lay165->setHorizontalSpacing(6);
    lay165->setVerticalSpacing(6);
    auto* w166 = new QLabel(nullptr);
    w166->setText(trUi_(QString::fromUtf8("Reference:")));
    w166->setToolTip(trUi_(QString::fromUtf8("The reference point for Rotate, Skew, Scale, Mirror.\nCan be:\n- Origin -> it is the 0, 0 point\n- Selection -> the center of the bounding box of the selected objects\n- Point -> a custom point defined by X,Y coordinates\n- Object -> the center of the bounding box of a specific object")));
    lay165->addWidget(w166, 0, 0, 1, 1);
    tools_transform_group_ref_combo = new FCComboBox(nullptr);
    tools_transform_group_ref_combo->setProperty("currentText", trUi_(QString::fromUtf8("Selection")));
    tools_transform_group_ref_combo->addItem(trUi_(QString::fromUtf8("Origin")));
    tools_transform_group_ref_combo->addItem(trUi_(QString::fromUtf8("Selection")));
    tools_transform_group_ref_combo->addItem(trUi_(QString::fromUtf8("Point")));
    tools_transform_group_ref_combo->addItem(trUi_(QString::fromUtf8("Object")));
    tools_transform_group_ref_combo->setCurrentIndex(1);
    lay165->addWidget(tools_transform_group_ref_combo, 0, 1, 1, 1);
    tools_transform_group_point_label = new QLabel(nullptr);
    tools_transform_group_point_label->setText(trUi_(QString::fromUtf8("Point:")));
    tools_transform_group_point_label->setToolTip(trUi_(QString::fromUtf8("A point of reference in format X,Y.")));
    lay165->addWidget(tools_transform_group_point_label, 1, 0, 1, 1);
    tools_transform_group_point_entry = new NumericalEvalTupleEntry(nullptr);
    tools_transform_group_point_entry->setText(trUi_(QString::fromUtf8("0, 0")));
    lay165->addWidget(tools_transform_group_point_entry, 1, 1, 1, 1);
    tools_transform_group_type_object_label = new QLabel(nullptr);
    tools_transform_group_type_object_label->setText(trUi_(QString::fromUtf8("Object:")));
    tools_transform_group_type_object_label->setToolTip(trUi_(QString::fromUtf8("The type of object used as reference.")));
    lay165->addWidget(tools_transform_group_type_object_label, 3, 0, 1, 1);
    tools_transform_group_type_obj_combo = new FCComboBox(nullptr);
    tools_transform_group_type_obj_combo->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    tools_transform_group_type_obj_combo->addItem(trUi_(QString::fromUtf8("Gerber")));
    tools_transform_group_type_obj_combo->addItem(trUi_(QString::fromUtf8("Excellon")));
    tools_transform_group_type_obj_combo->addItem(trUi_(QString::fromUtf8("Geometry")));
    lay165->addWidget(tools_transform_group_type_obj_combo, 3, 1, 1, 1);
    auto* w167 = new QLabel(nullptr);
    w167->setText(trUi_(QString::fromUtf8("<b>Rotate</b>")));
    lay165->addWidget(w167, 4, 0, 1, 2);
    tools_transform_group_rotate_label = new QLabel(nullptr);
    tools_transform_group_rotate_label->setText(trUi_(QString::fromUtf8("Angle:")));
    tools_transform_group_rotate_label->setToolTip(trUi_(QString::fromUtf8("Angle, in degrees.\nFloat number between -360 and 359.\nPositive numbers for CW motion.\nNegative numbers for CCW motion.")));
    lay165->addWidget(tools_transform_group_rotate_label, 6, 0, 1, 1);
    tools_transform_group_rotate_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_rotate_entry->setProperty("decimals", 4);
    tools_transform_group_rotate_entry->setProperty("maximum", 360.0);
    tools_transform_group_rotate_entry->setProperty("minimum", -360.0);
    tools_transform_group_rotate_entry->setProperty("singleStep", 15.0);
    tools_transform_group_rotate_entry->setProperty("value", 90.0);
    lay165->addWidget(tools_transform_group_rotate_entry, 6, 1, 1, 1);
    auto* w168 = new QLabel(nullptr);
    w168->setText(trUi_(QString::fromUtf8("<b>Skew</b>")));
    lay165->addWidget(w168, 8, 0, 1, 1);
    tools_transform_group_skew_link_cb = new FCCheckBox(nullptr);
    tools_transform_group_skew_link_cb->setChecked(true);
    tools_transform_group_skew_link_cb->setText(trUi_(QString::fromUtf8("Link")));
    tools_transform_group_skew_link_cb->setToolTip(trUi_(QString::fromUtf8("Link the Y entry to X entry and copy its content.")));
    lay165->addWidget(tools_transform_group_skew_link_cb, 8, 1, 1, 1);
    tools_transform_group_skewx_label = new QLabel(nullptr);
    tools_transform_group_skewx_label->setText(trUi_(QString::fromUtf8("X angle:")));
    tools_transform_group_skewx_label->setToolTip(trUi_(QString::fromUtf8("Angle, in degrees.\nFloat number between -360 and 359.")));
    lay165->addWidget(tools_transform_group_skewx_label, 9, 0, 1, 1);
    tools_transform_group_skewx_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_skewx_entry->setProperty("decimals", 4);
    tools_transform_group_skewx_entry->setProperty("maximum", 360.0);
    tools_transform_group_skewx_entry->setProperty("minimum", -360.0);
    tools_transform_group_skewx_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_skewx_entry, 9, 1, 1, 1);
    tools_transform_group_skewy_label = new QLabel(nullptr);
    tools_transform_group_skewy_label->setText(trUi_(QString::fromUtf8("Y angle:")));
    tools_transform_group_skewy_label->setToolTip(trUi_(QString::fromUtf8("Angle, in degrees.\nFloat number between -360 and 359.")));
    lay165->addWidget(tools_transform_group_skewy_label, 10, 0, 1, 1);
    tools_transform_group_skewy_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_skewy_entry->setProperty("decimals", 4);
    tools_transform_group_skewy_entry->setProperty("maximum", 360.0);
    tools_transform_group_skewy_entry->setProperty("minimum", -360.0);
    tools_transform_group_skewy_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_skewy_entry, 10, 1, 1, 1);
    auto* w169 = new QLabel(nullptr);
    w169->setText(trUi_(QString::fromUtf8("<b>Scale</b>")));
    lay165->addWidget(w169, 12, 0, 1, 1);
    tools_transform_group_scale_link_cb = new FCCheckBox(nullptr);
    tools_transform_group_scale_link_cb->setChecked(true);
    tools_transform_group_scale_link_cb->setText(trUi_(QString::fromUtf8("Link")));
    tools_transform_group_scale_link_cb->setToolTip(trUi_(QString::fromUtf8("Link the Y entry to X entry and copy its content.")));
    lay165->addWidget(tools_transform_group_scale_link_cb, 12, 1, 1, 1);
    tools_transform_group_scalex_label = new QLabel(nullptr);
    tools_transform_group_scalex_label->setText(trUi_(QString::fromUtf8("X factor:")));
    tools_transform_group_scalex_label->setToolTip(trUi_(QString::fromUtf8("Factor for scaling on X axis.")));
    lay165->addWidget(tools_transform_group_scalex_label, 14, 0, 1, 1);
    tools_transform_group_scalex_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_scalex_entry->setProperty("decimals", 4);
    tools_transform_group_scalex_entry->setProperty("maximum", 10000.0);
    tools_transform_group_scalex_entry->setProperty("singleStep", 0.1);
    tools_transform_group_scalex_entry->setProperty("value", 1.0);
    lay165->addWidget(tools_transform_group_scalex_entry, 14, 1, 1, 1);
    tools_transform_group_scaley_label = new QLabel(nullptr);
    tools_transform_group_scaley_label->setText(trUi_(QString::fromUtf8("Y factor:")));
    tools_transform_group_scaley_label->setToolTip(trUi_(QString::fromUtf8("Factor for scaling on Y axis.")));
    lay165->addWidget(tools_transform_group_scaley_label, 16, 0, 1, 1);
    tools_transform_group_scaley_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_scaley_entry->setProperty("decimals", 4);
    tools_transform_group_scaley_entry->setProperty("maximum", 10000.0);
    tools_transform_group_scaley_entry->setProperty("singleStep", 0.1);
    tools_transform_group_scaley_entry->setProperty("value", 1.0);
    lay165->addWidget(tools_transform_group_scaley_entry, 16, 1, 1, 1);
    auto* w170 = new QLabel(nullptr);
    w170->setText(trUi_(QString::fromUtf8("<b>Offset</b>")));
    lay165->addWidget(w170, 20, 0, 1, 2);
    tools_transform_group_offx_label = new QLabel(nullptr);
    tools_transform_group_offx_label->setText(trUi_(QString::fromUtf8("X val:")));
    tools_transform_group_offx_label->setToolTip(trUi_(QString::fromUtf8("Distance to offset on X axis. In current units.")));
    lay165->addWidget(tools_transform_group_offx_label, 22, 0, 1, 1);
    tools_transform_group_offx_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_offx_entry->setProperty("decimals", 4);
    tools_transform_group_offx_entry->setProperty("maximum", 10000.0);
    tools_transform_group_offx_entry->setProperty("minimum", -10000.0);
    tools_transform_group_offx_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_offx_entry, 22, 1, 1, 1);
    tools_transform_group_offy_label = new QLabel(nullptr);
    tools_transform_group_offy_label->setText(trUi_(QString::fromUtf8("Y val:")));
    tools_transform_group_offy_label->setToolTip(trUi_(QString::fromUtf8("Distance to offset on Y axis. In current units.")));
    lay165->addWidget(tools_transform_group_offy_label, 24, 0, 1, 1);
    tools_transform_group_offy_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_offy_entry->setProperty("decimals", 4);
    tools_transform_group_offy_entry->setProperty("maximum", 10000.0);
    tools_transform_group_offy_entry->setProperty("minimum", -10000.0);
    tools_transform_group_offy_entry->setProperty("singleStep", 0.1);
    lay165->addWidget(tools_transform_group_offy_entry, 24, 1, 1, 1);
    auto* w171 = new QLabel(nullptr);
    w171->setText(trUi_(QString::fromUtf8("<b>Buffer</b>")));
    lay165->addWidget(w171, 26, 0, 1, 1);
    tools_transform_group_buffer_rounded_cb = new FCCheckBox(nullptr);
    tools_transform_group_buffer_rounded_cb->setChecked(true);
    tools_transform_group_buffer_rounded_cb->setText(trUi_(QString::fromUtf8("Rounded")));
    tools_transform_group_buffer_rounded_cb->setToolTip(trUi_(QString::fromUtf8("If checked then the buffer will surround the buffered shape,\nevery corner will be rounded.\nIf not checked then the buffer will follow the exact geometry\nof the buffered shape.")));
    lay165->addWidget(tools_transform_group_buffer_rounded_cb, 26, 1, 1, 1);
    tools_transform_group_buffer_label = new QLabel(nullptr);
    tools_transform_group_buffer_label->setText(trUi_(QString::fromUtf8("Distance:")));
    tools_transform_group_buffer_label->setToolTip(trUi_(QString::fromUtf8("A positive value will create the effect of dilation,\nwhile a negative value will create the effect of erosion.\nEach geometry element of the object will be increased\nor decreased with the 'distance'.")));
    lay165->addWidget(tools_transform_group_buffer_label, 28, 0, 1, 1);
    tools_transform_group_buffer_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_buffer_entry->setProperty("decimals", 4);
    tools_transform_group_buffer_entry->setProperty("maximum", 10000.0);
    tools_transform_group_buffer_entry->setProperty("minimum", -10000.0);
    tools_transform_group_buffer_entry->setProperty("singleStep", 0.1);
    tools_transform_group_buffer_entry->setWrapping(true);
    lay165->addWidget(tools_transform_group_buffer_entry, 28, 1, 1, 1);
    tools_transform_group_buffer_factor_label = new QLabel(nullptr);
    tools_transform_group_buffer_factor_label->setText(trUi_(QString::fromUtf8("Value:")));
    tools_transform_group_buffer_factor_label->setToolTip(trUi_(QString::fromUtf8("A positive value will create the effect of dilation,\nwhile a negative value will create the effect of erosion.\nEach geometry element of the object will be increased\nor decreased to fit the 'Value'. Value is a percentage\nof the initial dimension.")));
    lay165->addWidget(tools_transform_group_buffer_factor_label, 30, 0, 1, 1);
    tools_transform_group_buffer_factor_entry = new FCDoubleSpinner(nullptr);
    tools_transform_group_buffer_factor_entry->setProperty("decimals", 4);
    tools_transform_group_buffer_factor_entry->setProperty("maximum", 1000.0);
    tools_transform_group_buffer_factor_entry->setProperty("minimum", -100.0);
    tools_transform_group_buffer_factor_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
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
    tools_solderpaste_group_solderpastelabel->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_solderpaste_group_solderpastelabel->setToolTip(trUi_(QString::fromUtf8("A tool to create GCode for dispensing\nsolder paste onto a PCB.")));
    tools_solderpaste_group_layout->addWidget(tools_solderpaste_group_solderpastelabel, 0);
    auto* lay172 = new QGridLayout();
    lay172->setContentsMargins(0, 0, 0, 0);
    lay172->setHorizontalSpacing(6);
    lay172->setVerticalSpacing(6);
    auto* w173 = new QLabel(nullptr);
    w173->setText(trUi_(QString::fromUtf8("<b><font color=\"green\">Tools Dia:</font></b>")));
    w173->setToolTip(trUi_(QString::fromUtf8("Diameters of the tools, separated by comma.\nThe value of the diameter has to use the dot decimals separator.\nValid values: 0.3, 1.0")));
    lay172->addWidget(w173, 0, 0, 1, 1);
    tools_solderpaste_group_nozzle_tool_dia_entry = new NumericalEvalTupleEntry(nullptr);
    tools_solderpaste_group_nozzle_tool_dia_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_solderpaste_group_nozzle_tool_dia_entry->setText(trUi_(QString::fromUtf8("1.0, 0.3")));
    lay172->addWidget(tools_solderpaste_group_nozzle_tool_dia_entry, 0, 1, 1, 1);
    tools_solderpaste_group_addtool_entry_lbl = new QLabel(nullptr);
    tools_solderpaste_group_addtool_entry_lbl->setText(trUi_(QString::fromUtf8("<b>New Nozzle Dia:</b>")));
    tools_solderpaste_group_addtool_entry_lbl->setToolTip(trUi_(QString::fromUtf8("Diameter for the new tool to add in the Tool Table")));
    lay172->addWidget(tools_solderpaste_group_addtool_entry_lbl, 1, 0, 1, 1);
    tools_solderpaste_group_addtool_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_addtool_entry->setProperty("decimals", 4);
    tools_solderpaste_group_addtool_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_addtool_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_addtool_entry->setProperty("value", 0.3);
    lay172->addWidget(tools_solderpaste_group_addtool_entry, 1, 1, 1, 1);
    tools_solderpaste_group_z_start_label = new QLabel(nullptr);
    tools_solderpaste_group_z_start_label->setText(trUi_(QString::fromUtf8("Z Dispense Start:")));
    tools_solderpaste_group_z_start_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) when solder paste dispensing starts.")));
    lay172->addWidget(tools_solderpaste_group_z_start_label, 2, 0, 1, 1);
    tools_solderpaste_group_z_start_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_start_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_start_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_start_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_start_entry->setProperty("value", 0.05);
    lay172->addWidget(tools_solderpaste_group_z_start_entry, 2, 1, 1, 1);
    tools_solderpaste_group_z_dispense_label = new QLabel(nullptr);
    tools_solderpaste_group_z_dispense_label->setText(trUi_(QString::fromUtf8("Z Dispense:")));
    tools_solderpaste_group_z_dispense_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) when doing solder paste dispensing.")));
    lay172->addWidget(tools_solderpaste_group_z_dispense_label, 3, 0, 1, 1);
    tools_solderpaste_group_z_dispense_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_dispense_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_dispense_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_dispense_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_dispense_entry->setProperty("value", 0.1);
    lay172->addWidget(tools_solderpaste_group_z_dispense_entry, 3, 1, 1, 1);
    tools_solderpaste_group_z_stop_label = new QLabel(nullptr);
    tools_solderpaste_group_z_stop_label->setText(trUi_(QString::fromUtf8("Z Dispense Stop:")));
    tools_solderpaste_group_z_stop_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) when solder paste dispensing stops.")));
    lay172->addWidget(tools_solderpaste_group_z_stop_label, 4, 0, 1, 1);
    tools_solderpaste_group_z_stop_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_stop_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_stop_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_stop_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_stop_entry->setProperty("value", 0.05);
    lay172->addWidget(tools_solderpaste_group_z_stop_entry, 4, 1, 1, 1);
    tools_solderpaste_group_z_travel_label = new QLabel(nullptr);
    tools_solderpaste_group_z_travel_label->setText(trUi_(QString::fromUtf8("Z Travel:")));
    tools_solderpaste_group_z_travel_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) for travel between pads\n(without dispensing solder paste).")));
    lay172->addWidget(tools_solderpaste_group_z_travel_label, 5, 0, 1, 1);
    tools_solderpaste_group_z_travel_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_travel_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_travel_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_travel_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_travel_entry->setProperty("value", 0.1);
    lay172->addWidget(tools_solderpaste_group_z_travel_entry, 5, 1, 1, 1);
    tools_solderpaste_group_z_toolchange_label = new QLabel(nullptr);
    tools_solderpaste_group_z_toolchange_label->setText(trUi_(QString::fromUtf8("Z Toolchange:")));
    tools_solderpaste_group_z_toolchange_label->setToolTip(trUi_(QString::fromUtf8("The height (Z) for tool (nozzle) change.")));
    lay172->addWidget(tools_solderpaste_group_z_toolchange_label, 6, 0, 1, 1);
    tools_solderpaste_group_z_toolchange_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_z_toolchange_entry->setProperty("decimals", 4);
    tools_solderpaste_group_z_toolchange_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_z_toolchange_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_z_toolchange_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_z_toolchange_entry, 6, 1, 1, 1);
    tools_solderpaste_group_xy_toolchange_label = new QLabel(nullptr);
    tools_solderpaste_group_xy_toolchange_label->setText(trUi_(QString::fromUtf8("Toolchange X-Y:")));
    tools_solderpaste_group_xy_toolchange_label->setToolTip(trUi_(QString::fromUtf8("The X,Y location for tool (nozzle) change.\nThe format is (x, y) where x and y are real numbers.")));
    lay172->addWidget(tools_solderpaste_group_xy_toolchange_label, 7, 0, 1, 1);
    tools_solderpaste_group_xy_toolchange_entry = new NumericalEvalTupleEntry(nullptr);
    tools_solderpaste_group_xy_toolchange_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    tools_solderpaste_group_xy_toolchange_entry->setText(trUi_(QString::fromUtf8("0.0, 0.0")));
    lay172->addWidget(tools_solderpaste_group_xy_toolchange_entry, 7, 1, 1, 1);
    tools_solderpaste_group_frxy_label = new QLabel(nullptr);
    tools_solderpaste_group_frxy_label->setText(trUi_(QString::fromUtf8("Feedrate X-Y:")));
    tools_solderpaste_group_frxy_label->setToolTip(trUi_(QString::fromUtf8("Feedrate (speed) while moving on the X-Y plane.")));
    lay172->addWidget(tools_solderpaste_group_frxy_label, 8, 0, 1, 1);
    tools_solderpaste_group_frxy_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_frxy_entry->setProperty("decimals", 4);
    tools_solderpaste_group_frxy_entry->setProperty("maximum", 910000.0);
    tools_solderpaste_group_frxy_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_frxy_entry->setProperty("value", 150.0);
    lay172->addWidget(tools_solderpaste_group_frxy_entry, 8, 1, 1, 1);
    tools_solderpaste_group_frz_label = new QLabel(nullptr);
    tools_solderpaste_group_frz_label->setText(trUi_(QString::fromUtf8("Feedrate Z:")));
    tools_solderpaste_group_frz_label->setToolTip(trUi_(QString::fromUtf8("Feedrate (speed) while moving vertically\n(on Z plane).")));
    lay172->addWidget(tools_solderpaste_group_frz_label, 9, 0, 1, 1);
    tools_solderpaste_group_frz_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_frz_entry->setProperty("decimals", 4);
    tools_solderpaste_group_frz_entry->setProperty("maximum", 910000.0);
    tools_solderpaste_group_frz_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_frz_entry->setProperty("value", 150.0);
    lay172->addWidget(tools_solderpaste_group_frz_entry, 9, 1, 1, 1);
    tools_solderpaste_group_frz_dispense_label = new QLabel(nullptr);
    tools_solderpaste_group_frz_dispense_label->setText(trUi_(QString::fromUtf8("Feedrate Z Dispense:")));
    tools_solderpaste_group_frz_dispense_label->setToolTip(trUi_(QString::fromUtf8("Feedrate (speed) while moving up vertically\nto Dispense position (on Z plane).")));
    lay172->addWidget(tools_solderpaste_group_frz_dispense_label, 10, 0, 1, 1);
    tools_solderpaste_group_frz_dispense_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_frz_dispense_entry->setProperty("decimals", 4);
    tools_solderpaste_group_frz_dispense_entry->setProperty("maximum", 910000.0);
    tools_solderpaste_group_frz_dispense_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_frz_dispense_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_frz_dispense_entry, 10, 1, 1, 1);
    tools_solderpaste_group_speedfwd_label = new QLabel(nullptr);
    tools_solderpaste_group_speedfwd_label->setText(trUi_(QString::fromUtf8("Spindle Speed FWD:")));
    tools_solderpaste_group_speedfwd_label->setToolTip(trUi_(QString::fromUtf8("The dispenser speed while pushing solder paste\nthrough the dispenser nozzle.")));
    lay172->addWidget(tools_solderpaste_group_speedfwd_label, 11, 0, 1, 1);
    tools_solderpaste_group_speedfwd_entry = new FCSpinner(nullptr);
    tools_solderpaste_group_speedfwd_entry->setProperty("maximum", 99999);
    tools_solderpaste_group_speedfwd_entry->setProperty("singleStep", 1000);
    tools_solderpaste_group_speedfwd_entry->setProperty("value", 300);
    lay172->addWidget(tools_solderpaste_group_speedfwd_entry, 11, 1, 1, 1);
    tools_solderpaste_group_dwellfwd_label = new QLabel(nullptr);
    tools_solderpaste_group_dwellfwd_label->setText(trUi_(QString::fromUtf8("Dwell FWD:")));
    tools_solderpaste_group_dwellfwd_label->setToolTip(trUi_(QString::fromUtf8("Pause after solder dispensing.")));
    lay172->addWidget(tools_solderpaste_group_dwellfwd_label, 12, 0, 1, 1);
    tools_solderpaste_group_dwellfwd_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_dwellfwd_entry->setProperty("decimals", 4);
    tools_solderpaste_group_dwellfwd_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_dwellfwd_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_dwellfwd_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_dwellfwd_entry, 12, 1, 1, 1);
    tools_solderpaste_group_speedrev_label = new QLabel(nullptr);
    tools_solderpaste_group_speedrev_label->setText(trUi_(QString::fromUtf8("Spindle Speed REV:")));
    tools_solderpaste_group_speedrev_label->setToolTip(trUi_(QString::fromUtf8("The dispenser speed while retracting solder paste\nthrough the dispenser nozzle.")));
    lay172->addWidget(tools_solderpaste_group_speedrev_label, 13, 0, 1, 1);
    tools_solderpaste_group_speedrev_entry = new FCSpinner(nullptr);
    tools_solderpaste_group_speedrev_entry->setProperty("maximum", 999999);
    tools_solderpaste_group_speedrev_entry->setProperty("singleStep", 1000);
    tools_solderpaste_group_speedrev_entry->setProperty("value", 200);
    lay172->addWidget(tools_solderpaste_group_speedrev_entry, 13, 1, 1, 1);
    tools_solderpaste_group_dwellrev_label = new QLabel(nullptr);
    tools_solderpaste_group_dwellrev_label->setText(trUi_(QString::fromUtf8("Dwell REV:")));
    tools_solderpaste_group_dwellrev_label->setToolTip(trUi_(QString::fromUtf8("Pause after solder paste dispenser retracted,\nto allow pressure equilibrium.")));
    lay172->addWidget(tools_solderpaste_group_dwellrev_label, 14, 0, 1, 1);
    tools_solderpaste_group_dwellrev_entry = new FCDoubleSpinner(nullptr);
    tools_solderpaste_group_dwellrev_entry->setProperty("decimals", 4);
    tools_solderpaste_group_dwellrev_entry->setProperty("maximum", 10000.0);
    tools_solderpaste_group_dwellrev_entry->setProperty("singleStep", 0.1);
    tools_solderpaste_group_dwellrev_entry->setProperty("value", 1.0);
    lay172->addWidget(tools_solderpaste_group_dwellrev_entry, 14, 1, 1, 1);
    auto* w174 = new QLabel(nullptr);
    w174->setText(trUi_(QString::fromUtf8("Preprocessor:")));
    w174->setToolTip(trUi_(QString::fromUtf8("Files that control the GCode generation.")));
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
    tools_corners_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_corners_group_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay175->addWidget(tools_corners_group_param_label, 0, 0, 1, 2);
    tools_corners_group_type_label = new FCLabel(nullptr);
    tools_corners_group_type_label->setText(trUi_(QString::fromUtf8("Type:")));
    tools_corners_group_type_label->setToolTip(trUi_(QString::fromUtf8("Shape of the marker.")));
    lay175->addWidget(tools_corners_group_type_label, 2, 0, 1, 1);
    tools_corners_group_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Semi-Cross")), QVariant(QString::fromUtf8("s"))}, {trUi_(QString::fromUtf8("Cross")), QVariant(QString::fromUtf8("c"))}}, Qt::Horizontal, true, nullptr);
    tools_corners_group_type_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay175->addWidget(tools_corners_group_type_radio, 2, 1, 1, 1);
    tools_corners_group_thick_label = new FCLabel(nullptr);
    tools_corners_group_thick_label->setText(trUi_(QString::fromUtf8("Thickness:")));
    tools_corners_group_thick_label->setToolTip(trUi_(QString::fromUtf8("The thickness of the line that makes the corner marker.")));
    lay175->addWidget(tools_corners_group_thick_label, 4, 0, 1, 1);
    tools_corners_group_thick_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_thick_entry->setProperty("decimals", 4);
    tools_corners_group_thick_entry->setProperty("maximum", 9.9999);
    tools_corners_group_thick_entry->setProperty("singleStep", 0.0001);
    tools_corners_group_thick_entry->setProperty("value", 0.1);
    tools_corners_group_thick_entry->setWrapping(true);
    lay175->addWidget(tools_corners_group_thick_entry, 4, 1, 1, 1);
    tools_corners_group_margin_label = new FCLabel(nullptr);
    tools_corners_group_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    tools_corners_group_margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay175->addWidget(tools_corners_group_margin_label, 6, 0, 1, 1);
    tools_corners_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_margin_entry->setProperty("decimals", 4);
    tools_corners_group_margin_entry->setProperty("maximum", 10000.0);
    tools_corners_group_margin_entry->setProperty("minimum", -10000.0);
    tools_corners_group_margin_entry->setProperty("singleStep", 0.1);
    lay175->addWidget(tools_corners_group_margin_entry, 6, 1, 1, 1);
    tools_corners_group_l_label = new FCLabel(nullptr);
    tools_corners_group_l_label->setText(trUi_(QString::fromUtf8("Length:")));
    tools_corners_group_l_label->setToolTip(trUi_(QString::fromUtf8("The length of the line that makes the corner marker.")));
    lay175->addWidget(tools_corners_group_l_label, 8, 0, 1, 1);
    tools_corners_group_l_entry = new FCDoubleSpinner(nullptr);
    tools_corners_group_l_entry->setProperty("decimals", 4);
    tools_corners_group_l_entry->setProperty("maximum", 10000.0);
    tools_corners_group_l_entry->setProperty("minimum", -10000.0);
    tools_corners_group_l_entry->setProperty("singleStep", 0.0001);
    tools_corners_group_l_entry->setProperty("value", 3.0);
    lay175->addWidget(tools_corners_group_l_entry, 8, 1, 1, 1);
    tools_corners_group_drill_dia_label = new FCLabel(nullptr);
    tools_corners_group_drill_dia_label->setText(trUi_(QString::fromUtf8("Drill Dia:")));
    tools_corners_group_drill_dia_label->setToolTip(trUi_(QString::fromUtf8("Drill Diameter.")));
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
    tools_calculators_group_vshape_tool_label->setText(trUi_(QString::fromUtf8("<b>V-Shape Tool Calculator:</b>")));
    tools_calculators_group_vshape_tool_label->setToolTip(trUi_(QString::fromUtf8("Calculate the tool diameter for a given V-shape tool,\nhaving the tip diameter, tip angle and\ndepth-of-cut as parameters.")));
    tools_calculators_group_layout->addWidget(tools_calculators_group_vshape_tool_label, 0);
    auto* lay176 = new QGridLayout();
    lay176->setContentsMargins(0, 0, 0, 0);
    lay176->setHorizontalSpacing(6);
    lay176->setVerticalSpacing(6);
    tools_calculators_group_tip_dia_label = new FCLabel(nullptr);
    tools_calculators_group_tip_dia_label->setText(trUi_(QString::fromUtf8("Tip Diameter:")));
    tools_calculators_group_tip_dia_label->setToolTip(trUi_(QString::fromUtf8("This is the tool tip diameter.\nIt is specified by manufacturer.")));
    lay176->addWidget(tools_calculators_group_tip_dia_label, 0, 0, 1, 1);
    tools_calculators_group_tip_dia_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_tip_dia_entry->setProperty("decimals", 4);
    tools_calculators_group_tip_dia_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_tip_dia_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_tip_dia_entry->setProperty("value", 0.2);
    lay176->addWidget(tools_calculators_group_tip_dia_entry, 0, 1, 1, 1);
    tools_calculators_group_tip_angle_label = new FCLabel(nullptr);
    tools_calculators_group_tip_angle_label->setText(trUi_(QString::fromUtf8("Tip Angle:")));
    tools_calculators_group_tip_angle_label->setToolTip(trUi_(QString::fromUtf8("This is the angle on the tip of the tool.\nIt is specified by manufacturer.")));
    lay176->addWidget(tools_calculators_group_tip_angle_label, 2, 0, 1, 1);
    tools_calculators_group_tip_angle_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_tip_angle_entry->setProperty("decimals", 4);
    tools_calculators_group_tip_angle_entry->setProperty("maximum", 180.0);
    tools_calculators_group_tip_angle_entry->setProperty("singleStep", 5.0);
    tools_calculators_group_tip_angle_entry->setProperty("value", 30.0);
    lay176->addWidget(tools_calculators_group_tip_angle_entry, 2, 1, 1, 1);
    tools_calculators_group_cut_z_label = new FCLabel(nullptr);
    tools_calculators_group_cut_z_label->setText(trUi_(QString::fromUtf8("Cut Z:")));
    tools_calculators_group_cut_z_label->setToolTip(trUi_(QString::fromUtf8("This is depth to cut into material.\nIn the CNCJob object it is the CutZ parameter.")));
    lay176->addWidget(tools_calculators_group_cut_z_label, 4, 0, 1, 1);
    tools_calculators_group_cut_z_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_cut_z_entry->setProperty("decimals", 4);
    tools_calculators_group_cut_z_entry->setProperty("maximum", 0.0);
    tools_calculators_group_cut_z_entry->setProperty("minimum", -10000.0);
    tools_calculators_group_cut_z_entry->setProperty("singleStep", 0.01);
    lay176->addWidget(tools_calculators_group_cut_z_entry, 4, 1, 1, 1);
    tools_calculators_group_plate_title_label = new FCLabel(nullptr);
    tools_calculators_group_plate_title_label->setText(trUi_(QString::fromUtf8("<b>ElectroPlating Calculator:</b>")));
    tools_calculators_group_plate_title_label->setToolTip(trUi_(QString::fromUtf8("This calculator is useful for those who plate the via/pad/drill holes,\nusing a method like graphite ink or calcium hypophosphite ink or palladium chloride.")));
    lay176->addWidget(tools_calculators_group_plate_title_label, 3, 0, 1, 2);
    tools_calculators_group_pcblengthlabel = new FCLabel(nullptr);
    tools_calculators_group_pcblengthlabel->setText(trUi_(QString::fromUtf8("Board Length:")));
    tools_calculators_group_pcblengthlabel->setToolTip(trUi_(QString::fromUtf8("This is the board length. In centimeters.")));
    lay176->addWidget(tools_calculators_group_pcblengthlabel, 6, 0, 1, 1);
    tools_calculators_group_pcblength_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_pcblength_entry->setProperty("decimals", 4);
    tools_calculators_group_pcblength_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_pcblength_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_pcblength_entry->setProperty("value", 10.0);
    lay176->addWidget(tools_calculators_group_pcblength_entry, 6, 1, 1, 1);
    tools_calculators_group_pcbwidthlabel = new FCLabel(nullptr);
    tools_calculators_group_pcbwidthlabel->setText(trUi_(QString::fromUtf8("Board Width:")));
    tools_calculators_group_pcbwidthlabel->setToolTip(trUi_(QString::fromUtf8("This is the board width.In centimeters.")));
    lay176->addWidget(tools_calculators_group_pcbwidthlabel, 8, 0, 1, 1);
    tools_calculators_group_pcbwidth_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_pcbwidth_entry->setProperty("decimals", 4);
    tools_calculators_group_pcbwidth_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_pcbwidth_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_pcbwidth_entry->setProperty("value", 10.0);
    lay176->addWidget(tools_calculators_group_pcbwidth_entry, 8, 1, 1, 1);
    tools_calculators_group_area_label = new FCLabel(nullptr);
    tools_calculators_group_area_label->setText(trUi_(QString::fromUtf8("Area:")));
    tools_calculators_group_area_label->setToolTip(trUi_(QString::fromUtf8("This is the board area.")));
    lay176->addWidget(tools_calculators_group_area_label, 10, 0, 1, 1);
    tools_calculators_group_area_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_area_entry->setProperty("decimals", 4);
    tools_calculators_group_area_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); tools_calculators_group_area_entry->setSizePolicy(sp); }
    tools_calculators_group_area_entry->setProperty("value", 100.0);
    lay176->addWidget(tools_calculators_group_area_entry, 10, 1, 1, 1);
    tools_calculators_group_cdensity_label = new FCLabel(nullptr);
    tools_calculators_group_cdensity_label->setText(trUi_(QString::fromUtf8("Current Density:")));
    tools_calculators_group_cdensity_label->setToolTip(trUi_(QString::fromUtf8("Current density to pass through the board. \nIn Amps per Square Feet ASF.")));
    lay176->addWidget(tools_calculators_group_cdensity_label, 12, 0, 1, 1);
    tools_calculators_group_cdensity_entry = new FCDoubleSpinner(nullptr);
    tools_calculators_group_cdensity_entry->setProperty("decimals", 4);
    tools_calculators_group_cdensity_entry->setProperty("maximum", 10000.0);
    tools_calculators_group_cdensity_entry->setProperty("singleStep", 0.1);
    tools_calculators_group_cdensity_entry->setProperty("value", 13.0);
    lay176->addWidget(tools_calculators_group_cdensity_entry, 12, 1, 1, 1);
    tools_calculators_group_growth_label = new FCLabel(nullptr);
    tools_calculators_group_growth_label->setText(trUi_(QString::fromUtf8("Copper Growth:")));
    tools_calculators_group_growth_label->setToolTip(trUi_(QString::fromUtf8("How thick the copper growth is intended to be.\nIn microns.")));
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
    tools_sub_group_sublabel->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools_sub_group_sublabel->setToolTip(trUi_(QString::fromUtf8("A tool to substract one Gerber or Geometry object\nfrom another of the same type.")));
    tools_sub_group_layout->addWidget(tools_sub_group_sublabel, 0);
    tools_sub_group_close_paths_cb = new FCCheckBox(nullptr);
    tools_sub_group_close_paths_cb->setChecked(true);
    tools_sub_group_close_paths_cb->setText(trUi_(QString::fromUtf8("Close paths")));
    tools_sub_group_close_paths_cb->setToolTip(trUi_(QString::fromUtf8("Checking this will close the paths cut by the subtractor object.")));
    tools_sub_group_layout->addWidget(tools_sub_group_close_paths_cb, 0);
    tools_sub_group_delete_sources_cb = new FCCheckBox(nullptr);
    tools_sub_group_delete_sources_cb->setText(trUi_(QString::fromUtf8("Delete source")));
    tools_sub_group_delete_sources_cb->setToolTip(trUi_(QString::fromUtf8("When checked will delete the source objects\nafter a successful operation.")));
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
    pref_tab_area->addTab(tools_tab, trUi_(QString::fromUtf8("TOOLS")));
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
    tools2_checkrules_group_crlabel->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_checkrules_group_crlabel->setToolTip(trUi_(QString::fromUtf8("A tool to check if Gerber files are within a set\nof Manufacturing Rules.")));
    tools2_checkrules_group_layout->addWidget(tools2_checkrules_group_crlabel, 0);
    tools2_checkrules_group_form_layout_1 = new QFormLayout();
    tools2_checkrules_group_form_layout_1->setContentsMargins(0, 0, 0, 0);
    tools2_checkrules_group_form_layout_1->setHorizontalSpacing(6);
    tools2_checkrules_group_form_layout_1->setVerticalSpacing(6);
    tools2_checkrules_group_trace_size_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_trace_size_cb->setChecked(true);
    tools2_checkrules_group_trace_size_cb->setText(trUi_(QString::fromUtf8("Trace Size:")));
    tools2_checkrules_group_trace_size_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum size for traces is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(0, QFormLayout::ItemRole(2), tools2_checkrules_group_trace_size_cb);
    tools2_checkrules_group_trace_size_lbl = new QLabel(nullptr);
    tools2_checkrules_group_trace_size_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_trace_size_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable trace size.")));
    tools2_checkrules_group_form_layout_1->setWidget(1, QFormLayout::ItemRole(0), tools2_checkrules_group_trace_size_lbl);
    tools2_checkrules_group_trace_size_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_trace_size_entry->setProperty("decimals", 4);
    tools2_checkrules_group_trace_size_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_trace_size_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_trace_size_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(1, QFormLayout::ItemRole(1), tools2_checkrules_group_trace_size_entry);
    tools2_checkrules_group_clearance_copper2copper_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_copper2copper_cb->setChecked(true);
    tools2_checkrules_group_clearance_copper2copper_cb->setText(trUi_(QString::fromUtf8("Copper to Copper clearance:")));
    tools2_checkrules_group_clearance_copper2copper_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between copper\nfeatures is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(2, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_copper2copper_cb);
    tools2_checkrules_group_clearance_copper2copper_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_copper2copper_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_copper2copper_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
    tools2_checkrules_group_form_layout_1->setWidget(3, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_copper2copper_lbl);
    tools2_checkrules_group_clearance_copper2copper_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_copper2copper_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(3, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_copper2copper_entry);
    tools2_checkrules_group_clearance_copper2ol_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_copper2ol_cb->setChecked(true);
    tools2_checkrules_group_clearance_copper2ol_cb->setText(trUi_(QString::fromUtf8("Copper to Outline clearance:")));
    tools2_checkrules_group_clearance_copper2ol_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between copper\nfeatures and the outline is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(4, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_copper2ol_cb);
    tools2_checkrules_group_clearance_copper2ol_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_copper2ol_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_copper2ol_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
    tools2_checkrules_group_form_layout_1->setWidget(5, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_copper2ol_lbl);
    tools2_checkrules_group_clearance_copper2ol_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_copper2ol_entry->setProperty("value", 1.0);
    tools2_checkrules_group_form_layout_1->setWidget(5, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_copper2ol_entry);
    tools2_checkrules_group_clearance_silk2silk_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_silk2silk_cb->setChecked(true);
    tools2_checkrules_group_clearance_silk2silk_cb->setText(trUi_(QString::fromUtf8("Silk to Silk Clearance:")));
    tools2_checkrules_group_clearance_silk2silk_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between silkscreen\nfeatures and silkscreen features is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(6, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_silk2silk_cb);
    tools2_checkrules_group_clearance_silk2silk_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_silk2silk_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_silk2silk_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
    tools2_checkrules_group_form_layout_1->setWidget(7, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_silk2silk_lbl);
    tools2_checkrules_group_clearance_silk2silk_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_silk2silk_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(7, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_silk2silk_entry);
    tools2_checkrules_group_clearance_silk2sm_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_silk2sm_cb->setChecked(true);
    tools2_checkrules_group_clearance_silk2sm_cb->setText(trUi_(QString::fromUtf8("Silk to Solder Mask Clearance:")));
    tools2_checkrules_group_clearance_silk2sm_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between silkscreen\nfeatures and soldermask features is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(8, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_silk2sm_cb);
    tools2_checkrules_group_clearance_silk2sm_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_silk2sm_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_silk2sm_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
    tools2_checkrules_group_form_layout_1->setWidget(9, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_silk2sm_lbl);
    tools2_checkrules_group_clearance_silk2sm_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_silk2sm_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(9, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_silk2sm_entry);
    tools2_checkrules_group_clearance_silk2ol_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_silk2ol_cb->setChecked(true);
    tools2_checkrules_group_clearance_silk2ol_cb->setText(trUi_(QString::fromUtf8("Silk to Outline Clearance:")));
    tools2_checkrules_group_clearance_silk2ol_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between silk\nfeatures and the outline is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(10, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_silk2ol_cb);
    tools2_checkrules_group_clearance_silk2ol_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_silk2ol_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_silk2ol_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
    tools2_checkrules_group_form_layout_1->setWidget(11, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_silk2ol_lbl);
    tools2_checkrules_group_clearance_silk2ol_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_silk2ol_entry->setProperty("value", 1.0);
    tools2_checkrules_group_form_layout_1->setWidget(11, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_silk2ol_entry);
    tools2_checkrules_group_clearance_sm2sm_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_clearance_sm2sm_cb->setChecked(true);
    tools2_checkrules_group_clearance_sm2sm_cb->setText(trUi_(QString::fromUtf8("Minimum Solder Mask Sliver:")));
    tools2_checkrules_group_clearance_sm2sm_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between soldermask\nfeatures and soldermask features is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(12, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_sm2sm_cb);
    tools2_checkrules_group_clearance_sm2sm_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_sm2sm_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_sm2sm_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
    tools2_checkrules_group_form_layout_1->setWidget(13, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_sm2sm_lbl);
    tools2_checkrules_group_clearance_sm2sm_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_sm2sm_entry->setProperty("value", 0.25);
    tools2_checkrules_group_form_layout_1->setWidget(13, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_sm2sm_entry);
    tools2_checkrules_group_ring_integrity_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_ring_integrity_cb->setChecked(true);
    tools2_checkrules_group_ring_integrity_cb->setText(trUi_(QString::fromUtf8("Minimum Annular Ring:")));
    tools2_checkrules_group_ring_integrity_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum copper ring left by drilling\na hole into a pad is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(14, QFormLayout::ItemRole(2), tools2_checkrules_group_ring_integrity_cb);
    tools2_checkrules_group_ring_integrity_lbl = new QLabel(nullptr);
    tools2_checkrules_group_ring_integrity_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_ring_integrity_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable ring value.")));
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
    tools2_checkrules_group_clearance_d2d_cb->setText(trUi_(QString::fromUtf8("Hole to Hole Clearance:")));
    tools2_checkrules_group_clearance_d2d_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the minimum clearance between a drill hole\nand another drill hole is met.")));
    tools2_checkrules_group_form_layout_1->setWidget(17, QFormLayout::ItemRole(2), tools2_checkrules_group_clearance_d2d_cb);
    tools2_checkrules_group_clearance_d2d_lbl = new QLabel(nullptr);
    tools2_checkrules_group_clearance_d2d_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_clearance_d2d_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable drill size.")));
    tools2_checkrules_group_form_layout_1->setWidget(18, QFormLayout::ItemRole(0), tools2_checkrules_group_clearance_d2d_lbl);
    tools2_checkrules_group_clearance_d2d_entry = new FCDoubleSpinner(nullptr);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("decimals", 4);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("maximum", 1000.0);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("singleStep", 0.1);
    tools2_checkrules_group_clearance_d2d_entry->setProperty("value", 0.3);
    tools2_checkrules_group_form_layout_1->setWidget(18, QFormLayout::ItemRole(1), tools2_checkrules_group_clearance_d2d_entry);
    tools2_checkrules_group_drill_size_cb = new FCCheckBox(nullptr);
    tools2_checkrules_group_drill_size_cb->setChecked(true);
    tools2_checkrules_group_drill_size_cb->setText(trUi_(QString::fromUtf8("Hole Size:")));
    tools2_checkrules_group_drill_size_cb->setToolTip(trUi_(QString::fromUtf8("This checks if the drill holes\nsizes are above the threshold.")));
    tools2_checkrules_group_form_layout_1->setWidget(19, QFormLayout::ItemRole(2), tools2_checkrules_group_drill_size_cb);
    tools2_checkrules_group_drill_size_lbl = new QLabel(nullptr);
    tools2_checkrules_group_drill_size_lbl->setText(trUi_(QString::fromUtf8("Min value:")));
    tools2_checkrules_group_drill_size_lbl->setToolTip(trUi_(QString::fromUtf8("Minimum acceptable clearance value.")));
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
    tools2_optimal_group_optlabel->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_optimal_group_optlabel->setToolTip(trUi_(QString::fromUtf8("A tool to find the minimum distance between\nevery two Gerber geometric elements")));
    tools2_optimal_group_layout->addWidget(tools2_optimal_group_optlabel, 0);
    auto* lay178 = new QGridLayout();
    lay178->setContentsMargins(0, 0, 0, 0);
    lay178->setHorizontalSpacing(6);
    lay178->setVerticalSpacing(6);
    tools2_optimal_group_precision_lbl = new QLabel(nullptr);
    tools2_optimal_group_precision_lbl->setText(trUi_(QString::fromUtf8("Precision:")));
    tools2_optimal_group_precision_lbl->setToolTip(trUi_(QString::fromUtf8("Number of decimals for the distances and coordinates in this tool.")));
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
    tools2_qrcode_group_qrlabel->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_qrcode_group_qrlabel->setToolTip(trUi_(QString::fromUtf8("A tool to create a QRCode that can be inserted\ninto a selected Gerber file, or it can be exported as a file.")));
    tools2_qrcode_group_layout->addWidget(tools2_qrcode_group_qrlabel, 0);
    auto* lay179 = new QGridLayout();
    lay179->setContentsMargins(0, 0, 0, 0);
    lay179->setHorizontalSpacing(6);
    lay179->setVerticalSpacing(6);
    tools2_qrcode_group_version_label = new QLabel(nullptr);
    tools2_qrcode_group_version_label->setText(trUi_(QString::fromUtf8("Version:")));
    tools2_qrcode_group_version_label->setToolTip(trUi_(QString::fromUtf8("QRCode version can have values from 1 (21x21 boxes)\nto 40 (177x177 boxes).")));
    lay179->addWidget(tools2_qrcode_group_version_label, 1, 0, 1, 1);
    tools2_qrcode_group_version_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_version_entry->setProperty("maximum", 40);
    tools2_qrcode_group_version_entry->setProperty("minimum", 1);
    tools2_qrcode_group_version_entry->setProperty("value", 1);
    tools2_qrcode_group_version_entry->setWrapping(true);
    lay179->addWidget(tools2_qrcode_group_version_entry, 1, 1, 1, 1);
    tools2_qrcode_group_error_label = new QLabel(nullptr);
    tools2_qrcode_group_error_label->setText(trUi_(QString::fromUtf8("Error correction:")));
    tools2_qrcode_group_error_label->setToolTip(trUi_(QString::fromUtf8("Parameter that controls the error correction used for the QR Code.\nL = maximum 7%% errors can be corrected\nM = maximum 15%% errors can be corrected\nQ = maximum 25%% errors can be corrected\nH = maximum 30%% errors can be corrected.")));
    lay179->addWidget(tools2_qrcode_group_error_label, 2, 0, 1, 1);
    tools2_qrcode_group_error_radio = new RadioSet({{trUi_(QString::fromUtf8("L")), QVariant(QString::fromUtf8("L"))}, {trUi_(QString::fromUtf8("M")), QVariant(QString::fromUtf8("M"))}, {trUi_(QString::fromUtf8("Q")), QVariant(QString::fromUtf8("Q"))}, {trUi_(QString::fromUtf8("H")), QVariant(QString::fromUtf8("H"))}}, Qt::Horizontal, true, nullptr);
    tools2_qrcode_group_error_radio->setToolTip(trUi_(QString::fromUtf8("Parameter that controls the error correction used for the QR Code.\nL = maximum 7%% errors can be corrected\nM = maximum 15%% errors can be corrected\nQ = maximum 25%% errors can be corrected\nH = maximum 30%% errors can be corrected.")));
    tools2_qrcode_group_error_radio->setValue(QVariant(QString::fromUtf8("L")));
    lay179->addWidget(tools2_qrcode_group_error_radio, 2, 1, 1, 1);
    tools2_qrcode_group_bsize_label = new QLabel(nullptr);
    tools2_qrcode_group_bsize_label->setText(trUi_(QString::fromUtf8("Box Size:")));
    tools2_qrcode_group_bsize_label->setToolTip(trUi_(QString::fromUtf8("Box size control the overall size of the QRcode\nby adjusting the size of each box in the code.")));
    lay179->addWidget(tools2_qrcode_group_bsize_label, 3, 0, 1, 1);
    tools2_qrcode_group_bsize_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_bsize_entry->setProperty("maximum", 9999);
    tools2_qrcode_group_bsize_entry->setProperty("minimum", 1);
    tools2_qrcode_group_bsize_entry->setProperty("value", 3);
    tools2_qrcode_group_bsize_entry->setWrapping(true);
    lay179->addWidget(tools2_qrcode_group_bsize_entry, 3, 1, 1, 1);
    tools2_qrcode_group_border_size_label = new QLabel(nullptr);
    tools2_qrcode_group_border_size_label->setText(trUi_(QString::fromUtf8("Border Size:")));
    tools2_qrcode_group_border_size_label->setToolTip(trUi_(QString::fromUtf8("Size of the QRCode border. How many boxes thick is the border.\nDefault value is 4. The width of the clearance around the QRCode.")));
    lay179->addWidget(tools2_qrcode_group_border_size_label, 4, 0, 1, 1);
    tools2_qrcode_group_border_size_entry = new FCSpinner(nullptr);
    tools2_qrcode_group_border_size_entry->setProperty("maximum", 9999);
    tools2_qrcode_group_border_size_entry->setProperty("minimum", 1);
    tools2_qrcode_group_border_size_entry->setProperty("value", 4);
    tools2_qrcode_group_border_size_entry->setWrapping(true);
    lay179->addWidget(tools2_qrcode_group_border_size_entry, 4, 1, 1, 1);
    tools2_qrcode_group_text_label = new QLabel(nullptr);
    tools2_qrcode_group_text_label->setText(trUi_(QString::fromUtf8("QRCode Data:")));
    tools2_qrcode_group_text_label->setToolTip(trUi_(QString::fromUtf8("QRCode Data. Alphanumeric text to be encoded in the QRCode.")));
    lay179->addWidget(tools2_qrcode_group_text_label, 5, 0, 1, 1);
    tools2_qrcode_group_text_data = new FCTextArea(nullptr);
    tools2_qrcode_group_text_data->setProperty("placeholderText", trUi_(QString::fromUtf8("Add here the text to be included in the QRCode...")));
    lay179->addWidget(tools2_qrcode_group_text_data, 6, 0, 1, 2);
    tools2_qrcode_group_pol_label = new QLabel(nullptr);
    tools2_qrcode_group_pol_label->setText(trUi_(QString::fromUtf8("Polarity:")));
    tools2_qrcode_group_pol_label->setToolTip(trUi_(QString::fromUtf8("Choose the polarity of the QRCode.\nIt can be drawn in a negative way (squares are clear)\nor in a positive way (squares are opaque).")));
    lay179->addWidget(tools2_qrcode_group_pol_label, 7, 0, 1, 1);
    tools2_qrcode_group_pol_radio = new RadioSet({{trUi_(QString::fromUtf8("Negative")), QVariant(QString::fromUtf8("neg"))}, {trUi_(QString::fromUtf8("Positive")), QVariant(QString::fromUtf8("pos"))}}, Qt::Horizontal, true, nullptr);
    tools2_qrcode_group_pol_radio->setToolTip(trUi_(QString::fromUtf8("Choose the type of QRCode to be created.\nIf added on a Silkscreen Gerber file the QRCode may\nbe added as positive. If it is added to a Copper Gerber\nfile then perhaps the QRCode can be added as negative.")));
    tools2_qrcode_group_pol_radio->setValue(QVariant(QString::fromUtf8("pos")));
    lay179->addWidget(tools2_qrcode_group_pol_radio, 7, 1, 1, 1);
    tools2_qrcode_group_bb_label = new QLabel(nullptr);
    tools2_qrcode_group_bb_label->setText(trUi_(QString::fromUtf8("Bounding Box:")));
    tools2_qrcode_group_bb_label->setToolTip(trUi_(QString::fromUtf8("The bounding box, meaning the empty space that surrounds\nthe QRCode geometry, can have a rounded or a square shape.")));
    lay179->addWidget(tools2_qrcode_group_bb_label, 8, 0, 1, 1);
    tools2_qrcode_group_bb_radio = new RadioSet({{trUi_(QString::fromUtf8("Rounded")), QVariant(QString::fromUtf8("r"))}, {trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("s"))}}, Qt::Horizontal, true, nullptr);
    tools2_qrcode_group_bb_radio->setToolTip(trUi_(QString::fromUtf8("The bounding box, meaning the empty space that surrounds\nthe QRCode geometry, can have a rounded or a square shape.")));
    tools2_qrcode_group_bb_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay179->addWidget(tools2_qrcode_group_bb_radio, 8, 1, 1, 1);
    tools2_qrcode_group_fill_color_label = new QLabel(nullptr);
    tools2_qrcode_group_fill_color_label->setText(trUi_(QString::fromUtf8("Fill Color:")));
    tools2_qrcode_group_fill_color_label->setToolTip(trUi_(QString::fromUtf8("Set the QRCode fill color (squares color).")));
    lay179->addWidget(tools2_qrcode_group_fill_color_label, 9, 0, 1, 1);
    tools2_qrcode_group_fill_color_entry = new FCColorEntry(nullptr);
    tools2_qrcode_group_fill_color_entry->setValue(QString::fromUtf8("#000000"));
    lay179->addWidget(tools2_qrcode_group_fill_color_entry, 9, 1, 1, 1);
    tools2_qrcode_group_back_color_label = new QLabel(nullptr);
    tools2_qrcode_group_back_color_label->setText(trUi_(QString::fromUtf8("Back Color:")));
    tools2_qrcode_group_back_color_label->setToolTip(trUi_(QString::fromUtf8("Set the QRCode background color.")));
    lay179->addWidget(tools2_qrcode_group_back_color_label, 10, 0, 1, 1);
    tools2_qrcode_group_back_color_entry = new FCColorEntry(nullptr);
    tools2_qrcode_group_back_color_entry->setValue(QString::fromUtf8("#FFFFFF"));
    lay179->addWidget(tools2_qrcode_group_back_color_entry, 10, 1, 1, 1);
    tools2_qrcode_group_sel_limit_label = new QLabel(nullptr);
    tools2_qrcode_group_sel_limit_label->setText(trUi_(QString::fromUtf8("Selection limit:")));
    tools2_qrcode_group_sel_limit_label->setToolTip(trUi_(QString::fromUtf8("Set the number of selected geometry\nitems above which the utility geometry\nbecomes just a selection rectangle.\nIncreases the performance when moving a\nlarge number of geometric elements.")));
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
    tools2_fiducials_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_fiducials_group_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay180->addWidget(tools2_fiducials_group_param_label, 0, 0, 1, 2);
    tools2_fiducials_group_dia_label = new QLabel(nullptr);
    tools2_fiducials_group_dia_label->setText(trUi_(QString::fromUtf8("Size:")));
    tools2_fiducials_group_dia_label->setToolTip(trUi_(QString::fromUtf8("This set the fiducial diameter if fiducial type is circular,\notherwise is the size of the fiducial.\nThe soldermask opening is double than that.")));
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
    tools2_fiducials_group_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    tools2_fiducials_group_margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay180->addWidget(tools2_fiducials_group_margin_label, 2, 0, 1, 1);
    tools2_fiducials_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_fiducials_group_margin_entry->setProperty("decimals", 4);
    tools2_fiducials_group_margin_entry->setProperty("maximum", 10000.0);
    tools2_fiducials_group_margin_entry->setProperty("minimum", -10000.0);
    tools2_fiducials_group_margin_entry->setProperty("singleStep", 0.1);
    tools2_fiducials_group_margin_entry->setProperty("value", 1.0);
    lay180->addWidget(tools2_fiducials_group_margin_entry, 2, 1, 1, 1);
    tools2_fiducials_group_mode_label = new QLabel(nullptr);
    tools2_fiducials_group_mode_label->setText(trUi_(QString::fromUtf8("Mode:")));
    tools2_fiducials_group_mode_label->setToolTip(trUi_(QString::fromUtf8("- 'Auto' - automatic placement of fiducials in the corners of the bounding box.\n- 'Manual' - manual placement of fiducials.")));
    lay180->addWidget(tools2_fiducials_group_mode_label, 3, 0, 1, 1);
    tools2_fiducials_group_mode_radio = new RadioSet({{trUi_(QString::fromUtf8("Auto")), QVariant(QString::fromUtf8("auto"))}, {trUi_(QString::fromUtf8("Manual")), QVariant(QString::fromUtf8("manual"))}}, Qt::Horizontal, false, nullptr);
    tools2_fiducials_group_mode_radio->setValue(QVariant(QString::fromUtf8("auto")));
    lay180->addWidget(tools2_fiducials_group_mode_radio, 3, 1, 1, 1);
    tools2_fiducials_group_pos_label = new QLabel(nullptr);
    tools2_fiducials_group_pos_label->setText(trUi_(QString::fromUtf8("Second fiducial:")));
    tools2_fiducials_group_pos_label->setToolTip(trUi_(QString::fromUtf8("The position for the second fiducial.\n- 'Up' - the order is: bottom-left, top-left, top-right.\n- 'Down' - the order is: bottom-left, bottom-right, top-right.\n- 'None' - there is no second fiducial. The order is: bottom-left, top-right.")));
    lay180->addWidget(tools2_fiducials_group_pos_label, 4, 0, 1, 1);
    tools2_fiducials_group_pos_radio = new RadioSet({{trUi_(QString::fromUtf8("Up")), QVariant(QString::fromUtf8("up"))}, {trUi_(QString::fromUtf8("Down")), QVariant(QString::fromUtf8("down"))}, {trUi_(QString::fromUtf8("None")), QVariant(QString::fromUtf8("no"))}}, Qt::Horizontal, false, nullptr);
    tools2_fiducials_group_pos_radio->setValue(QVariant(QString::fromUtf8("up")));
    lay180->addWidget(tools2_fiducials_group_pos_radio, 4, 1, 1, 1);
    auto* w181 = new QFrame(nullptr);
    w181->setProperty("frameShadow", 48);
    w181->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w181->setSizePolicy(sp); }
    lay180->addWidget(w181, 5, 0, 1, 2);
    tools2_fiducials_group_fid_type_label = new QLabel(nullptr);
    tools2_fiducials_group_fid_type_label->setText(trUi_(QString::fromUtf8("Fiducial Type:")));
    tools2_fiducials_group_fid_type_label->setToolTip(trUi_(QString::fromUtf8("The type of fiducial.\n- 'Circular' - this is the regular fiducial.\n- 'Cross' - cross lines fiducial.\n- 'Chess' - chess pattern fiducial.")));
    lay180->addWidget(tools2_fiducials_group_fid_type_label, 6, 0, 1, 1);
    tools2_fiducials_group_fid_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Circular")), QVariant(QString::fromUtf8("circular"))}, {trUi_(QString::fromUtf8("Cross")), QVariant(QString::fromUtf8("cross"))}, {trUi_(QString::fromUtf8("Chess")), QVariant(QString::fromUtf8("chess"))}}, Qt::Horizontal, false, nullptr);
    tools2_fiducials_group_fid_type_radio->setValue(QVariant(QString::fromUtf8("circular")));
    lay180->addWidget(tools2_fiducials_group_fid_type_radio, 6, 1, 1, 1);
    tools2_fiducials_group_line_thickness_label = new QLabel(nullptr);
    tools2_fiducials_group_line_thickness_label->setText(trUi_(QString::fromUtf8("Line thickness:")));
    tools2_fiducials_group_line_thickness_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
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
    tools2_cfill_group_cflabel->setText(trUi_(QString::fromUtf8("<b>Parameters</b>")));
    tools2_cfill_group_cflabel->setToolTip(trUi_(QString::fromUtf8("A tool to generate a Copper Thieving that can be added\nto a selected Gerber file.")));
    lay182->addWidget(tools2_cfill_group_cflabel, 0, 0, 1, 2);
    tools2_cfill_group_circle_steps_lbl = new FCLabel(nullptr);
    tools2_cfill_group_circle_steps_lbl->setText(trUi_(QString::fromUtf8("Circle Steps:")));
    tools2_cfill_group_circle_steps_lbl->setToolTip(trUi_(QString::fromUtf8("Number of steps (lines) used to interpolate circles.")));
    lay182->addWidget(tools2_cfill_group_circle_steps_lbl, 2, 0, 1, 1);
    tools2_cfill_group_circlesteps_entry = new FCSpinner(nullptr);
    tools2_cfill_group_circlesteps_entry->setProperty("maximum", 9999);
    tools2_cfill_group_circlesteps_entry->setProperty("minimum", 1);
    tools2_cfill_group_circlesteps_entry->setProperty("value", 64);
    lay182->addWidget(tools2_cfill_group_circlesteps_entry, 2, 1, 1, 1);
    tools2_cfill_group_clearance_label = new FCLabel(nullptr);
    tools2_cfill_group_clearance_label->setText(trUi_(QString::fromUtf8("Clearance:")));
    tools2_cfill_group_clearance_label->setToolTip(trUi_(QString::fromUtf8("This set the distance between the copper Thieving components\n(the polygon fill may be split in multiple polygons)\nand the copper traces in the Gerber file.")));
    lay182->addWidget(tools2_cfill_group_clearance_label, 4, 0, 1, 1);
    tools2_cfill_group_clearance_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_clearance_entry->setProperty("decimals", 4);
    tools2_cfill_group_clearance_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_clearance_entry->setProperty("value", 0.25);
    lay182->addWidget(tools2_cfill_group_clearance_entry, 4, 1, 1, 1);
    tools2_cfill_group_margin_label = new FCLabel(nullptr);
    tools2_cfill_group_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    tools2_cfill_group_margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay182->addWidget(tools2_cfill_group_margin_label, 6, 0, 1, 1);
    tools2_cfill_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_margin_entry->setProperty("decimals", 4);
    tools2_cfill_group_margin_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_margin_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_margin_entry, 6, 1, 1, 1);
    tools2_cfill_group_area_label = new FCLabel(nullptr);
    tools2_cfill_group_area_label->setText(trUi_(QString::fromUtf8("Area:")));
    tools2_cfill_group_area_label->setToolTip(trUi_(QString::fromUtf8("Thieving areas with area less then this value will not be added.")));
    lay182->addWidget(tools2_cfill_group_area_label, 8, 0, 1, 1);
    tools2_cfill_group_area_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_area_entry->setProperty("decimals", 4);
    tools2_cfill_group_area_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_area_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_area_entry->setProperty("value", 0.1);
    lay182->addWidget(tools2_cfill_group_area_entry, 8, 1, 1, 1);
    tools2_cfill_group_reference_label = new FCLabel(nullptr);
    tools2_cfill_group_reference_label->setText(trUi_(QString::fromUtf8("Reference:")));
    tools2_cfill_group_reference_label->setToolTip(trUi_(QString::fromUtf8("- 'Itself' - the copper thieving extent is based on the object extent.\n- 'Area Selection' - left mouse click to start selection of the area to be filled.\n- 'Reference Object' - will do copper thieving within the area specified by another object.")));
    lay182->addWidget(tools2_cfill_group_reference_label, 10, 0, 1, 1);
    tools2_cfill_group_reference_radio = new RadioSet({{trUi_(QString::fromUtf8("Itself")), QVariant(QString::fromUtf8("itself"))}, {trUi_(QString::fromUtf8("Area Selection")), QVariant(QString::fromUtf8("area"))}, {trUi_(QString::fromUtf8("Reference Object")), QVariant(QString::fromUtf8("box"))}}, Qt::Vertical, false, nullptr);
    tools2_cfill_group_reference_radio->setValue(QVariant(QString::fromUtf8("itself")));
    lay182->addWidget(tools2_cfill_group_reference_radio, 10, 1, 1, 1);
    tools2_cfill_group_bbox_type_label = new FCLabel(nullptr);
    tools2_cfill_group_bbox_type_label->setText(trUi_(QString::fromUtf8("Box Type:")));
    tools2_cfill_group_bbox_type_label->setToolTip(trUi_(QString::fromUtf8("- 'Rectangular' - the bounding box will be of rectangular shape.\n- 'Minimal' - the bounding box will be the convex hull shape.")));
    lay182->addWidget(tools2_cfill_group_bbox_type_label, 12, 0, 1, 1);
    tools2_cfill_group_bbox_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Rectangular")), QVariant(QString::fromUtf8("rect"))}, {trUi_(QString::fromUtf8("Minimal")), QVariant(QString::fromUtf8("min"))}}, Qt::Horizontal, false, nullptr);
    tools2_cfill_group_bbox_type_radio->setValue(QVariant(QString::fromUtf8("rect")));
    lay182->addWidget(tools2_cfill_group_bbox_type_radio, 12, 1, 1, 1);
    auto* w183 = new QFrame(nullptr);
    w183->setProperty("frameShadow", 48);
    w183->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w183->setSizePolicy(sp); }
    lay182->addWidget(w183, 14, 0, 1, 2);
    tools2_cfill_group_fill_type_label = new FCLabel(nullptr);
    tools2_cfill_group_fill_type_label->setText(trUi_(QString::fromUtf8("Fill Type:")));
    tools2_cfill_group_fill_type_label->setToolTip(trUi_(QString::fromUtf8("- 'Solid' - copper thieving will be a solid polygon.\n- 'Dots Grid' - the empty area will be filled with a pattern of dots.\n- 'Squares Grid' - the empty area will be filled with a pattern of squares.\n- 'Lines Grid' - the empty area will be filled with a pattern of lines.")));
    lay182->addWidget(tools2_cfill_group_fill_type_label, 16, 0, 1, 1);
    tools2_cfill_group_fill_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Solid")), QVariant(QString::fromUtf8("solid"))}, {trUi_(QString::fromUtf8("Dots Grid")), QVariant(QString::fromUtf8("dot"))}, {trUi_(QString::fromUtf8("Squares Grid")), QVariant(QString::fromUtf8("square"))}, {trUi_(QString::fromUtf8("Lines Grid")), QVariant(QString::fromUtf8("line"))}}, Qt::Vertical, false, nullptr);
    tools2_cfill_group_fill_type_radio->setValue(QVariant(QString::fromUtf8("solid")));
    lay182->addWidget(tools2_cfill_group_fill_type_radio, 16, 1, 1, 1);
    tools2_cfill_group_dots_label = new FCLabel(nullptr);
    tools2_cfill_group_dots_label->setText(trUi_(QString::fromUtf8("<b>Dots Grid Parameters</b>:")));
    lay182->addWidget(tools2_cfill_group_dots_label, 18, 0, 1, 2);
    tools2_cfill_group_dotdia_label = new FCLabel(nullptr);
    tools2_cfill_group_dotdia_label->setText(trUi_(QString::fromUtf8("Dia:")));
    tools2_cfill_group_dotdia_label->setToolTip(trUi_(QString::fromUtf8("Dot diameter in Dots Grid.")));
    lay182->addWidget(tools2_cfill_group_dotdia_label, 20, 0, 1, 1);
    tools2_cfill_group_dot_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_dot_dia_entry->setProperty("decimals", 4);
    tools2_cfill_group_dot_dia_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_dot_dia_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_dot_dia_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_dot_dia_entry, 20, 1, 1, 1);
    tools2_cfill_group_dotspacing_label = new FCLabel(nullptr);
    tools2_cfill_group_dotspacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    tools2_cfill_group_dotspacing_label->setToolTip(trUi_(QString::fromUtf8("Distance between each two dots in Dots Grid.")));
    lay182->addWidget(tools2_cfill_group_dotspacing_label, 22, 0, 1, 1);
    tools2_cfill_group_dot_spacing_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_dot_spacing_entry->setProperty("decimals", 4);
    tools2_cfill_group_dot_spacing_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_dot_spacing_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_dot_spacing_entry->setProperty("value", 2.0);
    lay182->addWidget(tools2_cfill_group_dot_spacing_entry, 22, 1, 1, 1);
    tools2_cfill_group_squares_label = new FCLabel(nullptr);
    tools2_cfill_group_squares_label->setText(trUi_(QString::fromUtf8("<b>Squares Grid Parameters</b>:")));
    lay182->addWidget(tools2_cfill_group_squares_label, 24, 0, 1, 2);
    tools2_cfill_group_square_size_label = new FCLabel(nullptr);
    tools2_cfill_group_square_size_label->setText(trUi_(QString::fromUtf8("Size:")));
    tools2_cfill_group_square_size_label->setToolTip(trUi_(QString::fromUtf8("Square side size in Squares Grid.")));
    lay182->addWidget(tools2_cfill_group_square_size_label, 26, 0, 1, 1);
    tools2_cfill_group_square_size_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_square_size_entry->setProperty("decimals", 4);
    tools2_cfill_group_square_size_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_square_size_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_square_size_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_square_size_entry, 26, 1, 1, 1);
    tools2_cfill_group_squares_spacing_label = new FCLabel(nullptr);
    tools2_cfill_group_squares_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    tools2_cfill_group_squares_spacing_label->setToolTip(trUi_(QString::fromUtf8("Distance between each two squares in Squares Grid.")));
    lay182->addWidget(tools2_cfill_group_squares_spacing_label, 28, 0, 1, 1);
    tools2_cfill_group_squares_spacing_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_squares_spacing_entry->setProperty("decimals", 4);
    tools2_cfill_group_squares_spacing_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_squares_spacing_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_squares_spacing_entry->setProperty("value", 2.0);
    lay182->addWidget(tools2_cfill_group_squares_spacing_entry, 28, 1, 1, 1);
    tools2_cfill_group_lines_label = new FCLabel(nullptr);
    tools2_cfill_group_lines_label->setText(trUi_(QString::fromUtf8("<b>Lines Grid Parameters</b>:")));
    lay182->addWidget(tools2_cfill_group_lines_label, 30, 0, 1, 2);
    tools2_cfill_group_line_size_label = new FCLabel(nullptr);
    tools2_cfill_group_line_size_label->setText(trUi_(QString::fromUtf8("Size:")));
    tools2_cfill_group_line_size_label->setToolTip(trUi_(QString::fromUtf8("Line thickness size in Lines Grid.")));
    lay182->addWidget(tools2_cfill_group_line_size_label, 32, 0, 1, 1);
    tools2_cfill_group_line_size_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_line_size_entry->setProperty("decimals", 4);
    tools2_cfill_group_line_size_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_line_size_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_line_size_entry->setProperty("value", 0.25);
    lay182->addWidget(tools2_cfill_group_line_size_entry, 32, 1, 1, 1);
    tools2_cfill_group_lines_spacing_label = new FCLabel(nullptr);
    tools2_cfill_group_lines_spacing_label->setText(trUi_(QString::fromUtf8("Spacing:")));
    tools2_cfill_group_lines_spacing_label->setToolTip(trUi_(QString::fromUtf8("Distance between each two lines in Lines Grid.")));
    lay182->addWidget(tools2_cfill_group_lines_spacing_label, 34, 0, 1, 1);
    tools2_cfill_group_lines_spacing_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_lines_spacing_entry->setProperty("decimals", 4);
    tools2_cfill_group_lines_spacing_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_lines_spacing_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_lines_spacing_entry->setProperty("value", 2.0);
    lay182->addWidget(tools2_cfill_group_lines_spacing_entry, 34, 1, 1, 1);
    tools2_cfill_group_robber_bar_label = new FCLabel(nullptr);
    tools2_cfill_group_robber_bar_label->setText(trUi_(QString::fromUtf8("<b>Robber Bar Parameters</b>")));
    tools2_cfill_group_robber_bar_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for the robber bar.\nRobber bar = copper border to help in pattern hole plating.")));
    lay182->addWidget(tools2_cfill_group_robber_bar_label, 36, 0, 1, 2);
    tools2_cfill_group_rb_margin_label = new FCLabel(nullptr);
    tools2_cfill_group_rb_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    tools2_cfill_group_rb_margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin for robber bar.")));
    lay182->addWidget(tools2_cfill_group_rb_margin_label, 38, 0, 1, 1);
    tools2_cfill_group_rb_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_rb_margin_entry->setProperty("decimals", 4);
    tools2_cfill_group_rb_margin_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_rb_margin_entry->setProperty("minimum", -10000.0);
    tools2_cfill_group_rb_margin_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_rb_margin_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_rb_margin_entry, 38, 1, 1, 1);
    tools2_cfill_group_rb_thickness_label = new FCLabel(nullptr);
    tools2_cfill_group_rb_thickness_label->setText(trUi_(QString::fromUtf8("Thickness:")));
    tools2_cfill_group_rb_thickness_label->setToolTip(trUi_(QString::fromUtf8("The robber bar thickness.")));
    lay182->addWidget(tools2_cfill_group_rb_thickness_label, 40, 0, 1, 1);
    tools2_cfill_group_rb_thickness_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_rb_thickness_entry->setProperty("decimals", 4);
    tools2_cfill_group_rb_thickness_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_rb_thickness_entry->setProperty("singleStep", 0.1);
    tools2_cfill_group_rb_thickness_entry->setProperty("value", 1.0);
    lay182->addWidget(tools2_cfill_group_rb_thickness_entry, 40, 1, 1, 1);
    tools2_cfill_group_patern_mask_label = new FCLabel(nullptr);
    tools2_cfill_group_patern_mask_label->setText(trUi_(QString::fromUtf8("<b>Pattern Plating Mask</b>")));
    tools2_cfill_group_patern_mask_label->setToolTip(trUi_(QString::fromUtf8("Generate a mask for pattern plating.")));
    lay182->addWidget(tools2_cfill_group_patern_mask_label, 42, 0, 1, 2);
    tools2_cfill_group_clearance_ppm_label = new FCLabel(nullptr);
    tools2_cfill_group_clearance_ppm_label->setText(trUi_(QString::fromUtf8("Clearance:")));
    tools2_cfill_group_clearance_ppm_label->setToolTip(trUi_(QString::fromUtf8("The distance between the possible copper thieving elements\nand/or robber bar and the actual openings in the mask.")));
    lay182->addWidget(tools2_cfill_group_clearance_ppm_label, 44, 0, 1, 1);
    tools2_cfill_group_clearance_ppm_entry = new FCDoubleSpinner(nullptr);
    tools2_cfill_group_clearance_ppm_entry->setProperty("decimals", 4);
    tools2_cfill_group_clearance_ppm_entry->setProperty("maximum", 10000.0);
    tools2_cfill_group_clearance_ppm_entry->setProperty("minimum", -10000.0);
    tools2_cfill_group_clearance_ppm_entry->setProperty("singleStep", 0.1);
    lay182->addWidget(tools2_cfill_group_clearance_ppm_entry, 44, 1, 1, 1);
    tools2_cfill_group_ppm_choice_label = new FCLabel(nullptr);
    tools2_cfill_group_ppm_choice_label->setText(trUi_(QString::fromUtf8("Add:")));
    tools2_cfill_group_ppm_choice_label->setToolTip(trUi_(QString::fromUtf8("Choose which additional geometry to include, if available.")));
    lay182->addWidget(tools2_cfill_group_ppm_choice_label, 46, 0, 1, 1);
    tools2_cfill_group_ppm_choice_radio = new RadioSet({{trUi_(QString::fromUtf8("Both")), QVariant(QString::fromUtf8("b"))}, {trUi_(QString::fromUtf8("Thieving")), QVariant(QString::fromUtf8("t"))}, {trUi_(QString::fromUtf8("Robber bar")), QVariant(QString::fromUtf8("r"))}, {trUi_(QString::fromUtf8("None")), QVariant(QString::fromUtf8("n"))}}, Qt::Vertical, false, nullptr);
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
    tools2_cal_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_cal_group_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay184->addWidget(tools2_cal_group_param_label, 0, 0, 1, 2);
    tools2_cal_group_cal_source_lbl = new QLabel(nullptr);
    tools2_cal_group_cal_source_lbl->setText(trUi_(QString::fromUtf8("<b>Source Type:</b>")));
    tools2_cal_group_cal_source_lbl->setToolTip(trUi_(QString::fromUtf8("The source of calibration points.\nIt can be:\n- Object -> click a hole geo for Excellon or a pad for Gerber\n- Free -> click freely on canvas to acquire the calibration points")));
    lay184->addWidget(tools2_cal_group_cal_source_lbl, 1, 0, 1, 1);
    tools2_cal_group_cal_source_radio = new RadioSet({{trUi_(QString::fromUtf8("Object")), QVariant(QString::fromUtf8("object"))}, {trUi_(QString::fromUtf8("Free")), QVariant(QString::fromUtf8("free"))}}, Qt::Horizontal, false, nullptr);
    tools2_cal_group_cal_source_radio->setValue(QVariant(QString::fromUtf8("object")));
    lay184->addWidget(tools2_cal_group_cal_source_radio, 1, 1, 1, 2);
    auto* w185 = new QFrame(nullptr);
    w185->setProperty("frameShadow", 48);
    w185->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w185->setSizePolicy(sp); }
    lay184->addWidget(w185, 2, 0, 1, 2);
    auto* w186 = new QLabel(nullptr);
    w186->setText(trUi_(QString::fromUtf8("Travel Z:")));
    w186->setToolTip(trUi_(QString::fromUtf8("Height (Z) for travelling between the points.")));
    lay184->addWidget(w186, 3, 0, 1, 1);
    tools2_cal_group_travelz_entry = new FCDoubleSpinner(nullptr);
    tools2_cal_group_travelz_entry->setProperty("decimals", 4);
    tools2_cal_group_travelz_entry->setProperty("maximum", 10000.0);
    tools2_cal_group_travelz_entry->setProperty("minimum", -10000.0);
    tools2_cal_group_travelz_entry->setProperty("singleStep", 0.1);
    tools2_cal_group_travelz_entry->setProperty("value", 2.0);
    lay184->addWidget(tools2_cal_group_travelz_entry, 3, 1, 1, 2);
    auto* w187 = new QLabel(nullptr);
    w187->setText(trUi_(QString::fromUtf8("Verification Z:")));
    w187->setToolTip(trUi_(QString::fromUtf8("Height (Z) for checking the point.")));
    lay184->addWidget(w187, 4, 0, 1, 1);
    tools2_cal_group_verz_entry = new FCDoubleSpinner(nullptr);
    tools2_cal_group_verz_entry->setProperty("decimals", 4);
    tools2_cal_group_verz_entry->setProperty("maximum", 10000.0);
    tools2_cal_group_verz_entry->setProperty("minimum", -10000.0);
    tools2_cal_group_verz_entry->setProperty("singleStep", 0.1);
    tools2_cal_group_verz_entry->setProperty("value", 0.1);
    lay184->addWidget(tools2_cal_group_verz_entry, 4, 1, 1, 2);
    tools2_cal_group_zeroz_cb = new FCCheckBox(nullptr);
    tools2_cal_group_zeroz_cb->setText(trUi_(QString::fromUtf8("Zero Z tool")));
    tools2_cal_group_zeroz_cb->setToolTip(trUi_(QString::fromUtf8("Include a sequence to zero the height (Z)\nof the verification tool.")));
    lay184->addWidget(tools2_cal_group_zeroz_cb, 5, 0, 1, 3);
    auto* w188 = new QLabel(nullptr);
    w188->setText(trUi_(QString::fromUtf8("Toolchange Z:")));
    w188->setToolTip(trUi_(QString::fromUtf8("Height (Z) for mounting the verification probe.")));
    lay184->addWidget(w188, 6, 0, 1, 1);
    tools2_cal_group_toolchangez_entry = new FCDoubleSpinner(nullptr);
    tools2_cal_group_toolchangez_entry->setProperty("decimals", 4);
    tools2_cal_group_toolchangez_entry->setProperty("maximum", 10000.0);
    tools2_cal_group_toolchangez_entry->setProperty("singleStep", 0.1);
    tools2_cal_group_toolchangez_entry->setProperty("value", 15.0);
    lay184->addWidget(tools2_cal_group_toolchangez_entry, 6, 1, 1, 2);
    auto* w189 = new QLabel(nullptr);
    w189->setText(trUi_(QString::fromUtf8("Toolchange X-Y:")));
    w189->setToolTip(trUi_(QString::fromUtf8("Toolchange X,Y position.\nIf no value is entered then the current\n(x, y) point will be used,")));
    lay184->addWidget(w189, 7, 0, 1, 1);
    tools2_cal_group_toolchange_xy_entry = new NumericalEvalTupleEntry(nullptr);
    tools2_cal_group_toolchange_xy_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay184->addWidget(tools2_cal_group_toolchange_xy_entry, 7, 1, 1, 2);
    auto* w190 = new QLabel(nullptr);
    w190->setText(trUi_(QString::fromUtf8("Second point:")));
    w190->setToolTip(trUi_(QString::fromUtf8("Second point in the Gcode verification can be:\n- top-left -> the user will align the PCB vertically\n- bottom-right -> the user will align the PCB horizontally")));
    lay184->addWidget(w190, 8, 0, 1, 1);
    tools2_cal_group_second_point_radio = new RadioSet({{trUi_(QString::fromUtf8("Top Left")), QVariant(QString::fromUtf8("tl"))}, {trUi_(QString::fromUtf8("Bottom Right")), QVariant(QString::fromUtf8("br"))}}, Qt::Vertical, true, nullptr);
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
    tools2_edrills_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_edrills_group_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay191->addWidget(tools2_edrills_group_param_label, 0, 0, 1, 2);
    tools2_edrills_group_padt_label = new QLabel(nullptr);
    tools2_edrills_group_padt_label->setText(trUi_(QString::fromUtf8("<b>Processed Pads Type:</b>")));
    tools2_edrills_group_padt_label->setToolTip(trUi_(QString::fromUtf8("The type of pads shape to be processed.\nIf the PCB has many SMD pads with rectangular pads,\ndisable the Rectangular aperture.")));
    lay191->addWidget(tools2_edrills_group_padt_label, 2, 0, 1, 2);
    tools2_edrills_group_circular_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_circular_cb->setChecked(true);
    tools2_edrills_group_circular_cb->setText(trUi_(QString::fromUtf8("Circular")));
    tools2_edrills_group_circular_cb->setToolTip(trUi_(QString::fromUtf8("Process Circular Pads.")));
    lay191->addWidget(tools2_edrills_group_circular_cb, 3, 0, 1, 2);
    tools2_edrills_group_oblong_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_oblong_cb->setText(trUi_(QString::fromUtf8("Oblong")));
    tools2_edrills_group_oblong_cb->setToolTip(trUi_(QString::fromUtf8("Process Oblong Pads.")));
    lay191->addWidget(tools2_edrills_group_oblong_cb, 4, 0, 1, 2);
    tools2_edrills_group_square_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_square_cb->setText(trUi_(QString::fromUtf8("Square")));
    tools2_edrills_group_square_cb->setToolTip(trUi_(QString::fromUtf8("Process Square Pads.")));
    lay191->addWidget(tools2_edrills_group_square_cb, 5, 0, 1, 2);
    tools2_edrills_group_rectangular_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_rectangular_cb->setText(trUi_(QString::fromUtf8("Rectangular")));
    tools2_edrills_group_rectangular_cb->setToolTip(trUi_(QString::fromUtf8("Process Rectangular Pads.")));
    lay191->addWidget(tools2_edrills_group_rectangular_cb, 6, 0, 1, 2);
    tools2_edrills_group_other_cb = new FCCheckBox(nullptr);
    tools2_edrills_group_other_cb->setText(trUi_(QString::fromUtf8("Others")));
    tools2_edrills_group_other_cb->setToolTip(trUi_(QString::fromUtf8("Process pads not in the categories above.")));
    lay191->addWidget(tools2_edrills_group_other_cb, 7, 0, 1, 2);
    auto* w192 = new QFrame(nullptr);
    w192->setProperty("frameShadow", 48);
    w192->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w192->setSizePolicy(sp); }
    lay191->addWidget(w192, 8, 0, 1, 2);
    tools2_edrills_group_hole_size_label = new QLabel(nullptr);
    tools2_edrills_group_hole_size_label->setText(trUi_(QString::fromUtf8("<b>Method:</b>")));
    tools2_edrills_group_hole_size_label->setToolTip(trUi_(QString::fromUtf8("The method for processing pads. Can be:\n- Fixed Diameter -> all holes will have a set size\n- Fixed Annular Ring -> all holes will have a set annular ring\n- Proportional -> each hole size will be a fraction of the pad size")));
    lay191->addWidget(tools2_edrills_group_hole_size_label, 9, 0, 1, 1);
    tools2_edrills_group_hole_size_radio = new RadioSet({{trUi_(QString::fromUtf8("Fixed Diameter")), QVariant(QString::fromUtf8("fixed"))}, {trUi_(QString::fromUtf8("Fixed Annular Ring")), QVariant(QString::fromUtf8("ring"))}, {trUi_(QString::fromUtf8("Proportional")), QVariant(QString::fromUtf8("prop"))}}, Qt::Vertical, false, nullptr);
    tools2_edrills_group_hole_size_radio->setValue(QVariant(QString::fromUtf8("fixed")));
    lay191->addWidget(tools2_edrills_group_hole_size_radio, 9, 1, 1, 1);
    auto* w193 = new QFrame(nullptr);
    w193->setProperty("frameShadow", 48);
    w193->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w193->setSizePolicy(sp); }
    lay191->addWidget(w193, 10, 0, 1, 2);
    tools2_edrills_group_fixed_label = new QLabel(nullptr);
    tools2_edrills_group_fixed_label->setText(trUi_(QString::fromUtf8("<b>Fixed Diameter</b>")));
    lay191->addWidget(tools2_edrills_group_fixed_label, 11, 0, 1, 2);
    tools2_edrills_group_dia_label = new QLabel(nullptr);
    tools2_edrills_group_dia_label->setText(trUi_(QString::fromUtf8("Value:")));
    tools2_edrills_group_dia_label->setToolTip(trUi_(QString::fromUtf8("Fixed hole diameter.")));
    lay191->addWidget(tools2_edrills_group_dia_label, 12, 0, 1, 1);
    tools2_edrills_group_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_dia_entry->setProperty("decimals", 4);
    tools2_edrills_group_dia_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_dia_entry->setProperty("value", 0.5);
    lay191->addWidget(tools2_edrills_group_dia_entry, 12, 1, 1, 1);
    tools2_edrills_group_ring_label = new QLabel(nullptr);
    tools2_edrills_group_ring_label->setText(trUi_(QString::fromUtf8("<b>Fixed Annular Ring</b>")));
    tools2_edrills_group_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring.\nThe copper sliver between the hole exterior\nand the margin of the copper pad.")));
    lay191->addWidget(tools2_edrills_group_ring_label, 13, 0, 1, 2);
    tools2_edrills_group_circular_ring_label = new QLabel(nullptr);
    tools2_edrills_group_circular_ring_label->setText(trUi_(QString::fromUtf8("Circular:")));
    tools2_edrills_group_circular_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for circular pads.")));
    lay191->addWidget(tools2_edrills_group_circular_ring_label, 14, 0, 1, 1);
    tools2_edrills_group_circular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_circular_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_circular_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_circular_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_circular_ring_entry, 14, 1, 1, 1);
    tools2_edrills_group_oblong_ring_label = new QLabel(nullptr);
    tools2_edrills_group_oblong_ring_label->setText(trUi_(QString::fromUtf8("Oblong:")));
    tools2_edrills_group_oblong_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for oblong pads.")));
    lay191->addWidget(tools2_edrills_group_oblong_ring_label, 15, 0, 1, 1);
    tools2_edrills_group_oblong_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_oblong_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_oblong_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_oblong_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_oblong_ring_entry, 15, 1, 1, 1);
    tools2_edrills_group_square_ring_label = new QLabel(nullptr);
    tools2_edrills_group_square_ring_label->setText(trUi_(QString::fromUtf8("Square:")));
    tools2_edrills_group_square_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for square pads.")));
    lay191->addWidget(tools2_edrills_group_square_ring_label, 16, 0, 1, 1);
    tools2_edrills_group_square_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_square_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_square_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_square_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_square_ring_entry, 16, 1, 1, 1);
    tools2_edrills_group_rectangular_ring_label = new QLabel(nullptr);
    tools2_edrills_group_rectangular_ring_label->setText(trUi_(QString::fromUtf8("Rectangular:")));
    tools2_edrills_group_rectangular_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for rectangular pads.")));
    lay191->addWidget(tools2_edrills_group_rectangular_ring_label, 17, 0, 1, 1);
    tools2_edrills_group_rectangular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_rectangular_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_rectangular_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_rectangular_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_rectangular_ring_entry, 17, 1, 1, 1);
    tools2_edrills_group_other_ring_label = new QLabel(nullptr);
    tools2_edrills_group_other_ring_label->setText(trUi_(QString::fromUtf8("Others:")));
    tools2_edrills_group_other_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for other pads.")));
    lay191->addWidget(tools2_edrills_group_other_ring_label, 18, 0, 1, 1);
    tools2_edrills_group_other_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_other_ring_entry->setProperty("decimals", 4);
    tools2_edrills_group_other_ring_entry->setProperty("maximum", 10000.0);
    tools2_edrills_group_other_ring_entry->setProperty("value", 0.2);
    lay191->addWidget(tools2_edrills_group_other_ring_entry, 18, 1, 1, 1);
    tools2_edrills_group_prop_label = new QLabel(nullptr);
    tools2_edrills_group_prop_label->setText(trUi_(QString::fromUtf8("<b>Proportional Diameter</b>")));
    lay191->addWidget(tools2_edrills_group_prop_label, 19, 0, 1, 2);
    tools2_edrills_group_factor_label = new QLabel(nullptr);
    tools2_edrills_group_factor_label->setText(trUi_(QString::fromUtf8("Factor:")));
    tools2_edrills_group_factor_label->setToolTip(trUi_(QString::fromUtf8("Proportional Diameter.\nThe hole diameter will be a fraction of the pad size.")));
    lay191->addWidget(tools2_edrills_group_factor_label, 20, 0, 1, 1);
    tools2_edrills_group_factor_entry = new FCDoubleSpinner(nullptr);
    tools2_edrills_group_factor_entry->setProperty("decimals", 4);
    tools2_edrills_group_factor_entry->setProperty("maximum", 100.0);
    tools2_edrills_group_factor_entry->setProperty("singleStep", 0.1);
    tools2_edrills_group_factor_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
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
    tools2_punch_group_param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_punch_group_param_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay194->addWidget(tools2_punch_group_param_label, 0, 0, 1, 2);
    tools2_punch_group_padt_label = new QLabel(nullptr);
    tools2_punch_group_padt_label->setText(trUi_(QString::fromUtf8("<b>Processed Pads Type:</b>")));
    tools2_punch_group_padt_label->setToolTip(trUi_(QString::fromUtf8("The type of pads shape to be processed.\nIf the PCB has many SMD pads with rectangular pads,\ndisable the Rectangular aperture.")));
    lay194->addWidget(tools2_punch_group_padt_label, 2, 0, 1, 2);
    tools2_punch_group_circular_cb = new FCCheckBox(nullptr);
    tools2_punch_group_circular_cb->setChecked(true);
    tools2_punch_group_circular_cb->setText(trUi_(QString::fromUtf8("Circular")));
    tools2_punch_group_circular_cb->setToolTip(trUi_(QString::fromUtf8("Process Circular Pads.")));
    lay194->addWidget(tools2_punch_group_circular_cb, 3, 0, 1, 2);
    tools2_punch_group_oblong_cb = new FCCheckBox(nullptr);
    tools2_punch_group_oblong_cb->setText(trUi_(QString::fromUtf8("Oblong")));
    tools2_punch_group_oblong_cb->setToolTip(trUi_(QString::fromUtf8("Process Oblong Pads.")));
    lay194->addWidget(tools2_punch_group_oblong_cb, 4, 0, 1, 2);
    tools2_punch_group_square_cb = new FCCheckBox(nullptr);
    tools2_punch_group_square_cb->setChecked(true);
    tools2_punch_group_square_cb->setText(trUi_(QString::fromUtf8("Square")));
    tools2_punch_group_square_cb->setToolTip(trUi_(QString::fromUtf8("Process Square Pads.")));
    lay194->addWidget(tools2_punch_group_square_cb, 5, 0, 1, 2);
    tools2_punch_group_rectangular_cb = new FCCheckBox(nullptr);
    tools2_punch_group_rectangular_cb->setText(trUi_(QString::fromUtf8("Rectangular")));
    tools2_punch_group_rectangular_cb->setToolTip(trUi_(QString::fromUtf8("Process Rectangular Pads.")));
    lay194->addWidget(tools2_punch_group_rectangular_cb, 6, 0, 1, 2);
    tools2_punch_group_other_cb = new FCCheckBox(nullptr);
    tools2_punch_group_other_cb->setText(trUi_(QString::fromUtf8("Others")));
    tools2_punch_group_other_cb->setToolTip(trUi_(QString::fromUtf8("Process pads not in the categories above.")));
    lay194->addWidget(tools2_punch_group_other_cb, 7, 0, 1, 2);
    auto* w195 = new QFrame(nullptr);
    w195->setProperty("frameShadow", 48);
    w195->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w195->setSizePolicy(sp); }
    lay194->addWidget(w195, 8, 0, 1, 2);
    tools2_punch_group_hole_size_label = new QLabel(nullptr);
    tools2_punch_group_hole_size_label->setText(trUi_(QString::fromUtf8("<b>Method:</b>")));
    tools2_punch_group_hole_size_label->setToolTip(trUi_(QString::fromUtf8("The punch hole source can be:\n- Excellon Object-> the Excellon object drills center will serve as reference.\n- Fixed Diameter -> will try to use the pads center as reference adding fixed diameter holes.\n- Fixed Annular Ring -> will try to keep a set annular ring.\n- Proportional -> will make a Gerber punch hole having the diameter a percentage of the pad diameter.")));
    lay194->addWidget(tools2_punch_group_hole_size_label, 9, 0, 1, 1);
    tools2_punch_group_hole_size_radio = new RadioSet({{trUi_(QString::fromUtf8("Excellon")), QVariant(QString::fromUtf8("exc"))}, {trUi_(QString::fromUtf8("Fixed Diameter")), QVariant(QString::fromUtf8("fixed"))}, {trUi_(QString::fromUtf8("Fixed Annular Ring")), QVariant(QString::fromUtf8("ring"))}, {trUi_(QString::fromUtf8("Proportional")), QVariant(QString::fromUtf8("prop"))}}, Qt::Vertical, false, nullptr);
    tools2_punch_group_hole_size_radio->setValue(QVariant(QString::fromUtf8("exc")));
    lay194->addWidget(tools2_punch_group_hole_size_radio, 9, 1, 1, 1);
    auto* w196 = new QFrame(nullptr);
    w196->setProperty("frameShadow", 48);
    w196->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w196->setSizePolicy(sp); }
    lay194->addWidget(w196, 10, 0, 1, 2);
    tools2_punch_group_fixed_label = new QLabel(nullptr);
    tools2_punch_group_fixed_label->setText(trUi_(QString::fromUtf8("<b>Fixed Diameter</b>")));
    lay194->addWidget(tools2_punch_group_fixed_label, 11, 0, 1, 2);
    tools2_punch_group_dia_label = new QLabel(nullptr);
    tools2_punch_group_dia_label->setText(trUi_(QString::fromUtf8("Value:")));
    tools2_punch_group_dia_label->setToolTip(trUi_(QString::fromUtf8("Fixed hole diameter.")));
    lay194->addWidget(tools2_punch_group_dia_label, 12, 0, 1, 1);
    tools2_punch_group_dia_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_dia_entry->setProperty("decimals", 4);
    tools2_punch_group_dia_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_dia_entry->setProperty("value", 0.5);
    lay194->addWidget(tools2_punch_group_dia_entry, 12, 1, 1, 1);
    tools2_punch_group_ring_label = new QLabel(nullptr);
    tools2_punch_group_ring_label->setText(trUi_(QString::fromUtf8("<b>Fixed Annular Ring</b>")));
    tools2_punch_group_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring.\nThe copper sliver between the hole exterior\nand the margin of the copper pad.")));
    lay194->addWidget(tools2_punch_group_ring_label, 13, 0, 1, 2);
    tools2_punch_group_circular_ring_label = new QLabel(nullptr);
    tools2_punch_group_circular_ring_label->setText(trUi_(QString::fromUtf8("Circular:")));
    tools2_punch_group_circular_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for circular pads.")));
    lay194->addWidget(tools2_punch_group_circular_ring_label, 14, 0, 1, 1);
    tools2_punch_group_circular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_circular_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_circular_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_circular_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_circular_ring_entry, 14, 1, 1, 1);
    tools2_punch_group_oblong_ring_label = new QLabel(nullptr);
    tools2_punch_group_oblong_ring_label->setText(trUi_(QString::fromUtf8("Oblong:")));
    tools2_punch_group_oblong_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for oblong pads.")));
    lay194->addWidget(tools2_punch_group_oblong_ring_label, 15, 0, 1, 1);
    tools2_punch_group_oblong_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_oblong_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_oblong_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_oblong_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_oblong_ring_entry, 15, 1, 1, 1);
    tools2_punch_group_square_ring_label = new QLabel(nullptr);
    tools2_punch_group_square_ring_label->setText(trUi_(QString::fromUtf8("Square:")));
    tools2_punch_group_square_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for square pads.")));
    lay194->addWidget(tools2_punch_group_square_ring_label, 16, 0, 1, 1);
    tools2_punch_group_square_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_square_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_square_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_square_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_square_ring_entry, 16, 1, 1, 1);
    tools2_punch_group_rectangular_ring_label = new QLabel(nullptr);
    tools2_punch_group_rectangular_ring_label->setText(trUi_(QString::fromUtf8("Rectangular:")));
    tools2_punch_group_rectangular_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for rectangular pads.")));
    lay194->addWidget(tools2_punch_group_rectangular_ring_label, 17, 0, 1, 1);
    tools2_punch_group_rectangular_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_rectangular_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_rectangular_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_rectangular_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_rectangular_ring_entry, 17, 1, 1, 1);
    tools2_punch_group_other_ring_label = new QLabel(nullptr);
    tools2_punch_group_other_ring_label->setText(trUi_(QString::fromUtf8("Others:")));
    tools2_punch_group_other_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for other pads.")));
    lay194->addWidget(tools2_punch_group_other_ring_label, 18, 0, 1, 1);
    tools2_punch_group_other_ring_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_other_ring_entry->setProperty("decimals", 4);
    tools2_punch_group_other_ring_entry->setProperty("maximum", 10000.0);
    tools2_punch_group_other_ring_entry->setProperty("value", 0.2);
    lay194->addWidget(tools2_punch_group_other_ring_entry, 18, 1, 1, 1);
    tools2_punch_group_prop_label = new QLabel(nullptr);
    tools2_punch_group_prop_label->setText(trUi_(QString::fromUtf8("<b>Proportional Diameter</b>")));
    lay194->addWidget(tools2_punch_group_prop_label, 19, 0, 1, 2);
    tools2_punch_group_factor_label = new QLabel(nullptr);
    tools2_punch_group_factor_label->setText(trUi_(QString::fromUtf8("Factor:")));
    tools2_punch_group_factor_label->setToolTip(trUi_(QString::fromUtf8("Proportional Diameter.\nThe hole diameter will be a fraction of the pad size.")));
    lay194->addWidget(tools2_punch_group_factor_label, 20, 0, 1, 1);
    tools2_punch_group_factor_entry = new FCDoubleSpinner(nullptr);
    tools2_punch_group_factor_entry->setProperty("decimals", 4);
    tools2_punch_group_factor_entry->setProperty("maximum", 100.0);
    tools2_punch_group_factor_entry->setProperty("singleStep", 0.1);
    tools2_punch_group_factor_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
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
    tools2_invert_group_sublabel->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    tools2_invert_group_sublabel->setToolTip(trUi_(QString::fromUtf8("A tool to invert Gerber geometry from positive to negative\nand in revers.")));
    tools2_invert_group_layout->addWidget(tools2_invert_group_sublabel, 0);
    auto* lay197 = new QGridLayout();
    lay197->setContentsMargins(0, 0, 0, 0);
    lay197->setHorizontalSpacing(6);
    lay197->setVerticalSpacing(6);
    tools2_invert_group_margin_label = new QLabel(nullptr);
    tools2_invert_group_margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    tools2_invert_group_margin_label->setToolTip(trUi_(QString::fromUtf8("Distance by which to avoid\nthe edges of the Gerber object.")));
    lay197->addWidget(tools2_invert_group_margin_label, 2, 0, 1, 2);
    tools2_invert_group_margin_entry = new FCDoubleSpinner(nullptr);
    tools2_invert_group_margin_entry->setProperty("decimals", 4);
    tools2_invert_group_margin_entry->setProperty("maximum", 10000.0);
    tools2_invert_group_margin_entry->setProperty("objectName", QString::fromUtf8("Margin"));
    tools2_invert_group_margin_entry->setProperty("value", 0.1);
    lay197->addWidget(tools2_invert_group_margin_entry, 3, 0, 1, 2);
    tools2_invert_group_join_label = new QLabel(nullptr);
    tools2_invert_group_join_label->setText(trUi_(QString::fromUtf8("Lines Join Style:")));
    tools2_invert_group_join_label->setToolTip(trUi_(QString::fromUtf8("The way that the lines in the object outline will be joined.\nCan be:\n- rounded -> an arc is added between two joining lines\n- square -> the lines meet in 90 degrees angle\n- bevel -> the lines are joined by a third line")));
    lay197->addWidget(tools2_invert_group_join_label, 5, 0, 1, 2);
    tools2_invert_group_join_radio = new RadioSet({{trUi_(QString::fromUtf8("Rounded")), QVariant(QString::fromUtf8("r"))}, {trUi_(QString::fromUtf8("Square")), QVariant(QString::fromUtf8("s"))}, {trUi_(QString::fromUtf8("Bevel")), QVariant(QString::fromUtf8("b"))}}, Qt::Vertical, false, nullptr);
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
    pref_tab_area->addTab(tools2_tab, trUi_(QString::fromUtf8("TOOLS 2")));
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
    fa_excellon_group_restore_btn->setText(trUi_(QString::fromUtf8("Restore")));
    fa_excellon_group_restore_btn->setToolTip(trUi_(QString::fromUtf8("Restore the extension list to the default state.")));
    lay200->addWidget(fa_excellon_group_restore_btn, 0);
    fa_excellon_group_del_all_btn = new FCButton(nullptr);
    fa_excellon_group_del_all_btn->setText(trUi_(QString::fromUtf8("Delete All")));
    fa_excellon_group_del_all_btn->setToolTip(trUi_(QString::fromUtf8("Delete all extensions from the list.")));
    lay200->addWidget(fa_excellon_group_del_all_btn, 0);
    fa_excellon_group_vertical_lay->addLayout(lay200, 0);
    auto* w201 = new QLabel(nullptr);
    w201->setText(trUi_(QString::fromUtf8("<b>Extensions list:</b>")));
    w201->setToolTip(trUi_(QString::fromUtf8("List of file extensions to be\nassociated with FlatCAM.")));
    fa_excellon_group_vertical_lay->addWidget(w201, 0);
    fa_excellon_group_exc_list_text = new FCTextArea(nullptr);
    { QFont f = fa_excellon_group_exc_list_text->font(); f.setPointSizeF(10.0); fa_excellon_group_exc_list_text->setFont(f); }
    fa_excellon_group_exc_list_text->setProperty("plainText", trUi_(QString::fromUtf8("drd, drill, drl, exc, ncd, tap, xln")));
    fa_excellon_group_exc_list_text->setReadOnly(true);
    fa_excellon_group_exc_list_text->setProperty("textInteractionFlags", 1);
    fa_excellon_group_vertical_lay->addWidget(fa_excellon_group_exc_list_text, 0);
    auto* lay202 = new QHBoxLayout();
    lay202->setContentsMargins(0, 0, 0, 0);
    lay202->setSpacing(6);
    fa_excellon_group_ext_label = new QLabel(nullptr);
    fa_excellon_group_ext_label->setText(trUi_(QString::fromUtf8("Extension:")));
    fa_excellon_group_ext_label->setToolTip(trUi_(QString::fromUtf8("A file extension to be added or deleted to the list.")));
    lay202->addWidget(fa_excellon_group_ext_label, 0);
    fa_excellon_group_ext_entry = new FCEntry(nullptr);
    lay202->addWidget(fa_excellon_group_ext_entry, 0);
    fa_excellon_group_vertical_lay->addLayout(lay202, 0);
    auto* lay203 = new QHBoxLayout();
    lay203->setContentsMargins(0, 0, 0, 0);
    lay203->setSpacing(6);
    fa_excellon_group_add_btn = new FCButton(nullptr);
    fa_excellon_group_add_btn->setText(trUi_(QString::fromUtf8("Add Extension")));
    fa_excellon_group_add_btn->setToolTip(trUi_(QString::fromUtf8("Add a file extension to the list")));
    lay203->addWidget(fa_excellon_group_add_btn, 0);
    fa_excellon_group_del_btn = new FCButton(nullptr);
    fa_excellon_group_del_btn->setText(trUi_(QString::fromUtf8("Delete Extension")));
    fa_excellon_group_del_btn->setToolTip(trUi_(QString::fromUtf8("Delete a file extension from the list")));
    lay203->addWidget(fa_excellon_group_del_btn, 0);
    fa_excellon_group_vertical_lay->addLayout(lay203, 0);
    fa_excellon_group_exc_list_btn = new FCButton(nullptr);
    fa_excellon_group_exc_list_btn->setText(trUi_(QString::fromUtf8("Apply Association")));
    fa_excellon_group_exc_list_btn->setToolTip(trUi_(QString::fromUtf8("Apply the file associations between\nFlatCAM and the files with above extensions.\nThey will be active after next logon.\nThis work only in Windows.")));
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
    fa_gcode_group_restore_btn->setText(trUi_(QString::fromUtf8("Restore")));
    fa_gcode_group_restore_btn->setToolTip(trUi_(QString::fromUtf8("Restore the extension list to the default state.")));
    lay204->addWidget(fa_gcode_group_restore_btn, 0);
    fa_gcode_group_del_all_btn = new FCButton(nullptr);
    fa_gcode_group_del_all_btn->setText(trUi_(QString::fromUtf8("Delete All")));
    fa_gcode_group_del_all_btn->setToolTip(trUi_(QString::fromUtf8("Delete all extensions from the list.")));
    lay204->addWidget(fa_gcode_group_del_all_btn, 0);
    fa_gcode_group_layout->addLayout(lay204, 0);
    fa_gcode_group_gco_list_label = new QLabel(nullptr);
    fa_gcode_group_gco_list_label->setText(trUi_(QString::fromUtf8("<b>Extensions list:</b>")));
    fa_gcode_group_gco_list_label->setToolTip(trUi_(QString::fromUtf8("List of file extensions to be\nassociated with FlatCAM.")));
    fa_gcode_group_layout->addWidget(fa_gcode_group_gco_list_label, 0);
    fa_gcode_group_gco_list_text = new FCTextArea(nullptr);
    { QFont f = fa_gcode_group_gco_list_text->font(); f.setPointSizeF(10.0); fa_gcode_group_gco_list_text->setFont(f); }
    fa_gcode_group_gco_list_text->setProperty("plainText", trUi_(QString::fromUtf8("cnc, din, dnc, ecs, eia, fan, fgc, fnc, gc, gcd, gcode, h, hnc, i, min, mpf, mpr, nc, ncc, ncg, ncp, ngc, out, ply, rol, sbp, tap, xpi")));
    fa_gcode_group_gco_list_text->setReadOnly(true);
    fa_gcode_group_gco_list_text->setProperty("textInteractionFlags", 1);
    fa_gcode_group_layout->addWidget(fa_gcode_group_gco_list_text, 0);
    auto* lay205 = new QHBoxLayout();
    lay205->setContentsMargins(0, 0, 0, 0);
    lay205->setSpacing(6);
    fa_gcode_group_ext_label = new QLabel(nullptr);
    fa_gcode_group_ext_label->setText(trUi_(QString::fromUtf8("Extension:")));
    fa_gcode_group_ext_label->setToolTip(trUi_(QString::fromUtf8("A file extension to be added or deleted to the list.")));
    lay205->addWidget(fa_gcode_group_ext_label, 0);
    fa_gcode_group_ext_entry = new FCEntry(nullptr);
    lay205->addWidget(fa_gcode_group_ext_entry, 0);
    fa_gcode_group_layout->addLayout(lay205, 0);
    auto* lay206 = new QHBoxLayout();
    lay206->setContentsMargins(0, 0, 0, 0);
    lay206->setSpacing(6);
    fa_gcode_group_add_btn = new FCButton(nullptr);
    fa_gcode_group_add_btn->setText(trUi_(QString::fromUtf8("Add Extension")));
    fa_gcode_group_add_btn->setToolTip(trUi_(QString::fromUtf8("Add a file extension to the list")));
    lay206->addWidget(fa_gcode_group_add_btn, 0);
    fa_gcode_group_del_btn = new FCButton(nullptr);
    fa_gcode_group_del_btn->setText(trUi_(QString::fromUtf8("Delete Extension")));
    fa_gcode_group_del_btn->setToolTip(trUi_(QString::fromUtf8("Delete a file extension from the list")));
    lay206->addWidget(fa_gcode_group_del_btn, 0);
    fa_gcode_group_layout->addLayout(lay206, 0);
    fa_gcode_group_gco_list_btn = new FCButton(nullptr);
    fa_gcode_group_gco_list_btn->setText(trUi_(QString::fromUtf8("Apply Association")));
    fa_gcode_group_gco_list_btn->setToolTip(trUi_(QString::fromUtf8("Apply the file associations between\nFlatCAM and the files with above extensions.\nThey will be active after next logon.\nThis work only in Windows.")));
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
    fa_gerber_group_restore_btn->setText(trUi_(QString::fromUtf8("Restore")));
    fa_gerber_group_restore_btn->setToolTip(trUi_(QString::fromUtf8("Restore the extension list to the default state.")));
    lay207->addWidget(fa_gerber_group_restore_btn, 0);
    fa_gerber_group_del_all_btn = new FCButton(nullptr);
    fa_gerber_group_del_all_btn->setText(trUi_(QString::fromUtf8("Delete All")));
    fa_gerber_group_del_all_btn->setToolTip(trUi_(QString::fromUtf8("Delete all extensions from the list.")));
    lay207->addWidget(fa_gerber_group_del_all_btn, 0);
    fa_gerber_group_layout->addLayout(lay207, 0);
    fa_gerber_group_grb_list_label = new QLabel(nullptr);
    fa_gerber_group_grb_list_label->setText(trUi_(QString::fromUtf8("<b>Extensions list:</b>")));
    fa_gerber_group_grb_list_label->setToolTip(trUi_(QString::fromUtf8("List of file extensions to be\nassociated with FlatCAM.")));
    fa_gerber_group_layout->addWidget(fa_gerber_group_grb_list_label, 0);
    fa_gerber_group_grb_list_text = new FCTextArea(nullptr);
    { QFont f = fa_gerber_group_grb_list_text->font(); f.setPointSizeF(10.0); fa_gerber_group_grb_list_text->setFont(f); }
    fa_gerber_group_grb_list_text->setProperty("plainText", trUi_(QString::fromUtf8("art, bot, bsm, cmp, crc, crs, dim, gb0, gb1, gb2, gb3, gb4, gb5, gb6, gb7, gb8, gb9, gbd, gbl, gbo, gbp, gbr, gbs, gdo, ger, gko, gm1, gm2, gm3, grb, gtl, gto, gtp, gts, ly15, ly2, mil, outline, pho, plc, pls, smb, smt, sol, spb, spt, ssb, sst, stc, sts, top, tsm")));
    fa_gerber_group_grb_list_text->setReadOnly(true);
    fa_gerber_group_grb_list_text->setProperty("textInteractionFlags", 1);
    fa_gerber_group_layout->addWidget(fa_gerber_group_grb_list_text, 0);
    auto* lay208 = new QHBoxLayout();
    lay208->setContentsMargins(0, 0, 0, 0);
    lay208->setSpacing(6);
    fa_gerber_group_ext_label = new QLabel(nullptr);
    fa_gerber_group_ext_label->setText(trUi_(QString::fromUtf8("Extension:")));
    fa_gerber_group_ext_label->setToolTip(trUi_(QString::fromUtf8("A file extension to be added or deleted to the list.")));
    lay208->addWidget(fa_gerber_group_ext_label, 0);
    fa_gerber_group_ext_entry = new FCEntry(nullptr);
    lay208->addWidget(fa_gerber_group_ext_entry, 0);
    fa_gerber_group_layout->addLayout(lay208, 0);
    auto* lay209 = new QHBoxLayout();
    lay209->setContentsMargins(0, 0, 0, 0);
    lay209->setSpacing(6);
    fa_gerber_group_add_btn = new FCButton(nullptr);
    fa_gerber_group_add_btn->setText(trUi_(QString::fromUtf8("Add Extension")));
    fa_gerber_group_add_btn->setToolTip(trUi_(QString::fromUtf8("Add a file extension to the list")));
    lay209->addWidget(fa_gerber_group_add_btn, 0);
    fa_gerber_group_del_btn = new FCButton(nullptr);
    fa_gerber_group_del_btn->setText(trUi_(QString::fromUtf8("Delete Extension")));
    fa_gerber_group_del_btn->setToolTip(trUi_(QString::fromUtf8("Delete a file extension from the list")));
    lay209->addWidget(fa_gerber_group_del_btn, 0);
    fa_gerber_group_layout->addLayout(lay209, 0);
    fa_gerber_group_grb_list_btn = new FCButton(nullptr);
    fa_gerber_group_grb_list_btn->setText(trUi_(QString::fromUtf8("Apply Association")));
    fa_gerber_group_grb_list_btn->setToolTip(trUi_(QString::fromUtf8("Apply the file associations between\nFlatCAM and the files with above extensions.\nThey will be active after next logon.\nThis work only in Windows.")));
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
    kw_group_restore_btn->setText(trUi_(QString::fromUtf8("Restore")));
    kw_group_restore_btn->setToolTip(trUi_(QString::fromUtf8("Restore the autocompleter keywords list to the default state.")));
    lay210->addWidget(kw_group_restore_btn, 0);
    kw_group_del_all_btn = new FCButton(nullptr);
    kw_group_del_all_btn->setText(trUi_(QString::fromUtf8("Delete All")));
    kw_group_del_all_btn->setToolTip(trUi_(QString::fromUtf8("Delete all autocompleter keywords from the list.")));
    lay210->addWidget(kw_group_del_all_btn, 0);
    kw_group_layout->addLayout(lay210, 0);
    kw_group_grb_list_label = new QLabel(nullptr);
    kw_group_grb_list_label->setText(trUi_(QString::fromUtf8("<b>Keywords list:</b>")));
    kw_group_grb_list_label->setToolTip(trUi_(QString::fromUtf8("List of keywords used by\nthe autocompleter in FlatCAM.\nThe autocompleter is installed\nin the Code Editor and for the Tcl Shell.")));
    kw_group_layout->addWidget(kw_group_grb_list_label, 0);
    kw_group_kw_list_text = new FCTextArea(nullptr);
    { QFont f = kw_group_kw_list_text->font(); f.setPointSizeF(10.0); kw_group_kw_list_text->setFont(f); }
    kw_group_kw_list_text->setProperty("plainText", trUi_(QString::fromUtf8("Desktop, Documents, FlatConfig, FlatPrj, False, Marius, My Documents, Paste_1, Repetier, Roland_MDX_20, True, Users, Toolchange_Custom, Toolchange_Probe_MACH3, Toolchange_manual, Users, all, axis, auto, axisoffset, box, center_x, center_y, columns, combine, connect, contour, default, depthperpass, dia, diatol, dist, drilled_dias, drillz, dpp, dwelltime, endxy, endz, extracut_length, f, feedrate, feedrate_z, grbl_11, GRBL_laser, gridoffsety, gridx, gridy, has_offset, holes, hpgl, iso_type, line_xyz, margin, marlin, method, milled_dias, minoffset, name, offset, opt_type, order, outname, overlap, passes, postamble, pp, ppname_e, ppname_g, preamble, radius, ref, rest, rows, shellvar_, scale_factor, spacing_columns, spacing_rows, spindlespeed, startz, startxy, toolchange_xy, toolchangez, tooldia, travelz, use_threads, value, x, x0, x1, y, y0, y1, z_cut, z_move")));
    kw_group_kw_list_text->setReadOnly(true);
    kw_group_kw_list_text->setProperty("textInteractionFlags", 1);
    kw_group_layout->addWidget(kw_group_kw_list_text, 0);
    auto* lay211 = new QHBoxLayout();
    lay211->setContentsMargins(0, 0, 0, 0);
    lay211->setSpacing(6);
    kw_group_kw_label = new QLabel(nullptr);
    kw_group_kw_label->setText(trUi_(QString::fromUtf8("Extension:")));
    kw_group_kw_label->setToolTip(trUi_(QString::fromUtf8("A keyword to be added or deleted to the list.")));
    lay211->addWidget(kw_group_kw_label, 0);
    kw_group_kw_entry = new FCEntry(nullptr);
    lay211->addWidget(kw_group_kw_entry, 0);
    kw_group_layout->addLayout(lay211, 0);
    auto* lay212 = new QHBoxLayout();
    lay212->setContentsMargins(0, 0, 0, 0);
    lay212->setSpacing(6);
    kw_group_add_btn = new FCButton(nullptr);
    kw_group_add_btn->setText(trUi_(QString::fromUtf8("Add keyword")));
    kw_group_add_btn->setToolTip(trUi_(QString::fromUtf8("Add a keyword to the list")));
    lay212->addWidget(kw_group_add_btn, 0);
    kw_group_del_btn = new FCButton(nullptr);
    kw_group_del_btn->setText(trUi_(QString::fromUtf8("Delete keyword")));
    kw_group_del_btn->setToolTip(trUi_(QString::fromUtf8("Delete a keyword from the list")));
    lay212->addWidget(kw_group_del_btn, 0);
    kw_group_layout->addLayout(lay212, 0);
    kw_group->setLayout(kw_group_layout);
    layout->addWidget(kw_group, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    util_defaults_form->setLayout(layout);
    fa_scroll_area->setWidget(util_defaults_form);
    fa_tab_lay->addWidget(fa_scroll_area, 0);
    fa_tab->setLayout(fa_tab_lay);
    pref_tab_area->addTab(fa_tab, trUi_(QString::fromUtf8("UTILITIES")));
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
    pref_defaults_button->setText(trUi_(QString::fromUtf8("Restore Defaults")));
    pref_defaults_button->setToolTip(trUi_(QString::fromUtf8("Restore the entire set of default values\nto the initial values loaded after first launch.")));
    pref_tab_bottom_layout_1->addWidget(pref_defaults_button, 0);
    pref_open_button = new FCButton(nullptr);
    pref_open_button->setIcon(QIcon(QString::fromUtf8(":/images/pref.png")));
    pref_open_button->setMinimumSize(QSize(130, 0));
    pref_open_button->setText(trUi_(QString::fromUtf8("Open Pref Folder")));
    pref_open_button->setToolTip(trUi_(QString::fromUtf8("Open the folder where FlatCAM save the preferences files.")));
    pref_tab_bottom_layout_1->addWidget(pref_open_button, 0);
    clear_btn = new FCButton(nullptr);
    clear_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    clear_btn->setMinimumSize(QSize(130, 0));
    clear_btn->setText(trUi_(QString::fromUtf8("Clear GUI Settings")));
    clear_btn->setToolTip(trUi_(QString::fromUtf8("Clear the GUI settings for FlatCAM,\nsuch as: layout, gui state, style, hdpi support etc.")));
    pref_tab_bottom_layout_1->addWidget(clear_btn, 0);
    pref_tab_bottom_layout->addLayout(pref_tab_bottom_layout_1, 0);
    pref_tab_bottom_layout_2 = new QHBoxLayout();
    pref_tab_bottom_layout_2->setContentsMargins(0, 0, 0, 0);
    pref_tab_bottom_layout_2->setSpacing(6);
    pref_apply_button = new FCButton(nullptr);
    pref_apply_button->setIcon(QIcon(QString::fromUtf8(":/images/apply32.png")));
    pref_apply_button->setMinimumSize(QSize(130, 0));
    pref_apply_button->setText(trUi_(QString::fromUtf8("Apply")));
    pref_apply_button->setToolTip(trUi_(QString::fromUtf8("Apply the current preferences without saving to a file.")));
    pref_tab_bottom_layout_2->addWidget(pref_apply_button, 0);
    pref_save_button = new FCButton(nullptr);
    pref_save_button->setIcon(QIcon(QString::fromUtf8(":/images/save_as.png")));
    pref_save_button->setMinimumSize(QSize(130, 0));
    pref_save_button->setText(trUi_(QString::fromUtf8("Save")));
    pref_save_button->setToolTip(trUi_(QString::fromUtf8("Save the current settings in the 'current_defaults' file\nwhich is the file storing the working default preferences.")));
    pref_tab_bottom_layout_2->addWidget(pref_save_button, 0);
    pref_close_button = new FCButton(nullptr);
    pref_close_button->setMinimumSize(QSize(130, 0));
    pref_close_button->setText(trUi_(QString::fromUtf8("Cancel")));
    pref_close_button->setToolTip(trUi_(QString::fromUtf8("Will not save the changes and will close the preferences window.")));
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
