#coding: utf-8
from grafo import Grafo
import Queue


class Sistema(object):
    def __init__(self):
        self.grafo = Grafo()

    def inicializar(self, nombre_archivo):
        """(str) -> None.
        Completa el grafo del sistema con los vertices y aristas presentes en
        el archivo."""
        leer_archivo(self.grafo, nombre_archivo)

    def recomendar(self, musico, cantidad):
        """(vertice, int) -> lista de contactos recomendados
        Se le recomienda una cantidad de contactos a agregar al musico
        indicado."""
        # Musico recomendado: amigos en comun
        recomendaciones = {}
        # Recorro los adyacentes a musico. Luego busco los adyacentes a esos y
        # por cada vertice encontrado se lo agrega a recomendaciones. Si ese
        # vertice vuelve a aparecer en otro "adyacente de adyacente", le
        # aumento el valor "amigos en comun"
        for ady in self.grafo.obtener_adyacencias(musico):
            for candidato in self.grafo.obtener_adyacencias(ady):
                # Si el musico ya es amigo del candidato, paso al siguiente
                if self.grafo.estan_conectados(musico, candidato):
                    continue
                if candidato in recomendaciones.keys():
                    recomendaciones[candidato] += 1
                else:
                    recomendaciones[candidato] = 1

        musicos_rec = recomendaciones.items()
        musicos_rec.sort(key = lambda musico_rec: musico_rec[1], reverse = True)
        for i in range(1, int(cantidad) + 1):
            print musicos_rec[i][0]

    def difundir(self, musicos):
        """(lista de musicos) -> lista de musicos que se enteraran de la
        informacion difundida."""
        informados = {}
        for musico in musicos:
            if self.grafo.esta_en_grafo(musico):
                informados[musico] = True
        while True:
            cambio = False
            posibles_informados = {}
            for musico in informados:
                amigos_musico = self.grafo.obtener_adyacencias(musico)
                for amigo in amigos_musico:
                    if amigo not in posibles_informados and \
                    amigo not in informados:
                        posibles_informados[amigo] = None
            for candidato in posibles_informados:
                # Esto itera esobre las claves (es lo mismo que agregar .keys())
                contador = 0
                contactos = self.grafo.obtener_adyacencias(candidato)
                long_necesaria = len(contactos)
                for contacto in contactos:
                    if contacto in informados:
                        contador += 1
                    if contador > (long_necesaria / 2):
                        cambio = True
                        informados[candidato] = True
                        continue
            if not cambio:
                break
        print informados.keys()

    def centralidad(self, cantidad):
        """(int) -> lista de vertices  mas centrales
        Hace BFS de cada vertice del grafo, recorre cada camino mínimo y por
        cada aparición de un vértice, le suma 1 a su contador. Imprime los
        vértices con mayor cantidad de ocurrencias en caminos mínimos."""
        try:
            int(cantidad)
        except:
            raise RuntimeError("(!)La cantidad debe ser un número entero")
        if int(cantidad) < 1:
            raise RuntimeError("(!)La cantidad debe ser un número positivo")
        contador = {}
        for vertice in self.grafo.obtener_vertices():
            contador[vertice] = 0
        for vertice in contador:
            padre, distancias = bfs(self.grafo, vertice)
            for alcanzado in distancias:
                vertice_actual = alcanzado
                while padre[vertice_actual] != vertice and \
                padre[vertice_actual] != None:
                    contador[padre[vertice_actual]] += 1
                    vertice_actual = padre[vertice_actual]
        devolucion = contador.items()
        devolucion.sort(key = lambda contador:contador[1], reverse = True)
        for x in range(int(cantidad)):
            print devolucion[x][0]


    def camino(self, musico1, musico2):
        """(musico1, musico2) -> None
        Imprime el camino entre dos musicos."""
        padre, distancias = bfs(self.grafo, musico1)
        imprimir_camino(padre, musico1, musico2)

    def distancias(self, musico):
        """ (musico) -> None
        Imprime la cantidad de musicos que se encuentran a distancia 
        1, 2, ..., n del musico."""
        padre, distancias = bfs(self.grafo, musico)
        contar_dist = {}

        for musico, dist in distancias.items():
            if dist == float("inf") or dist == 0:
                continue
            dist_s = str(dist)
            if dist_s not in contar_dist:
                contar_dist[dist_s] = [musico]
            else:
                contar_dist[dist_s].append(musico)

        for distancia in contar_dist:
            print distancia + ": ",
            for musico in contar_dist[distancia]:
                print musico,
            print

    def subgrupos(self):
        """Pre: -
        Post: imprime en orden decreciente el tamanio de los subgrupos
        presentes en el grafo.
        """
        vertices_faltantes = self.grafo.obtener_vertices()
        largo_subgrupos = []

        while vertices_faltantes:
            subgrupo = crear_subgrupo(self.grafo, vertices_faltantes[0])
            largo_subgrupos.append(len(subgrupo))
            for v in subgrupo:
                vertices_faltantes.remove(v)

        largo_subgrupos.sort(reverse = True)
        for subgrupo in largo_subgrupos:
            print subgrupo

##############################################################################
#   FUNCIONES AUXILIARES
##############################################################################


def crear_subgrupo(grafo, vertice):
    """(grafo, vertice) -> lista
    Pre: el grafo fue creado y el vertice pertenece al grafo.
    Post: devuelve una lista con el subgrupo al que pertenece el vertice."""
    subgrupo = []
    padre, distancias = bfs(grafo, vertice)
    for v in distancias:
        if distancias[v] < float("inf"):
            subgrupo.append(v)
    return subgrupo


def bfs(grafo, origen, destino = None):
    """(grafo, origen, destino) -> lista de padres, diccionario
    de distancias a origen de cada vertice.
    Realiza una busqueda por anchur del grafo."""
    visitados = {}
    padre = {}
    distancia = {}
    for vertice in grafo.obtener_vertices():
        distancia[vertice] = float("inf")
        padre[vertice] = None

    visitados[origen] = True
    distancia[origen] = 0
    cola = Queue.Queue()
    cola.put(origen)

    while not cola.empty():
        if destino is not None and destino in visitado:
            break
        actual = cola.get()
        for vecino in grafo.obtener_adyacencias(actual):
            if vecino not in visitados:
                visitados[vecino] = True
                distancia[vecino] = distancia[actual] + 1
                padre[vecino] = actual
                cola.put(vecino)

    return padre, distancia


def leer_archivo(grafo, nombre_archivo):
    """(str) -> None
    Lee la informacion de un archivo y agrega los vertices o aristas
    al grafo cuando corresponde."""
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
    """ (dicc, origen, destino) -> None
    Imprime el camino entre un vertice de origen y uno de
    destino."""
    if v == s:
        print s
    elif padre[v] is None:
        raise RuntimeError("(!)No hay un camino que conecte a estos músicos")
    else:
        imprimir_camino(padre, s, padre[v])
        print v
