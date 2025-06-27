#ifndef EXTRA_H
#define EXTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "map.h"

typedef struct {
    char nombre[100];
    char cpu_min[50];
    char gpu_min[50];
    int ram_min;
    char cpu_rec[50];
    char gpu_rec[50];
    int ram_rec;
} Juego;

typedef struct {
    char cpu[50];
    char gpu[50];
    int ram;
} EspecificacionesPC;

// estructuras de pilas, para historial (Amanda)
typedef struct NodoHistorial {
    char juego[100];
    struct NodoHistorial *siguiente; 
} NodoHistorial; 

typedef struct {
    NodoHistorial *tope;
} PilaHistorial;


int evaluar_compatibilidad(EspecificacionesPC *pc, Juego *juego);
void mostrar_juego_compatibilidad(Juego *juego, int compat);
/**
 * Función para leer y parsear una línea de un archivo CSV en campos
 * individuales.
 *
 * Esta función lee una línea de un archivo CSV, donde cada campo puede estar
 * opcionalmente entrecomillado y separado por un caracter definido como
 * 'separador'. La función maneja campos entrecomillados que pueden contener el
 * separador como parte del valor del campo.
 *
 * @param archivo Puntero a FILE que representa el archivo CSV abierto.
 * @param separador Caracter utilizado para separar los campos en la línea del
 * CSV.
 *
 * @return Retorna un puntero a un arreglo de cadenas (char*), donde cada
 * elemento representa un campo de la línea del CSV leída. Si no hay más líneas
 * para leer o se alcanza el fin del archivo, retorna NULL.
 *
 * Uso:
 * FILE* f = fopen("datos.csv", "r");
 * char** campos;
 * while ((campos = leer_linea_csv(f, ',')) != NULL) {
 *     // Procesar campos
 * }
 * fclose(f);
 *
 * Notas:
 * - La función utiliza memoria estática internamente, por lo que cada llamada
 * sobrescribe los datos de la llamada anterior.
 * - La función asume que ninguna línea del CSV excede MAX_LINE_LENGTH
 * caracteres y que no hay más de MAX_FIELDS campos por línea.
 */
char **leer_linea_csv(FILE *archivo, char separador);

List *split_string(const char *str, const char *delim);

// Función para limpiar la pantalla
void limpiarPantalla();

void presioneTeclaParaContinuar();

#endif