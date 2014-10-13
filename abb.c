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
	struct nodo_abb_t *izq;
	struct nodo_abb_t *der;
} nodo_abb;

typedef struct abb_iter
{
    abb_t *arbol;
    pila_t *pila;
    nodo_abb_t *actual;
}abb_iter_t;

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
