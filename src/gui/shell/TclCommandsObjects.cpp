// Comandos de la consola Tcl sobre archivos y objetos (tclCommands/ del original): open_gerber, open_excellon,
// open_gcode, open_project, save_project, open_svg, open_dxf, export_gerber / export_excellon / export_svg /
// export_dxf / export_gcode, delete, new_geometry / new_gerber / new_excellon, offset, scale, skew, mirror,
// set_origin, get_bounds, join_excellon / join_geometries, add_circle / add_rectangle / add_polygon / add_polyline,
// geo_union, exteriors, interiors, subtract_poly y subtract_rectangle.
#include <limits>

#include <QFileInfo>
#include <algorithm>

#include "core/OriginalBugs.h"
#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/shell/FCShell.h"
#include "gui/shell/TclCommand.h"
#include "gui/shell/TclCommands.h"
#include "gui/shell/TclHelpers.h"

namespace flatcam::gui {

namespace {

using namespace tclhelpers;

/**
 * Abre con el abridor y, si falla, otra vez desde la carpeta de `set_path` (`global_tcl_path`), como el original desde
 * Tcl.
 */
template <typename Open>
FlatCAMObj* openWithTclPath(const QString& filename, const QString& name, Open open) {
    FlatCAMObj* o = open(filename);
    if (!o) o = open(defStr("global_tcl_path") + "/" + name);
    return o;
}

class OpenGerber : public TclCommand {
public:
    explicit OpenGerber(App* a) : TclCommand(a, "TclCommandOpenGerber") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        if (args.has("follow"))
            raiseTclError("The 'follow' parameter is obsolete. To create 'follow' geometry use the 'follow' parameter for "
                          "the Tcl Command isolate()");
        const QString filename = args.str("filename");
        const QString outname = args.has("outname") ? args.str("outname") : baseName(filename);
        openWithTclPath(filename, outname, [this, &outname](const QString& f) { return app_->openGerber(f, outname); });
        return std::nullopt;
    }
};

class OpenExcellon : public TclCommand {
public:
    explicit OpenExcellon(App* a) : TclCommand(a, "TclCommandOpenExcellon") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString filename = args.str("filename");
        const QString outname = args.has("outname") ? args.str("outname") : baseName(filename);
        openWithTclPath(filename, outname, [this, &outname](const QString& f) { return app_->openExcellon(f, outname); });
        return std::nullopt;
    }
};

class OpenGCode : public TclCommand {
public:
    explicit OpenGCode(App* a) : TclCommand(a, "TclCommandOpenGCode") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString filename = args.str("filename");
        const QString outname = args.has("outname") ? args.str("outname") : baseName(filename);
        openWithTclPath(filename, outname, [this, &outname](const QString& f) { return app_->openGcode(f, outname); });
        return std::nullopt;
    }
};

class OpenProject : public TclCommand {
public:
    explicit OpenProject(App* a) : TclCommand(a, "TclCommandOpenProject") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString filename = args.str("filename");
        if (!QFileInfo::exists(filename)) app_->openProject(defStr("global_tcl_path") + "/" + baseName(filename));
        else app_->openProject(filename);
        return std::nullopt;
    }
};

class SaveProject : public TclCommand {
public:
    explicit SaveProject(App* a) : TclCommand(a, "TclCommandSaveProject") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        app_->saveProject(args.str("filename"));
        return std::nullopt;
    }
};

class OpenSvgOrDxf : public TclCommand {
public:
    OpenSvgOrDxf(App* a, bool dxf, const char* cls = nullptr)
        : TclCommand(a, cls ? cls : dxf ? "TclCommandOpenDXF" : "TclCommandOpenSVG"), dxf_(dxf) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString filename = args.str("filename");
        const QString outname = args.has("outname") ? args.str("outname") : baseName(filename);
        const QString type = args.has("type") ? args.str("type").toLower() : QString("geometry");
        if (type != "geometry" && type != "gerber")
            raiseTclError(QString("Option type can be 'geometry' or 'gerber' only, got '%1'.").arg(type));
        auto open = [this, &type, &outname](const QString& f) {
            return dxf_ ? app_->importDxf(f, type == "gerber", outname) : app_->importSvg(f, type == "gerber", outname);
        };
        QString used = filename;
        FlatCAMObj* o = open(filename);
        if (!o) {
            used = defStr("global_tcl_path") + "/" + outname;
            o = open(used);
            app_->shell()->appendOutput("No path provided or path is wrong. Using the default Path... \n");
            if (!o)
                return QString("Failed. The %1 command was used but could not open the %2 file")
                    .arg(dxf_ ? "OpenDXF" : "OpenSVG", dxf_ ? "DXF" : "SVG");
        }
        app_->ui()->inform("Opened: " + used);
        return std::nullopt;
    }

