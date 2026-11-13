/**
 * @file PcbWizard.h
 * @brief Asistente de PCB (port de `ToolPcbWizard.load_inf` y `on_file_loaded`): lee el `.INF` de PcbWizard (las
 * herramientas y el formato) y le agrega al Excellon la cabecera que le falta.
 */
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace flatcam {

/** @brief Lo que trae el `.INF`. */
struct PcbWizardInf {
    std::vector<std::pair<int, double>> tools;  ///< `tools_from_inf`: herramienta y diámetro, en el orden del archivo.
    std::optional<int> integral;                ///< Dígitos enteros del formato, si se encontró.
    std::optional<int> fractional;              ///< Dígitos decimales.
    std::optional<std::string> units;           ///< `INCH` o `METRIC`.
};

/**
 * @brief `readlines()` de Python en modo texto: las líneas con su `\n` (los `\r\n` y `\r` pasan a `\n`).
 * @param path Ruta del archivo. @param ok Si se pudo leer. @return Las líneas.
 */
std::vector<std::string> pcbWizardReadLines(const std::string& path, bool* ok = nullptr);

/**
 * @brief `load_inf`: agrega a `inf` las herramientas del archivo (como el original, no se vacía al leer otro `.INF`).
 * @param lines Las líneas del archivo. @param inf Lo leído hasta ahora.
 * @return `false` si no hay ninguna herramienta (el original avisa y no sigue).
 */
bool pcbWizardReadInf(const std::vector<std::string>& lines, PcbWizardInf& inf);

/**
 * @brief `on_file_loaded`: el Excellon con la cabecera agregada después de la línea que contiene `M48`.
 * @param excLines Las líneas del Excellon (con su fin de línea). @param inf Las herramientas.
 * @param integral Dígitos enteros. @param fractional Decimales. @param units `INCH` o `METRIC`. @param zeros `LZ` o
 * `TZ`. @param version Versión de FlatCAM. @param versionDate Su fecha. @param timeStr "Created on".
 * @return El texto.
 */
std::string pcbWizardMerge(const std::vector<std::string>& excLines, const PcbWizardInf& inf, int integral,
                           int fractional, const std::string& units, const std::string& zeros,
                           const std::string& version, const std::string& versionDate, const std::string& timeStr);

}  // namespace flatcam
