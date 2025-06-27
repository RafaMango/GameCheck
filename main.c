#include "extra.h"
#include "list.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARCHIVO_CATALOGO "videojuegosDos.csv"
#define ARCHIVO_HISTORIAL "historial.csv"
#define MAX_USERNAME 50
#define MAX_HISTORIAL 100
/*
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

typedef struct {
    char username[MAX_USERNAME];
    char juego[100];
} RegistroHistorial;
*/
// Tablas de puntajes aproximados para CPUs y GPUs
typedef struct
{
    char modelo[50];
    int puntaje;
} ComponentePuntaje;

ComponentePuntaje cpu_scores[] = {
    // INTEL Pentium/Celeron (Gama baja)
    {"Pentium 4", 10},
    {"Pentium D", 12},
    {"Pentium G4400", 18},
    {"Pentium G4500", 20},
    {"Pentium G4560", 25},
    {"Pentium G5400", 30},
    {"Pentium Gold G6400", 35},
    {"Celeron G1820", 15},
    {"Celeron G4900", 20},
    {"Celeron G5920", 25},

    // INTEL Core 2 Series
    {"Core 2 Duo E6300", 15},
    {"Core 2 Duo E8400", 18},
    {"Core 2 Quad Q6600", 20},
    {"Core 2 Quad Q9400", 22},
    {"Core 2 Extreme QX9770", 25},

    // INTEL Core i3 (1ra-10ma Gen)
    {"i3-2100", 30},
    {"i3-2120", 32},
    {"i3-3210", 35},
    {"i3-3220", 35},
    {"i3-3240", 36},
    {"i3-4130", 38},
    {"i3-4160", 38},
    {"i3-6100", 40},
    {"i3-7100", 42},
    {"i3-8100", 45},
    {"i3-9100", 50},
    {"i3-10100", 55},
    {"i3-12100", 70},

    // INTEL Core i5 (1ra-13ra Gen)
    {"i5-2400", 40},
    {"i5-2500", 45},
    {"i5-2500K", 50},
    {"i5-3470", 52},
    {"i5-3570", 54},
    {"i5-3570K", 55},
    {"i5-4460", 58},
    {"i5-4590", 60},
    {"i5-4690", 62},
    {"i5-4690K", 65},
    {"i5-6400", 65},
    {"i5-6500", 68},
    {"i5-6600", 70},
    {"i5-7400", 72},
    {"i5-7600K", 75},
    {"i5-8400", 78},
    {"i5-8500", 80},
    {"i5-8600", 80},
    {"i5-8600K", 82},
    {"i5-9400", 80},
    {"i5-9400F", 83},
    {"i5-9600K", 85},
    {"i5-10400", 85},
    {"i5-10600K", 90},
    {"i5-11400", 88},
    {"i5-11600K", 95},
    {"i5-12400", 100},
    {"i5-12600K", 110},
    {"i5-13600K", 125},

    // INTEL Core i7 (1ra-13ra Gen)
    {"i7-2600", 60},
    {"i7-2600K", 65},
    {"i7-3770", 65},
    {"i7-3770K", 70},
    {"i7-4770", 68},
    {"i7-4770K", 70},
    {"i7-4790", 70},
    {"i7-4790K", 72},
    {"i7-6700", 75},
    {"i7-6700K", 75},
    {"i7-7700", 78},
    {"i7-7700K", 80},
    {"i7-8700", 82},
    {"i7-8700K", 85},
    {"i7-9700", 88},
    {"i7-9700K", 90},
    {"i7-9800X", 95},
    {"i7-10700", 95},
    {"i7-10700K", 100},
    {"i7-11700K", 105},
    {"i7-12700K", 120},
    {"i7-13700K", 135},

    // INTEL Core i9 (7ma-13ra Gen)
    {"i9-7900X", 100},
    {"i9-9900K", 95},
    {"i9-9900X", 105},
    {"i9-10900K", 100},
    {"i9-10980XE", 110},
    {"i9-11900K", 110},
    {"i9-12900K", 130},
    {"i9-13900K", 140},

    // INTEL Xeon (Equivalentes a consumo)
    {"Xeon E3-1230", 55},
    {"Xeon E3-1270", 60},
    {"Xeon E3-1275", 62},
    {"Xeon E5-1650", 65},
    {"Xeon W-2123", 80},
    {"Xeon W-2175", 110},

    // AMD (para comparación)
    {"Athlon II X4", 25},
    {"FX-4300", 30},
    {"FX-6300", 35},
    {"FX-8350", 45},
    {"Ryzen 3 1200", 40},
    {"Ryzen 3 3100", 50},
    {"Ryzen 5 1400", 55},
    {"Ryzen 5 1600", 60},
    {"Ryzen 5 2600", 70},
    {"Ryzen 5 3600", 80},
    {"Ryzen 5 5600", 95},
    {"Ryzen 7 1700", 75},
    {"Ryzen 7 2700", 85},
    {"Ryzen 7 3700X", 85},
    {"Ryzen 7 5800X", 90},
    {"Ryzen 9 3900X", 95},
    {"Ryzen 9 5900X", 95},
    {"Ryzen 9 5950X", 100},
    {"Ryzen 9 7950X", 110}
};

