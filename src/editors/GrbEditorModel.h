/**
 * @file GrbEditorModel.h
 * @brief El modelo del editor de Gerber (port de lo que no es interfaz de `appEditors/AppGerberEditor.py`): los
 * elementos (`solid`, `follow`, `clear`) de cada apertura, las figuras de las herramientas de dibujo, engrosar,
 * escalar, poligonizar, marcar áreas y el Gerber que sale al guardar.
 */
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"
#include "parsers/Gerber.h"

namespace flatcam {

/// Un elemento del editor (`DrawToolShape` con un diccionario `solid`/`follow`/`clear`), compartido por puntero.
using GrbShapePtr = std::shared_ptr<ApertureGeometry>;

/** @brief Las figuras de las herramientas de dibujo del editor de Gerber. */
namespace grbdraw {

/**
 * @brief `PadEditorGrb.util_shape`: el pad de la apertura en @p p (C: círculo de 16 segmentos por cuarto; R:
 * rectángulo; O: óvalo).
 * @param ap La apertura. @param p Centro. @param steps `geometry_circle_steps`.
 * @return El elemento, o nada si la apertura no es C, R ni O.
 */
std::optional<ApertureGeometry> pad(const Aperture& ap, const Point2D& p, int steps);

/**
 * @brief El punto intermedio de una pista o región con codo (`bend_mode` 1 a 4; 5 es ángulo libre).
 *
 * Reproduce las tres variantes del original: la pista (`TrackEditorGrb`) y el primer tramo de la región usan
 * `old_y - gridy * (mx - my)` cuando el punto está más arriba (el codo sale para abajo); los tramos siguientes de la
 * región, `old_y + gridy * (my - mx)`.
 * @param mode Modo. @param old El último punto. @param cur El mouse. @param gx Grilla X. @param gy Grilla Y.
 * @param later Tramo de la región después del primero. @param prev El punto anterior (queda si no hay uno nuevo).
 * @return El punto, si hay.
 */
std::optional<Point2D> bend(int mode, const Point2D& old, const Point2D& cur, double gx, double gy, bool later,
                            std::optional<Point2D> prev);

/**
 * @brief `DiscSemiEditorGrb.make`: el semidisco (el polígono del arco) agrandado la mitad de @p buf.
 * @param mode 0 `c12`, 1 `132`, 2 `12c`. @param a Primero. @param b Segundo. @param c Tercero.
 * @param cw Sentido (en `132` sale de los puntos). @param buf `buf_val`. @param steps Segmentos por vuelta.
 * @return El elemento, o nada si los puntos están alineados.
 */
std::optional<ApertureGeometry> semiDisc(int mode, const Point2D& a, const Point2D& b, const Point2D& c, bool cw,
                                         double buf, int steps);

}  // namespace grbdraw

/** @brief Las aperturas del editor de Gerber (`storage_dict`) y lo que se hace con ellas. */
class GrbEditorModel {
public:
    /** @brief Una apertura con sus elementos. */
    struct Ap {
        std::string code;                  ///< Código (`10`, ...).
        Aperture ap;                       ///< Tipo, tamaño, ancho, alto...
        std::vector<GrbShapePtr> geometry; ///< Sus elementos.
    };
    std::vector<Ap> apertures;            ///< `storage_dict`, en el orden del Gerber.
    std::vector<GrbShapePtr> selected;    ///< `selected`.

    /** @brief Busca una apertura. @param code Código. @return La apertura, o nula. */
    Ap* aperture(const std::string& code);
    /** @brief `on_aperture_add(apcode='0')`: la apertura de las regiones, si no está. @return La apertura. */
    Ap& ensureZero();
    /** @brief La apertura de un elemento. @param s El elemento. @return El código, o nada. */
    std::optional<std::string> codeOf(const GrbShapePtr& s) const;

    /**
     * @brief `edit_fcgerber`: copia las aperturas con sus elementos; a cada sólido se le restan los borrados que
     * quedan adentro (`within`), como el original.
     * @param g El Gerber.
     */
    void load(const Gerber& g);

    /** @brief `add_gerber_shape`. @param code Apertura. @param e El elemento. @return Lo agregado. */
    GrbShapePtr add(const std::string& code, const ApertureGeometry& e);
    /** @brief `delete_shape`. @param s El elemento. */
    void remove(const GrbShapePtr& s);
    /** @brief `delete_selected`. */
    void deleteSelected();

    /**
     * @brief `SelectEditorGrb.selection_worker`: en cada apertura, el primer sólido de cada tramo de 77 que toca el
     * punto cambia de estado (elegido o no).
     * @param p El punto.
     */
    void clickSelect(const Point2D& p);

    /** @brief `draw_selection_area_handler`. @param a Esquina. @param b Opuesta. @param contains Contenidas o que
     *  toca. @param toggle Elección múltiple. */
    void areaSelect(const Point2D& a, const Point2D& b, bool contains, bool toggle);

    /** @brief `on_buffer`: engrosa los elegidos de esas aperturas (sólido, seguimiento y borrado).
     *  @param codes Aperturas elegidas en la tabla. @param dist Distancia. @param join 1 redondo, 2 inglete, 3 bisel. */
    void buffer(const std::vector<std::string>& codes, double dist, int join);
    /** @brief `on_scale`: escala los elegidos de esas aperturas desde su centro. @param codes Aperturas.
     *  @param factor Factor. */
    void scale(const std::vector<std::string>& codes, double factor);
    /** @brief `on_markarea`: los elementos con un área entre los límites. @param lower Menor. @param upper Mayor.
     *  @return Los elementos. */
    std::vector<GrbShapePtr> markArea(double lower, double upper) const;

    /**
     * @brief `PoligonizeEditorGrb.make`: une los exteriores de los elegidos (de una sola apertura).
     * @param lastAperture La apertura elegida (`last_aperture_selected`).
     * @return Falso si hay de varias aperturas o ninguno.
     */
    bool poligonize(const std::string& lastAperture);

    /**
     * @brief Mueve (o copia) los elegidos de esas aperturas.
     * @param codes Aperturas. @param dx X. @param dy Y. @param copy Copiar.
     */
    void move(const std::vector<std::string>& codes, double dx, double dy, bool copy);

    /** @brief `EraserEditorGrb.make`: los elegidos, movidos, borran lo que tocan. @param dx X. @param dy Y. */
    void erase(double dx, double dy);

    /** @brief El Gerber que sale al guardar (`new_edited_gerber`). */
    struct Out {
        ShapeList solid;                            ///< Geometría sólida.
        ShapeList follow;                           ///< Seguimiento.
        std::map<std::string, Aperture> apertures;  ///< Aperturas (las vacías no van).
        std::vector<std::string> order;             ///< Orden de las aperturas.
    };
    /** @brief `new_edited_gerber`. @return El Gerber. */
    Out result() const;
};

}  // namespace flatcam
