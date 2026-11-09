/**
 * @file Preferences.h
 * @brief Preferencias de la aplicación: port de `FlatCAMDefaults` (`defaults.py`) y de `PreferencesUIManager`
 * (`appGUI/preferences/PreferencesUIManager.py`), con la pestaña Preferencias generada desde la interfaz original.
 */
#pragma once

#include <functional>
#include <array>
#include <map>
#include <vector>
#include <QVariant>
#include <optional>
#include <string>

#include <QColor>
#include <QObject>
#include <QString>

#include "cam/PyNumber.h"
#include "gui/generated/PreferencesTabUIBase.h"
#include "io/JsonValue.h"

namespace flatcam::gui {

class MainGUI;

/**
 * @name Las opciones de la aplicación en curso (`self.app.defaults`)
 * Las herramientas y los objetos leen de acá sus valores (los `set_tool_ui` del original). Sin aplicación, las de
 * fábrica. Las conversiones son las de Python: `float(v)`, `int(v)`, `bool(v)`, `str(v)`.
 */
///@{
/** @brief Las opciones. @return Las de la última aplicación creada, o las de fábrica. */
const JsonValue& appDefaults();
/** @brief `float(defaults[key])`. @param key Clave. @param def Si no es un número. @return El valor. */
double defNum(const std::string& key, double def = 0.0);
/** @brief `int(defaults[key])`. @param key Clave. @param def Si no es un número. @return El valor. */
int defInt(const std::string& key, int def = 0);
/** @brief `bool(defaults[key])`. @param key Clave. @return El valor. */
bool defBool(const std::string& key);
/** @brief `str(defaults[key])`. @param key Clave. @return El texto. */
QString defStr(const std::string& key);
/** @brief El valor para un `RadioSet` (texto, número o booleano). @param key Clave. @return El valor. */
QVariant defVariant(const std::string& key);
/**
 * @brief Un par X,Y (`"0.0, 0.0"` en un texto o `[0.0, 0.0]` en una lista, como queda después de guardar).
 * @param key Clave. @return El par, o vacío (None, texto vacío o inválido).
 */
std::optional<std::array<double, 2>> defPair(const std::string& key);
/**
 * @brief Una lista de diámetros como la leen las herramientas (`tools_ncc_tools`...): un número, un texto con comas
 * (cada parte con `eval`) o una lista.
 * @param key Clave. @return Los diámetros (vacío si no se puede leer).
 */
std::vector<double> defDias(const std::string& key);
/**
 * @brief Un número con su tipo de Python (entero o real), para el G-code que lo imprime con `str()`.
 * @param key Clave. @return El número, o vacío si es None o no es un número.
 */
std::optional<PyNumber> defPy(const std::string& key);
/** @brief `eval()` de Python para números, tuplas y listas (los `NumericalEval*Entry`). @param text Texto. @return El valor, o nulo. */
JsonValue pyEvalLiteral(const QString& text);
///@}

/** @brief La pestaña Preferencias (`ui.preferences_tab` con su `pref_tab_area`), generada desde el original. */
class PreferencesTab : public PreferencesTabUIBase {
public:
    /** @brief Crea y arma la pestaña. @param parent Widget padre. */
    explicit PreferencesTab(QWidget* parent = nullptr);
};

/**
 * @brief Las opciones de la aplicación (`app.defaults`) y la pestaña Preferencias.
 *
 * Como el original: las opciones de fábrica (con `version` = "2024.4") se pisan con las de
 * `current_defaults.FlatConfig` de la carpeta de datos; un archivo de otra versión se descarta (la 2024.4 es beta y
 * vuelve a las de fábrica). Cada opción con un campo en la pestaña se lee y se escribe con el `get_value` /
 * `set_value` de su widget, conservando el tipo de Python del valor (entero, real, texto, booleano, tupla).
 *
 * Desvíos: lo que el original guarda con `QSettings` (tamaños de letra, tema, "machinist") va a `settings.ini` de la
 * carpeta de datos (así el port no pisa la configuración del FlatCAM en Python y los tests no tocan la del usuario);
 * el tema de fábrica del port es el claro ("white"), el único que dibuja.
 */
class Preferences : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el gestor (sin la pestaña, que se arma al abrirla). @param ui Ventana principal. @param parent Padre. */
    explicit Preferences(MainGUI* ui, QObject* parent = nullptr);
    /** @brief Deja de ser la fuente de @ref appDefaults. */
    ~Preferences() override;

