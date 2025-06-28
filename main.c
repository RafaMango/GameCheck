#include "extra.h"
#include "list.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_CATALOGO "videojuegosDos.csv"
#define ARCHIVO_HISTORIAL "historial.txt"

//LAS ESTRUCTURAS JUEGO, ESPECIFICACIONESPC Y REGISTROHISTORIAL SE DEFINEN EN extra.h

// Tablas de puntajes aproximados para CPUs y GPUs
typedef struct {
    char modelo[50];
    int puntaje;
} ComponenteConPuntaje;

ComponenteConPuntaje puntajes_cpu[] = {
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

ComponenteConPuntaje puntajes_gpu[] = {
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

void liberar_memoria(Map *mapa, List *lista) {
    // Liberar memoria de los juegos en la lista
    Juego *juego;
    while ((juego = list_popFront(lista)) != NULL) {
        free(juego);
    }

    // Liberar memoria del mapa (las claves son strdup)
    MapPair *pair;
    while ((pair = map_remove(mapa, list_first(lista)))) {
        free(pair->key); // Liberar la clave duplicada
        // El valor (juego) ya fue liberado en el paso anterior
    }

    // Liberar las estructuras principales
    map_destroy(mapa);
    list_clean(lista);
    free(lista);
}

// Función para obtener puntaje dado un modelo
int obtener_puntaje(ComponenteConPuntaje *tabla, int cantidad, const char *modelo_usuario)
{
    for (int i = 0; i < cantidad; i++)
    {
        if (strstr(modelo_usuario, tabla[i].modelo) != NULL)
        {
            return tabla[i].puntaje;
        }
    }
    return 0; // No encontrado o muy bajo
}

// Evaluar compatibilidad avanzada CPU+GPU+RAM
int evaluar_compatibilidad(EspecificacionesPC *pc, Juego *juego)
{
    // 1. Obtener los puntajes para los componentes del PC del usuario
    int puntaje_cpu_usuario = obtener_puntaje(puntajes_cpu, sizeof(puntajes_cpu) / sizeof(puntajes_cpu[0]), pc->cpu);
    int puntaje_gpu_usuario = obtener_puntaje(puntajes_gpu, sizeof(puntajes_gpu) / sizeof(puntajes_gpu[0]), pc->gpu);

    // 2. Obtener los puntajes para los requisitos MÍNIMOS del juego
    int puntaje_cpu_minimo = obtener_puntaje(puntajes_cpu, sizeof(puntajes_cpu) / sizeof(puntajes_cpu[0]), juego->cpu_minimo);
    int puntaje_gpu_minimo = obtener_puntaje(puntajes_gpu, sizeof(puntajes_gpu) / sizeof(puntajes_gpu[0]), juego->gpu_minimo);

    // 3. Obtener los puntajes para los requisitos RECOMENDADOS del juego
    int puntaje_cpu_recomendada = obtener_puntaje(puntajes_cpu, sizeof(puntajes_cpu) / sizeof(puntajes_cpu[0]), juego->cpu_recomendada);
    int puntaje_gpu_recomendada = obtener_puntaje(puntajes_gpu, sizeof(puntajes_gpu) / sizeof(puntajes_gpu[0]), juego->gpu_recomendada);

    // 4. Evaluar si se cumplen los requisitos RECOMENDADOS
    int cumple_recomendados = (puntaje_cpu_usuario >= puntaje_cpu_recomendada) &&
                             (puntaje_gpu_usuario >= puntaje_gpu_recomendada) &&
                             (pc->ram >= juego->ram_recomendada);

    if (cumple_recomendados) return 2; // Nivel de compatibilidad: Recomendado

    // 5. Si no cumple los recomendados, evaluar requisitos MÍNIMOS
    int cumple_minimos = (puntaje_cpu_usuario >= puntaje_cpu_minimo) &&
                        (puntaje_gpu_usuario >= puntaje_gpu_minimo) &&
                        (pc->ram >= juego->ram_minima);

    if (cumple_minimos) return 1; // Nivel de compatibilidad: Mínimos

    return 0; // Nivel de compatibilidad: No compatible
}

void agregar_al_historial(const char *nombre_usuario, const char *nombre_juego)
{
    // Crear el nuevo registro
    RegistroHistorial *nuevo_registro = malloc(sizeof(RegistroHistorial));
    strncpy(nuevo_registro->nombre_usuario, nombre_usuario, MAX_NOMBRE_USUARIO);
    strncpy(nuevo_registro->nombre_juego, nombre_juego, 100);

    // Leer el historial existente
    List *historial = list_create();
    FILE *archivo = fopen(ARCHIVO_HISTORIAL, "r");
    if (archivo)
    {
        char linea[200];
        while (fgets(linea, sizeof(linea), archivo))
        {
            linea[strcspn(linea, "\n")] = 0;
            char *usuario = strtok(linea, ";");
            char *juego = strtok(NULL, ";");

            if (usuario && juego)
            {
                RegistroHistorial *registro = malloc(sizeof(RegistroHistorial));
                strncpy(registro->nombre_usuario, usuario, MAX_NOMBRE_USUARIO);
                strncpy(registro->nombre_juego, juego, 100);
                list_pushBack(historial, registro);
            }
        }
        fclose(archivo);
    }

    // Agregar el nuevo registro al principio (FIFO)
    list_pushFront(historial, nuevo_registro);

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

    for (RegistroHistorial *registro = list_first(historial); registro != NULL; registro = list_next(historial))
    {
        fprintf(archivo, "%s;%s\n", registro->nombre_usuario, registro->nombre_juego);
    }

    fclose(archivo);
    list_clean(historial);
    free(historial);
}

void buscar_juego(Map *mapa, EspecificacionesPC *pc, const char *nombre_usuario)
{
    char nombre_juego[100];
    printf("Ingrese el nombre del juego a buscar: ");
    fgets(nombre_juego, sizeof(nombre_juego), stdin);
    nombre_juego[strcspn(nombre_juego, "\n")] = 0;

    Juego *juego = map_get(mapa, nombre_juego);
    if (juego == NULL)
    {
        printf("El juego '%s' no se encuentra en el catálogo.\n", nombre_juego);
        return;
    }

    int compatibilidad = 0;
    if (pc->ram > 0)
    {
        compatibilidad = evaluar_compatibilidad(pc, juego);
    }

    mostrar_juego_compatibilidad(juego, compatibilidad);
    agregar_al_historial(nombre_usuario, nombre_juego);
}

void mostrar_catalogo(List *lista, EspecificacionesPC *pc)
{
    printf("\n=== Catálogo Completo ===\n");
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        int compatibilidad = 0;
        if (pc->ram > 0) {
            compatibilidad = evaluar_compatibilidad(pc, juego);
        }
        mostrar_juego_compatibilidad(juego, compatibilidad);
    }
}

void ver_historial(const char *nombre_usuario)
{
    printf("\n=== Historial de búsquedas para %s ===\n", nombre_usuario);

    List *historial = list_create();
    FILE *archivo = fopen(ARCHIVO_HISTORIAL, "r");
    if (!archivo)
    {
        printf("No hay historial disponible.\n");
        return;
    }

    // Leer el archivo y cargar en lista
    char linea[200];
    while (fgets(linea, sizeof(linea), archivo))
    {
        linea[strcspn(linea, "\n")] = 0;
        char *usuario = strtok(linea, ";");
        char *juego = strtok(NULL, ";");

        if (usuario && juego)
        {
            RegistroHistorial *registro = malloc(sizeof(RegistroHistorial));
            strncpy(registro->nombre_usuario, usuario, MAX_NOMBRE_USUARIO);
            strncpy(registro->nombre_juego, juego, 100);
            list_pushBack(historial, registro);
        }
    }
    fclose(archivo);

    // Mostrar solo los del usuario
    int encontrado = 0;
    for (RegistroHistorial *registro = list_first(historial); registro != NULL; registro = list_next(historial))
    {
        if (strcmp(registro->nombre_usuario, nombre_usuario) == 0)
        {
            printf("- %s\n", registro->nombre_juego);
            encontrado = 1;
        }
    }

    if (!encontrado)
    {
        printf("No hay búsquedas registradas para este usuario.\n");
    }

    list_clean(historial);
    free(historial);
}

void agregar_juego(Map *mapa, List *lista) {
    // 1. Reservar memoria para el nuevo juego
    Juego *nuevo_juego = (Juego*)malloc(sizeof(Juego));
    if (nuevo_juego == NULL) {
        printf("Error: No se pudo asignar memoria para el nuevo juego.\n");
        return;
    }

    // 2. Solicitar datos del juego
    printf("\n=== Agregar nuevo juego ===\n");
    
    // 2.1. Nombre del juego
    printf("Nombre del juego: ");
    fgets(nuevo_juego->nombre, 100, stdin);
    nuevo_juego->nombre[strcspn(nuevo_juego->nombre, "\n")] = '\0';  // Eliminar salto de línea

    // 3. Verificar si el juego ya existe
    if (map_search(mapa, nuevo_juego->nombre) != NULL) {
        printf("Error: El juego '%s' ya existe en el catálogo.\n", nuevo_juego->nombre);
        free(nuevo_juego);
        return;
    }

    // 4. Solicitar requisitos mínimos
    printf("CPU mínimo (ej: i5-4690K): ");
    fgets(nuevo_juego->cpu_minimo, 50, stdin);
    nuevo_juego->cpu_minimo[strcspn(nuevo_juego->cpu_minimo, "\n")] = '\0';

    printf("GPU mínimo (ej: GTX 1060): ");
    fgets(nuevo_juego->gpu_minimo, 50, stdin);
    nuevo_juego->gpu_minimo[strcspn(nuevo_juego->gpu_minimo, "\n")] = '\0';

    // 4.1. Validar RAM mínima
    int ram_valida = 0;
    while (!ram_valida) {
        printf("RAM mínima (GB, mínimo 1): ");
        if (scanf("%d", &nuevo_juego->ram_minima) != 1) {
            printf("Entrada inválida. ");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        while (getchar() != '\n'); // Limpiar buffer
        
        if (nuevo_juego->ram_minima < 1) {
            printf("La RAM debe ser al menos 1GB. ");
        } else {
            ram_valida = 1;
        }
    }

    // 5. Solicitar requisitos recomendados
    printf("CPU recomendada (ej: i7-7700K): ");
    fgets(nuevo_juego->cpu_recomendada, 50, stdin);
    nuevo_juego->cpu_recomendada[strcspn(nuevo_juego->cpu_recomendada, "\n")] = '\0';

    printf("GPU recomendada (ej: RTX 2060): ");
    fgets(nuevo_juego->gpu_recomendada, 50, stdin);
    nuevo_juego->gpu_recomendada[strcspn(nuevo_juego->gpu_recomendada, "\n")] = '\0';

    // 5.1. Validar RAM recomendada
    ram_valida = 0;
    while (!ram_valida) {
        printf("RAM recomendada (GB, mínimo %d): ", nuevo_juego->ram_minima);
        if (scanf("%d", &nuevo_juego->ram_recomendada) != 1) {
            printf("Entrada inválida. ");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        if (nuevo_juego->ram_recomendada < nuevo_juego->ram_minima) {
            printf("La RAM recomendada no puede ser menor que la mínima (%dGB). ", nuevo_juego->ram_minima);
        } else {
            ram_valida = 1;
        }
    }

    // 6. Insertar el juego en las estructuras de datos
    char *clave = strdup(nuevo_juego->nombre);
    if (clave == NULL) {
        printf("Error: No se pudo duplicar el nombre del juego.\n");
        free(nuevo_juego);
        return;
    }

    // 6.1. Insertar en el mapa (usando la versión actual de map_insert)
    map_insert(mapa, clave, nuevo_juego);
    
    // 6.2. Insertar en la lista
    list_pushBack(lista, nuevo_juego);

    // 7. Confirmación
    printf("\nJuego '%s' agregado exitosamente!\n", nuevo_juego->nombre);
    printf("Requisitos mínimos: %s, %s, %dGB RAM\n", 
           nuevo_juego->cpu_minimo, nuevo_juego->gpu_minimo, nuevo_juego->ram_minima);
    printf("Requisitos recomendados: %s, %s, %dGB RAM\n", 
           nuevo_juego->cpu_recomendada, nuevo_juego->gpu_recomendada, nuevo_juego->ram_recomendada);
}

void guardar_catalogo(List *lista)
{
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "w");
    if (!archivo) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista))
    {
        fprintf(archivo, "%s,%s,%s,%d,%s,%s,%d\n",
                juego->nombre,
                juego->cpu_minimo,
                juego->gpu_minimo,
                juego->ram_minima,
                juego->cpu_recomendada,
                juego->gpu_recomendada,
                juego->ram_recomendada);
    }
    fclose(archivo);
    printf("Catálogo guardado exitosamente.\n");
}

