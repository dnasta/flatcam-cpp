// Comandos de la consola Tcl de CAM (tclCommands/ del original): options, follow, bounding_box, non_copper_regions,
// cncjob y write_gcode.
#include <algorithm>
#include <cmath>
#include <functional>

#include <QRegularExpression>

#include "core/OriginalBugs.h"
#include "cam/CncJob.h"
#include "cam/PolygonClearing.h"
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
#include "gui/tools/PaintTool.h"
#include "gui/tools/PropertiesTool.h"
#include "tools/Isolation.h"

namespace flatcam::gui {

namespace {

using namespace tclhelpers;

/** `solid_geometry` de un Gerber o una Geometría. */
ShapeList solidOf(FlatCAMObj* o) {
    if (auto* g = dynamic_cast<GerberObject*>(o)) return g->gerber().solidGeometry();
    if (auto* g = dynamic_cast<GeometryObject*>(o)) return g->solidGeometry();
    return {};
}

/** `obj.bounds()` con la precisión del original (el mínimo y el máximo de los límites de cada figura). */
std::optional<BoundingBox> exactBounds(FlatCAMObj* o) {
    ShapeList shapes = solidOf(o);
    if (auto* e = dynamic_cast<ExcellonObject*>(o))
        for (const auto& [id, t] : e->excellon().drillTools())
            shapes.insert(shapes.end(), t.solidGeometry.begin(), t.solidGeometry.end());
    std::optional<BoundingBox> b;
    for (const Shape& s : shapes) {
        const auto sb = s.bounds();
        if (!sb) continue;
        if (!b) b = *sb;
        else b = BoundingBox{std::min(b->xmin, sb->xmin), std::min(b->ymin, sb->ymin), std::max(b->xmax, sb->xmax),
                             std::max(b->ymax, sb->ymax)};
    }
    return b;
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
        } else if (!reproduceOriginalBugs()) {
            rounded = defBool(std::string(pre) + "rounded");  // corregido: la preferencia es un valor lógico
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
        if (noncopper_ && reproduceOriginalBugs()) {
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
    // Corregido: un 0 escrito vale (el original lo prueba como "truthy" y toma la preferencia).
    if (args.has(key) && args.hasValue(key) && (args.num(key) != 0 || !reproduceOriginalBugs())) return args.num(key);
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
        // Corregido: los límites de la cabecera son los de ahora (el original usa los de `options`, viejos después de
        // transformar con Tcl).
        if (!reproduceOriginalBugs()) geo->captureOptionBounds();

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
        if (args.has("startz") && args.hasValue("startz") && (args.num("startz") != 0 || !reproduceOriginalBugs()))
            startZ = args.num("startz");
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
            if (args.has("muted") && reproduceOriginalBugs())  // corregido: -muted vale
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
        // get_by_name(name): distingue mayúsculas (corregido: como los demás comandos, no)
        FlatCAMObj* o = app_->collection()->byName(name, reproduceOriginalBugs());
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
        const bool fixed = !reproduceOriginalBugs();
        Excellon picked;  // corregido: con -drilled_dias, solo las herramientas de esos diámetros (con -diatol en %)
        const bool someDias = fixed && args.has("drilled_dias") && args.str("drilled_dias") != "all";
        if (someDias) {
            std::vector<double> dias;
            for (const QString& d : args.str("drilled_dias").split(',', Qt::SkipEmptyParts)) {
                bool ok = false;
                dias.push_back(d.trimmed().toDouble(&ok));
                if (!ok) raiseTclError("Bad tools: " + d);
            }
            const double tol = args.has("diatol") && args.hasValue("diatol") ? args.num("diatol") / 100.0 : 0.0;
            for (const auto& [id, t] : exc->excellon().drillTools())
                for (double d : dias)
                    if (std::fabs(t.tooldia - d) <= d * tol + 1e-9) {
                        picked.addDrillTool(id, t.tooldia, t.drills);
                        break;
                    }
        }
        const Excellon& ex = someDias ? picked : exc->excellon();
        // job_init: lo que falla adentro lo atrapa new_object ("Object (cncjob) failed because: ...").
        try {
            const auto has = [&](const char* k) { return args.has(k) && args.hasValue(k); };
            // Los valores "truthy" de Python: un 0 toma el de las preferencias.
            const auto num = [&](const char* k, const char* def) {
                if (has(k) && args.num(k) != 0) return PyNumber(args.num(k));
                return defPy(def).value_or(PyNumber(0));
            };
            if (!fixed && args.has("drilled_dias") && args.str("drilled_dias") != "all") {
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
            // `args["tools_drill_toolchangexy"]`: la clave que no existe (KeyError; corregido: el X,Y escrito).
            if (args.has("toolchangexy") && !fixed) throw TclError("'tools_drill_toolchangexy'");
            const QString xyTc = fixed && has("toolchangexy") ? args.str("toolchangexy")
                                                              : defaultXY("tools_drill_toolchangexy");
            parseXY(xyTc, "The entered value for 'toolchangexy' needs to have the format x,y or in format (x, y) - no "
                          "spaces allowed. But always two comma separated values.");
            s.xyToolchange = xyTc.toStdString();
            // El comando guarda la Z final en job_obj.endz, que nadie lee: end_code usa z_end, el 2.0 del constructor.
            s.endZ = PyNumber(2.0);
            if (fixed && has("endz")) s.endZ = PyNumber(args.num("endz"));  // corregido: -endz vale
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
            // gc_header se arma en CNCJobObject.__init__, antes de options['type'] = 'Excellon' (corregido: Excellon)
            h.type = fixed ? "Excellon" : "Geometry";
            h.units = appUnits().toStdString();
            app_->addObject(std::make_unique<CNCJobObject>(outname, std::move(job), h, std::vector<CncToolRow>{}),
                            false);
        } catch (const TclError& e) {
            informInitializeFailed(app_, "cncjob", e.text());
        }
        return std::nullopt;
    }
};

/** milldrills y millslots: el original mira `obj.drills` / `obj.slots`, que la 2024.4 no tiene: fallan siempre. */
class MillHoles : public TclCommand {
public:
    MillHoles(App* a, bool slots) : TclCommand(a, slots ? "TclCommandMillSlots" : "TclCommandMillDrills"), slots_(slots) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        FlatCAMObj* o = app_->collection()->byName(args.str("name"));
        if (args.has("use_thread") && args.hasValue("use_thread")) pyTruthCapitalized(args.str("use_thread"));
        raiseTclError(QString("'%1' object has no attribute '%2'").arg(pyClassName(o), slots_ ? "slots" : "drills"));
        return std::nullopt;
    }

private:
    bool slots_;
};

/** cutout: el corte rectangular con puentes de la caja del objeto agrandada el margen más el radio de la fresa. */
class Cutout : public TclCommand {
public:
    explicit Cutout(App* a) : TclCommand(a, "TclCommandCutout") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const double marginPar = args.has("margin") ? args.num("margin") : defNum("tools_cutout_margin");
        const double diaPar = args.has("dia") ? args.num("dia") : defNum("tools_cutout_tooldia");
        const QString gaps = args.has("gaps") ? args.str("gaps") : PropertiesTool::pyStr(appDefaults()["tools_cutout_gaps_ff"]);
        const double gapsizePar = args.has("gapsize") ? args.num("gapsize") : defNum("tools_cutout_gapsize");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_cutout";
        FlatCAMObj* o = app_->collection()->byName(name);
        // geo_init_me: lo que falla adentro lo atrapa new_object.
        if (!o) {
            informInitializeFailed(app_, "geometry", "'NoneType' object has no attribute 'bounds'");
        } else if (gaps != "tb" && gaps != "lr" && gaps != "4") {
            informInitializeFailed(app_, "geometry", "'" + gaps + "'");
        } else {
            const double margin = marginPar + diaPar / 2;
            const double gapSize = diaPar + gapsizePar;
            const BoundingBox b0 = exactBounds(o).value_or(BoundingBox{0, 0, 0, 0});
            const double minx = b0.xmin - margin, maxx = b0.xmax + margin, miny = b0.ymin - margin,
                         maxy = b0.ymax + margin;
            const double midx = 0.5 * (minx + maxx), midy = 0.5 * (miny + maxy), hgap = 0.5 * gapSize;
            const std::vector<Point2D> pts{{midx - hgap, maxy}, {minx, maxy}, {minx, midy + hgap}, {minx, midy - hgap},
                                           {minx, miny},        {midx - hgap, miny}, {midx + hgap, miny},
                                           {maxx, miny},        {maxx, midy - hgap}, {maxx, midy + hgap},
                                           {maxx, maxy},        {midx + hgap, maxy}};
            std::vector<std::vector<int>> cuts;
            if (gaps == "tb") cuts = {{0, 1, 4, 5}, {6, 7, 10, 11}};
            else if (gaps == "lr") cuts = {{9, 10, 1, 2}, {3, 4, 7, 8}};
            else cuts = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}};
            ShapeList lines;
            for (const auto& c : cuts) {
                std::vector<Point2D> seg;
                for (int i : c) seg.push_back(pts[static_cast<std::size_t>(i)]);
                lines.push_back(Shape::lineString(seg));
            }
            auto out = std::make_unique<GeometryObject>(outname);
            out->tools()[1] = defaultGeometryTool();
            out->setSingleGeoShape(Shape::unaryUnion(lines));
            app_->addObject(std::move(out), false);
        }
        app_->ui()->inform("[success] Rectangular-form Cutout operation finished.");
        return std::nullopt;
    }
};

