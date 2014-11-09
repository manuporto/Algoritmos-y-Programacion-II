#ifndef VGENERICO_H
#define VGENERICO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct vector vector_generico_t;

/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/

// Crea un vector de tamanio tam
// Post: vector es una vector vacio de tamanio tam
vector_generico_t* vector_generico_crear(size_t tam);
	
// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_generico_destruir(vector_generico_t *vector, void destruir_dato(void *));

// Cambia el tamanio del vector
// Pre: el vector fue creado
// Post: el vector cambio de tamanio a nuevo_tam y devuelve true
// o el vector queda intacto y devuelve false si no se pudo cambiar el tamanio
// a nuevo_tam
bool vector_generico_redimensionar(vector_generico_t *vector, size_t nuevo_tam);

// Pre: el vector fue creado
// Post: devuelve el dato en la posicion pos. Devuelve false si la
// posicion es invalida (fuera del rango del vector, que va de 0 a tamanio-1)
void *vector_generico_obtener(vector_generico_t *vector, size_t pos);

// Almacena el valor en la posicion pos
// Pre: el vector fue creado
// Post: se almaceno el valor en la posicion pos. Devuelve false si la posicion
// es invalida (fuera del rango del vector, que va de 0 a tamanio-1) y true si
// se guardo el valor con exito.
bool vector_generico_guardar(vector_generico_t *vector, size_t pos, void *valor);

// Devuelve el tamanio del vector
// Pre: el vector fue creado
size_t vector_generico_obtener_tam(vector_generico_t *vector);

// Devuelve la cantidad de elementos que hay en el vector
size_t vector_generico_obtener_cant(vector_generico_t *vector);

#endif // VGENERICO_H
