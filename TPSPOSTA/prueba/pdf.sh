
PADRON=94485
CUATRIMESTRE="2013-2C"
ENTREGA="TP1"
if [ $# = 1 ];
then
	$ENTREGA = $1
fi
ENCODING="ISO-8859-1"
OUTPUTFILE="out.ps" #archivo intermedio
FORMATO="portrait" #portrait o landscape (vertical u horizontal)
COLUMNAS=1 #paginas por hoja (en columnas)
NUMEROS_LINEA=1 #cada cuantas lineas se imprime el numero de linea
ARCHIVO_ENTREGA="$PADRON-$ENTREGA"
NOMBRE_ZIP=$ARCHIVO_ENTREGA.zip
NOMBRE_PDF=$ARCHIVO_ENTREGA.pdf
ENCABEZADO="[75.41] Algoritmos y Programacion II"
PIE="Padron $PADRON (curso $CUATRIMESTRE) Entrega: $ENTREGA"


archivos=$(ls *.c *.h)

echo "Se van a parsear estos archivos: $archivos"

`a2ps $archivos "--header=$ENCABEZADO --footer=$PIE --output=$OUTPUTFILE"`

#a2ps $archivos  -Av --header=$ENCABEZADO --footer=$PIE --line-numbers=$NUMEROS_LINEA --borders=yes --columns=$COLUMNAS  --$FORMATO --output=$OUTPUTFILE --encoding=$ENCODING --tabsize=4 --major=columns --toc


