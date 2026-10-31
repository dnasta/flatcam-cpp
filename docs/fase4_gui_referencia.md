# Fase 4: cómo usa el usuario FlatCAM 2024.4 (referencia para la GUI)

Resumen de las 15 capturas que pasó el usuario (FlatCAM 2024.4 BETA en Windows, interfaz en español, placa "luz de noche
Pascual", 30/10/2026). Las capturas no están en el repositorio; los archivos de esa placa sí (`tests/data/luz_de_noche/`).

## Ventana principal

- Título: `FlatCAM 2024.4 BETA - 64bit - [3D]   Proyecto nuevo: no guardado`.
- Menú: Archivo, Editar, Opciones, Ver, Objetos, Herramienta, Ayuda.
- Barra de herramientas de íconos: abrir Gerber/Excellon/proyecto, guardar, editores (fuera de alcance), borrar, zoom a todo,
  medir, origen, reflejo/PCB a 2 caras, CutOut, NCC, Paint, Isolation (`Iso`), Perforación, etc.
- **Panel izquierdo** con tres pestañas: `Proyecto` (árbol de objetos), `Propiedades` (el objeto elegido) y la de la
  herramienta abierta, cuyo título cambia: `Herramienta de pintura`, `Herramienta NCC`, `Herramienta de Perforación`,
  `Herramienta de Corte`. Es una columna angosta (~320 px) con desplazamiento vertical.
- **Canvas** a la derecha, en una pestaña `Área de la parcela`: fondo blanco, grilla gris con números en los ejes, ejes X e Y
  en rojo oscuro por el origen, cursor en cruz roja y un recuadro arriba a la izquierda con `Dx`, `Dy`, `X`, `Y` en mm
  (distancia desde el último clic y posición).
- **Barra de estado**: a la izquierda un mensaje con un punto de color (verde: OK; rojo: objeto seleccionado); a la derecha
  `X: … Y: …`, el botón de ajuste a la grilla con su paso (`0.1`), una casilla con el paso de la grilla (`0.1000`), íconos,
  `[mm]` y el estado (`Ocioso.`).

## Colores (de las opciones guardadas en el proyecto)

| Objeto | Relleno | Línea |
|---|---|---|
| Gerber | `#BBF268bf` (verde claro) | `#006E20bf` |
| Excellon | `#C40000bf` (rojo) | `#750000bf` |
| Geometría | — | `#FFAAAA` (rosa) |
| CNC Job | `#5E6CFFFF` | `#4650BDFF` |
| Viajes del CNC Job | `#F0E24D4c` | `#B5AB3A4c` (amarillo) |
| Anotaciones del CNC Job | texto `#990000`, 9 pt | números de orden en cada agujero o trazado |

## Paneles que usa (valores de esta placa)

- **Perforación** (`Herramienta de Perforación`, modo BASIC): combo del Excellon; tabla `# | Diámetro | Taladros | Muesca`
  con totales (120 taladros, 0 ranuras); `Orden de la Herram: No / Adelante / Atrás`; `Buscar en la BD`; parámetros por
  herramienta (Corte Z −2,4 y −3,4 para la de 3,4 mm, Profund. Múlti, Viaje Z 2, Avance Z 10, Eje de velocidad 10000,
  Habitar, Offset Z); `Aplicar Parám. a todas las herramientas`; comunes: Cambio de herram (sí, Z 19), Fin del movi. Z 19,
  X,Y fin `0,0`, Postprocesador `default`, Áreas de exclusión; botones `Generar objeto CNCJob` y `Restablecer la Herramienta`.
- **Corte** (`Herramienta de Corte`): objeto fuente Soltero/Panel y Gerber/Geometría; combo del objeto; Forma convexa;
  Diá. de herramienta 1,2 (con `Buscar y agregar` y `Elija de DB`); Corte Z −1,8, Profund. Múlti, Margen 0, Tamaño de la
  brecha 0,6, Tipo de brecha Puente/Delgado/M-Bites; `Brechas: 4`; dos botones `Generar Geometría` (libre y rectangular;
  usó el libre); huecos manuales.
- **Pintura** (`Herramienta de pintura`): tipo de objeto Geometry/Gerber; objeto (`Logo`); tabla de herramientas
  (`0.1000`, tipo `C1`); orden de herramientas; agregar desde la base; Superposición 80 %, Compensar 0, Método Estándar,
  Conectar y Contorno; Resto; Selección `Todos`; `Generar Geometría`.
- **NCC** (`Compensación sin cobre`): como Paint más Operación Limpiar/Aislamiento, tipo de fresado, Margen 0, Compensar,
  Selección `Sí mismo`, `Verificar validez` (tildado) y el botón `Óptimo` para el diámetro.
- **Propiedades de una geometría** (`Objeto de geometría`): nombre, `Editor de geometría`, `PROPIEDADES`, tabla de
  herramientas (`Dia`, `TT`, `P` = graficar), parámetros (Corte Z, Viaje Z, Avance X-Y, Avance Z, Eje de velocidad;
  con `GRBL_laser`: Avance X-Y 500 y `Poder del laser` 300), comunes (Cambio de herra. Z, Fin del movi. Z, X,Y fin,
  Postprocesador), `Generar objeto CNCJob`, accesos a Fresado/Pintura/NCC y transformaciones (escala y desplazamiento).

## Flujo de esta placa

1. Abrir los Gerber (`Edge_Cuts`, `B_Cu`, `Logo`) y el Excellon `PTH.drl`.
2. Reflejar en X el cobre, el logo y el taladro (cara inferior).
3. Perforación → CNC Job `default` → exportar.
4. Corte libre del contorno (4 puentes de 0,6) → geometría → propiedades → CNC Job `default` (avance 20) → exportar.
5. Paint del Logo → geometría → CNC Job `GRBL_laser` (avance 500, potencia 300) → exportar.
6. NCC de `B_Cu` → geometría → CNC Job `GRBL_laser` → exportar y **editar a mano** (agrega `G00 Z0` al empezar y quita el
   `G00 Z15.00` final: es la variante `GcodeFlavor::FlatCAM8_994` del port, la predeterminada).

Los G-code de la 2024.4 (`tests/data/luz_de_noche/*.nc`) muestran además que la 2024.4 simplifica todas las trayectorias de
geometría con `global_tolerance / 20 = 0,00025` (también el contorno) y que el ejecutable instalado difiere del fuente en el
orden de dos líneas del comienzo (`GcodeFlavor::FlatCAM2024_4Release`).