ComponentePuntaje gpu_scores[] = {
    // GPUs INTEGRADAS (Muy bajas)
    {"Intel HD Graphics", 5},
    {"Intel HD 3000", 10},
    {"Intel HD 4000", 12},
    {"Intel HD 530", 15},
    {"Intel UHD 630", 25},
    {"Intel Iris Xe", 40},
    
    // NVIDIA GT/GTX/RTX
    {"GT 710", 15},
    {"GT 730", 20},
    {"GT 1030", 30},
    {"GTX 460", 35},
    {"GTX 550 Ti", 30},
    {"GTX 560", 38},
    {"GTX 560 Ti", 40},
    {"GTX 650", 35},
    {"GTX 660", 40},
    {"GTX 670", 45},
    {"GTX 750", 40},
    {"GTX 750 Ti", 45},
    {"GTX 760", 50},
    {"GTX 770", 55},
    {"GTX 780", 60},
    {"GTX 950", 50},
    {"GTX 960", 55},
    {"GTX 970", 60},
    {"GTX 980", 70},
    {"GTX 1050", 65},
    {"GTX 1050 Ti", 70},
    {"GTX 1060", 75},
    {"GTX 1070", 80},
    {"GTX 1070 Ti", 85},
    {"GTX 1080", 85},
    {"GTX 1080 Ti", 90},
    {"GTX 1650", 70},
    {"GTX 1660", 80},
    {"GTX 1660 Ti", 85},
    {"RTX 2060", 90},
    {"RTX 2070", 95},
    {"RTX 2080", 100},
    {"RTX 2080 Ti", 105},
    {"RTX 3050", 85},
    {"RTX 3060", 100},
    {"RTX 3060 Ti", 105},
    {"RTX 3070", 105},
    {"RTX 3070 Ti", 110},
    {"RTX 3080", 110},
    {"RTX 3080 Ti", 115},
    {"RTX 3090", 115},
    {"RTX 3090 Ti", 120},
    {"RTX 4070", 120},
    {"RTX 4080", 130},
    {"RTX 4090", 140},
    
    // AMD Radeon
    {"Radeon HD 5770", 20},
    {"Radeon HD 7850", 30},
    {"Radeon R7 260X", 35},
    {"Radeon R9 270", 40},
    {"Radeon R9 280", 45},
    {"Radeon R9 290", 50},
    {"Radeon R9 380", 55},
    {"Radeon R9 390", 60},
    {"RX 460", 50},
    {"RX 470", 55},
    {"RX 480", 60},
    {"RX 550", 40},
    {"RX 560", 50},
    {"RX 570", 60},
    {"RX 580", 65},
    {"RX 590", 70},
    {"RX 5500 XT", 75},
    {"RX 5600 XT", 80},
    {"RX 5700", 85},
    {"RX 5700 XT", 90},
    {"RX 6600", 80},
    {"RX 6600 XT", 85},
    {"RX 6700 XT", 85},
    {"RX 6800", 90},
    {"RX 6800 XT", 95},
    {"RX 6900 XT", 95},
    {"RX 6950 XT", 100},
    {"RX 7600", 85},
    {"RX 7700 XT", 100},
    {"RX 7800 XT", 110},
    {"RX 7900 XT", 110},
    {"RX 7900 XTX", 120}
};

