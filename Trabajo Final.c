#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura del nodo FIFO (Cola de pedidos)
typedef struct NodoFIFO {
    char nombre_destino[50];
    int cantidad_solicitada;
    struct NodoFIFO* siguiente;
} NodoFIFO;

// Estructura del nodo AVL (Inventario por fecha)
typedef struct NodoAVL {
    int fecha_vencimiento;  // Formato AAAAMMDD
    int stock_total;
    char nombre_producto[50];
    NodoFIFO* cabeza_pedidos;
    NodoFIFO* cola_pedidos;
    struct NodoAVL* izquierda;
    struct NodoAVL* derecha;
    int altura;
} NodoAVL;

// Prototipos de funciones
int max(int a, int b);
int obtener_altura(NodoAVL* nodo);
int obtener_balance(NodoAVL* nodo);
NodoAVL* crear_nodo_avl(int fecha, int stock, const char* producto);
NodoAVL* rotacion_derecha(NodoAVL* y);
NodoAVL* rotacion_izquierda(NodoAVL* x);
NodoAVL* insertar_avl(NodoAVL* nodo, int fecha, int stock, const char* producto);
void encolar_pedido(NodoAVL* nodo, const char* destino, int cantidad);
NodoAVL* encontrar_min(NodoAVL* nodo);
NodoAVL* eliminar_avl(NodoAVL* raiz, int fecha);
void liberar_cola_fifo(NodoFIFO* cabeza);
int cancelar_pedido(NodoAVL* raiz, int fecha, const char* destino);
void reporte_inorder(NodoAVL* raiz);
int contar_pedidos(NodoFIFO* cabeza);
void liberar_arbol(NodoAVL* raiz);
NodoAVL* buscar_nodo(NodoAVL* raiz, int fecha);

// Función auxiliar: máximo de dos números
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Obtener altura de un nodo
int obtener_altura(NodoAVL* nodo) {
    if (nodo == NULL)
        return 0;
    return nodo->altura;
}

// Obtener factor de balanceo
int obtener_balance(NodoAVL* nodo) {
    if (nodo == NULL)
        return 0;
    return obtener_altura(nodo->izquierda) - obtener_altura(nodo->derecha);
}

// Crear un nuevo nodo AVL
NodoAVL* crear_nodo_avl(int fecha, int stock, const char* producto) {
    NodoAVL* nodo = (NodoAVL*)malloc(sizeof(NodoAVL));
    nodo->fecha_vencimiento = fecha;
    nodo->stock_total = stock;
    strcpy(nodo->nombre_producto, producto);
    nodo->cabeza_pedidos = NULL;
    nodo->cola_pedidos = NULL;
    nodo->izquierda = NULL;
    nodo->derecha = NULL;
    nodo->altura = 1;
    return nodo;
}

// Rotación simple a la derecha
NodoAVL* rotacion_derecha(NodoAVL* y) {
    NodoAVL* x = y->izquierda;
    NodoAVL* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(obtener_altura(y->izquierda), obtener_altura(y->derecha)) + 1;
    x->altura = max(obtener_altura(x->izquierda), obtener_altura(x->derecha)) + 1;

    return x;
}

// Rotación simple a la izquierda
NodoAVL* rotacion_izquierda(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(obtener_altura(x->izquierda), obtener_altura(x->derecha)) + 1;
    y->altura = max(obtener_altura(y->izquierda), obtener_altura(y->derecha)) + 1;

    return y;
}

