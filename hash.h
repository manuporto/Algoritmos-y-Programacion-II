/*
 * =====================================================================================
 *
 *       Filename:  hash.h
 *
 *    Description:  Archivo de cabecera de el hash.
 *
 *        Version:  1.0
 *        Created:  06/10/14 01:31:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

// Definicion de la estructura del hash.
typedef struct hash hash_t;

// Definicion del iterador del hash.
typedef struct hash_iter hash_iter_t;

// Definicion de la funcion de estruir dato.
typedef void (*hash_destruir_dato_t)(void *);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL HASH
 *-----------------------------------------------------------------------------*/

// Crea un hash.
// Post: devuelve una nueva tabla de hash vacia.
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);

// Guarda una clave y su dato.
// Pre: la tabla de hash fue creada.
// Post: devuelve true si puede almacenar la clave y el dato, false en caso
// contrario.
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

// Borra un elemento de la tabla de hash.
// Pre: la tabla de hash fue creada.
// Post: devuelve un puntero a void con el dato correspondiente a la clave
// y borra la clave con su dato. Devuelve un puntero a NULL si esta no existe.
void *hash_borrar(hash_t *hash, const char *clave);

// Obtiene el dato correspondiente a la clave recibida por parametro.
// Pre: la tabla de hash fue creada.
// Post: devuelve un puntero a void con el dato correspondiente a la clave.
// Devuelve un puntero a NULL si esta no existe.
void *hash_obtener(const hash_t *hash, const char *clave);

// Revisa si la clave pertenece a la tabla de hash.
// Pre: la tabla de hash fue creada.
// Post: devuelve true si la clave pertenece, false en caso contrario.
bool hash_pertenece(const hash_t *hash, const char *clave);

// Pre: la tabla de hash fue creada.
// Post: devuelve la cantidad de claves con sus datos existentes en la tabla.
size_t hash_cantidad(const hash_t *hash);

// Destruye la tabla de hash.
// Pre: la tabla de hash fue creada.
// Post: se eliminaron todas las claves con sus datos de la tabla de hash.
void hash_destruir(hash_t *hash);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR
 *-----------------------------------------------------------------------------*/

hash_iter_t *hash_iter_crear(const hash_t *hash);
bool hash_iter_avanzar(hash_iter_t *iter);
const char *hash_iter_ver_actual(const hash_iter_t *iter);
bool hash_iter_al_final(const hash_iter_t *iter);
void hash_iter_destruir(hash_iter_t* iter);
