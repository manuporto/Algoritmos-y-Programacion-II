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

typedef nodo_abb{
	struct nodo_abb_t *izq;
	struct nodo_abb_t *der;
} nodo_abb;

