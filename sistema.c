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
#define FACT_AGR 2

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

void imprimir_error(size_t codigo){
    switch(codigo){
        case 1:
            printf("ERROR_COMANDO_INVALIDO\n");
            break;
        case 2:
            printf("ERROR_TWIT_ID_INVALIDO\n");
            break;
        case 3:
            printf("ERROR_TWIT_DEMASIADO_LARGO\n");
        default:
            break;
    }
}
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

sistema_t *sistema_crear(){
    sistema_t *sistema = malloc(sizeof(sistema_t));
    if(!sistema) return NULL;

    sistema->hash = hash_crear(NULL);
    if(!sistema->hash){
        free(sistema);
        return NULL;
    }

    // Tire un tamanio random, quizas convenga crearlo con mas capacidad.
    sistema->tweets = vector_generico_crear(10);
    if(!sistema->tweets){
        hash_destruir(sistema->hash);
        free(sistema);
        return NULL;
    }

    return sistema;
}

void sistema_twittear(sistema_t *sistema, char *nombre, char *tweet){
    if(strlen(tweet) > 150){
        imprimir_error(3);
        return;
    }
    
    // ATENCION: No se esta verificando las posibles fallas de estas
    // funciones/primitivas. Hay que ver como tratamos este tema.
    // Falta tratar los hashtags y usuarios en el mensaje!!!
    tweet_t *tweet = tweet_crear(sistema, nombre, tweet);
    vector_generico_guardar(sistema->tweets, tweet->id, tweet);

    if(hash_pertenece(sistema->hash, tweet->usuario)){
        vector_t *tweets_asociados = hash_obtener(sistema->hash, tweet->usuario);
        size_t pos = vector_obtener_cantidad(tweets_asociados);
        size_t tam = vector_obtener_tamanio(tweets_asociados)
        if(tam  == pos){
            vector_redimensionar(tweets_asociados, tam * FACT_AGR);
        }
        vector_guardar(tweets_asociados, pos, tweet->id);
    }else{
        vector_t *vector = vector_crear(10);
        vector_guardar(vector, 0, tweet->id);
        hash_guardar(sistema->hash, tweet->usuario, vector);
    }
}

void sistema_favorito(sistema_t *sistema, char *id){
    if(id > vector_obtener_cantidad(sistema->tweets)){
        imprimir_error(2);
        return;
    } 
    sistema->tweets[id]->favs++;
}

void sistema_buscar(sistema_t *sistema, char *buscado, char *orden, char *cantidad){
    return;
}
