// Los comandos Tcl de objetos (new_geometry, add_*, geo_union, exteriors, interiors, subtract_*, bounds, offset, scale,
// skew, mirror, set_origin, delete, get_names), contra el original con la consola de tkinter y la GeometryObject
// original (tests/tools/oracle_tcl_objects.py -> tests/data/tcl_shell/objects.json).
#include <gtest/gtest.h>

#include <cmath>

#include <QFile>
#include <QTemporaryDir>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include "core/OriginalBugs.h"
#include "core/Shape.h"
#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "gui/shell/FCShell.h"

using namespace flatcam;
using namespace flatcam::gui;

namespace {

QJsonObject oracle() {
    QFile f(QString(FLATCAM_TEST_DATA_DIR) + "/tcl_shell/objects.json");
    EXPECT_TRUE(f.open(QIODevice::ReadOnly));
    return QJsonDocument::fromJson(f.readAll()).object();
}

/** `solid_geometry` del original: nada (None), una lista o una figura sola. */
std::vector<Shape> shapesOf(const QJsonValue& v) {
    std::vector<Shape> out;
    if (v.isArray())
        for (const QJsonValue& w : v.toArray()) out.push_back(Shape::fromWkt(w.toString().toStdString()));
    else if (v.isObject())
        out.push_back(Shape::fromWkt(v.toObject()["single"].toString().toStdString()));
    return out;
}

/** Cantidad de vértices (de todas las partes y anillos). */
size_t vertexCount(const Shape& s) {
    size_t n = 0;
    for (const Shape& p : s.parts()) {
        if (p.type() == ShapeType::Polygon) {
            n += p.exterior().coordinates().size();
            for (const Shape& r : p.interiors()) n += r.coordinates().size();
        } else if (!p.isEmpty()) {
            n += p.coordinates().size();
        }
    }
    return n;
}

/**
 * Los trazados del NCC: paint_connect elige entre empates exactos de `rtree.nearest` (el port toma el primero
 * insertado), así que cambian algunas uniones: los puntos y el largo a menos del 2 % y del 1 % (con GEOS 3.11, la del
 * oráculo, los puntos dan exactos salvo en el resto, a 0,15 %; con 3.15 el engrosado del resto cambia el 1,2 %) y la
 * cantidad de trazados a menos del 35 %.
 */
void expectSameTotals(const std::vector<Shape>& port, const std::vector<Shape>& orig) {
    double lp = 0, lo = 0;
    size_t np = 0, no = 0;
    for (const Shape& p : port) {
        lp += p.length();
        np += vertexCount(p);
    }
    for (const Shape& p : orig) {
        lo += p.length();
        no += vertexCount(p);
    }
    EXPECT_NEAR(static_cast<double>(np), static_cast<double>(no), 0.02 * static_cast<double>(no));
    EXPECT_NEAR(lp, lo, 0.01 * lo);
    EXPECT_NEAR(static_cast<double>(port.size()), static_cast<double>(orig.size()), 0.35 * static_cast<double>(orig.size()) + 1);
}

void expectSameShapes(const std::vector<Shape>& port, const std::vector<Shape>& orig) {
    ASSERT_EQ(port.size(), orig.size());
    for (size_t i = 0; i < port.size(); ++i) {
        SCOPED_TRACE("figura " + std::to_string(i));
        EXPECT_EQ(port[i].type(), orig[i].type());
        // Con GEOS 3.12 / 3.15 la unión empieza algunos anillos en otro vértice que la 3.11 del oráculo (misma
        // geometría y cantidad de vértices), al engrosar muchas figuras deja hasta 1 % menos de vértices sobre la
        // misma figura (el aislamiento del logo: 8.837 contra 8.912, con 4e-9 mm² de diferencia) y, en las líneas,
        // junta tramos que se tocan en un extremo (mismos puntos en otras partes): ahí se pide la misma geometría.
        const bool lines = orig[i].type() == ShapeType::MultiLineString || orig[i].type() == ShapeType::GeometryCollection;
        const auto near = [](const Shape& a, const Shape& b) { return a.within(b.buffer(1e-6)); };
        const bool same = port[i].equalsExact(orig[i], 1e-9) ||
                          (near(port[i], orig[i]) && near(orig[i], port[i]) &&
                           std::abs(port[i].length() - orig[i].length()) < 1e-7 &&
                           (lines || std::abs(static_cast<double>(vertexCount(port[i])) -
                                             static_cast<double>(vertexCount(orig[i]))) <=
                                         0.01 * static_cast<double>(vertexCount(orig[i]))));
        EXPECT_TRUE(same) << "area " << port[i].area() << " / " << orig[i].area() << " largo " << port[i].length() << " / "
                          << orig[i].length() << " vértices " << vertexCount(port[i]) << " / " << vertexCount(orig[i])
                          << " dif " << port[i].difference(orig[i]).area() + orig[i].difference(port[i]).area();
    }
}

}  // namespace