    /** @brief La versión que se guarda en las preferencias (`App.version`). @return "2024.4". */
    static const char* version();
    /** @brief Las opciones de fábrica (`FlatCAMDefaults.factory_defaults`, con su versión). @return Las opciones. */
    static const JsonValue& factoryDefaults();

    /** @brief Las opciones vigentes (`app.defaults`). @return Las opciones. */
    const JsonValue& defaults() const { return defaults_; }
    /** @brief La copia para deshacer los cambios (`current_defaults`). @return Las opciones. */
    const JsonValue& currentDefaults() const { return currentDefaults_; }
    /** @brief Una opción. @param key Clave. @return El valor, o nulo. */
    const JsonValue& value(const std::string& key) const { return defaults_[key]; }
    /**
     * @brief Cambia una opción y, como el `LoudDict` del original (`on_defaults_dict_change`), su campo en la pestaña.
     * @param key Clave. @param v Valor.
     */
    void set(const std::string& key, JsonValue v);

    /**
     * @brief Prepara la carpeta de datos y carga las preferencias (lo del arranque de `App.__init__`): crea
     * `current_defaults.FlatConfig` vacío si no está, escribe `factory_defaults.FlatConfig` (de solo lectura) si falta
     * o es de otra versión, y carga el primero. También fija `global_theme` con el tema guardado.
     * @param dataPath La carpeta de datos.
     */
    void setup(const QString& dataPath);
    /** @brief Carpeta de datos. @return La ruta. */
    const QString& dataPath() const { return dataPath_; }
    /**
     * @brief `FlatCAMDefaults.load`: carga un archivo de preferencias.
     * @param filename Archivo. @return `true` si se pudo leer.
     */
    bool load(const QString& filename);
    /** @brief `FlatCAMDefaults.write`: guarda las opciones (JSON con sangría 2 y claves ordenadas). @param filename Archivo. @return Si se pudo. */
    bool write(const QString& filename) const;
    /** @brief `reset_to_factory_defaults`. */
    void resetToFactoryDefaults();
    /** @brief Si al cargar se encontraron preferencias de otra versión (`old_defaults_found`). @return `true` si sí. */
    bool oldDefaultsFound() const { return oldDefaultsFound_; }
    /**
     * @brief `save_factory_defaults`: escribe las de fábrica si el archivo falta, está vacío o es de otra versión.
     * @param filename Archivo.
     */
    static void saveFactoryDefaults(const QString& filename);
    /** @brief `theme()`: el tema guardado ("white" de fábrica en el port). @return El tema. */
    QString theme() const;
    /** @brief Los ajustes que el original guarda con `QSettings`. @return Ruta del `settings.ini`. */
    QString settingsFile() const;

    /** @brief La pestaña (se arma la primera vez, con los campos escritos desde las opciones). @return La pestaña. */
    PreferencesTab* tab();
    /** @brief Si la pestaña ya está armada. @return `true` si sí. */
    bool hasTab() const { return tab_ != nullptr; }
    /** @brief Las claves con campo en la pestaña (`defaults_form_fields`). @return Clave → nombre del miembro. */
    static const std::map<std::string, std::pair<std::string, std::string>>& fields();

