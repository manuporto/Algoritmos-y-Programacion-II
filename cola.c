#include "cola.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct nodo {
	void* dato;
	struct nodo* proximo;
}nodo_t;

struct cola{
	nodo_t* primero;
	nodo_t* ultimo;
};


// Crea un nodo
// Recibe un puntero a un valor
// Devuelve un puntero a un nodo
nodo_t* nodo_crear(void* valor){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL) return NULL;
	
	nuevo_nodo->dato = valor;
	nuevo_nodo->proximo = NULL;
	return nuevo_nodo;
}

// Destruye el nodo
// Pre: el nodo fue creado
void nodo_destruir(nodo_t* nodo){
	free(nodo);
}


// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t* cola_crear(){
	cola_t *cola = malloc(sizeof(cola_t));
	
	if (cola == NULL) return NULL;
	cola->primero = NULL;
	cola->ultimo = NULL;
	return cola; 
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo = nodo_crear(valor);
	// Si falló la creación del nodo, hay error en encolar
	if (nodo == NULL) return false;

	// Si no hay elementos en la cola, lo que encola queda como primero
	// y último
	if(cola_esta_vacia(cola)){
		cola->primero = nodo;
		cola->ultimo = nodo;
	}
	else{
		cola->ultimo->proximo = nodo;
		cola->ultimo = nodo;
	}
	return true;
}

// Devuelve verdadero o falso, según si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola){
	if (cola->primero != NULL) return false;
	return true;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;
	nodo_t *desencolado = cola->primero;
	void* devolucion = desencolado->dato;
	if (cola->primero == cola->ultimo){
		cola->primero = NULL;
		cola->ultimo = NULL;
	}
	else{
		cola->primero = cola->primero->proximo;
	}
	nodo_destruir(desencolado);
	return devolucion;
}

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)) return NULL;
	
	return cola->primero->dato;
}

// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	while(!cola_esta_vacia(cola)){
		void* desencolado = cola_desencolar(cola);
		if(destruir_dato != NULL) destruir_dato(desencolado);
	}
	free(cola);
}


