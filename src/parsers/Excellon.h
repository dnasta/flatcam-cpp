/**
 * @file Excellon.h
 * @brief Lectura de archivos de taladrado Excellon (port de `Excellon` en `appParsers/ParseExcellon.py`).
 */
#pragma once

#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/Geometry.h"

namespace flatcam {

/**
 * @brief Una herramienta de taladrado de un archivo Excellon.
 *
 * Equivale a `tools[n]` de FlatCAM: diámetro, puntos de taladro, ranuras y la geometría sólida que resulta de
 * "dibujar" cada agujero con su diámetro.
 */
struct ExcellonTool {
    double tooldia{0.0};                                   ///< Diámetro de la herramienta.
    std::vector<Point2D> drills;                           ///< Posiciones de los agujeros (`drills`).
    std::vector<std::pair<Point2D, Point2D>> slots;        ///< Ranuras: punto inicial y final (`slots`).
    ShapeList solidGeometry;                               ///< Círculos y ranuras ya dibujados.
};

/**
 * @brief Opciones de lectura de Excellon (las preferencias `excellon_*` de FlatCAM).
 *
 * Los valores por defecto son los de las preferencias de FlatCAM 8.994 / 2024.4.
 */
struct ExcellonOptions {
    std::string zeros{"L"};                 ///< Supresión de ceros: `"L"`, `"T"` o vacío si el archivo no la indica.
    int formatUpperMm{3};                   ///< Dígitos enteros en milímetros (`excellon_format_upper_mm`).
    int formatLowerMm{3};                   ///< Dígitos decimales en milímetros.
    int formatUpperIn{2};                   ///< Dígitos enteros en pulgadas.
    int formatLowerIn{4};                   ///< Dígitos decimales en pulgadas.
    std::string excellonUnits{"INCH"};      ///< Unidades por defecto del archivo: `"INCH"` o `"METRIC"`.
    int circleSteps{64};                    ///< Segmentos por circunferencia completa.
    Units units{Units::Millimeters};        ///< Unidades de la aplicación (`units`).
    double toollessDiameter{1.0};           ///< Diámetro inventado cuando el archivo no trae diámetros.
};

/**
 * @brief Lector de archivos Excellon: herramientas, agujeros y ranuras.
 *
 * Soporta cabecera (`M48`, `METRIC`/`INCH`, `FMAT`, definiciones `TnCd`), coordenadas con y sin punto decimal,
 * repeticiones (`R`), ranuras con `G85` o con `G00`/`G01`, archivos sin cabecera y los comentarios de
 * Allegro y Altium. Tras leer el archivo se debe llamar a @ref createGeometry (el programa original lo hace
 * en `new_object`); @ref parseFile lo hace automáticamente.
 *
 * ### Diferencias respecto de Python
 * - La herramienta activa antes de cualquier `Tn` se guarda en la herramienta 0 (Python usaba la clave `""`).
 * - @ref rotate sin punto no está disponible: en el original rotaba cada agujero sobre sí mismo (no hacía nada).
 * - La geometría por herramienta se guarda además en @ref Geometry::tools para uniformidad con el resto.
 *
 * @code
 * flatcam::ExcellonOptions opt;
 * opt.zeros = "T"; opt.excellonUnits = "METRIC";
 * flatcam::Excellon e(opt);
 * e.parseFile("taladros.drl");
 * int n = e.drillTools().at(1).drills.size();
 * @endcode
 */
class Excellon : public Geometry {
public:
    /**
     * @brief Crea un lector vacío.
     * @param options Opciones de lectura.
     */
    explicit Excellon(const ExcellonOptions& options = ExcellonOptions());

    /** @brief Destructor. */
    ~Excellon() override;

    /**
     * @brief Lee un archivo Excellon y crea su geometría (`parse_file` + `create_geometry`).
     * @param path Ruta del archivo.
     * @return `true` si se leyó sin errores.
     */
    bool parseFile(const std::string& path);

    /**
     * @brief Interpreta un Excellon ya dividido en líneas (`parse_lines`).
     * @param lines Líneas del archivo.
     * @return `true` si se leyó sin errores (ver @ref lastError).
     */
    bool parseLines(const std::vector<std::string>& lines);

    /**
     * @brief Agrega una herramienta con sus agujeros sin leer ningún archivo (como hace la herramienta "PCB a 2 caras").
     * @param id Número de la herramienta.
     * @param diameter Diámetro de la herramienta.
     * @param drills Posiciones de los agujeros, en el orden en que se taladran si no se optimiza.
     * @param slots Ranuras (punto inicial y final); por ejemplo, al restaurar un proyecto.
     * Después hay que llamar a @ref createGeometry.
     */
    void addDrillTool(int id, double diameter, const std::vector<Point2D>& drills,
                      const std::vector<std::pair<Point2D, Point2D>>& slots = {});

    /**
     * @brief Crea la geometría de cada agujero y ranura (`create_geometry`).
     * @return `true` si se creó correctamente.
     */
    bool createGeometry();

    /**
     * @brief Interpreta una coordenada sin punto decimal (`parse_number`).
     * @param text Texto numérico, con signo opcional.
     * @return El valor, o `std::nullopt` si no se pudo interpretar.
     */
    std::optional<double> parseNumber(const std::string& text) const;

