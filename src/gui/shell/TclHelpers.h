/**
 * @file TclHelpers.h
 * @brief Ayudantes de los comandos de la consola Tcl: lo que el original hace con Python (`type(obj)`,
 * `bool(eval(texto.capitalize()))`, `repr` de un `float`) y el nombre de un archivo.
 */
#pragma once

#include <cmath>

#include <QString>

#include "cam/PyNumber.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/shell/TclCommand.h"

namespace flatcam::gui::tclhelpers {

/** @brief `filename.split('/')[-1].split('\\')[-1]`. @param f Ruta. @return El nombre. */
inline QString baseName(const QString& f) { return f.section('/', -1).section('\\', -1); }

/** @brief El tipo de un objeto como lo escribe `type(obj)` en los mensajes. @param o Objeto. @return El texto. */
inline QString pyType(const FlatCAMObj* o) {
    switch (o->kind()) {
        case ObjectKind::Gerber: return "<class 'appObjects.FlatCAMGerber.GerberObject'>";
        case ObjectKind::Excellon: return "<class 'appObjects.FlatCAMExcellon.ExcellonObject'>";
        case ObjectKind::Geometry: return "<class 'appObjects.FlatCAMGeometry.GeometryObject'>";
        case ObjectKind::CNCJob: return "<class 'appObjects.FlatCAMCNCJob.CNCJobObject'>";
        case ObjectKind::Script: return "<class 'appObjects.FlatCAMScript.ScriptObject'>";
        case ObjectKind::Document: return "<class 'appObjects.FlatCAMDocument.DocumentObject'>";
    }
    return "<class 'object'>";
}

/**
 * @brief El nombre de la clase de un objeto (`type(obj).__name__`), como en los `AttributeError` de Python.
 * @param o Objeto (nulo: `NoneType`). @return El nombre.
 */
inline QString pyClassName(const FlatCAMObj* o) {
    if (!o) return "NoneType";
    const QString t = pyType(o);
    return t.mid(t.lastIndexOf('.') + 1).chopped(2);
}

/**
 * @brief `bool(eval(texto))` para los textos que usan los comandos: `True`, `False`, `None` o un número (otro nombre
 * es un error, como el `NameError` de Python).
 * @param t Texto. @return El valor.
 */
inline bool pyEvalTruth(const QString& t) {
    if (t == "True") return true;
    if (t == "False" || t == "None") return false;
    bool ok = false;
    const double d = t.toDouble(&ok);
    if (ok) return d != 0;
    throw TclError(QString("name '%1' is not defined").arg(t));
}

/** @brief `bool(eval(texto.capitalize()))`. @param t Texto. @return El valor. */
inline bool pyTruthCapitalized(QString t) {
    if (!t.isEmpty()) t = t.left(1).toUpper() + t.mid(1).toLower();
    return pyEvalTruth(t);
}

/** @brief Un `float` como lo escribe Tcl (igual que `repr` de Python). @param v Valor. @return El texto. */
inline QString tclFloat(double v) {
    if (std::isinf(v)) return v > 0 ? "Inf" : "-Inf";
    if (std::isnan(v)) return "NaN";
    return QString::fromStdString(pyFloatRepr(v));
}

}  // namespace flatcam::gui::tclhelpers
