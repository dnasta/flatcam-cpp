/**
 * @file CNCJobObject.h
 * @brief Objeto CNC Job: el G-code generado, su dibujo y su exportación (port de `appObjects/FlatCAMCNCJob.py`).
 */
#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "cam/CncJob.h"
#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

/** @brief Datos de una herramienta del trabajo, para la tabla de Propiedades (`exc_cnc_tools`). */
struct CncToolRow {
    int tool{0};          ///< Número de herramienta.
    double dia{0.0};      ///< Diámetro.
    int drills{0};        ///< Cantidad de agujeros.
    int slots{0};         ///< Cantidad de ranuras.
    double cutZ{0.0};     ///< Z de corte (con el desplazamiento aplicado).
    QString offset{"Path"};    ///< Desplazamiento de la herramienta (trabajos de geometría).
    QString type{"Rough"};     ///< Tipo de trabajo (trabajos de geometría).
    QString toolType{"C1"};    ///< Forma de la herramienta (trabajos de geometría).
};

/**
 * @brief Objeto CNC Job (`CNCJobObject`).
 *
 * Guarda el trabajo generado y lo dibuja como el original con `cncjob_plot_kind = all`: los viajes en amarillo
 * translúcido y lo que corta en azul (los agujeros, como círculos del diámetro de su herramienta).
 */
class CNCJobObject : public FlatCAMObj {
    Q_OBJECT
public:
    /**
     * @brief Crea el objeto.
     * @param name Nombre (por ejemplo `<objeto>_cnc`).
     * @param job Trabajo ya generado.
     * @param header Datos de la cabecera del archivo (tipo de origen, unidades).
     * @param tools Herramientas usadas (tabla de Propiedades).
     */
    CNCJobObject(QString name, std::unique_ptr<CncJob> job, GcodeHeaderInfo header, std::vector<CncToolRow> tools);

    /** @brief El trabajo. @return Referencia. */
    const CncJob& job() const { return *job_; }

    /** @brief Herramientas usadas. @return Las filas. */
    const std::vector<CncToolRow>& tools() const { return tools_; }

    /**
     * @brief Texto completo del archivo, con la cabecera de FlatCAM y la fecha actual (`export_gcode`).
     * @return El G-code.
     */
    std::string exportText() const;

    /**
     * @brief Guarda el G-code en un archivo, con los fines de línea del sistema como el original.
     * @param filename Ruta.
     * @return `true` si se pudo escribir.
     */
    bool saveTo(const QString& filename) const;

    /** @brief Viajes, cortes y agujeros. @return Datos. */
    PlotLayerData plotData() const override;

    /** @brief Límites del recorrido. @return Rectángulo. */
    QRectF bounds() const override;

    /** @brief Panel `CNCObjectUI` cargado. @return El panel. */
    QWidget* propertiesUi() override;

    /**
     * @brief Cabecera guardada en un proyecto (`gc_header`): al exportar se usa tal cual, como el original, en vez de
     * generar una con la fecha actual.
     * @param header Texto de la cabecera.
     */
    void setStoredHeader(std::string header) { storedHeader_ = std::move(header); }

    /** @brief Color de los viajes (`cncjob_travel_line`). @return El color. */
    static QColor travelColor();

private:
    std::unique_ptr<CncJob> job_;
    GcodeHeaderInfo header_;
    std::vector<CncToolRow> tools_;
    std::optional<std::string> storedHeader_;
};

}  // namespace flatcam::gui