/** El texto de la consola sin el tiempo de `initialize` ("... 0.000012 seconds executing initialize()."). */
QString withoutTimes(QString t) {
    static const QRegularExpression re("\\d+\\.\\d{6} seconds executing initialize\\(\\)");
    return t.replace(re, "T seconds executing initialize()");
}

TEST(GuiTclObjects, SessionLikeTheOriginal) {
    App app;
    // `delete` pregunta antes de borrar (on_delete); el oráculo no tiene ese diálogo.
    setAppDefault("global_delete_confirmation", JsonValue::boolean(false));
    FCShell* sh = app.shell();
    QTemporaryDir tmp;
    QHash<QString, QJsonObject> expected;
    for (const QJsonValue& v : oracle()["session"].toArray()) {
        const QJsonObject s = v.toObject();
        SCOPED_TRACE(s["cmd"].toString().toStdString());
        const QString before = sh->browser()->toPlainText();
        const QString data = QString(FLATCAM_TEST_DATA_DIR);
        sh->commandLine()->setPlainText(s["cmd"].toString().replace("@DIR@", tmp.path()).replace("@DATA@", data));
        sh->execCurrentCommand();
        const QString after = sh->browser()->toPlainText();
        const QString added =
            withoutTimes(after.startsWith(before) ? after.mid(before.size()) : after)
                .replace(tmp.path(), "@DIR@")
                .replace(data, "@DATA@");
        if (s["simulated"].toBool()) {
            // App.on_delete está simulado en el oráculo (no avisa): solo la orden.
            EXPECT_EQ(added.section('\n', 0, 0).toStdString(), s["added"].toString().section('\n', 0, 0).toStdString());
        } else if (s["traceback"].toBool()) {
            // Error de Python que no es de Tcl: el port no muestra el traceback (y sin él, no se pasa a mayúsculas
            // lo que el original pasa por un "]" del traceback): se compara la primera línea del error.
            const QString exp = withoutTimes(s["added"].toString());
            EXPECT_EQ(added.section('\n', 0, 1).toUpper().toStdString(), exp.section('\n', 0, 1).toUpper().toStdString());
        } else {
            EXPECT_EQ(added.toStdString(), withoutTimes(s["added"].toString()).toStdString());
        }
        // Los archivos que escribió write_gcode, sin la fecha ni la versión de la cabecera.
        const QJsonObject files = s["files"].toObject();
        for (auto it = files.begin(); it != files.end(); ++it) {
            SCOPED_TRACE(it.key().toStdString());
            QFile f(tmp.filePath(it.key()));
            ASSERT_TRUE(f.open(QIODevice::ReadOnly | QIODevice::Text));
            QString text;
            for (const QString& l : QString::fromUtf8(f.readAll()).split('\n', Qt::KeepEmptyParts))
                if (!l.contains("Created on") && !l.contains("GENERATED BY FLATCAM")) text += l + '\n';
            text.chop(1);
            EXPECT_EQ(text.toStdString(), it.value().toString().toStdString());
        }
        // Cada paso trae los nombres de todos los objetos y el estado de los que cambiaron; el de los demás es el
        // último que se vio, y se vuelven a comparar todos.
        for (const QJsonValue& ov : s["objects"].toArray()) expected[ov.toObject()["name"].toString()] = ov.toObject();
        const QJsonArray names = s["names"].toArray();
        ASSERT_EQ(static_cast<int>(app.collection()->objects().size()), names.size());
        for (const QJsonValue& nv : names) {
            const QJsonObject oo = expected.value(nv.toString());
            SCOPED_TRACE(oo["name"].toString().toStdString());
            if (oo.contains("kind")) {  // un CNC Job, Excellon o Gerber: solo que exista y sea de ese tipo
                const FlatCAMObj* o = app.collection()->byName(oo["name"].toString());
                ASSERT_NE(o, nullptr);
                const QString k = oo["kind"].toString();
                EXPECT_EQ(o->kind(), k == "excellon" ? ObjectKind::Excellon
                                     : k == "gerber" ? ObjectKind::Gerber
                                                     : ObjectKind::CNCJob);
                if (oo.contains("tools")) {  // un Excellon creado por un comando: diámetro, agujeros y área
                    const auto& tools = const_cast<ExcellonObject*>(static_cast<const ExcellonObject*>(o))->excellon().drillTools();
                    const QJsonArray ot = oo["tools"].toArray();
                    ASSERT_EQ(static_cast<int>(tools.size()), ot.size());
                    auto it = tools.begin();
                    for (const QJsonValue& tv : ot) {
                        const QJsonObject t = tv.toObject();
                        EXPECT_DOUBLE_EQ(it->second.tooldia, t["tooldia"].toDouble());
                        const QJsonArray dr = t["drills"].toArray();
                        ASSERT_EQ(static_cast<int>(it->second.drills.size()), dr.size());
                        for (int i = 0; i < dr.size(); ++i) {
                            EXPECT_NEAR(it->second.drills[static_cast<size_t>(i)].x, dr[i].toArray()[0].toDouble(), 1e-12);
                            EXPECT_NEAR(it->second.drills[static_cast<size_t>(i)].y, dr[i].toArray()[1].toDouble(), 1e-12);
                        }
                        double area = 0;
                        for (const Shape& g : it->second.solidGeometry) area += g.area();
                        EXPECT_NEAR(area, t["area"].toDouble(), 1e-9);
                        ++it;
                    }
                }
                continue;
            }
            auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName(oo["name"].toString()));
            ASSERT_NE(geo, nullptr);
            EXPECT_EQ(geo->isSingleGeo(), !oo["multigeo"].toBool());
            const bool relaxed = oo["relaxed"].toBool();
            const auto compare = [relaxed](const std::vector<Shape>& a, const std::vector<Shape>& b) {
                if (relaxed) expectSameTotals(a, b);
                else expectSameShapes(a, b);
            };
            compare(geo->solidGeometry(), shapesOf(oo["solid"]));
            ASSERT_TRUE(geo->tools().count(1));
            if (oo["multigeo"].toBool()) {
                compare(geo->tools().at(1).solidGeometry, shapesOf(oo["tool1"]));
            } else {
                // Un objeto de una sola geometría se dibuja y se corta con su solid_geometry: la herramienta 1 la copia
                // (desvío en el panelizado de uno de una sola geometría: en el original la herramienta 1 queda con la
                // geometría del objeto de origen, que no se usa).
                expectSameShapes(geo->tools().at(1).solidGeometry, geo->solidGeometry());
            }
        }
        if (HasFailure()) return;  // el primer paso que difiere (los siguientes arrastran la diferencia)
    }
}

