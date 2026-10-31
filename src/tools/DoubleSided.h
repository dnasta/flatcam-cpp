/**
 * @file DoubleSided.h
 * @brief PCB a 2 caras: agujeros de alineación y reflejo de la placa (port de `appTools/ToolDblSided.py`).
 */
#pragma once

#include <vector>

#include "core/Types.h"
#include "parsers/Excellon.h"

namespace flatcam {

/**
 * @brief Agujeros de alineación para fabricar una placa por las dos caras.
 *
 * Reproduce `on_create_alignment_holes` de FlatCAM: cada agujero que se pide genera además su reflejo respecto de un
 * eje que pasa por un punto, y así la cara de abajo cae justo sobre la de arriba al darla vuelta.
 */
class DoubleSided {
public:
    /**
     * @brief Centro de una caja (la ubicación "caja" del eje: `0.5 * (min + max)`).
     * @param box Límites del objeto de referencia.
     * @return El centro de la caja.
     */
    static Point2D boxCenter(const BoundingBox& box);

    /**
     * @brief Agujeros de alineación con sus reflejos.
     *
     * Para cada agujero pedido se emite primero el agujero y después su reflejo (`affinity.scale` con origen en el
     * punto del eje), como en el original.
     * @param holes Agujeros pedidos.
     * @param axis Eje de alineación: con `X` se invierte Y (eje horizontal); con `Y` se invierte X (eje vertical).
     * @param pivot Punto por el que pasa el eje.
     * @return Los agujeros, en el orden del original: `[a, reflejo(a), b, reflejo(b), ...]`.
     */
    static std::vector<Point2D> alignmentDrills(const std::vector<Point2D>& holes, Axis axis, const Point2D& pivot);

    /**
     * @brief Excellon de una sola herramienta con los agujeros de alineación (objeto "Alignment Drills").
     * @param holes Agujeros pedidos.
     * @param axis Eje de alineación.
     * @param pivot Punto por el que pasa el eje.
     * @param diameter Diámetro de la herramienta.
     * @param options Opciones del Excellon (pasos por círculo y unidades).
     * @return El objeto, con su geometría creada.
     */
    static Excellon alignmentExcellon(const std::vector<Point2D>& holes, Axis axis, const Point2D& pivot, double diameter,
                                      const ExcellonOptions& options = ExcellonOptions());
};

}  // namespace flatcam
