#include "TableroReinas.h"
#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

int main() {
    #ifdef _WIN32
    // Configurar consola para colores
    system("color f0");  // Fondo blanco, texto negro
    #endif
    
    int n;
    cout << "========================================" << endl;
    cout << "         PROBLEMA DE LAS N REINAS        " << endl;
    cout << "========================================" << endl;
    cout << "Ingrese el numero de reinas: ";
    cin >> n;

    if (n <= 0) {
        cout << "El numero debe ser positivo." << endl;
        system("pause");
        return 1;
    }

    TableroReinas tablero(n);
    
    // Generar archivo con 0s y 1s
    cout << "\nGenerando archivo del tablero..." << endl;
    tablero.generarArchivoTablero();
    
    // Leer y dibujar el tablero desde el archivo
    string nombreArchivo = "tablero_" + to_string(n) + "x" + to_string(n) + ".txt";
    cout << "\nLeyendo y dibujando desde el archivo..." << endl;
    tablero.leerYDibujarTablero(nombreArchivo);
    
    // Opcional: Resolver el problema de las reinas
    cout << "\n¿Deseas resolver el problema de las N reinas? (1=Si, 0=No): ";
    int opcion;
    cin >> opcion;
    
    if (opcion == 1) {
        bool exito = tablero.resolver();
        if (exito) {
            cout << "Procesos persistieron: " << tablero.getIntentos() << endl;
        }
    }

    cout << "\nPresione cualquier tecla para salir...";
    system("pause");

    return 0;
}