TEST(GuiTclObjects, FilesProjectsAndJoins) {
    App app;
    setAppDefault("global_delete_confirmation", JsonValue::boolean(false));
    FCShell* sh = app.shell();
    const QString dir = QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    bool ok = true;
    sh->eval("open_gerber {" + dir + "Edge_Cuts.gbr} -outname edge", &ok);
    EXPECT_TRUE(ok);
    sh->eval("open_excellon {" + dir + "PTH.drl}", &ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(sh->eval("get_names", &ok).toStdString(), "edge\nluz de noche Pascual-PTH.drl");
    EXPECT_FALSE(sh->eval("bounds edge", &ok).isEmpty());
    // follow: una Geometría con la geometría de seguimiento del Gerber (las líneas centrales del contorno).
    sh->eval("follow edge", &ok);
    EXPECT_TRUE(ok);
    auto* fol = dynamic_cast<GeometryObject*>(app.collection()->byName("edge_follow"));
    ASSERT_NE(fol, nullptr);
    auto* edge = dynamic_cast<GerberObject*>(app.collection()->byName("edge"));
    ASSERT_NE(edge, nullptr);
    EXPECT_EQ(fol->solidGeometry().size(), edge->gerber().followGeometry().size());
    EXPECT_FALSE(fol->solidGeometry().empty());
    EXPECT_EQ(sh->eval("follow edge_follow", &ok).toStdString(),
              "Expected GerberObject, got edge_follow <class 'appObjects.FlatCAMGeometry.GeometryObject'>.");
    sh->eval("delete edge_follow", &ok);
    // non_copper_regions: la caja menos el cobre; además apaga el dibujo del objeto de origen (plot_cb.toggle()).
    sh->eval("non_copper_regions edge -rounded 0", &ok);
    EXPECT_TRUE(ok);
    EXPECT_FALSE(app.collection()->byName("edge")->plotEnabled());
    sh->eval("delete edge_noncopper", &ok);
    app.collection()->byName("edge")->setPlotEnabled(true);
    // Errores del original: export_gerber y export_dxf exigen `obj_name`, que no es un argumento; export_excellon y
    // export_svg le pasan `name` a MenuFileHandlers, que no lo acepta.
    EXPECT_EQ(sh->eval("export_gerber edge", &ok).toStdString(), "Missing required argument 'obj_name'.");
    EXPECT_FALSE(ok);
    const QString err = sh->eval("export_excellon {luz de noche Pascual-PTH.drl}", &ok);
    EXPECT_FALSE(ok);
    EXPECT_EQ(err.toStdString(), "MenuFileHandlers.export_excellon() got an unexpected keyword argument 'name'");
    // join_excellons con uno solo avisa; con dos crea el objeto.
    EXPECT_TRUE(sh->eval("join_excellons j {luz de noche Pascual-PTH.drl}", &ok).startsWith("No Excellon objects"));
    // El NPTH de "luz de noche" no tiene agujeros: como el original, no se abre.
    sh->eval("open_excellon {" + dir + "NPTH.drl} -outname npth", &ok);
    EXPECT_EQ(app.collection()->byName("npth"), nullptr);
    sh->eval("open_excellon {" + QString(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Drill.drl} -outname npth", &ok);
    sh->eval("join_excellons j {luz de noche Pascual-PTH.drl} npth", &ok);
    EXPECT_TRUE(ok);
    ASSERT_NE(app.collection()->byName("j"), nullptr);
    EXPECT_EQ(app.collection()->byName("j")->kind(), ObjectKind::Excellon);
    // Guardar el proyecto, vaciarlo y volver a abrirlo.
    QTemporaryDir tmp;
    const QString prj = tmp.filePath("p.FlatPrj");
    sh->eval("save_project {" + prj + "}", &ok);
    EXPECT_TRUE(ok);
    EXPECT_TRUE(QFile::exists(prj));
    sh->eval("new", &ok);
    EXPECT_TRUE(app.collection()->objects().empty());
    sh->eval("open_project {" + prj + "}", &ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(sh->eval("get_names", &ok).toStdString(), "edge\nluz de noche Pascual-PTH.drl\nnpth\nj");
}

TEST(GuiFixed, TclDrillcncjobAndBoundingBox) {
    flatcam::OriginalBugsScope fixed(false);
    App app;
    FCShell* sh = app.shell();
    bool ok = false;
    sh->eval("open_excellon {" + QString(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Drill.drl} -outname drl", &ok);
    ASSERT_TRUE(ok);
    // Z negativa (la de fábrica), orden T sin repetir, -endz y -toolchangexy; el nombre sin distinguir mayúsculas.
    sh->eval("drillcncjob DRL -drillz -1.5 -toolchangez 15 -toolchangexy 1,2 -endz 7 -opt_type T -outname c", &ok);
    EXPECT_TRUE(ok) << sh->eval("set errorInfo").toStdString();
    auto* job = dynamic_cast<CNCJobObject*>(app.collection()->byName("c"));
    ASSERT_NE(job, nullptr);
    const std::string g = job->exportText();
    EXPECT_NE(g.find("G21"), std::string::npos);           // código de inicio
    EXPECT_NE(g.find("Z-1.5"), std::string::npos);
    EXPECT_NE(g.find("Z7.0"), std::string::npos) << g.substr(g.size() > 400 ? g.size() - 400 : 0);
    EXPECT_EQ(g.find("Total drills for tool T1 = 1"), std::string::npos);
    EXPECT_NE(g.find("Excellon"), std::string::npos);
    // 192 agujeros, ninguno repetido: una bajada por agujero.
    std::size_t downs = 0;
    for (std::size_t p = g.find("G01 Z-1.5"); p != std::string::npos; p = g.find("G01 Z-1.5", p + 1)) ++downs;
    EXPECT_EQ(downs, 192u);
    // -drilled_dias: solo esa herramienta.
    sh->eval("drillcncjob drl -drillz -1 -drilled_dias 0.8 -diatol 5 -outname c2", &ok);
    EXPECT_TRUE(ok) << sh->eval("set errorInfo").toStdString();
    EXPECT_NE(app.collection()->byName("c2"), nullptr);
    // bounding_box con la preferencia de las esquinas (un valor lógico) y non_copper_regions no esconde el origen.
    sh->eval("open_gerber {" + QString(FLATCAM_TEST_DATA_DIR) + "/PCB principal-Edge_Cuts.gm1} -outname edge", &ok);
    sh->eval("bounding_box edge", &ok);
    EXPECT_TRUE(ok) << sh->eval("set errorInfo").toStdString();
    sh->eval("non_copper_regions edge", &ok);
    EXPECT_TRUE(ok);
    EXPECT_TRUE(app.collection()->byName("edge")->plotEnabled());
}

TEST(GuiFixed, TclMillPanelizeAlignNccPaintCutoutIsolate) {
    flatcam::OriginalBugsScope fixed(false);
    App app;
    FCShell* sh = app.shell();
    bool ok = false;
    const QString dir = QString(FLATCAM_TEST_DATA_DIR) + "/luz_de_noche/luz de noche Pascual-";
    const auto run = [&](const QString& c) {
        const QString r = sh->eval(c, &ok);
        EXPECT_TRUE(ok) << c.toStdString() << ": " << r.toStdString();
        return r;
    };
    run("open_excellon {" + dir + "PTH.drl} -outname drl");
    run("open_gerber {" + dir + "Logo.gbr} -outname logo");
    run("open_gerber {" + dir + "Edge_Cuts.gbr} -outname edge");
    // milldrills fresa los agujeros.
    run("milldrills drl -tooldia 0.5 -outname m");
    ASSERT_NE(app.collection()->byName("m"), nullptr);
    EXPECT_FALSE(dynamic_cast<GeometryObject*>(app.collection()->byName("m"))->solidGeometry().empty());
    // panelize de un Excellon: 2 x 3 copias, sin el error de formato.
    run("panelize drl -columns 2 -rows 3 -outname p");
    auto* p = dynamic_cast<ExcellonObject*>(app.collection()->byName("p"));
    ASSERT_NE(p, nullptr);
    std::size_t src = 0, dst = 0;
    for (const auto& [id, t] : dynamic_cast<ExcellonObject*>(app.collection()->byName("drl"))->excellon().drillTools())
        src += t.drills.size();
    for (const auto& [id, t] : p->excellon().drillTools()) dst += t.drills.size();
    EXPECT_EQ(dst, src * 6);
    // aligndrill con -grid y sin -box: la grilla sobre el propio objeto.
    run("aligndrill edge -dia 1 -axis X -grid 10 -gridoffset 0 -outname a");
    auto* a = dynamic_cast<ExcellonObject*>(app.collection()->byName("a"));
    ASSERT_NE(a, nullptr);
    EXPECT_FALSE(a->excellon().drillTools().empty());
    // ncc_clear: con resto el sufijo es _ncc_rm; sin resto _ncc, y con el método estándar.
    run("ncc_clear logo -tooldia 0.2 -all 1 -method standard");
    EXPECT_NE(app.collection()->byName("logo_ncc"), nullptr);
    // paint -all pinta.
    run("paint logo -tooldia 0.1 -all 1 -method standard -outname lp");
    auto* lp = dynamic_cast<GeometryObject*>(app.collection()->byName("lp"));
    ASSERT_NE(lp, nullptr);
    EXPECT_FALSE(lp->solidGeometry().empty());
    // cutout con un objeto que no está es un error.
    sh->eval("cutout nada", &ok);
    EXPECT_FALSE(ok);
    // isolate con pasadas separadas: un nombre por pasada.
    run("isolate edge -dia 0.2 -passes 2 -combine 0 -outname iso");
    EXPECT_NE(app.collection()->byName("iso_1"), nullptr);
    EXPECT_NE(app.collection()->byName("iso_2"), nullptr);
}
