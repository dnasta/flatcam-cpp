# Parámetros de los G-code de referencia (`tests/data/*.nc`)

Extraídos de los encabezados de los propios `.nc` (FlatCAM los escribe como comentarios) y del análisis de
sus movimientos. Todo está en milímetros. Sirve como entrada de los tests de la fase 3: la herramienta del
port debe generar el mismo G-code a partir de los Gerber/Excellon con estos parámetros.

> **Atención:** los cinco `.nc` fueron generados por **FlatCAM v8.994 (fecha de versión 2020/11/7)**, no por la
> 2024.4 β cuyos fuentes están en `reference/`. Hay que confirmar con una exportación de la 2024.4 si ambas
> producen el mismo G-code (ver "Pendientes" al final).

## Resumen de los cinco trabajos

| Archivo `.nc` | Origen en FlatCAM | Operación | Postprocesador | Entrada del port |
|---|---|---|---|---|
| `DosCau_PPal-B_Cu.nc` | `PCB ppal-B_Cu.gbl_ncc` | NCC con láser (cobre de la cara inferior) | `GRBL_laser` | `PCB principal-B_Cu.gbl` |
| `DosCau_PPal-Logo.nc` | `PCB principal-B_Logo.gbr_mt_paint` | Paint con láser (logo) | `GRBL_laser` | `PCB principal-B_Logo.gbr` |
| `DosCau_PPal-Edge_Cuts.nc` | `PCB ppal-Edge_Cuts.gm1_cutout` | Corte del contorno con fresa | `default` (Mach3) | `PCB principal-Edge_Cuts.gm1` |
| `DosCau_PPal-Drill.nc` | `PCB ppal-Drill.drl` | Taladrado | `default` (Excellon, Mach3) | `PCB principal-Drill.drl` |
| `DosCau_PPal-Alinea.nc` | `PCB ppal-Alinea.drl` | Taladros de alineación (PCB a 2 caras) | `default` (Excellon, Mach3) | Se genera con la herramienta "PCB a 2 caras" (no hay `.drl`) |

Los nombres de origen salen del encabezado (`Name:`); los Gerber se renombraron después de `ppal` a
`principal`.

Los archivos `PCB principal-B_Mask.gbs` y `PCB principal-F_Silkscreen.gto` no tienen `.nc`: son casos de
prueba extra para el parser de Gerber (máscara negativa, serigrafía con arcos).

## Parámetros por archivo

### `DosCau_PPal-B_Cu.nc` (NCC, láser)
Creado el lunes 11/11/2024 15:24.

| Parámetro | Valor |
|---|---|
| Postprocesador | `GRBL_laser` |
| Avance de trabajo (Feedrate) | 500,0 mm/min |
| Avance de rápidos (Feedrate rapids) | 1500,0 mm/min |
| Z Focus | 2,0 mm |
| Potencia del láser (Spindle Speed) | 300,0 |
| Pasos por círculo (Steps per circle) | 64 |
| Rango X | -35,4124 … 35,6520 |
| Rango Y | -36,7244 … 36,9060 |

Cuerpo del G-code: 502.025 líneas, 723 trazados (227 cerrados), 499.094 puntos, longitud total de trazado
68.471 mm, segmento mediano 0,0196 mm. No contiene movimientos en Z (salvo `G00 Z0` inicial): el
postprocesador láser los omite. Cada trazado se enciende con `M03 S300.0` y se apaga con `M5`.

### `DosCau_PPal-Logo.nc` (Paint, láser)
Creado el martes 12/11/2024 15:16. Mismo postprocesador y parámetros que el anterior:
`GRBL_laser`, avance 500,0, rápidos 1500,0, Z Focus 2,0, potencia 300,0, 64 pasos por círculo.
Rango X: -4,2967 … 0,3067. Rango Y: -9,9844 … 2,8751.
Cuerpo: 6.593 líneas, 38 trazados (4 cerrados), 6.402 puntos, longitud total 226,9 mm.

### `DosCau_PPal-Edge_Cuts.nc` (CutOut)
Creado el lunes 11/11/2024 15:36. Postprocesador de geometría `default`.

| Parámetro | Valor |
|---|---|
| Diámetro de herramienta | 1,2 mm |
| Avance XY | 100,0 mm/min |
| Avance Z | 100,0 mm/min |
| Avance de rápidos | 1500,0 mm/min |
| Z de corte (Z_Cut) | -4,5 mm |
| Z de movimiento (Z_Move) | 2,0 mm |
| Z de cambio de herramienta / Z final | 19,0 mm |
| X,Y final | 0,0000 , 0,0000 |
| Z inicial | ninguno (`None`) |
| Velocidad del husillo | 1000 RPM |
| Pasos por círculo | 64 |
| Rango X / Y | ±37,65 / ±39,15 |

