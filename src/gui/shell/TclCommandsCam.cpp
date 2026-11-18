// Comandos de la consola Tcl de CAM (tclCommands/ del original): options, follow, bounding_box, non_copper_regions,
// cncjob y write_gcode.
#include <QRegularExpression>

#include "cam/CncJob.h"
#include "gui/App.h"
#include "gui/ExclusionAreasManager.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Preferences.h"
#include "gui/ProjectIO.h"
#include "gui/Translation.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/shell/TclCommand.h"
#include "gui/shell/TclCommands.h"
#include "gui/shell/TclHelpers.h"
#include "gui/tools/PropertiesTool.h"

namespace flatcam::gui {

namespace {

using namespace tclhelpers;

/** `solid_geometry` de un Gerber o una Geometría. */
ShapeList solidOf(FlatCAMObj* o) {
    if (auto* g = dynamic_cast<GerberObject*>(o)) return g->gerber().solidGeometry();
    if (auto* g = dynamic_cast<GeometryObject*>(o)) return g->solidGeometry();
    return {};
}

/** Una Geometría nueva como la de `new_object('geometry', ...)` con `set_ui` y la herramienta de `cnctooldia`. */
std::unique_ptr<GeometryObject> newSingleGeo(const QString& name, double tooldia) {
    auto g = std::make_unique<GeometryObject>(name);
    GeometryTool t = defaultGeometryTool();
    t.tooldia = tooldia;
    g->tools()[1] = t;
    return g;
}

/**
 * Las claves de `options` en el orden en que las inserta el original: `name` (FlatCAMObj), las del `__init__` de cada
 * tipo, las `<tipo>_*` de la aplicación y las de las herramientas en el orden de las opciones (`new_object`) y lo
 * demás (los límites) al final.
 */
QStringList optionsOrder(ObjectKind kind, const JsonValue& ops) {
    QStringList init{"name"};
    QString pre;
    QStringList extra;
    switch (kind) {
        case ObjectKind::Gerber:
            init << "plot" << "multicolored" << "solid" << "noncoppermargin" << "noncopperrounded" << "bboxmargin"
                 << "bboxrounded" << "aperture_display" << "follow" << "milling_type";
            pre = "gerber_";
            extra << "tools_iso_";
            break;
        case ObjectKind::Excellon:
            init << "plot" << "solid" << "multicolored" << "merge_fuse_tools" << "tooldia" << "milling_dia"
                 << "slot_tooldia" << "format_upper_in" << "format_lower_in" << "format_upper_mm" << "lower_mm"
                 << "zeros" << "units" << "update" << "optimization_type" << "search_time";
            pre = "excellon_";
            extra << "tools_drill_" << "tools_mill_";
            break;
        case ObjectKind::Geometry:
            init << "plot" << "multicolored" << "cutz" << "vtipdia" << "vtipangle" << "travelz" << "feedrate"
                 << "feedrate_z" << "feedrate_rapid" << "spindlespeed" << "dwell" << "dwelltime" << "multidepth"
                 << "depthperpass" << "extracut" << "extracut_length" << "endz" << "endxy" << "area_exclusion"
                 << "area_shape" << "area_strategy" << "area_overz" << "startz" << "toolchange" << "toolchangez"
                 << "toolchangexy" << "ppname_g" << "z_pdepth" << "feedrate_probe" << "cnctooldia";
            pre = "geometry_";
            extra << "tools_mill_";
            break;
        case ObjectKind::CNCJob:
            init << "plot" << "tooldia" << "append" << "prepend" << "dwell" << "dwelltime" << "type"
                 << "toolchange_macro" << "toolchange_macro_enable";
            pre = "cncjob_";
            break;
        case ObjectKind::Script:
            init << "plot" << "type" << "source_file";
            pre = "script_";
            break;
        case ObjectKind::Document:
            pre = "document_";
            break;
    }
    QStringList out;
    const auto add = [&](const QString& k) {
        if (!out.contains(k) && ops.find(k.toStdString())) out << k;
    };
    for (const QString& k : init) add(k);
    for (const QString& k : TclCommandMeta::defaultsOrder()) {
        if (k.startsWith(pre)) add(k.mid(pre.size()));
        for (const QString& e : extra)
            if (k.startsWith(e)) add(k);
    }
    for (const auto& [k, v] : ops.members()) add(QString::fromStdString(k));
    return out;
}

class Options : public TclCommand {
public:
    explicit Options(App* a) : TclCommand(a, "TclCommandOptions") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        FlatCAMObj* o = app_->collection()->byName(args.str("name"));
        if (!o) raiseTclError("'NoneType' object has no attribute 'options'");
        // '\n'.join("%s: %s" % (o, ops[o])): las opciones en el orden en que se guardan.
        QStringList lines;
        const JsonValue d = ProjectIO::toDict(*o, app_->appOptions());
        if (const JsonValue* ops = d.find("options"))
            for (const QString& k : optionsOrder(o->kind(), *ops))
                lines << k + ": " + PropertiesTool::pyStr(*ops->find(k.toStdString()));
        return lines.join('\n');
    }
};

class Follow : public TclCommand {
public:
    explicit Follow(App* a) : TclCommand(a, "TclCommandFollow") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_follow";
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) raiseTclError("Object not found: " + name);
        if (o->kind() != ObjectKind::Gerber) raiseTclError(QString("Expected GerberObject, got %1 %2.").arg(name, pyType(o)));
        // GerberObject.follow_geo: la geometría de seguimiento, con cnctooldia = tools_iso_tooldia.
        auto geo = newSingleGeo(outname, defNum("tools_iso_tooldia", 0.1));
        geo->solidGeometry() = static_cast<GerberObject*>(o)->gerber().followGeometry();
        geo->setSingleGeo(true);
        app_->addObject(std::move(geo));
        return std::nullopt;
    }
};

