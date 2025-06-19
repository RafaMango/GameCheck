#include "extra.h"
#include "list.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//hola
#define ARCHIVO_CATALOGO "videojuegosDos.csv"
#define MAX_USERNAME 50

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

// Tablas de puntajes aproximados para CPUs y GPUs
typedef struct {
    char modelo[50];
    int puntaje;
} ComponentePuntaje;

ComponentePuntaje cpu_scores[] = {
    {"i3-3210", 30},
    {"i5-2500K", 50},
    {"i7-4790", 70},
    {"Ryzen 5 2600", 60},
    {"Ryzen 7 3700X", 80},
    {"Core i9", 90},
    {"Ryzen 9", 90}
};

ComponentePuntaje gpu_scores[] = {
    {"GTX 660", 40},
    {"GTX 1060", 65},
    {"RTX 3060", 80},
    {"RTX 4090", 100},
    {"RX 570", 60},
    {"RX 6800", 85}
};

// Función para obtener puntaje dado un modelo (busca substring)
int obtener_puntaje(ComponentePuntaje *tabla, int n, const char *modelo_usuario) {
    for (int i = 0; i < n; i++) {
        if (strstr(modelo_usuario, tabla[i].modelo) != NULL) {
            return tabla[i].puntaje;
        }
    }
    return 0; // No encontrado o muy bajo
}

// Evaluar compatibilidad avanzada CPU+GPU+RAM
int evaluar_compatibilidad(EspecificacionesPC *pc, Juego *juego) 
{
    int cpu_user_score = obtener_puntaje(cpu_scores, sizeof(cpu_scores)/sizeof(cpu_scores[0]), pc->cpu);
    int gpu_user_score = obtener_puntaje(gpu_scores, sizeof(gpu_scores)/sizeof(gpu_scores[0]), pc->gpu);

    int cpu_min_score = obtener_puntaje(cpu_scores, sizeof(cpu_scores)/sizeof(cpu_scores[0]), juego->cpu_min);
    int gpu_min_score = obtener_puntaje(gpu_scores, sizeof(gpu_scores)/sizeof(gpu_scores[0]), juego->gpu_min);

    int cpu_rec_score = obtener_puntaje(cpu_scores, sizeof(cpu_scores)/sizeof(cpu_scores[0]), juego->cpu_rec);
    int gpu_rec_score = obtener_puntaje(gpu_scores, sizeof(gpu_scores)/sizeof(gpu_scores[0]), juego->gpu_rec);

    int cumple_min = (cpu_user_score >= cpu_min_score) && (gpu_user_score >= gpu_min_score) && (pc->ram >= juego->ram_min);
    int cumple_rec = (cpu_user_score >= cpu_rec_score) && (gpu_user_score >= gpu_rec_score) && (pc->ram >= juego->ram_rec);

    if (cumple_rec) return 2; // Recomendado
    else if (cumple_min) return 1; // Minimos
    else return 0; // No compatible
}

// Función para mostrar un juego con compatibilidad
void mostrar_juego_compatibilidad(Juego *juego, int compat) 
{
    printf("\n%s\n", juego->nombre);
    printf("  Minimos: CPU: %s | GPU: %s | RAM: %dGB\n", juego->cpu_min, juego->gpu_min, juego->ram_min);
    printf("  Recomendados: CPU: %s | GPU: %s | RAM: %dGB\n", juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
    printf("  Compatibilidad: ");
    if (compat == 2) printf("Recomendado\n");
    else if (compat == 1) printf("Cumple minimos\n");
    else printf("No compatible\n");
}

// Función para cargar catálogo
void cargarCatalogo(Map *mapa, List *lista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de catalogo.\n");
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
        list_pushBack(lista, juego);
        map_insert(mapa, strdup(juego->nombre), juego);
    }
    fclose(archivo);
}

// Función para mostrar juegos compatibles ordenados
void verJuegosCompatibles(List *lista, EspecificacionesPC *pc) {
    typedef struct {
        Juego *juego;
        int compat;
    } Compat;

    List *compatibles = list_create();
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        int comp = evaluar_compatibilidad(pc, juego);
        if (comp > 0) {
            Compat *c = malloc(sizeof(Compat));
            c->juego = juego;
            c->compat = comp;
            list_pushBack(compatibles, c);
        }
    }

    // Ordenar compatibles por compatibilidad (descendente)
    int n = 0;
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles)) n++;
    for (int i = 0; i < n - 1; i++) {
        Compat *ci = list_first(compatibles);
        for (int j = 0; j < n - i -1; j++) {
            Compat *cj = list_next(compatibles);
            if (cj->compat > ci->compat) {
                Compat tmp = *ci;
                *ci = *cj;
                *cj = tmp;
            }
            ci = cj;
        }
    }

    printf("\n=== Juegos compatibles ===\n");
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles)) {
        mostrar_juego_compatibilidad(c->juego, c->compat);
    }

    // Liberar memoria auxiliar
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles)) free(c);
    free(compatibles);
}

// Función para ingresar especificaciones del usuario
void ingresarEspecificaciones(EspecificacionesPC *pc) {
    printf("Ingrese CPU (ej: i7-4790): ");
    fgets(pc->cpu, 50, stdin);
    pc->cpu[strcspn(pc->cpu, "\n")] = 0;
    printf("Ingrese GPU (ej: GTX 1060): ");
    fgets(pc->gpu, 50, stdin);
    pc->gpu[strcspn(pc->gpu, "\n")] = 0;
    printf("Ingrese RAM (GB): ");
    scanf("%d", &pc->ram);
    while(getchar() != '\n'); // limpiar buffer
}

// Función para mostrar menú y controlar flujo
void menuPrincipal() {
    Map *mapa = sorted_map_create((int (*)(void*, void*))strcmp);
    List *lista = list_create();

    cargarCatalogo(mapa, lista);

    EspecificacionesPC pc = {"", "", 0};
    char username[MAX_USERNAME];
    printf("Ingrese su nombre de usuario: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;

    int opcion;
    do {
        printf("\n--- Menú ---\n");
        printf("1) Ingresar especificaciones PC\n");
        printf("2) Ver juegos compatibles\n");
        printf("3) Salir\n");
        printf("Seleccione opcion: ");
        scanf("%d", &opcion);
        while(getchar() != '\n'); // limpiar buffer

        switch(opcion) {
            case 1:
                ingresarEspecificaciones(&pc);
                break;
            case 2:
                if (pc.ram == 0) {
                    printf("Primero ingrese sus especificaciones.\n");
                } else {
                    verJuegosCompatibles(lista, &pc);
                }
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while(opcion != 3);
}

int main() {
    menuPrincipal();
    return 0;
}