Deducido del G-code (no figura en el encabezado):
- Corte **exterior** con margen 0: el Gerber del contorno es un rectángulo de ±37,0 × ±38,5 con trazo de
  0,1 mm; la trayectoria del centro de la fresa queda a 0,65 mm (0,05 del trazo + 0,6 de radio).
- Una sola pasada en profundidad (Z -4,5 una vez por tramo).
- **Dos puentes (gaps)**, uno arriba y otro abajo, centrados en X = 0: la trayectoria se interrumpe entre
  X = -0,8 y X = +0,8 (1,6 mm de hueco en el recorrido del centro de la fresa) en Y = ±39,15. Corresponde a
  la opción de puentes "arriba y abajo" (2tb). El valor exacto del parámetro "tamaño del puente" se
  confirmará leyendo la herramienta en la fase 3.
- Esquinas redondeadas con 16 segmentos por cuarto de círculo (equivale a los 64 pasos por círculo).

### `DosCau_PPal-Drill.nc` (taladrado)
Creado el lunes 11/11/2024 15:40. Postprocesador de Excellon `default`.

| Herramienta | Diámetro | Avance Z | Rápidos | Z_Cut | Offset Z | Z_Move | Agujeros |
|---|---|---|---|---|---|---|---|
| T1 | 0,8 mm | 100,0 | 1500 | -3,5 | 0,0 | 2 | 90 |
| T2 | 1,0 mm | 100,0 | 1500 | -3,5 | 0,0 | 2 | 96 |
| T3 | 1,5 mm | 100,0 | 1500 | -3,5 | 0,0 | 2 | 2 |
| T4 | 3,4 mm | 100,0 | 1500 | **-4,5** | 0,0 | 2 | 4 |

Total: 192 agujeros (coincide con los 192 flashes `D03` del Gerber de máscara).
Z de cambio de herramienta 19,0 mm, X,Y de cambio 0,0000 , 0,0000, Z inicial `None`, Z final 19,0 mm,
X,Y final 0,0000 , 0,0000, husillo 1000 RPM, 64 pasos por círculo.
Rango X: -33,9620 … 32,2400. Rango Y: -33,5540 … 32,2860.
Secuencia por agujero: `G00 X.. Y..` → `G01 Z<Z_Cut>` → `G01 Z0` → `G00 Z2.0000`. Hay cambio de
herramienta con `T<n>`, `M6` y `M0`.

Deducido: **los taladros salen reflejados en X.** El Excellon trae `X-26.239 Y24.194` y el G-code
`X26.2390 Y24.1940`, o sea x → -x respecto de X = 0 (cara inferior del PCB).

### `DosCau_PPal-Alinea.nc` (taladros de alineación)
Creado el lunes 11/11/2024 15:43. Postprocesador de Excellon `default`.

| Parámetro | Valor |
|---|---|
| Herramienta T1 | 0,4 mm |
| Avance Z | 50,0 mm/min |
| Rápidos | 1500 |
| Z_Cut | -3,5 mm |
| Offset Z | 0,0 |
| Z_Move | 2 mm |
| Cambio de herramienta, final | Z 19,0 mm, X,Y 0,0000 , 0,0000 |
| Husillo | 1000 RPM |
| Rango X / Y | ±38,2 / ±39,7 (incluye el radio de la fresa) |

Posiciones taladradas: `(0, 0)`, `(-38, 39,5)`, `(38, 39,5)`, `(38, -39,5)`, `(-38, -39,5)`. El
encabezado indica "Total drills for tool T1 = 4": el punto `(0, 0)` aparece como primer movimiento
además de los cuatro de las esquinas; se aclarará al leer la herramienta "PCB a 2 caras".

## Estimaciones desde el G-code (no son parámetros confirmados)

- **NCC (`B_Cu`)**: la distancia entre trazados vecinos tiene mediana de 0,04 mm y moda en 0,02 mm, es decir
  pasadas muy juntas, propias de un láser. Es coherente con una herramienta de 0,1 mm y 80 % de superposición
  (0,1 × (1 − 0,8) = 0,02 mm), que son los valores de las capturas de la sección siguiente. Coincide, pero no
  está confirmado que se hayan usado esos valores en este proyecto.
- **Paint (`Logo`)**: la separación entre trazados no muestra un patrón claro; no se pudo estimar.

## Valores de los paneles de FlatCAM 2024.4 (capturas del 30/10/2026)