/** bounding_box y non_copper_regions: la caja (con margen y esquinas redondeadas o no) o la caja menos el cobre. */
class BoxRegion : public TclCommand {
public:
    BoxRegion(App* a, bool noncopper)
        : TclCommand(a, noncopper ? "TclCommandNregions" : "TclCommandBbox"), noncopper_(noncopper) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const QString outname = args.has("outname") ? args.str("outname") : name + (noncopper_ ? "_noncopper" : "_bbox");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) raiseTclError(QString("%1: %2").arg(tr_("Object not found"), name));
        if (o->kind() != ObjectKind::Gerber && o->kind() != ObjectKind::Geometry)
            raiseTclError(QString("%1 %2: %3.").arg(tr_("Expected GerberObject or GeometryObject, got"), name, pyType(o)));
        const char* pre = noncopper_ ? "gerber_noncopper" : "gerber_bbox";
        const double margin = args.has("margin") ? args.num("margin") : defNum(std::string(pre) + "margin", 0.1);
        bool rounded = false;
        if (args.has("rounded")) {
            rounded = args.hasValue("rounded") && pyTruthCapitalized(args.str("rounded"));
        } else {
            // bool(eval(defaults[...])): el valor de fábrica es un bool, y eval() solo acepta texto.
            const JsonValue& v = appDefaults()[std::string(pre) + "rounded"];
            if (!v.isString()) raiseTclError("eval() arg 1 must be a string, bytes or code object");
            rounded = pyEvalTruth(QString::fromStdString(v.text()));
        }
        const Shape geo = Shape::unaryUnion(solidOf(o));
        Shape box = geo.envelope().buffer(margin);
        if (!rounded) box = box.envelope();  // sin esquinas redondeadas
        auto out = std::make_unique<GeometryObject>(outname);
        out->tools()[1] = defaultGeometryTool();
        out->setSingleGeoShape(noncopper_ ? box.difference(geo) : box);
        app_->addObject(std::move(out), false);
        if (noncopper_) {
            // ui.plot_cb.toggle() del objeto de origen: deja de dibujarse (o vuelve a dibujarse).
            o->setPlotEnabled(!o->plotEnabled());
            app_->ui()->plotCanvas()->setLayerVisible(o->id(), o->plotEnabled());
        }
        return std::nullopt;
    }

private:
    bool noncopper_;
};

/** `args[key] if key in args and args[key] else defaults[defKey]` con un número. */
double argOrDefault(const TclArgs& args, const std::string& key, const std::string& defKey) {
    if (args.has(key) && args.hasValue(key) && args.num(key) != 0) return args.num(key);
    return defNum(defKey);
}

