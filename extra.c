#include "extra.h"


#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 300

char **leer_linea_csv(FILE *archivo, char separador) {
  static char linea[MAX_LINE_LENGTH];
  static char *campos[MAX_FIELDS];
  char *ptr, *start;
  int idx = 0;

  if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL) {
    return NULL; // No hay más líneas para leer
  }

  // Eliminar salto de linea
  linea[strcspn(linea, "\n")] = '\0';

  ptr = start = linea;
  while (*ptr) {
    if (idx >= MAX_FIELDS - 1)
      break;

    if (*ptr == '\"') { // Inicio de un campo entrecomillado
      start = ++ptr;    // Saltar la comilla inicial
      while (*ptr && !(*ptr == '\"' && *(ptr + 1) == separador))
        ptr++;
    } else { // Campo sin comillas
      start = ptr;
      while (*ptr && *ptr != separador)
        ptr++;
    }

    if (*ptr) {
      *ptr = '\0'; // Reemplazar comilla final o separador por terminación
      ptr++;
      if (*ptr == separador)
        ptr++;
    }

    // Quitar comilla final si existe
    if (*(ptr - 2) == '\"') {
      *(ptr - 2) = '\0';
    }

    campos[idx++] = start;
  }

  campos[idx] = NULL; // Marcar el final del array
  return campos;
}

List *split_string(const char *str, const char *delim) {
  List *result = list_create();
  char *token = strtok((char *)str, delim);

  while (token != NULL) {
    // Eliminar espacios en blanco al inicio del token
    while (*token == ' ') {
      token++;
    }

    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1;
    while (*end == ' ' && end > token) {
      *end = '\0';
      end--;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token);

    // Agregar el nuevo string a la lista
    list_pushBack(result, new_token);

    // Obtener el siguiente token
    token = strtok(NULL, delim);
  }

  return result;
}

// Función para limpiar la pantalla
//void limpiarPantalla() { system("clear"); }

void limpiarPantalla() {
  #ifdef _WIN32
      system("cls");
  #else
      system("clear");
  #endif
  }

void presioneTeclaParaContinuar() 
{
  /*
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
  */
  printf("Presione Enter para continuar...");
  while (getchar() != '\n'); // limpia buffer
      getchar(); // espera Enter
}

//funciones de pila para historial (Amanda)
pilaHistorial *crearPila(){
      PilaHistorial *pila = malloc(sizeof(pilaHistorial));
    pila->tope = NULL;
    return pila;
} // le reserva memoria a la pila

void apilar(PilaHistorial *pila, const char *Juego) { //para nueva busqueda, crea un nuevo nodo
  NodoHistorial *nuevo = malloc(sizeof(NodoHistorial));
  strncpy(nuevo->juego, juego, 99); //le copia la información 
  nuevo->juego[99] = '\0';
  nuevo->siguiente = pila->tope;
  pila->tope = nuevo;
}

void mostrarPila(PilaHistorial *pila){
  NodoHistorial *actual = pila->tope;
  if (!actual){
    printf("No hay historial.\n");
    return;
  }
  printf("Historial\n");
  while (actual != NULL){
    printf("- %s\n", actual->juego);
    actual = actual->siguiente;
  }
}

void liberarPila(PilaHistorial *pila){
  NodoHistorial *actual = pila->tope;
  while (actual != NULL){
    NodoHistorial *temp = actual;
    actual = actual->siguiente;
    free(temp);
  }
  free(pila);
}