// Insertar en el árbol AVL
NodoAVL* insertar_avl(NodoAVL* nodo, int fecha, int stock, const char* producto) {
    // 1. Inserción normal de BST
    if (nodo == NULL)
        return crear_nodo_avl(fecha, stock, producto);

    if (fecha < nodo->fecha_vencimiento)
        nodo->izquierda = insertar_avl(nodo->izquierda, fecha, stock, producto);
    else if (fecha > nodo->fecha_vencimiento)
        nodo->derecha = insertar_avl(nodo->derecha, fecha, stock, producto);
    else {
        // Fecha ya existe, actualizar stock
        nodo->stock_total += stock;
        return nodo;
    }

    // 2. Actualizar altura del nodo ancestro
    nodo->altura = 1 + max(obtener_altura(nodo->izquierda), obtener_altura(nodo->derecha));

    // 3. Obtener factor de balanceo
    int balance = obtener_balance(nodo);

    // 4. Casos de desbalanceo

    // Caso Izquierda-Izquierda
    if (balance > 1 && fecha < nodo->izquierda->fecha_vencimiento)
        return rotacion_derecha(nodo);

    // Caso Derecha-Derecha
    if (balance < -1 && fecha > nodo->derecha->fecha_vencimiento)
        return rotacion_izquierda(nodo);

    // Caso Izquierda-Derecha
    if (balance > 1 && fecha > nodo->izquierda->fecha_vencimiento) {
        nodo->izquierda = rotacion_izquierda(nodo->izquierda);
        return rotacion_derecha(nodo);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && fecha < nodo->derecha->fecha_vencimiento) {
        nodo->derecha = rotacion_derecha(nodo->derecha);
        return rotacion_izquierda(nodo);
    }

    return nodo;
}

// Buscar nodo por fecha
NodoAVL* buscar_nodo(NodoAVL* raiz, int fecha) {
    if (raiz == NULL || raiz->fecha_vencimiento == fecha)
        return raiz;
    
    if (fecha < raiz->fecha_vencimiento)
        return buscar_nodo(raiz->izquierda, fecha);
    
    return buscar_nodo(raiz->derecha, fecha);
}

// Encolar pedido en la cola FIFO
void encolar_pedido(NodoAVL* nodo, const char* destino, int cantidad) {
    NodoFIFO* nuevo_pedido = (NodoFIFO*)malloc(sizeof(NodoFIFO));
    strcpy(nuevo_pedido->nombre_destino, destino);
    nuevo_pedido->cantidad_solicitada = cantidad;
    nuevo_pedido->siguiente = NULL;

    if (nodo->cola_pedidos == NULL) {
        nodo->cabeza_pedidos = nuevo_pedido;
        nodo->cola_pedidos = nuevo_pedido;
    } else {
        nodo->cola_pedidos->siguiente = nuevo_pedido;
        nodo->cola_pedidos = nuevo_pedido;
    }
}

// Encontrar el nodo con valor mínimo
NodoAVL* encontrar_min(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual->izquierda != NULL)
        actual = actual->izquierda;
    return actual;
}

// Liberar cola FIFO
void liberar_cola_fifo(NodoFIFO* cabeza) {
    NodoFIFO* temp;
    while (cabeza != NULL) {
        temp = cabeza;
        cabeza = cabeza->siguiente;
        free(temp);
    }
}

