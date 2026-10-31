@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 445 tests pasan en Linux; en el Mac del usuario (GEOS 3.15.0) pasaron 424 con el
  commit `555c470`, falta confirmar el parche del pintado.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones de Gerber, postprocesadores `default` y
  `GRBL_laser`, G-code de taladrado y de geometrías (`CncJob`), **CutOut** (`src/tools/`), capa de G-code del láser y
  **Paint estándar** (`EndpointStorage`, `PolygonClearing`, `Paint`, orden `R`).
- **Verificación:** todos los goldens salen de los originales con **Shapely 1.8.4** (y `rtree` real para Paint). Byte por byte
  con los `.nc` reales: el contorno y `B_Cu.nc`/`Logo.nc` desde sus propios trazados. El Paint del `Logo` desde el Gerber
  da 38 trazados en el mismo orden con 3,5 micras de diferencia (no es idéntico: la 8.994 simplifica con otros desempates).
- **Decisión del usuario:** el G-code del láser sale como el de su FlatCAM 8.994 (con `G00 Z0`), variante por defecto.
- **Siguiente:** NCC (`ToolNCC`): región a limpiar, margen y contorno, y compararla con `B_Cu.nc`; después `Alinea`.
- **Pendiente de que el usuario aplique:** parche `fase3l-paint`.
- Detalle completo en `AGENTS.md`, secciones 4, 7, 10 y 11.
