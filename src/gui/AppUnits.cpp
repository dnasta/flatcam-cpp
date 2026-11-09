// Cambiar las unidades de la aplicación (App.on_toggle_units, on_toggle_units_click, scale_defaults y
// set_screen_units del original).
#include <set>

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

namespace {

/// Las claves de `defaults` que se escalan al cambiar de unidades (la lista `dimensions` de on_toggle_units).
const char* const kDimensions[] = {
    // General
    "global_gridx", "global_gridy", "global_snap_max", "global_tolerance", "global_tpdf_bmargin", "global_tpdf_tmargin",
    "global_tpdf_rmargin", "global_tpdf_lmargin",
    // Gerber
    "gerber_noncoppermargin", "gerber_bboxmargin", "gerber_editor_newsize", "gerber_editor_lin_pitch",
    "gerber_editor_buff_f", "gerber_editor_newdim", "gerber_editor_ma_low", "gerber_editor_ma_high",
    // Excellon
    "excellon_milling_dia", "excellon_tooldia", "excellon_slot_tooldia", "excellon_editor_newdia",
    "excellon_editor_lin_pitch", "excellon_editor_slot_lin_pitch", "excellon_editor_slot_length",
    // Geometría
    "geometry_cutz", "geometry_depthperpass", "geometry_travelz", "geometry_feedrate", "geometry_feedrate_rapid",
    "geometry_toolchangez", "geometry_feedrate_z", "geometry_toolchangexy", "geometry_cnctooldia", "geometry_endz",
    "geometry_endxy", "geometry_extracut_length", "geometry_z_pdepth", "geometry_feedrate_probe", "geometry_startz",
    "geometry_segx", "geometry_segy", "geometry_area_overz",
    // CNC Job
    "cncjob_tooldia", "cncjob_al_travelz", "cncjob_al_probe_depth", "cncjob_al_grbl_jog_step", "cncjob_al_grbl_jog_fr",
    "cncjob_al_grbl_travelz",
    // Isolation
    "tools_iso_tool_vtipdia", "tools_iso_tooldia", "tools_iso_tool_cutz",
    // Perforación
    "tools_drill_cutz", "tools_drill_depthperpass", "tools_drill_travelz", "tools_drill_endz", "tools_drill_endxy",
    "tools_drill_feedrate_z", "tools_drill_toolchangez", "tools_drill_drill_overlap", "tools_drill_offset",
    "tools_drill_toolchangexy", "tools_drill_startz", "tools_drill_feedrate_rapid", "tools_drill_feedrate_probe",
    "tools_drill_z_pdepth", "tools_drill_area_overz",
    // NCC
    "tools_ncc_tools", "tools_ncc_margin", "tools_ncc_offset_value", "tools_ncc_cutz", "tools_ncc_tipdia",
    "tools_ncc_newdia",
    // Corte
    "tools_cutout_tooldia", "tools_cutout_margin", "tools_cutout_z", "tools_cutout_depthperpass", "tools_cutout_gapsize",
    "tools_cutout_gap_depth", "tools_cutout_mb_dia", "tools_cutout_mb_spacing",
    // Paint
    "tools_paint_tooldia", "tools_paint_offset", "tools_paint_cutz", "tools_paint_tipdia", "tools_paint_newdia",
    // PCB a 2 caras, Film, Panelizar, Calculadoras, Transformaciones
    "tools_2sided_drilldia", "tools_film_boundary", "tools_film_scale_stroke", "tools_panelize_spacing_columns",
    "tools_panelize_spacing_rows", "tools_panelize_constrainx", "tools_panelize_constrainy", "tools_calc_vshape_tip_dia",
    "tools_calc_vshape_cut_z", "tools_transform_ref_point", "tools_transform_offset_x", "tools_transform_offset_y",
    "tools_transform_buffer_dis",
    // Pasta de soldar
    "tools_solderpaste_tools", "tools_solderpaste_new", "tools_solderpaste_z_start", "tools_solderpaste_z_dispense",
    "tools_solderpaste_z_stop", "tools_solderpaste_z_travel", "tools_solderpaste_z_toolchange",
    "tools_solderpaste_xy_toolchange", "tools_solderpaste_frxy", "tools_solderpaste_frz",
    "tools_solderpaste_frz_dispense",
    // Esquinas, reglas, QR, "copper thieving", fiduciales, calibración, extraer taladros, perforar, invertir
    "tools_corners_thickness", "tools_corners_length", "tools_corners_margin", "tools_cr_trace_size_val",
    "tools_cr_c2c_val", "tools_cr_c2o_val", "tools_cr_s2s_val", "tools_cr_s2sm_val", "tools_cr_s2o_val",
    "tools_cr_sm2sm_val", "tools_cr_ri_val", "tools_cr_h2h_val", "tools_cr_dh_val", "tools_qrcode_border_size",
    "tools_copper_thieving_clearance", "tools_copper_thieving_margin", "tools_copper_thieving_dots_dia",
    "tools_copper_thieving_dots_spacing", "tools_copper_thieving_squares_size",
    "tools_copper_thieving_squares_spacing", "tools_copper_thieving_lines_size",
    "tools_copper_thieving_lines_spacing", "tools_copper_thieving_rb_margin", "tools_copper_thieving_rb_thickness",
    "tools_copper_thieving_mask_clearance", "tools_fiducials_dia", "tools_fiducials_margin",
    "tools_fiducials_line_thickness", "tools_cal_travelz", "tools_cal_verz", "tools_cal_toolchangez",
    "tools_cal_toolchange_xy", "tools_edrills_hole_fixed_dia", "tools_edrills_circular_ring",
    "tools_edrills_oblong_ring", "tools_edrills_square_ring", "tools_edrills_rectangular_ring",
    "tools_edrills_others_ring", "tools_punch_hole_fixed_dia", "tools_punch_circular_ring", "tools_punch_oblong_ring",
    "tools_punch_square_ring", "tools_punch_rectangular_ring", "tools_punch_others_ring", "tools_invert_margin"};

/// Las que son listas de números (o un número o un texto con comas).
const std::set<std::string> kListKeys = {
    "geometry_cnctooldia", "tools_ncc_tools", "tools_solderpaste_tools", "tools_iso_tooldia", "tools_paint_tooldia",
    "tools_transform_ref_point", "tools_cal_toolchange_xy", "gerber_editor_newdim", "tools_drill_toolchangexy",
    "tools_drill_endxy", "geometry_toolchangexy", "geometry_endxy", "tools_solderpaste_xy_toolchange"};

/// `dec_format`: float('%.*f' % (dec, val)).
JsonValue decFormat(double v, int dec) { return JsonValue::real(QString::number(v, 'f', dec).toDouble()); }

/// `float(v)` de Python (número, booleano o texto con un número).
std::optional<double> toFloat(const JsonValue& v) {
    if (v.isNumber() || v.isBool()) return v.toDouble();
    if (v.isString()) {
        bool ok = false;
        const double d = QString::fromStdString(v.text()).trimmed().toDouble(&ok);
        if (ok) return d;
    }
    return std::nullopt;
}

/// `bool(v)` de Python.
bool truthy(const JsonValue& v) {
    if (v.isNull()) return false;
    if (v.isBool() || v.isNumber()) return v.toDouble() != 0;
    if (v.isString()) return !v.text().empty();
    if (v.isArray()) return !v.items().empty();
    return true;
}

}  // namespace

