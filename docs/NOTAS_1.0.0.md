# FlatCAM-cpp 1.0.0

Primera versión completa del port de **FlatCAM 2024.4 beta** a C++17 y Qt6. Corre en macOS con Apple Silicon (paquete
`.dmg` autocontenido), y compila y pasa los tests en Linux y Windows.

## Qué trae

- **Archivos:** abrir Gerber (RS-274X, macros, regiones, arcos), Excellon, G-code y proyectos `.FlatPrj` (también los
  guardados por el FlatCAM en Python, y el original abre los que guarda el port); importar SVG, DXF, PDF, HPGL2 e
  imágenes; exportar Gerber, Excellon, SVG, DXF, PNG, el G-code y la impresión a PDF.
- **Herramientas:** Isolation, NCC (limpieza de cobre), Paint, Perforación, Corte, PCB a 2 caras, Panelizar, Film,
  Transformaciones, Calibración, Autonivelación, Pasta de soldar, Verificación de reglas, Robo de cobre, Fiduciales,
  Código QR, Marcas de esquina, Extraer taladros, Perforar Gerber, Restar, Óptimo, Compensación de grabado, Alinear
  objetos, Invertir Gerber, Calculadoras, Distancia, Mover, Propiedades, la base de datos de herramientas y el
  asistente de PcbWizard.
- **G-code:** los 20 postprocesadores del original, idénticos método por método.
- **Editores** de Geometría (con textos con fuentes), Excellon, Gerber y G-code.
- **Preferencias** (los 485 campos), nivel Básico y Avanzado, unidades mm/pulgadas, marcadores, archivos recientes.
- **Consola Tcl** con el intérprete Tcl real y los 65 comandos del original; scripts y documentos.
- **Idiomas:** alemán, inglés, español, francés, italiano, portugués de Brasil, rumano, ruso y turco (las traducciones
  del original). Arranca en el idioma del sistema; se cambia en Preferencias -> General -> Idiomas -> "Aplicar idioma".

## Cómo se verificó

Cada parte se comparó contra el código original de FlatCAM 2024.4 ejecutado con oráculos en Python (Shapely 1.8.4) y,
donde había, contra el G-code real del ejecutable 2024.4: el corte sale idéntico byte por byte, Paint y NCC con los
mismos trazados, el taladrado con los mismos agujeros. La suite corre con dos versiones de GEOS (3.12 y 3.15).

## Diferencias con el original

- **Errores del original corregidos.** Más de 200 errores encontrados al portar (listados en `AGENTS.md`, sección 14)
  están corregidos en la aplicación: comandos Tcl que nunca funcionaban (exportar, fresar, pintar, panelizar...),
  importaciones de SVG, DXF y PDF, el editor de Geometría (la herramienta Buffer), los postprocesadores, Film,
  Calibración, etc. Con `--original-bugs` (o `FLATCAM_ORIGINAL_BUGS=1`) la aplicación se comporta como el original.
- **Desvíos conscientes**, documentados en `AGENTS.md` (sección 11): el orden de los agujeros lo calcula un optimizador
  propio (en vez de OR-Tools); el estilo de la interfaz es Fusion en todas las plataformas; los archivos se leen en el
  hilo de la interfaz; el G-code de láser por defecto es la variante de la 8.994 (`G00 Z0` al empezar).
- **Pendiente:** aplicar el mapa de alturas de la autonivelación al G-code (en el original tampoco está hecho).

## Instalar

- **macOS:** abrir `FlatCAM-1.0.0-macOS-arm64.dmg` y arrastrar FlatCAM a Aplicaciones. La firma es ad hoc: la primera
  vez hay que abrirla con clic derecho -> Abrir.
- **Linux y Windows:** compilar desde el código (ver `README.md`).