// Función para obtener puntaje dado un modelo (busca substring)
// Esta función busca si el modelo de CPU o GPU del usuario está presente en la tabla de componentes.
// Si lo encuentra, devuelve el puntaje asociado al modelo.
// n es el número de elementos en la tabla.
// Si no encuentra el modelo, devuelve 0 (lo que indica que no es compatible
int obtener_puntaje(ComponentePuntaje *tabla, int n, const char *modelo_usuario)
{
    for (int i = 0; i < n; i++)
    {
        if (strstr(modelo_usuario, tabla[i].modelo) != NULL)
        {
            return tabla[i].puntaje;
        }
    }
    return 0; // No encontrado o muy bajo
}

// Evaluar compatibilidad avanzada CPU+GPU+RAM
// Evalúa si la PC del usuario cumple con los requisitos mínimos o recomendados de un juego,
// basado en los puntajes de CPU y GPU y la cantidad de RAM.
int evaluar_compatibilidad(EspecificacionesPC *pc, Juego *juego)
{
    // 1. Obtener los puntajes para los componentes del PC del usuario.
    int cpu_user_score = obtener_puntaje(cpu_scores, sizeof(cpu_scores) / sizeof(cpu_scores[0]), pc->cpu);
    int gpu_user_score = obtener_puntaje(gpu_scores, sizeof(gpu_scores) / sizeof(gpu_scores[0]), pc->gpu);

    // 2. Obtener los puntajes para los requisitos MÍNIMOS del juego.
    int cpu_min_score = obtener_puntaje(cpu_scores, sizeof(cpu_scores) / sizeof(cpu_scores[0]), juego->cpu_min);
    int gpu_min_score = obtener_puntaje(gpu_scores, sizeof(gpu_scores) / sizeof(gpu_scores[0]), juego->gpu_min);

    // 3. Obtener los puntajes para los requisitos RECOMENDADOS del juego.
    int cpu_rec_score = obtener_puntaje(cpu_scores, sizeof(cpu_scores) / sizeof(cpu_scores[0]), juego->cpu_rec);
    int gpu_rec_score = obtener_puntaje(gpu_scores, sizeof(gpu_scores) / sizeof(gpu_scores[0]), juego->gpu_rec);

    // 4. Evaluar si se cumplen los requisitos RECOMENDADOS.
    // Para ello, el PC debe igualar o superar CADA UNO de los requisitos: CPU, GPU y RAM.
    int cumple_recomendados = (cpu_user_score >= cpu_rec_score) &&
                              (gpu_user_score >= gpu_rec_score) &&
                              (pc->ram >= juego->ram_rec);

    if (cumple_recomendados)
    {
        return 2; // Nivel de compatibilidad: Recomendado
    }

    // 5. Si no cumple los recomendados, evaluar si se cumplen los requisitos MÍNIMOS.
    // El PC debe igualar o superar CADA UNO de los requisitos: CPU, GPU y RAM.
    int cumple_minimos = (cpu_user_score >= cpu_min_score) &&
                         (gpu_user_score >= gpu_min_score) &&
                         (pc->ram >= juego->ram_min);

    if (cumple_minimos)
    {
        return 1; // Nivel de compatibilidad: Mínimos
    }

    // 6. Si no cumple ni los mínimos, no es compatible.
    return 0; // Nivel de compatibilidad: No compatible
}