/** isolate: GerberObject.isolate (las pasadas de aislamiento en una Geometría o una por pasada). */
class Isolate : public TclCommand {
public:
    explicit Isolate(App* a) : TclCommand(a, "TclCommandIsolate") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_iso";
        const bool follow = args.has("follow") && args.hasValue("follow") && !args.str("follow").isEmpty();
        bool combine = false;
        if (args.has("combine")) {
            combine = args.hasValue("combine") && pyTruthCapitalized(args.str("combine"));
        } else {
            // bool(eval(defaults["tools_iso_combine_passes"])): de fábrica es un bool y eval() solo acepta texto.
            const JsonValue& v = appDefaults()["tools_iso_combine_passes"];
            if (!v.isString()) raiseTclError("eval() arg 1 must be a string, bytes or code object");
            combine = pyEvalTruth(QString::fromStdString(v.text()));
        }
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) raiseTclError("Object not found: " + name);
        if (o->kind() != ObjectKind::Gerber) raiseTclError(QString("Expected GerberObject, got %1 %2.").arg(name, pyType(o)));
        const ShapeList work = static_cast<GerberObject*>(o)->gerber().solidGeometry();
        const double dia = args.has("dia") ? args.num("dia") : defNum("tools_iso_tooldia");
        const int passes = args.has("passes") ? static_cast<int>(args.num("passes")) : static_cast<int>(defNum("tools_iso_passes"));
        const double overlap = (args.has("overlap") ? args.num("overlap") : defNum("tools_iso_overlap")) / 100.0;
        const bool invert = defStr("tools_iso_milling_type") == "cl";
        const int isoT = args.has("iso_type") ? static_cast<int>(args.num("iso_type")) : 2;
        const int steps = defInt("gerber_circle_steps", 64);
        // generate_envelope de una pasada: la envolvente como la deja el original (un Polygon o un MultiPolygon).
        const auto envelope = [&](int pass) -> std::optional<Shape> {
            const double off = dia * ((2 * pass + 1) / 2.0) - (pass * overlap * dia);
            ShapeList geom;
            bool single = false;
            if (follow) {
                geom = work;  // isolation_geometry(follow=...) devuelve la geometría tal cual
            } else {
                if (isoT < 0 || isoT > 2) return std::nullopt;  // "fail": iterar el texto deja un MultiPolygon vacío
                geom = flatcam::Isolation::isolationGeometry(work, off, static_cast<flatcam::IsolationType>(isoT), steps);
                single = isoT == 2 && geom.size() == 1;  // unary_union de una sola figura: un Polygon
            }
            if (!invert) return single ? geom.front() : Shape::collection(geom);
            const ShapeList rev = flatcam::Isolation::invertDirection(geom);
            if (single) return rev.front();
            return Shape::collection(rev);
        };
        const auto makeGeo = [&](const QString& iso, ShapeList solid, bool multi) {
            auto g = newSingleGeo(iso, dia);
            g->solidGeometry() = solid;
            g->tools()[1].solidGeometry = solid;
            if (!multi) g->setSingleGeo(true);
            app_->addObject(std::move(g), false);
        };
        const auto empty = [](const ShapeList& l) {
            for (const Shape& g : l)
                if (!g.isEmpty()) return false;
            return true;
        };
        if (combine) {
            ShapeList solid;
            for (int i = 0; i < passes; ++i) {
                const auto env = envelope(i);
                solid.push_back(env.value_or(Shape::collection({})));
            }
            if (empty(solid)) {
                informInitializeFailed(app_, "geometry", "Empty Geometry");
                return std::nullopt;
            }
            makeGeo(outname, solid, passes > 1);
        } else {
            for (int i = 0; i < passes; ++i) {
                const auto env = envelope(i);
                const ShapeList solid{env.value_or(Shape::collection({}))};
                if (empty(solid)) {
                    informInitializeFailed(app_, "geometry", "Empty Geometry");
                    continue;
                }
                makeGeo(outname, solid, false);
            }
        }
        return std::nullopt;
    }
};

