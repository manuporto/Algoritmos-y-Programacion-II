/*
 * =====================================================================================
 *  Archivo:  hash.c
 *  Descripcion:  Implementacion de la tabla de hash en C.
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
 *
 * =====================================================================================
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#define TAM_INI 10000
#define FACT_AGR 2
#define FACT_RED 4
#define VAL_INI 0
#define M 31

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

// Definicion de el par clave/dato que ira en cada posicion de la lista
// enlazada creada en cada posicion del vector.
typedef struct nodo_hash
{
    char *clave;
    void *dato;
}nodo_hash_t;

struct hash
{
    void** vector;
    size_t tamanio;
    size_t cantidad;
    // Revisar el tipo puede que sea hash_destruir_dato_t
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
    hash_t *hash;
    size_t posicion_vector;
    size_t datos_pasados;
    lista_iter_t *iter_interno;
    char *clave_actual;
};



/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES
 *-----------------------------------------------------------------------------*/

// Funcion de hash basada en la K&R
// Post: devuelve un entero que representa el indice del vector donde se 
// guardaran la clave y el valor.
static size_t funcion_hash(const hash_t *hash, const char *clave, int largo)
{
    size_t codigo = VAL_INI;
    for(size_t i = 0; i < largo; ++i)
        codigo = M * codigo + clave[i];
    return codigo % hash->tamanio;
}

// Falta redimensionar.

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE NODO HASH
 *-----------------------------------------------------------------------------*/

// Crea un nodo_hash.
// Post: crea un nodo_hash con una clave y un dato.
static nodo_hash_t *nodo_crear(char *clave, void *dato)
{
    nodo_hash_t *nodo_hash = malloc(sizeof(nodo_hash_t));
    if(!nodo_hash) return NULL;

    nodo_hash->clave = clave;
    nodo_hash->dato = dato;

    return nodo_hash;
}

// Destruye el par clave-dato.
// Pre: el nodo fue creado.
static void nodo_destruir(nodo_hash_t *nodo_hash, void destruir_dato(void*))
{
    free(nodo_hash->clave);
    if(destruir_dato != NULL) destruir_dato(nodo_hash->dato);
    free(nodo_hash);
}

static nodo_hash_t *nodo_buscar(const hash_t *hash, const char *clave)
{
    size_t codigo = funcion_hash(hash, clave, strlen(clave));
    if(lista_esta_vacia(hash->vector[codigo])) return NULL;

    lista_iter_t *iter_lista = lista_iter_crear(hash->vector[codigo]);
    while(!lista_iter_al_final(iter_lista))
    {
        nodo_hash_t *nodo_hash_actual = lista_iter_ver_actual(iter_lista);
        if(!strcmp(nodo_hash_actual->clave, clave))
            return nodo_hash_actual;
    }

    return NULL;
}
/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL HASH
 *-----------------------------------------------------------------------------*/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t *hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;

    // Revisar! Puede que ese +1 este de mas.
    hash->vector = malloc(sizeof(void*)*TAM_INI+1);
    if(!hash->vector)
    {
        free(hash);
        return NULL;
    }
 
    hash->tamanio = TAM_INI;
    hash->cantidad = 0;
    for(size_t i = 0; i <= TAM_INI; i++) hash->vector[i] = lista_crear();
    // Revisar
    hash->destruir_dato = destruir_dato;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
    size_t codigo = funcion_hash(hash, clave, strlen(clave));

    char *clave_cpy = malloc(sizeof(char));
    if(!clave_cpy) return false;

    nodo_hash_t *nodo_hash = nodo_crear(clave_cpy, dato);
    if(!nodo_hash)
    {
        free(clave_cpy);
        return false;
    } 

    hash->cantidad++;
    // Si la lista de la posicion esta vacia, estoy seguro de que no esta.
    if(lista_esta_vacia(hash->vector[codigo])) 
        return lista_insertar_ultimo(hash->vector[codigo], nodo_hash); 

    // Si la lista no esta vacia es posible que ya este guardando otro dato
    // con la misma clave.
    // Si no lo encontro, inserto el nuevo par a lo ultimo.
    void *dato_rec = hash_borrar(hash, clave);
    if(!dato_rec)
        return lista_insertar_ultimo(hash->vector[codigo], nodo_hash);

    // Si dato no es NULL, significa que existia un par y debo eliminar el
    // dato para luego insertar el nuevo par.
    hash->destruir_dato(dato_rec);
    return lista_insertar_ultimo(hash->vector[codigo], nodo_hash);
}