// Eliminar nodo del árbol AVL
NodoAVL* eliminar_avl(NodoAVL* raiz, int fecha) {
    if (raiz == NULL)
        return raiz;

    // Búsqueda del nodo a eliminar
    if (fecha < raiz->fecha_vencimiento)
        raiz->izquierda = eliminar_avl(raiz->izquierda, fecha);
    else if (fecha > raiz->fecha_vencimiento)
        raiz->derecha = eliminar_avl(raiz->derecha, fecha);
    else {
        // Nodo encontrado - liberar cola FIFO primero
        liberar_cola_fifo(raiz->cabeza_pedidos);

        // Nodo con uno o ningún hijo
        if ((raiz->izquierda == NULL) || (raiz->derecha == NULL)) {
            NodoAVL* temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;

            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;

            free(temp);
        } else {
            // Nodo con dos hijos
            NodoAVL* temp = encontrar_min(raiz->derecha);

            raiz->fecha_vencimiento = temp->fecha_vencimiento;
            raiz->stock_total = temp->stock_total;
            strcpy(raiz->nombre_producto, temp->nombre_producto);
            raiz->cabeza_pedidos = temp->cabeza_pedidos;
            raiz->cola_pedidos = temp->cola_pedidos;

            temp->cabeza_pedidos = NULL;
            raiz->derecha = eliminar_avl(raiz->derecha, temp->fecha_vencimiento);
        }
    }

    if (raiz == NULL)
        return raiz;

    // Actualizar altura
    raiz->altura = 1 + max(obtener_altura(raiz->izquierda), obtener_altura(raiz->derecha));

    // Balancear el árbol
    int balance = obtener_balance(raiz);

    // Caso Izquierda-Izquierda
    if (balance > 1 && obtener_balance(raiz->izquierda) >= 0)
        return rotacion_derecha(raiz);

    // Caso Izquierda-Derecha
    if (balance > 1 && obtener_balance(raiz->izquierda) < 0) {
        raiz->izquierda = rotacion_izquierda(raiz->izquierda);
        return rotacion_derecha(raiz);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && obtener_balance(raiz->derecha) <= 0)
        return rotacion_izquierda(raiz);

    // Caso Derecha-Izquierda
    if (balance < -1 && obtener_balance(raiz->derecha) > 0) {
        raiz->derecha = rotacion_derecha(raiz->derecha);
        return rotacion_izquierda(raiz);
    }

    return raiz;
}

// Cancelar un pedido específico de la cola FIFO
int cancelar_pedido(NodoAVL* raiz, int fecha, const char* destino) {
    NodoAVL* nodo = buscar_nodo(raiz, fecha);
    if (nodo == NULL) {
        printf("No existe producto con esa fecha.\n");
        return 0;
    }

    NodoFIFO* actual = nodo->cabeza_pedidos;
    NodoFIFO* anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->nombre_destino, destino) == 0) {
            // Restaurar stock
            nodo->stock_total += actual->cantidad_solicitada;

            // Eliminar de la cola
            if (anterior == NULL) {
                nodo->cabeza_pedidos = actual->siguiente;
                if (nodo->cabeza_pedidos == NULL)
                    nodo->cola_pedidos = NULL;
            } else {
                anterior->siguiente = actual->siguiente;
                if (actual == nodo->cola_pedidos)
                    nodo->cola_pedidos = anterior;
            }

            free(actual);
            printf("Pedido cancelado exitosamente. Stock restaurado.\n");
            return 1;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    printf("No se encontró pedido para el destino especificado.\n");
    return 0;
}

// Contar pedidos en cola
int contar_pedidos(NodoFIFO* cabeza) {
    int count = 0;
    while (cabeza != NULL) {
        count++;
        cabeza = cabeza->siguiente;
    }
    return count;
}

// Reporte In-Order
void reporte_inorder(NodoAVL* raiz) {
    if (raiz != NULL) {
        reporte_inorder(raiz->izquierda);
        
        printf("\n--- Producto: %s ---\n", raiz->nombre_producto);
        printf("Fecha Vencimiento: %d\n", raiz->fecha_vencimiento);
        printf("Stock Disponible: %d unidades\n", raiz->stock_total);
        printf("Pedidos en Cola: %d\n", contar_pedidos(raiz->cabeza_pedidos));
        
        if (raiz->cabeza_pedidos != NULL) {
            printf("Lista de Pedidos:\n");
            NodoFIFO* pedido = raiz->cabeza_pedidos;
            int num = 1;
            while (pedido != NULL) {
                printf("  %d. Destino: %s - Cantidad: %d\n", 
                       num++, pedido->nombre_destino, pedido->cantidad_solicitada);
                pedido = pedido->siguiente;
            }
        }
        
        reporte_inorder(raiz->derecha);
    }
}

