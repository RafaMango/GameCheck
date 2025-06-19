#include "extra.h"
#include "list.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_CATALOGO "videojuegos.csv"
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

// --------- Funciones auxiliares ---------
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_juego(Juego *juego) {
    printf("\nNombre: %s\n", juego->nombre);
    printf("  Minimos: CPU: %s | GPU: %s | RAM: %d GB\n", juego->cpu_min, juego->gpu_min, juego->ram_min);
    printf("  Recomendados: CPU: %s | GPU: %s | RAM: %d GB\n", juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
}

int is_equal_str(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

int lower_than_str(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) < 0;
}

int compararRequisitos(EspecificacionesPC *pc, Juego *juego) 
{
    int cumple = 0;
    if (pc->ram >= juego->ram_rec) cumple = 2;
    else if (pc->ram >= juego->ram_min) cumple = 1;
    else cumple = 0;
    // Puedes agregar comparaciones de CPU/GPU si quieres ser más estricto
    return cumple;
}

// --------- Funciones principales ---------
void cargarCatalogo(Map *mapa, List *lista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de catálogo.\n");
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

void guardarCatalogo(List *lista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "w");
    if (!archivo) return;
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        fprintf(archivo, "%s,%s,%s,%d,%s,%s,%d\n",
            juego->nombre, juego->cpu_min, juego->gpu_min, juego->ram_min,
            juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
    }
    fclose(archivo);
}

void mostrarCatalogo(List *lista) {
    puts("\n=== Catálogo de Juegos ===");
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        print_juego(juego);
    }
    presioneTeclaParaContinuar();
}

void ingresarEspecificaciones(EspecificacionesPC *pc) {
    printf("Ingrese Tipo de CPU: ");
    fgets(pc->cpu, 50, stdin); pc->cpu[strcspn(pc->cpu, "\n")] = 0;
    printf("Ingrese Tipo de GPU: ");
    fgets(pc->gpu, 50, stdin); pc->gpu[strcspn(pc->gpu, "\n")] = 0;
    printf("Ingrese RAM (en GB): ");
    scanf("%d", &pc->ram); limpiarBuffer();
    printf("Especificaciones guardadas!\n");
    presioneTeclaParaContinuar();
}

void verJuegosCompatibles(List *lista, EspecificacionesPC *pc) {
    typedef struct { 
        Juego *juego; 
        int puntaje; 
    } Compat;

    List *compatibles = list_create();
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        int p = compararRequisitos(pc, juego);
        if (p > 0) {
            Compat *c = malloc(sizeof(Compat));
            c->juego = juego; c->puntaje = p;
            list_pushBack(compatibles, c);
        }
    }

    // Ordenar de mayor a menor puntaje (Bubble sort usando solo funciones de lista)
    int n = 0;
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles)) n++;
    for (int i = 0; i < n - 1; i++) {
        Compat *ci = list_first(compatibles);
        for (int j = 0; j < n - i - 1; j++) {
            Compat *cj = list_next(compatibles);
            if (cj->puntaje > ci->puntaje) {
                // Intercambiar los datos (swap)
                int tmp_puntaje = ci->puntaje;
                Juego *tmp_juego = ci->juego;
                ci->puntaje = cj->puntaje;
                ci->juego = cj->juego;
                cj->puntaje = tmp_puntaje;
                cj->juego = tmp_juego;
            }
            ci = cj;
        }
    }

    puts("\nJuegos compatibles:");
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles)) {
        printf("%s (", c->juego->nombre);
        if (c->puntaje == 2) printf("Recomendado)\n");
        else if (c->puntaje == 1) printf("Mínimos)\n");
    }
    presioneTeclaParaContinuar();
    // Liberar memoria auxiliar
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles)) free(c);
    free(compatibles);
}


void verHistorial(Map *historial, char *username) {
    MapPair *pair = map_search(historial, username);
    if (!pair || !pair->value) {
        printf("Sin historial.\n");
    } else {
        puts("\nHistorial de búsqueda:");
        for (char *nombre = list_first(pair->value); nombre; nombre = list_next(pair->value)) {
            printf("- %s\n", nombre);
        }
    }
    presioneTeclaParaContinuar();
}

