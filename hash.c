/*
 * =====================================================================================
 *  Archivo:  hash.c
 *  Descripcion:  Implementacion de la tabla de hash en C.
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
 * =====================================================================================
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#define TAM_INI 3000
#define FACT_CAR 2
#define FACT_AGR 2
#define FACT_RED 3
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
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter
{
    const hash_t *hash;
    size_t posicion_vector;
    size_t datos_pasados;
    lista_iter_t *iter_interno;
    char *clave_actual;
};

/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES
 *-----------------------------------------------------------------------------*/

static void nodo_destruir(nodo_hash_t *nodo_hash, void destruir_dato(void*));
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

// Devuelve el factor de carga de una tabla de hash.
// Pre: la tabla de hash fue creada.
// Post: devuelve un float que representa el factor de carga del hash.
static float hash_factor_de_carga(const hash_t *hash)
{
    return hash->cantidad/hash->tamanio;
}

// Reubica todos los pares presentes de un hash en uno nuevo. No modifica
// el hash original.
// Pre: tanto hash_viejo como hash_nuevo fueron creados.
static void hash_rehash(const hash_t *hash_viejo, hash_t *hash_nuevo)
{
    hash_iter_t *iter_hash_v = hash_iter_crear(hash_viejo);

    while(!hash_iter_al_final(iter_hash_v))
    {
        // Obtengo la informacion de cada par del hash_viejo.
        const char *clave_act = hash_iter_ver_actual(iter_hash_v);
        void *dato_act = hash_obtener(hash_viejo, clave_act);

        // Guardo la informacion obtenida en el hash_nuevo.
        hash_guardar(hash_nuevo, clave_act, dato_act);
        hash_iter_avanzar(iter_hash_v);
    }
    hash_iter_destruir(iter_hash_v);
}

