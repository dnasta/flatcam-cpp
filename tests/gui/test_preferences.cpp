// Tests de las Preferencias (port de FlatCAMDefaults y PreferencesUIManager), contra el original con
// tests/data/preferences/oracle.json (tests/tools/oracle_preferences.py).
#include <gtest/gtest.h>

#include <QApplication>
#include <QFile>
#include <QKeyEvent>
#include <QFileInfo>
#include <QSettings>
#include <QTemporaryDir>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/tools/PropertiesTool.h"
#include "gui/tools/ToolPanels.h"
#include "gui/objects/GeometryObject.h"
#include "gui/widgets/FCDetachableTab.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/RadioSet.h"
#include "gui/widgets/FCLabel.h"
#include "io/ProjectFile.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {

JsonValue readJson(const QString& path) {
    QFile f(path);
    EXPECT_TRUE(f.open(QIODevice::ReadOnly)) << path.toStdString();
    JsonValue v;
    std::string err;
    EXPECT_TRUE(ProjectFile::parse(f.readAll().toStdString(), v, err)) << err;
    return v;
}

const JsonValue& oracle() {
    static const JsonValue o = readJson(QString(FLATCAM_TEST_DATA_DIR) + "/preferences/oracle.json");
    return o;
}

/// Una tecla en la ventana (los atajos los atiende el filtro de eventos de la aplicación).
bool key(App& app, int k, Qt::KeyboardModifiers m) {
    QKeyEvent ev(QEvent::KeyPress, k, m);
    QApplication::sendEvent(app.ui(), &ev);
    return true;
}

/// El valor escrito como JSON (así se comparan tipo y texto: 1 contra 1.0, [0.5] contra "0.5").
std::string js(const JsonValue& v) { return ProjectFile::dump(v, true); }

struct TempApp {
    QTemporaryDir dir;
    App app;
    TempApp() {
        app.setDataPath(dir.path());
        app.preferences()->setAutoAnswer(true);
    }
    Preferences* p() { return app.preferences(); }
};

}  // namespace

TEST(GuiPreferences, CreatesTheDataFilesLikeTheOriginal) {
    TempApp t;
    const QString cur = t.dir.path() + "/current_defaults.FlatConfig";
    const QString fac = t.dir.path() + "/factory_defaults.FlatConfig";
    ASSERT_TRUE(QFile::exists(cur));
    ASSERT_TRUE(QFile::exists(fac));
    QFile f(cur);
    ASSERT_TRUE(f.open(QIODevice::ReadOnly));
    EXPECT_EQ(f.readAll(), QByteArray("{}"));
    const JsonValue factory = readJson(fac);
    EXPECT_EQ(factory["version"].text(), "2024.4");
    EXPECT_TRUE(factory["version"].isString());
    EXPECT_EQ(js(factory), js(Preferences::factoryDefaults()));
    EXPECT_FALSE(QFileInfo(fac).isWritable());  // de solo lectura, como el original
    // El tema guardado: el claro de fábrica en el port.
    EXPECT_EQ(t.p()->value("global_theme").text(), "white");
}

TEST(GuiPreferences, FormMatchesTheOriginalForEveryField) {
    TempApp t;
    Preferences* p = t.p();
    ASSERT_EQ(Preferences::fields().size(), 485u);
    // Al arrancar (show_preferences_gui con __init_color_pickers y defaults_write_form) y después defaults_read_form():
    // mismo valor y mismo tipo que el original en las 485 claves. Los controles de alfa reescriben el alfa de los colores
    // en minúsculas ("#BBF268bf"), también en las opciones. El tema es el único distinto (el port solo tiene el claro).
    int checked = 0;
    for (const auto& [k, v] : oracle()["factory_read"].members()) {
        if (k == "global_theme") {
            ++checked;
            continue;
        }
        EXPECT_EQ(js(p->value(k)).size() > 0, true);
        if (k.find("_fill") != std::string::npos || k.find("_line") != std::string::npos)
            EXPECT_EQ(js(p->value(k)), js(v)) << "app.defaults " << k;
        const auto mine = p->formValue(k);
        ASSERT_TRUE(mine.has_value()) << k;
        EXPECT_EQ(js(*mine), js(v)) << k;
        ++checked;
    }
    EXPECT_EQ(checked, 485);
    // El texto de los campos de texto (formatos de set_value: '%.4f', str(), None...).
    for (const auto& [k, v] : oracle()["factory_text"].members()) {
        if (v.isNull()) continue;
        const auto& [member, py] = Preferences::fields().at(k);
        QObject* w = p->tab()->membersByName().at(member);
        QString text;
        if (auto* e = qobject_cast<QLineEdit*>(w)) text = e->text();
        else if (auto* a = qobject_cast<QPlainTextEdit*>(w)) text = a->toPlainText();
        else if (auto* c = qobject_cast<FCColorEntry*>(w)) text = c->value();
        EXPECT_EQ(text.toStdString(), v.text()) << k << " (" << py << ")";
    }
    // Después de pasar por un archivo (las tuplas son listas).
    p->writeForm(&oracle()["factory_read"]);
    for (const auto& [k, v] : oracle()["roundtrip_read"].members()) EXPECT_EQ(js(*p->formValue(k)), js(v)) << k;
    // Error del original que se reproduce: editar el color de relleno de la selección deja solo su alfa.
    p->tab()->general_gui_group_sf_color_entry->setValue("#123456");
    Q_EMIT p->tab()->general_gui_group_sf_color_entry->editingFinished();
    EXPECT_EQ(p->value("global_sel_fill").text(), "bf");
    // El alfa de Gerber cambia el de los dos colores (sin completar con ceros, como hex() de Python).
    p->tab()->gerber_gen_group_gerber_alpha_entry->setValue(5);
    EXPECT_EQ(p->value("gerber_plot_fill").text(), "#BBF2685");
    EXPECT_EQ(p->value("gerber_plot_line").text(), "#006E205");
    EXPECT_EQ(p->tab()->gerber_gen_group_fill_color_entry->value().toStdString(), "#BBF2685");  // escribe el campo
}