    /** @brief Herramientas de taladrado por número. @return El mapa de herramientas. */
    const std::map<int, ExcellonTool>& drillTools() const { return drillTools_; }

    /**
     * @brief Una copia con las herramientas renumeradas 1, 2, 3... en el orden dado (lo que hace `build_tool_ui` de
     * Perforación con `excellon_tools` antes de generar el G-code: el número `T` es la posición en la tabla).
     * @param order Números de las herramientas, en el orden nuevo (las que no están se descartan).
     * @return La copia.
     */
    Excellon renumbered(const std::vector<int>& order) const;

    /**
     * @brief Los números de las herramientas en el orden en que aparecen en el archivo (el del diccionario `tools` del
     * original; el mapa de @ref drillTools va por número).
     * @return Los números.
     */
    std::vector<int> toolIdsInOrder() const;

    /**
     * @brief Agrega agujeros al final de una herramienta (las ranuras que Perforación convierte en agujeros).
     * @param id Número de la herramienta (tiene que existir). @param drills Posiciones.
     */
    void appendDrills(int id, const std::vector<Point2D>& drills);
    /** @brief Cambia el diámetro de una herramienta. @param id Número. @param dia Diámetro. */
    void setToolDiameter(int id, double dia);

    /** @brief Supresión de ceros leída (`"L"`, `"T"` o vacío). @return El valor. */
    const std::string& zeros() const { return zeros_; }
    /** @brief Cambia la supresión de ceros (`zeros`, por ejemplo al panelizar). @param z El valor. */
    void setZeros(std::string z) { zeros_ = std::move(z); }
    /** @brief Unidades que declaró el archivo, si las declaró. @return Las unidades. */
    const std::optional<Units>& unitsFound() const { return unitsFound_; }
    /** @brief Dígitos enteros del formato en milímetros. @return La cantidad. */
    int formatUpperMm() const { return formatUpperMm_; }
    /** @brief Dígitos decimales del formato en milímetros. @return La cantidad. */
    int formatLowerMm() const { return formatLowerMm_; }
    /** @brief Dígitos enteros del formato en pulgadas. @return La cantidad. */
    int formatUpperIn() const { return formatUpperIn_; }
    /** @brief Dígitos decimales del formato en pulgadas. @return La cantidad. */
    int formatLowerIn() const { return formatLowerIn_; }
    /** @brief Texto fuente leído (`source_file`). @return El texto. */
    const std::string& sourceFile() const { return sourceFile_; }
    /** @brief Reemplaza el texto fuente (`source_file`, por ejemplo después de moverlo). @param text El texto. */
    void setSourceFile(std::string text) { sourceFile_ = std::move(text); }
    /** @brief Mensaje del último error de lectura. @return El mensaje, vacío si no hubo. */
    const std::string& lastError() const { return lastError_; }

    /** @brief Convierte unidades, escalando agujeros y diámetros (`convert_units`). @param objUnits Unidades de destino. @return Factor aplicado. */
    double convertUnits(Units objUnits) override;
    /** @brief Escala agujeros y ranuras (`scale`). @param xfactor Factor en X. @param yfactor Factor en Y. @param point Punto fijo (origen por defecto). */
    void scale(double xfactor, double yfactor, const std::optional<Point2D>& point = std::nullopt) override;
    /** @brief Desplaza agujeros y ranuras (`offset`). @param vect Vector de desplazamiento. */
    void offset(const Point2D& vect) override;
    /** @brief Refleja agujeros y ranuras (`mirror`). @param axis Eje. @param point Punto por el que pasa el eje. */
    void mirror(Axis axis, const Point2D& point) override;
    /** @brief Rota agujeros y ranuras (`rotate`). @param angle Ángulo en grados. @param point Centro de giro. */
    void rotate(double angle, const Point2D& point) override;
    /** @brief Inclina agujeros y ranuras (`skew`). @param angleX Ángulo en X. @param angleY Ángulo en Y. @param point Punto fijo. */
    void skew(double angleX, double angleY, const Point2D& point) override;
    /**
     * @brief Engrosa los agujeros (`Excellon.buffer`): suma @p distance al diámetro de cada herramienta (o lo multiplica si
     * @p factor) y rehace la geometría.
     * @param distance Distancia, o factor si @p factor.
     * @param join No se usa (el original solo lo pasa).
     * @param factor Si @p distance es un factor.
     */
    void buffer(double distance, JoinStyle join, bool factor) override;

private:
    /** @brief Aplica una transformación a todos los agujeros y ranuras y recrea la geometría. */
    void transformAll(const AffineTransform& t);

    ExcellonOptions options_;
    std::map<int, ExcellonTool> drillTools_;
    std::vector<int> definitionOrder_;  ///< Orden en que aparecieron las herramientas.
    ExcellonTool& toolRef(int id);
    std::string zeros_;
    std::optional<Units> unitsFound_;
    std::string excellonUnits_;
    int formatUpperIn_{2}, formatLowerIn_{4}, formatUpperMm_{3}, formatLowerMm_{3};
    std::string sourceFile_;
    std::string lastError_;
    bool routingFlag_{true};
    bool matchRoutingStart_{false}, matchRoutingStop_{false};
    bool diameterless_{false};
};

}  // namespace flatcam
