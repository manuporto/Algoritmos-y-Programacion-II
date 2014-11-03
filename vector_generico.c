#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector_generico.h"

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct vector{
	size_t tam;
	size_t cant;
	void **datos;
};

/*******************************************************************
 *                        IMPLEMENTACION
 ******************************************************************/

// Crea un vector de tamaño tam
// Post: vector es una vector vacío de tamaño tam
vector_generico_t* vector_generico_crear(size_t tam){
	vector_generico_t* vector = malloc(sizeof(vector_generico_t));
	if (vector == NULL) return NULL;

	vector->datos = malloc(tam * sizeof(void*));
	if (vector->datos == NULL){
	    free(vector);
	    return NULL;
	}
	vector->tam = tam;
	vector->cant = 0;
	return vector;
}

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_generico_destruir(vector_generico_t *vector, void destruir_dato(void *)){
	if(destruir_dato)
		for(size_t i = 0; i < vector->tam; i++) 
			destruir_dato(vector->datos[i]);
	free(vector->datos);
	free(vector);
}

// Cambia el tamaño del vector
// Pre: el vector fue creado
// Post: el vector cambió de tamaño a nuevo_tam y devuelve true
// o el vector queda intacto y devuelve false si no se pudo cambiar el tamaño
// a nuevo_tam
bool vector_generico_redimensionar(vector_generico_t *vector, size_t tam_nuevo){
	void **datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(void*));
	// Cuando tam_nuevo es 0, es correcto que devuelva NULL
	// En toda otra situación significa que falló el realloc
	if (tam_nuevo > 0 && datos_nuevo == NULL)
	    return false;
	
	vector->datos = datos_nuevo;
	if (tam_nuevo < vector->cant) vector->cant = tam_nuevo;
	vector->tam = tam_nuevo;
	return true;
}

// Almacena en valor el dato guardado en la posición pos del vector
// Pre: el vector fue creado
// Post: se almacenó en valor el dato en la posición pos. Devuelve false si la
// posición es inválida (fuera del rango del vector, que va de 0 a tamaño-1)
void *vector_generico_obtener(vector_generico_t *vector, size_t pos){
	if(pos < 0 || pos >= vector -> tam)
		return false;
	return vector->datos[pos];
}

// Almacena el valor en la posición pos
// Pre: el vector fue creado
// Post: se almacenó el valor en la posición pos. Devuelve false si la posición
// es inválida (fuera del rango del vector, que va de 0 a tamaño-1) y true si
// se guardó el valor con éxito.
bool vector_generico_guardar(vector_generico_t *vector, size_t pos, void *valor){
	if(vector->cant >= vector->tam) vector_generico_redimensionar(vector, vector->tam * 2);
	if(pos < 0 || pos >= vector -> tam)
		return false;
	
	vector->datos[pos] = valor;
	vector->cant = vector->cant + 1;
	return true;
}

// Devuelve el tamaño del vector
// Pre: el vector fue creado
size_t vector_generico_obtener_tam(vector_generico_t *vector){
	return vector->tam;
}

// Devuelve la cantidad de elementos que hay en el vector
size_t vector_generico_obtener_cant(vector_generico_t *vector){
	return vector->cant;
}