void mostrar_juego_compatibilidad(Juego *juego, int compatibilidad)
{
    printf("\n%s\n", juego->nombre);
    printf("  Mínimos: CPU: %s | GPU: %s | RAM: %dGB\n", juego->cpu_minimo, juego->gpu_minimo, juego->ram_minima);
    printf("  Recomendados: CPU: %s | GPU: %s | RAM: %dGB\n", juego->cpu_recomendada, juego->gpu_recomendada, juego->ram_recomendada);
    printf("  Compatibilidad: ");
    if (compatibilidad == 2)
        printf("Cumple recomendados\n");
    else if (compatibilidad == 1)
        printf("Cumple mínimos\n");
    else
        printf("No compatible\n");
}

// Función auxiliar para verificar si un juego ya existe
int juego_existe(Map *mapa, char *nombre_juego) {
    if (!mapa || !nombre_juego) {
        return 0;
    }
    
    return map_get(mapa, nombre_juego) != NULL;
}

void cargar_catalogo(Map *mapa, List *lista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de catálogo. Se creará uno nuevo al guardar.\n");
        return;
    }
    
    int juegos_cargados = 0;
    char linea[256];
    
    while (fgets(linea, sizeof(linea), archivo)) {
        char *token;
        char *campos[7];
        int i = 0;
        
        token = strtok(linea, ",");
        while (token != NULL && i < 7) {
            campos[i++] = token;
            token = strtok(NULL, ",");
        }
        
        if (i < 7) {
            printf("Error: Formato incorrecto en línea del archivo.\n");
            continue;
        }
        
        // Verificar si el juego ya existe
        if (map_get(mapa, campos[0]) != NULL) {
            printf("Advertencia: El juego '%s' ya existe y será omitido.\n", campos[0]);
            continue;
        }

        Juego *juego = malloc(sizeof(Juego));
        if (!juego) {
            printf("Error: Memoria insuficiente.\n");
            break;
        }
        
        // Copiar todos los campos
        strncpy(juego->nombre, campos[0], 99);
        juego->nombre[99] = '\0';
        
        strncpy(juego->cpu_minimo, campos[1], 49);
        juego->cpu_minimo[49] = '\0';
        
        strncpy(juego->gpu_minimo, campos[2], 49);
        juego->gpu_minimo[49] = '\0';
        
        juego->ram_minima = atoi(campos[3]);
        
        strncpy(juego->cpu_recomendada, campos[4], 49);
        juego->cpu_recomendada[49] = '\0';
        
        strncpy(juego->gpu_recomendada, campos[5], 49);
        juego->gpu_recomendada[49] = '\0';
        
        juego->ram_recomendada = atoi(campos[6]);

        // Insertar en el mapa y lista
        char *clave = strdup(juego->nombre);
        if (!clave) {
            printf("Error: No se pudo duplicar el nombre del juego.\n");
            free(juego);
            continue;
        }
        
        map_insert(mapa, clave, juego);
        list_pushBack(lista, juego);
        juegos_cargados++;
    }
    
    fclose(archivo);
    printf("Se cargaron %d juegos correctamente.\n", juegos_cargados);
}

