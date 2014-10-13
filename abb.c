/*
 * =====================================================================================
 *  Archivo:  abb.c
 *  Descripcion:  Implementacion del árbol binario de búsqueda en C.
 *  Autores: Porto Manuel Ignacio, Sueiro Ignacio Andres 
 *  Padrones: 96587, 96817  
 * =====================================================================================
 */


/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------
 */

struct abb{
	nodo_abb_t *raiz;
	size_t cantidad;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir_dato;
};

typedef struct nodo_abb{
	char *clave;
	void *dato;
	struct nodo_abb_t *izq;
	struct nodo_abb_t *der;
} nodo_abb_t;

typedef struct abb_iter
{
    abb_t *arbol;
    pila_t *pila;
    nodo_abb_t *actual;
}abb_iter_t;

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE NODO ABB
 *-----------------------------------------------------------------------------*/

// Crea un nodo_hash.
// Post: crea un nodo_hash con una clave y un dato.
static nodo_abb_t *nodo_crear(char *clave, void *dato){
    nodo_abb_t *nodo_abb = malloc(sizeof(nodo_abb_t));
    if(!nodo_hash) return NULL;
    
    char *clave_copia = malloc(sizeof(char)*strlen(clave)+1);
    if(!clave_copia){
    	free(nodo_abb);
    	return NULL;
    }
    
    strcpy(clave_copia, clave)
    nodo_abb->clave = clave_copia;
    nodo_abb->dato = dato;
    nodo_abb->izq = NULL;
    nodo_abb->der = NULL;

    return nodo_hash;
}

// Destruye el par clave-dato.
// Pre: el nodo fue creado.
static void nodo_destruir(nodo_abb_t *nodo_abb, void destruir_dato(void*))
{
    free(nodo_hash->clave);
    if(destruir_dato) destruir_dato(nodo_hash->dato);
    free(nodo_hash);
}

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ÁRBOL
 *-----------------------------------------------------------------------------*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t *abb = malloc(sizeof(abb_t));
	if(!abb) return NULL;
	abb->raiz = NULL;
	abb->comparar = cmp;
	abb->destruir_dato = destruir_dato;
	abb->cantidad = 0;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_abb_t *nodo_nuevo = nodo_crear(clave, dato);
	if(!nodo_nuevo) return false;
	//Caso A: el árbol estaba vacio
	if(!arbol->raiz){
		arbol->raiz = nodo_nuevo;
		return true;
	}
	//Caso B: tengo que recorrer el árbol
	nodo_abb_t *nodo_actual = arbol->raiz;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		// La clave es mas grande que la actual. Avanzo a la derecha
		if(cmp > 0) nodo_actual = nodo_actual->der;
		// La clave es mas chica que la actual. Avanzo a la izquierda
		if(cmp < 0) nodo_actual = nodo_actual->izq;
		if(cmp == 0){
			//Significa que encontré un nodo con la misma clave. Hay que reempla
			//zarlo
			
			//Que hago con el dato cuando lo tengo que reemplazar. Lo destruyo
			// o dejo que el usuario se encargue?
			nodo_actual->dato = nodo_nuevo->dato;
			nodo_destruir(nodo_nuevo, NULL);
			return true;
		}
	}
	// La clave no existía y estoy parado en un NULL en la posición donde debo
	// insertar el nuevo nodo
	nodo_actual = nodo_nuevo;
	arbol->cantidad += 1;
	return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_abb_t nodo_actual = arbol->raiz;
	while(nodo_actual){
		int cmp = arbol->comparar(clave, nodo_actual->clave);
		if(cmp > 0) nodo_actual = nodo_actual->der;
		if(cmp < 0) nodo_actual = nodo_actual->izq;
		if(cmp == 0) return nodo_actual->dato;
	}
	// Si llegó hasta acá, significa que la clave no está presente
	return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(!abb_obtener(arbol, clave)) return false;
	return true;
}



/*-----------------------------------------------------------------------------
 *  PRIMITIVA DEL ITERADOR INTERNO
 *-----------------------------------------------------------------------------*/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DEL ITERADOR EXTERNO
 *-----------------------------------------------------------------------------*/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol)
{
    abb_iter_t *abb_iter = malloc(sizeof(abb_iter_t));
    if(!abb_iter) return NULL;

    abb_iter->pila = pila_crear();
    if(!abb_iter->pila)
    {
        free(abb_iter);
        return NULL;
    }
    abb_iter->arbol = arbol;

}
bool abb_iter_in_avanzar(abb_iter_t *iter)
{
    if(abb_iter_in_al_final) return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
    if(!iter->actual) return NULL;
    return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter)
{
    return !abb_iter_in_ver_actual(iter);
}

void abb_iter_in_destruir(abb_iter_t* iter)
{
    pila_destruir(abb_iter->pila);
    free(iter);
}
