import sys

class Sistema(object):

	def setPoloNorte(self, idPoloNorte):
		self.idPoloNorte = idPoloNorte

	def setCapacidad(self, capacidad):
		self.capacidad = capacidad

	def setFabricas(self, fabricas):
		self.fabricas = fabricas
		self.setFabricasVisitar()

	def setMapa(self, mapa):
		self.mapa = mapa

	def setEsquinas(self, esquinas):
		self.esquinas = esquinas

	def setFabricasVisitar(self):
		fabricas = []
		ordenadas = []
		# Ordeno por cierre, apertura e id
		for x in self.fabricas:
			lista = [self.fabricas[x][2], self.fabricas[x][1], x]
			ordenadas.append(lista)
		ordenadas.sort()
		# En ordenadas tengo todas las fabricas ordenadas bien, itero para armar la lista a recorrer
		hasta = 0
		indice = 0
		while (indice < len(ordenadas)):
			cierre = ordenadas[indice][0]
			apertura = ordenadas[indice][1]
			if (apertura >= hasta):
				fabricas.append(ordenadas[indice][2])
				hasta = cierre
			indice += 1
		self.fabricasVisitar = fabricas

	def listarFabricas(self):
		print "Cantidad: " + str((len(self.fabricasVisitar)))
		for fab in self.fabricasVisitar:
			fabrica = self.fabricas[fab]
			print str(fab) + "," + formatear(fabrica[1]) + "," + formatear(fabrica[2])

	def caminoOptimo(self, idFabrica):
		idEsquina = self.fabricas[idFabrica][0]
		camino = self.mapa.recorridoMinimo(self.idPoloNorte, idEsquina)
		distancia = self.mapa.distanciaMinima(self.idPoloNorte, idEsquina)
		if "." in str(distancia):
			indiceLongitud = str(distancia).index(".")
		else:
			indiceLongitud = len(str(distancia))
		print "Distancia: " + str(distancia)[:indiceLongitud] + " Metros"
		for esquina in camino:
			# [2] = latitud ; [3] = longitud
			print self.esquinas[esquina][2] + "," + self.esquinas[esquina][3]

	# Formato de fabricas: fabricasVisitar[idFabrica] = [idEsquina, apertura, cierre, [] ]
	def verJuguetesMaximos(self, idFabrica):
			
		if idFabrica not in self.fabricasVisitar:
			print "Error: La fabrica con id " + str(idFabrica) + " no existe"
			return
		# Armo la solucion optima en una lista, porque lo unico que necesito
		# es la ultima solucion, no todas las anteriores (dinamica)
		# con la matriz se iba a los canios cuando le pedias todas las fabricas
		juguetes = self.fabricas[idFabrica][3]
		solVieja = []
		filas = len(juguetes) + 1
		columnas = self.capacidad + 1
		juguetesViejos = []
		juguetesNuevos = []
		for i in range(columnas):
			solVieja.append(0)
			juguetesViejos.append([])
		solNueva = []
		for fila in range(filas):
			solNueva = list()
			juguetesNuevos = []
			for i in range(columnas):
				juguetesNuevos.append([])
			if fila != 0:
				idj = juguetes[fila - 1][0]
				valor = juguetes[fila - 1][1]
				peso = juguetes[fila - 1][2]	
				for col in range(columnas):
					if col != 0:
						# Solucion si dejo intacta la mochila
						sol1 = solVieja[col]
						# Solucion si saco un peso del actual juguete y le sumo el nuevo valor
						sol2 = 0
						if col - peso >= 0:
							sol2 = solVieja[col - peso] + valor
						solNueva.append(max(sol1, sol2))
						if sol1 < sol2:
							juguetesNuevos[col] = juguetesViejos[col - peso] + [idj]
						else:
							juguetesNuevos[col] = juguetesViejos[col]
							
					else:
						solNueva.append(0)
				solVieja = []
				juguetesViejos = []
				for i in range(len(juguetesNuevos)):
					juguetesViejos.append(juguetesNuevos[i])

				for i in range(len(solNueva)):
					solVieja.append(solNueva[i])							
		solucion = solNueva[self.capacidad]
		return solucion, juguetesNuevos[len(juguetesNuevos) - 1]

	def valuarJuguetesTotal(self):
		total = 0
		for i in self.fabricasVisitar:
			tupla = self.verJuguetesMaximos(i)
			total += tupla[0]
		print "Total: " + str(total) + " sonrisas"
	
	def valuarJuguetes(self, idFabrica):
		tupla = self.verJuguetesMaximos(idFabrica)
		if tupla[0] != None:
			print "Total: " + str(tupla[0]) + " sonrisas"

	def listarJuguetes(self, idFabrica):
		sonrisas, juguetes = self.verJuguetesMaximos(idFabrica)
		if sonrisas != None:
			juguetes.sort()
			print "Total: " + str(sonrisas) + " sonrisas"
			for juguete in juguetes:
				print juguete

	def graficarRutas(self, idFabrica):
		# Recorrido
		idEsquina = self.fabricas[idFabrica][0]
		camino = self.mapa.recorridoMinimo(self.idPoloNorte, idEsquina)
		
		nombreArchivo = str(idFabrica) + ".kml"
		encoding = "?xml version=\"1.0\" encoding=\"UTF-8\"?"
		kml = "xmlns=\"http://www.opengis.net/kml/2.2\""
		archivo = open(nombreArchivo, 'w')
		archivo.write("<" + encoding + ">" + "\n")
		archivo.write("<kml " + kml + ">" + "\n")
		archivo.write("<Document> \n")

		latInicial = self.esquinas[camino[0]][2]
		longInicial = self.esquinas[camino[0]][3]
		# Punto inicial
		escribirPunto(archivo, "Polo norte", "Comienzo del recorrido",latInicial, longInicial)

		# Escribo el camino
		archivo.write("<Placemark>" + "\n")
		archivo.write("\t<name> " + "Recorrido" + "</name>" + "\n")
		archivo.write("\t<description> " + "Recorrido hasta fabrica " + str(idFabrica) + "</description>" + "\n")
		archivo.write("\t\t<LineString>\n")
		archivo.write("\t\t\t<coordinates>\n")
		i = 0
		for esquina in camino:
			# 2 y 3
			archivo.write("\t\t\t\t" + self.esquinas[esquina][2] + "," + self.esquinas[esquina][3] + "\n")
		archivo.write("\t\t\t</coordinates>\n")
		archivo.write("\t\t</LineString>\n")
		archivo.write("</Placemark>" + "\n")
		ultimo = camino[len(camino) - 1]
		latFinal = self.esquinas[ultimo][2]
		longFinal = self.esquinas[ultimo][3]
		escribirPunto(archivo, "Fabrica " + str(idFabrica), "Fin del recorrido",latFinal, longFinal)

		archivo.write("</Document> \n")
		archivo.write("</kml>" + "\n")
		archivo.close()
		print "OK"

def escribirPunto(archivo, nombre, descripcion, latitud, longitud):
	archivo.write("<Placemark>" + "\n")
	archivo.write("\t<name> " + nombre + "</name>" + "\n")
	archivo.write("\t<description> " + descripcion + "</description>" + "\n")
	archivo.write("\t<Point>" + "\n")
	archivo.write("\t\t<coordinates>" + latitud + "," + longitud + "</coordinates>" +"\n")
	archivo.write("\t</Point>" + "\n")
	archivo.write("</Placemark>" + "\n")

			
def formatear(horario):
	horas = horario / 60
	minutos = horario % 60
	sufH = "" # Los suf(x) son usados para rellenar un 0 en caso de que la hora no de
	sufM = ""
	if (horas / 10) == 0:
		sufH = "0"
	if (minutos / 10) == 0:
		sufM = "0"
	return sufH + str(horas) + ":" + sufM + str(minutos)

#EOF
