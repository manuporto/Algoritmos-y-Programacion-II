#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "sistema.h"

struct sistema{
	hash_t *hash;
	vector_generico_t *tweets;
};

struct tweet{
	char *mensaje;
	size_t id;
	size_t favs;
};
