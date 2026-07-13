#include <iostream>

using namespace std;

// Función para imprimir usando aritmética de punteros
void imprimirCuadrado(int* matriz, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Equivalente a matriz[i][j]
            cout << *(matriz + i * n + j) << " ";
        }
        cout << "\n";
    }
    cout << "-----------------\n";
}

// Validación analítica mediante punteros
bool esSeguro(int* matriz, int fila, int col, int num, int n) {
    for (int i = 0; i < n; i++) {
        // Verifica la fila: *(matriz + fila * n + i)
        // Verifica la columna: *(matriz + i * n + col)
        if (*(matriz + fila * n + i) == num || *(matriz + i * n + col) == num) {
            return false;
        }
    }
    return true;
}

// Backtracking estricto con punteros
bool resolverCuadradoLatino(int* matriz, int fila, int col, int n) {
    if (fila == n) {
        return true;
    }

    if (col == n) {
        return resolverCuadradoLatino(matriz, fila + 1, 0, n);
    }

    if (*(matriz + fila * n + col) != 0) {
        return resolverCuadradoLatino(matriz, fila, col + 1, n);
    }

    for (int num = 1; num <= n; num++) {
        if (esSeguro(matriz, fila, col, num, n)) {
            *(matriz + fila * n + col) = num; // Asignación

            if (resolverCuadradoLatino(matriz, fila, col + 1, n)) {
                return true;
            }

            *(matriz + fila * n + col) = 0; // Backtrack
        }
    }
    return false;
}

int main() {
    int n;
    cout << "Introduce el tamano del Cuadrado Latino (N): ";
    cin >> n;

    if (n <= 0) return 1;

    // Asignación dinámica de memoria para una matriz bidimensional plana
    int* matriz = new int[n * n];

    // Inicializar a cero usando punteros
    for (int i = 0; i < n * n; i++) {
        *(matriz + i) = 0;
    }

    if (resolverCuadradoLatino(matriz, 0, 0, n)) {
        cout << "\nCuadrado Latino solucionado:\n";
        imprimirCuadrado(matriz, n);
    } else {
        cout << "No se encontro solucion." << endl;
    }

    // Liberación obligatoria de la memoria dinámica
    delete[] matriz;
    return 0;
}