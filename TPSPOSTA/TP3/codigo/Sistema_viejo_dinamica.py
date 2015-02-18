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
		# Ordeno por cierre, apertura y id
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
		indiceLongitud = str(distancia).index(".")
		print "Distancia: " + str(distancia)[:indiceLongitud] + " Metros"
		for esquina in camino:
			# 2 y 3
			print self.esquinas[esquina][2] + "," + self.esquinas[esquina][3]

	# Formato de fabricas: fabricasVisitar[idFabrica] = [idEsquina, apertura, cierre, [] ]
	def miVerJuguetesMaximos(self, idFabrica, validar=True):
		juguetes = []
		if idFabrica == None:
			#Todos los juguetes de las fabricas a visitar
			for fabrica in self.fabricasVisitar:
				if len(self.fabricas[fabrica][3]) > 0:
					for l in self.fabricas[fabrica][3]:
						juguetes.append(l)
		else:
			#Juguetes para la fabrica con id idFabrica
			juguetes = self.fabricas[idFabrica][3]
		if idFabrica not in self.fabricasVisitar and validar:
			print "Error: La fabrica con id " + str(idFabrica) + " no existe"
			return
		#Armo la solucion optima en una matriz (dinamica)
		solVieja = []
		filas = len(juguetes) + 1
		columnas = self.capacidad + 1
		for i in range(columnas):
			solVieja.append(0)
		solNueva = []
		for fila in range(filas):
			solNueva = list()
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
					else:
						solNueva.append(0)
				solVieja = []
				for i in range(len(solNueva)):				
					solVieja.append(solNueva[i])

		solucion = solNueva[self.capacidad]
		return solucion

	""" FUNCION QUE ANDA BIEN!!!!!!!!!!!!!!!!!!!
	# Formato de fabricas: fabricasVisitar[idFabrica] = [idEsquina, apertura, cierre, [] ]
	def miVerJuguetesMaximos(self, idFabrica, validar=True):
		juguetes = []
		if idFabrica == None:
			#Todos los juguetes de las fabricas a visitar
			for fabrica in self.fabricas:
				if len(self.fabricas[fabrica][3]) > 0:
					for l in self.fabricas[fabrica][3]:
						juguetes.append(l)
		else:
			#Juguetes para la fabrica con id idFabrica
			juguetes = self.fabricas[idFabrica][3] 
		if idFabrica not in self.fabricasVisitar and validar:
			print "Error: La fabrica con id " + str(idFabrica) + " no existe"
			return
		#Armo la solucion optima en una matriz (dinamica)
		mat = []
		filas = len(juguetes) + 1
		columnas = self.capacidad + 1
		mat = matriz(filas, columnas)
		for fila in range(filas):
			if fila != 0:
				#print juguetes[fila - 1]
				idj = juguetes[fila - 1][0]
				valor = juguetes[fila - 1][1]
				peso = juguetes[fila - 1][2]			
				for col in range(columnas):
					if col != 0:
						# Solucion si dejo intacta la mochila
						sol1 = mat[fila - 1][col]

						# Solucion si saco un peso del actual juguete y le sumo el nuevo valor
						sol2 = 0
						if col - peso >= 0:
							sol2 = mat[fila - 1][col - peso] + valor
						mat[fila][col] = max(sol1, sol2)
		solucion = mat[len(juguetes)][self.capacidad]
		
	"""

	def valuarJuguetesTotal(self):
		total = 0
		for i in self.fabricasVisitar:
			total += self.miVerJuguetesMaximos(i)
		print "Total: " + str(total) + " sonrisas"
	
	def valuarJuguetes(self, idFabrica):
		print "Total: " + str(self.miVerJuguetesMaximos(idFabrica)) + " sonrisas"

	def graficarRutas(self, idFabrica):
		# Recorrido
		camino = self.mapa.recorridoMinimo(self.idPoloNorte, idEsquina)
		
		nombre = "Fabrica"
		encoding = "?xml version=\"1.0\" encoding=\"UTF-8\"?"
		kml = "xmlns=\"http://www.opengis.net/kml/2.2\""
		archivo = open('workfile.kml', 'r+')
		archivo.write("<" + encoding + ">")
		archivo.write("<kml " + kml + ">")
		archivo.write("<Placemark>")
		archivo.write("<name> " + nombre + "</name>")
		archivo.write("<description> " + descripcion + "</description>")
		for esquina in camino:
			# 2 y 3
			archivo.write("<Point>")
			archivo.write("<coordinates>")
			archivo.write(self.esquinas[esquina][2] + "," + self.esquinas[esquina][3])
			archivo.write("</coordinates>")
			print self.esquinas[esquina][2] + "," + self.esquinas[esquina][3]
			archivo.write("</Point>")
		archivo.write("</Placemark>")
		archivo.write("</kml")
		archivo.close()
		
		

	""" CODIGO NICO"""
	def valuarJuguetesNICO(self, idFabrica):
		juguetesParaListar = self.verMaximosJuguetes(idFabrica)
		sonrisasTotal = 0
		for juguete in juguetesParaListar:
			sonrisasTotal += juguete[1]
		print "Total: " + str(sonrisasTotal) + " sonrisas"
		

	def valuarJuguetesTotalNICO(self):
		juguetesParaListar = self.verMaximosJuguetes(None)
		# Listo los juguetes que se seleccionaron
		juguetesParaListar.sort()
		for juguete in juguetesParaListar:
			print juguete[0]

	

	#return: [juguete1, juguete2,...]
	def verMaximosJuguetes(self, idFabrica):
		juguetes = []
		if (idFabrica == None):
			for fabrica in self.fabricasVisitar:#Todos los juguetes de las fabricas a visitar
				juguetes.append(self.fabricas[fabrica][3])
		else:
			juguetes = self.fabricas[idFabrica][3] #Juguetes para la fabrica con id idFabrica
		#Creo matriz de valores (vacia)
		matriz = {}
		for juguete in juguetes:
			i = 0
			lista = []
			while (i < self.capacidad):
				lista.append([0, [juguete]]) #Inicializo todo en 0
				i += 1
			matriz[juguete[0]] = lista
		#Empiezo a completar valores
		listaAnterior = None
		jugueteAnterior = None
		juguetesParaListar = [] #HAY QUE GUARDAR LOS IDS DE LOS JUGUETES Y NO TENGO NI PUTA IDEA COMO
		for juguete in juguetes:
			lista = matriz[juguete[0]]
			i = 0
			while (i < len(lista)):
				if (juguete[2] == i):
					if (listaAnterior == None):
						lista[i][0] = juguete[1] #Guardo el valor del juguete
						lista[i][1].append(juguete)
					else:
						valorAnterior = listaAnterior[i][0]
						valorNuevo = listaAnterior[i - juguete[2]][0] + juguete[1]
						if (valorNuevo >= valorAnterior):#comparo los valores si agrego o no el juguete
							jugueteAnterior = listaAnterior[i - juguete[2]][1]
							lista[i][0] = valorNuevo
							lista[i][1].append(jugueteAnterior)
						else:
							jugueteAnterior = listaAnterior[i][1]
							lista[i][0] = valorAnterior
							lista[i][1] = jugueteAnterior
				i += 1
			listaAnterior = lista
			
		return juguetesParaListar

	def listarJuguetes(self, idFabrica):		
		juguetesParaListar = self.verMaximosJuguetes(idFabrica)
		# Listo los juguetes que se seleccionaron
		juguetesParaListar.sort()
		for juguete in juguetesParaListar:
			print juguete[0]
	""" FIN CODIGO NICO"""

	#DEBUGGGGGGGGGGGGGGGGGGGGGGGGGGGGG

	def conectados(self, vertice):
		self.mapa.conectados(vertice)

	def rec(self, v1, v2):
		print "desde: " + str(v1) + " hasta: " + str(v2)
		l = self.mapa.recorridoMinimo(v1,v2)
		d = self.mapa.distanciaMinima(v1,v2)
		print "Distancia: " + str(d) + " metros"
		print l

	def most(self):
		self.mapa.mostrar()

	def ar(self, v):
		print self.mapa.obtenerAristas(v)

			
def formatear(horario):
	horas = horario / 60
	minutos = horario % 60
	sufH = "" # Los suf"x" son usados para rellenar un 0 en caso de que la hora no de
	sufM = ""
	if (horas / 10) == 0:
		sufH = "0"
	if (minutos / 10) == 0:
		sufM = "0"
	return sufH + str(horas) + ":" + sufM + str(minutos)

def matriz(filas, columnas):
	matriz = []
	for i in range(filas):
		fila = []
		for j in range(columnas):
			fila.append(0)
		matriz.append(fila)
	return matriz

	
#EOF
