#include "extra.h"
#include "list.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_CATALOGO "videojuegosDos.csv"
#define ARCHIVO_HISTORIAL "historial.txt"
#define MAX_NOMBRE_USUARIO 50
#define MAX_HISTORIAL 100

typedef struct {
    char nombre[100];
    char cpu_minimo[50];
    char gpu_minimo[50];
    int ram_minima;
    char cpu_recomendada[50];
    char gpu_recomendada[50];
    int ram_recomendada;
} Juego;

typedef struct {
    char cpu[50];
    char gpu[50];
    int ram;
} EspecificacionesPC;

typedef struct {
    char nombre_usuario[MAX_NOMBRE_USUARIO];
    char nombre_juego[100];
} RegistroHistorial;

// Tablas de puntajes aproximados para CPUs y GPUs
typedef struct {
    char modelo[50];
    int puntaje;
} ComponenteConPuntaje;

ComponenteConPuntaje puntajes_cpu[] = {
    // ... (contenido igual pero con comentarios en español)
};

ComponenteConPuntaje puntajes_gpu[] = {
    // ... (contenido igual pero con comentarios en español)
};

// Función para obtener puntaje dado un modelo
int obtener_puntaje(ComponenteConPuntaje *tabla, int cantidad, const char *modelo_usuario) {
    for (int i = 0; i < cantidad; i++) {
        if (strstr(modelo_usuario, tabla[i].modelo) != NULL) {
            return tabla[i].puntaje;
        }
    }
    return 0; // No encontrado o muy bajo
}