private:
    bool dxf_;
};

/**
 * export_gerber, export_excellon, export_svg y export_dxf: el original pasa el argumento `name` a funciones que lo
 * llaman `obj_name`, y Python falla siempre (se reproduce).
 */
class BrokenExport : public TclCommand {
public:
    BrokenExport(App* a, const char* cls, const char* fn) : TclCommand(a, cls), fn_(fn) {
        // Corregido: el argumento obligatorio es `name` (el original pide un `obj_name` que no existe).
        if (!reproduceOriginalBugs()) setRequired({"name"});
    }

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        if (reproduceOriginalBugs())
            raiseTclError(QString("MenuFileHandlers.%1() got an unexpected keyword argument 'name'").arg(fn_));
        // Corregido: exporta el objeto al archivo (sin el nombre, a la carpeta de Tcl con el nombre del objeto).
        const QString name = args.str("name");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) raiseTclError("Object not found: " + name);
        QString file = args.has("filename") ? args.str("filename") : QString();
        const QString ext = fn_ == "export_gerber" ? ".gbr" : fn_ == "export_excellon" ? ".drl"
                            : fn_ == "export_svg" ? ".svg" : ".dxf";
        if (file.isEmpty()) file = defStr("global_tcl_path") + "/" + name + ext;
        bool ok = false;
        if (fn_ == "export_gerber" || fn_ == "export_excellon") {
            const ObjectKind want = fn_ == "export_gerber" ? ObjectKind::Gerber : ObjectKind::Excellon;
            if (o->kind() != want)
                raiseTclError(QString("Expected %1, got %2 %3.").arg(want == ObjectKind::Gerber ? "Gerber" : "Excellon",
                                                                    name, pyType(o)));
            ok = app_->exportObject(o, file);
        } else if (fn_ == "export_svg") {
            ok = app_->exportSvgFile(o, file);
        } else {
            auto* geo = dynamic_cast<GeometryObject*>(o);
            if (!geo) raiseTclError(QString("Expected Geometry, got %1 %2.").arg(name, pyType(o)));
            ok = app_->exportDxfFile(geo, file);
        }
        if (!ok) raiseTclError("Could not export: " + file);
        return std::nullopt;
    }

private:
    QString fn_;
};

class ExportGcode : public TclCommand {
public:
    explicit ExportGcode(App* a) : TclCommand(a, "TclCommandExportGcode") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) raiseTclError("Object not found: " + name);
        if (o->kind() != ObjectKind::CNCJob) raiseTclError(QString("Expected CNCjob, got %1 %2.").arg(name, pyType(o)));
        // get_gcode arma el texto y el original lo descarta: el comando no hace nada visible (corregido: el G-code va a
        // la consola, que es lo que dice su ayuda).
        if (!reproduceOriginalBugs())
            return QString::fromStdString(static_cast<CNCJobObject*>(o)->exportText(args.str("preamble").toStdString(),
                                                                                   args.str("postamble").toStdString()));
        return std::nullopt;
    }
};

class Delete : public TclCommand {
public:
    explicit Delete(App* a) : TclCommand(a, "TclCommandDelete") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        bool forced = false;
        if (args.has("f")) forced = args.hasValue("f") ? pyTruthCapitalized(args.str("f")) : true;
        if (args.has("name")) {
            for (FlatCAMObj* o : app_->collection()->objects()) app_->setActive(o, false);
            FlatCAMObj* o = app_->collection()->byName(args.str("name"));
            if (!o) return QString("Command failed: 'NoneType' object has no attribute 'item'");
            app_->setActive(o, true);
        } else {
            for (FlatCAMObj* o : app_->collection()->objects()) app_->setActive(o, true);
        }
        app_->deleteSelected(forced);
        return std::nullopt;
    }
};