/** geocutout: el contorno (de una Geometría, o el aislamiento exterior de un Gerber) menos los rectángulos de los puentes. */
class GeoCutout : public TclCommand {
public:
    explicit GeoCutout(App* a) : TclCommand(a, "TclCommandGeoCutout") {}

protected:
    /** substract_rectangle_geo: las líneas de @p geo (los polígonos, por sus anillos) menos el rectángulo. */
    static Shape subtractRectangle(const ShapeList& geo, double x0, double y0, double x1, double y1) {
        ShapeList flat;
        std::function<void(const Shape&)> add = [&](const Shape& g) {
            if (g.isEmpty() && g.numParts() == 0) return;
            if (isMultiType(g.type())) {
                for (const Shape& p : g.parts()) add(p);
            } else if (g.type() == ShapeType::Polygon) {
                flat.push_back(g.exterior());
                for (const Shape& h : g.interiors()) flat.push_back(h);
            } else {
                flat.push_back(g);
            }
        };
        for (const Shape& g : geo) add(g);
        const Shape box = Shape::polygon({{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}});
        ShapeList diffs;
        for (const Shape& t : flat)
            if (t.type() == ShapeType::LineString || t.type() == ShapeType::LinearRing) diffs.push_back(t.difference(box));
        return Shape::unaryUnion(diffs);
    }

    static bool isMultiType(ShapeType t) {
        return t == ShapeType::MultiPolygon || t == ShapeType::MultiLineString || t == ShapeType::MultiPoint ||
               t == ShapeType::GeometryCollection;
    }

    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const double margin = args.has("margin") ? args.num("margin") : defNum("tools_cutout_margin");
        const double dia = args.has("dia") ? args.num("dia") : defNum("tools_cutout_tooldia");
        const QString gaps = args.has("gaps") ? args.str("gaps") : PropertiesTool::pyStr(appDefaults()["tools_cutout_gaps_ff"]);
        double gapsize = args.has("gapsize") ? args.num("gapsize") : defNum("tools_cutout_gapsize");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_cutout";
        FlatCAMObj* o = app_->collection()->byName(name);
        if (dia == 0) {
            app_->ui()->inform("[WARNING] " + tr_("Tool Diameter is zero value. Change it to a positive real number."));
            return QString("Tool Diameter is zero value. Change it to a positive real number.");
        }
        static const QStringList valid{"lr", "tb", "2lr", "2tb", "4", "8"};
        if (!valid.contains(gaps)) {
            app_->ui()->inform("[WARNING] " + tr_("Gaps value can be only one of: 'lr', 'tb', '2lr', '2tb', 4 or 8."));
            return std::nullopt;
        }
        if (!o) raiseTclError("'NoneType' object has no attribute 'bounds'");
        const BoundingBox b = exactBounds(o).value_or(BoundingBox{0, 0, 0, 0});
        const double px = 0.5 * (b.xmin + b.xmax) + margin, py = 0.5 * (b.ymin + b.ymax) + margin;
        const double lx = (b.xmax - b.xmin) + margin * 2, ly = (b.ymax - b.ymin) + margin * 2;
        gapsize = gapsize / 2 + dia / 2;
        ShapeList geo;
        if (o->kind() == ObjectKind::Geometry) {
            geo = static_cast<GeometryObject*>(o)->solidGeometry();
        } else if (o->kind() == ObjectKind::Gerber) {
            geo = flatcam::Isolation::isolationGeometry(static_cast<GerberObject*>(o)->gerber().solidGeometry(), dia / 2,
                                                        flatcam::IsolationType::Exterior, defInt("gerber_circle_steps", 64),
                                                        JoinStyle::Mitre);
        } else {
            app_->ui()->inform("[ERROR] " + tr_("Cancelled. Object type is not supported."));
            return std::nullopt;
        }
        Shape cut;
        bool any = false;
        const auto sub = [&](double x0, double y0, double x1, double y1) {
            cut = subtractRectangle(any ? ShapeList{cut} : geo, x0, y0, x1, y1);
            any = true;
        };
        if (gaps == "8" || gaps == "2lr") {
            sub(b.xmin - gapsize, py - gapsize + ly / 4, b.xmax + gapsize, py + gapsize + ly / 4);
            sub(b.xmin - gapsize, py - gapsize - ly / 4, b.xmax + gapsize, py + gapsize - ly / 4);
        }
        if (gaps == "8" || gaps == "2tb") {
            sub(px - gapsize + lx / 4, b.ymin - gapsize, px + gapsize + lx / 4, b.ymax + gapsize);
            sub(px - gapsize - lx / 4, b.ymin - gapsize, px + gapsize - lx / 4, b.ymax + gapsize);
        }
        if (gaps == "4" || gaps == "lr") sub(b.xmin - gapsize, py - gapsize, b.xmax + gapsize, py + gapsize);
        if (gaps == "4" || gaps == "tb") sub(px - gapsize, b.ymin - gapsize, px + gapsize, b.ymax + gapsize);
        auto out = std::make_unique<GeometryObject>(outname);
        out->tools()[1] = defaultGeometryTool();
        out->setSingleGeoShape(cut);
        if (o->kind() == ObjectKind::Geometry && o->plotEnabled()) {  // app_obj.disable_plots(objects=[cutout_obj])
            o->setPlotEnabled(false);
            app_->ui()->plotCanvas()->setLayerVisible(o->id(), false);
        }
        app_->ui()->inform("[success] " + tr_("Any-form Cutout operation finished."));
        app_->addObject(std::move(out), false);
        return std::nullopt;
    }
};

