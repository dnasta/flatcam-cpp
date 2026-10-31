@AGENTS.md

## Claude Code

Las instrucciones del proyecto están en `AGENTS.md` (se importan arriba). En Claude Code podés commitear y hacer
`push` directamente a `main` (sin generar archivos `.patch`) y compilar y ejecutar la aplicación en la propia
máquina del usuario.

## Reglas permanentes del usuario

1. Verificá y corregí todo lo que haga falta **apenas lo detectes**, sin esperar a que lo pida (código, tests y docs).
2. Al terminar cada respuesta, actualizá el estado del proyecto en este archivo y en `AGENTS.md`.

## Estado actual (se actualiza al final de cada respuesta)

- **Fase 3 en curso** (fases 1 y 2 hechas). 420 tests pasan en Linux; en el Mac del usuario (GEOS 3.15.0) pasaron 376 con el
  commit `1c52023`, falta confirmar el parche del CutOut.
- **Hecho:** núcleo de geometría, parsers de Gerber y Excellon, transformaciones de Gerber, postprocesadores `default` y
  `GRBL_laser`, G-code de taladrado (`CncJob`, optimizador propio `PathOptimizer`) y de geometrías (profundidad múltiple,
  corte extra, puntos, tolerancia, segmentación, desplazamiento de herramienta) y **CutOut** (libre, rectangular y puente
  manual, en `src/tools/`).
- **Verificación:** todos los goldens salen de los originales con **Shapely 1.8.4**. Reproducidos byte por byte con el
  `.nc` real del usuario: el contorno (`Edge_Cuts.gm1` -> CutOut rectangular -> G-code); y el taladro salvo el orden de
  los agujeros.
- **Siguiente:** NCC y Paint con láser (`GRBL_laser`) contra `B_Cu.nc` y `B_Logo.nc`, después `Alinea` (PCB a 2 caras).
- **Pendiente de que el usuario aplique:** parche `fase3i-cutout`.
- Detalle completo en `AGENTS.md`, secciones 4 y 11.
