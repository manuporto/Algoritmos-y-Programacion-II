#ifndef SISTEMA_H
#define SISTEMA_H

/*-----------------------------------------------------------------------------
 *  DEFINICION DE ESTRUCTURAS
 *-----------------------------------------------------------------------------*/

typedef struct sistema sistema_t;

/*-----------------------------------------------------------------------------
 *  FUNCIONES AUXILIARES
 *-----------------------------------------------------------------------------*/

// Imprime el mensaje de error correspondiente al codigo pasado por parametro.
// Codigo 1: ERROR_COMANDO_INVALIDO
// Codigo 2: ERROR_TWIT_ID_INVALIDO
// Codigo 3: ERROR_TWIT_DEMASIADO_LARGO
void imprimir_error(int codigo);

/*-----------------------------------------------------------------------------
 *  PRIMITIVAS DE SISTEMA
 *-----------------------------------------------------------------------------*/

// Post: Crea y devuelve un puntero al sistema. En caso de error devuelve NULL.
sistema_t *sistema_crear();

// Recibe por parametro el sistema al que se agregara el tweet, el usuario que
// lo crea y el contenido del mensaje.
// Pre: el sistema fue creado
// Post: se agrega el tweet al sistema
void sistema_twittear(sistema_t *sistema, char *nombre, char *tweet);

// Recibe por parametro el sistema en el que esta el tweet y el id que 
// corresponde al mensaje que se quiere agregar a favoritos. (Dentro de la 
// primitiva se convierte el id a numero)
// Pre: el sistema fue creado
// Post: se le suma 1 a la cantidad de favoritos del tweet
void sistema_favorito(sistema_t *sistema, char *id);

// Recibe el sistema en el cual se buscaran los tweets, el parametro de 
// busqueda, el orden (cronologico o de favoritos) y la cantidad (0 si se desean
// ver todos los tweets).
// Pre: el sistema fue creado.
// Post: se muestran por pantalla todos los resultados de busqueda
void sistema_buscar(sistema_t *sistema, char *buscado, char *orden, char *cantidad);

// Destruye el sistema.
// Pre: el sistema fue creado.
// Post: Se destruye el sistema y la informacion asociada a el.
void sistema_destruir(sistema_t *sistema);

#endif // SISTEMA_H