void agregarAlHistorial(const char *username, const char *juego) 
{
    // Leer todo el historial existente
    
    RegistroHistorial *nuevoRegistro = malloc(sizeof(RegistroHistorial));
    strncpy(nuevoRegistro->username, username, MAX_USERNAME);
    strncpy(nuevoRegistro->juego, juego, 100);
    List *historial = list_create();
    FILE *archivo = fopen(ARCHIVO_HISTORIAL, "r");
    
    if (archivo)
    {   char linea[200];
        while(fgets(linea, sizeof(linea), archivo))
        {
            linea[strcspn(linea, "\n")] = 0; // Eliminar salto de línea
            char *user = strtok(linea, ";");
            char *game = strtok(NULL, ";");
            
            if (user && game) 
            {
                RegistroHistorial *reg = malloc(sizeof(RegistroHistorial));
                strncpy(reg->username, user, MAX_USERNAME);
                strncpy(reg->juego, game, 100);
                list_pushBack(historial, reg);
            }
        }
        fclose(archivo);
    }

    // Agregar el nuevo registro al principio (FIFO)
    list_pushFront(historial, nuevoRegistro);
    
    // Mantener solo los últimos MAX_HISTORIAL registros
    while (list_size(historial) > MAX_HISTORIAL)
    {
        free(list_popBack(historial));
    }
    
    // Escribir el historial actualizado
    archivo = fopen(ARCHIVO_HISTORIAL, "w");
    if (!archivo) 
    {
        list_clean(historial);
        free(historial);
        return;
    }
    
    for (RegistroHistorial *reg = list_first(historial); reg != NULL; reg = list_next(historial)) 
    {
        fprintf(archivo, "%s;%s\n", reg->username, reg->juego);
    }
    
    fclose(archivo);
    list_clean(historial);
    free(historial);
}

void buscarJuego(Map *mapa, EspecificacionesPC *pc, const char *username) {
    char nombreJuego[100];
    printf("Ingrese el nombre del juego a buscar: ");
    fgets(nombreJuego, sizeof(nombreJuego), stdin);
    nombreJuego[strcspn(nombreJuego, "\n")] = 0;

    Juego *juego = map_get(mapa, nombreJuego);
    if (juego == NULL) {
        printf("El juego '%s' no se encuentra en el catálogo.\n", nombreJuego);
        return;
    }

    int compatibilidad = 0;
    if (pc->ram > 0) {
        compatibilidad = evaluar_compatibilidad(pc, juego);
    }

    mostrar_juego_compatibilidad(juego, compatibilidad);
    agregarAlHistorial(username, nombreJuego);
}

void mostrarCatalogo(List *lista, EspecificacionesPC *pc)
{
    printf("\n=== Catálogo Completo ===\n");
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista))
    {
        int compatibilidad = 0;
        if (pc->ram > 0)
        { // Si se han ingresado especificaciones
            compatibilidad = evaluar_compatibilidad(pc, juego);
        }
        mostrar_juego_compatibilidad(juego, compatibilidad);
    }
}

void verHistorial(const char *username) {
    printf("\n=== Historial de búsquedas para %s ===\n", username);
    
    List *historial = list_create();
    FILE *archivo = fopen(ARCHIVO_HISTORIAL, "r");
    if (!archivo) {
        printf("No hay historial disponible.\n");
        return;
    }
    
    // Leer el archivo y cargar en lista
    char linea[200];
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
        char *user = strtok(linea, ";");
        char *game = strtok(NULL, ";");
        
        if (user && game) {
            RegistroHistorial *reg = malloc(sizeof(RegistroHistorial));
            strncpy(reg->username, user, MAX_USERNAME);
            strncpy(reg->juego, game, 100);
            list_pushBack(historial, reg);
        }
    }
    fclose(archivo);
    
    // Mostrar solo los del usuario
    int encontrado = 0;
    for (RegistroHistorial *reg = list_first(historial); reg != NULL; reg = list_next(historial)) {
        if (strcmp(reg->username, username) == 0) {
            printf("- %s\n", reg->juego);
            encontrado = 1;
        }
    }
    
    if (!encontrado) {
        printf("No hay búsquedas registradas para este usuario.\n");
    }
    
    list_clean(historial);
    free(historial);
}