int comparar_juegos_compatibilidad(const void *a, const void *b)
{
    const Juego *juegoA = *(const Juego **)a;
    const Juego *juegoB = *(const Juego **)b;
    EspecificacionesPC *pc = (EspecificacionesPC *)juegoA->cpu_minimo; // Truco para pasar el PC

    int compatA = evaluar_compatibilidad(pc, (Juego *)juegoA);
    int compatB = evaluar_compatibilidad(pc, (Juego *)juegoB);

    // Orden ascendente: 0 (no compatibles) primero, luego 1 (mínimos), luego 2 (recomendados)
    if (compatA < compatB) return -1;
    if (compatA > compatB) return 1;
    return 0;
}

void ver_juegos_compatibles(List *lista, EspecificacionesPC *pc)
{
    // Crear un arreglo temporal para ordenar
    int cantidad_juegos = list_size(lista);
    Juego **juegos_ordenados = malloc(cantidad_juegos * sizeof(Juego *));

    // Copiar los juegos al arreglo
    int i = 0;
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) 
    {
        juegos_ordenados[i++] = juego;
    }

    // Usar qsort para ordenar
    qsort(juegos_ordenados, cantidad_juegos, sizeof(Juego *), comparar_juegos_compatibilidad);

    // Mostrar los juegos compatibles
    printf("\n=== Juegos compatibles ===\n");

    // Primero mostrar no compatibles (compatibilidad 0)
    for (i = 0; i < cantidad_juegos; i++) 
    {
        int compatibilidad = evaluar_compatibilidad(pc, juegos_ordenados[i]);
        if (compatibilidad == 0) {
            mostrar_juego_compatibilidad(juegos_ordenados[i], compatibilidad);
        }
    }

    // Luego mostrar que cumplen mínimos (compatibilidad 1)
    for (i = 0; i < cantidad_juegos; i++) 
    {
        int compatibilidad = evaluar_compatibilidad(pc, juegos_ordenados[i]);
        if (compatibilidad == 1) {
            mostrar_juego_compatibilidad(juegos_ordenados[i], compatibilidad);
        }
    }

    // Finalmente mostrar recomendados (compatibilidad 2)
    for (i = 0; i < cantidad_juegos; i++) 
    {
        int compatibilidad = evaluar_compatibilidad(pc, juegos_ordenados[i]);
        if (compatibilidad == 2) {
            mostrar_juego_compatibilidad(juegos_ordenados[i], compatibilidad);
        }
    }

    free(juegos_ordenados);
}

