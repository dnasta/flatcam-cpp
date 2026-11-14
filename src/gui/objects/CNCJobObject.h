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
    JsonValue data;            ///< Parámetros con que se generó (`cnc_tools[..]['data']`), para guardar el proyecto.
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
    /** @brief Datos de la cabecera del archivo (nombre, tipo, unidades...). @return Los datos. */
    const GcodeHeaderInfo& header() const { return header_; }

    /** @brief La autonivelación del trabajo (se crea al pedirla). @param app La aplicación. @return El controlador. */
    class CNCAutolevel* autolevel(class App* app);
    /** @brief La autonivelación, si ya se creó. @return El controlador o nulo. */
    class CNCAutolevel* autolevelIfAny() const { return autolevel_; }

    /** @brief Herramientas usadas. @return Las filas. */
    const std::vector<CncToolRow>& tools() const { return tools_; }

    /**
     * @brief Texto completo del archivo, con la cabecera de FlatCAM y la fecha actual (`export_gcode`).
     *
     * Como el original: un fragmento vacío se reemplaza por el de las Preferencias (`cncjob_prepend` /
     * `cncjob_append`); con `cncjob_footer` termina en `M02`; los postprocesadores sin cabecera (`include_header =
     * False`: Roland_MDX_20 e ISEL_ICP_CNC) dan solo los fragmentos y el cuerpo, y con hpgl las órdenes `PA` pasan a
     * coordenadas enteras.
     * @param preamble Fragmento del principio (`preamble`).
     * @param postamble Fragmento del final (`postamble`).
     * @return El G-code.
     */
    std::string exportText(std::string preamble = {}, std::string postamble = {}) const;

    /**
     * @brief Guarda el G-code en un archivo (`export_gcode` con `filename`): con los fines de línea del sistema, o
     * `\r\n` si `cncjob_line_ending` está activo.
     * @param filename Ruta.
     * @param preamble Fragmento del principio.
     * @param postamble Fragmento del final.
     * @return `true` si se pudo escribir.
     */
    bool saveTo(const QString& filename, const std::string& preamble = {}, const std::string& postamble = {}) const;

    /** @brief El postprocesador de la primera herramienta (`ppname_g` / `tools_drill_ppname_e`). @return El nombre. */
    std::string preprocessorName() const;

    /**
     * @brief `set_ui`: toma los fragmentos de las Preferencias y arma `source_file` con `export_gcode` (lo que carga el
     * editor de G-code). Lo llama la aplicación al agregar el objeto.
     */
    void initSourceFile();
    /** @brief `source_file`: el G-code que carga el editor de G-code (y que este reemplaza al guardar). @return Texto. */
    const QString& sourceFile() const { return sourceFile_; }
    /** @brief Cambia `source_file`. @param s Texto. */
    void setSourceFile(const QString& s) { sourceFile_ = s; }
    /** @brief La cabecera con que se armó `source_file` (`gc_header`). @return Texto. */
    const std::string& sourceHeader() const { return sourceHeader_; }
    /** @brief `prepend_snippet`. @return Texto. */
    const QString& prependSnippet() const { return prependSnippet_; }
    /** @brief `append_snippet`. @return Texto. */
    const QString& appendSnippet() const { return appendSnippet_; }

    /**
     * @brief Cabecera del archivo (`gc_header`): la guardada en el proyecto o una nueva con la fecha actual.
     * @return El texto.
     */
    std::string headerText() const;

    /** @brief Si es un trabajo de taladrado (`origin_kind == 'excellon'`). @return `true` si lo es. */
    bool isDrilling() const { return !tools_.empty() && (tools_.front().drills > 0 || tools_.front().slots > 0); }

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
    class CNCAutolevel* autolevel_{nullptr};  ///< Hijo de este objeto (QObject).
    std::vector<CncToolRow> tools_;
    std::optional<std::string> storedHeader_;
    QString sourceFile_;
    std::string sourceHeader_;
    QString prependSnippet_;
    QString appendSnippet_;
    /** Arma el archivo con una cabecera dada. */
    std::string compose(const std::string& header, std::string preamble, std::string postamble) const;
};

}  // namespace flatcam::gui
