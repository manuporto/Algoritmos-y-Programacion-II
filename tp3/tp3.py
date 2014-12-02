from sistema import Sistema


def main():
    RECOMENDAR = "recomendar"
    DIFUNDIR = "difundir"
    CENTRALIDAD = "centralidad"
    CAMINO = "camino"
    DISTANCIAS = "distancias"
    SUBGRUPOS = "subgrupos"

    programa = Sistema()
    programa.inicializar("grafo_chico.txt")
    linea = raw_input()

    while linea:
        palabras = linea.split()
        if palabras[0] == RECOMENDAR and len(palabras) == 3:
            programa.recomendar(palabras[1], palabras[2])

        elif palabras[0] == DIFUNDIR and len(palabras) > 1:
            programa.difundir(palabras[1:])

        elif palabras[0] == CENTRALIDAD and len(palabras) == 2:
            programa.centralidad(palabras[1])

        elif palabras[0] == DISTANCIAS and len(palabras) == 2:
            programa.distancias(palabras[1])

        elif palabras[0] == CAMINO:
            musicos = palabras[1].split(",")
            if len(musicos) != 2:
                continue
            programa.camino(musicos[0], musicos[1])

        elif palabras[0] == SUBGRUPOS and len(palabras) == 1:
            programa.subgrupos()

        else:
            print "Aca va un ERROR"
            # Imprimir error de comando.
        linea = raw_input()

if __name__ == '__main__':
    main()
