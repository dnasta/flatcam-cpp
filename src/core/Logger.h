/**
 * @file Logger.h
 * @brief Registro de mensajes mínimo, equivalente al módulo `logging` que usa FlatCAM.
 */
#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <utility>

namespace flatcam {

/** @brief Severidad de un mensaje de registro. */
enum class LogLevel {
    Debug,    ///< Detalle para depuración.
    Info,     ///< Información general.
    Warning,  ///< Situación anómala que no impide continuar.
    Error     ///< Fallo de una operación.
};

/**
 * @brief Registro global con destino reemplazable.
 *
 * Por defecto escribe en `std::cerr` los mensajes de nivel Warning o superior. La interfaz gráfica
 * podrá instalar su propio destino con @ref setSink para mostrarlos en la consola de la aplicación.
 */
class Logger {
public:
    /** @brief Función que recibe cada mensaje. */
    using Sink = std::function<void(LogLevel, const std::string&)>;

    /**
     * @brief Reemplaza el destino de los mensajes.
     * @param sink Nuevo destino; si está vacío se restablece el destino por defecto.
     */
    static void setSink(Sink sink) { instance() = std::move(sink); }

    /**
     * @brief Registra un mensaje.
     * @param level Severidad.
     * @param message Texto del mensaje.
     */
    static void log(LogLevel level, const std::string& message) {
        if (instance()) {
            instance()(level, message);
        } else if (level >= LogLevel::Warning) {
            std::cerr << (level == LogLevel::Error ? "[ERROR] " : "[WARN] ") << message << '\n';
        }
    }

    /** @brief Registra un mensaje de depuración. @param m Texto. */
    static void debug(const std::string& m) { log(LogLevel::Debug, m); }
    /** @brief Registra un mensaje informativo. @param m Texto. */
    static void info(const std::string& m) { log(LogLevel::Info, m); }
    /** @brief Registra una advertencia. @param m Texto. */
    static void warning(const std::string& m) { log(LogLevel::Warning, m); }
    /** @brief Registra un error. @param m Texto. */
    static void error(const std::string& m) { log(LogLevel::Error, m); }

private:
    /** @brief Destino actual (almacenado como variable estática de la función). */
    static Sink& instance() {
        static Sink sink;
        return sink;
    }
};

}  // namespace flatcam
