/**
 * @file QRCodeTool.h
 * @brief Herramienta Código QR (port de `appTools/ToolQRCode.py`).
 */
#pragma once

#include <optional>

#include <QMetaObject>
#include <QPointF>

#include "gui/generated/QRCodeToolUIBase.h"
#include "gui/tools/ToolInfo.h"
#include "tools/QrCode.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica del código QR (Alt+Q): arma el código con la biblioteca `qrcode` portada, lo muestra siguiendo
 * al mouse y con un clic lo inserta en el Gerber elegido (que se modifica, como el original); también exporta el código
 * a PNG y a SVG.
 */
class QRCodeTool : public QRCodeToolUIBase {
    Q_OBJECT
public:
    /** @brief Capa auxiliar de la vista previa. */
    static constexpr int kLayerId = -6;
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit QRCodeTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Generar código QR" (`execute`): arma la geometría y espera el clic. @return `false` si no se pudo. */
    bool execute();
    /** @brief Un movimiento del mouse (`on_mouse_move`): dibuja el código ahí. @param world Posición (ajustada). */
    void onMove(const QPointF& world);
    /** @brief Un clic (`on_mouse_release`): con el izquierdo inserta el código. @param world Posición. @param b Botón. */
    void onRelease(const QPointF& world, Qt::MouseButton b);
    /** @brief Exporta a PNG (`job_thread_qr_png`). @param path Archivo. @return `true` si se guardó. */
    bool exportPng(const QString& path);
    /** @brief Exporta a SVG (`job_thread_qr_svg`). @param path Archivo. @return `true` si se guardó. */
    bool exportSvg(const QString& path);
    /** @brief Si espera el clic. @return `true` si lo espera. */
    bool isPlacing() const { return placing_; }

private:
    /** @brief El código del panel. @return La matriz, o vacío si no se pudo (con el mensaje). */
    std::optional<QrMatrix> makeMatrix();
    /** @brief `make(pos)`: inserta el código en el Gerber. @param pos Posición. */
    void make(const QPointF& pos);
    /** @brief Deja de seguir al mouse (`on_exit`). */
    void onExit();
    /** @brief Fondo transparente (`on_transparent_back_color`). @param on Marcado. */
    void onTransparent(bool on);

    App* app_;
    FlatCAMObj* grb_{nullptr};
    std::optional<QrGeometry> geo_;
    bool placing_{false};
    QString oldBackColor_;
    QMetaObject::Connection moveConn_, releaseConn_;
};

}  // namespace flatcam::gui
