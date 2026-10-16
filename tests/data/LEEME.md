# Datos de prueba reales

Poné acá los archivos de un proyecto real para que los tests de las fases 2 y 3 comparen el port contra
el FlatCAM original:

- Los Gerber (`.gbl`, `.gm1`, `.gbr`, ...) y los Excellon (`.drl`) del proyecto.
- Los `.nc` generados por el FlatCAM de Windows a partir de esos archivos.
- Un archivo `PARAMETROS.txt` con, para cada `.nc`: herramienta usada, diámetro de fresa, profundidad de
  corte, avances, pasadas, solapamiento, postprocesador y cualquier otro parámetro que hayas cambiado.
  Sin esos números no se puede reproducir el mismo G-code.
- Opcional: el `.FlatPrj` del proyecto.

Los archivos son de texto y pesan poco, así que se pueden versionar con git.