void buscarJuego(Map *mapa, Map *historial, char *username, EspecificacionesPC *pc) {
    char nombre[100];
    printf("Ingrese el nombre del juego: ");
    fgets(nombre, 100, stdin); nombre[strcspn(nombre, "\n")] = 0;
    MapPair *pair = map_search(mapa, nombre);
    if (pair) {
        Juego *juego = pair->value;
        print_juego(juego);
        int comp = compararRequisitos(pc, juego);
        printf("Compatibilidad: ");
        if (comp == 2) puts("Recomendado!");
        else if (comp == 1) puts("Cumple mínimos.");
        else puts("No compatible.");
        // Guardar en historial
        MapPair *hpair = map_search(historial, username);
        List *hist = hpair ? hpair->value : NULL;
        if (!hist) {
            hist = list_create();
            map_insert(historial, strdup(username), hist);
        }
        list_pushBack(hist, strdup(juego->nombre));
    } else {
        printf("Juego no encontrado. ¿Deseas ingresarlo manualmente? (S/N): ");
        char resp = getchar(); limpiarBuffer();
        if (resp == 'S' || resp == 's') {
            // Reutilizar agregarJuego
            printf("Redirigiendo a agregar juego...\n");
            presioneTeclaParaContinuar();
        }
    }
    presioneTeclaParaContinuar();
}

void agregarJuego(Map *mapa, List *lista) {
    Juego *juego = malloc(sizeof(Juego));
    printf("Ingrese nombre del juego: ");
    fgets(juego->nombre, 100, stdin); juego->nombre[strcspn(juego->nombre, "\n")] = 0;
    printf("Ingrese requisitos mínimos (CPU): ");
    fgets(juego->cpu_min, 50, stdin); juego->cpu_min[strcspn(juego->cpu_min, "\n")] = 0;
    printf("Ingrese requisitos mínimos (GPU): ");
    fgets(juego->gpu_min, 50, stdin); juego->gpu_min[strcspn(juego->gpu_min, "\n")] = 0;
    printf("Ingrese requisitos mínimos (RAM): ");
    scanf("%d", &juego->ram_min); limpiarBuffer();
    printf("Ingrese requisitos recomendados (CPU): ");
    fgets(juego->cpu_rec, 50, stdin); juego->cpu_rec[strcspn(juego->cpu_rec, "\n")] = 0;
    printf("Ingrese requisitos recomendados (GPU): ");
    fgets(juego->gpu_rec, 50, stdin); juego->gpu_rec[strcspn(juego->gpu_rec, "\n")] = 0;
    printf("Ingrese requisitos recomendados (RAM): ");
    scanf("%d", &juego->ram_rec); limpiarBuffer();
    list_pushBack(lista, juego);
    map_insert(mapa, strdup(juego->nombre), juego);
    // Guardar en archivo
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "a");
    if (archivo) {
        fprintf(archivo, "%s,%s,%s,%d,%s,%s,%d\n",
            juego->nombre, juego->cpu_min, juego->gpu_min, juego->ram_min,
            juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
        fclose(archivo);
    }
    printf("Juego agregado!\n");
    presioneTeclaParaContinuar();
}

// --------- Menú principal ---------
void mostrarMenu() {
    puts("\n=== Menú Principal ===");
    puts("1) Ver catálogo");
    puts("2) Ingresar especificaciones del PC");
    puts("3) Ver juegos compatibles");
    puts("4) Buscar juego");
    puts("5) Ver historial de búsqueda");
    puts("6) Agregar juego al catálogo");
    puts("7) Salir");
    printf("Seleccione una opción: ");
}

int main() 
{
    limpiarPantalla();
    Map *mapa = sorted_map_create(lower_than_str);
    List *lista = list_create();
    Map *historial = map_create(is_equal_str);

    cargarCatalogo(mapa, lista);

    char username[MAX_USERNAME];
    printf("Ingrese su nombre de usuario: ");
    fgets(username, MAX_USERNAME, stdin); username[strcspn(username, "\n")] = 0;
    if (!map_search(historial, username))
        map_insert(historial, strdup(username), list_create());

    EspecificacionesPC pc = {"", "", 0};

    int opcion;
    do {
        limpiarPantalla();
        mostrarMenu();
        scanf("%d", &opcion); limpiarBuffer();
        switch(opcion) {
            case 1: mostrarCatalogo(lista); 
            break;
            case 2: ingresarEspecificaciones(&pc); 
            break;
            case 3: verJuegosCompatibles(lista, &pc); 
            break;
            case 4: buscarJuego(mapa, historial, username, &pc); break;
            case 5: verHistorial(historial, username); break;
            case 6: agregarJuego(mapa, lista); break;
            case 7: puts("Guardando y saliendo..."); break;
            default: puts("Opción inválida."); presioneTeclaParaContinuar();
        }
    } while(opcion != 7);

    guardarCatalogo(lista);
    // Aquí podrías guardar el historial en un archivo si lo deseas
    return 0;
}