class NewObject : public TclCommand {
public:
    NewObject(App* a, const char* cls, ObjectKind kind) : TclCommand(a, cls), kind_(kind) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        std::unique_ptr<FlatCAMObj> o;
        if (kind_ == ObjectKind::Geometry) {
            // new_object('geometry', ...) sin herramientas: set_ui le pone una de geometry_cnctooldia.
            auto g = std::make_unique<GeometryObject>(args.has("name") ? args.str("name") : QString("new_geo"));
            g->tools()[1] = defaultGeometryTool();
            g->setSingleGeo(false);  // multigeo = False; set_ui: tools[1]['solid_geometry'] = None
            o = std::move(g);
        } else if (kind_ == ObjectKind::Gerber) {
            o = std::make_unique<GerberObject>(args.has("name") ? args.str("name") : QString("new_grb"),
                                               std::make_unique<Gerber>());
        } else {
            o = std::make_unique<ExcellonObject>(args.has("name") ? args.str("name") : QString("new_exc"),
                                                 std::make_unique<Excellon>());
        }
        app_->addObject(std::move(o), false);
        return std::nullopt;
    }

private:
    ObjectKind kind_;
};

class Offset : public TclCommand {
public:
    explicit Offset(App* a) : TclCommand(a, "TclCommandOffset") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const double x = args.has("x") ? args.num("x") : 0.0, y = args.has("y") ? args.num("y") : 0.0;
        if (x == 0.0 && y == 0.0) return std::nullopt;
        FlatCAMObj* o = app_->collection()->byName(args.str("name"));
        if (!o) raiseTclError("'NoneType' object has no attribute 'offset'");
        if (o->offset(x, y)) app_->objectChanged(o);
        app_->informTransformed(o, "offset");
        return std::nullopt;
    }
};

class Scale : public TclCommand {
public:
    explicit Scale(App* a) : TclCommand(a, "TclCommandScale") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        FlatCAMObj* o = app_->collection()->byName(args.str("name"));
        if (!o) raiseTclError("'NoneType' object has no attribute 'bounds'");
        const QRectF b = o->bounds();
        const Point2D center{b.left() + b.width() / 2, b.top() + b.height() / 2};
        Point2D point = center;
        if (args.has("origin")) {
            const QString org = args.str("origin");
            if (org == "origin") point = {0, 0};
            else if (org == "min_bounds") point = {b.left(), b.top()};
            else if (org == "center") point = center;
            else {
                const JsonValue v = pyEvalLiteral(org);
                if (!v.isArray() || v.items().size() < 2)
                    raiseTclError(tr_("Expected -origin <origin> or -origin <min_bounds> or -origin <center> or - origin "
                                      "3.0,4.2.") + (reproduceOriginalBugs() ? "\n" : ""));
                point = {v.items()[0].toDouble(), v.items()[1].toDouble()};
            }
        }
        bool changed = false;
        double sx = 1.0, sy = 1.0;
        if (args.has("factor")) {
            sx = sy = args.num("factor");
            changed = o->scale(sx, sy, point);
        } else if (!args.has("x") && !args.has("y")) {
            raiseTclError(tr_("Expected -x <value> -y <value>."));
        } else {
            // Con un solo eje, el otro factor es 0 (la figura se aplasta), como el original.
            // (corregido: el otro eje queda igual, factor 1)
            const double other = reproduceOriginalBugs() ? 0.0 : 1.0;
            sx = args.has("x") ? args.num("x") : other;
            sy = args.has("y") ? args.num("y") : other;
            changed = o->scale(sx, sy, point);
        }
        if (changed) app_->objectChanged(o);
        if (!(sx == 1.0 && sy == 1.0)) app_->informTransformed(o, "scale");  // con (1, 1) el original sale antes
        return std::nullopt;
    }
};

class Skew : public TclCommand {
public:
    explicit Skew(App* a) : TclCommand(a, "TclCommandSkew") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const double ax = args.has("x") ? args.num("x") : 0.0, ay = args.has("y") ? args.num("y") : 0.0;
        if (ax == 0.0 && ay == 0.0) return std::nullopt;
        FlatCAMObj* o = app_->collection()->byName(args.str("name"));
        if (!o) raiseTclError("'NoneType' object has no attribute 'bounds'");
        const QRectF b = o->bounds();
        if (o->skew(ax, ay, {b.left(), b.top()})) app_->objectChanged(o);
        app_->informTransformed(o, "skew");
        return std::nullopt;
    }
};