/** panelize: copias del objeto en filas y columnas, separadas por la caja más el espaciado (Geometría o Excellon). */
class Panelize : public TclCommand {
public:
    explicit Panelize(App* a) : TclCommand(a, "TclCommandPanelize") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) return QString("Object not found: %1").arg(name);
        FlatCAMObj* box = args.has("box") ? app_->collection()->byName(args.str("box")) : o;
        const int columns = args.has("columns") ? static_cast<int>(args.num("columns")) : 0;
        const int rows = args.has("rows") ? static_cast<int>(args.num("rows")) : 0;
        if (!args.has("columns") && !args.has("rows"))
            return QString("ERROR: Specify either -columns or -rows. The one not specified it will assumed to be 0");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_panelized";
        const bool threaded = args.has("use_thread") && args.hasValue("use_thread") && pyTruthCapitalized(args.str("use_thread"));
        // int(float): el espaciado se trunca.
        const double spCols = args.has("spacing_columns") ? std::trunc(args.num("spacing_columns")) : 5;
        const double spRows = args.has("spacing_rows") ? std::trunc(args.num("spacing_rows")) : 5;
        if (!box) raiseTclError("'NoneType' object has no attribute 'bounds'");
        const BoundingBox b = exactBounds(box).value_or(BoundingBox{0, 0, 0, 0});
        const double lx = b.xmax - b.xmin + spCols, ly = b.ymax - b.ymin + spRows;
        const auto copies = [&](const ShapeList& src) {
            ShapeList out;
            double cy = 0.0;
            for (int r = 0; r < rows; ++r) {
                double cx = 0.0;
                for (int c = 0; c < columns; ++c) {
                    for (const Shape& g : src) out.push_back(g.translate(cx, cy));
                    cx += lx;
                }
                cy += ly;
            }
            return out;
        };
        app_->ui()->inform("Generating panel ... Please wait.");
        if (o->kind() == ObjectKind::Excellon) {
            // job_init_excellon: tools.copy() comparte los diccionarios y create_geometry los vacía antes de fallar
            // con las claves 'drills', 'slots' y 'solid_geometry' que agregó; después bounds() falla y no se crea.
            static_cast<ExcellonObject*>(o)->excellon().clearToolGeometry();
            app_->shellMessage(QString("%1 %2. %3 seconds executing initialize().")
                                   .arg(tr_("New object with name:"), outname, QString::number(0.0, 'f', 6)));
        } else {
            auto out = std::make_unique<GeometryObject>(outname);
            if (o->kind() == ObjectKind::Geometry) {
                auto* g = static_cast<GeometryObject*>(o);
                out->tools() = g->tools();
                if (!g->isSingleGeo()) {
                    for (auto& [id, t] : out->tools()) t.solidGeometry = copies(g->tools().at(id).solidGeometry);
                } else {
                    out->solidGeometry() = copies(g->solidGeometry());
                    out->setSingleGeo(true);
                }
            } else {
                out->tools()[1] = defaultGeometryTool();
                out->solidGeometry() = copies(solidOf(o));
                out->setSingleGeo(true);
            }
            app_->addObject(std::move(out), false);
        }
        // '[success]' % _("Done."): sin hilo es un TypeError; con hilo lo atrapa job_thread.
        if (!threaded) raiseTclError("not all arguments converted during string formatting");
        return std::nullopt;
    }
};

/** Un Excellon de una herramienta con esos agujeros (`init_obj.tools = {'1': {...}}` + `create_geometry`). */
std::unique_ptr<ExcellonObject> drillObject(const QString& name, double dia, const std::vector<Point2D>& drills) {
    auto ex = std::make_unique<Excellon>();
    ex->addDrillTool(1, dia, drills);
    ex->createGeometry();
    return std::make_unique<ExcellonObject>(name, std::move(ex));
}

/** aligndrill: los agujeros pedidos (o los extremos de una grilla sobre la caja) y sus reflejos en el eje. */
class AlignDrill : public TclCommand {
public:
    explicit AlignDrill(App* a) : TclCommand(a, "TclCommandAlignDrill") {}

protected:
    /** `eval("[" + holes + "]")` para una lista de pares `(x, y)`. */
    static std::vector<Point2D> parseHoles(const QString& text) {
        static const QRegularExpression pair(
            R"(^\s*\(\s*([-+]?[0-9.]+(?:[eE][-+]?[0-9]+)?)\s*,\s*([-+]?[0-9.]+(?:[eE][-+]?[0-9]+)?)\s*\)\s*(,|$))");
        std::vector<Point2D> out;
        QString rest = text;
        while (!rest.trimmed().isEmpty()) {
            const auto m = pair.match(rest);
            if (!m.hasMatch()) raiseTclError("invalid syntax (<string>, line 1)");
            out.push_back({m.captured(1).toDouble(), m.captured(2).toDouble()});
            rest = rest.mid(m.capturedEnd());
        }
        return out;
    }

    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_aligndrill";
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) return QString("Object not found: %1").arg(name);
        if (o->kind() != ObjectKind::Geometry && o->kind() != ObjectKind::Gerber && o->kind() != ObjectKind::Excellon)
            return QString("ERROR: Only Gerber, Geometry and Excellon objects can be used.");
        const QString axis = args.str("axis").toUpper();
        const bool holesArg = args.has("holes");
        if (!(holesArg || (args.has("grid") && args.has("gridoffset"))))
            return QString("ERROR: Specify -holes or -grid with -gridoffset ");
        const std::vector<Point2D> holes = holesArg ? parseHoles(args.str("holes")) : std::vector<Point2D>{};
        if (axis != "X" && axis != "Y") raiseTclError("'" + axis + "'");
        const double xscale = axis == "X" ? 1.0 : -1.0, yscale = axis == "X" ? -1.0 : 1.0;
        if (!args.has("dia")) raiseTclError("'dia'");
        const double dia = args.num("dia");
        double px = 0, py = 0;
        std::optional<BoundingBox> bb;
        if (args.has("box")) {
            FlatCAMObj* box = app_->collection()->byName(args.str("box"));
            if (!box) return QString("Object box not found: %1").arg(args.str("box"));
            bb = exactBounds(box).value_or(BoundingBox{0, 0, 0, 0});
            px = 0.5 * (bb->xmin + bb->xmax);
            py = 0.5 * (bb->ymin + bb->ymax);
        } else {
            px = py = args.has("dist") ? args.num("dist") : 0.0;
        }
        // alligndrill_init_me
        std::vector<Point2D> drills;
        bool created = true;
        const auto addWithMirror = [&](const Point2D& p) {  // affinity.scale(point, xscale, yscale, origin=(px, py))
            drills.push_back(p);
            drills.push_back({xscale * p.x + (px - px * xscale), yscale * p.y + (py - py * yscale)});
        };
        if (holesArg) {
            for (const Point2D& h : holes) addWithMirror(h);
        } else if (bb) {
            if (!args.has("minoffset")) {
                informInitializeFailed(app_, "excellon", "'minoffset'");
                created = false;
            } else {
                const double axisoffset = args.has("axisoffset") ? args.num("axisoffset") : 0;
                const double grid = args.num("grid"), minoff = args.num("minoffset");
                const double lo = axis == "X" ? bb->xmin : bb->ymin, hi = axis == "X" ? bb->xmax : bb->ymax;
                double first = args.num("gridoffset");
                while ((lo - minoff) < first) first -= grid;
                double last = args.num("gridoffset");
                while ((hi + minoff) > last) last += grid;
                if (axis == "X") {
                    addWithMirror({first, axisoffset});
                    addWithMirror({last, axisoffset});
                } else {
                    addWithMirror({axisoffset, first});
                    addWithMirror({axisoffset, last});
                }
            }
        }
        if (created) {
            // Sin -box y con -grid, init devuelve un texto antes de armar las herramientas: queda un Excellon vacío.
            if (!holesArg && !bb) app_->addObject(std::make_unique<ExcellonObject>(outname, std::make_unique<Excellon>()), false);
            else app_->addObject(drillObject(outname, dia, drills), false);
        }
        return QString("Ok. Align Drills Excellon object created");
    }
};

