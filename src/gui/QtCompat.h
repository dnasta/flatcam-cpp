/**
 * @file QtCompat.h
 * @brief Compatibilidad con versiones de Qt 6 anteriores a la 6.7 (por ejemplo la 6.4 de Ubuntu 24.04).
 */
#ifndef FLATCAM_GUI_QTCOMPAT_H
#define FLATCAM_GUI_QTCOMPAT_H

#include <QCheckBox>
#include <QtGlobal>

namespace flatcam::gui {

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
/** @brief Tipo del argumento de la señal de cambio de estado de una casilla (`Qt::CheckState`). */
using CheckStateArg = Qt::CheckState;
/** @brief La señal de cambio de estado de una casilla (`checkStateChanged`, desde Qt 6.7). */
#define FC_CHECK_STATE_CHANGED &QCheckBox::checkStateChanged
#else
/** @brief Tipo del argumento de la señal de cambio de estado de una casilla (`int` antes de Qt 6.7). */
using CheckStateArg = int;
/** @brief La señal de cambio de estado de una casilla (`stateChanged` antes de Qt 6.7). */
#define FC_CHECK_STATE_CHANGED &QCheckBox::stateChanged
#endif

}  // namespace flatcam::gui

#endif
