/*
 * =====================================================================================
 *  Archivo:  sistema.c
 *  Description:  Archivo de sistema del tp2
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
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

void imprimir_error(int codigo){
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

// Analiza un mensaje en busca de palabras destacadas (palabras que comiencen
// con "@" o "#") En caso de encontrarlas las guarda en una cola.
// Pre: usuario y mensaje no son cadenas vacias.
// Post: devuelve una cola con las palabras destacadas.
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
// Post: devuelve un puntero a un tweet. En caso de error devuelve NULL.
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
	tweet_t *tweet = malloc(sizeof(tweet_t));
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
static int tweet_cmp_t(const void *a, const void *b){
    const tweet_t *tweet1 = a;
    const tweet_t *tweet2 = b;
    if(tweet1->favs > tweet2->favs) return 1;
    else if(tweet1->favs == tweet2->favs) return 0;
    return -1;
}

// Destruye el tweet.
// Pre: el tweet fue creado.
static void tweet_destruir(void *a){
	tweet_t *tweet = a;
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

    sistema->hash = hash_crear((hash_destruir_dato_t)vector_destruir);
    if(!sistema->hash){
        free(sistema);
        return NULL;
    }

    sistema->tweets = vector_generico_crear(10);
    if(!sistema->tweets){
        hash_destruir(sistema->hash);
        free(sistema);
        return NULL;
    }

    return sistema;
}

void sistema_twittear(sistema_t *sistema, char *nombre, char *mensaje){
    if(!nombre || !mensaje){
        imprimir_error(1);
        return;
    }

    if(strlen(mensaje) > 150){
        imprimir_error(3);
        return;
    }
    
    tweet_t *tweet = tweet_crear(sistema, nombre, mensaje);
    vector_generico_guardar(sistema->tweets, tweet->id, tweet);

    cola_t *cola = analizar_tweet(nombre, mensaje);
    // Como mínimo la cola tiene al usuario que realizó el tweet
    char *palabra_destacada = cola_desencolar(cola);
    while(palabra_destacada){
        if(hash_pertenece(sistema->hash, palabra_destacada)){
            vector_t *tweets_asociados = hash_obtener(sistema->hash, palabra_destacada);
            size_t pos = vector_obtener_cantidad(tweets_asociados);
            // El id del tweet mas reciente se inserta al final del vector.
            vector_guardar(tweets_asociados, pos, tweet->id);
        }else{
            vector_t *vector = vector_crear(10);
            vector_guardar(vector, 0, tweet->id);
            hash_guardar(sistema->hash, palabra_destacada, vector);
        }
        palabra_destacada = cola_desencolar(cola);
    }
    cola_destruir(cola, NULL);
    printf("OK %zu\n", tweet->id);
}

void sistema_favorito(sistema_t *sistema, char *id){
    char *id_c = strtok(id, " ");
    char *extra = strtok(NULL, "");
    // Si hay palabras de mas, se imprime el error de comando invalido.
    if(extra){
        imprimir_error(1);
        return;
    }

    long id_l = strtol(id_c, NULL, 10);
    if(id_l > vector_obtener_cantidad(sistema->tweets) || id_l < 0){
        imprimir_error(2);
        return;
    }
    tweet_t *tweet_favorito = vector_generico_obtener(sistema->tweets, id_l);
    tweet_favorito->favs++;
    printf("OK %lu\n", id_l);
}

void sistema_buscar(sistema_t *sistema, char *buscado, char *orden, char *cantidad){
    const char cronologico[] = "cronologico";
    const char popular[] = "popular";
    if(!orden || !cantidad || (strcmp(orden, cronologico) && strcmp(orden, popular))){
        imprimir_error(1);
        return;
    }

    bool e_tweets = hash_pertenece(sistema->hash, buscado);
    if(!e_tweets){
        printf("OK 0\n");
        return;
    }

    long cant_mostrar = strtol(cantidad, NULL, 10);
    vector_t *tweets_ids = hash_obtener(sistema->hash, buscado);
    long cant_tweets = vector_obtener_cantidad(tweets_ids);
    // Si la cantidad a mostrar es 0 o si es mayor que los tweets existentes
    // referidos a buscado, se imprimen todos los tweets.
    if(cant_mostrar == 0 || cant_mostrar > cant_tweets)
        cant_mostrar = cant_tweets;
    printf("OK %lu\n", cant_mostrar);

    if(!strcmp(orden, cronologico)){
        int id;
        char *usuario;
        char *tweet;
        for(int i = cant_tweets - 1; i >= cant_tweets - cant_mostrar; i--){
            // Se van mostrando los tweets del final hacia el inicio.
            vector_obtener(tweets_ids, i, &id);
            tweet_t *actual = vector_generico_obtener(sistema->tweets, id); 
            usuario = actual->usuario;
            tweet = actual->mensaje;
            printf("%i %s %s\n", id, usuario, tweet);
        }
    }
    else if(!strcmp(orden, popular)){
        // Se van obteniendo los ids y luego se busca en sistema->tweets
        // el tweet correspondiente a ese id. Luego se guarda el tweet en el
        // vector tweets.
        void **tweets = malloc(sizeof(tweet_t) * cant_tweets);
        for(size_t i = 0; i < cant_tweets; i++){
            int id;
            vector_obtener(tweets_ids, i, &id);
            tweet_t *tweet = vector_generico_obtener(sistema->tweets, id);
            tweets[i] = tweet;
        }

        // Se crea un heap, de maximo, de los tweets y se le van desencolando
        // los tweets con mayor cantidad de favoritos.
        heap_t *tweets_ordenados = heapify(tweets, cant_tweets, tweet_cmp_t);
        for(size_t i = 0; i < cant_mostrar && 
                !heap_esta_vacio(tweets_ordenados); i++){
            tweet_t *tweet_actual = heap_desencolar(tweets_ordenados);
            printf("%zu %s %s\n", tweet_actual->id, tweet_actual->usuario, 
                    tweet_actual->mensaje);
        }

        heap_destruir(tweets_ordenados, NULL);
    }
}

void sistema_destruir(sistema_t *sistema){
    hash_destruir(sistema->hash);
    vector_generico_destruir(sistema->tweets, tweet_destruir);
    free(sistema);
}
