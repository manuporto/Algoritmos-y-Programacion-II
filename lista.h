#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/*-----------------------------------------------------------------------------
 *  DEFINICION DE LOS TIPOS DE DATOS
 *-----------------------------------------------------------------------------*/
// Definicion de la lista enlazada.
typedef struct lista lista_t;

// Definicion del iterador de la lista enlazada.
typedef struct lista_iter lista_iter_t;

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE LA LISTA
 *-----------------------------------------------------------------------------*/

// Crea una lista.
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear();

// Devuelve verdadero o falso, segun si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de
// error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, valor se encuentra al principio
// de la lista
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de 
// error.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento a la lista, valor se encuentra al final de
// la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Si la lista tiene elementos borra el primer elemento. Si esta vacia, devuelve
// NULL.
// Pre: la lista fue creada.
// Post: se elimino el valor del primer elemento. La lista contiene un elemento
// menos, si no estaba vacia. 
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista esta vacia devuelve
// NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista, cuando no esta vacia.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el largo de la lista. Si esta vacia devuelve 0.
// Pre: la lista fue creada.
// Post: se devolvio el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la funcion destruir_dato por parametro, para
// cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una funcion capaz de destruir los datos
// de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR EXTERNO
 *-----------------------------------------------------------------------------*/

// Crea un iterador.
// Post: devuelve un iterador.
lista_iter_t *lista_iter_crear(const lista_t *lista);
// El iterador avanza una posicion en la lista.
// Pre: el iterador fue creado.
// Post: devuelve true si pudo avanzar o false si llego al final de la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor actual al  que apunta el iterador.
// Pre: el iterador fue creado.
// Post: devuelve el valor apuntado por el iterador actualmente. Si la lista esta
// vacia o esta en el final devuelve NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Verifica si se llego o no al final de la lista
// Pre: la lista fue creada
// Post: Si se llego al final de la lista devuelve true, si no false.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimino el iterador.
void lista_iter_destruir(lista_iter_t *iter);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE LISTAS JUNTO CON EL ITERADOR
 *-----------------------------------------------------------------------------*/

// Inserta el dato entre el valor actual apuntado por el iterador y el elemento 
// anterior.
// Pre: la lista fue creada, el iterador fue creado.
// Post: devuelve true si se  pudo insertar el dato. Devuelve false en caso de 
// error. 
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

// Elimina el dato apuntado por el iterador.
// Pre: la lista fue creada, el iterador fue creado.
// Post: el elemento apuntado por el iterador fue eliminado.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

// Itera una posicion en la lista. La funcion de callback "visitar" recibe el 
// dato y un puntero extra, y devuelve true si se debe seguir iterando,
// false en caso contrario.
// Pre: la lista fue creada.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

#endif // LISTA_H
