@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 445 tests pasan en Linux con GEOS 3.12 y con 3.15.0; en el Mac del usuario (GEOS 3.15.0)
  pasaron 443 de 445 con `c09f003` (2 tolerancias, ya corregidas): falta confirmar el parche `fase3m`.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones de Gerber, postprocesadores `default` y
  `GRBL_laser`, G-code de taladrado y de geometrías (`CncJob`), **CutOut**, capa de G-code del láser y **Paint estándar**.
- **Regla de entrega nueva:** correr toda la suite con GEOS 3.12 (sistema) **y** 3.15.0 (micromamba) antes de entregar; las
  tolerancias se fijan en micras, no en el último bit (detalle en `AGENTS.md`, sección 6).
- **Verificación:** goldens de los originales con Shapely 1.8.4 (y `rtree` real). Byte por byte con los `.nc` reales: contorno y
  `B_Cu.nc`/`Logo.nc` desde sus trazados. El Paint del `Logo` desde el Gerber: 38 trazados en el mismo orden, 3,5 a 5 micras.
- **Decisión del usuario:** el G-code del láser sale como el de su FlatCAM 8.994 (con `G00 Z0`), variante por defecto.
- **Siguiente:** NCC (`ToolNCC`): región a limpiar, margen y contorno, y compararla con `B_Cu.nc`; después `Alinea`.
- **Pendiente de que el usuario aplique:** parche `fase3m-geos315`.
- Detalle completo en `AGENTS.md`, secciones 4, 6, 7, 10 y 11.
