/**
 * @file GeoEditor.h
 * @brief El editor de Geometría (port de `appEditors/AppGeoEditor.py`) sobre `GeoEditorModel`: las
 * herramientas de dibujo con su vista previa, la elección con el mouse, los atajos y los paneles de Pintar,
 * Transformar y Texto.
 */
#pragma once

#include <memory>
#include <optional>

#include <QObject>
#include <QPointer>
#include <QPointF>
#include <algorithm>

#include "editors/GeoEditorModel.h"
#include "parsers/FontParser.h"
#include "gui/generated/GeoEditorUIBase.h"
#include "gui/generated/GeoPaintToolUIBase.h"
#include "gui/generated/GeoTextToolUIBase.h"
#include "gui/generated/GeoTransformToolUIBase.h"

class QAction;

namespace flatcam::gui {

class App;
class GeometryObject;
class GeoEditor;

/** @brief `PaintOptionsTool`: el panel de Pintar del editor. */
class GeoPaintTool : public GeoPaintToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param ed El editor. @param parent Padre. */
    GeoPaintTool(GeoEditor* ed, QWidget* parent = nullptr);
    /** @brief `set_tool_ui`: los valores de `tools_paint_*`. */
    void setToolUi();
    /** @brief `on_paint`. */
    void onPaint();

private:
    GeoEditor* ed_;
};

/** @brief `TransformEditorTool`: el panel de Transformaciones del editor. */
class GeoTransformTool : public GeoTransformToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param ed El editor. @param parent Padre. */
    GeoTransformTool(GeoEditor* ed, QWidget* parent = nullptr);
    /** @brief `set_tool_ui`: los valores de `tools_transform_*`. */
    void setToolUi();
    /** @brief `on_calculate_reference`. @param ref Índice de la referencia (vacío: la del combo). @return El punto. */
    std::optional<Point2D> reference(std::optional<int> ref = std::nullopt);
    /** @brief `on_rotate`. @param val Ángulo (vacío: el del campo). @param ref Referencia. */
    void onRotate(std::optional<double> val = std::nullopt, std::optional<int> ref = std::nullopt);
    /** @brief `on_flipx` (refleja en Y). */
    void onFlipX();
    /** @brief `on_flipy` (refleja en X). */
    void onFlipY();
    /** @brief `on_skewx`. @param val Ángulo. @param ref Referencia. */
    void onSkewX(std::optional<double> val = std::nullopt, std::optional<int> ref = std::nullopt);
    /** @brief `on_skewy`: inclina en Y. @param val El ángulo (o el del campo). @param ref La referencia (o la del combo). */
    void onSkewY(std::optional<double> val = std::nullopt, std::optional<int> ref = std::nullopt);
    /** @brief `on_scalex`. */
    void onScaleX();
    /** @brief `on_scaley`. */
    void onScaleY();
    /** @brief `on_offx`. @param val Distancia (vacío: la del campo). */
    void onOffX(std::optional<double> val = std::nullopt);
    /** @brief `on_offy`: desplaza en Y. @param val La distancia (o la del campo). */
    void onOffY(std::optional<double> val = std::nullopt);
    /** @brief `on_buffer_by_distance`. */
    void onBufferByDistance();
    /** @brief `on_buffer_by_factor`. */
    void onBufferByFactor();
    /** @brief `on_rotate_key` (Espacio): pide el ángulo y gira alrededor de la selección. */
    void onRotateKey();
    /** @brief `on_offx_key` (Alt+X). */
    void onOffXKey();
    /** @brief `on_offy_key` (Alt+Y): como el original, desplaza en X. */
    void onOffYKey();
    /** @brief `on_skewx_key` (Shift+X). */
    void onSkewXKey();
    /** @brief `on_skewy_key` (Shift+Y): como el original, inclina en X. */
    void onSkewYKey();

private:
    void done(const QString& msg);
    GeoEditor* ed_;
};

