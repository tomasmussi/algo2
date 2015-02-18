ARCHIVOS=curso.c  curso.h  lista.c  lista.h  main.c  makefile  pila.c  pila.h  sesion.c  sesion.h  sistema.c  sistema.h
ENCABEZADO="[75.41] Algoritmos y Programacion II"
PIE="Padrón $(PADRON) (curso $(CUATRIMESTRE)) Entrega: $(ENTREGA)"

a2ps $(ARCHIVOS) -Av --header=$(ENCABEZADO) --footer=$(PIE) --line-numbers=$(NUMEROS_LINEA) --borders=yes --columns=$(COLUMNAS)  --$(FORMATO) --output=$(OUTPUTFILE) --encoding=$(ENCODING) --tabsize=4 --major=columns --toc  | ps2pdf $(OUTPUTFILE) $(NOMBRE_PDF)
        rm *.ps #elimino los archivos temporales
