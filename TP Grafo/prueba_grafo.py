
import Grafo
from Grafo import *


def printTest(prueba, mensaje ,ok):
	estaOk = "OK" if ok else "ERROR"
	print prueba + ", " + mensaje + ": " + estaOk
	

def pruebaGrafoVacio():
	print "\n\n"
	prueba = "Prueba Grafo vacio"
	grafo = Grafo()
	a1 = 1
	a2 = 2
	printTest(prueba, "borrar vertice inexistente" , not grafo.borrarVertice(a1))
	printTest(prueba, "agregar arista mal", not grafo.agregarArista(a1,a2, 0))
	printTest(prueba, "listar aristas", None == grafo.obtenerAristas(a1))
	printTest(prueba, "hay arista", not grafo.hayArista(a1, a2))


def pruebaGrafoVertices():
	print "\n\n"
	prueba = "Prueba Grafo vertices"
	grafo = Grafo()
	v1 = 1
	v2 = 2
	v3 = 3
	v4 = 4
	printTest(prueba, "agregar vertice v1", grafo.agregarVertice(v1))
	printTest(prueba, "agregar vertice v1 otra vez",not grafo.agregarVertice(v1))
	printTest(prueba, "agregar vertice v2", grafo.agregarVertice(v2))
	printTest(prueba, "agregar vertice v3", grafo.agregarVertice(v3))
	printTest(prueba, "agregar vertice v4", grafo.agregarVertice(v4))

	# Recorrido a,d
	printTest(prueba, "distancia minima <v1, v2>", grafo.distanciaMinima(v1,v2) == grafo.infinito)
	lista = grafo.recorridoMinimo(v1,v2)	
	printTest(prueba, "recorrido bien devuelto", len(lista) == 0)

	printTest(prueba, "no hay arista entre v1 y v2", not grafo.hayArista(v1,v2))
	printTest(prueba, "agregar arista entre v1 y v2", grafo.agregarArista(v1,v2, 13))
	printTest(prueba, "hay arista entre v1 y v2", grafo.hayArista(v1,v2))
	printTest(prueba, "peso de arista entre v1 y v2", grafo.obtenerPeso(v1,v2) == 13)
	printTest(prueba, "hay arista entre v1 y v2", grafo.hayArista(v1,v2))
	printTest(prueba, "reemplazar arista entre v1 y v2", grafo.agregarArista(v1,v2, 2))
	printTest(prueba, "peso de arista entre v1 y v2", grafo.obtenerPeso(v1,v2) == 2)
	printTest(prueba, "agregar arista entre v2 y v3", grafo.agregarArista(v2, v3, 15))
	printTest(prueba, "hay arista entre v2 y v3", grafo.hayArista(v2,v3))
	printTest(prueba, "peso de arista entre v2 y v3", grafo.obtenerPeso(v2,v3) == 15)
	printTest(prueba, "no hay arista entre v3 y v4",not grafo.hayArista(v3,v4))

def pruebaGrafoBorrarVertices():
	print "\n\n"
	prueba = "Prueba Grafo borrar vertices"
	grafo = Grafo()
	a = "a"
	b = "b"
	c = "c"
	d = "d"
	e = "e"
	f = "f"
	printTest(prueba, "agregar vertice a", grafo.agregarVertice(a))
	printTest(prueba, "agregar vertice b", grafo.agregarVertice(b))
	printTest(prueba, "agregar vertice c", grafo.agregarVertice(c))
	printTest(prueba, "agregar vertice d", grafo.agregarVertice(d))
	printTest(prueba, "agregar vertice e", grafo.agregarVertice(e))
	printTest(prueba, "agregar vertice f", grafo.agregarVertice(f))
	printTest(prueba, "agregar arista <a,b>", grafo.agregarArista(a,b,2))
	printTest(prueba, "agregar arista <b,c>", grafo.agregarArista(b,c,3))
	printTest(prueba, "agregar arista <a,f>", grafo.agregarArista(a,f,1))
	printTest(prueba, "agregar arista <b,f>", grafo.agregarArista(b,f,2))
	printTest(prueba, "agregar arista <f,c>", grafo.agregarArista(f,c,3))
	printTest(prueba, "agregar arista <f,e>", grafo.agregarArista(f,e,1))
	printTest(prueba, "agregar arista <a,c>", grafo.agregarArista(a,c,5))
	printTest(prueba, "agregar arista <c,e>", grafo.agregarArista(c,e,1))
	printTest(prueba, "agregar arista <c,d>", grafo.agregarArista(c,d,5))
	printTest(prueba, "agregar arista <e,d>", grafo.agregarArista(e,d,2))
	printTest(prueba, "borrar arista <a,b>", grafo.borrarArista(a,b))
	printTest(prueba, "volver a borrar arista <a,b>", not grafo.borrarArista(a,b))
	printTest(prueba, "no hay arista entre <a,b>", not grafo.hayArista(a,b))
	printTest(prueba, "borrar vertice c", grafo.borrarVertice(c))
	printTest(prueba, "no hay arista con <a,c>", not grafo.hayArista(a,c))
	printTest(prueba, "no hay arista con <e,c>", not grafo.hayArista(e,c))
	printTest(prueba, "no hay arista con <b,c>", not grafo.hayArista(b,c))
	

