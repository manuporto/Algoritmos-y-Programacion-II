/*
 * =====================================================================================
 *  Archivo:  abb.c
 *  Descripcion:  Implementacion del árbol binario de búsqueda en C.
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
 * =====================================================================================
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------
 */
typedef struct nodo_abb{
	char *clave;
	void *dato;
	struct nodo_abb *izq;
	struct nodo_abb *der;
} nodo_abb_t;

struct abb{
	nodo_abb_t *raiz;
	size_t cantidad;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir_dato;
};

struct abb_iter
{
    const abb_t *arbol;
    pila_t *pila;
};

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE NODO ABB
 *-----------------------------------------------------------------------------*/

// Crea un nodo_abb.
// Post: crea un nodo_abb con una clave y un dato.
static nodo_abb_t *nodo_crear(const char *clave, void *dato){
    nodo_abb_t *nodo_abb = malloc(sizeof(nodo_abb_t));
    if(!nodo_abb) return NULL;
    
    char *clave_copia = malloc(sizeof(char)*strlen(clave)+1);
    if(!clave_copia){
    	free(nodo_abb);
    	return NULL;
    }
    
    strcpy(clave_copia, clave);
    nodo_abb->clave = clave_copia;
    nodo_abb->dato = dato;
    nodo_abb->izq = NULL;
    nodo_abb->der = NULL;

    return nodo_abb;
}

// Destruye el par clave-dato.
// Pre: el nodo fue creado.
static void nodo_destruir(nodo_abb_t *nodo_abb, void destruir_dato(void*))
{
    free(nodo_abb->clave);
    if(destruir_dato) destruir_dato(nodo_abb->dato);
    free(nodo_abb);
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ÁRBOL
 *-----------------------------------------------------------------------------*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t *abb = malloc(sizeof(abb_t));
	if(!abb) return NULL;
	abb->raiz = NULL;
	abb->comparar = cmp;
	abb->destruir_dato = destruir_dato;
	abb->cantidad = 0;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_abb_t *nodo_nuevo = nodo_crear(clave, dato);
	if(!nodo_nuevo) return false;
	//Caso A: el árbol estaba vacio
	if(!arbol->raiz){
		arbol->raiz = nodo_nuevo;
		return true;
	}
	//Caso B: tengo que recorrer el árbol
	nodo_abb_t *nodo_actual = arbol->raiz;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		// La clave es mas grande que la actual. Avanzo a la derecha
		if(cmp > 0) nodo_actual = nodo_actual->der;
		// La clave es mas chica que la actual. Avanzo a la izquierda
		if(cmp < 0) nodo_actual = nodo_actual->izq;
		if(cmp == 0){
			//Significa que encontré un nodo con la misma clave. Hay que reempla
			//zarlo
			void *aux = nodo_actual->dato;
			nodo_actual->dato = nodo_nuevo->dato;
			arbol->destruir_dato(aux);
			//Destruyo el nodo nuevo porque "reciclo" el nodo viejo, cambiandole
			//el dato
			nodo_destruir(nodo_nuevo, NULL);
			return true;
		}
	}
	// La clave no existía y estoy parado en un NULL en la posición donde debo
	// insertar el nuevo nodo
	nodo_actual = nodo_nuevo;
	arbol->cantidad += 1;
	return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_abb_t *nodo_actual = arbol->raiz;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		if(cmp > 0) nodo_actual = nodo_actual->der;
		if(cmp < 0) nodo_actual = nodo_actual->izq;
		if(cmp == 0) return nodo_actual->dato;
	}
	// Si llegó hasta acá, significa que la clave no está presente
	return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(!abb_obtener(arbol, clave)) return false;
	return true;
}

void abb_destruir(abb_t *arbol){
	abb_iter_t *iter = abb_iter_in_crear(arbol);
	while(!abb_iter_in_al_final(iter)){
		nodo_abb_t *nodo = abb_iter_in_ver_actual(iter);
		abb_iter_in_avanzar(iter);
		nodo_destruir(nodo, arbol->destruir_dato);
	}
}

void *abb_borrar(abb_t *arbol, const char *clave){
	nodo_abb_t *nodo_actual = arbol->raiz;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		// La clave es mas grande que la actual. Avanzo a la derecha
		if(cmp > 0) nodo_actual = nodo_actual->der;
		// La clave es mas chica que la actual. Avanzo a la izquierda
		if(cmp < 0) nodo_actual = nodo_actual->izq;
		if(cmp == 0){
			// Caso A: no tiene hijos
			void *devolucion = nodo_actual->dato;
			if(!nodo_actual->izq && !nodo_actual->der) 
				nodo_destruir(nodo_actual, NULL);
			// Caso B: tiene un solo hijo
			else if(!nodo_actual->izq || !nodo_actual->der){
				nodo_abb_t *aux = nodo_actual;
				if(!nodo_actual->izq) nodo_actual = nodo_actual->der;
				else nodo_actual = nodo_actual->izq;
				nodo_destruir(aux, NULL);
			}
			// Caso C: tiene dos hijos
			else{
				nodo_abb_t *aux = nodo_actual;
				nodo_abb_t *heredero = nodo_actual->der;
				while(heredero->izq) heredero = heredero->izq;
				nodo_actual = heredero;
				heredero = heredero->der;
				nodo_destruir(aux, NULL);
			}
			return devolucion;
		}
	}
	// La clave no está
	return NULL;
}


/*-----------------------------------------------------------------------------
 *  PRIMITIVA DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra)
{
    abb_iter_t *abb_iter = abb_iter_in_crear(arbol);
    nodo_abb_t *nodo_actual = pila_ver_tope(abb_iter->pila);
    const char *clave = abb_iter_in_ver_actual(abb_iter);
    void *dato = nodo_actual->dato;
    bool seguir = visitar(clave, dato, extra);
    while(!abb_iter_in_al_final(abb_iter) && seguir)
    {
        abb_iter_in_avanzar(abb_iter);
        nodo_actual = pila_ver_tope(abb_iter->pila);
        clave = abb_iter_in_ver_actual(abb_iter);
        dato = nodo_actual->dato;
        seguir = visitar(clave, dato, extra);
    }

    abb_iter_in_destruir(abb_iter);
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR EXTERNO
 *-----------------------------------------------------------------------------*/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol)
{
    abb_iter_t *abb_iter = malloc(sizeof(abb_iter_t));
    if(!abb_iter) return NULL;

    abb_iter->pila = pila_crear();
    if(!abb_iter->pila)
    {
        free(abb_iter);
        return NULL;
    }
    nodo_abb_t *nodo_actual = arbol->raiz;
    while(nodo_actual){
    	pila_apilar(abb_iter->pila, nodo_actual);
    	nodo_actual = nodo_actual->izq;
    }
    abb_iter->arbol = arbol;
    return abb_iter;

}
bool abb_iter_in_avanzar(abb_iter_t *iter)
{
    if(abb_iter_in_al_final(iter)) return false;
    nodo_abb_t *nodo_actual = pila_desapilar(iter->pila);
    if(nodo_actual->der){
    	pila_apilar(iter->pila, nodo_actual->der);
    	nodo_actual = nodo_actual->izq;
    	while(nodo_actual) {
    		pila_apilar(iter->pila, nodo_actual);
    		nodo_actual = nodo_actual->izq;
    	}
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
    if(abb_iter_in_al_final(iter)) return NULL;
	nodo_abb_t *nodo_actual = pila_ver_tope(iter->pila);
    return nodo_actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter)
{
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter)
{
    pila_destruir(iter->pila);
    free(iter);
}
