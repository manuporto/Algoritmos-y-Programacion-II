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

 struct heap{
 	void **datos;
 	size_t cantidad;
 	size_t tam;
 	cmp_func_t heap_cmp;
 };

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

void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}
