/**
 * @file OriginalBugs.h
 * @brief Interruptor global para reproducir los errores del FlatCAM original (sección 14 de AGENTS.md).
 *
 * El port corrige los errores conocidos del FlatCAM 2024.4 en Python (desvíos conscientes). Mientras se portaba se
 * los reproducía a propósito para poder comparar el G-code y los archivos con el original; esos comportamientos siguen
 * en el código detrás de este interruptor:
 * - apagado (de fábrica, en la aplicación): el comportamiento corregido;
 * - encendido: exactamente como el original. Lo encienden los tests que comparan con los oráculos y la opción
 *   `--original-bugs` de la línea de comandos (o la variable de entorno `FLATCAM_ORIGINAL_BUGS=1`).
 */
#ifndef FLATCAM_CORE_ORIGINALBUGS_H
#define FLATCAM_CORE_ORIGINALBUGS_H

namespace flatcam {

/** @brief Si se reproducen los errores del original. @return Verdadero si está encendido. */
bool reproduceOriginalBugs();

/** @brief Enciende o apaga la reproducción de los errores del original. @param on El estado nuevo. */
void setReproduceOriginalBugs(bool on);

/** @brief Cambia el interruptor mientras vive el objeto y lo restaura al destruirse (para los tests). */
class OriginalBugsScope {
public:
    /** @brief Fija el estado. @param on El estado mientras vive el objeto. */
    explicit OriginalBugsScope(bool on) : saved_(reproduceOriginalBugs()) { setReproduceOriginalBugs(on); }
    /** @brief Restaura el estado anterior. */
    ~OriginalBugsScope() { setReproduceOriginalBugs(saved_); }
    OriginalBugsScope(const OriginalBugsScope&) = delete;             ///< No se copia.
    OriginalBugsScope& operator=(const OriginalBugsScope&) = delete;  ///< No se copia.

private:
    bool saved_;
};

}  // namespace flatcam

#endif
