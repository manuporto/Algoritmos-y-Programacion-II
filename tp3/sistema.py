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
        for i in range(cantidad):
            print musicos_rec[i][0]

    def difundir(self, musicos):
        '''(lista de musicos) -> lista de musicos que se enteraran de la
        informacion difundida.
        '''
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
                    if not amigo in posibles_informados and amigo not in informados:
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
    '''
    Pre: el grafo fue creado y el vertice pertenece al grafo.
    Post: devuelve una lista con el subgrupo al que pertenece el vertice.
    '''
    subgrupo = []
    padre, distancias = bfs(grafo, vertice)
    for v in distancias:
        if distancias[v] < float("inf"):
            subgrupo.append(v)
    return subgrupo


def bfs(grafo, s):
    visitados = []
    padre = {}
    d = {}
    for v in grafo.obtener_vertices():
        d[v] = float("inf")
        padre[v] = None

    visitados.append(s)
    d[s] = 0
    cola = Queue.Queue()
    cola.put(s)

    while not cola.empty():
        u = cola.get()
        for v in grafo.obtener_adyacencias(u):
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
