// Los comandos Tcl de objetos (new_geometry, add_*, geo_union, exteriors, interiors, subtract_*, bounds, offset, scale,
// skew, mirror, set_origin, delete, get_names), contra el original con la consola de tkinter y la GeometryObject
// original (tests/tools/oracle_tcl_objects.py -> tests/data/tcl_shell/objects.json).
#include <gtest/gtest.h>

#include <cmath>

#include <QFile>
#include <QTemporaryDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/Shape.h"
#include "gui/App.h"
#include "gui/Preferences.h"
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

void expectSameShapes(const std::vector<Shape>& port, const std::vector<Shape>& orig) {
    ASSERT_EQ(port.size(), orig.size());
    for (size_t i = 0; i < port.size(); ++i) {
        SCOPED_TRACE("figura " + std::to_string(i));
        EXPECT_EQ(port[i].type(), orig[i].type());
        // Con GEOS 3.12 / 3.15 la unión empieza algunos anillos en otro vértice que la 3.11 del oráculo (misma
        // geometría y cantidad de vértices) y, en las líneas, junta tramos que se tocan en un extremo (mismos puntos
        // en otras partes): ahí se pide la misma geometría.
        const bool lines = orig[i].type() == ShapeType::MultiLineString || orig[i].type() == ShapeType::GeometryCollection;
        const auto near = [](const Shape& a, const Shape& b) { return a.within(b.buffer(1e-7)); };
        const bool same = port[i].equalsExact(orig[i], 1e-9) ||
                          (near(port[i], orig[i]) && near(orig[i], port[i]) &&
                           std::abs(port[i].length() - orig[i].length()) < 1e-7 &&
                           (lines || port[i].coordinates().size() == orig[i].coordinates().size()));
        EXPECT_TRUE(same) << port[i].toWkt(17) << "\n" << orig[i].toWkt(17);
    }
}

}  // namespace

TEST(GuiTclObjects, SessionLikeTheOriginal) {
    App app;
    // `delete` pregunta antes de borrar (on_delete); el oráculo no tiene ese diálogo.
    setAppDefault("global_delete_confirmation", JsonValue::boolean(false));
    FCShell* sh = app.shell();
    for (const QJsonValue& v : oracle()["session"].toArray()) {
        const QJsonObject s = v.toObject();
        SCOPED_TRACE(s["cmd"].toString().toStdString());
        const QString before = sh->browser()->toPlainText();
        sh->commandLine()->setPlainText(s["cmd"].toString());
        sh->execCurrentCommand();
        const QString after = sh->browser()->toPlainText();
        const QString added = after.startsWith(before) ? after.mid(before.size()) : after;
        if (s["simulated"].toBool()) {
            // App.on_delete está simulado en el oráculo (no avisa): solo la orden.
            EXPECT_EQ(added.section('\n', 0, 0).toStdString(), s["added"].toString().section('\n', 0, 0).toStdString());
        } else if (s["traceback"].toBool()) {
            // Error de Python que no es de Tcl: el port no muestra el traceback (y sin él, no se pasa a mayúsculas
            // lo que el original pasa por un "]" del traceback): se compara la primera línea del error.
            const QString exp = s["added"].toString();
            EXPECT_EQ(added.section('\n', 0, 1).toUpper().toStdString(), exp.section('\n', 0, 1).toUpper().toStdString());
        } else {
            EXPECT_EQ(added.toStdString(), s["added"].toString().toStdString());
        }
        const QJsonArray objs = s["objects"].toArray();
        ASSERT_EQ(static_cast<int>(app.collection()->objects().size()), objs.size());
        for (const QJsonValue& ov : objs) {
            const QJsonObject oo = ov.toObject();
            SCOPED_TRACE(oo["name"].toString().toStdString());
            auto* geo = dynamic_cast<GeometryObject*>(app.collection()->byName(oo["name"].toString()));
            ASSERT_NE(geo, nullptr);
            EXPECT_TRUE(geo->isSingleGeo());
            expectSameShapes(geo->solidGeometry(), shapesOf(oo["solid"]));
            // Un objeto de una sola geometría se dibuja y se corta con su solid_geometry: la herramienta 1 la copia.
            ASSERT_TRUE(geo->tools().count(1));
            expectSameShapes(geo->tools().at(1).solidGeometry, geo->solidGeometry());
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
