#include <iostream>
using namespace std;

 int N = 10;

// Función para llenar el arreglo
void llenarArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int valor;
        do {
            cout << "Ingrese el valor positivo #" << i + 1 << ": ";
            cin >> valor;
            if (valor <= 0) {
                cout << "El valor debe ser positivo, intente de nuevo.\n";
            }
        } while (valor <= 0);
        arr[i] = valor;
    }
}

// Función para mostrar el arreglo
void mostrarArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Ordenamiento Selection Sort ascendente
void ordenarSelectionAsc(int arr[], int n) {
    cout << "\n--- Ordenando con Selection Sort Ascendente ---\n";
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // intercambio
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;

        cout << "Paso " << i + 1 << ": ";
        mostrarArray(arr, n);
    }
}

// Ordenamiento Insertion Sort descendente
void ordenarInsertionDesc(int arr[], int n) {
    cout << "\n--- Ordenando con Insertion Sort Descendente ---\n";
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // voy moviendo los mayores hacia la derecha
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;

        cout << "Paso " << i << ": ";
        mostrarArray(arr, n);
    }
}

// Programa principal con menú
int main() {
    int arreglo[N];
    int copia[N]; 
    bool lleno = false; 
    int opcion;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Llenar arreglo\n";
        cout << "2. Mostrar arreglo\n";
        cout << "3. Ordenar con Selection Sort (Ascendente)\n";
        cout << "4. Ordenar con Insertion Sort (Descendente)\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            llenarArray(arreglo, N);
            lleno = true;
            for (int i = 0; i < N; i++) copia[i] = arreglo[i]; // guardo copia
            break;
        case 2:
            if (lleno) {
                cout << "Arreglo actual: ";
                mostrarArray(arreglo, N);
            } else {
                cout << "Primero debe llenar el arreglo.\n";
            }
            break;
        case 3:
            if (lleno) {
                for (int i = 0; i < N; i++) arreglo[i] = copia[i]; // vuelvo al original
                ordenarSelectionAsc(arreglo, N);
                cout << "Resultado final (Ascendente): ";
                mostrarArray(arreglo, N);
            } else {
                cout << "Primero debe llenar el arreglo.\n";
            }
            break;
        case 4:
            if (lleno) {
                for (int i = 0; i < N; i++) arreglo[i] = copia[i]; // vuelvo al original
                ordenarInsertionDesc(arreglo, N);
                cout << "Resultado final (Descendente): ";
                mostrarArray(arreglo, N);
            } else {
                cout << "Primero debe llenar el arreglo.\n";
            }
            break;
        case 5:
            cout << "Saliendo del programa...\n";
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 5);

    return 0;
}