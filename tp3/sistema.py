from grafo import Grafo
import Queue


class Sistema(object):
    def __init__(self):
        self.grafo = Grafo()

    def inicializar(self, nombre_archivo):
        leer_archivo(self.grafo, nombre_archivo)

    def recomendar(self, musico, cantidad):
        '''(vertice, int) -> lista de contactos recomendados
        Se le recomienda una cantidad de contactos a agregar al musico
        indicado.'''
        # Musico recomendado: amigos en comun
        recomendaciones = {}
        # Recorro los adyacentes a musico. Luego busco los adyacentes a esos y
        # por cada vertice encontrado se lo agrega a recomendaciones. Si ese
        # vertice vuelve a aparecer en otro "adyacente de adyacente", le
        # aumento el valor "amigos en comun"
        for ady in self.grafo.adyacencias[musico]:
            for candidato in self.grafo.adyacencias[ady]:
                if candidato in recomendaciones.keys():
                    recomendaciones[candidato] += 1
                else:
                    recomendaciones[candidato] = 1

        musicos_rec = recomendaciones.items()
        sorted(musicos_rec,
               key = lambda music_rec: musicos_rec[1],
               reverse= True)
        for recomendado in musicos_rec:
            print musicos_rec[0]

    def difundir(self, musicos):
        '''(lista de musicos) -> lista de musicos que se enteraran de la
        informacion difundida.
        '''
        pass

    def centralidad(self, cantidad):
        '''(int) -> lista de vertices  mas centrales'''

    def camino(self, musico1, musico2):
        padre, distancias = bfs(self.grafo, musico1)
        imprimir_camino(padre, musico1, musico2)

    def distancias(self, musico):
        padre, distancias = bfs(self.grafo, musico)
        contar_dist = {}

        for d in distancias.values():
            d_s = str(d)
            if d_s not in contar_dist.keys():
                contar_dist[d_s] = 1
            else:
                contar_dist[d_s] += 1

        for i in xrange(1, len(contar_dist.keys()) - 1):
            print contar_dist[str(i)]

##############################################################################
#   FUNCIONES AUXILIARES
##############################################################################


def bfs(grafo, s):
    visitados = []
    padre = {}
    d = {}
    for v in grafo.vertices:
        d[v] = float("inf")
        padre[v] = None

    visitados.append(s)
    d[s] = 0
    cola = Queue.Queue()
    cola.put(s)

    while not cola.empty():
        u = cola.get()
        for v in grafo.adyacencias[u]:
            if v not in visitados:
                visitados.append(v)
                d[v] = d[u] + 1
                padre[v] = u
                cola.put(v)
    # Todavia no se muy bien que conviene devolver, asi que devuelvo tanto
    # padre como d.

    return padre, d


def leer_archivo(grafo, nombre_archivo):
    archivo = open(nombre_archivo)
    # Se supone que toda la informacion del archivo es correcta. Ej:
    # cant_vertices es en efecto, igual a los vertices presentados, no hay
    # aristas repetidas, etc.
    cant_vertices = archivo.readline()
    for i in xrange(int(cant_vertices)):
        linea_actual = archivo.readline()
        linea_split = linea_actual.split()
        vertice_actual = linea_split[1]
        grafo.agregar_vertice(vertice_actual)

    cant_aristas = archivo.readline()
    for i in xrange(int(cant_aristas)):
        linea_actual = archivo.readline()
        linea_split = linea_actual.split()
        vertice1 = linea_split[1]
        vertice2 = linea_split[2]
        grafo.agregar_arista(vertice1, vertice2)

    archivo.close()


def imprimir_camino(padre, s, v):
    if v == s:
        print s
    elif padre[v] is None:
        print "No hay camino/ERROR"
    else:
        imprimir_camino(padre, s, padre[v])
        print v
