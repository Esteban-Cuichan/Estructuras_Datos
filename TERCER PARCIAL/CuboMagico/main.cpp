#include <iostream>
#include <iomanip>

using namespace std;

void imprimirCubo(int* matriz, int n) {
    int constanteMagica = n * (n * n + 1) / 2;
    cout << "\nConstante Magica: " << constanteMagica << "\n\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(4) << *(matriz + i * n + j) << " ";
        }
        cout << "\n";
    }
    cout << "-------------------------------------\n";
}

void generarCuboMagico(int n) {
    // Asignación dinámica de memoria
    int* matriz = new int[n * n];

    // Inicializar a cero con aritmética de punteros
    for (int i = 0; i < n * n; i++) {
        *(matriz + i) = 0;
    }

    int i = 0;
    int j = n / 2;

    for (int num = 1; num <= n * n; num++) {
        *(matriz + i * n + j) = num; // Asignar número en la posición calculada

        int siguiente_i = i - 1;
        int siguiente_j = j + 1;

        if (siguiente_i < 0) {
            siguiente_i = n - 1;
        }
        if (siguiente_j == n) {
            siguiente_j = 0;
        }

        // Acceso analítico al contenido de la celda destino mediante punteros
        if (*(matriz + siguiente_i * n + siguiente_j) != 0) {
            i = i + 1;
        } else {
            i = siguiente_i;
            j = siguiente_j;
        }
    }

    imprimirCubo(matriz, n);

    // Liberación de memoria dinámica
    delete[] matriz;
}

int main() {
    int n;
    cout << "Introduce el tamano del Cubo Magico (IMPAR): ";
    cin >> n;

    if (n <= 0 || n % 2 == 0) {
        cout << "Error: Debe ser un numero impar positivo." << endl;
        return 1;
    }

    generarCuboMagico(n);

    return 0;
}