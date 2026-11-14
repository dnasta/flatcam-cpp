/**
 * @file TclCommand.h
 * @brief Los comandos de la consola Tcl (port de `tclCommands/TclCommand.py`): la clase base con los datos de cada
 * comando (alias, descripción, argumentos y opciones con su tipo de Python, obligatorios y ayuda, extraídos del
 * original en `resources/tcl_commands.json`), el análisis de argumentos (`parse_arguments`, `check_args`), la ayuda
 * (`get_decorated_help`) y el registro (`register_all_commands`).
 */
#pragma once

#include <QString>
#include <QStringList>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace flatcam::gui {

class App;

/**
 * @brief El error de un comando (`TclErrorException` y los demás errores de Python): el texto pasa a Tcl como error.
 */
class TclError : public std::runtime_error {
public:
    /** @brief Crea el error. @param text Mensaje. */
    explicit TclError(const QString& text) : std::runtime_error(text.toStdString()), text_(text) {}
    /** @brief El mensaje. @return Texto. */
    const QString& text() const { return text_; }

private:
    QString text_;
};

/** @brief Un valor de argumento ya convertido: `None`, texto (`str`), `float` o `int`. */
using TclValue = std::variant<std::monostate, QString, double, long long>;

/** @brief Los argumentos de un comando después de `check_args`. */
struct TclArgs {
    std::map<std::string, TclValue> named;  ///< Los con nombre y las opciones (`args`).
    QStringList unnamed;                     ///< Los demás (`unnamed_args`).
    /** @brief Si está la clave (`key in args`). @param key Clave. @return `true` si está. */
    bool has(const std::string& key) const { return named.count(key) > 0; }
    /** @brief Si está y no es `None`. @param key Clave. @return `true` si tiene valor. */
    bool hasValue(const std::string& key) const;
    /** @brief El texto (`str(args[key])`). @param key Clave. @return El texto. */
    QString str(const std::string& key) const;
    /** @brief El número (`float` o `int`). @param key Clave. @return El valor. */
    double num(const std::string& key) const;
};

/** @brief Los datos de un comando del original (`tcl_commands.json`). */
struct TclCommandMeta {
    QString className;                                    ///< Clase de Python (`TclCommandGetSys`).
    bool signaled{false};                                 ///< Deriva de `TclCommandSignaled` (acepta `-timeout`).
    bool registered{true};  ///< Lo importa `tclCommands/__init__.py` (si no, el original no lo registra: `import_svg`).
    QStringList aliases;                                  ///< Nombres del comando.
    QString description;                                  ///< Descripción de `help`.
    std::vector<std::pair<QString, QString>> argNames;    ///< Argumentos por posición: nombre y tipo.
    std::vector<std::pair<QString, QString>> optionTypes; ///< Opciones `-nombre valor`: nombre y tipo.
    QStringList required;                                 ///< Obligatorios.
    QString helpMain;                                     ///< `help['main']`.
    std::vector<std::pair<QString, QString>> helpArgs;    ///< `help['args']`, en orden.
    QStringList examples;                                 ///< `help['examples']`.
    /** @brief Los datos de una clase. @param className Clase. @return Los datos, o vacío si no está. */
    static std::optional<TclCommandMeta> forClass(const QString& className);
    /** @brief Todos los comandos del original, en el orden del archivo. @return Los datos. */
    static const std::vector<TclCommandMeta>& all();
    /** @brief El orden de las claves de `defaults.py` (el de `app.defaults`). @return Las claves. */
    static const QStringList& defaultsOrder();
};

/** @brief Un comando de la consola (`TclCommand`). */
class TclCommand {
public:
    /** @brief Crea el comando con los datos de su clase del original. @param app App. @param className Clase. */
    TclCommand(App* app, const QString& className);
    virtual ~TclCommand() = default;
    /** @brief Los datos del comando. @return Los datos. */
    const TclCommandMeta& meta() const { return meta_; }
    /** @brief `get_decorated_help`: la ayuda que muestra `help <comando>`. @return El texto. */
    QString decoratedHelp() const;
    /**
     * @brief `parse_arguments`: separa los `-opción valor` de los argumentos sueltos.
     * @param args Argumentos de Tcl. @param[out] options Opciones (una sin valor queda vacía).
     * @return Los argumentos sueltos.
     */
    static QStringList parseArguments(const QStringList& args, std::vector<std::pair<QString, std::optional<QString>>>& options);
    /** @brief `check_args`: convierte cada argumento a su tipo y comprueba los obligatorios. @param args Argumentos. @return Los argumentos. */
    TclArgs checkArgs(const QStringList& args) const;
    /**
     * @brief `execute_wrapper` + `execute`: corre el comando.
     * @param args Argumentos de Tcl.
     * @return El resultado para Tcl (`None` es vacío: se muestra "None"); un error lanza @ref TclError.
     */
    std::optional<QString> run(const QStringList& args);

protected:
    /** @brief `execute`. @param args Argumentos ya convertidos. @return El resultado, o vacío (`None`). */
    virtual std::optional<QString> execute(const TclArgs& args) = 0;
    /** @brief `raise_tcl_error`. @param text Mensaje. */
    [[noreturn]] static void raiseTclError(const QString& text) { throw TclError(text); }
    App* app_;                 ///< La aplicación.
    QStringList originalArgs_; ///< `original_args`.

private:
    TclCommandMeta meta_;
};

/**
 * @brief `float(texto)` de Python.
 * @param text Texto. @param[out] error El mensaje de `ValueError` si no se puede.
 * @return El valor, o vacío.
 */
std::optional<double> pyFloatFromString(const QString& text, QString* error = nullptr);
/**
 * @brief `int(texto)` de Python.
 * @param text Texto. @param[out] error El mensaje de `ValueError` si no se puede.
 * @return El valor, o vacío.
 */
std::optional<long long> pyIntFromString(const QString& text, QString* error = nullptr);
/** @brief `repr()` de un texto de Python. @param s Texto. @return La representación. */
QString pyReprString(const QString& s);

/**
 * @brief `register_all_commands`: crea los comandos del port (los que ya están portados).
 * @param app La aplicación.
 * @return Los comandos.
 */
std::vector<std::unique_ptr<TclCommand>> createTclCommands(App* app);

}  // namespace flatcam::gui
