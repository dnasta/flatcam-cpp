@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 376 tests pasan en Linux y en el Mac del usuario (GEOS 3.15.0, commit `95f5c79`).
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones de Gerber, postprocesadores `default` y
  `GRBL_laser`, G-code de taladrado (`CncJob`, optimizador propio `PathOptimizer`) y de geometrías (profundidad múltiple,
  corte extra, puntos, tolerancia, segmentación, desplazamiento de herramienta).
- **Verificación:** todos los goldens salen de los originales con **Shapely 1.8.4**; el contorno del usuario se reproduce
  byte por byte y el taladro salvo el orden de los agujeros.
- **Siguiente:** CutOut (geometría con puentes desde `Edge_Cuts.gm1`), después NCC y Paint con láser, y `Alinea`.
- **Pendiente de que el usuario aplique:** parche `fase3h-estado` (golden de Gerber con Shapely 1.8.4 + este estado).
- Detalle completo en `AGENTS.md`, secciones 4 y 11.