// Redimensiona el vector de hash.
// Pre: el hash fue creado.
// Post: el vector de hash es redimensionado y sus claves vuelven a pasar
// por la funcion de hash para reubicarse.
static bool hash_redimensionar(hash_t *hash, size_t tam_nuevo)
{
    hash_t *hash_nuevo = malloc(sizeof(hash_t));
    if(!hash_nuevo) return false;

    hash_nuevo->vector = malloc(sizeof(void*) * tam_nuevo);
    if(!hash_nuevo->vector)
    {
        free(hash_nuevo);
        return false;
    } 
    
    hash_nuevo->tamanio = tam_nuevo;
    hash_nuevo->cantidad = 0;
    hash_nuevo->destruir_dato = hash->destruir_dato;
  
    for(size_t i = 0; i < tam_nuevo; i++) 
        hash_nuevo->vector[i] = lista_crear();

     // Llamo a hash_rehash para que reubique todas las claves en la nueva
    // tabla de hash.
    hash_rehash(hash, hash_nuevo);

    for(size_t i = 0; i < hash->tamanio; i++)
    {
        while(!lista_esta_vacia(hash->vector[i]))
        {
            nodo_hash_t *nodo_hash_actual = 
                lista_borrar_primero(hash->vector[i]);
            nodo_destruir(nodo_hash_actual, NULL);
        }
        lista_destruir(hash->vector[i], NULL);
    }
    free(hash->vector);
    hash = memcpy(hash, hash_nuevo, sizeof(hash_t));
    free(hash_nuevo);
    
    return true;
}

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
    if(destruir_dato) destruir_dato(nodo_hash->dato);
    free(nodo_hash);
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL HASH
 *-----------------------------------------------------------------------------*/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t *hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;

    hash->vector = malloc(sizeof(void*) * TAM_INI);
    if(!hash->vector)
    {
        free(hash);
        return NULL;
    }
 
    hash->tamanio = TAM_INI;
    hash->cantidad = 0;
    for(size_t i = 0; i < TAM_INI; i++) hash->vector[i] = lista_crear();
    hash->destruir_dato = destruir_dato;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
    if(hash_factor_de_carga(hash) >= FACT_CAR)
    {
        size_t tam_nuevo = hash->tamanio * FACT_AGR;
        if(!hash_redimensionar(hash, tam_nuevo)) 
            return false;    
    }else if(hash_factor_de_carga(hash) < FACT_CAR/4)
    {
        size_t tam_nuevo = hash->tamanio /  FACT_RED;
        if(!hash_redimensionar(hash, tam_nuevo))
            return false;
    }
    
    size_t codigo = funcion_hash(hash, clave, strlen(clave));

    char *clave_cpy = malloc(sizeof(char)*strlen(clave) + 1);
    if(!clave_cpy) return false;

    strcpy(clave_cpy, clave);
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
    // Si no lo encuentro, inserto el nuevo par a lo ultimo.
    void *dato_rec = hash_borrar(hash, clave);
    if(!dato_rec)
        return lista_insertar_ultimo(hash->vector[codigo], nodo_hash);

    // Si dato no es NULL, significa que existia un par y debo eliminar el
    // dato para luego insertar el nuevo par.
    if(hash->destruir_dato) hash->destruir_dato(dato_rec);
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
            lista_iter_destruir(iter_lista);
            return dato;
        }
        lista_iter_avanzar(iter_lista);
    }
    lista_iter_destruir(iter_lista);
    // Si no encontro nada devuelvo NULL.
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave)
{
    size_t codigo = funcion_hash(hash, clave, strlen(clave));
    if(lista_esta_vacia(hash->vector[codigo])) return NULL;

    lista_iter_t *iter_lista = lista_iter_crear(hash->vector[codigo]);
    while(!lista_iter_al_final(iter_lista))
    {
        nodo_hash_t *nodo_hash_actual = lista_iter_ver_actual(iter_lista);
        if(!strcmp(nodo_hash_actual->clave, clave)){
            lista_iter_destruir(iter_lista);
            return nodo_hash_actual->dato;
        }
        lista_iter_avanzar(iter_lista);
    }
    lista_iter_destruir(iter_lista);
    return NULL;
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
    for(size_t i = 0; i < hash->tamanio; i++)
    {
        while(!lista_esta_vacia(hash->vector[i]))
        {
            nodo_hash_t *nodo_hash_actual = 
                lista_borrar_primero(hash->vector[i]);
            nodo_destruir(nodo_hash_actual, hash->destruir_dato);
        }
        lista_destruir(hash->vector[i], NULL);
    }
    free(hash->vector);
    free(hash);
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR
 *-----------------------------------------------------------------------------*/

hash_iter_t *hash_iter_crear(const hash_t *hash)
{
    hash_iter_t *hash_iter = calloc(sizeof(hash_iter_t), 1);
    if(!hash_iter) return NULL;
    hash_iter->hash = hash;
    size_t i;
    // Avanzo hasta la primera lista que contiene algún elemento
    for(i = 0; i < hash->tamanio; i++)
    {
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

bool hash_iter_avanzar(hash_iter_t *iter)
{
    if(hash_iter_al_final(iter)) return false;
    // Avanzo un lugar en la lista
    lista_iter_avanzar(iter->iter_interno);
    
    // Caso especial en caso de que antes de avanzar esté en el último nodo de la última lista
    if(iter->datos_pasados == iter->hash->cantidad -1)
    {
        iter->clave_actual = NULL;
    }
    // En cualquier otro caso
    else
    {
        // Si el actual es NULL, se terminó la lista y tengo que ir a la siguiente que tenga
        // elementos.
        // También tengo que destruir el iterador interno y crear otro cuando encuentre una nueva
        // lista
        if(!lista_iter_ver_actual(iter->iter_interno))
        {
            lista_iter_destruir(iter->iter_interno);
            size_t i;
            for (i = iter->posicion_vector + 1; i < iter->hash->tamanio; i++)
            {
                if(lista_esta_vacia(iter->hash->vector[i])) continue;
                iter->iter_interno = lista_iter_crear(iter->hash->vector[i]);
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

const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
    if(hash_iter_al_final(iter)) return NULL;
    return iter->clave_actual;
}

bool hash_iter_al_final(const hash_iter_t *iter)
{
    return (iter->datos_pasados == iter->hash->cantidad);
}

void hash_iter_destruir(hash_iter_t* iter)
{
    lista_iter_destruir(iter->iter_interno);
    free(iter);
}