void agregarJuego(Map *mapa, List *lista)
{
    Juego *nuevoJuego = malloc(sizeof(Juego));

    printf("\n=== Agregar nuevo juego ===\n");
    printf("Nombre del juego: ");
    fgets(nuevoJuego->nombre, 100, stdin);
    nuevoJuego->nombre[strcspn(nuevoJuego->nombre, "\n")] = 0;

    printf("CPU minimo: ");
    fgets(nuevoJuego->cpu_min, 50, stdin);
    nuevoJuego->cpu_min[strcspn(nuevoJuego->cpu_min, "\n")] = 0;

    printf("GPU minimo: ");
    fgets(nuevoJuego->gpu_min, 50, stdin);
    nuevoJuego->gpu_min[strcspn(nuevoJuego->gpu_min, "\n")] = 0;

    printf("RAM minimo (GB): ");
    scanf("%d", &nuevoJuego->ram_min);
    while (getchar() != '\n')
        ;

    printf("CPU recomendado: ");
    fgets(nuevoJuego->cpu_rec, 50, stdin);
    nuevoJuego->cpu_rec[strcspn(nuevoJuego->cpu_rec, "\n")] = 0;

    printf("GPU recomendado: ");
    fgets(nuevoJuego->gpu_rec, 50, stdin);
    nuevoJuego->gpu_rec[strcspn(nuevoJuego->gpu_rec, "\n")] = 0;

    printf("RAM recomendada (GB): ");
    scanf("%d", &nuevoJuego->ram_rec);
    while (getchar() != '\n')
        ;

    // Agregar a las estructuras
    list_pushBack(lista, nuevoJuego);
    map_insert(mapa, strdup(nuevoJuego->nombre), nuevoJuego);

    printf("Juego agregado exitosamente!\n");
}
// Función para guardar catálogo
// Guarda el catálogo de videojuegos en un archivo CSV.
void guardarCatalogo(List *lista)
{
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "w");
    if (!archivo)
    {
        printf("Error al abrir el archivo para guardar. \n");
        return;
    }
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista))
    {
        fprintf(archivo, "%s,%s,%s,%d,%s,%s,%d\n",
                juego->nombre,
                juego->cpu_min,
                juego->gpu_min,
                juego->ram_min,
                juego->cpu_rec,
                juego->gpu_rec,
                juego->ram_rec);
    }
    fclose(archivo);
    printf("Catalogo guardado exitosamente.\n");
}
// Función para mostrar un juego con compatibilidad
// Función para mostrar un juego con compatibilidad
// Muestra los detalles de un juego (como los requisitos mínimos y recomendados)
// y su compatibilidad con la PC del usuario (Recomendado, Mínimos, o No compatible).
void mostrar_juego_compatibilidad(Juego *juego, int compat)
{
    printf("\n%s\n", juego->nombre);
    printf("  Minimos: CPU: %s | GPU: %s | RAM: %dGB\n", juego->cpu_min, juego->gpu_min, juego->ram_min);
    printf("  Recomendados: CPU: %s | GPU: %s | RAM: %dGB\n", juego->cpu_rec, juego->gpu_rec, juego->ram_rec);
    printf("  Compatibilidad: ");
    if (compat == 2)
        printf("Cumple recomendados\n");
    else if (compat == 1)
        printf("Cumple minimos\n");
    else
        printf("No compatible\n");
}

