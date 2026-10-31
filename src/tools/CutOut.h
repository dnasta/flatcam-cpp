/**
 * @file CutOut.h
 * @brief Herramienta de corte de contorno con puentes (port de `appTools/ToolCutOut.py`).
 */
#pragma once

#include <string>
#include <vector>

#include "core/Shape.h"
#include "core/Types.h"

namespace flatcam {

/** @brief Dónde se dejan los puentes (`tools_cutout_gaps_ff`). */
enum class CutoutGaps {
    None,    ///< Sin puentes: el contorno queda cerrado.
    LR,      ///< Un puente a la izquierda y otro a la derecha.
    TB,      ///< Un puente arriba y otro abajo.
    TwoLR,   ///< Dos puentes por cada lado izquierdo y derecho.
    TwoTB,   ///< Dos puentes por cada lado superior e inferior.
    Four,    ///< Cuatro puentes: izquierda, derecha, arriba y abajo.
    Eight    ///< Ocho puentes: dos por cada lado.
};

/** @brief Cómo se recorta el objeto de origen. */
enum class CutoutKind {
    Single,  ///< Un solo contorno alrededor de todo el objeto.
    Multi    ///< Un contorno por cada figura del objeto.
};

/** @brief Opciones del corte de contorno libre (los `tools_cutout_*` de FlatCAM). */
struct CutoutOptions {
    double toolDia{3.0};          ///< Diámetro de la fresa (`tools_cutout_tooldia`).
    double margin{0.0};           ///< Margen entre el objeto y el corte (`tools_cutout_margin`).
    CutoutGaps gaps{CutoutGaps::Four};   ///< Ubicación de los puentes (`tools_cutout_gaps_ff`).
    double gapSize{4.0};          ///< Ancho de cada puente (`tools_cutout_gapsize`).
    bool convexBox{false};        ///< Usar la envolvente convexa del objeto (`tools_cutout_convexshape`).
    CutoutKind kind{CutoutKind::Single};  ///< Un contorno o uno por figura (`tools_cutout_obj_kind`).
    bool thinBridges{false};      ///< Los puentes se fresan a poca profundidad (`tools_cutout_gap_type == "bt"`).
    double thinDepth{0.0};        ///< Profundidad de los puentes finos (`tools_cutout_gap_depth`); 0 los desactiva.
    bool mouseBites{false};       ///< Los puentes llevan agujeros "mouse bites" (`tools_cutout_gap_type == "mb"`).
    double mbDia{0.5};            ///< Diámetro de los agujeros (`tools_cutout_mb_dia`).
    double mbSpacing{0.25};       ///< Espacio entre agujeros (`tools_cutout_mb_spacing`).
};

/** @brief Resultado del corte de contorno. */
struct CutoutResult {
    Shape solidGeometry;                 ///< Contorno final con los puentes abiertos (líneas unidas).
    ShapeList gapsSolidGeometry;         ///< Tramos de los puentes finos (solo con @ref CutoutOptions::thinBridges).
    std::vector<Point2D> mouseBiteHoles; ///< Centros de los agujeros (solo con @ref CutoutOptions::mouseBites).
};

/**
 * @brief Genera la geometría de corte de contorno de un objeto, con puentes para que la placa no se suelte.
 *
 * Es un port de `CutOut.on_freeform_cutout` y de sus funciones auxiliares. El objeto se rodea con una línea
 * desplazada la mitad de la fresa más el margen, y a esa línea se le abren los puentes: se restan cajas del ancho
 * del puente más el diámetro de la fresa.
 *
 * @code
 * flatcam::Gerber gerber;
 * gerber.parseFile("Edge_Cuts.gm1");
 * flatcam::CutoutOptions opt;
 * opt.toolDia = 1.2;
 * opt.gaps = flatcam::CutoutGaps::TB;
 * opt.gapSize = 0.4;
 * flatcam::CutoutResult result;
 * flatcam::CutOut::freeform(gerber.solidGeometry(), true, opt, result);
 * @endcode
 */
class CutOut {
public:
    /**
     * @brief Corte de contorno libre (`on_freeform_cutout`).
     * @param objectGeometry Figuras del objeto a recortar.
     * @param isGerber `true` si el objeto es un Gerber (el contorno se desplaza por fuera); `false` si es una
     *        geometría (el contorno se usa tal cual).
     * @param options Opciones del corte.
     * @param[out] result Contorno, puentes finos y agujeros.
     * @param[out] error Mensaje si falla.
     * @return `true` si se generó el contorno.
     */
    static bool freeform(const ShapeList& objectGeometry, bool isGerber, const CutoutOptions& options,
                         CutoutResult& result, std::string& error);

