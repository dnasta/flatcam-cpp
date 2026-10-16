/**
 * @file GeometryError.h
 * @brief Excepción lanzada cuando falla una operación geométrica.
 */
#pragma once

#include <stdexcept>
#include <string>

namespace flatcam {

/**
 * @brief Error de una operación geométrica (por ejemplo, una geometría inválida o un fallo de GEOS).
 *
 * Reemplaza a las excepciones de Shapely/GEOS (`TopologicalError`, `ValueError`, ...) que FlatCAM
 * captura con `try/except` en Python.
 */
class GeometryError : public std::runtime_error {
public:
    /**
     * @brief Crea el error con un mensaje descriptivo.
     * @param what Descripción del fallo.
     */
    explicit GeometryError(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace flatcam