/**
 * Un X,Y escrito como texto ("1,2", "(1, 2)"): `len(eval(texto)) != 2` da el error del comando; después
 * `[float(eval(a)) for a in re.sub('[()\[\]]', '', texto).split(",")]`.
 */
std::array<double, 2> parseXY(const QString& text, const QString& formatError) {
    QString t = text;
    t.remove(QRegularExpression("[()\\[\\]]"));
    const QStringList parts = t.split(',', Qt::SkipEmptyParts);
    if (parts.size() == 1) {
        bool isInt = false;
        parts[0].trimmed().toLongLong(&isInt);
        throw TclError(QString("object of type '%1' has no len()").arg(isInt ? "int" : "float"));
    }
    if (parts.size() != 2) throw TclError(formatError);
    return {parts[0].trimmed().toDouble(), parts[1].trimmed().toDouble()};
}

/** El texto de un X,Y de las preferencias (`str(defaults[key])`), o "0, 0" si no hay. */
QString defaultXY(const std::string& key) {
    const JsonValue& v = appDefaults()[key];
    if (v.isNull() || (v.isString() && v.text().empty())) return "0, 0";
    if (v.isString()) return QString::fromStdString(v.text());
    return PropertiesTool::pyStr(v);
}

/** `app.inform.emit` de los avisos del generador de G-code, traducidos. */
void informMessages(App* app, const QStringList& msgs) {
    for (const QString& m : msgs) app->ui()->inform(m);
}

/** El mensaje de `new_object` cuando `initialize` lanza una excepción (sin el traceback de Python). */
void informInitializeFailed(App* app, const QString& kind, const QString& error) {
    app->ui()->inform("[ERROR_NOTCL] " + tr_("An internal error has occurred. See shell.\n") +
                      tr_("Object ({kind}) failed because: {error} \n\n").replace("{kind}", kind).replace("{error}", error));
}

/** cncjob: el G-code de una Geometría (generatecncjob o, en una multigeo, mtool_gen_cncjob). */
class Cncjob : public TclCommand {
public:
    explicit Cncjob(App* a) : TclCommand(a, "TclCommandCncjob") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const bool muted = args.has("muted") && args.hasValue("muted") && pyTruthCapitalized(args.str("muted"));
        if (!args.has("name")) {
            if (!muted) raiseTclError("Object name is missing");
            return QString("fail");
        }
        const QString name = args.str("name");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_cnc";
        FlatCAMObj* o = app_->collection()->byName(name, false);
        if (!o) {
            if (!muted) raiseTclError("Object not found: " + name);
            return QString("fail");
        }
        if (o->kind() != ObjectKind::Geometry) {
            if (!muted) raiseTclError(QString("Expected GeometryObject, got %1 %2.").arg(name, pyType(o)));
            return std::nullopt;
        }
        auto* geo = static_cast<GeometryObject*>(o);

