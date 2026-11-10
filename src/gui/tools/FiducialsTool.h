/**
 * @file FiducialsTool.h
 * @brief Herramienta Fiduciales (port de `appTools/ToolFiducials.py`).
 */
#pragma once

#include <set>
#include <vector>

#include <QMetaObject>
#include <QPointF>

#include "core/Types.h"
#include "gui/generated/FiducialsToolUIBase.h"
#include "gui/tools/ToolInfo.h"

class QTableWidgetItem;

namespace flatcam::gui {

class App;
class FCEntry;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Fiduciales (Alt+F): agrega al cobre fiduciales circulares, en cruz o de tablero en las
 * esquinas de la caja (modo automático) o donde se hace clic (modo manual), en un Gerber `<nombre>_fid`, y las aberturas
 * de la máscara (del doble del tamaño) en los mismos puntos.
 */
class FiducialsTool : public FiducialsToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit FiducialsTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Agregar fiducial" (`add_fiducials`): en automático crea el Gerber; en manual espera los clics. */
    void addFiducials();
    /** @brief "Agregar abertura de máscara" (`add_soldermask_opening`). @return El objeto nuevo, o `nullptr`. */
    FlatCAMObj* addSoldermaskOpening();
    /**
     * @brief Un clic en el canvas en el modo manual (`on_mouse_release`).
     * @param world Posición (ya ajustada a la grilla). @param button Botón.
     */
    void onRelease(const QPointF& world, Qt::MouseButton button);
    /** @brief Si espera clics (`handlers_connected`). @return `true` si los espera. */
    bool isPicking() const { return picking_; }
    /** @brief El último objeto creado. @return El objeto, o `nullptr`. */
    FlatCAMObj* lastCreated() const { return lastCreated_; }
    /** @brief Campo de las coordenadas abajo a la izquierda (`bottom_left_coords_entry`). */
    FCEntry* bottom_left_coords_entry{nullptr};
    /** @brief Campo de las coordenadas arriba a la derecha (`top_right_coords_entry`). */
    FCEntry* top_right_coords_entry{nullptr};
    /** @brief Campo de las coordenadas del segundo punto (`sec_points_coords_entry`). */
    FCEntry* sec_points_coords_entry{nullptr};

private:
    /** @brief Habilita el tercer punto (`on_second_point`). @param val `up`, `down` o `no`. */
    void onSecondPoint(const QString& val);
    /** @brief Cambio de modo (`on_method_change`). @param val `auto` o `manual`. */
    void onMethodChange(const QString& val);
    /** @brief Habilita el grosor de línea para la cruz (`on_fiducial_type`). @param val Forma. */
    void onFiducialType(const QString& val);
    /** @brief Avanza según la cantidad de puntos (`check_points`). */
    void checkPoints();
    /** @brief `add_fiducials_geo` sobre @p obj. @return El objeto nuevo, o `nullptr` si el original falla. */
    FlatCAMObj* addGeo(FlatCAMObj* obj, double size, const QString& type);
    /** @brief Termina (`on_exit`). */
    void onExit();
    /** @brief Conecta o desconecta los clics del canvas. @param on Conectar. */
    void setPicking(bool on);
    /** @brief Llena un combo con los Gerber, con el último elegido (`is_last`). @param combo El combo. */
    void fillCombo(FCComboBox* combo);

    App* app_;
    FlatCAMObj* grb_{nullptr};
    FlatCAMObj* lastCreated_{nullptr};
    std::vector<Point2D> points_;
    QString secPosition_;
    bool picking_{false};
    QTableWidgetItem* idItem3_{nullptr};
    QTableWidgetItem* secLabel_{nullptr};
    QMetaObject::Connection conn_;
};

}  // namespace flatcam::gui
