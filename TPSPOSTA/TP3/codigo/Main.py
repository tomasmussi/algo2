#!/usr/bin/python
import sys
import Sistema

from FileReader import *
from Sistema import *

""" Esta funcion se encargara de invocar los comandos. """
def invocarComando(sistema, linea):
	splitedLine = linea.split()
	comando = splitedLine[0]
	if (comando == "listar_fabricas"):
		sistema.listarFabricas()
	elif (comando == "valuar_juguetes"):
		sistema.valuarJuguetes(int(splitedLine[1]))	
	elif (comando == "valuar_juguetes_total"):
		sistema.valuarJuguetesTotal()
	elif (comando == "camino_optimo"):
		sistema.caminoOptimo(int(splitedLine[1]))
	elif (comando == "listar_juguetes"):
		sistema.listarJuguetes(int(splitedLine[1]))
	elif comando == "graficar_rutas":
		sistema.graficarRutas(int (splitedLine[1]))

def main():

	if (len(sys.argv) != 6):
		print "No se puede ejecutar el programa"
		print "Uso: <capacidad> <id polo norte> <archivo de fabricas> <archivo de juguetes> <archivo de mapa>"
		return 1

	# Creo el sistema y seteo id del polo norte y capacidad del trineo
	sistema = Sistema()
	sistema.setCapacidad(int(sys.argv[1]))
	sistema.setPoloNorte(int(sys.argv[2]))

	# Leo y cargo las fabricas al sistema
	fabricas = cargarFabricas(sys.argv[3])
	sistema.setFabricas(fabricas)

	# Leo y cargo los juguetes, le paso la referencia a fabricas
	# para que directamente se guarden los juguetes en una lista
	cargarJuguetes(sys.argv[4], fabricas)

	# Leo y cargo el mapa al sistema, necesito las fabricas
	mapa, esquinas = cargarMapa(sys.argv[5], fabricas)
	sistema.setMapa(mapa)
	sistema.setEsquinas(esquinas)

	continuar = True
	try:
		linea = raw_input()
		while (continuar):
			if (linea is not ""):
				invocarComando(sistema, linea)
			linea = raw_input()
	except EOFError, e:
		continuar = False
	return 0

if __name__ == "__main__":
    main()

