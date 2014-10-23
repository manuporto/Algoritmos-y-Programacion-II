#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int cmp(const void *a, const void *b){
	if ((int)a > (int)b) return 1;
	else if((int)a == (int)b) return 0;
	else return -1;
}

int main(){
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
	return 0;
}