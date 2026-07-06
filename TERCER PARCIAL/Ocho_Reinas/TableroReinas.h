#ifndef TABLEROREINAS_H
#define TABLEROREINAS_H

#include <vector>
#include <string>
using namespace std;

class TableroReinas {
private:
    int n;
    vector<vector<int>> tablero;
    vector<int> reinas;
    int intentos;
    int solucionesEncontradas;

    bool esSeguro(int fila, int columna);
    bool colocarReinas(int fila);
    void guardarEstado(int reinaActual);
    void imprimirTableroSimple();

public:
    TableroReinas(int n);
    ~TableroReinas();
    
    bool resolver();
    int getIntentos() const;
    void imprimirTablero();
    
    // Nuevas funciones
    void generarArchivoTablero();  // Genera el archivo con 0s y 1s
    void leerYDibujarTablero(const string& nombreArchivo);  // Lee y dibuja el tablero
};

#endif