    /** @brief `defaults_read_form`: lee todos los campos a las opciones. */
    void readForm();
    /** @brief `defaults_write_form`: escribe los campos desde las opciones. @param source Otra fuente, o `nullptr`. */
    void writeForm(const JsonValue* source = nullptr);
    /** @brief `defaults_write_form_field`. @param key Clave. @param source Otra fuente, o `nullptr`. */
    void writeFormField(const std::string& key, const JsonValue* source = nullptr);
    /** @brief El valor de un campo (`get_value`), con el tipo de Python. @param key Clave. @return El valor, o vacío. */
    std::optional<JsonValue> formValue(const std::string& key);

    /** @brief `on_preferences`: abre la pestaña Preferencias. */
    void open();
    /** @brief `on_toggle_preferences` (el rótulo de la barra de estado). */
    void toggle();
    /** @brief Si la pestaña está abierta en el área de dibujo. @return `true` si sí. */
    bool isOpen() const;
    /**
     * @brief `on_save_button`: "Guardar" (con @p saveToFile) o "Aplicar".
     *
     * Como el original, "Aplicar" no lee el formulario: solo copia las opciones a `current_defaults`.
     * @param saveToFile Guardar en el archivo y cerrar la pestaña.
     */
    void onSave(bool saveToFile);
    /** @brief `on_pref_close_button`: "Cerrar" sin guardar (vuelve a `current_defaults`). */
    void onCloseButton();
    /** @brief `on_restore_defaults_preferences`: "Restaurar valores predeterminados". */
    void onRestoreDefaults();
    /** @brief `on_preferences_open_folder`: "Abrir carpeta de preferencias". */
    void openFolder();
    /** @brief `on_gui_clear`: borrar los ajustes de la interfaz. @param forced Sin preguntar. */
    void clearGuiSettings(bool forced = false);
    /** @brief `save_defaults`: guarda `current_defaults.FlatConfig`. @param silent Sin mensaje. @return Si se pudo. */
    bool saveDefaults(bool silent = false);
    /** @brief Si hay cambios sin guardar (`preferences_changed_flag`). @return `true` si sí. */
    bool changed() const { return changedFlag_; }

    /** @brief En los tests: respuesta a las preguntas en vez de mostrar el cuadro. @param answer `true` = Sí. */
    void setAutoAnswer(std::optional<bool> answer) { autoAnswer_ = answer; }

public Q_SLOTS:
    /** @brief `on_preferences_edited`: marca la pestaña en rojo y avisa (una vez). */
    void onPreferencesEdited();

Q_SIGNALS:
    /** @brief Se guardaron o se cargaron opciones (para que la aplicación copie las del proyecto). */
    void saved();
    /** @brief Antes de guardar desde "Guardar": `on_options_app2project` (leer el formulario y copiar al proyecto). */
    void optionsToProject();

private:
    int tabIndex() const;
    void connectGroups();       ///< Las señales de cada grupo (`*PrefGroupUI`), sus colores y las asociaciones.
    void initColorPickers();    ///< `__init_color_pickers`.
    void resetAlphaControls();  ///< Los controles de alfa en 0, como al construirlos en el original.
    void connectEdited(bool on);
    void onTabClosed();
    bool ask(const QString& title, const QString& text, const QString& icon, const QString& yes, const QString& no,
             bool defaultYes);
    void saveToolbarView();

    MainGUI* ui_;
    PreferencesTab* tab_{nullptr};
    std::map<std::string, QObject*> members_;  ///< Los campos de la pestaña por nombre.
    QString dataPath_;
    JsonValue defaults_;
    JsonValue currentDefaults_;
    bool oldDefaultsFound_{false};
    bool changedFlag_{false};
    bool ignoreTabCloseEvent_{false};
    bool closingTab_{false};  ///< Guardando desde el aviso al cerrar la pestaña.
    bool edConnected_{false};
    QColor oldColor_;
    std::optional<bool> autoAnswer_;

public:
    /** @brief Lo que hace `on_app_restart` (la aplicación lo fija; los tests pueden reemplazarlo). */
    std::function<void()> onRestart;
};

}  // namespace flatcam::gui