// Mejorar la función ingresar_especificaciones
void ingresar_especificaciones(EspecificacionesPC *pc) 
{
    printf("Ingrese CPU (ej: i7-4790): ");
    fgets(pc->cpu, 50, stdin);
    pc->cpu[strcspn(pc->cpu, "\n")] = 0;

    printf("Ingrese GPU (ej: GTX 1060): ");
    fgets(pc->gpu, 50, stdin);
    pc->gpu[strcspn(pc->gpu, "\n")] = 0;

    // Validar entrada de RAM
    int ram_valida = 0;
    while (!ram_valida)
    {
        printf("Ingrese RAM (GB, mínimo 1): ");
        if (scanf("%d", &pc->ram) != 1) {
            printf("Entrada inválida. ");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }
        while (getchar() != '\n');

        if (pc->ram < 1)
        {
            printf("La RAM debe ser al menos 1GB. ");
        } else
        {
            ram_valida = 1;
        }
    }
}

void eliminar_juegos_repetidos(Map *mapa, List *lista) {
    // Crear un mapa temporal para detectar duplicados
    Map *mapa_temporal = map_create(NULL); // Usamos NULL porque no necesitamos función de comparación
    
    List *lista_unica = list_create(); // Lista para almacenar juegos únicos
    int juegos_eliminados = 0;

    printf("\n=== Eliminando juegos duplicados ===\n");

    // Recorrer la lista original
    Juego *juego_actual = list_first(lista);
    while (juego_actual != NULL) {
        // Verificar si el juego ya está en el mapa temporal
        if (map_search(mapa_temporal, juego_actual->nombre) == NULL) {
            // Si no está, agregarlo al mapa y a la lista única
            char *clave = strdup(juego_actual->nombre);
            if (clave == NULL) {
                printf("Error: Memoria insuficiente para procesar '%s'\n", juego_actual->nombre);
                continue;
            }
            
            map_insert(mapa_temporal, clave, juego_actual);
            list_pushBack(lista_unica, juego_actual);
        } else {
            // Si está duplicado, contarlo para el reporte
            juegos_eliminados++;
            printf("Encontrado duplicado: %s\n", juego_actual->nombre);
        }
        
        juego_actual = list_next(lista);
    }

    // Actualizar la lista original
    list_clean(lista); // Limpiar la lista original
    
    // Copiar los elementos únicos de vuelta a la lista original
    Juego *juego_unico = list_first(lista_unica);
    while (juego_unico != NULL) {
        list_pushBack(lista, juego_unico);
        juego_unico = list_next(lista_unica);
    }

    // Actualizar el mapa principal (opcional, si es necesario)
    // map_clean(mapa);
    // Copiar los elementos del mapa temporal al principal

    // Liberar recursos temporales
    list_clean(lista_unica); // Solo limpia los nodos, no los juegos
    free(lista_unica);
    
    // No liberamos los juegos del mapa temporal porque son los mismos que quedaron en la lista
    map_clean(mapa_temporal);
    free(mapa_temporal);

    printf("\nResumen:\n");
    printf("- Juegos únicos conservados: %d\n", list_size(lista));
    printf("- Juegos duplicados eliminados: %d\n", juegos_eliminados);
}

