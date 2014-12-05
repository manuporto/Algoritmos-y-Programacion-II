#coding: utf-8
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
        try:
            if palabras[0] == RECOMENDAR:
                if len(palabras) == 3:
                    programa.recomendar(palabras[1], palabras[2])
                else:
                    raise RuntimeError("(!)Los parámetros son incorrectos.\n\
(!)Se espera 'comando musico cantidad'") 

            elif palabras[0] == DIFUNDIR:
                if len(palabras) > 1:
                    programa.difundir(palabras[1:])
                else:
                    raise RuntimeError("(!)Inserte al menos un musico con el \
cual difundir la información")

            elif palabras[0] == CENTRALIDAD:
                if len(palabras) == 2:
                    programa.centralidad(palabras[1])
                else:
                    raise RuntimeError("(!)Debe insertar la cantidad de musico \
populares que desea conocer")

            elif palabras[0] == DISTANCIAS: 
                if len(palabras) == 2:
                    programa.distancias(palabras[1])
                else:
                    raise RuntimeError("(!)Debe insertar un músicos para \
conocer a que distancia se encuentran los demás artistas")

            elif palabras[0] == CAMINO:
                musicos = palabras[1].split(",")
                if len(musicos) != 2:
                    raise RuntimeError("(!)Debe insertar un músico del cual \
partir (primero) y uno al cual llegar (segundo) separados por una coma (,)")
                programa.camino(musicos[0], musicos[1])

            elif palabras[0] == SUBGRUPOS:
                if len(palabras) == 1:
                    programa.subgrupos()
                else:
                    raise RuntimeError("(!)Este comando no recibe parámetros \
extra")
            else:
                raise RuntimeError("(!)Comando desconocido. Por favor ingrese \
alguno de los siguiente comandos:\n\t-recomendar\n\t-difundir\n\t-centralidad\n\
\t-distancias\n\t-camino\n\t-subgrupo")
        except RuntimeError, e:
            print e

        linea = raw_input()

if __name__ == '__main__':
    main()