// Función para cargar catálogo
// Función para cargar catálogo
// Lee el archivo CSV de videojuegos, extrae los datos y los carga en una lista y un mapa.
// La lista contiene los juegos en orden secuencial y el mapa permite búsquedas rápidas por nombre.
void cargarCatalogo(Map *mapa, List *lista)
{
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo)
    {
        printf("No se pudo abrir el archivo de catalogo.\n");
        return;
    }
    char **campos;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL)
    {
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
// Función para mostrar juegos compatibles ordenados
// Muestra los juegos que son compatibles con la PC del usuario, ordenados primero por los recomendados
// y luego por los que cumplen los requisitos mínimos. Utiliza una lista auxiliar para almacenar los juegos.
/***
CAMBIAR METODO DE ORDENAMIENDO A QSORT, YA QUE ESTA EN BUBBLE SORT Y ES MENOS EFICIENTE
***/
void verJuegosCompatibles(List *lista, EspecificacionesPC *pc)
{
    typedef struct
    {
        Juego *juego;
        int compat;
    } Compat;

    List *compatibles = list_create();
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista))
    {
        int comp = evaluar_compatibilidad(pc, juego);
        if (comp > 0)
        {
            Compat *c = malloc(sizeof(Compat));
            c->juego = juego;
            c->compat = comp;
            list_pushBack(compatibles, c);
        }
    }

    // Ordenar compatibles por compatibilidad (descendente)
    int n = 0;
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles))
        n++;
    for (int i = 0; i < n - 1; i++)
    {
        Compat *ci = list_first(compatibles);
        for (int j = 0; j < n - i - 1; j++)
        {
            Compat *cj = list_next(compatibles);
            if (cj->compat > ci->compat)
            {
                Compat tmp = *ci;
                *ci = *cj;
                *cj = tmp;
            }
            ci = cj;
        }
    }

    printf("\n=== Juegos compatibles ===\n");
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles))
    {
        mostrar_juego_compatibilidad(c->juego, c->compat);
    }

    // Liberar memoria auxiliar
    for (Compat *c = list_first(compatibles); c; c = list_next(compatibles))
        free(c);
    free(compatibles);
}

// Función para ingresar especificaciones del usuario
// Permite al usuario ingresar los detalles de su PC (CPU, GPU, RAM) para verificar compatibilidad
// con los juegos del catálogo.
void ingresarEspecificaciones(EspecificacionesPC *pc)
{
    printf("Ingrese CPU (ej: i7-4790): ");
    fgets(pc->cpu, 50, stdin);
    pc->cpu[strcspn(pc->cpu, "\n")] = 0;
    printf("Ingrese GPU (ej: GTX 1060): ");
    fgets(pc->gpu, 50, stdin);
    pc->gpu[strcspn(pc->gpu, "\n")] = 0;
    printf("Ingrese RAM (GB): ");
    scanf("%d", &pc->ram);
    while (getchar() != '\n')
        ; // limpiar buffer
}

// Función para mostrar menú y controlar flujo
void menuPrincipal()
{
    system("chcp 65001");
    system("cls");

    Map *mapa = sorted_map_create((int (*)(void *, void *))strcmp); // Crear mapa ordenado para juegos
    List *lista = list_create();

    cargarCatalogo(mapa, lista);

    EspecificacionesPC pc = {"", "", 0};
    char username[MAX_USERNAME];
    printf("Ingrese su nombre de usuario: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;

    //PilaHistorial *historialPila = cargarHistorial(username);

    int opcion;
    do
    {
        printf("\n--- Menú Principal ---\n");
        printf("1) Ver Catalogo\n");
        printf("2) Ingresar especificaciones PC\n");
        printf("3) Ver juegos compatibles\n");
        printf("4) Buscar    juego\n");
        printf("5) Ver historial de busqueda\n");
        printf("6) Agregar juego al catalogo\n");
        printf("7) Salir\n");
        printf("Seleccione opcion: ");
        scanf("%d", &opcion);
        while (getchar() != '\n')
            ; // limpiar buffer

        switch (opcion)
        {
        case 1:
            mostrarCatalogo(lista, &pc);
            break;
        case 2:
            ingresarEspecificaciones(&pc);
            break;
        case 3:
            if (pc.ram == 0)
            {
                printf("Primero ingrese sus especificaciones.\n");
            }
            else
            {
                verJuegosCompatibles(lista, &pc);
            }
            break;
        case 4:
            buscarJuego(mapa, &pc, username);
            break;
        case 5:
            verHistorial(username);
            break;
        case 6:
            agregarJuego(mapa, lista);
            break;
        case 7:
            guardarCatalogo(lista);
            
            //liberarPila(historialPila);
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    } while (opcion != 7);

    // Liberar memoria
    // (Implementar función para liberar memoria de mapa y lista)
}

int main()
{
    menuPrincipal();
    return 0;
}