def pruebaRecorridoConBorrado():
	print "\n\n"
	prueba = "Prueba Grafo recorrido con borrado de vertices"
	grafo = Grafo()
	a = "a"
	b = "b"
	c = "c"
	d = "d"
	e = "e"
	f = "f"
	printTest(prueba, "agregar vertice a", grafo.agregarVertice(a))
	printTest(prueba, "agregar vertice b", grafo.agregarVertice(b))
	printTest(prueba, "agregar vertice c", grafo.agregarVertice(c))
	printTest(prueba, "agregar vertice d", grafo.agregarVertice(d))
	printTest(prueba, "agregar vertice e", grafo.agregarVertice(e))
	printTest(prueba, "agregar vertice f", grafo.agregarVertice(f))
	printTest(prueba, "agregar arista <a,b>", grafo.agregarArista(a,b,2))
	printTest(prueba, "agregar arista <b,c>", grafo.agregarArista(b,c,3))
	printTest(prueba, "agregar arista <a,f>", grafo.agregarArista(a,f,1))
	printTest(prueba, "agregar arista <b,f>", grafo.agregarArista(b,f,2))
	printTest(prueba, "agregar arista <f,c>", grafo.agregarArista(f,c,3))
	printTest(prueba, "agregar arista <f,e>", grafo.agregarArista(f,e,1))
	printTest(prueba, "agregar arista <a,c>", grafo.agregarArista(a,c,6))
	printTest(prueba, "agregar arista <c,e>", grafo.agregarArista(c,e,1))
	printTest(prueba, "agregar arista <c,d>", grafo.agregarArista(c,d,5))
	printTest(prueba, "agregar arista <e,d>", grafo.agregarArista(e,d,2))

	grafo.mostrar()

	# Recorrido a,d
	printTest(prueba, "distancia minima <a,d>", grafo.distanciaMinima(a,d) == 4)
	lista = grafo.recorridoMinimo(a,d)
	ok = lista[0] == a and lista[1] == f and lista[2] == e and lista[3] == d
	printTest(prueba, "recorrido bien devuelto", ok)

	# Recorrido d,a
	printTest(prueba, "distancia minima <d,a>", grafo.distanciaMinima(d,a) == 4)
	lista = grafo.recorridoMinimo(d,a)
	ok = lista[0] == d and lista[1] == e and lista[2] == f and lista[3] == a
	printTest(prueba, "recorrido bien devuelto", ok)
	grafo.mostrar()

	printTest(prueba, "borrar vertice f", grafo.borrarVertice(f))
	grafo.mostrar()
	# Recorrido a,d
	printTest(prueba, "distancia minima <a,d>", grafo.distanciaMinima(a,d) == 8)
	lista = grafo.recorridoMinimo(a,d)
	ok = lista[0] == a and lista[1] == b and lista[2] == c and lista[3] == e and lista[4] == d
	printTest(prueba, "recorrido bien devuelto", ok)


