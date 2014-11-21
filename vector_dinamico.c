#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector_dinamico.h"

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

struct vector
{
	size_t tam;
	size_t cant;
	int *datos;
};

/*******************************************************************
 *                        IMPLEMENTACION
 ******************************************************************/

// Crea un vector de tama�o tam
// Post: vector es una vector vac�o de tama�o tam
vector_t* vector_crear(size_t tam)
{
	vector_t* vector = malloc(sizeof(vector_t));
	if (vector == NULL) return NULL;

	vector->datos = malloc(tam * sizeof(int));
	if (vector->datos == NULL)
	{
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
void vector_destruir(vector_t *vector)
{
	free(vector -> datos);
	free(vector);
}

// Cambia el tama�o del vector
// Pre: el vector fue creado
// Post: el vector cambi� de tama�o a nuevo_tam y devuelve true
// o el vector queda intacto y devuelve false si no se pudo cambiar el tama�o
// a nuevo_tam
bool vector_redimensionar(vector_t *vector, size_t tam_nuevo)
{
	int* datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));
	// Cuando tam_nuevo es 0, es correcto que devuelva NULL
	// En toda otra situaci�n significa que fall� el realloc
	if (tam_nuevo > 0 && datos_nuevo == NULL)
	{
	    return false;
	}
	vector->datos = datos_nuevo;
	if(tam_nuevo < vector->cant) vector->cant = tam_nuevo;
	vector->tam = tam_nuevo;
	return true;
}

// Almacena en valor el dato guardado en la posici�n pos del vector
// Pre: el vector fue creado
// Post: se almacen� en valor el dato en la posici�n pos. Devuelve false si la
// posici�n es inv�lida (fuera del rango del vector, que va de 0 a tama�o-1)
bool vector_obtener(vector_t *vector, size_t pos, int *valor)
{
	if(pos < 0 || pos >= vector -> tam){
		return false;
	}
	else{
		*valor = vector -> datos[pos];
		return true;
	}
}

// Almacena el valor en la posici�n pos
// Pre: el vector fue creado
// Post: se almacen� el valor en la posici�n pos. Devuelve false si la posici�n
// es inv�lida (fuera del rango del vector, que va de 0 a tama�o-1) y true si
// se guard� el valor con �xito.
bool vector_guardar(vector_t *vector, size_t pos, int valor)
{
	if(vector->cant >= vector->tam) vector_redimensionar(vector, vector->tam * 2);
	if(pos < 0 || pos >= vector -> tam){
		return false;
	}
	else{
		vector -> datos[pos] = valor;
		vector->cant = vector->cant + 1;
		return true;
	}
}

// Devuelve el tama�o del vector
// Pre: el vector fue creado
size_t vector_obtener_tamanio(vector_t *vector)
{
	return vector -> tam;
}

// Devuelve la cantidad de elementos que hay en el vector
size_t vector_obtener_cantidad(vector_t *vector){
	return vector->cant;
}