        // Los valores de los argumentos o de las preferencias, como el original.
        double tooldia = 0;
        PyNumber tooldiaOption;
        if (args.has("dia") && args.hasValue("dia") && args.num("dia") != 0) {
            tooldia = args.num("dia");
            tooldiaOption = PyNumber(tooldia);
        } else {
            // geometry_cnctooldia es un texto ("2.4"): str() lo escribe tal cual.
            const JsonValue& v = appDefaults()["geometry_cnctooldia"];
            const QString t = v.isString() ? QString::fromStdString(v.text()) : PropertiesTool::pyStr(v);
            tooldia = t.toDouble();
            tooldiaOption = t.contains('.') || t.contains('e') ? PyNumber(tooldia) : PyNumber(t.toInt());
        }
        const double zCut = argOrDefault(args, "z_cut", "geometry_cutz");
        const double zMove = argOrDefault(args, "z_move", "geometry_travelz");
        const double feedrate = argOrDefault(args, "feedrate", "geometry_feedrate");
        const double feedrateZ = argOrDefault(args, "feedrate_z", "geometry_feedrate_z");
        const double feedrateRapid = argOrDefault(args, "feedrate_rapid", "geometry_feedrate_rapid");
        bool extracut = defBool("geometry_extracut");
        double extracutLength = defNum("geometry_extracut_length");
        if (args.has("extracut_length")) {
            extracut = true;
            extracutLength = args.hasValue("extracut_length") ? args.num("extracut_length") : 0.0;
        }
        bool multidepth = defBool("geometry_multidepth");
        double dpp = defNum("geometry_depthperpass");
        if (args.has("dpp")) {
            multidepth = true;
            if (args.hasValue("dpp")) dpp = args.num("dpp");
        }
        std::optional<double> startZ;
        if (args.has("startz") && args.hasValue("startz") && args.num("startz") != 0) startZ = args.num("startz");
        else if (auto d = defPy("geometry_startz")) startZ = d->value();
        const double endZ = argOrDefault(args, "endz", "geometry_endz");
        const QString endxyText =
            args.has("endxy") && args.hasValue("endxy") && !args.str("endxy").isEmpty() ? args.str("endxy") : defaultXY("geometry_endxy");
        const std::array<double, 2> endXY = parseXY(
            endxyText, "The entered value for 'endxy' needs to have the format x,y or in format (x, y) - no spaces "
                       "allowed. But always two comma separated values.");
        std::optional<int> spindle;
        if (args.has("spindlespeed") && args.hasValue("spindlespeed") && args.num("spindlespeed") != 0)
            spindle = static_cast<int>(args.num("spindlespeed"));
        bool dwell = defBool("geometry_dwell");
        double dwellTime = defNum("geometry_dwelltime");
        if (args.has("dwelltime")) {
            dwell = true;
            if (args.hasValue("dwelltime")) dwellTime = args.num("dwelltime");
        }
        const QString pp = args.has("pp") && args.hasValue("pp") && !args.str("pp").isEmpty() ? args.str("pp")
                                                                                              : defStr("geometry_ppname_g");
        bool toolchange = defBool("geometry_toolchange");
        double toolchangeZ = defNum("geometry_toolchangez");
        if (args.has("toolchangez")) {
            toolchange = true;
            if (args.hasValue("toolchangez")) toolchangeZ = args.num("toolchangez");
        }
        const QString tcxyText = args.has("toolchangexy") && args.hasValue("toolchangexy") &&
                                         !args.str("toolchangexy").isEmpty()
                                     ? args.str("toolchangexy")
                                     : defaultXY("geometry_toolchangexy");
        const std::array<double, 2> toolchangeXY = parseXY(
            tcxyText, "The entered value for 'toolchangexy' needs to have the format x,y or in format (x, y) - no "
                      "spaces allowed. But always two comma separated values.");

