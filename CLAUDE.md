@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 423 tests pasan en Linux; en el Mac del usuario (GEOS 3.15.0) pasaron 420 con el
  commit `d265e06`, falta confirmar el parche del láser.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones de Gerber, postprocesadores `default` y
  `GRBL_laser`, G-code de taladrado (`CncJob`, optimizador propio `PathOptimizer`) y de geometrías (profundidad múltiple,
  corte extra, puntos, tolerancia, segmentación, desplazamiento de herramienta), **CutOut** (libre, rectangular y puente
  manual, en `src/tools/`) y la **capa de G-code del láser**.
- **Verificación:** todos los goldens salen de los originales con **Shapely 1.8.4**. Reproducidos byte por byte con los `.nc`
  reales del usuario: el contorno (`Edge_Cuts.gm1` -> CutOut rectangular -> G-code), `B_Cu.nc` y `Logo.nc` (desde sus propios
  trazados) y el taladro salvo el orden de los agujeros.
- **Hallazgo:** el G-code del láser de la 8.994 difiere del de la 2024.4 (`G00 Z0` en la cabecera, orden del Z inicial, sin
  `G00 Z19.00` al final). Se cubre con `GcodeFlavor`; falta decidir con el usuario la variante por defecto.
- **Siguiente:** generar los trazados de NCC y Paint (`clear_polygon`, `ToolNCC`, `ToolPaint`) y compararlos con los de
  `B_Cu.nc` y `Logo.nc`; después `Alinea` (PCB a 2 caras).
- **Pendiente de que el usuario aplique:** parche `fase3j-laser`.
- Detalle completo en `AGENTS.md`, secciones 4 y 11.
