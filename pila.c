#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define TAM_INICIAL 50


/*
La estructura pila contendrá:
	dato: un puntero a punteros del tipo void
	tamanio: cantidad de elementos que puede contener la pila
	cantidad: cantidad de elementos que realmente tiene la pila
*/
typedef struct _pila {
	void** datos;
	size_t tam;
	size_t cantidad;
} _pila;


/*
Crea una pila.
Post: devuelve una nueva pila vacía.
*/
pila_t* pila_crear(){
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL){
		return NULL;
	}
	pila->datos = malloc(TAM_INICIAL * sizeof(void*));
	if (pila->datos == NULL){
		free(pila);
		return NULL;
	}
	pila->tam = TAM_INICIAL;
	pila->cantidad = 0;
	return pila;
}


/*
Destruye la pila.
Pre: la pila fue creada.
Post: se eliminaron todos los elementos de la pila.
*/
void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
	pila = NULL;
}


/*
Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
Pre: la pila fue creada.
*/
bool pila_esta_vacia(const pila_t *pila){
	if (pila->cantidad > 0){
		return false;
	}
	else{
		return true;
	}
}


/*
Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
Pre: la pila fue creada.
Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
*/
bool pila_apilar(pila_t *pila, void *valor){
	if (pila->tam == pila->cantidad){
		void** datos_nuevo = realloc(pila->datos, 2 * pila->tam * sizeof(void*));
		if (datos_nuevo == NULL){
			return false;
		}
		else{
			pila->datos = datos_nuevo;
			pila->tam = pila->tam * 2;
		}
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad = pila->cantidad + 1;
	return true;
}


/*
Obtiene el valor del tope de la pila. Si la pila tiene elementos,
se devuelve el valor del tope. Si está vacía devuelve NULL.
Pre: la pila fue creada.
Post: se devolvió el valor del tope de la pila, cuando la pila no está
vacía, NULL en caso contrario.
*/
void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila) == true){
		return NULL;
	}
	else{
		return pila -> datos[pila -> cantidad - 1];
	}
}


/*
Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
NULL.
Pre: la pila fue creada.
Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
y la pila contiene un elemento menos.
*/
void* pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila) == true){
		return NULL;
	}
	else{
		void *devolucion = pila_ver_tope(pila);
		pila->cantidad = pila->cantidad -1;
		// Pido que sea mayor a 15 así si apilo y desapilo en los primeros pasos
		// no tenga que hacer realloc y nunca "baje" del tamanio inicial
		if (pila->cantidad > 15 && pila->tam - pila->cantidad > pila->tam * 0.75){
			void** datos_nuevo = realloc(pila->datos, pila->tam / 2 * sizeof(void*));
			// Si falla el realloc, salteo este paso sin perder informacion
			if (datos_nuevo != NULL){
				pila->datos = datos_nuevo;
				pila->tam = pila->tam / 2;
			}
		}
		return devolucion;
	}
}
	
/*
int main(){
	int *prueba;
	prueba = malloc(sizeof(int));
	*prueba = 10;
	pila_t *pila = pila_crear();
	if (pila -> tam == 50){
		printf("Funciona crear\n");
	}
	else{
		printf("No funciona crear\n");
	}
	pila_apilar(pila, prueba);
	if (pila -> cantidad == 1){
		printf("Funciona apilar\n");
	}
	else{
		printf("No funciona apilar\n");
	}
	void *prueba_tope = pila_ver_tope(pila);
	if (prueba_tope != NULL){
		printf("Funciona ver tope\n");
	}
	else{
		printf("No funciona ver tope\n");
	}
	pila_desapilar(pila);
	if (pila -> cantidad == 0){
		printf("Funciona desapilar\n");
	}
	else{
		printf("No funciona desapilar\n");
	}
	for(int i = 0; i < 55; i++){
		pila_apilar(pila, prueba);
	}
	printf("%i\n%i\n", pila->tam, pila->cantidad);
	for(int j = 55; j > 5; j--){
		pila_desapilar(pila);
	}
	printf("%i\n%i\n", pila->tam, pila->cantidad);
	pila_destruir(pila);
	free(prueba);
	return 0;
}
*/