class Mirror : public TclCommand {
public:
    explicit Mirror(App* a) : TclCommand(a, "TclCommandMirror") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) return "Object not found: " + name;
        if (o->kind() != ObjectKind::Gerber && o->kind() != ObjectKind::Geometry && o->kind() != ObjectKind::Excellon)
            return QString("ERROR: Only Gerber, Excellon and Geometry objects can be mirrored.");
        const Axis axis = args.has("axis") && args.str("axis").toUpper() == "X" ? Axis::X : Axis::Y;
        if (args.has("box")) {
            FlatCAMObj* box = app_->collection()->byName(args.str("box"));
            if (!box) return "Object box not found: " + args.str("box");
            const QRectF b = box->bounds();
            if (o->mirror(axis, {0.5 * (b.left() + b.right()), 0.5 * (b.top() + b.bottom())})) app_->objectChanged(o);
            app_->informTransformed(o, "mirror");
            return std::nullopt;
        }
        if (args.has("origin")) {
            const JsonValue v = pyEvalLiteral(args.str("origin"));
            if (!v.isArray() || v.items().size() < 2) return "Invalid distance: " + args.str("origin");
            if (o->mirror(axis, {v.items()[0].toDouble(), v.items()[1].toDouble()})) app_->objectChanged(o);
            app_->informTransformed(o, "mirror");
        }
        // Sin -box ni -origin el original no refleja nada (corregido: alrededor del centro del propio objeto).
        if (!args.has("origin") && !reproduceOriginalBugs()) {
            const QRectF b = o->bounds();
            if (o->mirror(axis, {0.5 * (b.left() + b.right()), 0.5 * (b.top() + b.bottom())})) app_->objectChanged(o);
            app_->informTransformed(o, "mirror");
        }
        return std::nullopt;
    }
};

class SetOrigin : public TclCommand {
public:
    explicit SetOrigin(App* a) : TclCommand(a, "TclCommandSetOrigin") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        double lx = 0, ly = 0;
        if (args.has("auto")) {
            // bool(args['auto']): cualquier texto no vacío es verdadero.
            if (args.hasValue("auto") && !args.str("auto").isEmpty()) {
                double minx = 1e300, miny = 1e300;
                for (FlatCAMObj* o : app_->collection()->objects()) {
                    const QRectF b = o->bounds();
                    minx = std::min(minx, b.left());
                    miny = std::min(miny, b.top());
                }
                if (app_->collection()->objects().empty()) minx = miny = 0;
                lx = 0 - minx;
                ly = 0 - miny;
            }
        } else if (args.has("loc")) {
            std::vector<double> loc;
            for (const QString& c : args.str("loc").split(',')) {
                if (c.isEmpty()) continue;
                const JsonValue v = pyEvalLiteral(c);
                if (!v.isNumber()) raiseTclError(QString("could not convert string to float: %1").arg(pyReprString(c)));
                loc.push_back(v.toDouble());
            }
            // location[1] antes de comprobar (corregido: el aviso de las dos coordenadas)
            if (loc.size() < 2 && reproduceOriginalBugs()) raiseTclError("list index out of range");
            if (loc.size() != 2)
                raiseTclError(QString("%1: %2").arg(tr_("Expected a pair of (x, y) coordinates. Got"))
                                  .arg(loc.size()));
            lx = loc[0];
            ly = loc[1];
        }
        app_->setOriginAt(QPointF(-lx, -ly), false);  // on_set_zero_click(location=loc, noplot=True)
        // El original escribe dos veces la X ('{0:.4f}, {0:.4f}').
        app_->shellMessage(QString("[success] Tcl %1: %2").arg(tr_("Origin set by offsetting all loaded objects with "),
                                                                QString::asprintf("%.4f, %.4f", lx,
                                                                                  reproduceOriginalBugs() ? lx : ly)));
        return std::nullopt;
    }
};