/** @brief `TextInputTool`: el texto que se agrega con la herramienta Texto. */
class GeoTextTool : public GeoTextToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param ed El editor. @param parent Padre. */
    GeoTextTool(GeoEditor* ed, QWidget* parent = nullptr);
    /** @brief `on_apply_button`: arma el texto con la fuente elegida (`ParseFont.font_to_geometry`). */
    void onApply();
    /** @brief `text_path`: el texto armado (vacío si no hay). */
    ShapeList textPath;
    /**
     * @brief Si el último "Aplicar" no encontró la fuente: en el original `text_path` queda con el texto
     * "flatcam font parse failed" y el clic avisa del error.
     */
    bool textFailed{false};
    bool fontBold{false};    ///< `font_bold`.
    bool fontItalic{false};  ///< `font_italic`.
    QString fontName;        ///< `font_name`: la familia elegida en el combo.

    /**
     * @brief Las fuentes instaladas por tipo (`ParseFont.get_fonts_by_types`), leídas una sola vez (el original las
     * lee cada vez que se elige la herramienta).
     * @return El lector de fuentes.
     */
    static FontParser& fonts();
    /**
     * @brief Usa estas carpetas en vez de las del sistema y vuelve a leer las fuentes (para las pruebas).
     * @param dirs Carpetas.
     */
    static void setFontDirectories(const std::vector<std::string>& dirs);

private:
    GeoEditor* ed_;
};

class FCDoubleSpinner;
class FCComboBox;
class FCButton;

/**
 * @brief `BufferSelectionTool`: el panel de la herramienta Buffer del editor (distancia, esquinas y los tres botones).
 *
 * En el original no se puede abrir (`'%S:'` no es un formato válido, sección 14 de AGENTS.md); con los errores
 * corregidos se arma a mano, con los mismos campos.
 */
class GeoBufferTool : public QWidget {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param ed El editor. @param parent Padre. */
    explicit GeoBufferTool(GeoEditor* ed, QWidget* parent = nullptr);
    /** @brief Hace el engrosado pedido (`on_buffer`, `on_buffer_int`, `on_buffer_ext`). @param kind Cuál. */
    void apply(GeoEditorModel::BufferKind kind);
    FCDoubleSpinner* buffer_distance_entry;  ///< Distancia.
    FCComboBox* buffer_corner_cb;            ///< Redondo, Cuadrado, Biselado.
    FCButton* buffer_int_button;             ///< "Buffer interior".
    FCButton* buffer_ext_button;             ///< "Buffer exterior".
    FCButton* buffer_button;                 ///< "Buffer completo".

private:
    GeoEditor* ed_;
};

/**
 * @brief `AppGeoEditor`: edita la geometría de un objeto Geometría (una herramienta de un multigeo).
 *
 * Mientras está activo, los clics del canvas van al editor (`connect_canvas_event_handlers`) y las teclas, a
 * @ref handleKey (la rama `geo_editor` de `MainGUI.keyPressEvent`).
 */