/** aligndrillgrid: un Excellon con una grilla de agujeros. */
class AlignDrillGrid : public TclCommand {
public:
    explicit AlignDrillGrid(App* a) : TclCommand(a, "TclCommandAlignDrillGrid") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString outname = args.has("outname") ? args.str("outname") : QString("new_aligndrill_grid");
        const double offx = args.has("gridoffsetx") ? args.num("gridoffsetx") : 0;
        const double offy = args.has("gridoffsety") ? args.num("gridoffsety") : 0;
        if (!args.has("dia")) raiseTclError("'dia'");
        const double dia = args.num("dia");
        std::vector<Point2D> drills;
        double cy = 0;
        for (int r = 0; r < static_cast<int>(args.num("rows")); ++r) {
            double cx = 0;
            for (int c = 0; c < static_cast<int>(args.num("columns")); ++c) {
                drills.push_back({cx + offx, cy + offy});
                cx += args.num("gridx");
            }
            cy += args.num("gridy");
        }
        app_->addObject(drillObject(outname, dia, drills), false);
        return std::nullopt;
    }
};

/** `float('%.*f' % (decimals, v))`. */
double roundedDia(double v, int decimals = 4) { return QString::number(v, 'f', decimals).toDouble(); }

/** `bool(eval(texto.capitalize()))` de un argumento o `bool(eval(str(defaults[clave])))`. */
bool argTruth(const TclArgs& args, const char* name, const char* key) {
    if (args.has(name)) return args.hasValue(name) && pyTruthCapitalized(args.str(name));
    return pyEvalTruth(PropertiesTool::pyStr(appDefaults()[key]));
}

/** Las figuras de `solid_geometry` de un objeto y si es una lista (Gerber, Geometría y Excellon). */
ShapeList solidWithExcellon(FlatCAMObj* o, bool& isList) {
    isList = true;
    if (auto* g = dynamic_cast<GeometryObject*>(o)) isList = g->solidIsList();
    ShapeList shapes = solidOf(o);
    if (auto* e = dynamic_cast<ExcellonObject*>(o))
        for (const auto& [id, t] : e->excellon().drillTools())
            shapes.insert(shapes.end(), t.solidGeometry.begin(), t.solidGeometry.end());
    return shapes;
}

/** Las partes de una figura que da un polígono o un `MultiPolygon` (`MultiPolygon([p])` o `.geoms`). */
ShapeList polygonsOf(const Shape& s) {
    if (s.type() == ShapeType::Polygon) return {s};
    return s.parts();
}

/** ncc_clear: NonCopperClear.clear_copper_tcl (sin la interfaz de la herramienta). */
class NccClear : public TclCommand {
public:
    explicit NccClear(App* a) : TclCommand(a, "TclCommandCopperClear") {}

protected:
    /** Una herramienta de `tools_storage` (`ncc_tools`). */
    struct StoredTool {
        double dia;          ///< `float('%.4f' % tool)`.
        ShapeList solid;     ///< `solid_geometry`.
    };

    /** El índice de la primera herramienta con ese diámetro (los dos redondeados a 4 decimales). */
    static std::size_t toolFor(const std::vector<StoredTool>& tools, double tool) {
        for (std::size_t i = 0; i < tools.size(); ++i)
            if (roundedDia(tools[i].dia) == roundedDia(tool)) return i;
        return 0;  // current_uid queda en 1
    }

    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* obj = app_->collection()->byName(name);
        if (!obj) return QString("Object not found: %1").arg(name);
        const QString tooldia = args.has("tooldia") ? args.str("tooldia") : PropertiesTool::pyStr(appDefaults()["tools_ncc_tools"]);
        const double overlap = (args.has("overlap") ? args.num("overlap") : defNum("tools_ncc_overlap")) / 100.0;
        const QString order = args.has("order") ? args.str("order") : defStr("tools_ncc_order");
        const double margin = args.has("margin") ? args.num("margin") : defNum("tools_ncc_margin");
        // El método se compara con 0 y 1 pero es un texto: siempre termina en clear_polygon3 (líneas).
        const bool connect = argTruth(args, "connect", "tools_ncc_connect");
        const bool contour = argTruth(args, "contour", "tools_ncc_contour");
        const bool hasOffset = args.has("offset");
        const double offset = hasOffset ? args.num("offset") : 0.0;
        std::vector<double> tools;
        for (const QString& d : tooldia.split(',')) {
            if (d.isEmpty()) continue;
            bool ok = false;
            const double v = d.trimmed().toDouble(&ok);
            if (!ok) raiseTclError(QString("invalid syntax (<string>, line 1)"));
            tools.push_back(v);
        }
        const bool rest = argTruth(args, "rest", "tools_ncc_rest");
        const QString outname = args.has("outname") ? args.str("outname") : name + (rest ? "_ncc" : "_ncc_rm");
        std::vector<StoredTool> storage;
        for (double t : tools) storage.push_back({roundedDia(t, appDecimals()), {}});
        FlatCAMObj* sel = obj;
        int select = 0;
        if (!args.has("all")) {
            if (!args.has("box")) raiseTclError(tr_("Expected either -box <value> or -all."));
            select = 2;
            if (FlatCAMObj* b = app_->collection()->byName(args.str("box"))) sel = b;  // si no está, el propio objeto
        }

