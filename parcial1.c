#include <stdio.h>

// Definición de la estructura
typedef struct {
    int codigo;
    float nota;
} Estudiante;

// Datos predefinidos de los estudiantes
Estudiante estudiantes[] = {
    {2024101, 4.5},
    {2024102, 3.2},
    {2024103, 2.8},
    {2024104, 4.0},
    {2024105, 3.7}
};
int n = sizeof(estudiantes) / sizeof(estudiantes[0]);

// --- Funciones Recursivas ---

// a) Nota máxima
float notaMaxima(Estudiante arr[], int n) {
    if (n == 1) return arr[0].nota;
    float maxRestante = notaMaxima(arr, n - 1);
    return (arr[n - 1].nota > maxRestante) ? arr[n - 1].nota : maxRestante;
}

// b) Promedio
float promedio(Estudiante arr[], int n) {
    if (n == 1) return arr[0].nota;
    return (promedio(arr, n - 1) * (n - 1) + arr[n - 1].nota) / n;
}

// c) Selection Sort recursivo por código
int indiceMinimo(Estudiante arr[], int i, int j) {
    if (i == j) return i;
    int k = indiceMinimo(arr, i + 1, j);
    return (arr[i].codigo < arr[k].codigo) ? i : k;
}

void selectionSortRecursivo(Estudiante arr[], int n, int index) {
    if (index == n) return;
    int k = indiceMinimo(arr, index, n - 1);
    if (k != index) {
        Estudiante temp = arr[k];
        arr[k] = arr[index];
        arr[index] = temp;
    }
    selectionSortRecursivo(arr, n, index + 1);
}

// d) Mostrar estudiantes
void mostrarEstudiantes(Estudiante arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Codigo: %d - Nota: %.2f\n", arr[i].codigo, arr[i].nota);
    }
}

// --- Menú Principal ---
int main() {
    int opcion;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Nota maxima del curso\n");
        printf("2. Promedio del curso\n");
        printf("3. Ordenar por codigo (Selection Sort Recursivo)\n");
        printf("4. Mostrar estudiantes\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                printf("Nota maxima: %.2f\n", notaMaxima(estudiantes, n));
                break;
            case 2:
                printf("Promedio del curso: %.2f\n", promedio(estudiantes, n));
                break;
            case 3:
                selectionSortRecursivo(estudiantes, n, 0);
                printf("Lista ordenada:\n");
                mostrarEstudiantes(estudiantes, n);
                break;
            case 4:
                printf("Lista de estudiantes:\n");
                mostrarEstudiantes(estudiantes, n);
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while(opcion != 5);

    return 0;
}
