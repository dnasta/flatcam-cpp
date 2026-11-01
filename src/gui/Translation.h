/**
 * @file Translation.h
 * @brief Traducción de los mensajes de la aplicación, con la misma tabla que FlatCAM (`translate/es.bin`).
 */
#pragma once

#include <QString>

namespace flatcam::gui {

/**
 * @brief Traduce un texto del original en inglés al idioma de la interfaz (por ahora, español).
 *
 * Equivale al `_()` de FlatCAM: busca el texto exacto en la tabla `:/es.json` (generada desde `translate/es.bin` por
 * `tools/gui_dump/extract_resources.py`) y, si no está, devuelve el texto original.
 * @param english Texto en inglés, tal como aparece en el código de FlatCAM.
 * @return El texto traducido.
 */
QString tr_(const char* english);

/**
 * @brief Igual que la versión con `const char*`, para textos que ya son `QString`.
 * @param english Texto en inglés, tal como aparece en el código de FlatCAM.
 * @return El texto traducido.
 */
QString tr_(const QString& english);

}  // namespace flatcam::gui