class Bounds : public TclCommand {
public:
    explicit Bounds(App* a) : TclCommand(a, "TclCommandBounds") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString objects = args.has("objects") ? args.str("objects") : QString("None");
        QStringList names;
        if (args.has("objects"))
            for (const QString& n : objects.split(','))
                if (!n.isEmpty()) names << n;
        if (names.isEmpty())
            raiseTclError(QString("%1: %2:").arg(tr_("Expected a list of objects names separated by comma. Got"), objects));
        QStringList out;
        for (const QString& n : names) {
            FlatCAMObj* o = app_->collection()->byName(n);
            if (!o) raiseTclError("'NoneType' object has no attribute 'bounds'");
            if (o->kind() == ObjectKind::Excellon && !static_cast<ExcellonObject*>(o)->excellon().drillTools().empty()) {
                bool any = false;
                for (const auto& [id, t] : static_cast<ExcellonObject*>(o)->excellon().drillTools()) any = any || !t.solidGeometry.empty();
                if (!any) {  // bounds_rec de listas vacías (después de un panelize, sección 14): infinitos
                    const double inf = std::numeric_limits<double>::infinity();
                    out << "{" + QStringList{tclFloat(inf), tclFloat(inf), tclFloat(-inf), tclFloat(-inf)}.join(' ') + "}";
                    continue;
                }
            }
            const QRectF b = o->bounds();
            out << "{" + QStringList{tclFloat(b.left()), tclFloat(b.top()), tclFloat(b.right()), tclFloat(b.bottom())}.join(' ') + "}";
        }
        app_->ui()->inform(QString("[success] %1 ...").arg(tr_("TclCommand Bounds done.")));
        return out.join(' ');
    }
};

class Join : public TclCommand {
public:
    Join(App* a, bool excellon) : TclCommand(a, excellon ? "TclCommandJoinExcellon" : "TclCommandJoinGeometry"),
                                  excellon_(excellon) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        std::vector<FlatCAMObj*> objs;
        for (const QString& n : args.unnamed) {
            FlatCAMObj* o = app_->collection()->byName(n);
            if (!o) return "Object not found: " + n;
            objs.push_back(o);
        }
        if (objs.size() < 2)
            return excellon_ ? QString("No Excellon objects to be joined or less than two Excellon objects specified for "
                                       "merging.")
                             : QString("No Geometry objects to be joined or less than two Geometry objects specified for "
                                       "merging.");
        if (excellon_) app_->joinExcellon(&objs, args.str("outname"));
        else app_->joinGeometry(&objs, args.str("outname"));
        return std::nullopt;
    }

private:
    bool excellon_;
};

/** add_circle, add_rectangle, add_polygon, add_polyline: en la geometría sólida y en la herramienta 1. */
class AddShape : public TclCommand {
public:
    enum class Kind { Circle, Rectangle, Polygon, Polyline };
    AddShape(App* a, const char* cls, Kind k) : TclCommand(a, cls), kind_(k) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        if (kind_ == Kind::Rectangle && !args.unnamed.isEmpty())
            raiseTclError("Too many arguments. Correct format: [\"add_rectangle geo_name xmin ymin xmax ymax\"]");
        FlatCAMObj* o = app_->collection()->byName(name);
        const bool strict = kind_ == Kind::Polygon || kind_ == Kind::Polyline;
        if (!o) {
            if (strict) raiseTclError("Object not found: " + name);
            return "Object not found: " + name;
        }
        if (o->kind() != ObjectKind::Geometry) raiseTclError(QString("Expected Geometry, got %1 %2.").arg(name, pyType(o)));
        auto* geo = static_cast<GeometryObject*>(o);
        Shape shape;
        if (kind_ == Kind::Circle) {
            shape = Shape::point({args.num("center_x"), args.num("center_y")})
                        .buffer(args.num("radius"), defInt("geometry_circle_steps", 64));
        } else if (kind_ == Kind::Rectangle) {
            const double x0 = args.num("x0"), y0 = args.num("y0"), x1 = args.num("x1"), y1 = args.num("y1");
            shape = Shape::polygon({{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}});
        } else {
            if (args.unnamed.size() % 2 != 0) raiseTclError("Incomplete coordinates.");
            std::vector<Point2D> pts;
            for (int i = 0; i + 1 < args.unnamed.size(); i += 2) {
                QString err;
                const auto x = pyFloatFromString(args.unnamed[i], &err);
                if (!x) raiseTclError(err);
                const auto y = pyFloatFromString(args.unnamed[i + 1], &err);
                if (!y) raiseTclError(err);
                pts.push_back({*x, *y});
            }
            shape = kind_ == Kind::Polygon ? Shape::polygon(pts) : Shape::lineString(pts);
        }
        if (!shape.isValid()) return std::nullopt;  // return "fail" (el comando no lo muestra)
        const auto err = geo->addShapeLikeTheOriginal(shape, kind_ == Kind::Circle);
        app_->objectChanged(geo);
        if (err) raiseTclError(*err);
        return std::nullopt;
    }