// Evaluar compatibilidad avanzada CPU+GPU+RAM
int evaluar_compatibilidad(EspecificacionesPC *pc, Juego *juego) {
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

void agregar_al_historial(const char *nombre_usuario, const char *nombre_juego) {
    // Crear el nuevo registro
    RegistroHistorial *nuevo_registro = malloc(sizeof(RegistroHistorial));
    strncpy(nuevo_registro->nombre_usuario, nombre_usuario, MAX_NOMBRE_USUARIO);
    strncpy(nuevo_registro->nombre_juego, nombre_juego, 100);
    
    // Leer el historial existente
    List *historial = list_create();
    FILE *archivo = fopen(ARCHIVO_HISTORIAL, "r");
    if (archivo) {
        char linea[200];
        while (fgets(linea, sizeof(linea), archivo)) {
            linea[strcspn(linea, "\n")] = 0;
            char *usuario = strtok(linea, ";");
            char *juego = strtok(NULL, ";");
            
            if (usuario && juego) {
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
    while (list_size(historial) > MAX_HISTORIAL) {
        free(list_popBack(historial));
    }
    
    // Escribir el historial actualizado
    archivo = fopen(ARCHIVO_HISTORIAL, "w");
    if (!archivo) {
        list_clean(historial);
        free(historial);
        return;
    }
    
    for (RegistroHistorial *registro = list_first(historial); registro != NULL; registro = list_next(historial)) {
        fprintf(archivo, "%s;%s\n", registro->nombre_usuario, registro->nombre_juego);
    }
    
    fclose(archivo);
    list_clean(historial);
    free(historial);
}

void buscar_juego(Map *mapa, EspecificacionesPC *pc, const char *nombre_usuario) {
    char nombre_juego[100];
    printf("Ingrese el nombre del juego a buscar: ");
    fgets(nombre_juego, sizeof(nombre_juego), stdin);
    nombre_juego[strcspn(nombre_juego, "\n")] = 0;

    Juego *juego = map_get(mapa, nombre_juego);
    if (juego == NULL) {
        printf("El juego '%s' no se encuentra en el catálogo.\n", nombre_juego);
        return;
    }

    int compatibilidad = 0;
    if (pc->ram > 0) {
        compatibilidad = evaluar_compatibilidad(pc, juego);
    }

    mostrar_juego_compatibilidad(juego, compatibilidad);
    agregar_al_historial(nombre_usuario, nombre_juego);
}

void mostrar_catalogo(List *lista, EspecificacionesPC *pc) {
    printf("\n=== Catálogo Completo ===\n");
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        int compatibilidad = 0;
        if (pc->ram > 0) {
            compatibilidad = evaluar_compatibilidad(pc, juego);
        }
        mostrar_juego_compatibilidad(juego, compatibilidad);
    }
}

void ver_historial(const char *nombre_usuario) {
    printf("\n=== Historial de búsquedas para %s ===\n", nombre_usuario);
    
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
        char *usuario = strtok(linea, ";");
        char *juego = strtok(NULL, ";");
        
        if (usuario && juego) {
            RegistroHistorial *registro = malloc(sizeof(RegistroHistorial));
            strncpy(registro->nombre_usuario, usuario, MAX_NOMBRE_USUARIO);
            strncpy(registro->nombre_juego, juego, 100);
            list_pushBack(historial, registro);
        }
    }
    fclose(archivo);
    
    // Mostrar solo los del usuario
    int encontrado = 0;
    for (RegistroHistorial *registro = list_first(historial); registro != NULL; registro = list_next(historial)) {
        if (strcmp(registro->nombre_usuario, nombre_usuario) == 0) {
            printf("- %s\n", registro->nombre_juego);
            encontrado = 1;
        }
    }
    
    if (!encontrado) {
        printf("No hay búsquedas registradas para este usuario.\n");
    }
    
    list_clean(historial);
    free(historial);
}

void agregar_juego(Map *mapa, List *lista) {
    Juego *nuevo_juego = malloc(sizeof(Juego));

    printf("\n=== Agregar nuevo juego ===\n");
    printf("Nombre del juego: ");
    fgets(nuevo_juego->nombre, 100, stdin);
    nuevo_juego->nombre[strcspn(nuevo_juego->nombre, "\n")] = 0;

    printf("CPU mínimo: ");
    fgets(nuevo_juego->cpu_minimo, 50, stdin);
    nuevo_juego->cpu_minimo[strcspn(nuevo_juego->cpu_minimo, "\n")] = 0;

    printf("GPU mínimo: ");
    fgets(nuevo_juego->gpu_minimo, 50, stdin);
    nuevo_juego->gpu_minimo[strcspn(nuevo_juego->gpu_minimo, "\n")] = 0;

    printf("RAM mínima (GB): ");
    scanf("%d", &nuevo_juego->ram_minima);
    while (getchar() != '\n');

    printf("CPU recomendada: ");
    fgets(nuevo_juego->cpu_recomendada, 50, stdin);
    nuevo_juego->cpu_recomendada[strcspn(nuevo_juego->cpu_recomendada, "\n")] = 0;

    printf("GPU recomendada: ");
    fgets(nuevo_juego->gpu_recomendada, 50, stdin);
    nuevo_juego->gpu_recomendada[strcspn(nuevo_juego->gpu_recomendada, "\n")] = 0;

    printf("RAM recomendada (GB): ");
    scanf("%d", &nuevo_juego->ram_recomendada);
    while (getchar() != '\n');

    list_pushBack(lista, nuevo_juego);
    map_insert(mapa, strdup(nuevo_juego->nombre), nuevo_juego);

    printf("Juego agregado exitosamente!\n");
}

void guardar_catalogo(List *lista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "w");
    if (!archivo) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
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

void mostrar_juego_compatibilidad(Juego *juego, int compatibilidad) {
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

void cargar_catalogo(Map *mapa, List *lista) {
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de catálogo.\n");
        return;
    }
    char **campos;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Juego *juego = malloc(sizeof(Juego));
        strncpy(juego->nombre, campos[0], 99);
        strncpy(juego->cpu_minimo, campos[1], 49);
        strncpy(juego->gpu_minimo, campos[2], 49);
        juego->ram_minima = atoi(campos[3]);
        strncpy(juego->cpu_recomendada, campos[4], 49);
        strncpy(juego->gpu_recomendada, campos[5], 49);
        juego->ram_recomendada = atoi(campos[6]);
        list_pushBack(lista, juego);
        map_insert(mapa, strdup(juego->nombre), juego);
    }
    fclose(archivo);
}

void ver_juegos_compatibles(List *lista, EspecificacionesPC *pc) {
    typedef struct {
        Juego *juego;
        int compatibilidad;
    } JuegoCompatibilidad;

    List *compatibles = list_create();
    for (Juego *juego = list_first(lista); juego != NULL; juego = list_next(lista)) {
        int comp = evaluar_compatibilidad(pc, juego);
        if (comp > 0) {
            JuegoCompatibilidad *jc = malloc(sizeof(JuegoCompatibilidad));
            jc->juego = juego;
            jc->compatibilidad = comp;
            list_pushBack(compatibles, jc);
        }
    }

    // Ordenar compatibles por compatibilidad (descendente)
    int cantidad = 0;
    for (JuegoCompatibilidad *jc = list_first(compatibles); jc; jc = list_next(compatibles))
        cantidad++;
    for (int i = 0; i < cantidad - 1; i++) {
        JuegoCompatibilidad *jc1 = list_first(compatibles);
        for (int j = 0; j < cantidad - i - 1; j++) {
            JuegoCompatibilidad *jc2 = list_next(compatibles);
            if (jc2->compatibilidad > jc1->compatibilidad) {
                JuegoCompatibilidad tmp = *jc1;
                *jc1 = *jc2;
                *jc2 = tmp;
            }
            jc1 = jc2;
        }
    }

    printf("\n=== Juegos compatibles ===\n");
    for (JuegoCompatibilidad *jc = list_first(compatibles); jc; jc = list_next(compatibles)) {
        mostrar_juego_compatibilidad(jc->juego, jc->compatibilidad);
    }

    // Liberar memoria auxiliar
    for (JuegoCompatibilidad *jc = list_first(compatibles); jc; jc = list_next(compatibles))
        free(jc);
    free(compatibles);
}

void ingresar_especificaciones(EspecificacionesPC *pc) {
    printf("Ingrese CPU (ej: i7-4790): ");
    fgets(pc->cpu, 50, stdin);
    pc->cpu[strcspn(pc->cpu, "\n")] = 0;
    printf("Ingrese GPU (ej: GTX 1060): ");
    fgets(pc->gpu, 50, stdin);
    pc->gpu[strcspn(pc->gpu, "\n")] = 0;
    printf("Ingrese RAM (GB): ");
    scanf("%d", &pc->ram);
    while (getchar() != '\n');
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
            guardar_catalogo(lista);
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