        // clear_copper_tcl
        app_->ui()->inform(tr_("NCC Tool started. Reading parameters."));
        if (tools.empty()) return std::nullopt;  // 'fail'
        app_->ui()->inform(tr_("NCC Tool. Preparing non-copper polygons."));
        Shape bbox;
        bool isList = true;
        const ShapeList geoN = solidWithExcellon(sel, isList);
        if (select == 0) {
            Shape env;
            if (!isList && geoN.size() == 1 && geoN[0].type() == ShapeType::MultiPolygon) env = geoN[0].convexHull();
            else if (isList && geoN.size() == 1 && geoN[0].type() == ShapeType::Polygon) env = Shape::unaryUnion(geoN);
            else env = Shape::unaryUnion(geoN).convexHull();
            bbox = env.buffer(margin, 16, JoinStyle::Mitre);
        } else if (sel->kind() == ObjectKind::Geometry) {
            ShapeList items = geoN;
            if (!isList && !geoN.empty() && geoN[0].type() != ShapeType::Polygon && geoN[0].type() != ShapeType::LineString &&
                geoN[0].type() != ShapeType::LinearRing && geoN[0].type() != ShapeType::Point)
                items = geoN[0].parts();  // una figura múltiple se recorre por sus partes
            ShapeList buff;
            for (const Shape& g : items) buff.push_back(g.buffer(margin, 16, JoinStyle::Mitre));
            bbox = Shape::unaryUnion(buff);
        } else if (sel->kind() == ObjectKind::Gerber) {
            // unary_union(self.ncc_obj.solid_geometry): el objeto de la herramienta (de la interfaz), que no hay.
            raiseTclError("'NoneType' object has no attribute 'solid_geometry'");
        } else {
            app_->ui()->inform("[ERROR_NOTCL] " + tr_("The reference object type is not supported."));
            return std::nullopt;
        }

        const QString units = appUnits().toLower();
        const auto started = [&](double tool) {
            app_->ui()->inform(QString("[success] %1 = %2%3 %4")
                                   .arg(tr_("NCC Tool clearing with tool diameter"), tclFloat(tool), units, tr_("started.")));
        };
        const auto failed = [&]() {
            app_->shellMessage(QString("%1 %2. %3 seconds executing initialize().")
                                   .arg(tr_("New object with name:"), outname, QString::number(0.0, 'f', 6)));
            return std::nullopt;
        };
        const auto bufferingStart = [&]() { app_->ui()->inform("[WARNING_NOTCL] " + tr_("Buffering") + " ..."); };
        const auto bufferingEnd = [&]() { app_->ui()->inform("[success] " + tr_("Buffering finished") + " ..."); };

