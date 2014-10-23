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
#define TAM_INI 50

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

 struct heap{
 	void **datos;
 	size_t cantidad;
 	size_t tam;
 	cmp_func_t cmp;
 };

/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES
 *-----------------------------------------------------------------------------*/

static size_t obtener_pos_padre(heap, size_t pos);

void swap_vector(void **datos, size_t pos1, size_t pos2){
	void *aux = vector[pos1];
	vector[pos1] = vector[pos2];
	vector[pos2] = aux;
}

size_t devolver_pos_hijo_mayor(heap_t *heap, size_t pos1, size_t pos2){
	if(heap->cmp(heap->vector[pos1], heap->vector[pos2]) >= 0) return pos1;
	else return pos2;
}

static void upheap(heap_t *heap, void* elem);

void downheap(heap_t *heap, size_t pos_ini){
	size_t pos_actual = pos_ini;
	while(pos_actual < heap->cantidad){
		void *dato_actual = heap->datos[pos_actual]
		size_t pos_hijo_izq = 2 * pos_actual + 1;
		size_t pos_hijo_der = 2 * pos_actual + 2;
		void *hijo_izq = NULL;
		void *hijo_der = NULL;
		if(pos_hijo_izq <= heap->cantidad) hijo_izq = heap->datos[2 * pos_actual + 1];
		if(pos_hijo_der <= heap->cantidad) hijo_der = heap->datos[2 * pos_actual + 2];
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
			size_t pos_mayor = devolver_pos_hijo_mayor(heap, pos_hijo_izq, pos_hijo_der);
			if(heap->cmp(heap->vector[pos_mayor], dato_actual) < 1) break;
			else{
				swap_vector(heap->vector, pos_actual, pos_mayor);
				pos_actual = pos_mayor;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS
 *-----------------------------------------------------------------------------*/

 heap_t *heap_crear(cmp_func_t cmp){
 	heap_t *heap = malloc(sizeof(heap_t));
 	if(!heap) return NULL;

 	heap->datos = malloc(sizeof(void*)*TAM_INI);
 	if(!heap->datos){
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
 		for(size_t i = 0; i < cantidad; i++) destruir_elemento(heap->datos[i]);
 	}
 	free(heap->datos);
 	free(heap);
 }

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return(heap->cantidad == 0);
}

bool heap_encolar(heap_t *heap, void *elem)
{
    if(!elem) return false;
    if(heap->cantidad == heap->tam) // Redimensionar
    heap->datos[heap->cantidad+1] = elem;
    upheap(heap, elem);
    heap->cantidad++;
    return true;
}

void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap);
