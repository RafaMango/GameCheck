#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extra.h"
#include "list.h"
#include "map.h"

typedef struct
{
    char nombre[100];
    char cpu_min[50];
    char gpu_min[50];
    int ram_min;
    char cpu_rec[50];
    char gpu_rec[50];
    int ram_rec;
} Juego;

typedef struct
{
    char cpu[50];
    char gpu[50];
    int ram;
} EspecificacionesPC;

void limpiarBuffer();

void print_juego(Juego *juego);

int is_equal_str(void *key1, void *key2);

int lower_than_str(void *key1, void *key2);

int compararRequisitos(EspecificacionesPC *pc, Juego *juego);

void cargarCatalogo(Map *mapa, List *lista);

void guardarCatalogo(List *lista);

void mostrarCatalogo(List *lista);

void ingresarEspecificaciones(EspecificacionesPC *pc);

void verJuegosCompatibles(List *lista, EspecificacionesPC *pc);

void verHistorial(Map *historial, char *username);

void buscarJuego(Map *mapa, Map *historial, char *username, EspecificacionesPC *pc);

void agregarJuego(Map *mapa, List *lista);

void mostrarMenu();