void menu_principal() {
    system("chcp 65001");
    system("cls");

    Map *mapa = sorted_map_create((int (*)(void *, void *))strcmp);
    List *lista = list_create();

    cargar_catalogo(mapa, lista);

    EspecificacionesPC pc = {"", "", 0};
    char nombre_usuario[MAX_NOMBRE_USUARIO];
    printf("Ingrese su nombre de usuario: ");
    fgets(nombre_usuario, MAX_NOMBRE_USUARIO, stdin);
    nombre_usuario[strcspn(nombre_usuario, "\n")] = 0;

    int opcion;
    do {
        printf("\n--- Menú Principal ---\n");
        printf("1) Ver catálogo\n");
        printf("2) Ingresar especificaciones PC\n");
        printf("3) Ver juegos compatibles\n");
        printf("4) Buscar juego\n");
        printf("5) Ver historial de búsqueda\n");
        printf("6) Agregar juego al catálogo\n");
        printf("7) Salir\n");
        printf("Seleccione opción: ");
        scanf("%d", &opcion);
        while (getchar() != '\n');

        switch (opcion) {
        case 1:
            mostrar_catalogo(lista, &pc);
            break;
        case 2:
            ingresar_especificaciones(&pc);
            break;
        case 3:
            if (pc.ram == 0) {
                printf("Primero ingrese sus especificaciones.\n");
            } else {
                ver_juegos_compatibles(lista, &pc);
            }
            break;
        case 4:
            buscar_juego(mapa, &pc, nombre_usuario);
            break;
        case 5:
            ver_historial(nombre_usuario);
            break;
        case 6:
            agregar_juego(mapa, lista);
            break;
        case 7:
            eliminar_juegos_repetidos(mapa, lista);
            guardar_catalogo(lista);
            liberar_memoria(mapa, lista); // Liberar memoria del mapa y lista
            printf("Saliendo...\n");
            break;
        default:
            printf("Opción inválida.\n");
        }
    } while (opcion != 7);

    // Liberar memoria
    // (Implementar función para liberar memoria de mapa y lista)
}

int main() {
    menu_principal();
    return 0;
}