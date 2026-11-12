/**
 * @file RulesCheck.h
 * @brief Verificación de reglas (port de las comprobaciones de `appTools/ToolRulesCheck.py`): tamaño de pistas y de
 * agujeros, separaciones entre figuras de uno o dos Gerber, entre agujeros y anillo mínimo, y el informe.
 */
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Un Gerber para las reglas: su nombre y sus aperturas. */
struct RulesGerber {
    std::string name;                           ///< Nombre del objeto.
    std::map<std::string, Aperture> apertures;  ///< Aperturas.
    std::vector<std::string> order;             ///< Orden de las aperturas (el del diccionario del original).
};

/** @brief Un Excellon para las reglas: su nombre y sus herramientas. */
struct RulesExcellon {
    std::string name;                              ///< Nombre del objeto.
    std::vector<std::pair<double, ShapeList>> tools;  ///< Diámetro y geometría de cada herramienta, en orden.
};

/** @brief Un punto del informe: una coordenada o, en el anillo mínimo, un `Point` de Shapely. */
struct RulesPoint {
    Point2D p;             ///< El punto.
    bool shapely{false};   ///< Es un `Point` (agujero que toca el borde del cobre): el informe del original falla.
};

/** @brief El resultado de una regla (`rule_title, violations` con un solo elemento). */
struct RulesResult {
    std::string title;               ///< Nombre de la regla.
    std::vector<std::string> names;  ///< Archivos.
    bool namesIsList{true};          ///< `name` es una lista (si no, es el único nombre).
    std::vector<RulesPoint> points;  ///< Violaciones (sin repetir).
    std::string failed;              ///< `points = ['Failed. ...']`.
    std::vector<double> dias;        ///< Para el tamaño de agujeros: los diámetros chicos.
    bool hasDias{false};             ///< El resultado lleva `dia` en vez de `points`.
    std::vector<double> sizes;       ///< Para el tamaño de pistas: los tamaños chicos.
};

/** @brief `check_traces_size`. @param list Los cobres. @param size Mínimo. @return Un resultado por cobre. */
std::vector<RulesResult> rulesTracesSize(const std::vector<RulesGerber>& list, double size);

/** @brief `check_inside_gerber_clearance`. @param g El Gerber. @param size Mínimo. @param title Regla.
 *  @return El resultado. */
RulesResult rulesInsideClearance(const RulesGerber& g, double size, const std::string& title);

/** @brief `check_gerber_clearance` (dos o tres Gerber: el último contra los demás). @param list Gerber.
 *  @param size Mínimo. @param title Regla. @return El resultado. */
RulesResult rulesGerberClearance(const std::vector<RulesGerber>& list, double size, const std::string& title);

/** @brief `check_holes_size`. @param list Excellon. @param size Mínimo. @return Un resultado por Excellon. */
std::vector<RulesResult> rulesHolesSize(const std::vector<RulesExcellon>& list, double size);

/** @brief `check_holes_clearance`. @param list Excellon. @param size Mínimo. @return El resultado. */
RulesResult rulesHolesClearance(const std::vector<RulesExcellon>& list, double size);

/** @brief `check_gerber_annular_ring` (un cobre y un Excellon). @param g Cobre. @param e Excellon.
 *  @param size Mínimo. @param title Regla. @return El resultado. */
RulesResult rulesAnnularRing(const RulesGerber& g, const RulesExcellon& e, double size, const std::string& title);

/**
 * @brief `on_tool_finished`: el informe en HTML (el texto del Documento `Rules_check_results`).
 * @param results Los resultados, en el orden en que se pidieron.
 * @param decimals `app.decimals`. @param status "STATUS"/"FAILED"/"PASSED" y el texto de sin violaciones, traducidos.
 * @return El texto, o vacío si el original falla (un punto que es un `Point` de Shapely).
 */
std::optional<std::string> rulesReport(const std::vector<RulesResult>& results, int decimals,
                                       const std::vector<std::string>& status);

}  // namespace flatcam
