class Grafo(object):
    def __init__(self, adyacencias=None):
        self.adyacencias = {}

    def agregar_vertice(self, vertice):
        self.adyacencias[vertice] = {}

    def agregar_arista(self, vertice1, vertice2):
        self.adyacencias[vertice1][vertice2] = 1
        self.adyacencias[vertice2][vertice1] = 1

    def es_alcanzable(self, vertice1, vertice2):
        # Como es un grafo no dirigido solo verifico que vertice2 este en el
        # dicc de vertice1
        if vertice2 in self.adyacencias[vertice1]:
            return True
        # Si no cumple lo anterior habria que hacer un BFS para ver si esta no?
        # Y ver si alguno de los caminos desde vertice1 conduce a vertice2.

    def eliminar_vertice(self, vertice):
        for vertice_b in self.adyacencias[vertice].keys():
            del self.adyacencias[vertice_b][vertice]

        del self.adyacencias[vertice]

    def eliminar_arista(self, vertice1, vertice2):
        del self.adyacencias[vertice1][vertice2]
        del self.adyacencias[vertice2][vertice1]
