// Los manejadores de los grupos de la pestaña Preferencias (las señales que conecta cada `*PrefGroupUI` del original,
// `__init_color_pickers` de PreferencesUIManager y las asociaciones de archivos y palabras clave de app_Main).
#include <QSettings>
#include <QStringList>

#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCPrefWidgets.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

namespace {

/// `hex(v)[2:] if int(hex(v)[2:], 16) > 0 else '00'` (sin completar con ceros: 5 da "5").
std::string alphaHex(int v) { return v > 0 ? QString::number(v, 16).toStdString() : "00"; }

/// `s[a:b]` de Python sobre un texto.
std::string slice(const std::string& s, std::size_t a, std::size_t b = std::string::npos) {
    if (a >= s.size()) return {};
    return s.substr(a, b == std::string::npos ? std::string::npos : b - a);
}

const QStringList kGrbList = {"art", "bot", "bsm", "cmp", "crc", "crs", "dim", "g4", "gb0", "gb1", "gb2", "gb3", "gb5",
                              "gb6", "gb7", "gb8", "gb9", "gbd", "gbl", "gbo", "gbp", "gbr", "gbs", "gdo", "ger", "gko",
                              "gml", "gm1", "gm2", "gm3", "grb", "gtl", "gto", "gtp", "gts", "ly15", "ly2", "mil",
                              "outline", "pho", "plc", "pls", "smb", "smt", "sol", "spb", "spt", "ssb", "sst", "stc",
                              "sts", "top", "tsm"};
const QStringList kExcList = {"drd", "drl", "drill", "exc", "ncd", "tap", "txt", "xln"};
const QStringList kGcodeList = {"cnc", "din", "dnc", "ecs", "eia", "fan", "fgc", "fnc", "gc", "gcd", "gcode", "h",
                                "hnc", "i", "min", "mpf", "mpr", "nc", "ncc", "ncg", "ngc", "ncp", "out", "ply", "rol",
                                "sbp", "tap", "xpi"};
const QStringList kDefaultKeywords = {
    "Desktop", "Documents", "FlatConfig", "FlatPrj", "False", "Marius", "My Documents", "Paste_1", "Repetier",
    "Roland_MDX_20", "Users", "Toolchange_Custom", "Toolchange_Probe_MACH3", "Toolchange_manual", "True", "Users", "all",
    "auto", "axis", "axisoffset", "box", "center_x", "center_y", "columns", "combine", "connect", "contour", "default",
    "depthperpass", "dia", "diatol", "dist", "drilled_dias", "drillz", "dpp", "dwelltime", "extracut_length", "endxy",
    "enz", "f", "feedrate", "feedrate_z", "grbl_11", "GRBL_laser", "gridoffsety", "gridx", "gridy", "has_offset",
    "holes", "hpgl", "iso_type", "line_xyz", "margin", "marlin", "method", "milled_dias", "minoffset", "name", "offset",
    "opt_type", "order", "outname", "overlap", "passes", "postamble", "pp", "ppname_e", "ppname_g", "preamble",
    "radius", "ref", "rest", "rows", "shellvar_", "scale_factor", "spacing_columns", "spacing_rows", "spindlespeed",
    "startz", "startxy", "toolchange_xy", "toolchangez", "travelz", "tooldia", "use_threads", "value", "x", "x0", "x1",
    "y", "y0", "y1", "z_cut", "z_move"};

}  // namespace

