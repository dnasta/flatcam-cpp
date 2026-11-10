/**
 * @file EtchCompensation.h
 * @brief Compensación de grabado: agranda el cobre de un Gerber lo que se come el grabado lateral (port de
 * `ToolEtchCompensation.on_compensate`, `appTools/ToolEtchCompensation.py`).
 */
#pragma once

#include <map>
#include <optional>
#include <string>

#include "core/Shape.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Cómo se calcula el desplazamiento (el `ratio_radio` del original). */
enum class EtchRatio {
    Factor,    ///< `"factor"`: espesor por el factor de grabado.
    EtchList,  ///< `"etch_list"`: espesor dividido 0,33 (CuCl2) o 0,25 (los demás grabadores).
    Manual     ///< `"manual"`: el desplazamiento escrito, en micras.
};

/**
 * @brief El desplazamiento en mm, como `on_compensate`.
 * @param ratio Tipo. @param thicknessUm Espesor del cobre en micras. @param factor Factor de grabado (solo
 * `EtchRatio::Factor`). @param etchant Grabador (solo `EtchRatio::EtchList`: `"CuCl2"` u otro).
 * @param offsetUm Desplazamiento en micras (solo `EtchRatio::Manual`).
 * @return El desplazamiento, o vacío si el factor es 0 (el original avisa y después falla con `NameError`).
 */
std::optional<double> etchOffset(EtchRatio ratio, double thicknessUm, double factor, const std::string& etchant,
                                 double offsetUm);

/** @brief El Gerber compensado. */
struct EtchResult {
    ShapeList solid;                            ///< Partes de la unión de los polígonos engrosados.
    std::map<std::string, Aperture> apertures;  ///< Aperturas con sus tamaños y sus figuras sólidas engrosadas.
};

/**
 * @brief Engrosa @p offset cada polígono de la geometría sólida y cada figura sólida de las aperturas, con @p steps
 * segmentos por cuarto de círculo, y une los polígonos (`unary_union`). En R y O suma @p offset al ancho y al alto y
 * el tamaño pasa a ser la diagonal; en los demás tipos lo suma al tamaño (si lo tienen), como el original.
 * @param g El Gerber. @param offset Desplazamiento en mm. @param steps `gerber_circle_steps`.
 * @return La geometría y las aperturas nuevas (sin geometría de seguimiento, como el original).
 */
EtchResult etchCompensate(const Gerber& g, double offset, int steps = 64);

}  // namespace flatcam
