#ifndef SISTEMA_H
#define SISTEMA_H

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

typedef struct sistema sistema_t;

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE SISTEMA
 *-----------------------------------------------------------------------------*/

// Post: Crea y devuelve un puntero al sistema
sistema_t *sistema_crear();

// Recibe por parámetro el sistema al que se agregará el tweet, el usuario que
// lo crea y el contenido del mensaje.
// Pre: el sistema fue creado
// Post: se agrega el tweet al sistema
void sistema_twittear(sistema_t *sistema, char *nombre, char *tweet);

// Recibe por parámetro el sistema en el que está el tweet y el id que correspon
// de al mensaje que se quiere agregar a favoritos. (Dentro de la primitiva se
// convierte el id a número)
// Pre: el sistema fue creado
// Post: se le suma 1 a la cantidad de favoritos del tweet
void sistema_favorito(sistema_t *sistema, char *id);

// Recibe el sistema en el cual se buscaran los tweets, el parámetro de búsqueda,
// el orden (cronológico o de favoritos) y la cantidad (0 si se desean ver todos
// los tweets).
// Pre: el sistema fue creado.
// Post: se muestran por pantalla todos los resultados de búsqueda
void sistema_buscar(sistema_t *sistema, char *buscado, char *orden, char *cantidad);

// Destruye el sistema.
// Pre: el sistema fue creado.
// Post: Se destruye el sistema y la informacion asociada a el.
void sistema_destruir(sistema_t *sistema);

#endif // SISTEMA_H
