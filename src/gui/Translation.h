/**
 * @file Translation.h
 * @brief Traducción de la interfaz a los idiomas del original (los `.bin` de `translate/`): alemán, inglés, español, francés,
 * italiano, portugués de Brasil, rumano, ruso y turco.
 *
 * Las tablas están en `:/i18n/<idioma>.json` (las arma `tools/gui_dump/build_translations.py`): `msg` son los
 * mensajes de la lógica (`tr_`, el `_()` del original, con su diccionario `.bin` y los textos propios del port) y `ui`
 * los textos de la interfaz generada (`trUi_`), tal como los muestra el original en ese idioma.
 */
#pragma once

#include <QString>
#include <QStringList>

namespace flatcam::gui {

/**
 * @brief Traduce un mensaje del original en inglés al idioma de la interfaz.
 *
 * Equivale al `_()` de FlatCAM: busca el texto exacto en la tabla `msg` del idioma y, si no está, devuelve el texto
 * original.
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

/**
 * @brief Un texto de la interfaz generada (`src/gui/generated/`): el que muestra el original en el idioma elegido.
 * @param english El texto del original en inglés (completo, con atajos y signos).
 * @return El texto en el idioma de la interfaz, o el mismo si no cambia.
 */
QString trUi_(const QString& english);

/** @brief Los códigos de los idiomas disponibles (`de`, `en`, `es`...), en el orden de sus nombres. @return Los códigos. */
QStringList availableLanguages();

/**
 * @brief El nombre de un idioma en ese idioma ("Español", "Deutsch"...), como en el combo de Preferencias.
 * @param code Código. @return El nombre, o vacío si no está.
 */
QString languageName(const QString& code);

/**
 * @brief El código de un idioma a partir de su nombre ("Español" -> "es").
 * @param name Nombre. @return El código, o vacío si no está.
 */
QString languageCode(const QString& name);

/**
 * @brief El idioma del sistema, si está disponible (`es_AR` -> `es`, `pt_BR` -> `pt_BR`); si no, inglés.
 * @return Su código.
 */
QString systemLanguage();

/** @brief El idioma de la interfaz (de fábrica, el del sistema). @return Su código. */
QString language();

/**
 * @brief Cambia el idioma de la interfaz. Los textos ya creados no cambian: se usa al arrancar (el original reinicia
 * la aplicación al cambiarlo).
 * @param code Código del idioma.
 * @return Falso si no está disponible (queda el anterior).
 */
bool setLanguage(const QString& code);

/**
 * @brief El idioma con que arranca la aplicación: `FLATCAM_LANGUAGE`, si no el guardado en `settings.ini`
 * (`language`), y si no el del sistema.
 * @param settingsFile Ruta del `settings.ini`.
 * @return Su código.
 */
QString startupLanguage(const QString& settingsFile);

}  // namespace flatcam::gui