void App::scaleDefaults(double factor) {
    Preferences* p = prefs_;
    const int dec = appDecimals();
    for (const char* dim : kDimensions) {
        const JsonValue v = p->value(dim);
        if (kListKeys.count(dim)) {
            if (!truthy(v)) continue;
            JsonValue list;
            if (v.isString()) {
                list = pyEvalLiteral(QString::fromStdString(v.text()));  // eval(...): falla -> se saltea
                if (list.isNull()) continue;
            } else {
                list = v;
            }
            std::vector<JsonValue> items;
            if (list.isNumber()) items = {list};
            else if (list.isArray()) items = list.items();
            else continue;
            JsonValue conv = JsonValue::array();
            for (const JsonValue& x : items) {
                const auto f = toFloat(x);
                if (!f) return;  // float() falla: el original se corta con la excepción
                conv.push(decFormat(*f * factor, dec));
            }
            p->set(dim, conv);
        } else if (std::string(dim) == "global_gridx" || std::string(dim) == "global_gridy") {
            if (auto f = toFloat(v)) p->set(dim, decFormat(*f * factor, dec));
        } else if (truthy(v)) {
            if (auto f = toFloat(v)) p->set(dim, decFormat(*f * factor, dec));
        }
    }
}

void App::setScreenUnits(const QString& units) { ui_->units_label->setText("[" + units.toLower() + "]"); }