TEST(GuiPreferences, EvaluatesTypedFormulasLikePython) {
    TempApp t;
    Preferences* p = t.p();
    for (const auto& [k, cases] : oracle()["typed_read"].members()) {
        QObject* w = p->tab()->membersByName().at(Preferences::fields().at(k).first);
        for (const JsonValue& c : cases.items()) {
            static_cast<QLineEdit*>(w)->setText(QString::fromStdString(c.items()[0].text()));
            EXPECT_EQ(js(*p->formValue(k)), js(c.items()[1])) << k << " '" << c.items()[0].text() << "'";
        }
    }
}

TEST(GuiPreferences, SaveWritesTheFileAndANewAppLoadsIt) {
    QTemporaryDir dir;
    {
        App app;
        app.setDataPath(dir.path());
        Preferences* p = app.preferences();
        p->setAutoAnswer(true);
        p->open();
        EXPECT_TRUE(p->isOpen());
        auto* overlap = p->tab()->tools_ncc_group_ncc_overlap_entry;  // NCC: 40 % de fábrica
        overlap->setValue(80.0);
        EXPECT_TRUE(p->changed());  // valueChanged marca las preferencias como editadas
        p->tab()->pref_save_button->click();
        EXPECT_FALSE(p->isOpen());  // Guardar cierra la pestaña
        EXPECT_FALSE(p->changed());
        EXPECT_EQ(js(p->value("tools_ncc_overlap")), "80.0");
        const JsonValue saved = readJson(dir.path() + "/current_defaults.FlatConfig");
        EXPECT_EQ(js(saved["tools_ncc_overlap"]), "80.0");
        EXPECT_EQ(saved["version"].text(), "2024.4");
        EXPECT_EQ(saved.members().size(), Preferences::factoryDefaults().members().size());
        // Los ajustes de QSettings van al settings.ini de la carpeta de datos.
        QSettings s(dir.path() + "/settings.ini", QSettings::IniFormat);
        EXPECT_EQ(s.value("hud_font_size").toInt(), p->tab()->general_app_set_group_hud_font_size_spinner->value());
    }
    App again;
    again.setDataPath(dir.path());
    EXPECT_EQ(js(again.preferences()->value("tools_ncc_overlap")), "80.0");
    EXPECT_EQ(again.preferences()->tab()->tools_ncc_group_ncc_overlap_entry->value(), 80.0);
    EXPECT_EQ(js(again.defaults()["tools_ncc_overlap"]), "80.0");  // app.defaults
}

TEST(GuiPreferences, ApplyDoesNotReadTheFormLikeTheOriginal) {
    TempApp t;
    Preferences* p = t.p();
    p->open();
    p->tab()->tools_ncc_group_ncc_overlap_entry->setValue(70.0);
    p->tab()->pref_apply_button->click();
    // Error del original que se reproduce: "Aplicar" no lee el formulario (las opciones quedan como estaban).
    EXPECT_EQ(js(p->value("tools_ncc_overlap")), js(Preferences::factoryDefaults()["tools_ncc_overlap"]));
    EXPECT_TRUE(p->isOpen());
    EXPECT_FALSE(p->changed());
    EXPECT_EQ(js(readJson(t.dir.path() + "/current_defaults.FlatConfig")), "{}");  // no escribe el archivo
}