    /**
     * @brief Corte de contorno rectangular (`on_rectangular_cutout`).
     *
     * Rodea el objeto con un rectángulo de sus límites, agrandado por la mitad de la fresa más el margen en un
     * Gerber, y le abre los puentes. Es el que genera esquinas redondeadas de radio igual a la mitad de la fresa
     * alrededor de esquinas cuadradas. Sin puentes (`CutoutGaps::None`) el original no llega a generar líneas y
     * falla, y también falla con margen negativo en varias figuras de un Gerber y con "mouse bites" en varias figuras
     * (un `TypeError` del original); este port devuelve un error en esos casos.
     * @param objectGeometry Figuras del objeto a recortar.
     * @param isGerber `true` si el objeto es un Gerber.
     * @param options Opciones del corte (no usa @ref CutoutOptions::convexBox).
     * @param[out] result Contorno, puentes finos y agujeros.
     * @param[out] error Mensaje si falla.
     * @return `true` si se generó el contorno.
     */
    static bool rectangular(const ShapeList& objectGeometry, bool isGerber, const CutoutOptions& options,
                            CutoutResult& result, std::string& error);

    /**
     * @brief Caja que se resta para abrir un puente manual en la posición pulsada (`cutting_geo`).
     * @param pos Posición del puente (ya ajustada a la grilla).
     * @param toolDia Diámetro de la fresa.
     * @param gapSize Ancho del puente.
     * @return Un cuadrado de lado `toolDia + gapSize` centrado en @p pos.
     */
    static Shape cuttingBox(const Point2D& pos, double toolDia, double gapSize);

    /**
     * @brief Abre un puente manual en un contorno ya generado (`on_manual_cutout`).
     * @param solidGeometry Contorno actual.
     * @param pos Posición del puente.
     * @param toolDia Diámetro de la fresa.
     * @param gapSize Ancho del puente.
     * @return El contorno con el puente abierto y las líneas unidas.
     */
    static Shape manualGap(const Shape& solidGeometry, const Point2D& pos, double toolDia, double gapSize);

    /**
     * @brief Expande figuras múltiples y polígonos en líneas (`CutOut.flatten`).
     *
     * De un polígono solo se toma el contorno exterior, igual que el original.
     * @param geometry Figuras.
     * @return Lista de figuras simples no vacías.
     */
    static ShapeList flatten(const ShapeList& geometry);

    /**
     * @brief Límites que abarcan todas las figuras (`CutOut.recursive_bounds`).
     * @param geometry Figuras.
     * @return Los límites, o nada si no hay figuras con contenido.
     */
    static std::optional<BoundingBox> recursiveBounds(const ShapeList& geometry);

    /**
     * @brief Resta una caja de las líneas de la geometría (`subtract_poly_from_geo`).
     * @param geometry Figuras (solo se recortan las líneas y anillos).
     * @param xmin Esquina inferior izquierda de la caja, X.
     * @param ymin Esquina inferior izquierda de la caja, Y.
     * @param xmax Esquina superior derecha de la caja, X.
     * @param ymax Esquina superior derecha de la caja, Y.
     * @return La unión de las líneas recortadas.
     */
    static Shape subtractBoxFromGeo(const ShapeList& geometry, double xmin, double ymin, double xmax, double ymax);

    /**
     * @brief Resta una figura de las líneas de la geometría (`subtract_geo`).
     * @param target Figuras de las que se resta.
     * @param subtractor Figura a restar.
     * @return La unión de las líneas recortadas.
     */
    static Shape subtractGeo(const ShapeList& target, const Shape& subtractor);

    /**
     * @brief Partes de la geometría que caen dentro de una figura (`intersect_geo`).
     * @param target Figuras a intersectar.
     * @param second Figura con la que se intersecta.
     * @return Las intersecciones, expandidas a figuras simples.
     */
    static ShapeList intersectGeo(const ShapeList& target, const Shape& second);
};

}  // namespace flatcam
