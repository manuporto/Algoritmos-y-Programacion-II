CC = gcc
CFLAGS = -Wall -pedantic --std=c99 -g

SOURCES = heap.c
HEADERS = $(SOURCES:.c=.h) 
OBJECTS = $(SOURCES:.c=.o)
EXEC= prueba_heap

VALGRIND = valgrind --track-origins=yes --leak-check=full
#Fin de la configuracion

all: $(EXEC) $(OBJECTS)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) prueba_heap.c -o $(EXEC)

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -c

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	$(VALGRIND) ./$(EXEC)

clean:
	rm -f *.o $(EXEC)