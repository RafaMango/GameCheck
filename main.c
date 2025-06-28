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
    // ... (contenido igual pero con comentarios en español)
};

ComponenteConPuntaje puntajes_gpu[] = {
    // ... (contenido igual pero con comentarios en español)
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
    //map_destroy(mapa);
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

// Mejorar cargar_catalogo
void cargar_catalogo(Map *mapa, List *lista)
{
    FILE *archivo = fopen(ARCHIVO_CATALOGO, "r");
    if (!archivo)
    {
        printf("No se pudo abrir el archivo de catálogo. Se creará uno nuevo al guardar.\n");
        return;
    }
    
    int juegos_cargados = 0;
    char **campos;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL)
    {
        if (campos[0] == NULL || campos[1] == NULL || campos[2] == NULL || 
            campos[3] == NULL || campos[4] == NULL || campos[5] == NULL || campos[6] == NULL) {
            printf("Error: Formato inválido en línea del archivo. Se omitirá.\n");
            continue;
        }
        
        Juego *juego = malloc(sizeof(Juego));
        if (!juego)
        {
            printf("Error: Memoria insuficiente.\n");
            fclose(archivo);
            return;
        }
        
        // Asignación directa sin validación (riesgoso)
        strncpy(juego->nombre, campos[0], 99);
        strncpy(juego->cpu_minimo, campos[1], 49);
        strncpy(juego->gpu_minimo, campos[2], 49);
        juego->ram_minima = atoi(campos[3]); // Sin validar
        strncpy(juego->cpu_recomendada, campos[4], 49);
        strncpy(juego->gpu_recomendada, campos[5], 49);
        juego->ram_recomendada = atoi(campos[6]); // Sin validar
        
        list_pushBack(lista, juego);
        map_insert(mapa, strdup(juego->nombre), juego);
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