void Preferences::connectGroups() {
    PreferencesTab* t = tab_;
    auto text = [this](const char* key) { return defaults_[key].text(); };
    auto color = [](FCColorEntry* e) { return e->value().toStdString(); };
    auto onEdit = [this](FCColorEntry* e, std::function<void()> f) {
        connect(e, &FCColorEntry::editingFinished, this, std::move(f));
    };
    // Un color con alfa propio: el campo cambia los 7 primeros caracteres y conserva el alfa guardado.
    auto keepAlpha = [this, text, color, onEdit](FCColorEntry* e, const char* key) {
        onEdit(e, [this, text, color, e, key] {
            set(key, JsonValue::string(slice(color(e), 0, 7) + slice(text(key), 7, 9)));
        });
    };
    // Un control de alfa: cambia el alfa de los dos colores.
    auto alpha = [this, text](FCSliderWithSpinner* s, const char* fill, const char* line) {
        connect(s, &FCSliderWithSpinner::valueChanged, this, [this, text, fill, line](int v) {
            set(fill, JsonValue::string(slice(text(fill), 0, 7) + alphaHex(v)));
            set(line, JsonValue::string(slice(text(line), 0, 7) + alphaHex(v)));
        });
    };
    auto plain = [this, color, onEdit](FCColorEntry* e, const char* key) {
        onEdit(e, [this, color, e, key] { set(key, JsonValue::string(color(e))); });
    };

    // General -> GUI (GeneralGUIPrefGroupUI). Error del original que se reproduce: el color de relleno de la selección
    // de izquierda a derecha se reemplaza por su propio alfa (`global_sel_fill[7:9]`).
    onEdit(t->general_gui_group_sf_color_entry, [this, text] {
        set("global_sel_fill", JsonValue::string(slice(text("global_sel_fill"), 7, 9)));
    });
    keepAlpha(t->general_gui_group_sl_color_entry, "global_sel_line");
    alpha(t->general_gui_group_left_right_alpha_entry, "global_sel_fill", "global_sel_line");
    keepAlpha(t->general_gui_group_alt_sf_color_entry, "global_alt_sel_fill");
    keepAlpha(t->general_gui_group_alt_sl_color_entry, "global_alt_sel_line");
    alpha(t->general_gui_group_right_left_alpha_entry, "global_alt_sel_fill", "global_alt_sel_line");
    plain(t->general_gui_group_draw_color_entry, "global_draw_color");
    plain(t->general_gui_group_sel_draw_color_entry, "global_sel_draw_color");
    plain(t->general_gui_group_proj_color_entry, "global_proj_item_color");
    plain(t->general_gui_group_proj_color_dis_entry, "global_proj_item_dis_color");
    // handle_style / handle_hdpi: van a los ajustes (se aplican al reiniciar).
    connect(t->general_gui_group_style_combo, &QComboBox::textActivated, this, [this](const QString& style) {
        QSettings(settingsFile(), QSettings::IniFormat).setValue("style", style);
    });
    connect(t->general_gui_group_hdpi_cb, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state) {
        QSettings(settingsFile(), QSettings::IniFormat).setValue("hdpi", static_cast<int>(state));
    });
    // General -> Aplicación: la pantalla de bienvenida y la consola al arrancar.
    connect(t->general_app_group_splash_cb, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state) {
        QSettings(settingsFile(), QSettings::IniFormat).setValue("splash_screen", state != Qt::Unchecked ? 1 : 0);
    });
    connect(t->general_app_group_shell_startup_cb, &QCheckBox::clicked, this, [this](bool state) {
        if (state != ui_->shell_dock->isVisible()) ui_->shell_dock->setVisible(state);
    });
    plain(t->general_app_set_group_mouse_cursor_entry, "global_cursor_color");

    // Gerber -> General.
    keepAlpha(t->gerber_gen_group_fill_color_entry, "gerber_plot_fill");
    keepAlpha(t->gerber_gen_group_line_color_entry, "gerber_plot_line");
    alpha(t->gerber_gen_group_gerber_alpha_entry, "gerber_plot_fill", "gerber_plot_line");
    connect(t->gerber_gen_group_clear_colors_button, &QPushButton::clicked, this, [this] {
        if (JsonValue* l = defaults_.find("gerber_color_list"); l && l->isArray()) l->items().clear();
        ui_->inform("[WARNING_NOTCL] " + tr_("Stored colors for Gerber objects are deleted."));
    });
    // Gerber -> Avanzadas: el "buffering" diferido solo sin buffering.
    connect(t->gerber_adv_opt_group_buffering_radio, &RadioSet::activatedCustom, this, [t](const QString& v) {
        t->gerber_adv_opt_group_delayed_buffer_cb->setDisabled(v != "no");
    });

    // Excellon -> General.
    keepAlpha(t->excellon_gen_group_fill_color_entry, "excellon_plot_fill");
    keepAlpha(t->excellon_gen_group_line_color_entry, "excellon_plot_line");
    alpha(t->excellon_gen_group_excellon_alpha_entry, "excellon_plot_fill", "excellon_plot_line");
    connect(t->excellon_gen_group_excellon_defaults_button, &QPushButton::clicked, this, [t] {
        t->excellon_gen_group_excellon_format_lower_in_entry->setValue(4);
        t->excellon_gen_group_excellon_format_upper_in_entry->setValue(2);
        t->excellon_gen_group_excellon_format_lower_mm_entry->setValue(3);
        t->excellon_gen_group_excellon_format_upper_mm_entry->setValue(3);
        t->excellon_gen_group_excellon_zeros_radio->setValue(QString("L"));
        t->excellon_gen_group_excellon_units_radio->setValue(QString("INCH"));
    });
    connect(t->excellon_gen_group_excellon_optimization_radio, &RadioSet::activatedCustom, this, [t] {
        const bool m = t->excellon_gen_group_excellon_optimization_radio->value().toString() == "M";
        t->excellon_gen_group_optimization_time_label->setDisabled(!m);
        t->excellon_gen_group_optimization_time_entry->setDisabled(!m);
    });
    // on_update_exc_export: con "Actualizar exportación" el formato de exportación sigue al general.
    auto formatChanged = [t] {
        const bool metric = t->excellon_gen_group_excellon_units_radio->value().toString().toUpper() == "METRIC";
        t->excellon_exp_group_format_whole_entry->setValue(
            (metric ? t->excellon_gen_group_excellon_format_upper_mm_entry : t->excellon_gen_group_excellon_format_upper_in_entry)
                ->value());
        t->excellon_exp_group_format_dec_entry->setValue(
            (metric ? t->excellon_gen_group_excellon_format_lower_mm_entry : t->excellon_gen_group_excellon_format_lower_in_entry)
                ->value());
    };
    auto exportLinks = std::make_shared<std::vector<QMetaObject::Connection>>();
    auto updateExport = [this, t, formatChanged, exportLinks](bool on) {
        for (const auto& c : *exportLinks) disconnect(c);
        exportLinks->clear();
        if (!on) return;
        for (FCSpinner* s : {t->excellon_gen_group_excellon_format_upper_in_entry, t->excellon_gen_group_excellon_format_lower_in_entry,
                             t->excellon_gen_group_excellon_format_upper_mm_entry, t->excellon_gen_group_excellon_format_lower_mm_entry})
            exportLinks->push_back(connect(s, &FCSpinner::returnPressed, this, formatChanged));
        exportLinks->push_back(connect(t->excellon_gen_group_excellon_zeros_radio, &RadioSet::activatedCustom, this,
                                       [t](const QString& v) { t->excellon_exp_group_zeros_radio->setValue(v + "Z"); }));
        exportLinks->push_back(connect(t->excellon_gen_group_excellon_units_radio, &RadioSet::activatedCustom, this,
                                       [t, formatChanged](const QString& v) {
                                           t->excellon_exp_group_excellon_units_radio->setValue(v);
                                           formatChanged();
                                       }));
    };
    connect(t->excellon_gen_group_update_excellon_cb, &QCheckBox::checkStateChanged, this,
            [updateExport](Qt::CheckState state) { updateExport(state != Qt::Unchecked); });
    updateExport(defaults_["excellon_update"].toBool());
    // Excellon -> Exportar: los ceros solo con formato decimal no.
    connect(t->excellon_exp_group_format_radio, &RadioSet::activatedCustom, this, [t] {
        const bool dec = t->excellon_exp_group_format_radio->value().toString() == "dec";
        t->excellon_exp_group_zeros_label->setDisabled(dec);
        t->excellon_exp_group_zeros_radio->setDisabled(dec);
    });

    // Geometría -> General: el color de la línea siempre opaco.
    onEdit(t->geometry_gen_group_line_color_entry, [this, color, t] {
        set("geometry_plot_line", JsonValue::string(slice(color(t->geometry_gen_group_line_color_entry), 0, 7) + "FF"));
    });
    connect(t->geometry_gen_group_opt_algorithm_radio, &RadioSet::activatedCustom, this, [t](const QString& v) {
        t->geometry_gen_group_optimization_time_label->setDisabled(v != "M");
        t->geometry_gen_group_optimization_time_entry->setDisabled(v != "M");
    });

    // CNC Job.
    keepAlpha(t->cncjob_gen_group_tfill_color_entry, "cncjob_travel_fill");
    keepAlpha(t->cncjob_gen_group_tline_color_entry, "cncjob_travel_line");
    alpha(t->cncjob_gen_group_cncjob_alpha_entry, "cncjob_travel_fill", "cncjob_travel_line");
    keepAlpha(t->cncjob_gen_group_fill_color_entry, "cncjob_plot_fill");
    keepAlpha(t->cncjob_gen_group_line_color_entry, "cncjob_plot_line");
    plain(t->cncjob_adv_opt_group_annotation_fontcolor_entry, "cncjob_annotation_fontcolor");

    // Herramientas.
    plain(t->tools_film_group_film_color_entry, "tools_film_color");
    plain(t->tools2_qrcode_group_fill_color_entry, "tools_qrcode_fill_color");
    plain(t->tools2_qrcode_group_back_color_entry, "tools_qrcode_back_color");

    // Utilidades: asociaciones de archivos y palabras clave (restore_extensions, delete_all_extensions,
    // add_extension, del_extension).
    struct List {
        FCTextArea* text;
        FCEntry* entry;
        QPushButton* restore;
        QPushButton* delAll;
        QPushButton* add;
        QPushButton* del;
        QStringList defaults;
    };
    QStringList excNoTxt = kExcList;
    excNoTxt.removeAll("txt");  // 'txt' no se asocia (hay demasiados .txt que no son Excellon)
    const List lists[] = {
        {t->fa_excellon_group_exc_list_text, t->fa_excellon_group_ext_entry, t->fa_excellon_group_restore_btn,
         t->fa_excellon_group_del_all_btn, t->fa_excellon_group_add_btn, t->fa_excellon_group_del_btn, excNoTxt},
        {t->fa_gcode_group_gco_list_text, t->fa_gcode_group_ext_entry, t->fa_gcode_group_restore_btn,
         t->fa_gcode_group_del_all_btn, t->fa_gcode_group_add_btn, t->fa_gcode_group_del_btn, kGcodeList},
        {t->fa_gerber_group_grb_list_text, t->fa_gerber_group_ext_entry, t->fa_gerber_group_restore_btn,
         t->fa_gerber_group_del_all_btn, t->fa_gerber_group_add_btn, t->fa_gerber_group_del_btn, kGrbList},
        {t->kw_group_kw_list_text, t->kw_group_kw_entry, t->kw_group_restore_btn, t->kw_group_del_all_btn,
         t->kw_group_add_btn, t->kw_group_del_btn, kDefaultKeywords},
    };
    for (const List& l : lists) {
        connect(l.restore, &QPushButton::clicked, this, [l] { l.text->setValue(l.defaults.join(", ")); });
        connect(l.delAll, &QPushButton::clicked, this, [l] { l.text->setValue(""); });
        auto current = [l] { return l.text->value().remove(' ').split(','); };
        connect(l.add, &QPushButton::clicked, this, [l, current] {
            const QString ext = l.entry->text();
            if (ext.isEmpty()) return;
            QStringList old = current();
            if (old.contains(ext)) return;
            old << ext;
            old.sort();  // list.sort() de Python: por código de carácter
            l.text->setValue(old.join(", "));
        });
        connect(l.del, &QPushButton::clicked, this, [l, current] {
            const QString ext = l.entry->text();
            if (ext.isEmpty()) return;
            QStringList old = current();
            if (!old.contains(ext)) return;
            old.removeOne(ext);
            old.sort();
            l.text->setValue(old.join(", "));
        });
    }
    // on_register_files: asocia las extensiones en el registro de Windows (en otros sistemas no hace nada: desvío,
    // el original solo funciona en Windows).
    struct Reg {
        QPushButton* btn;
        FCTextArea* text;
        const char* key;
        const char* msg;
    };
    const Reg regs[] = {
        {t->fa_excellon_group_exc_list_btn, t->fa_excellon_group_exc_list_text, "fa_excellon",
         "Selected Excellon file extensions registered with FlatCAM."},
        {t->fa_gcode_group_gco_list_btn, t->fa_gcode_group_gco_list_text, "fa_gcode",
         "Selected GCode file extensions registered with FlatCAM."},
        {t->fa_gerber_group_grb_list_btn, t->fa_gerber_group_grb_list_text, "fa_gerber",
         "Selected Gerber file extensions registered with FlatCAM."},
    };
    for (const Reg& r : regs) {
        connect(r.btn, &QPushButton::clicked, this, [this, r] {
#ifdef Q_OS_WIN
            QSettings reg("HKEY_CURRENT_USER\\Software\\Classes", QSettings::NativeFormat);
            QStringList exts = r.text->value().remove(' ').split(',');
            exts.removeAll("");
            for (const QString& e : exts) reg.setValue("." + e + "/.", "FlatCAM");
            for (const QString& e : QString::fromStdString(defaults_[r.key].text()).remove(' ').split(','))
                if (!exts.contains(e)) reg.remove("." + e);
            ui_->inform("[success] " + tr_(r.msg));
#else
            (void)r;
            ui_->inform("[WARNING_NOTCL] " + QString::fromUtf8("Las asociaciones de archivos solo se registran en Windows."));
#endif
        });
    }
}

