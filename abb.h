/*
 * =====================================================================================
 *  Archivo:  abb.h
 *  Descripcion: Archivo de cabecera de el abb.  
 *  Autores: Sueiro Ignacio Andres, Porto Manuel Ignacio 
 *  Padrones: 96817, 96587
 * =====================================================================================
 */


/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);

typedef struct abb_iter abb_iter_t;

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ABB
 *-----------------------------------------------------------------------------*/

// Crea el abb.
// Post: devuelve el abb, NULL en caso de error.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda un par clave-valor en una posición del árbol que le corresponde según
// su clave. Si la clave ya está presente, se reemplaza el dato asociado
// Pre: el árbol fue creado
// Post: devuelve true si todo OK, false en caso de error.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

void *abb_borrar(abb_t *arbol, const char *clave);

// Pre: el árbol fue creado
// Post: devuelve el valor asociado a la clave pasado por parámetro. NULL si la
// clave no está presente.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Pre: el árbol fue creado
// Post: devuelve true si la clave pasada por parámetro está presente en el
// árbol, false en caso contrario
bool abb_pertenece(const abb_t *arbol, const char *clave);

size_t abb_cantidad(abb_t *arbol);

void abb_destruir(abb_t *arbol);

/*-----------------------------------------------------------------------------
 *  PRIMITIVA DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

// Itera un arbol binario de forma "inorder".
// Pre: el abb fue creado, la primitiva recibe una funcion de callback 
// "visitar" que devuelve true si se debe seguir iterando o false en caso
// contrario.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR EXTERNO
 *-----------------------------------------------------------------------------*/

// Crea un iterador de abb.
// Pre: el arbol fue creado.
// Post: devuelve un iterador en la posicion inicial.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza un nodo del abb.
// Pre: el iterador fue creado.
// Post: devuelve true si pudo avanzar, false en caso contrario.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Pre: el iterador fue creado.
// Post: devuelve la clave correspondiente a la posicion actual
// del iterador.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Informa si se esta o no al final del abb.
// Pre: el iterador fue creado.
// Post: devuelve true si se esta al final del abb, y false en caso 
// contrario.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
void abb_iter_in_destruir(abb_iter_t* iter);