// Liberar todo el árbol
void liberar_arbol(NodoAVL* raiz) {
    if (raiz != NULL) {
        liberar_arbol(raiz->izquierda);
        liberar_arbol(raiz->derecha);
        liberar_cola_fifo(raiz->cabeza_pedidos);
        free(raiz);
    }
}

// Función principal
int main() {
    NodoAVL* raiz = NULL;
    int opcion;
    int fecha, cantidad, stock;
    char producto[50], destino[50];

    printf("=== SISTEMA DE GESTION LOGISTICA ===\n");
    printf("Puerto de Distribucion de Alimentos - Buenaventura\n\n");

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1. Recepcion de Mercancia (Insertar en AVL)\n");
        printf("2. Registrar Pedido de Despacho (Encolar en FIFO)\n");
        printf("3. Eliminar Producto Completo\n");
        printf("4. Cancelar Pedido Especifico\n");
        printf("5. Reporte de Estado (Recorrido In-Order)\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n--- RECEPCION DE MERCANCIA ---\n");
                printf("Ingrese fecha de vencimiento (AAAAMMDD): ");
                scanf("%d", &fecha);
                printf("Ingrese cantidad a recibir: ");
                scanf("%d", &stock);
                printf("Ingrese nombre del producto: ");
                scanf(" %[^\n]", producto);
                
                raiz = insertar_avl(raiz, fecha, stock, producto);
                printf("Mercancia registrada exitosamente.\n");
                break;

            case 2:
                printf("\n--- REGISTRAR PEDIDO DE DESPACHO ---\n");
                printf("Ingrese fecha de vencimiento del lote (AAAAMMDD): ");
                scanf("%d", &fecha);
                
                NodoAVL* nodo_encontrado = buscar_nodo(raiz, fecha);
                if (nodo_encontrado == NULL) {
                    printf("No existe producto con esa fecha de vencimiento.\n");
                } else {
                    printf("Producto encontrado: %s (Stock: %d)\n", 
                           nodo_encontrado->nombre_producto, nodo_encontrado->stock_total);
                    printf("Ingrese destino (ej: Nuqui, Guapi): ");
                    scanf(" %[^\n]", destino);
                    printf("Ingrese cantidad solicitada: ");
                    scanf("%d", &cantidad);
                    
                    if (cantidad > nodo_encontrado->stock_total) {
                        printf("Error: Stock insuficiente.\n");
                    } else {
                        encolar_pedido(nodo_encontrado, destino, cantidad);
                        nodo_encontrado->stock_total -= cantidad;
                        printf("Pedido registrado exitosamente.\n");
                    }
                }
                break;

            case 3:
                printf("\n--- ELIMINAR PRODUCTO COMPLETO ---\n");
                printf("Ingrese fecha de vencimiento del producto a eliminar (AAAAMMDD): ");
                scanf("%d", &fecha);
                
                raiz = eliminar_avl(raiz, fecha);
                printf("Producto eliminado del sistema.\n");
                break;

            case 4:
                printf("\n--- CANCELAR PEDIDO ESPECIFICO ---\n");
                printf("Ingrese fecha de vencimiento del lote (AAAAMMDD): ");
                scanf("%d", &fecha);
                printf("Ingrese destino del pedido a cancelar: ");
                scanf(" %[^\n]", destino);
                
                cancelar_pedido(raiz, fecha, destino);
                break;

            case 5:
                printf("\n===== REPORTE DE ESTADO DEL INVENTARIO =====\n");
                printf("(Ordenado por fecha de vencimiento - Proximas a vencer primero)\n");
                if (raiz == NULL) {
                    printf("No hay productos en el inventario.\n");
                } else {
                    reporte_inorder(raiz);
                }
                break;

            case 0:
                printf("\nCerrando sistema...\n");
                liberar_arbol(raiz);
                printf("Memoria liberada. Hasta pronto!\n");
                break;

            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 0);

    return 0;
}