TEST(GuiPreferences, CloseRestoresTheFormAndRestoreGoesBackToFactory) {
    {
        TempApp t;
        Preferences* p = t.p();
        p->open();
        p->tab()->tools_ncc_group_ncc_overlap_entry->setValue(70.0);
        p->tab()->pref_close_button->click();
        EXPECT_FALSE(p->isOpen());
        EXPECT_EQ(p->tab()->tools_ncc_group_ncc_overlap_entry->value(), 40.0);
        // Error del original que se reproduce: al reescribir el formulario las señales siguen conectadas y las
        // preferencias quedan marcadas como editadas aunque se cerraron sin guardar.
        EXPECT_TRUE(p->changed());
        // on_defaults_dict_change: cambiar una opción escribe su campo.
        p->set("tools_ncc_overlap", JsonValue::real(55.0));
        EXPECT_EQ(p->tab()->tools_ncc_group_ncc_overlap_entry->value(), 55.0);
    }
    TempApp t;
    Preferences* p = t.p();
    p->set("tools_ncc_overlap", JsonValue::real(55.0));
    p->open();
    p->tab()->pref_defaults_button->click();
    EXPECT_EQ(js(p->value("tools_ncc_overlap")), js(Preferences::factoryDefaults()["tools_ncc_overlap"]));
    EXPECT_EQ(p->tab()->tools_ncc_group_ncc_overlap_entry->value(), 40.0);
    EXPECT_TRUE(p->changed());
    const int idx = t.app.ui()->plot_tab_area->indexOf(p->tab());
    EXPECT_EQ(t.app.ui()->plot_tab_area->tabBar()->tabTextColor(idx), QColor("red"));
    EXPECT_EQ(p->tab()->pref_apply_button->styleSheet(), "QPushButton {color: red;}");
}

TEST(GuiPreferences, OldVersionFilesAreResetLikeTheBeta) {
    QTemporaryDir dir;
    {
        QFile f(dir.path() + "/current_defaults.FlatConfig");
        ASSERT_TRUE(f.open(QIODevice::WriteOnly));
        f.write(R"({"version": 8.994, "tools_ncc_overlap": 80})");
    }
    App app;
    app.setDataPath(dir.path());
    EXPECT_TRUE(app.preferences()->oldDefaultsFound());
    EXPECT_EQ(js(app.preferences()->value("tools_ncc_overlap")), js(Preferences::factoryDefaults()["tools_ncc_overlap"]));
}

TEST(GuiPreferences, StatusLabelTogglesAndShortcutsOpen) {
    TempApp t;
    Preferences* p = t.p();
    MainGUI* ui = t.app.ui();
    ui->menueditpreferences->trigger();
    EXPECT_TRUE(p->isOpen());
    EXPECT_EQ(ui->plot_tab_area->currentWidget()->objectName(), "preferences_tab");
    EXPECT_TRUE(ui->pref_status_label->styleSheet().contains("lightseagreen"));
    p->toggle();
    EXPECT_FALSE(p->isOpen());
    EXPECT_TRUE(ui->pref_status_label->styleSheet().isEmpty());
    EXPECT_TRUE(key(t.app, Qt::Key_P, Qt::ShiftModifier));
    EXPECT_TRUE(p->isOpen());
    // Ctrl+S en la pestaña: aplica (no guarda el proyecto).
    p->tab()->tools_ncc_group_ncc_overlap_entry->setValue(60.0);
    EXPECT_TRUE(p->changed());
    EXPECT_TRUE(key(t.app, Qt::Key_S, Qt::ControlModifier));
    EXPECT_FALSE(p->changed());
    // Cerrar la pestaña con cambios pregunta si se guardan (respuesta automática: sí).
    p->tab()->tools_ncc_group_ncc_overlap_entry->setValue(65.0);
    ui->plot_tab_area->closeTab(ui->plot_tab_area->indexOf(p->tab()));
    EXPECT_EQ(js(readJson(t.dir.path() + "/current_defaults.FlatConfig")["tools_ncc_overlap"]), "65.0");
}