def pruebaGrafoRecorrido():
	print "\n\n"
	prueba = "Prueba Grafo recorrido distantcia minima (Dijkstra)"
	a = "a"
	b = "b"
	c = "c"
	d = "d"
	e = "e"
	f = "f"
	grafo = Grafo()
	printTest(prueba, "agregar vertice a", grafo.agregarVertice(a))
	printTest(prueba, "agregar vertice b", grafo.agregarVertice(b))
	printTest(prueba, "agregar vertice c", grafo.agregarVertice(c))
	printTest(prueba, "agregar vertice d", grafo.agregarVertice(d))
	printTest(prueba, "agregar vertice e", grafo.agregarVertice(e))
	printTest(prueba, "agregar vertice f", grafo.agregarVertice(f))
	printTest(prueba, "agregar arista <a,b>", grafo.agregarArista(a,b,2))
	printTest(prueba, "agregar arista <b,c>", grafo.agregarArista(b,c,3))
	printTest(prueba, "agregar arista <a,f>", grafo.agregarArista(a,f,1))
	printTest(prueba, "agregar arista <b,f>", grafo.agregarArista(b,f,2))
	printTest(prueba, "agregar arista <f,c>", grafo.agregarArista(f,c,3))
	printTest(prueba, "agregar arista <f,e>", grafo.agregarArista(f,e,1))
	printTest(prueba, "agregar arista <a,c>", grafo.agregarArista(a,c,5))
	printTest(prueba, "agregar arista <c,e>", grafo.agregarArista(c,e,1))
	printTest(prueba, "agregar arista <c,d>", grafo.agregarArista(c,d,5))
	printTest(prueba, "agregar arista <e,d>", grafo.agregarArista(e,d,2))

	# Recorrido a,d
	printTest(prueba, "distancia minima <a,d>", grafo.distanciaMinima(a,d) == 4)
	lista = grafo.recorridoMinimo(a,d)
	ok = lista[0] == a and lista[1] == f and lista[2] == e and lista[3] == d
	printTest(prueba, "recorrido bien devuelto", ok)

	# Recorrido d,a
	printTest(prueba, "distancia minima <d,a>", grafo.distanciaMinima(d,a) == 4)
	lista = grafo.recorridoMinimo(d,a)
	ok = lista[0] == d and lista[1] == e and lista[2] == f and lista[3] == a
	printTest(prueba, "recorrido bien devuelto", ok)

	# Recorrido a,c
	printTest(prueba, "distancia minimo <a,c>", grafo.distanciaMinima(a,c) == 3)
	lista = grafo.recorridoMinimo(a,c)
	ok = lista[0] == a and lista[1] == f and lista[2] == e and lista[3] == c
	printTest(prueba, "recorrido bien devuelto", ok)

	# Recorrido e,b
	printTest(prueba, "distancia minimo <e,b>", grafo.distanciaMinima(e,b) == 3)
	lista = grafo.recorridoMinimo(e,b)
	ok = lista[0] == e and lista[1] == f and lista[2] == b
	printTest(prueba, "recorrido bien devuelto", ok)

	# Recorrido e,b
	printTest(prueba, "distancia minimo <e,b>", grafo.distanciaMinima(e,b) == 3)
	lista = grafo.recorridoMinimo(e,b)
	ok = lista[0] == e and lista[1] == f and lista[2] == b
	printTest(prueba, "recorrido bien devuelto", ok)

def pruebaGrafoMismoVertice():
	print "\n\n"
	prueba = "Prueba Grafo recorrido mismo vertice"
	grafo = Grafo()
	v1 = 1
	grafo.agregarVertice(v1)
	rec = grafo.recorridoMinimo(v1,v1)
	printTest(prueba, "recorrido minimo <v1,v1>", rec[0] == v1)
	printTest(prueba, "distancia minima <v1, v1>", grafo.distanciaMinima(v1, v1) == 0)




def main():
	pruebaGrafoVacio()
	pruebaGrafoVertices()
	pruebaGrafoBorrarVertices()
	pruebaGrafoRecorrido()
	pruebaRecorridoConBorrado()
	pruebaGrafoMismoVertice()




main()
