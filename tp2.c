/*
 * =====================================================================================
 *
 *       Filename:  tp2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/11/14 11:21:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "lectura.h"
#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    const char twittear[] = "twittear";
    const char favorito[] = "favorito";
    const char buscar[] = "buscar";

    char *linea = leer_linea();
    sistema_t *sistema = sistema_crear();
    while(!feof(stdin)){
        char *comando = strtok(linea, " ");
        if(!strcmp(twittear, comando)){
            char *nombre = strtok(NULL, " ");
            char *twit = strtok(NULL, "");
            sistema_twittear(sistema, nombre, twit);
        }else if(!strcmp(favorito, comando)){
            char *id = strtok(NULL, "");
            sistema_favorito(sistema, id);
        }else if(!strcmp(buscar, comando)){
            char *buscado = strtok(NULL, " ");
            char *orden = strtok(NULL, " ");
            char *cantidad = strtok(NULL, "");
            sistema_buscar(sistema, buscado, orden, cantidad);
        }
        else{
            imprimir_error(1);
        }
        free(linea);
        linea = leer_linea();
    }

    free(linea);
    sistema_destruir(sistema);
    return 0;

}
