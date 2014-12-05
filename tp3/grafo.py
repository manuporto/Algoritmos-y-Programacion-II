class Grafo(object):
    def __init__(self):
    """Crea un grafo implementado como lista de adyacencias.
    self.adyacencias es un diccionario de diccionarios donde
    cada clave es el id del vertices y su dato es otro diccionario
    con los ids de los vertices adyacentes a el."""
        self.adyacencias = {}

    def agregar_vertice(self, vertice):
    """(vertice) -> None
    Pre: el grafo fue creado.
    Post: se le agrega un nuevo vertice al grafo."""
        if vertice in self.adyacencias:
            return
        self.adyacencias[vertice] = {}

    def agregar_arista(self, vertice1, vertice2):
    """(vertice1, vertice2) -> None
    Pre: el grafo fue creado, ambos vertices existen en el grafo.
    Post: se agrega una arista que une a ambos vertices."""
        if vertice1 in self.adyacencias[vertice2]:
            return
        self.adyacencias[vertice1][vertice2] = 1
        self.adyacencias[vertice2][vertice1] = 1

    def eliminar_vertice(self, vertice):
    """(vertice) -> None
    Pre: el grafo fue creado y el vertice existe en el grafo.
    Post: se elimina el vertice y sus aristas con otros vertices."""
        for vertice_b in self.adyacencias[vertice].keys():
            del self.adyacencias[vertice_b][vertice]

        del self.adyacencias[vertice]

    def eliminar_arista(self, vertice1, vertice2):
    """(vertice1, vertice2) -> None
    Pre: el grafo fue creado y ambos vertices existen en el grafo.
    Post: La arista que unia a ambos vertices fue eliminada."""
        del self.adyacencias[vertice1][vertice2]
        del self.adyacencias[vertice2][vertice1]

    def obtener_adyacencias(self, vertice):
    """(vertice) -> lista 
    Pre: el grafo fue creado y el vertice existe en el grafo.
    Post: devuelve una lista con los vertices adyacentes al grafo."""
        if vertice in self.adyacencias:
            return self.adyacencias[vertice].keys()
        return []

    def obtener_vertices(self):
    """ (None) -> lista
    Pre: el grafo fue creado.
    Post: devuelve una lista con todos los vertices del grafo."""
        return self.adyacencias.keys()

    def estan_conectados(self, vertice1, vertice2):
    """ (vertice1, vertice2) -> bool
    Pre: el grafo fue creado y ambos vertices existen en el grafo.
    Post: devuelve True si los vertices son adyacentes, False en 
    caso contrario."""
        return vertice2 in self.adyacencias[vertice1]

    def esta_en_grafo(self, vertice):
    """ (vertice) -> bool
    Pre: el grafo fue creado.
    Post: devuelve True si el vertice esta en el grafo, False en
    caso contrario."""
        return vertice in self.adyacencias.keys()
