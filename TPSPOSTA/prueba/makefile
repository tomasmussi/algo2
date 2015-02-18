PADRON=94485
ARCHIVOS=*.h *.c #Tipos de archivos que se van a agregar
CUATRIMESTRE=2013-2C
ENTREGA=TP1
ENCODING=ISO-8859-1
OUTPUTFILE=out.ps #archivo intermedio
FORMATO=landscape #portrait o landscape (vertical u horizontal)
COLUMNAS=2 #paginas por hoja (en columnas)
NUMEROS_LINEA=1 #cada cuantas lineas se imprime el numero de linea
ARCHIVO_ENTREGA=$(PADRON)-$(ENTREGA)
NOMBRE_ZIP= $(ARCHIVO_ENTREGA).zip
NOMBRE_PDF= $(ARCHIVO_ENTREGA).pdf
ENCABEZADO = "[75.41] Algoritmos y Programacion II"
PIE = "Padron $(PADRON) (curso $(CUATRIMESTRE)) Entrega: $(ENTREGA)"
 
 
pdf: clean
	a2ps $(ARCHIVOS) -Av --header=$(ENCABEZADO) --footer=$(PIE) --line-numbers=$(NUMEROS_LINEA) --borders=yes --columns=$(COLUMNAS)  --$(FORMATO) --output=$(OUTPUTFILE) --encoding=$(ENCODING) --tabsize=4 --major=columns --toc  | ps2pdf $(OUTPUTFILE) $(NOMBRE_PDF)
	rm *.ps #elimino los archivos temporales
 
entrega: pdf
	zip $(NOMBRE_ZIP) $(ARCHIVOS) *.pdf
 
clean:
	rm -f *.zip *.pdf
