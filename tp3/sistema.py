from grafo import *
import queue


class Sistema(object):
    def __init__(self):
        self.grafo = Grafo()

    def inicializar(self, nombre_archivo):
        leer_archivo(nombre_archivo, self.grafo)

    def recomendar(self, musico, cantidad):
        '''(vertice, int) -> lista de contactos recomendados
        Se le recomienda una cantidad de contactos a agregar al musico
        indicado.'''
        pass

    def difundir(self, musicos):
        '''(lista de musicos) -> lista de musicos que se enteraran de la
        informacion difundida.
        '''
        pass

    def centralidad(self, cantidad):
        '''(int) -> lista de vertices  mas centrales'''


##############################################################################
#   FUNCIONES AUXILIARES
##############################################################################

def bfs(grafo, s):
    cola = queue.Queue()


def leer_archivo(nombre_archivo, grafo):
    archivo = open(nombre)
    # Se supone que toda la informacion del archivo es correcta. Ej:
    # cant_vertices es en efecto, igual a los vertices presentados, no hay
    # aristas repetidas, etc.
    cant_vertices = archivo.readline()
    for i in xrange(cant_vertices):
        linea_actual = archivo.readline()
        linea_split = linea_actual.split()
        vertice_actual = linea_split[1]
        grafo.agregar_vertice(vertice_actual)

    cant_aristas = archivo.readline()
    for i in xrange(cant_aristas):
        linea_actual = archivo_readline()
        linea_split = linea_actual.split()
        vertice1 = linea_split[1]
        vertice2 = linea_split[2]
        grafo.agregar_arista(vertice1, vertice2)

    archivo.close()
