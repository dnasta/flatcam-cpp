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
  pasadas muy juntas, propias de un láser. No alcanza para deducir el diámetro de herramienta ni el
  solapamiento.
- **Paint (`Logo`)**: la separación entre trazados no muestra un patrón claro; no se pudo estimar.

## Pendientes (no están en los `.nc`)

1. **NCC y Paint**: diámetro de herramienta, solapamiento, método (estándar, semilla, líneas), margen,
   "conectar" y "contorno". FlatCAM no los escribe en el G-code. Los da el panel de cada herramienta o el
   `.FlatPrj`.
2. **Versión**: confirmar si la 2024.4 genera el mismo G-code que la 8.994 con el mismo proyecto.
3. **Reflejo (mirror)**: confirmar el eje y el punto usados para la cara inferior (por el Excellon, X → -X
   respecto de X = 0) y si `B_Cu` y `Logo` también se reflejan.
4. **Tamaño del puente** del CutOut y origen del taladro `(0, 0)` en `Alinea`.
