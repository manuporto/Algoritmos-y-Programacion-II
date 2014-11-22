/*
 * =====================================================================================
 *  Archivo:  lista.c
 *  Descripcion:  Implementacion de la lista enlazada en C.
 *  Autor:  Manuel Ignacio Porto
 *  Padron:  96587
 * =====================================================================================
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

typedef struct nodo
{
    void *dato;
    struct nodo *siguiente;
}nodo_lista_t;

struct lista
{
    nodo_lista_t *primero;
    nodo_lista_t *ultimo;
    size_t largo;
};

struct lista_iter
{
    nodo_lista_t *actual;
    nodo_lista_t *anterior;
};

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL NODO
 *-----------------------------------------------------------------------------*/

nodo_lista_t *nodo_crear(void *dato)
{
    nodo_lista_t *nodo = malloc(sizeof(nodo_lista_t));
    if(!nodo) return NULL;

    nodo->dato = dato;
    nodo->siguiente = NULL;

    return nodo;
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE LA LISTA
 *-----------------------------------------------------------------------------*/

lista_t *lista_crear()
{
    lista_t *lista = malloc(sizeof(lista_t));
    if(!lista) return NULL;

    lista->primero = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista)
{
    return !lista->primero;
}

bool lista_insertar_primero(lista_t *lista, void *dato)
{
    nodo_lista_t *nodo = nodo_crear(dato);
    if(!nodo) return false;
    
    // Caso borde en el que la lista esta vacia y se le inserta el primer elemento
    // Primero y ultimo son el mismo elemento.
    if(lista_esta_vacia(lista))
    {
        lista->ultimo = nodo;
    }else
    {
        nodo->siguiente = lista->primero;
    }
    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato)
{
    nodo_lista_t *nodo = nodo_crear(dato);
    if(!nodo) return false;

    // Idem caso borde de insertar primero.
    if(lista_esta_vacia(lista))
    {
        lista->primero = nodo;
    }else
    {
        (lista->ultimo)->siguiente = nodo;
    }

    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista)
{
    if(lista_esta_vacia(lista)) return NULL;
    
    void *dato = (lista->primero)->dato;
    nodo_lista_t *nodo_aux = lista->primero;
    lista->primero = (lista->primero)->siguiente;
    free(nodo_aux);
    lista->largo--;

    return dato; 
}

void *lista_ver_primero(const lista_t *lista)
{
    if(lista_esta_vacia(lista)) return NULL;

    return (lista->primero)->dato;
}

size_t lista_largo(const lista_t *lista)
{
    return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *))
{
    void *dato;
    while(!lista_esta_vacia(lista))
       {
           dato = lista_borrar_primero(lista);
           if(destruir_dato)
           {
               destruir_dato(dato);
           }
       }
    free(lista); 
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR EXTERNO
 *-----------------------------------------------------------------------------*/

lista_iter_t *lista_iter_crear(const lista_t *lista)
{
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    if(!iter) return NULL;

    iter->anterior = lista->primero;
    iter->actual = lista->primero;

    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter)
{
    if(lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = (iter->actual)->siguiente;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter)
{
    if(lista_iter_al_final(iter)) return NULL;
    return (iter->actual)->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter)
{
    return !iter->actual;
}

void lista_iter_destruir(lista_iter_t *iter)
{
    free(iter);
}
/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE LISTA JUNTO CON EL ITERADOR
 *-----------------------------------------------------------------------------*/

bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato)
{
    if(lista_esta_vacia(lista) || iter->actual == lista->primero)
    {
        if(!lista_insertar_primero(lista, dato)) return false;

        iter->anterior = lista->primero;
        iter->actual = lista->primero;
    }else if(lista_iter_al_final(iter))
    {
        if(!lista_insertar_ultimo(lista, dato)) return false;

        iter->actual = lista->ultimo;
    }else
    {
        nodo_lista_t *nodo = nodo_crear(dato);
        if(!nodo) return false;

        nodo->siguiente = iter->actual;
        (iter->anterior)->siguiente = nodo;
        iter->actual = nodo;
        lista->largo++;
    }

    return true;
}

void *lista_borrar(lista_t *lista, lista_iter_t *iter)
{
    if(lista_esta_vacia(lista) || lista_iter_al_final(iter)) return NULL;

    void *dato;

    if(iter->actual == lista->primero)
    {
        dato = lista_borrar_primero(lista);
        iter->actual = lista->primero;
    }else
    {
        nodo_lista_t *nodo_aux = iter->actual;
        dato = (iter->actual)->dato;

        (iter->anterior)->siguiente = (iter->actual)->siguiente;
        iter->actual = (iter->anterior)->siguiente;

        if(nodo_aux == lista->ultimo) lista->ultimo = iter->anterior;
        lista->largo--;
        free(nodo_aux);
    }
        return dato;
}
/*-----------------------------------------------------------------------------
 *  PRIMITIVA DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra)
{
    lista_iter_t *iter = lista_iter_crear(lista);
    if(!iter) return;
    bool seguir;

    while(!lista_iter_al_final(iter))
    {
        seguir = visitar(lista_iter_ver_actual(iter), extra);
        if(!seguir) break;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}
