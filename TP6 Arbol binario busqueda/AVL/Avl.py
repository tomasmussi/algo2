


class Avl(object):

	def __init__(self):
		self.raiz = None
		self.cantidad = 0

	def guardar(self, clave, valor):
		print "Guardado de un elemento"

	def borrar(self, clave):
		print "Borrado de la clave"

	def obtener(self, clave):
		print "Obtencion de clave"

	def pertenece(self, clave):
		print "Pertenece"

	def cantidad(self):
		return self.cantidad

	def __str__(self):
		return "Un arbol AVL"


class Nodo(object):

	def __init__(self, clave, valor):
		self.izq = None
		self.der = None
		self.clave = clave
		self.valor = valor

	def __str__(self):
		return "Nodo: " + str(self.clave) + " con valor: " + str(self.valor)