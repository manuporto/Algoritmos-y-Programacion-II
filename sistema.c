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
#include "cola.h"
#include "vector_dinamico.h"
#include "vector_generico.h"
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

static cola_t *analizar_tweet(char* usuario, char* mensaje){
    char *palabra = strtok(mensaje, " ");
    cola_t *cola = cola_crear();
    cola_encolar(cola, usuario);
    while(palabra){
        if(palabra[0] == '@' || palabra[0] == '#')
            cola_encolar(cola, palabra);
        palabra = strtok(NULL, " ");
    }
    return cola;
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE TWEET
 *-----------------------------------------------------------------------------*/

// Crea un tweet.
// Pre: devuelve un puntero a un tweet. En caso de error devuelve NULL.
static tweet_t *tweet_crear(sistema_t *sistema, char *usuario, char *mensaje){
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

// Compara dos tweets en base a la cantidad de favoritos de cada uno.
// Pre: los dos tweets fueron creados.
// Post: devuelve 1 si los favs de tweet1 son mayores a los de tweet2.
// 0 en caso de ser iguales y -1 si la cantidad de favs de tweet1 es menor
// que la de tweet2.
static size_t tweet_cmp(tweet_t *tweet1, tweet_t *tweet2){
    if(tweet1->favs > tweet2->favs) return 1;
    else if(tweet1->favs == tweet2->favs) return 0;
    return -1;
}

// Destruye el tweet.
static void tweet_destruir(tweet_t *tweet){
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
    tweet_t *tweet = tweet_crear(sistema, nombre, tweet);
    vector_generico_guardar(sistema->tweets, tweet->id, tweet);

    cola_t *cola = analizar_tweet(nombre, tweet);
    // Como mínimo la cola tiene al usuario que realizó el tweet
    char *palabra_destacada = cola_desencolar(cola);
    while(palabra_destacada){
        if(hash_pertenece(sistema->hash, tweet->usuario)){
            vector_t *tweets_asociados = hash_obtener(sistema->hash, tweet->usuario);
            size_t pos = vector_obtener_cantidad(tweets_asociados);
            vector_guardar(tweets_asociados, pos, tweet->id);
        }else{
            vector_t *vector = vector_crear(10);
            vector_guardar(vector, 0, tweet->id);
            hash_guardar(sistema->hash, tweet->usuario, vector);
        }
    }
    printf("OK %zu\n", tweet->id);
}

void sistema_favorito(sistema_t *sistema, char *id){
    long id_l = strtol(id, NULL, 10);
    if(id_l > vector_obtener_cantidad(sistema->tweets) || id_l < 0){
        imprimir_error(2);
        return;
    }
    vector_generico_obtener(sistema->tweets, id_l)->favs++;
    printf("OK %lu\n", id_l)
}

void sistema_buscar(sistema_t *sistema, char *buscado, char *orden, char *cantidad){
    const char cronologico[] = "cronologico";
    const char popular[] = "popular";
    if(strcmp(orden, cronologico) && strcmp(orden, popular)){
        imprimir_error(1);
        return;
    }
    unsigned long cant_mostrar = strtol(cantidad, NULL, 10);
    vector_t *tweets_ids = hash_obtener(sistema->hash, buscado);
    size_t cant_tweets = vector_obtener_cantidad(tweets_ids);
    if(cant_mostrar == 0 || cant_mostrar >= cant_tweets)
        cant_mostrar = cant_tweets;

    printf("OK %zu\n", cant_mostrar);
    if(!strcmp(orden, cronologico)){
        size_t id;
        char *usuario;
        char *tweet;
        for(size_t i = cant_mostrar - 1; i >= 0; i--){
            id = vector_obtener(tweets_ids, i);
            usuario = vector_generico_obtener(sistema->tweets, id)->usuario;
            tweet = vector_generico_obtener(sistema->tweets, id)->mensaje;
            printf("%zu %s %s\n", id, usuario, tweet);
        }
    }
    else if(!strcmp(orden, popular)){
        vector_generico_t *tweets = vector_generico_crear(cant_tweets);
        for(size_t i = 0; i < cant_tweets; i++){
            size_t id = vector_obtener(tweets_id, i);
            tweet_t *tweet = vector_generico_obtener(sistema->tweets, id);
            vector_generico_guardar(tweets, tweet);
        }

        heap_t *tweets_ordenados = heapify(tweets, cant_tweets, tweet_cmp);
        for(size_t i = 0; i < cant_mostrar && 
                !heap_esta_vacio(tweets_ordenados); i++){
            tweet_t *tweet_actual = heap_desencolar(tweets_ordenados);
            printf("%zu %s %s\n", id, usuario, tweet);
        }

        heap_destruir(tweets_ordenados, NULL);
        vector_generico_destruir(tweets, NULL);
        // Crear un vector de tweets (la estructura completa) en el mismo orden
        // que están los IDs. Hacer heapify. Sacar hasta alcanzar la cantidad a
        // mostrar
    }
}