        if (geo->isSingleGeo()) {
            // generatecncjob(**args): no tiene el argumento `muted`.
            if (args.has("muted"))
                raiseTclError("GeometryObject.generatecncjob() got an unexpected keyword argument 'muted'");
            Geometry2Settings s;
            s.preprocessor = pp.toStdString();
            s.units = appUnits().toStdString();
            s.coordsDecimals = defInt("cncjob_coords_decimals", 4);
            s.frDecimals = defInt("cncjob_fr_decimals", 2);
            s.decimals = appDecimals();
            s.stepsPerCircle = defInt("cncjob_steps_per_circle", 64);
            s.toolDia = tooldia;
            s.toolDiaOption = tooldiaOption;
            s.tolerance = defNum("global_tolerance", 0.005) / 20;
            s.cutZ = zCut;
            s.travelZ = zMove;
            s.feedrate = feedrate;
            s.feedrateZ = feedrateZ;
            s.feedrateRapid = feedrateRapid;
            s.spindleSpeed = spindle;  // spindlespeed if spindlespeed else options['spindlespeed'] (0): int o None
            s.dwell = dwell;
            s.dwellTime = dwellTime;
            s.multidepth = multidepth;
            s.depthPerPass = dpp;
            s.toolchange = toolchange;
            s.toolchangeZ = toolchangeZ;
            s.toolchangeXY = toolchangeXY;
            s.extracut = extracut;
            s.extracutLength = extracutLength;
            s.startZ = startZ;
            s.endZ = endZ;
            s.endXY = endXY;
            s.segX = defNum("geometry_segx");
            s.segY = defNum("geometry_segy");
            s.fPlunge = defBool("geometry_f_plunge");
            s.zPdepth = defNum("geometry_z_pdepth", -0.02);
            s.feedrateProbe = defNum("geometry_feedrate_probe", 75);
            s.machinist = defBool("global_machinist_setting");
            s.headerBounds = geo->optionBoundsBox();  // job_obj.options['xmin'...] = self.options['xmin'...]
            s.geometryName = geo->name().toStdString();
            s.jobName = outname.toStdString();
            s.exclusionAreas = app_->excAreas()->areas();
            auto job = std::make_unique<CncJob>();
            std::vector<CncMessage> msgs;
            const bool ok = job->generateFromGeometry2(geo->solidGeometry(), s, msgs);
            informMessages(app_, cncMessagesText(msgs));
            if (!ok) {
                // job_init: `res, start_gcode = 'fail'` no se puede desempaquetar.
                if (job->lastError() == "fail")
                    informInitializeFailed(app_, "cncjob", "too many values to unpack (expected 2)");
                else informInitializeFailed(app_, "cncjob", QString::fromStdString(job->lastError()));
                return std::nullopt;
            }
            // job_init: gcode_parse sobre el cuerpo (self.gcode) y el aviso final.
            informMessages(app_, gcodeParseMessages(job->gcode()));
            app_->ui()->inform("[success] " + tr_("Finished G-Code processing") + "...");
            GcodeHeaderInfo h;
            h.version = "2024.4";
            h.versionDate = "2024/04/15";
            h.type = "Geometry";
            h.units = appUnits().toStdString();
            app_->addObject(std::make_unique<CNCJobObject>(outname, std::move(job), h, std::vector<CncToolRow>{}),
                            false);
            return std::nullopt;
        }
        // Multigeo: una copia de las herramientas con los parámetros del comando y mtool_gen_cncjob.
        std::map<int, GeometryTool> local = geo->tools();
        std::vector<int> ids;
        for (auto& [id, t] : local) {
            ids.push_back(id);
            GeometryToolSettings& d = t.data;
            d.cutZ = zCut;
            d.travelZ = zMove;
            d.feedrate = feedrate;
            d.feedrateZ = feedrateZ;
            d.feedrateRapid = feedrateRapid;
            d.multidepth = multidepth;
            d.extracut = extracut;
            d.extracutLength = extracutLength;
            d.depthPerPass = dpp;
            t.toolchange = toolchange;
            d.toolchangeZ = toolchangeZ;
            d.toolchangeXY = toolchangeXY;
            d.startZ = startZ;
            d.endZ = endZ;
            d.endXY = endXY;
            d.spindleSpeed = spindle ? *spindle : 0;
            d.dwell = dwell;
            d.dwellTime = dwellTime;
            t.ppname = pp;
        }
        QString error;
        QStringList messages;
        std::unique_ptr<CNCJobObject> job = geo->makeCncJob(ids, error, app_->excAreas()->areas(), &local, &messages);
        for (const QString& m : messages) app_->ui()->inform(m);
        if (!job) {
            app_->ui()->inform(error);
            return std::nullopt;
        }
        job->setName(outname);
        app_->addObject(std::move(job), false);
        return std::nullopt;
    }
};

/** write_gcode: exporta el G-code de un CNC Job (export_gcode con from_tcl). */
class WriteGCode : public TclCommand {
public:
    explicit WriteGCode(App* a) : TclCommand(a, "TclCommandWriteGCode") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const QString filename = args.str("filename");
        const std::string preamble = args.has("preamble") ? args.str("preamble").toStdString() : std::string();
        const std::string postamble = args.has("postamble") ? args.str("postamble").toStdString() : std::string();
        const bool muted = args.has("muted") && args.hasValue("muted") && pyTruthCapitalized(args.str("muted"));
        FlatCAMObj* o = app_->collection()->byName(name);
        auto* job = dynamic_cast<CNCJobObject*>(o);
        if (!job) {
            if (muted) return std::nullopt;
            const QString type = !o ? "NoneType"
                                    : QString(o->kind() == ObjectKind::Gerber     ? "GerberObject"
                                              : o->kind() == ObjectKind::Excellon ? "ExcellonObject"
                                              : o->kind() == ObjectKind::Geometry ? "GeometryObject"
                                              : o->kind() == ObjectKind::Script   ? "ScriptObject"
                                                                                  : "DocumentObject");
            return QString("Operation failed: '%1' object has no attribute 'export_gcode'").arg(type);
        }
        if (!job->saveTo(filename, preamble, postamble))
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("No such file or directory"));
        return std::nullopt;
    }
};

