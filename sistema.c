/*
 * =====================================================================================
 *  Archivo:  sistema.c
 *  Description:  Archivo de sistema del tp2
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "sistema.h"

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

struct sistema{
    hash_t *hash;
    vector_generico_t *tweets;
};

typedef struct tweet{
    char *usuario;
    char *mensaje;
    size_t id;
    size_t favs;
}tweet_t;

/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE TWEET
 *-----------------------------------------------------------------------------*/
tweet_t *tweet_crear(sistema_t *sistema, char *usuario, char *mensaje){
	char *usuario_cpy = malloc(sizeof(char) * strlen(usuario) + 1);
	if(!usuario_cpy)
		return NULL;
	strcpy(usuario_cpy, usuario);
	char *mensaje_cpy = malloc(sizeof(char) * strlen(mensaje) + 1);
	if(!mensaje_cpy){
		free(usuario_cpy);
		return NULL;
	}	
	strcpy(mensaje_cpy, mensaje);
	tweet_t *tweet = malloc(sizeof(tweet));
	if(!tweet){
		free(usuario_cpy);
		free(mensaje_cpy);
		return NULL;
	}
	tweet->usuario = usuario_cpy;
	tweet->mensaje = mensaje_cpy;
	tweet->id = vector_generico_obtener_cant(sistema->tweets);
	tweet->favs = 0;
	return tweet;
}

void tweet_destruir(tweet_t *tweet){
	free(tweet->usuario);
	free(tweet->mensaje);
	free(tweet);
}
/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE SISTEMA
 *-----------------------------------------------------------------------------*/
