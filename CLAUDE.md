@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 424 tests pasan en Linux; en el Mac del usuario (GEOS 3.15.0) pasaron 423 con el
  commit `33def09`, falta confirmar el parche que fija la variante 8.994 por defecto.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones de Gerber, postprocesadores `default` y
  `GRBL_laser`, G-code de taladrado (`CncJob`, optimizador propio `PathOptimizer`) y de geometrías (profundidad múltiple,
  corte extra, puntos, tolerancia, segmentación, desplazamiento de herramienta), **CutOut** (libre, rectangular y puente
  manual, en `src/tools/`) y la **capa de G-code del láser**.
- **Verificación:** todos los goldens salen de los originales con **Shapely 1.8.4**. Reproducidos byte por byte con los `.nc`
  reales del usuario: el contorno (`Edge_Cuts.gm1` -> CutOut rectangular -> G-code), `B_Cu.nc` y `Logo.nc` (desde sus propios
  trazados) y el taladro salvo el orden de los agujeros.
- **Decisión del usuario:** el G-code del láser sale como el de su FlatCAM 8.994 (con `G00 Z0`); es la variante por defecto
  (`GcodeFlavor::FlatCAM8_994`). La 2024.4 sigue disponible y la usan los tests contra el código original.
- **Siguiente:** generar los trazados de NCC y Paint (`FlatCAMRTreeStorage`, `paint_connect`, `clear_polygon`, `ToolPaint`,
  `ToolNCC`) y compararlos con los de `B_Cu.nc` y `Logo.nc`; plan detallado en `AGENTS.md` sección 10. Después `Alinea`.
- **Pendiente de que el usuario aplique:** parche `fase3k-flavor`.
- Detalle completo en `AGENTS.md`, secciones 4, 10 y 11.
