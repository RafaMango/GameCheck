#include "extra.h"
#include "list.h"
#include "map.h"
#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_CATALOGO "videojuegos.csv"
#define MAX_USERNAME 50

int main()
{
    limpiarPantalla();
    Map *mapa = sorted_map_create(lower_than_str);
    List *lista = list_create();
    Map *historial = map_create(is_equal_str);

    cargarCatalogo(mapa, lista);

    char username[MAX_USERNAME];
    printf("Ingrese su nombre de usuario: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;
    if (!map_search(historial, username))
        map_insert(historial, strdup(username), list_create());

    EspecificacionesPC pc = {"", "", 0};

    int opcion;
    do
    {
        limpiarPantalla();
        mostrarMenu();
        scanf("%d", &opcion);
        limpiarBuffer();
        switch (opcion)
        {
        case 1:
            mostrarCatalogo(lista);
            break;
        case 2:
            ingresarEspecificaciones(&pc);
            break;
        case 3:
            verJuegosCompatibles(lista, &pc);
            break;
        case 4:
            buscarJuego(mapa, historial, username, &pc);
            break;
        case 5:
            verHistorial(historial, username);
            break;
        case 6:
            agregarJuego(mapa, lista);
            break;
        case 7:
            puts("Guardando y saliendo...");
            break;
        default:
            puts("Opción inválida.");
            presioneTeclaParaContinuar();
        }
    } while (opcion != 7);

    guardarCatalogo(lista);
    // Aquí podrías guardar el historial en un archivo si lo deseas
    return 0;
}
