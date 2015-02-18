import heapq

from heapq import *


class Grafo(object):

	def __init__(self, infinito = 10000000000):
		self.vertices = {}
		self.infinito = infinito

	def agregarVertice(self, vertice):
		if vertice in self.vertices:
			return False
		self.vertices[vertice] = {}
		return True

	def borrarVertice(self, vertice):
		vertices = self.vertices
		if not vertice in vertices:
			return False
		vertices.pop(vertice)
		for otros in vertices.keys():
			if vertice in vertices[otros]:
				vertices[otros].pop(vertice)
		return True

	def agregarArista(self, v1, v2, peso = 0.0):
		vertices = self.vertices
		if v1 not in vertices or v2 not in vertices:
			return False
		vertices[v1][v2] = peso
		vertices[v2][v1] = peso
		return True

	def borrarArista(self, v1, v2):
		vertices = self.vertices
		# Verifico que los vertices existan
		if v1 not in vertices or v2 not in vertices:
			return False
		# Verifico que la arista exista
		if v2 not in vertices[v1] or v1 not in vertices[v2]:
			return False
		# Borro
		vertices[v1].pop(v2)
		vertices[v2].pop(v1)
		return True

	def obtenerPeso(self, v1, v2):
		vertices = self.vertices
		# Verifico que los vertices existan
		if v1 not in vertices or v2 not in vertices:
			return None
		# Verifico que la arista exista
		if v2 not in vertices[v1] or v1 not in vertices[v2]:
			return None
		return vertices[v1][v2]

	def obtenerAristas(self, v):
		if not v in self.vertices:
			return None
		return self.vertices[v].keys()

	def hayArista(self, v1, v2):
		if v1 not in self.vertices or v2 not in self.vertices:
			return False
		return v1 in self.vertices[v2]


	def recorridoDijkstra(self, v):
		# LLamo vertices para no estar todo el tiempo con self
		vertices = self.vertices
		# Inicializo arreglos de distancia y anteriores
		distancia = dict()
		visitado = dict()
		anterior = dict()
		for x in vertices:
			distancia[x] = self.infinito
			visitado[x] = False
			anterior[x] = None
		heap = []
		distancia[v] = 0
		heappush(heap, {distancia[v]: v})
		while (len(heap) != 0):
			diccionario = heappop(heap)
			unVertice = diccionario.values()[0]
			visitado[unVertice] = True
			for vecino in vertices[unVertice].keys():
				dist = distancia[unVertice] + vertices[unVertice][vecino]
				if dist < distancia[vecino]:
					distancia[vecino] = dist
					anterior[vecino] = unVertice
					heappush(heap, {distancia[vecino]: vecino})


		return distancia, anterior


	def recorridoMinimo(self, v1, v2):
		if v1 == v2:
			return [v1]
		dist, anteriores = self.recorridoDijkstra(v1)
		camino = []
		if dist[v2] == self.infinito:
			return camino
		camino.append(v2)
		vertice = anteriores[v2]
		while vertice != v1:
			camino.append(vertice)
			vertice = anteriores[vertice]
		camino.append(v1)
		camino.reverse()
		return camino
		

	def distanciaMinima(self, v1, v2):
		if v1 == v2:
			return 0.0
		distancia, anteriores = self.recorridoDijkstra(v1)
		return distancia[v2]

	def mostrar(self):
		for x in self.vertices.keys():
			print str(x) + " = " + str(self.vertices[x])
		
