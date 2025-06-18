/* ------------ app.c ------------ */
#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_CATALOGO "videojuegos.csv"
#define MAX_USERNAME 50

void cargarCatalogo(Map *catalogoMapa, List *catalogoLista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo) {
        printf("Error al abrir el catalogo de juegos\n");
        return;
    }

    char **campos;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Juego *juego = malloc(sizeof(Juego));
        
        strncpy(juego->nombre, campos[0], 99);
        strncpy(juego->cpu_min, campos[1], 49);
        strncpy(juego->gpu_min, campos[2], 49);
        juego->ram_min = atoi(campos[3]);
        strncpy(juego->cpu_rec, campos[4], 49);
        strncpy(juego->gpu_rec, campos[5], 49);
        juego->ram_rec = atoi(campos[6]);

        list_pushBack(catalogoLista, juego);
        map_insert(catalogoMapa, juego->nombre, juego);
    }
    fclose(archivo);
}

void guardarCatalogo(List *catalogoLista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "w");
    if (!archivo) return;

    for (Juego *juego = list_first(catalogoLista); juego != NULL; juego = list_next(catalogoLista)) {
        fprintf(archivo, "\"%s\",%s,%s,%d,%s,%s,%d\n",
                juego->nombre, juego->cpu_min, juego->gpu_min, juego->ram_min,
                juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
    }
    fclose(archivo);
}

int compararRequisitos(EspecificacionesPC pc, Juego *juego) {
    int score = 0;
    
    // Comparación de RAM
    if (pc.ram >= juego->ram_rec) score = 2;
    else if (pc.ram >= juego->ram_min) score = 1;
    
    // Comparación de CPU/GPU (implementación básica)
    if (strstr(pc.cpu, juego->cpu_min) && strstr(pc.gpu, juego->gpu_min)) {
        score = score > 0 ? score : 1;
    }
    
    return score;
}

void mostrarJuego(Juego *juego, EspecificacionesPC pc) {
    printf("\n=== %s ===\n", juego->nombre);
    printf("Requisitos minimos:\n- CPU: %s\n- GPU: %s\n- RAM: %dGB\n",
           juego->cpu_min, juego->gpu_min, juego->ram_min);
    printf("Requisitos recomendados:\n- CPU: %s\n- GPU: %s\n- RAM: %dGB\n",
           juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
    
    int compatibilidad = compararRequisitos(pc, juego);
    printf("\nCompatibilidad: ");
    if (compatibilidad == 2) printf("Recomendado!\n");
    else if (compatibilidad == 1) printf("Minimos\n");
    else printf("No compatible\n");
}

void buscarJuego(Map *catalogoMapa, Map *historialUsuarios, const char *username) {
    char nombre[100];
    printf("Ingrese nombre del juego: ");
    fgets(nombre, 100, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    MapPair *pair = map_search(catalogoMapa, nombre);
    if (pair) {
        Juego *juego = pair->value;
        mostrarJuego(juego, ((MapPair*)map_search(historialUsuarios, username))->value);
        
        // Guardar en historial
        List *historial = map_search(historialUsuarios, username)->value;
        list_pushBack(historial, strdup(nombre));
    } else {
        printf("Juego no encontrado.\n");
    }
    presioneTeclaParaContinuar();
}

// Resto de implementaciones de funciones...