private:
    Kind kind_;
};

class GeoUnion : public TclCommand {
public:
    explicit GeoUnion(App* a) : TclCommand(a, "TclCommandGeoUnion") {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) return "Object not found: " + name;
        // Geometry.union: la geometría sólida pasa a ser [unary_union(...)].
        if (auto* g = dynamic_cast<GeometryObject*>(o)) {
            g->setSolidGeometry({Shape::unaryUnion(g->solidGeometry())});
        } else if (auto* gr = dynamic_cast<GerberObject*>(o)) {
            gr->gerber().solidGeometry() = {Shape::unaryUnion(gr->gerber().solidGeometry())};
        } else if (auto* ex = dynamic_cast<ExcellonObject*>(o)) {
            // Excellon también deriva de Geometry: une su solid_geometry (no cambia los agujeros ni el dibujo).
            ex->excellon().solidGeometry() = {Shape::unaryUnion(ex->excellon().solidGeometry())};
        } else if (o->kind() == ObjectKind::CNCJob) {
            return std::nullopt;  // CNCjob deriva de Geometry: une una solid_geometry que no se usa
        } else {
            raiseTclError(QString("'%1' object has no attribute 'union'").arg(pyType(o).section('.', -1).chopped(2)));
        }
        app_->objectChanged(o);
        return std::nullopt;
    }
};

class Rings : public TclCommand {
public:
    Rings(App* a, bool interiors) : TclCommand(a, interiors ? "TclCommandInteriors" : "TclCommandExteriors"),
                                    interiors_(interiors) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        const QString name = args.str("name");
        const QString outname = args.has("outname") ? args.str("outname") : name + (interiors_ ? "_interiors" : "_exteriors");
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) raiseTclError("Object not found: " + name);
        if (o->kind() == ObjectKind::CNCJob || o->kind() == ObjectKind::Script || o->kind() == ObjectKind::Document)
            raiseTclError(QString("Expected Geometry, got %1 %2.").arg(name, pyType(o)));
        // get_exteriors / get_interiors: los anillos de cada polígono (de las figuras múltiples también).
        ShapeList rings;
        for (const Shape& s : o->solidShapes())
            for (const Shape& p : s.parts()) {
                if (p.type() != ShapeType::Polygon) continue;
                if (interiors_)
                    for (const Shape& r : p.interiors()) rings.push_back(r);
                else
                    rings.push_back(p.exterior());
            }
        auto geo = std::make_unique<GeometryObject>(outname);
        geo->solidGeometry() = rings;
        geo->tools()[1] = defaultGeometryTool();
        geo->setSingleGeo(true);  // set_ui: la herramienta 1 con la misma lista
        app_->addObject(std::move(geo), interiors_);  // exteriors con plot=False
        return std::nullopt;
    }

private:
    bool interiors_;
};

class SubtractShape : public TclCommand {
public:
    SubtractShape(App* a, bool rect) : TclCommand(a, rect ? "TclCommandSubtractRectangle" : "TclCommandSubtractPoly"),
                                       rect_(rect) {}

protected:
    std::optional<QString> execute(const TclArgs& args) override {
        if (rect_ && !args.has("name"))
            raiseTclError(QString("%1:").arg(tr_("No Geometry name in args. Provide a name and try again.")));
        const QString name = args.str("name");
        std::vector<Point2D> pts;
        QString err;
        auto num = [&err](const QString& t) {
            const auto v = pyFloatFromString(t, &err);
            if (!v) throw TclError(err);
            return *v;
        };
        if (rect_) {
            if (args.unnamed.size() != 4) raiseTclError("Incomplete coordinates. There are 4 required: x0 y0 x1 y1.");
            const double x0 = num(args.unnamed[0]), y0 = num(args.unnamed[1]), x1 = num(args.unnamed[2]),
                         y1 = num(args.unnamed[3]);
            pts = {{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}};
        } else {
            if (args.unnamed.size() % 2 != 0) return QString("Incomplete coordinate.");
            for (int i = 0; i + 1 < args.unnamed.size(); i += 2) pts.push_back({num(args.unnamed[i]), num(args.unnamed[i + 1])});
        }
        FlatCAMObj* o = app_->collection()->byName(name);
        if (!o) return "Object not found: " + name;
        auto* geo = dynamic_cast<GeometryObject*>(o);
        if (!geo) raiseTclError(QString("'%1' object has no attribute 'subtract_polygon'").arg(pyType(o).section('.', -1).chopped(2)));
        // subtract_polygon: solo los trayectos (contornos y líneas) menos el polígono; queda la unión de las restas.
        const Shape tool = Shape::unaryUnion({Shape::polygon(pts)});
        std::vector<Shape> diffs;
        for (const Shape& s : geo->solidGeometry())
            for (const Shape& p : s.parts()) {
                if (p.type() == ShapeType::Polygon) {
                    diffs.push_back(p.exterior().difference(tool));
                    for (const Shape& r : p.interiors()) diffs.push_back(r.difference(tool));
                } else if (p.type() == ShapeType::LineString || p.type() == ShapeType::LinearRing) {
                    diffs.push_back(p.difference(tool));
                }
            }
        // El original deja una figura sola (no una lista) y después add_circle falla; corregido: la lista de sus partes.
        if (reproduceOriginalBugs()) geo->setSolidGeometry({Shape::unaryUnion(diffs)}, false);
        else geo->setSolidGeometry(Shape::unaryUnion(diffs).parts(), true);
        app_->objectChanged(geo);
        return std::nullopt;
    }

private:
    bool rect_;
};

}  // namespace