void App::onToggleUnitsClick() {
    // on_toggle_units_click (tecla Q y Editar -> Cambiar unidades): cambia la opción de la pestaña sin escalar las
    // preferencias (no_pref=True).
    RadioSet* r = prefs_->tab()->general_app_group_units_radio;
    unitsRadioMuted_ = true;  // el original desconecta activated_custom mientras cambia la opción
    r->setValue(prefs_->value("units").text() == "MM" ? QString("IN") : QString("MM"));
    unitsRadioMuted_ = false;
    onToggleUnits(true);
}

void App::onToggleUnits(bool noPref) {
    if (toggleUnitsIgnore_) return;
    RadioSet* radio = prefs_->tab()->general_app_group_units_radio;
    const QString newUnits = radio->value().toString().toUpper();
    if (newUnits == QString::fromStdString(prefs_->value("units").text()).toUpper()) return;
    const double factor = newUnits == "MM" ? 25.4 : 1 / 25.4;
    bool ok = true;
    if (!unitsAutoAnswer_) {
        QMessageBox box(ui_.get());
        box.setWindowTitle(tr_("Toggle Units"));
        box.setWindowIcon(QIcon(":/images/toggle_units32.png"));
        box.setIcon(QMessageBox::Question);
        box.setText(tr_("Changing the units of the project\nwill scale all objects.\n\nDo you want to continue?"));
        QPushButton* bOk = box.addButton(tr_("Ok"), QMessageBox::AcceptRole);
        box.addButton(tr_("Cancel"), QMessageBox::RejectRole);
        box.setDefaultButton(bOk);
        box.exec();
        ok = box.clickedButton() == bOk;
    } else {
        ok = *unitsAutoAnswer_;
    }
    double valX = 0, valY = 0;
    if (ok) {
        if (!noPref) {
            prefs_->readForm();
            scaleDefaults(factor);
            // defaults_write_form(fl_units=...): con unidades solo escribe la grilla (el resto ya lo escribió el
            // aviso de cada cambio).
            prefs_->writeFormField("global_gridx");
            prefs_->writeFormField("global_gridy");
            prefs_->set("units", JsonValue::string(newUnits.toStdString()));
            Q_EMIT prefs_->optionsToProject();  // on_options_app2project
        }
        const Units u = newUnits == "MM" ? Units::Millimeters : Units::Inches;
        for (FlatCAMObj* o : collection_->objects()) {
            o->convertUnits(u);
            objectChanged(o);  // las propiedades guardadas y el panel (build_ui)
        }
        // La grilla de la barra: global_gridx por el factor (la Y igual a la X si están enlazadas; así, con gridx,
        // también en el original).
        valX = toFloatOr(prefs_->value("global_gridx")) * factor;
        valY = ui_->grid_gap_link_cb->isChecked() ? valX : toFloatOr(prefs_->value("global_gridx")) * factor;
        plotAll();
        setScreenUnits(newUnits);
        shouldWeSave_ = true;
        ui_->inform("[success] " + tr_("Converted units to") + ": " + newUnits);
    } else {
        toggleUnitsIgnore_ = true;  // deshacer el cambio de la opción
        radio->setValue(QString::fromStdString(prefs_->value("units").text()).toUpper() == "MM" ? QString("IN")
                                                                                                  : QString("MM"));
        toggleUnitsIgnore_ = false;
        valX = toFloatOr(prefs_->value("global_gridx"));
        valY = toFloatOr(prefs_->value("global_gridy"));
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
    }
    // defaults_read_form(): todas las opciones desde la pestaña, salvo la grilla.
    prefs_->readForm();
    prefs_->set("global_gridx", JsonValue::real(valX));
    prefs_->set("global_gridy", JsonValue::real(valY));
    const int dec = appDecimals();
    ui_->grid_gap_x_entry->setValue(valX, dec);
    ui_->grid_gap_y_entry->setValue(valY, dec);
}

double App::toFloatOr(const JsonValue& v, double def) { return toFloat(v).value_or(def); }

}  // namespace flatcam::gui
