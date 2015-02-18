import csv
import Grafo
import math

from Grafo import *

# Esta funcion se encargara de cargar todas las Fabricas desde el archivo dado por parametro
def cargarFabricas(path):
	file = open(path, "rb")
	lector = csv.reader(file)
	fabricas = {}
	for row in lector:
		idFabrica = int(row[0])
		idEsquina = int(row[1])
		apertura = int(row[2])	#cantidad de minutos desde de las 00:00
		cierre = int(row[3])	#cantidad de minutos desde de las 00:00
		fabricas[idFabrica] = [idEsquina, apertura, cierre, [] ] #La lista es para cuando se ingresen los jugetes

	file.close()
	return fabricas

# Esta funcion se encargara de cargar el Mapa y su informacion desde el archivo dado por parametro
def cargarMapa(path, fabricas):
	file = open(path, "rU")
	lector = csv.reader(file)
	cantidad = int(lector.next()[0]) # Esta es la primer linea del archivo que dice donde cambia de formato

	mapa = Grafo()
	esquinas = {}
	i = 0
	while i < cantidad:
		row = lector.next()
		idEsquina = row[0]
		x = row[1]
		y = row[2]
		latitud = row[3]
		longitud = row[4]
		esquinas[int(idEsquina)] = [float(x), float(y), latitud, longitud]
		mapa.agregarVertice(int(idEsquina))
		i += 1

	cantidad = int(lector.next()[0])
	i = 0
	while i < cantidad:
		# No se si hace falta guardar las calles, hay que releer el enunciado
		row = lector.next()
		idCalle = row[0]
		idEsqInicial = int(row[1])
		idEsqFinal = int(row[2])
		x1 = float(esquinas[idEsqInicial][0])
		y1 = float(esquinas[idEsqInicial][1])
		x2 = float(esquinas[idEsqFinal][0])
		y2 = float(esquinas[idEsqFinal][1])
		peso = calcularDistancia(x1,y1, x2,y2)
		mapa.agregarArista(int(idEsqInicial), int(idEsqFinal), peso)
		i += 1
	file.close()
	return mapa, esquinas

# Esta funcion se encargara de cargar todos los juguetes desde el archivo dado por parametro 
def cargarJuguetes(path, fabricas):
	file = open(path, "rb")
	lector = csv.reader(file)
	for row in lector:
		idFabrica = int(row[0])
		idJuguete = int(row[1])
		valor = int(row[2])
		peso = int(row[3])
		# Posicion 3 es la lista de juguetes
		fabricas[idFabrica][3].append([idJuguete, valor, peso])
	file.close()

def calcularDistancia(x1,y1, x2,y2):
	d2 = ((x2 - x1) ** 2) + ((y2 - y1) ** 2)
	return math.sqrt(d2)

# EOF