        // gen_clear_area / gen_clear_area_rest
        if (!rest) {
            app_->ui()->inform(tr_("NCC Tool. Finished non-copper polygons. Normal copper clearing task started."));
        }
        Shape empty;
        bool restSolidIsList = false;
        {
            if (rest) {
                app_->ui()->inform("_(NCC Tool. Rest machining copper clearing task started.");
                app_->ui()->inform("NCC Tool. Calculate 'empty' area.");
            } else {
                app_->ui()->inform(tr_("NCC Tool. Calculate 'empty' area."));
            }
            Shape sol;
            if (obj->kind() == ObjectKind::Gerber) {
                if (!rest && defStr("gerber_buffering") == "no") {
                    informInitializeFailed(app_, "geometry", "'list' object has no attribute 'buffer'");
                    return std::nullopt;
                }
                sol = Shape::unaryUnion(solidOf(obj));
                restSolidIsList = rest;  // en el resto, sol_geo es la lista del Gerber
            } else if (obj->kind() == ObjectKind::Geometry) {
                sol = Shape::unaryUnion(solidOf(obj));
            } else {
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("The selected object is not suitable for copper clearing."));
                if (!rest) return failed();
                // gen_clear_area_rest devuelve None (no 'fail'): new_object crea la Geometría vacía.
                auto g = std::make_unique<GeometryObject>(outname);
                g->tools()[1] = defaultGeometryTool();
                g->setSingleGeo(true);
                app_->addObject(std::move(g), false);
                return std::nullopt;
            }
            if (hasOffset) {
                bufferingStart();
                if (restSolidIsList) {
                    informInitializeFailed(app_, "geometry", "'list' object has no attribute 'buffer'");
                    return std::nullopt;
                }
                sol = sol.buffer(offset);
                bufferingEnd();
            }
            empty = bbox.difference(sol);
            if (empty.isEmpty()) {
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("Could not get the extent of the area to be non copper cleared."));
                return failed();
            }
        }
        if (empty.type() == ShapeType::Polygon) empty = Shape::collection({empty});

        auto out = std::make_unique<GeometryObject>(outname);
        std::vector<std::size_t> setOrder;  // las herramientas en el orden en que se agregan a geo_obj.tools
        const auto clearPart = [&](const Shape& pol, double tool, ShapeList& into) -> bool {
            const auto lines = PolygonClearing::clearPolygonLines(pol, tool, defInt("gerber_circle_steps", 64), overlap,
                                                                  connect, contour);
            if (!lines) return false;
            for (const auto& l : *lines) into.push_back(PolygonClearing::toShape(l));
            return true;
        };
        if (!rest) {
            std::vector<double> sorted = tools;
            if (order == "fwd") std::sort(sorted.begin(), sorted.end());
            else if (order == "rev") std::sort(sorted.begin(), sorted.end(), std::greater<double>());
            app_->ui()->inform(tr_("NCC Tool. Finished calculation of 'empty' area."));
            double offsetA = 0;
            for (double t : sorted) offsetA += t;
            std::optional<Shape> cleared;
            for (double tool : sorted) {
                started(tool);
                ShapeList clearedGeo;
                offsetA -= (tool - 1e-12);
                Shape area = empty.buffer(-offsetA);
                if (cleared) area = area.difference(*cleared);
                for (const Shape& part : polygonsOf(area)) {
                    const Shape p = part.buffer(0);
                    if (!p.isValid()) continue;
                    for (const Shape& pol : polygonsOf(p))
                        if (pol.type() == ShapeType::Polygon) clearPart(pol, tool, clearedGeo);
                }
                if (!clearedGeo.empty()) {
                    cleared = empty.buffer(-offsetA * (1 + overlap)).buffer(-tool / 1.999999).buffer(tool / 1.999999).buffer(0);
                    storage[toolFor(storage, tool)].solid = clearedGeo;
                }
            }
            bool any = false;
            for (const StoredTool& t : storage) any = any || !t.solid.empty();
            if (!any) {
                app_->ui()->inform("[ERROR] " + tr_("There is no NCC Geometry in the file.\n"
                                                    "Usually it means that the tool diameter is too big for the painted geometry.\n"
                                                    "Change the painting parameters and try again."));
                return failed();
            }
            app_->ui()->inform("[success] " + tr_("NCC Tool clear all done."));
            for (std::size_t i = 0; i < storage.size(); ++i)
                if (!storage[i].solid.empty()) setOrder.push_back(i);
        } else {
            std::vector<double> sorted = tools;
            std::sort(sorted.begin(), sorted.end(), std::greater<double>());
            ShapeList area = polygonsOf(empty.buffer(0));
            app_->ui()->inform("NCC Tool. Finished calculation of 'empty' area.");
            ShapeList clearedByLast, restGeo;
            for (double tool : sorted) {
                started(tool);
                const double used = tool - 1e-12;
                Shape a = Shape::collection(area);
                for (const Shape& r : clearedByLast) a = a.difference(r);
                clearedByLast.clear();
                ShapeList allparts;
                for (const Shape& p : polygonsOf(a)) allparts.push_back(p.buffer(0));
                allparts.insert(allparts.end(), restGeo.begin(), restGeo.end());
                restGeo.clear();
                area = allparts;
                bool anyCleared = false;
                ShapeList clearedArea;
                for (const Shape& part : area) {
                    const Shape p = part.buffer(0);
                    if (!p.isValid()) continue;
                    for (const Shape& pol : polygonsOf(p)) {
                        if (clearPart(pol, used, clearedArea)) anyCleared = true;
                        else restGeo.push_back(pol);
                    }
                }
                if (anyCleared) {
                    for (const Shape& l : clearedArea) clearedByLast.push_back(l.buffer(used / 2));
                    const std::size_t i = toolFor(storage, tool);
                    storage[i].solid = clearedArea;
                    if (std::find(setOrder.begin(), setOrder.end(), i) == setOrder.end()) setOrder.push_back(i);
                }
            }
            if (setOrder.empty()) return failed();
            app_->ui()->inform("[success] " + tr_("NCC Tool Rest Machining clear all done."));
        }
        ShapeList solid;
        for (std::size_t i : setOrder) {
            GeometryTool t = defaultGeometryTool();
            t.tooldia = storage[i].dia;
            t.solidGeometry = storage[i].solid;
            out->tools()[static_cast<int>(i) + 1] = t;
            solid.insert(solid.end(), storage[i].solid.begin(), storage[i].solid.end());
        }
        out->solidGeometry() = solid;
        app_->addObject(std::move(out), false);
        return std::nullopt;
    }
};

/**
 * paint: ToolPaint.paint_poly_all / paint_poly / paint_poly_ref y paint_geo. El método llega como texto (el de las
 * preferencias también, por el `str()`) y paint_polygon_worker lo compara con números: no pinta nunca.
 */
class PaintCmd : public TclCommand {
public:
    explicit PaintCmd(App* a) : TclCommand(a, "TclCommandPaint") {}

protected:
    /** recurse de paint_poly_all / paint_poly_area: las figuras sueltas (los anillos, como polígonos). */
    static void flatten(const Shape& g, bool top, ShapeList& out) {
        const bool multi = g.type() == ShapeType::MultiPolygon || g.type() == ShapeType::MultiLineString ||
                           g.type() == ShapeType::MultiPoint || g.type() == ShapeType::GeometryCollection;
        if (!top && (g.isEmpty() || !g.isValid())) return;
        if (multi) {
            for (const Shape& p : g.parts()) flatten(p, false, out);
        } else if (g.type() == ShapeType::LinearRing) {
            out.push_back(Shape::polygon(g.coordinates()));
        } else {
            out.push_back(g);
        }
    }

