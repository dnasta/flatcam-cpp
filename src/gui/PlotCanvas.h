/**
 * @file PlotCanvas.h
 * @brief Área de dibujo con OpenGL (reemplaza a `VisPyCanvas` y `PlotCanvas` del original, que usan vispy).
 */
#pragma once

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QPointF>
#include <QRectF>
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace flatcam::gui {

/** @brief Figuras de un objeto con colores propios (otra herramienta, o los viajes engrosados de un CNC Job). */
struct PlotGroup {
    std::vector<float> fill;   ///< Triángulos de relleno: x, y por vértice.
    std::vector<float> lines;  ///< Segmentos del contorno: x, y por vértice.
    QColor fillColor;          ///< Color del relleno.
    QColor lineColor;          ///< Color del contorno.
};

/**
 * @brief Lo que se dibuja de un objeto: triángulos de relleno y segmentos de contorno, con sus colores.
 *
 * Equivale a las figuras que cada objeto agrega a la `ShapeCollection` de vispy (`add_shape` con `color` y `face_color`).
 */
struct PlotLayerData {
    std::vector<float> fill;   ///< Triángulos de relleno: x, y por vértice, de a tres vértices.
    std::vector<float> lines;  ///< Segmentos del contorno: x, y por vértice, de a dos vértices.
    QColor fillColor;          ///< Color del relleno (con transparencia).
    QColor lineColor;          ///< Color del contorno.
    std::vector<PlotGroup> groups;  ///< Grupos con sus propios colores, dibujados encima del relleno y el contorno.
    std::vector<float> travel; ///< Segmentos de viaje de un CNC Job (de a dos vértices), dibujados encima.
    QColor travelColor;        ///< Color de los viajes (`cncjob_travel_line`).
    QRectF bounds;             ///< Límites de lo dibujado, en mm.
};

/**
 * @brief Canvas 2D con la misma apariencia y manejo que el de FlatCAM 2024.4 (tema claro).
 *
 * Reproduce `VisPyCanvas`: un margen de 10 píxeles, el eje Y a la izquierda (55 px) y el eje X abajo (30 px) con sus
 * números, y la vista con borde negro y fondo blanco, grilla gris, las líneas rojas por el origen (`v_line`, `h_line`),
 * la cruz del cursor y el recuadro HUD con `Dx`, `Dy`, `X`, `Y`. La cámara (`Camera`, un `PanZoomCamera` de vispy)
 * mantiene la proporción 1:1, empieza mostrando el rectángulo (-25, -25, 150, 150), acerca y aleja con la rueda
 * (factor `1.007^30` por paso, alrededor del cursor) y se desplaza arrastrando con el botón derecho
 * (`global_pan_button` = 2).
 */
class PlotCanvas : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    /** @brief Crea el canvas. @param parent Widget padre. */
    explicit PlotCanvas(QWidget* parent = nullptr);

    /** @brief Muestra el rectángulo pedido entero, centrado (`zoom_fit` / `vispy_fit`). @param rect Rectángulo en mm. */
    void fitTo(const QRectF& rect);

    /** @brief Acerca (factor > 1) o aleja alrededor de un punto (`zoom`). @param factor Factor. @param center Centro en mm. */
    void zoom(double factor, const QPointF& center);

    /** @brief Muestra u oculta el recuadro HUD (`on_toggle_hud`). @param on Mostrar. */
    void setHudEnabled(bool on);

    /** @brief Muestra u oculta las líneas por el origen (`on_toggle_axis`). @param on Mostrar. */
    void setAxisEnabled(bool on);

    /** @brief Muestra u oculta la grilla (`on_toggle_grid_lines`). @param on Mostrar. */
    void setGridEnabled(bool on);

    /** @brief Si el recuadro HUD está visible. @return El estado. */
    bool hudEnabled() const { return hudEnabled_; }

    /** @brief Si las líneas por el origen están visibles. @return El estado. */
    bool axisEnabled() const { return axisEnabled_; }

    /**
     * @brief Pasa de píxeles del widget a coordenadas de la placa (`translate_coords`).
     * @param pixel Posición en el widget.
     * @return Posición en mm.
     */
    QPointF toWorld(const QPointF& pixel) const;

    /**
     * @brief Pasa de coordenadas de la placa a píxeles del widget (`translate_coords_2`).
     * @param world Posición en mm.
     * @return Posición en el widget.
     */
    QPointF toPixel(const QPointF& world) const;

    /** @brief Rectángulo de la vista (sin márgenes ni ejes), en píxeles del widget. @return El rectángulo. */
    QRectF viewRect() const;

    /**
     * @brief Agrega o reemplaza lo que se dibuja de un objeto.
     * @param id Identificador del objeto (lo elige quien llama).
     * @param data Triángulos, segmentos y colores.
     */
    void setLayer(int id, PlotLayerData data);

    /** @brief Muestra u oculta lo dibujado de un objeto. @param id Identificador. @param visible Mostrar. */
    void setLayerVisible(int id, bool visible);

    /** @brief Borra lo dibujado de un objeto. @param id Identificador. */
    void removeLayer(int id);

    /**
     * @brief Fija el orden de dibujo (cada capa tapa a las anteriores); las capas que no estén en la lista se dibujan
     * antes, por identificador.
     * @param idsBottomFirst Identificadores desde el de más abajo.
     */
    void setLayerOrder(std::vector<int> idsBottomFirst);

    /** @brief Orden de dibujo vigente. @return Identificadores desde el de más abajo. */
    const std::vector<int>& layerOrder() const { return order_; }

    /** @brief Límites de todo lo visible, o vacío si no hay nada. @return El rectángulo en mm. */
    std::optional<QRectF> layersBounds() const;

    /**
     * @brief Encuadra todo lo visible con un 2 % de margen por lado (`fit_view`); sin objetos muestra (-1, -1, 20, 20).
     */
    void fitView();

Q_SIGNALS:
    /** @brief El cursor se movió sobre la vista. @param world Posición en mm. */
    void mouseMoved(QPointF world);
    /** @brief Clic en la vista. @param world Posición en mm. @param button Botón. */
    void mouseClicked(QPointF world, Qt::MouseButton button);

protected:
    /** @brief Prepara OpenGL. */
    void initializeGL() override;
    /** @brief Dibuja todo. */
    void paintGL() override;
    /** @brief Mantiene el centro al cambiar de tamaño. @param w Ancho. @param h Alto. */
    void resizeGL(int w, int h) override;
    /** @brief Zoom con la rueda. @param event Evento. */
    void wheelEvent(QWheelEvent* event) override;
    /** @brief Comienza un desplazamiento o registra un clic. @param event Evento. */
    void mousePressEvent(QMouseEvent* event) override;
    /** @brief Desplaza y actualiza la posición del cursor. @param event Evento. */
    void mouseMoveEvent(QMouseEvent* event) override;
    /** @brief Termina el desplazamiento. @param event Evento. */
    void mouseReleaseEvent(QMouseEvent* event) override;
    /** @brief Oculta la cruz del cursor al salir. @param event Evento. */
    void leaveEvent(QEvent* event) override;

private:
    /// Datos de un objeto ya cargados en la placa de video.
    struct Layer {
        PlotLayerData data;
        bool visible{true};
        bool dirty{true};
        std::unique_ptr<QOpenGLBuffer> fillVbo;
        std::unique_ptr<QOpenGLBuffer> lineVbo;
        std::unique_ptr<QOpenGLBuffer> travelVbo;
        std::vector<std::unique_ptr<QOpenGLBuffer>> groupVbos;  ///< Relleno y contorno de cada grupo, alternados.
    };
    void drawLayers(const QRectF& vr);
    void drawGrid(class QPainter& p, const QRectF& vr) const;
    void drawAxesLabels(class QPainter& p, const QRectF& vr) const;
    void drawHud(class QPainter& p, const QRectF& vr) const;
    double tickStep() const;

    QPointF center_{50.0, 50.0};  ///< Centro de la vista en mm.
    double scale_{1.0};           ///< Píxeles por mm.
    bool firstResize_{true};
    bool hudEnabled_{true};       // global_hud
    bool axisEnabled_{true};      // global_axis
    bool gridEnabled_{true};      // global_grid_lines
    bool panning_{false};
    QPointF lastPan_;
    bool cursorInside_{false};
    QPointF cursorWorld_{0, 0};
    QPointF lastClick_{0, 0};
    std::map<int, Layer> layers_;
    std::vector<int> order_;  ///< Orden de dibujo, desde abajo.
    std::unique_ptr<QOpenGLShaderProgram> program_;
};

}  // namespace flatcam::gui
