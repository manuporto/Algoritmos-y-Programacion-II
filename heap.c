/*
 * =====================================================================================
 *  Archivo:  heap.c
 *  Descripcion:  Implementacion del heap en C.
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
 * =====================================================================================
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#define TAM_INI 50

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

 struct heap{
 	void **vector;
 	size_t cantidad;
 	size_t tam;
 	cmp_func_t cmp;
 };

/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES
 *-----------------------------------------------------------------------------*/

// Post: devuelve el indice donde se encuentra el padre del elemento.
static size_t obtener_pos_padre(size_t pos)
{
    return (pos - 1) / 2;
}

// Post: deuvelve el indice donde se ecuentra el hijo izquierdo del elemento.
static size_t obtener_pos_izq(size_t pos)
{
    return 2 * pos + 1;
}

// Post: devuelve el indice donde se encuentra el hijo derecho del elemento.
static size_t obtener_pos_der(size_t pos)
{
    return 2 * pos + 2;
}

static void swap_vector(void **vector, size_t pos1, size_t pos2){
	void *aux = vector[pos1];
	vector[pos1] = vector[pos2];
	vector[pos2] = aux;
}

static size_t obtener_pos_hijo_mayor(heap_t *heap, size_t pos1, size_t pos2){
	if(heap->cmp(heap->vector[pos1], heap->vector[pos2]) >= 0) return pos1;
	else return pos2;
}

static void upheap(heap_t *heap, size_t pos)
{
    if(heap_cantidad(heap) == 1) return;

    while(pos > 0)
    {
        void *dato_actual = heap->vector[pos];
        size_t pos_padre = obtener_pos_padre(pos);
        void *dato_padre = heap->vector[pos_padre];
        if(heap->cmp(dato_padre, dato_actual) >= 0) return;
        else
        {
            swap_vector(heap->vector, pos, pos_padre);
            pos = pos_padre;
        }

    }
}

static void downheap(heap_t *heap, size_t pos_ini){
	size_t pos_actual = pos_ini;
	while(pos_actual < heap->cantidad){
		void *dato_actual = heap->vector[pos_actual];
		size_t pos_hijo_izq = obtener_pos_izq(pos_actual);
		size_t pos_hijo_der = obtener_pos_der(pos_actual);
		void *hijo_izq = NULL;
		void *hijo_der = NULL;
		if(pos_hijo_izq < heap->cantidad) hijo_izq = heap->vector[pos_hijo_izq];
		if(pos_hijo_der < heap->cantidad) hijo_der = heap->vector[pos_hijo_der];
		// No hay más hijos
		if(!hijo_izq && !hijo_der) break;
		// Solo hay un izquierdo
		else if(hijo_izq && !hijo_der){
			// El hijo izquierdo es menor o igual al dato actual
			if(heap->cmp(hijo_izq, dato_actual) < 1) break;
			// El hijo izquierdo es mayor
			else{
				swap_vector(heap->vector, pos_actual, pos_hijo_izq);
				pos_actual = pos_hijo_izq;
			}
		}
		// Hay dos hijos
		else{
			size_t pos_mayor = obtener_pos_hijo_mayor(heap, pos_hijo_izq, pos_hijo_der);
			if(heap->cmp(heap->vector[pos_mayor], dato_actual) < 1) break;
			else{
				swap_vector(heap->vector, pos_actual, pos_mayor);
				pos_actual = pos_mayor;
			}
		}
	}
}

heap_t *heapify(void **vector, size_t cant, cmp_func_t cmp){
	heap_t *heap_aux = heap_crear(cmp);
	void **vector_aux = heap_aux->vector;
	heap_aux->vector = vector;
	free(vector_aux);
	heap_aux->cantidad = cant;
	heap_aux->tam = cant;
	heap_aux->cmp = cmp;
	// Solo hay que hacer downheap de los elementos con hijos
	long long actual = cant / 2 - 1;
	while(actual >= 0){
		downheap(heap_aux, actual);
		actual--;
	}
	return heap_aux;
}
/*-----------------------------------------------------------------------------
 *  PRIMITIVAS
 *-----------------------------------------------------------------------------*/

 heap_t *heap_crear(cmp_func_t cmp){
 	heap_t *heap = malloc(sizeof(heap_t));
 	if(!heap) return NULL;

 	heap->vector = malloc(sizeof(void*)*TAM_INI);
 	if(!heap->vector){
 		free(heap);
 		return NULL;
 	}
 	heap->cantidad = 0;
 	heap->tam = TAM_INI;
 	heap->cmp = cmp;
 	return heap;
 }

 void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
 	if(destruir_elemento){
 		for(size_t i = 0; i < heap->cantidad; i++) destruir_elemento(heap->vector[i]);
 	}
 	free(heap->vector);
 	free(heap);
 }

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return heap_cantidad(heap) == 0;
}

bool heap_encolar(heap_t *heap, void *elem)
{
    if(!elem) return false;
    if(heap_cantidad(heap) == heap->tam); // Redimensionar
    heap->vector[heap_cantidad(heap)] = elem;
    heap->cantidad++;
    upheap(heap, heap_cantidad(heap)-1);
    return true;
}

void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	return heap->vector[0];
}

void *heap_desencolar(heap_t *heap)
{
    // Falta evaluar una condicion para que redimensione.
    
    if(heap_cantidad(heap) == 0) return NULL;
    // Falta ver caso de cuando es el ultimo elemento.
    void *dato_dev = heap->vector[0];
    heap->vector[0] = heap->vector[heap_cantidad(heap)-1];
    heap->cantidad--;
    downheap(heap, 0);
    return dato_dev;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t *heap = heapify(elementos, cant, cmp);
	while(heap->cantidad > 1){
		swap_vector(heap->vector, 0, heap->cantidad - 1);
		heap->cantidad--;
		downheap(heap, 0);
	}
	free(heap);
}