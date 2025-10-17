#include <stdio.h>
#include <stdlib.h>

typedef struct pasajero {
    int documento;              // Documento del pasajero
    struct pasajero *ant;       // Apunta al pasajero anterior
    struct pasajero *sig;       // Apunta al siguiente pasajero
} pasajero;

pasajero *cab = NULL; // Cabecera de la lista (primer pasajero)

void registrarPasajero();
void mostrarPasajeros();
void recorrerAvion();

int main() {
    int opc;
    do {
        printf("\n==== MENU DEL AVION ====\n");
        printf("1. Registrar pasajero\n");
        printf("2. Mostrar pasajeros\n");
        printf("3. Recorrer avion\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                registrarPasajero();
                break;
            case 2:
                mostrarPasajeros();
                break;
            case 3:
                recorrerAvion();
                break;
            case 4:
                printf("\nSaliendo del sistema del avión...\n");
                break;
            default:
                printf("\nOpción inválida, intente de nuevo.\n");
        }
    } while (opc != 4);

    return 0;
}

void registrarPasajero() {
    int numero;
    printf("\nIngrese el número de documento del pasajero: ");
    scanf("%d", &numero);

    if (cab == NULL) {
        cab = (pasajero *) malloc(sizeof(pasajero));
        cab->documento = numero;
        cab->ant = cab->sig = NULL;
        printf("Pasajero registrado en el primer asiento.\n");
    } else {
        pasajero *nuevo = (pasajero *) malloc(sizeof(pasajero));
        nuevo->documento = numero;
        nuevo->sig = cab;
        nuevo->ant = NULL;
        cab->ant = nuevo;
        cab = nuevo;
        printf("Pasajero registrado en un nuevo asiento.\n");
        // No se debe liberar 'nuevo' aquí, ya que lo usamos en la lista
    }
}

void mostrarPasajeros() {
    pasajero *actual = cab;

    if (actual == NULL) {
        printf("\nNo hay pasajeros registrados en el avión.\n");
        return;
    }

    printf("\nPasajeros dentro del avión:\n");
    while (actual != NULL) {
        printf("Documento: %d -> ", actual->documento);
        actual = actual->sig;
    }
    printf("NULL (Final del avión)\n");
}

void recorrerAvion() {
    int opc;
    pasajero *actual = cab;

    if (actual == NULL) {
        printf("\nNo hay pasajeros para recorrer.\n");
        return;
    }

    do {
        printf("\n1. Avanzar al siguiente pasajero\n");
        printf("2. Retroceder al pasajero anterior\n");
        printf("3. Terminar recorrido\n");
        printf("Seleccione: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                printf("Antes de avanzar - Pasajero con documento: %d\n", actual->documento);
                actual = actual->sig;
                if (actual == NULL)
                    printf("Ya llegaste al final del avión.\n");
                break;
            case 2:
                printf("Antes de retroceder - Pasajero con documento: %d\n", actual->documento);
                actual = actual->ant;
                if (actual == NULL)
                    printf("Ya estás en el inicio del avión.\n");
                break;
            case 3:
                printf("Recorrido terminado.\n");
                break;
            default:
                printf("Opción inválida.\n");
        }

    } while ((opc != 3) && (actual != NULL));
}