    /** find_polygon: la primera figura (recorriendo las múltiples) que contiene el punto. */
    static std::optional<Shape> findPolygon(const ShapeList& geoset, const Point2D& pt) {
        std::function<std::optional<Shape>(const Shape&)> rec = [&](const Shape& g) -> std::optional<Shape> {
            const bool multi = g.type() == ShapeType::MultiPolygon || g.type() == ShapeType::MultiLineString ||
                               g.type() == ShapeType::MultiPoint || g.type() == ShapeType::GeometryCollection;
            if (multi) {
                for (const Shape& p : g.parts())
                    if (auto r = rec(p)) return r;
                return std::nullopt;
            }
            const Shape t = g.type() == ShapeType::LinearRing ? Shape::polygon(g.coordinates()) : g;
            if (t.contains(Shape::point(pt))) return t;
            return std::nullopt;
        };
        for (const Shape& g : geoset)
            if (auto r = rec(g)) return r;
        return std::nullopt;
    }

    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* obj = app_->collection()->byName(name);
        const QString tooldia = args.has("tooldia") ? args.str("tooldia") : PropertiesTool::pyStr(appDefaults()["tools_paint_tooldia"]);
        const QString order = args.has("order") ? args.str("order") : defStr("tools_paint_order");
        const double offset = args.has("offset") ? args.num("offset") : defNum("tools_paint_offset");
        if (args.has("connect")) argTruth(args, "connect", "tools_paint_connect");
        if (args.has("contour")) argTruth(args, "contour", "tools_paint_contour");
        const QString outname = args.has("outname") ? args.str("outname") : name + "_paint";
        std::vector<double> tools;
        for (const QString& d : tooldia.split(',')) {
            if (d.isEmpty()) continue;
            bool ok = false;
            const double v = d.trimmed().toDouble(&ok);
            if (!ok) raiseTclError(QString("invalid syntax (<string>, line 1)"));
            tools.push_back(v);
        }
        if (!obj) return QString("Object not found: %1").arg(name);
        const auto started = [&](const QString& task) {
            QString msg = tr_("Paint Tool.") + " " + task;
            if (obj->kind() == ObjectKind::Gerber && defStr("gerber_buffering") == "no")
                msg += " " + tr_("Buffering geometry...");
            app_->ui()->inform(msg);
        };
        bool isList = true;
        const ShapeList solid = solidWithExcellon(obj, isList);
        if (args.has("all")) {
            started(tr_("Paint all polygons task started."));
            ShapeList area;
            if (isList) {
                for (const Shape& g : solid) flatten(g, false, area);
            } else {
                for (const Shape& g : solid) flatten(g, true, area);
            }
            if (area.empty()) {
                app_->ui()->inform("[WARNING] " + tr_("No polygon found."));
                return std::nullopt;
            }
            paintGeo(area, tools, order, offset, outname);
            return std::nullopt;
        }
        if (args.has("single")) {
            const QString single = args.hasValue("single") ? args.str("single") : QString();
            if (single.isEmpty())
                raiseTclError(QString("%1 Got: %2").arg(tr_("Expected a tuple value like -single 3.2,0.1."), single));
            std::vector<double> xy;
            for (const QString& a : single.split(',')) {
                if (a.isEmpty()) continue;
                bool ok = false;
                const double v = a.trimmed().toDouble(&ok);
                if (!ok) raiseTclError(QString("invalid syntax (<string>, line 1)"));
                xy.push_back(v);
            }
            if (!xy.empty() && xy.size() != 2) {
                QStringList items;
                for (double v : xy) items << tclFloat(v);
                raiseTclError(QString("%1 Got: [%2]").arg(tr_("Expected a tuple value like -single 3.2,0.1."), items.join(", ")));
            }
            if (xy.empty()) raiseTclError("list index out of range");
            started(tr_("Normal painting polygon task started."));
            const auto poly = findPolygon(solid, {xy[0], xy[1]});
            if (!poly) {
                app_->ui()->inform("[WARNING] " + tr_("No polygon found."));
                return QString("Could not find a Polygon at the specified location.");
            }
            paintGeo({*poly}, tools, order, offset, outname);
            return std::nullopt;
        }
        if (args.has("box")) {
            FlatCAMObj* box = app_->collection()->byName(args.str("box"));
            if (!box) raiseTclError("'NoneType' object has no attribute 'solid_geometry'");
            bool boxList = true;
            const ShapeList geo = solidWithExcellon(box, boxList);
            // Salvo un MultiPolygon suelto, el original usa self.bound_obj (de la interfaz), que no hay.
            if (boxList || geo.size() != 1 || geo[0].type() != ShapeType::MultiPolygon) {
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("No object available."));
                return std::nullopt;
            }
            const Shape selRect = geo[0].convexHull().buffer(0.0000001, 16, JoinStyle::Mitre);
            // paint_poly_area
            started(tr_("Painting area task started."));
            ShapeList polys;
            for (const Shape& g : solid) {
                const ShapeList parts = g.type() == ShapeType::MultiPolygon ? g.parts() : ShapeList{g};
                polys.insert(polys.end(), parts.begin(), parts.end());
            }
            ShapeList area;
            flatten(Shape::collection(polys).intersection(selRect), true, area);
            if (area.empty()) {
                app_->ui()->inform("[WARNING] " + tr_("No polygon found."));
                return std::nullopt;
            }
            paintGeo(area, tools, order, offset, outname);
            return std::nullopt;
        }
        raiseTclError(tr_("None of the following args: 'box', 'single', 'all' were used.\nPaint failed.") + ":");
        return std::nullopt;
    }

    /** paint_geo / job_normal_clear: con el método como texto, ninguna figura se pinta y no se crea nada. */
    void paintGeo(const ShapeList& geometry, std::vector<double> tools, const QString& order, double offset,
                  const QString& outname) {
        if (order == "fwd") std::sort(tools.begin(), tools.end());
        else if (order == "rev") std::sort(tools.begin(), tools.end(), std::greater<double>());
        const QString units = app_->paintTool ? app_->paintTool->toolUnits().toLower() : QString();
        for (double tool : tools) {
            app_->ui()->inform(QString("[success] %1 %2%3 %4")
                                   .arg(tr_("Painting with tool diameter = "), tclFloat(tool), units, tr_("started")));
            bool any = false;
            for (const Shape& pol : geometry) {
                const Shape b = pol.buffer(-offset);
                if (b.isEmpty()) continue;
                any = true;
                // paint_polygon_worker: ningún método coincide.
                app_->ui()->inform("[ERROR_NOTCL] " + tr_("Geometry could not be painted completely"));
            }
            if (!any) app_->ui()->inform("[WARNING_NOTCL] " + tr_("Margin parameter too big. Tool is not used"));
        }
        // Todas las herramientas quedan sin geometría: initialize devuelve 'fail'.
        app_->shellMessage(QString("%1 %2. %3 seconds executing initialize().")
                               .arg(tr_("New object with name:"), outname, QString::number(0.0, 'f', 6)));
        app_->ui()->inform("[ERROR] " + tr_("Failed."));
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
    out.push_back(std::make_unique<MillHoles>(app, false));
    out.push_back(std::make_unique<MillHoles>(app, true));
    out.push_back(std::make_unique<Cutout>(app));
    out.push_back(std::make_unique<Isolate>(app));
    out.push_back(std::make_unique<GeoCutout>(app));
    out.push_back(std::make_unique<Panelize>(app));
    out.push_back(std::make_unique<AlignDrill>(app));
    out.push_back(std::make_unique<AlignDrillGrid>(app));
    out.push_back(std::make_unique<NccClear>(app));
    out.push_back(std::make_unique<PaintCmd>(app));
    out.push_back(std::make_unique<WriteGCode>(app));
}

}  // namespace flatcam::gui