void Preferences::initColorPickers() {
    PreferencesTab* t = tab_;
    auto alphaOf = [this](const char* key) {
        bool ok = false;
        const int v = QString::fromStdString(defaults_[key].text()).mid(7, 2).toInt(&ok, 16);
        return ok ? v : 0;
    };
    auto str = [this](const char* key) { return QString::fromStdString(defaults_[key].text()); };
    t->gerber_gen_group_fill_color_entry->setValue(str("gerber_plot_fill"));
    t->gerber_gen_group_line_color_entry->setValue(str("gerber_plot_line"));
    t->gerber_gen_group_gerber_alpha_entry->setValue(alphaOf("gerber_plot_fill"));
    t->excellon_gen_group_fill_color_entry->setValue(str("excellon_plot_fill"));
    t->excellon_gen_group_line_color_entry->setValue(str("excellon_plot_line"));
    t->excellon_gen_group_excellon_alpha_entry->setValue(alphaOf("excellon_plot_fill"));
    t->geometry_gen_group_line_color_entry->setValue(str("geometry_plot_line"));
    t->cncjob_gen_group_tfill_color_entry->setValue(str("cncjob_travel_fill"));
    t->cncjob_gen_group_tline_color_entry->setValue(str("cncjob_travel_line"));
    t->cncjob_gen_group_cncjob_alpha_entry->setValue(alphaOf("cncjob_travel_fill"));
    t->cncjob_gen_group_fill_color_entry->setValue(str("cncjob_plot_fill"));
    t->cncjob_gen_group_line_color_entry->setValue(str("cncjob_plot_line"));
    t->general_gui_group_sf_color_entry->setValue(str("global_sel_fill"));
    t->general_gui_group_sl_color_entry->setValue(str("global_sel_line"));
    t->general_gui_group_left_right_alpha_entry->setValue(alphaOf("global_sel_fill"));
    t->general_gui_group_alt_sf_color_entry->setValue(str("global_alt_sel_fill"));
    t->general_gui_group_alt_sl_color_entry->setValue(str("global_alt_sel_line"));
    // Como el original: el alfa de derecha a izquierda se toma de `global_sel_fill`, no de `global_alt_sel_fill`.
    t->general_gui_group_right_left_alpha_entry->setValue(alphaOf("global_sel_fill"));
    t->general_gui_group_draw_color_entry->setValue(str("global_draw_color"));
    t->general_gui_group_sel_draw_color_entry->setValue(str("global_sel_draw_color"));
    t->general_gui_group_proj_color_entry->setValue(str("global_proj_item_color"));
    t->general_gui_group_proj_color_dis_entry->setValue(str("global_proj_item_dis_color"));
    t->general_app_set_group_mouse_cursor_entry->setValue(str("global_cursor_color"));
    t->cncjob_adv_opt_group_annotation_fontcolor_entry->setValue(str("cncjob_annotation_fontcolor"));
    t->tools_film_group_film_color_entry->setValue(str("tools_film_color"));
    t->tools2_qrcode_group_fill_color_entry->setValue(str("tools_qrcode_fill_color"));
    t->tools2_qrcode_group_back_color_entry->setValue(str("tools_qrcode_back_color"));
}

void Preferences::resetAlphaControls() {
    // Los controles de alfa del original arrancan en 0 (FCSliderWithSpinner(0, 255, 1)): así __init_color_pickers los
    // cambia y sus manejadores reescriben el alfa de los colores (en minúsculas), como en el original.
    PreferencesTab* t = tab_;
    for (FCSliderWithSpinner* s : {t->gerber_gen_group_gerber_alpha_entry, t->excellon_gen_group_excellon_alpha_entry,
                                   t->cncjob_gen_group_cncjob_alpha_entry, t->general_gui_group_left_right_alpha_entry,
                                   t->general_gui_group_right_left_alpha_entry}) {
        const QSignalBlocker b(s);
        const QSignalBlocker b2(s->spinner);
        const QSignalBlocker b3(s->slider);
        s->setValue(0);
        s->slider->setValue(0);
    }
}

}  // namespace flatcam::gui
