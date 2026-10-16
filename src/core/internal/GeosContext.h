/**
 * @file GeosContext.h
 * @brief Detalle interno: manejo del contexto reentrante de GEOS (no forma parte de la API pública).
 */
#pragma once

#include <geos_c.h>

#include <string>

namespace flatcam::detail {

/**
 * @brief Acceso al contexto GEOS del hilo actual y al último mensaje de error.
 *
 * Cada hilo tiene su propio contexto (API reentrante `_r` de GEOS), por lo que las operaciones
 * geométricas se pueden ejecutar en paralelo en hilos distintos.
 */
class GeosContext {
public:
    /** @brief Contexto GEOS del hilo actual (se crea la primera vez que se pide). */
    static GEOSContextHandle_t handle();

    /** @brief Último mensaje de error informado por GEOS en este hilo. */
    static std::string& lastError();
};

}  // namespace flatcam::detail
