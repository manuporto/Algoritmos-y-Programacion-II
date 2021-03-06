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
	void *dato_aux = nodo1->dato;
	nodo1->clave = nodo2->clave;
	nodo1->dato = nodo2->dato;
	nodo2->clave = clave_aux;
	nodo2->dato = dato_aux;
	// Solo cambio las referencias a la clave y el dato porque quiero que la
	// estructura del nodo conserve las referencias a sus respectivos hijos
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
	if(!arbol->raiz) 
        arbol->raiz = nodo_nuevo;
	else{
		nodo_abb_t *nodo_actual = arbol->raiz;
		nodo_abb_t *nodo_padre;
		int cmp;
		while(nodo_actual){
			
			cmp = arbol->comparar(clave, nodo_actual->clave);
			// La clave es mas grande que la actual. Avanzo a la derecha
			if(cmp!=0) nodo_padre = nodo_actual;
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
		else if(cmp < 0)nodo_padre->izq = nodo_nuevo;
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

int es_hijo_izquierdo_o_derecho(nodo_abb_t *padre, nodo_abb_t *hijo){
	if(padre->der == hijo) return 1;
	else return -1;
}

void* abb_borrar(abb_t *arbol, const char* clave){
	if(abb_cantidad(arbol) == 0) return NULL;
	nodo_abb_t *padre;
	nodo_abb_t *nodo_actual = arbol->raiz;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		if(cmp != 0) padre = nodo_actual;
		if(cmp > 0){
			nodo_actual = nodo_actual->der;
			continue;
		}
		else if(cmp < 0){
			nodo_actual = nodo_actual->izq;
			continue;
		}
		else{
			void *devolucion = nodo_actual->dato;
			// Caso A: No tiene hijos
			if(!nodo_actual->der && !nodo_actual->izq){
				// No tiene hijos y es la raiz del arbol
				if(nodo_actual == arbol->raiz) arbol->raiz = NULL;
                else{
                	int IoD = es_hijo_izquierdo_o_derecho(padre, nodo_actual);
                    if(IoD == -1) padre->izq = NULL;
                    else padre->der = NULL;
                }
			}
			// Caso B: Tiene un hijo a izquierda o a derecha
			else if(!nodo_actual->izq || !nodo_actual->der){
				// Se quiere borrar la raiz
				if(nodo_actual == arbol->raiz){
					if(!arbol->raiz->izq) arbol->raiz = arbol->raiz->der;
					else arbol->raiz = arbol->raiz->izq;
				}
				else{
					int IoD = es_hijo_izquierdo_o_derecho(padre, nodo_actual);
					// Solo tiene hijos a derecha
					if(!nodo_actual->izq){
						// El nodo que quiero borrar es un hijo izquierdo
						if(IoD == -1) padre->izq = nodo_actual->der;
						// Es un derecho
						else padre->der = nodo_actual->der;
					}
					// Solo tiene hijos a izquierda
					else{
						// El nodo que quiero borrar es un hijo izquierdo
						if(IoD == -1) padre->izq = nodo_actual->izq;
						// Es un derecho
						else padre->der = nodo_actual->izq;
					}
				}
			}
			// Caso C: Tiene hijos de los dos lados
			else{
				nodo_abb_t *padre_heredero = nodo_actual;
				nodo_abb_t *heredero = nodo_actual->der;
				nodo_abb_t *hijo_heredero = heredero->izq;
				while(hijo_heredero){
					padre_heredero = heredero;
					heredero = hijo_heredero;
					hijo_heredero = hijo_heredero->izq;
				}
				nodo_swap_clave_dato(heredero, nodo_actual);
				// Ya hice el cambio correspondiente. Heredero ahora tiene los datos que deben ser
				// destruidos
				// Caso 1: el heredero es el hijo derecho del actual
				if(nodo_actual->der == heredero) padre_heredero->der = heredero->der;
				// Caso 2: el heredero tenia hijos a la derecha. Heredero solo puede ser un hijo
				// izquierdo
				else if(heredero->der) padre_heredero->izq = heredero->der;
				// Caso 3: el heredero era un nodo hoja
				else padre_heredero->izq = NULL;
				// Cambio la referencia de nodo actual para después borrar el nodo correcto.
				// Recordemos que por el swap de arriba, heredero ahora tiene los datos que quiero
				// borrar
				nodo_actual = heredero;
			}
			nodo_destruir(nodo_actual, arbol->destruir_dato);
			arbol->cantidad = arbol->cantidad -1;
            return devolucion;
		}
	}
	// La clave no estaba
	return NULL;
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVA DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra)
{
	if(abb_cantidad(arbol) == 0) return;
	
    abb_iter_t *abb_iter = abb_iter_in_crear(arbol);
    bool seguir;
    do
    {
    	nodo_abb_t *nodo_actual = pila_ver_tope(abb_iter->pila);
    	const char *clave = abb_iter_in_ver_actual(abb_iter);
    	void *dato = nodo_actual->dato;
    	seguir = visitar(clave, dato, extra);
    	abb_iter_in_avanzar(abb_iter);
    }while(!abb_iter_in_al_final(abb_iter) && seguir);
    
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
    while(nodo_actual)
    {
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
    if(nodo_actual->der)
    {
    	pila_apilar(iter->pila, nodo_actual->der);
    	nodo_actual = nodo_actual->der;
    	while(nodo_actual->izq)
    	{
    		pila_apilar(iter->pila, nodo_actual->izq);
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