/** drillcncjob: el G-code de un Excellon con generate_from_excellon_by_tool (sin la interfaz). */
class Drillcncjob : public TclCommand {
public:
    explicit Drillcncjob(App* a) : TclCommand(a, "TclCommandDrillcncjob") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* o = app_->collection()->byName(name);  // get_by_name(name): distingue mayúsculas
        const QString outname = args.has("outname") ? args.str("outname") : name + "_cnc";
        const bool muted = args.has("muted") && args.hasValue("muted") && pyTruthCapitalized(args.str("muted"));
        if (!o) {
            if (!muted) raiseTclError("Object not found: " + name);
            return QString("fail");
        }
        if (o->kind() != ObjectKind::Excellon) {
            if (!muted) raiseTclError(QString("Expected ExcellonObject, got %1 %2.").arg(name, pyType(o)));
            return QString("fail");
        }
        auto* exc = static_cast<ExcellonObject*>(o);
        const Excellon& ex = exc->excellon();
        // job_init: lo que falla adentro lo atrapa new_object ("Object (cncjob) failed because: ...").
        try {
            const auto has = [&](const char* k) { return args.has(k) && args.hasValue(k); };
            // Los valores "truthy" de Python: un 0 toma el de las preferencias.
            const auto num = [&](const char* k, const char* def) {
                if (has(k) && args.num(k) != 0) return PyNumber(args.num(k));
                return defPy(def).value_or(PyNumber(0));
            };
            if (args.has("drilled_dias") && args.str("drilled_dias") != "all") {
                // obj.tools[tool]["C"]: en la 2024.4 las herramientas no tienen "C" (KeyError: 'C').
                QString err = !ex.drillTools().empty() && !args.str("drilled_dias").split(',', Qt::SkipEmptyParts).isEmpty()
                                  ? "'C'"
                                  : "'diatol'";
                if (args.has("diatol") && err == "'diatol'") err = "invalid syntax";
                throw TclError("Bad tools: " + err);
            }
            ExcellonByToolSettings s;
            s.zCut = has("drillz") ? PyNumber(args.num("drillz")) : defPy("tools_drill_cutz").value_or(PyNumber(0));
            if (args.has("toolchangez")) {
                s.toolchange = true;
                s.zToolchange = has("toolchangez") ? PyNumber(args.num("toolchangez"))
                                                   : defPy("tools_drill_toolchangez").value_or(PyNumber(0));
            } else {
                s.toolchange = defBool("tools_drill_toolchange");
                s.zToolchange = PyNumber(defNum("tools_drill_toolchangez"));
            }
            // `args["tools_drill_toolchangexy"]`: la clave que no existe (KeyError).
            if (args.has("toolchangexy")) throw TclError("'tools_drill_toolchangexy'");
            const QString xyTc = defaultXY("tools_drill_toolchangexy");
            parseXY(xyTc, "The entered value for 'toolchangexy' needs to have the format x,y or in format (x, y) - no "
                          "spaces allowed. But always two comma separated values.");
            s.xyToolchange = xyTc.toStdString();
            // El comando guarda la Z final en job_obj.endz, que nadie lee: end_code usa z_end, el 2.0 del constructor.
            s.endZ = PyNumber(2.0);
            const QString xyEnd = has("endxy") && !args.str("endxy").isEmpty() ? args.str("endxy")
                                                                               : defaultXY("tools_drill_endxy");
            parseXY(xyEnd, "The entered value for 'xy_end' needs to have the format x,y or in format (x, y) - no spaces "
                           "allowed. But always two comma separated values.");
            s.xyEnd = xyEnd.toStdString();
            s.optType = has("opt_type") && !args.str("opt_type").isEmpty() ? args.str("opt_type").toStdString() : "B";
            s.preprocessor = has("pp") && !args.str("pp").isEmpty() ? args.str("pp").toStdString()
                                                                    : defStr("tools_drill_ppname_e").toStdString();
            if (args.has("dpp")) {
                s.multidepth = true;
                if (!has("dpp")) throw TclError("'dpp'");  // obj.options["dpp"]
                s.depthPerCut = PyNumber(args.num("dpp"));
            } else {
                s.multidepth = defBool("tools_drill_multidepth");
                s.depthPerCut = defPy("tools_drill_depthperpass").value_or(PyNumber(0));
            }
            s.zMove = num("travelz", "tools_drill_travelz");
            s.feedrate = s.zFeedrate = num("feedrate_z", "tools_drill_feedrate_z");
            s.feedrateRapid = num("feedrate_rapid", "tools_drill_feedrate_rapid");
            if (args.has("spindlespeed")) s.spindleSpeed = PyNumber(has("spindlespeed") ? args.num("spindlespeed") : 0.0);
            s.spindleDir = defStr("tools_drill_spindledir").toStdString();
            if (args.has("dwelltime")) {
                s.dwell = true;
                s.dwellTime = has("dwelltime") ? PyNumber(args.num("dwelltime")) : PyNumber(defNum("tools_drill_dwelltime"));
            } else {
                s.dwell = defBool("tools_drill_dwell");
                s.dwellTime = defPy("tools_drill_dwelltime").value_or(PyNumber(0));
            }
            if (has("startz")) s.startZ = PyNumber(args.num("startz"));
            else if (auto d = defPy("tools_drill_startz"); d && d->value() != 0) s.startZ = d;
            else s.startZ = defPy("tools_drill_travelz");
            s.units = appUnits().toStdString();
            s.decimals = appDecimals();
            s.coordsDecimals = defInt("cncjob_coords_decimals", 4);
            s.frDecimals = defInt("cncjob_fr_decimals", 2);
            s.stepsPerCircle = defInt("cncjob_steps_per_circle", 64);
            s.machinist = defBool("global_machinist_setting");
            const QRectF b = exc->bounds();
            s.bounds = BoundingBox{b.left(), b.top(), b.right(), b.bottom()};
            s.exclusionAreas = app_->excAreas()->areas();
            auto job = std::make_unique<CncJob>();
            std::vector<CncMessage> msgs;
            const bool ok = job->generateFromExcellonByTool(ex, s, msgs);
            informMessages(app_, cncMessagesText(msgs));
            if (!ok) {
                if (job->lastError() != "fail") throw TclError(QString::fromStdString(job->lastError()));
                // initialize devolvió 'fail': new_object avisa igual y no crea nada.
                app_->shellMessage(QString("%1 %2. %3 seconds executing initialize().")
                                       .arg(tr_("New object with name:"), outname, QString::number(0.0, 'f', 6)));
                return std::nullopt;
            }
            informMessages(app_, gcodeParseMessages(job->gcode()));
            GcodeHeaderInfo h;
            h.version = "2024.4";
            h.versionDate = "2024/04/15";
            h.type = "Geometry";  // gc_header se arma en CNCJobObject.__init__, antes de options['type'] = 'Excellon'
            h.units = appUnits().toStdString();
            app_->addObject(std::make_unique<CNCJobObject>(outname, std::move(job), h, std::vector<CncToolRow>{}),
                            false);
        } catch (const TclError& e) {
            informInitializeFailed(app_, "cncjob", e.text());
        }
        return std::nullopt;
    }
};

}  // namespace

void addCamTclCommands(App* app, std::vector<std::unique_ptr<TclCommand>>& out) {
    out.push_back(std::make_unique<Options>(app));
    out.push_back(std::make_unique<Follow>(app));
    out.push_back(std::make_unique<BoxRegion>(app, false));
    out.push_back(std::make_unique<BoxRegion>(app, true));
    out.push_back(std::make_unique<Cncjob>(app));
    out.push_back(std::make_unique<Drillcncjob>(app));
    out.push_back(std::make_unique<WriteGCode>(app));
}

}  // namespace flatcam::gui