void addObjectTclCommands(App* app, std::vector<std::unique_ptr<TclCommand>>& out) {
    out.push_back(std::make_unique<OpenGerber>(app));
    out.push_back(std::make_unique<OpenExcellon>(app));
    out.push_back(std::make_unique<OpenGCode>(app));
    out.push_back(std::make_unique<OpenProject>(app));
    out.push_back(std::make_unique<SaveProject>(app));
    out.push_back(std::make_unique<OpenSvgOrDxf>(app, false));
    out.push_back(std::make_unique<OpenSvgOrDxf>(app, true));
    // Corregido: import_svg está escrito en el original pero no se registra (es lo mismo que open_svg).
    if (!reproduceOriginalBugs()) out.push_back(std::make_unique<OpenSvgOrDxf>(app, false, "TclCommandImportSvg"));
    out.push_back(std::make_unique<BrokenExport>(app, "TclCommandExportGerber", "export_gerber"));
    out.push_back(std::make_unique<BrokenExport>(app, "TclCommandExportExcellon", "export_excellon"));
    out.push_back(std::make_unique<BrokenExport>(app, "TclCommandExportSVG", "export_svg"));
    out.push_back(std::make_unique<BrokenExport>(app, "TclCommandExportDXF", "export_dxf"));
    out.push_back(std::make_unique<ExportGcode>(app));
    out.push_back(std::make_unique<Delete>(app));
    out.push_back(std::make_unique<NewObject>(app, "TclCommandNewGeometry", ObjectKind::Geometry));
    out.push_back(std::make_unique<NewObject>(app, "TclCommandNewGerber", ObjectKind::Gerber));
    out.push_back(std::make_unique<NewObject>(app, "TclCommandNewExcellon", ObjectKind::Excellon));
    out.push_back(std::make_unique<Offset>(app));
    out.push_back(std::make_unique<Scale>(app));
    out.push_back(std::make_unique<Skew>(app));
    out.push_back(std::make_unique<Mirror>(app));
    out.push_back(std::make_unique<SetOrigin>(app));
    out.push_back(std::make_unique<Bounds>(app));
    out.push_back(std::make_unique<Join>(app, true));
    out.push_back(std::make_unique<Join>(app, false));
    out.push_back(std::make_unique<AddShape>(app, "TclCommandAddCircle", AddShape::Kind::Circle));
    out.push_back(std::make_unique<AddShape>(app, "TclCommandAddRectangle", AddShape::Kind::Rectangle));
    out.push_back(std::make_unique<AddShape>(app, "TclCommandAddPolygon", AddShape::Kind::Polygon));
    out.push_back(std::make_unique<AddShape>(app, "TclCommandAddPolyline", AddShape::Kind::Polyline));
    out.push_back(std::make_unique<GeoUnion>(app));
    out.push_back(std::make_unique<Rings>(app, false));
    out.push_back(std::make_unique<Rings>(app, true));
    out.push_back(std::make_unique<SubtractShape>(app, false));
    out.push_back(std::make_unique<SubtractShape>(app, true));
}

}  // namespace flatcam::gui
