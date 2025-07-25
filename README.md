# GAMECHECK - Verificación de Requisitos de Videojuegos

**Curso:** ICI2240-3 - Facultad de Ingeniería  
**Profesor:** Fabián Pizarro  
**Fecha de entrega:** Viernes, 27 de junio de 2025

## Descripción de la Aplicación

**GAMECHECK** es una aplicación desarrollada en C que permite a los usuarios verificar si su computadora cumple con los requisitos mínimos o recomendados para ejecutar diversos videojuegos. Los usuarios pueden iniciar sesión, guardar un historial de sus búsquedas y comparar las especificaciones de su PC con los requisitos de un catálogo de videojuegos.

## Tabla de contenidos

- [GAMECHECK - Verificación de Requisitos de Videojuegos](#gamecheck---verificación-de-requisitos-de-videojuegos)
  - [Descripción de la Aplicación](#descripción-de-la-aplicación)
  - [Tabla de contenidos](#tabla-de-contenidos)
  - [Funcionalidades](#funcionalidades)
  - [Limitaciones de la aplicación](#limitaciones-de-la-aplicación)
  - [Interacción](#interacción)
  - [Estructuras de Datos](#estructuras-de-datos)
  - [Tipos de Datos Abstractos (TDAs)](#tipos-de-datos-abstractos-tdas)
    - [Listas:](#listas)
    - [Mapas (Hashmaps):](#mapas-hashmaps)
  - [Flujo de la Aplicación](#flujo-de-la-aplicación)
    - [Carga Inicial del Catálogo:](#carga-inicial-del-catálogo)
    - [Gestión de Especificaciones del Usuario:](#gestión-de-especificaciones-del-usuario)
    - [Comparación y Puntuación de Juegos:](#comparación-y-puntuación-de-juegos)
    - [Persistencia de Datos:](#persistencia-de-datos)
  - [Desafíos y Consideraciones](#desafíos-y-consideraciones)
    - [Comparación de Requisitos:](#comparación-de-requisitos)
    - [Gestión del Historial de Búsqueda:](#gestión-del-historial-de-búsqueda)
    - [Persistencia de Datos:](#persistencia-de-datos-1)
  - [Compilación](#compilación)
  - [Autores](#autores)

## Funcionalidades

1. **Inicio de sesión:**  
   El usuario ingresa su nombre y la aplicación carga su historial de búsquedas si existe.

2. **Catálogo de videojuegos:**  
   Se muestra una lista de juegos con sus requisitos mínimos y recomendados de hardware.

3. **Especificaciones del PC:**  
   El usuario puede ingresar las especificaciones de su computadora (CPU, GPU, RAM) para verificar la compatibilidad con los juegos.

4. **Ver juegos compatibles:**  
   La aplicación compara las especificaciones del PC del usuario con los requisitos de cada juego en el catálogo y asigna una puntuación para indicar la compatibilidad.

5. **Historial de búsqueda:**  
   El usuario puede ver un historial de los juegos que ha buscado anteriormente.

6. **Buscar un juego:**  
   El usuario puede buscar un juego en el catálogo. Si el juego no está en el catálogo, se le ofrece la opción de agregarlo manualmente.

7. **Agregar juego al catálogo:**  
   Los usuarios pueden agregar nuevos juegos al catálogo con sus requisitos mínimos y recomendados.

8. **Salir:**  
   Guarda el historial y finaliza la aplicación.

## Limitaciones de la aplicación

- La búsqueda de juegos debe hacerse con el nombre exacto, utilizando mayúscula al inicio de cada palabra.

- No permite eliminar ni editar juegos del catálogo una vez agregados.

- No se validan completamente los nombres ingresados de CPU o GPU, lo que puede causar coincidencias imprecisas en la puntuación.

- No considera resoluciones gráficas, sistema operativo ni otras especificaciones comunes (como DirectX o almacenamiento).

- La comparación entre componentes se basa únicamente en coincidencias parciales de texto (strstr), lo cual puede generar errores si se ingresan nombres poco específicos.

- La aplicación no posee una interfaz gráfica avanzada; se limita a una interfaz por consola.

- Solo se guarda el historial de búsqueda, no el resultado detallado de la comparación (por diseño).

## Interacción

El usuario será guiado a través de un menú en consola para acceder a las diferentes funcionalidades.

Los datos de los juegos y el historial de búsqueda se guardarán en archivos CSV (videojuegos.csv, historial\_[usuario].csv).

## Estructuras de Datos

La aplicación utiliza las siguientes estructuras de datos:

**Juego:**

```c
typedef struct {
    char nombre[100];
    char cpu_min[50];
    char gpu_min[50];
    int ram_min;
    char cpu_rec[50];
    char gpu_rec[50];
    int ram_rec;
} Juego;
```

**Especificaciones del PC:**

```c
typedef struct{
    char cpu[50];
    char gpu[50];
    int ram;
} EspecificacionesPC;
```

**Componente con el Puntaje**

```
typedef struct {
    char modelo[50];
    int puntaje;
} ComponenteConPuntaje;
```

## Tipos de Datos Abstractos (TDAs)

### Listas:

Se utiliza una lista para almacenar y recorrer los juegos del catálogo. Esta estructura permite un acceso rápido por índice O(1) y un recorrido secuencial O(n), adecuado para el tamaño moderado del catálogo.

### Mapas (Hashmaps):

Se emplean mapas para gestionar el historial de búsqueda de cada usuario, optimizando el acceso a datos específicos por usuario con un tiempo promedio O(1).

### Carga Inicial del Catálogo:

La aplicación lee el archivo videojuegos.csv al iniciar, cargando los juegos en memoria.

### Gestión de Especificaciones del Usuario:

El usuario ingresa las especificaciones de su PC, que se almacenan temporalmente para las comparaciones.

### Comparación y Puntuación de Juegos:

Para verificar la compatibilidad, se compara el PC del usuario con los requisitos del juego y se asigna una puntuación:

```
2 Pts: Cumple los requisitos recomendados

1 Pts: Cumple los requisitos mínimos

0 Pts: No cumple
```

### Persistencia de Datos:

Los datos de los juegos y el historial de búsqueda se almacenan en archivos CSV para persistencia a largo plazo.

## Desafíos y Consideraciones

# Dificultad por ambigüedad (Intel i5 vs 10th gen).
01. Comparar CPU/GPU:
   |Requiere eficiencia, separación clara y persistencia.
02. Manejo del historial por usuario:
   |Desarrollo de sistema de puntuación.
03. Ordenamiento por compatibilidad:
   |Aplicación de algoritmo de ordenamiento.
04. Interfaz en consola:
   |Validaciones, múltiples menús, interacción fluida.

### Comparación de Requisitos:

La interpretación de los requisitos de CPU y GPU puede ser ambigua, especialmente con modelos de generaciones diferentes.

### Gestión del Historial de Búsqueda:

Se requiere una estructura eficiente para gestionar y acceder al historial de búsquedas por usuario utilizando mapas.

### Persistencia de Datos:

Es necesario mantener la sincronización entre los datos en memoria y los archivos CSV, garantizando que los nuevos juegos o búsquedas se guarden correctamente.

## Compilación

Para compilar el programa en C, ejecuta el siguiente comando:

```bash
gcc main.c list.c map.c extra.c -o Gamecheck.exe
```

O

```bash
gcc *.c -o Gamecheck.exe
```

Para ejecutar el programa:

```bash
./gamecheck
```

## Autores
// Por errores en el login del Git, muchos de los cambios no fueron aplicados por los nombres de cada uno, si no por "Your name".

Rafaela Bascuñán - Coordinadora, Realizo parte de main.c, creo csv con juegos además de implementar funciones para leerlo. Por ultimo, creo parte del extra.c y extra.h.

Amanda Ramírez - Comunicadora, Realizo las listas (list.c y list.h) y funciones de main.c además de verificar la implementacion de comentarios en gran parte del programa. Por último terminó los archivos extra.c y extra.h.

Alejandro Muñoz - Gestor de Calidad, Realizo parte del main.c y la creacion del historial del usuario, se encargo de crear el README.md y afinar la terminal de interaccion del usuario.

Fernando Cárdenas - Responsable de Integración y Consistencia, Realizo parte del main.c e implementar los mapas (map.c y map.h), ademas de verificar que los csv se esten creando correctamente o leyendo correctamente. Por último, verifico si se leían correctamente las comas en el terminal.