**Ojo:** las capturas son del proyecto **luz de noche Pascual** (`luz de noche Pascual-B_Cu.gbr`, `-Logo.gbr`,
`-Edge_Cuts.gbr`, `-PTH.drl`), **no del dosificador** del que salen los `.nc` de esta carpeta. El título de la
ventana dice "Proyecto nuevo: no guardado". Sirven para conocer las opciones de cada herramienta y los valores
que el usuario suele usar, pero **no se pueden dar por válidos para los `DosCau_*`**.

| Herramienta | Valores en las capturas (luz de noche Pascual) |
|---|---|
| **NCC** ("Herramienta NCC", Gerber) | Herramienta 0,1 mm (tipo C1). Orden de herramientas: Atrás. Operación: Limpiar. Superposición 80,0000 %. Método: Estándar. Margen 0. Conectar ✓. Contorno ✓. Compensar ☐ (0,0000). Resto ☐. Selección: Sí mismo. Verificar validez ✓. |
| **Paint** ("Herramienta de pintura", Gerber) | Herramienta 0,1 mm (C1). Orden: Atrás. Superposición 80,000 %. Compensar 0,0000. Método: Estándar. Conectar ✓. Contorno ✓. Resto ☐. Selección: Todos. |
| **CutOut** ("Herramienta de Corte") | Objeto simple (Soltero), tipo Gerber, forma convexa ☐. Fresa 1,2 mm. Corte Z -1,8. Profundidad múltiple ☐ (0,6). Margen 0. Tamaño de la brecha 0,6. Tipo de brecha: Puente. Brechas automáticas: 4. Cursor grande ✓ (manual). |
| Geometría del CutOut (pestaña Propiedades) | Viaje Z 2,0. Avance X-Y 20,0. Avance Z 60,0. Eje de velocidad 10000. Habitar ☐ (1,0). Cambio de herramienta Z ✓ 19,0. Fin del movimiento Z 19,0. X,Y fin 0,0. Postprocesador `default`. Áreas de exclusión ☐. Acabado ☐. |
| **Perforación** ("Herramienta de Perforación", Excellon `-PTH.drl`) | Herramientas: 0,8 mm × 84, 1,0 × 29, 1,5 × 4, 3,4 × 3 (120 taladros, 0 ranuras). Orden: No. Corte Z -2,4 (otra captura: -3,4). Profundidad múltiple ☐ (0,7). Viaje Z 2,0. Avance Z 20,0 (otra captura: 10,0). Eje de velocidad 10000. Habitar ☐ (1,0). Offset Z 0,0. Cambio de herramienta ✓, Z 19,0. Fin del movimiento Z 19,0. X,Y fin 0,0. Postprocesador `default`. Áreas de exclusión ☐. |
| Geometría NCC / Paint con láser (Propiedades) | Herramienta 0,1 mm. Avance X-Y 500,0. Poder del láser 300. Cambio de herramienta Z 15,0 (desactivado). X,Y fin: 0,0 (NCC) y `None` (Paint). Postprocesador `GRBL_laser`. Áreas de exclusión ☐. Acabado ☐. |

**Qué coincide con los `.nc` del dosificador:** postprocesador `GRBL_laser` con avance 500 y potencia 300; Z de
viaje 2,0; Z de cambio de herramienta y final 19,0; X,Y final 0,0; postprocesador `default` en taladro y corte;
paso de las pasadas del NCC (0,02 mm).

**Qué NO coincide (son ajustes de otro proyecto):** Z de corte del CutOut (-1,8 contra -4,5), avances del CutOut
(20 y 60 contra 100 y 100), brechas (0,6 mm y 4 contra 2 puentes arriba y abajo), Z de corte de los taladros
(-2,4 o -3,4 contra -3,5 y -4,5), avance Z de los taladros (20 o 10 contra 100) y velocidad del husillo (10000
contra 1000 RPM).

Nota: "Herramienta de Perforación" es la herramienta de taladrado nueva de la 2024.4. El encabezado de
`DosCau_PPal-Drill.nc` dice `Preprocessor Excellon`, así que ese G-code salió del flujo viejo (8.994).

## Pendientes (no están en los `.nc`)

1. **Parámetros de NCC y Paint del proyecto del dosificador**: las capturas son de otro proyecto. Falta
   confirmar si para los `DosCau_*` se usó herramienta 0,1 mm, superposición 80 %, método estándar, margen 0,
   conectar y contorno. Lo ideal es el `.FlatPrj` del dosificador.
2. **Versión**: confirmar si la 2024.4 genera el mismo G-code que la 8.994 con el mismo proyecto.
3. **Reflejo (mirror)**: confirmar el eje y el punto usados para la cara inferior (por el Excellon, X → -X
   respecto de X = 0) y si `B_Cu` y `Logo` también se reflejan.
4. **Tamaño del puente** del CutOut del dosificador y origen del taladro `(0, 0)` en `Alinea`.