class GeoEditor : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param app La aplicación. */
    explicit GeoEditor(App* app);
    ~GeoEditor() override;

    /** @brief El panel del editor (`geo_edit_widget`), que va en la pestaña Propiedades. @return El panel. */
    GeoEditorUIBase* panel() const { return panel_; }
    /** @brief El panel de Pintar. @return El panel. */
    GeoPaintTool* paintTool() const { return paintTool_; }
    /** @brief El panel de Transformaciones. @return El panel. */
    GeoTransformTool* transformTool() const { return transformTool_; }
    /** @brief El panel de Texto. @return El panel. */
    GeoTextTool* textTool() const { return textTool_; }
    /** @brief El panel de Buffer (con los errores del original corregidos). @return El panel. */
    GeoBufferTool* bufferTool() const { return bufferTool_; }
    /** @brief La distancia del canvas que equivale a unos píxeles del clic (para elegir figuras). @return mm. */
    double clickTolerance() const;
    /** @brief El modelo (figuras y elección). @return El modelo. */
    GeoEditorModel& model() { return model_; }

    /** @brief Si el editor está activo. @return `true` si lo está. */
    bool active() const { return active_; }

    /**
     * @brief `edit_fcgeometry`: abre la geometría de una herramienta del objeto en el editor.
     * @param obj El objeto. @param tool La herramienta (`multigeo_tool`).
     */
    void edit(GeometryObject* obj, int tool);

    /** @brief `update_fcgeometry`: devuelve lo editado a la herramienta y al objeto, y cierra. @param obj El objeto. */
    void save(GeometryObject* obj);

    /** @brief `deactivate`: cierra el editor sin guardar. */
    void deactivate();

    /** @brief `select_tool`: aprieta el botón de la herramienta y la activa. @param name Nombre (`circle`, ...). */
    void selectTool(const QString& name);

    /** @brief El nombre de la herramienta activa. @return El nombre. */
    QString activeToolName() const;

    /**
     * @brief La rama `geo_editor` de `MainGUI.keyPressEvent`.
     * @param key Tecla. @param modifiers Modificadores. @return `true` si la tecla se usó.
     */
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);

    /** @brief `snap`: ajusta a la esquina más cercana (K) y a la grilla. @param p Punto. @return El punto. */
    Point2D snap(const Point2D& p) const;

    /** @name Eventos del canvas (públicos para los tests) */
    ///@{
    /** @brief `on_canvas_click`. @param world Posición. @param button Botón. */
    void onPress(const QPointF& world, Qt::MouseButton button);
    /** @brief `on_canvas_move`. @param world Posición. @param leftDragging Arrastrando con el izquierdo. */
    void onMove(const QPointF& world, bool leftDragging);
    /** @brief `on_geo_click_release`. @param world Posición. @param button Botón. @param dragged Si hubo arrastre. */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    ///@}

    /** @brief Las operaciones de la barra (`union`, `intersection`, `subtract`, `cutpath`) y borrar. */
    void unite();
    /** @brief `intersection`. */
    void intersection();
    /** @brief `subtract`. */
    void subtract();
    /** @brief `cutpath`. */
    void cutpath();
    /** @brief `on_delete_btn`. */
    void deleteSelected();

    /** @brief `replot` / `plot_all`: dibuja las figuras (las elegidas en otro color). */
    void replot();
    /** @brief `build_ui`: la lista de figuras del panel. */
    void buildUi();

    /** @brief `paint_tooldia`: el diámetro con el que se pintó por última vez (vacío si no se pintó). */
    std::optional<double> paintTooldia;

    App* app() const { return app_; }  ///< @brief La aplicación. @return La aplicación.

private:
    struct Tool;
    void onToolSelect(const QString& name);
    void newTool(const QString& name);
    void onShapeComplete();
    void updateUtility(const Point2D& data);
    void drawUtility(const ShapeList& geo);
    void deleteUtility();
    void onMoveClick();
    void onCopyClick();
    void syncTreeSelection();
    void cleanUpActive();

    App* app_;
    QPointer<GeoEditorUIBase> panel_;
    QPointer<GeoPaintTool> paintTool_;
    QPointer<GeoTransformTool> transformTool_;
    QPointer<GeoTextTool> textTool_;
    QPointer<GeoBufferTool> bufferTool_;
    GeoEditorModel model_;
    std::unique_ptr<Tool> tool_;
    std::map<QString, QAction*> buttons_;
    bool active_{false};
    bool inAction_{false};
    bool toolbarOldState_{false};
    std::optional<bool> selectionType_;  ///< `app.selection_type`.
    Point2D pos_{0, 0};                  ///< `self.pos`: el último clic.
    Point2D mouse_{0, 0};                ///< `snap_x`, `snap_y`.
    bool cornerSnap_{false};             ///< `options['corner_snap']`.
    int tool_id_{0};                     ///< `multigeo_tool`.
};

}  // namespace flatcam::gui
