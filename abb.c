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

void nodo_swap_clave_dato(nodo_abb_t *nodo1, nodo_abb_t *nodo2){
	char *clave_aux = nodo1->clave;
	void *dato_aux = nodo1->clave;
	nodo1->clave = nodo2->clave;
	nodo1->dato = nodo2->dato;
	nodo2->clave = clave_aux;
	nodo2->dato = dato_aux;
	// Solo cambio las referencias a la clave y el dato porque quiero que la
	// estructura del nodo conserve las referencias a sus respectivos hijos
}

void nodo_swap_completo(nodo_abb_t *nodo1, nodo_abb_t *nodo2){
	// Podría reutilizar el código de arriba, pero es más directo hacerlo así
	nodo_abb_t *aux = nodo1;
	*nodo1 = *nodo2;
	*nodo2 = *aux;
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
	if(!arbol->raiz) arbol->raiz = nodo_nuevo;
	else{
		nodo_abb_t *nodo_actual = arbol->raiz;
		nodo_abb_t *nodo_padre;
		int cmp;
		while(nodo_actual){
			nodo_padre = nodo_actual;
			cmp = arbol->comparar(clave, nodo_actual->clave);
			// La clave es mas grande que la actual. Avanzo a la derecha
			if(cmp > 0) nodo_actual = nodo_actual->der;
			// La clave es mas chica que la actual. Avanzo a la izquierda
			if(cmp < 0) nodo_actual = nodo_actual->izq;
			if(cmp == 0){
				//Significa que encontré un nodo con la misma clave. Hay que reempla
				//zarlo
				void *aux = nodo_actual->dato;
				nodo_actual->dato = dato;
				if(arbol->destruir_dato) arbol->destruir_dato(aux);
				//Destruyo el nodo nuevo porque "reciclo" el nodo viejo, cambiandole
				//el dato
				nodo_destruir(nodo_nuevo, NULL);
				return true;
			}
		}
		// La clave no existía y debo insertarla. Tengo que verificar si va a la
		// derecha o izquierda del padre
		if(cmp > 0) nodo_padre->der = nodo_nuevo;
		else nodo_padre->izq = nodo_nuevo;
	}
	arbol->cantidad = arbol->cantidad + 1;
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

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

void abb_destruir_recursivo(nodo_abb_t *nodo, void destruir_dato(void*)){
	if(!nodo) return;
	abb_destruir_recursivo(nodo->izq, destruir_dato);
	abb_destruir_recursivo(nodo->der, destruir_dato);
	nodo_destruir(nodo, destruir_dato);
}
void abb_destruir(abb_t *arbol){
	abb_destruir_recursivo(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

void *abb_borrar(abb_t *arbol, const char *clave){
	nodo_abb_t *nodo_actual = arbol->raiz;
	nodo_abb_t *nodo_padre = NULL;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		if(cmp != 0) nodo_padre = nodo_actual;
		// La clave es mas grande que la actual. Avanzo a la derecha
		if(cmp > 0) nodo_actual = nodo_actual->der;
		// La clave es mas chica que la actual. Avanzo a la izquierda
		if(cmp < 0) nodo_actual = nodo_actual->izq;
		if(cmp == 0){
			// Caso A: no tiene hijos
			void *devolucion = nodo_actual->dato;
			if(!nodo_actual->izq && !nodo_actual->der){
				if(nodo_actual != arbol->raiz){
					if(nodo_padre->izq == nodo_actual) nodo_padre->izq = NULL;
					else nodo_padre->der = NULL;
				}
				else arbol->raiz = NULL;
				nodo_destruir(nodo_actual, NULL);
			}
			// Caso B: tiene un solo hijo
			else if(!nodo_actual->izq || !nodo_actual->der){
				if(!nodo_actual->izq){
					if(!nodo_padre) arbol->raiz = nodo_actual->der;
					else if(nodo_padre->izq == nodo_actual) 
						nodo_padre->izq = nodo_actual->der;
					else nodo_padre->der = nodo_actual->der;
				}
				else{
					if(!nodo_padre) arbol->raiz = nodo_actual->izq;
					else if(nodo_padre->izq == nodo_actual) 
						nodo_padre->izq = nodo_actual->izq;
					else nodo_padre->der = nodo_actual->izq;	
				} 
				nodo_destruir(nodo_actual, NULL);
			}
			// Caso C: tiene dos hijos
			else{
				nodo_abb_t *heredero = nodo_actual->der;
				nodo_abb_t *padre_heredero = nodo_actual;
				while(heredero->izq) {
					padre_heredero = heredero;
					heredero = heredero->izq;
				}
				//El heredero es el hijo derecho del actual
				if(nodo_actual->der == heredero){
					if(nodo_padre->izq == nodo_actual) 
						nodo_padre->izq = heredero;
					else nodo_padre->der = heredero;
					// Le agrego todos los hijos que el borrado tenía a izquierda
					heredero->izq = nodo_actual->izq;
					nodo_destruir(nodo_actual, NULL);
				}
				// El heredero no tiene hijos derechos
				else if(!heredero->der){
					nodo_swap_clave_dato(nodo_actual, heredero);
					nodo_destruir(heredero, NULL);
				}
				// El heredero tiene hijos derechos
				else{
					// El heredero solo puede ser hijo izquierdo de su padre
					// en estos casos
					nodo_swap_clave_dato(nodo_actual, heredero);
					padre_heredero->izq = heredero->der;
					nodo_destruir(heredero, NULL);
				}
			}
			arbol->cantidad = arbol->cantidad - 1;
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