// set_tool_ui: las herramientas leen sus valores de app.defaults (lo que el usuario guarda en Preferencias).
TEST(GuiPreferences, ToolsReadTheirValuesFromThePreferences) {
    QTemporaryDir dir;
    {
        // Las preferencias habituales del usuario: NCC y Paint con 80 %, método estándar y una herramienta de 0,1.
        App app;
        app.setDataPath(dir.path());
        Preferences* p = app.preferences();
        p->open();
        PreferencesTab* t = p->tab();
        t->tools_ncc_group_ncc_overlap_entry->setValue(80.0);
        t->tools_ncc_group_ncc_method_combo->setIndexValue(0);
        t->tools_ncc_group_ncc_tool_dia_entry->setText("0.1");
        t->tools_paint_group_paintoverlap_entry->setValue(80.0);
        t->tools_paint_group_painttooldia_entry->setText("0.1, 0.2");
        t->tools_iso_group_overlap_entry->setValue(25.0);
        t->tools_cutout_group_cutout_tooldia_entry->setValue(1.2);
        t->geometry_opt_group_cutz_entry->setValue(-1.5);
        t->pref_save_button->click();
        EXPECT_FALSE(p->changed());  // volver a cargar el archivo no marca nada (LoudDict avisa solo si cambia)
    }
    App app;
    app.setDataPath(dir.path());
    EXPECT_EQ(js(app.defaults()["tools_ncc_tools"]), "0.1");  // eval("0.1"): un número
    EXPECT_EQ(PropertiesTool::pyStr(app.defaults()["tools_paint_tooldia"]).toStdString(), "[0.1, 0.2]");
    app.nccTool->setToolUi();
    EXPECT_EQ(app.nccTool->ncc_overlap_entry->value(), 80.0);
    EXPECT_EQ(app.nccTool->ncc_method_combo->currentIndex(), 0);
    EXPECT_EQ(app.nccTool->tools_table->rowCount(), 1);
    app.paintTool->setToolUi();
    EXPECT_EQ(app.paintTool->paintoverlap_entry->value(), 80.0);
    EXPECT_EQ(app.paintTool->tools_table->rowCount(), 2);  // la lista de diámetros de un archivo guardado
    app.isolationTool->setToolUi();
    EXPECT_EQ(app.isolationTool->iso_overlap_entry->value(), 25.0);
    app.cutoutTool->setToolUi();
    EXPECT_EQ(app.cutoutTool->dia->value(), 1.2);
    EXPECT_EQ(defaultGeometryTool().data.cutZ, -1.5);  // geometry_cutz
}

// on_toggle_units y scale_defaults contra el original (tests/tools/oracle_units.py): las 551 opciones después de pasar
// a pulgadas desde la pestaña, de volver a mm y de la tecla Q (que no escala las preferencias).
TEST(GuiPreferences, ToggleUnitsMatchesTheOriginal) {
    const JsonValue o = readJson(QString(FLATCAM_TEST_DATA_DIR) + "/preferences/units.json");
    TempApp t;
    t.app.setUnitsAutoAnswer(true);
    Preferences* p = t.p();
    auto compare = [&](const char* stage) {
        int diffs = 0;
        for (const auto& [k, v] : o[stage].members()) {
            // El tema, el idioma y la versión son del entorno del oráculo (fc_runtime), no del cambio de unidades.
            if (k == "global_theme" || k == "global_language" || k == "version") continue;
            if (js(p->value(k)) != js(v)) {
                ++diffs;
                ADD_FAILURE() << stage << " " << k << ": " << js(p->value(k)) << " != " << js(v);
            }
        }
        return diffs;
    };
    EXPECT_EQ(compare("startup"), 0);
    RadioSet* radio = p->tab()->general_app_group_units_radio;
    radio->setValue(QString("IN"));
    t.app.onToggleUnits(false);
    EXPECT_EQ(compare("to_inch"), 0);
    EXPECT_EQ(t.app.ui()->units_label->text(), "[in]");
    radio->setValue(QString("MM"));
    t.app.onToggleUnits(false);
    EXPECT_EQ(compare("back_to_mm"), 0);
    t.app.onToggleUnitsClick();
    EXPECT_EQ(compare("no_pref"), 0);
}

// Los objetos se convierten: un Gerber y un Excellon en pulgadas (las coordenadas divididas por 25,4).
TEST(GuiPreferences, ToggleUnitsConvertsTheObjects) {
    TempApp t;
    t.app.setUnitsAutoAnswer(true);
    const std::string dir = std::string(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    FlatCAMObj* g = t.app.openGerber(QString::fromStdString(dir + "Edge_Cuts.gbr"));
    ASSERT_NE(g, nullptr);
    const QRectF before = g->bounds();
    t.app.onToggleUnitsClick();
    EXPECT_EQ(t.p()->value("units").text(), "IN");
    EXPECT_NEAR(g->bounds().width(), before.width() / 25.4, 1e-9);
    EXPECT_NEAR(g->bounds().left(), before.left() / 25.4, 1e-9);
    // Lo que se abre después queda en pulgadas.
    FlatCAMObj* g2 = t.app.openGerber(QString::fromStdString(dir + "Edge_Cuts.gbr"));
    ASSERT_NE(g2, nullptr);
    EXPECT_NEAR(g2->bounds().width(), before.width() / 25.4, 1e-9);
}