void *hash_borrar(hash_t *hash, const char *clave)
{
    size_t codigo = funcion_hash(hash, clave, strlen(clave));
    // Si la lista esta vacia no existe el par.
    if(lista_esta_vacia(hash->vector[codigo])) return NULL;

    // Si la lista contenia elementos debo verificar si esta o no el par.
    lista_iter_t *iter_lista = lista_iter_crear(hash->vector[codigo]);
    while(!lista_iter_al_final(iter_lista))
    {
        nodo_hash_t *nodo_hash_actual = lista_iter_ver_actual(iter_lista);
        if(!strcmp(nodo_hash_actual->clave, clave))
        {
            nodo_hash_t *nodo_hash_viejo =
                lista_borrar(hash->vector[codigo], iter_lista);
            void *dato = nodo_hash_viejo->dato;
            nodo_destruir(nodo_hash_viejo, NULL);
            hash->cantidad--;
            return dato;
        }
    }

    // Si no encontro nada devuelvo NULL.
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave)
{
    nodo_hash_t *nodo_hash_buscado = nodo_buscar(hash, clave);
    if(!nodo_hash_buscado) return NULL;

    return nodo_hash_buscado->dato;

}

bool hash_pertenece(const hash_t *hash, const char *clave)
{
    return hash_obtener(hash, clave) != NULL;
}

size_t hash_cantidad(const hash_t *hash)
{
    return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
    for(size_t i = 0; i <= hash->tamanio; i++)
    {
        if(lista_esta_vacia(hash->vector[i])) 
        {
            lista_destruir(hash->vector[i], NULL);
            continue;
        }

        while(!lista_esta_vacia(hash->vector[i]))
        {
            nodo_hash_t *nodo_hash_actual = 
                lista_borrar_primero(hash->vector[i]);
            nodo_destruir(nodo_hash_actual, hash->destruir_dato);
        }

    }
}


/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR
 *-----------------------------------------------------------------------------*/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    if(hash_cantidad(hash)) return NULL;
    
    hash_iter_t *hash_iter = malloc(sizeof(hash_iter_t));
    if(!hash_iter) return NULL;
    hash_iter->hash = hash;
    size_t i;
    // Avanzo hasta la primera lista que contiene algún elemento
    for(i = 0; i < hash->tam; i++){
        if(lista_esta_vacia(hash->vector[i])) continue;
        hash_iter->iter_interno = lista_iter_crear(hash->vector[i]);
        nodo_hash_t *dic_actual = (nodo_hash_t*)lista_iter_ver_actual(hash_iter->iter_interno);
        hash_iter->clave_actual = dic_actual->clave;
        break;
    }
    // Guardo el lugar del vector donde me quedé e inicializo en cero la cantidad de datos que pasé
    hash_iter->posicion_vector = i;
    hash_iter->datos_pasados = 0;
    return hash_iter;
}


bool hash_iter_avanzar(hash_iter_t *iter){
    if(hash_iter_esta_al_final(iter)) return false;
    // Avanzo un lugar en la lista
    lista_iter_avanzar(iter->iter_interno);
    
    // Caso especial en caso de que antes de avanzar esté en el último nodo de la última lista
    if(iter->datos_pasados == iter->hash->cantidad -1){
        iter->clave_actual = NULL;
    }
    // En cualquier otro caso
    else{
        // Si el actual es NULL, se terminó la lista y tengo que ir a la siguiente que tenga
        // elementos.
        // También tengo que destruir el iterador interno y crear otro cuando encuentre una nueva
        // lista
        if(!lista_iter_ver_actual(iter->iter_interno)){
            lista_iter_destruir(iter->iter_interno);
            size_t i;
            for (i = iter->posicion_vector + 1; i < iter->hash->tam; i++){
                if(lista_esta_vacia(iter->hash->vector[i])) continue;
                iter->iter_interno = lista_iter_crear(hash->vector[i]);
                break;
            } 
            // Actualizo la posicion del vector
            iter->posicion_vector = i;
        }
    
        nodo_hash_t *dic_actual = (nodo_hash_t*)lista_iter_ver_actual(iter->iter_interno);
        iter->clave_actual = dic_actual->clave;
    }
    iter->datos_pasados += 1;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_esta_al_final(iter)) return NULL;
    return iter->clave_actual;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return (iter->datos_pasados == iter->hash->cantidad);
}

void hash_iter_destruir(hash_iter_t* iter){
    lista_iter_destruir(iter->iter_interno);
    free(iter);
}
