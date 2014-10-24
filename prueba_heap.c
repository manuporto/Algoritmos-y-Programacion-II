#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES    
 *-----------------------------------------------------------------------------*/

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int cmp(const void *a, const void *b){
	if ((int)a > (int)b) return 1;
	else if((int)a == (int)b) return 0;
	else return -1;
}

/*-----------------------------------------------------------------------------
 *  PRUEBAS UNITARIAS
 *-----------------------------------------------------------------------------*/

void prueba_heap_vacio()
{
    printf("INICIO PRUEBAS HEAP VACIO\n");

    // Pasar NULL a la funcion de destruccion puede traer errores.
    heap_t *heap = heap_crear(NULL);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));
	print_test("Prueba heap ver maximo, es NULL", !heap_ver_max(heap));
	print_test("Prueba heap desencolar, es NULL", !heap_desencolar(heap));

	heap_destruir(heap, NULL);
}

void prueba_heap_encolar()
{
    printf("INICIO PRUEBAS HEAP ENCOLAR\n");

    heap_t *heap = heap_crear(cmp);
    int elem1 = 2;
    int elem2 = 5;
    int elem3 = 1;

    print_test("Prueba heap insertar elem1", heap_encolar(heap, &elem1));
    print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap ver maximo es elem1", heap_ver_max(heap) == &elem1);
    print_test("Prueba heap insertar elem2", heap_encolar(heap, &elem2));
    print_test("Prueba heap insertar elem3", heap_encolar(heap, &elem3));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Prueba heap ver maximo es elem3", heap_ver_max(heap) == &elem3);

    heap_destruir(heap, NULL);
}

void prueba_heap_desencolar()
{
    printf("INICIO PRUEBAS HEAP DESENCOLAR\n");

    heap_t *heap = heap_crear(cmp);
    int elem1 = 2;
    int elem2 = 5;
    int elem3 = 1;

    print_test("Prueba heap insertar elem1", heap_encolar(heap, &elem1));
    print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap ver maximo es elem1", heap_ver_max(heap) == &elem1);
    print_test("Prueba heap insertar elem2", heap_encolar(heap, &elem2));
    print_test("Prueba heap insertar elem3", heap_encolar(heap, &elem3));
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Prueba heap ver maximo es elem2", heap_ver_max(heap) == &elem2);

    print_test("Prueba heap desencolar es elem2", heap_desencolar(heap) == &elem2);
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap desencolar es elem1", heap_desencolar(heap) == &elem1);
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap desencolar es elem2", heap_desencolar(heap) == &elem2);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));
    print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap));
    print_test("Prueba heap desencolar es NULL", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
}

void prueba_heap_destruir()
{
    printf("INICIO PRUEBAS HEAP DESTRUIR\n");
    
    heap_t *heap = heap_crear(cmp);
    int *elem1 = malloc(sizeof(int));
    int *elem2 = malloc(sizeof(int));
    int *elem3 = malloc(sizeof(int));
    *elem1 = 7;
    *elem2 = 5;
    *elem3 = 9;

    print_test("Prueba heap insertar elem1", heap_encolar(heap, elem1));
    print_test("Prueba heap insertar elem2", heap_encolar(heap, elem2));
    print_test("Prueba heap insertar elem3", heap_encolar(heap, elem3));
	print_test("Prueba heap ver maximo es elem3", heap_ver_max(heap) == elem3);
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
    print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));

    // Se desencola un elemento y se dejan los otros dos para ser destruidos
    // con el heap.
    print_test("Prueba heap desencolar es elem3", heap_desencolar(heap) == elem3);

    heap_destruir(heap, free);
    print_test("Prueba heap destruir", true);
}

void prueba_heap_volumen(size_t elementos)
{
    printf("INICIO DE PRUEBAS HEAP VOLUMEN\n");
    
    heap_t *heap = heap_crear(cmp);
    int vector[elementos];
    srand(time(NULL));

    for(size_t i = 0; i < elementos; i++)
    {
        vector[i] = rand() % 7 + i;
        heap_encolar(heap, &vector[i]);
    }
    
    while(heap_cantidad(heap) > elementos/4)
    {
        heap_desencolar(heap);
    }

    heap_destruir(heap, NULL);
    print_test("Prueba heap volumen", true);
}
void prueba_heap_sort()
{
    int uno = 1;
	int tres = 3;
	int cinco = 5;
	int seis = 6;
	int siete = 7;
	int ocho = 8;
	int nueve = 9;
	int *vector[] = {&seis, &tres, &cinco, &ocho, &uno, &nueve, &siete};
	printf("%p\n", (void*)vector[0]);
	printf("%p\n", (void*)vector[1]);
	printf("%p\n", (void*)vector[2]);
	printf("%p\n", (void*)vector[3]);
	printf("%p\n", (void*)vector[4]);
	printf("%p\n", (void*)vector[5]);
	printf("%p\n", (void*)vector[7]);
	printf("\n\n");

	heap_sort((void**)vector, 7, cmp);
	printf("%p\n", (void*)vector[0]);
	printf("%p\n", (void*)vector[1]);
	printf("%p\n", (void*)vector[2]);
	printf("%p\n", (void*)vector[3]);
	printf("%p\n", (void*)vector[4]);
	printf("%p\n", (void*)vector[5]);
	printf("%p\n", (void*)vector[7]);

}

/*-----------------------------------------------------------------------------
 *  PROGRAMA PRINCIPAL
 *-----------------------------------------------------------------------------*/

int main(){
    prueba_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_desencolar();
    prueba_heap_destruir();
    //prueba_heap_volumen();
    //prueba_heap_sort();
	return 0;
}
