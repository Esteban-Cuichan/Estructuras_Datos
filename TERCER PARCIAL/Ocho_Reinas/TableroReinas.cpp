#include "TableroReinas.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

TableroReinas::TableroReinas(int n):n(n),intentos(0),solucionesEncontradas(0) {
    tablero.assign(n,vector<int>(n,0));
    reinas.assign(n,-1);
    srand(time(0));
}

TableroReinas::~TableroReinas() {}

bool TableroReinas::esSeguro(int fila,int columna) {
    for(int i=0;i<fila;i++) {
        if(reinas[i]==columna) return false;
    }
    
    for(int i=0;i<fila;i++) {
        if(abs(reinas[i]-columna)==abs(i-fila)) return false;
    }
    
    return true;
}

bool TableroReinas::colocarReinas(int fila) {
    if(fila==n) {
        solucionesEncontradas++;
        return true;
    }

    vector<int> posiciones(n);
    for(int i=0;i<n;i++) posiciones[i]=i;
    
    for(int i=n-1;i>0;i--) {
        int j=rand()%(i+1);
        swap(posiciones[i],posiciones[j]);
    }

    for(int columna:posiciones) {
        if(esSeguro(fila,columna)) {
            reinas[fila]=columna;
            tablero[fila][columna]=1;
            intentos++;
            
            guardarEstado(fila+1);
            cout<<"\nInsertando reina "<<fila+1<<" en posicion ("<<fila<<", "<<columna<<")"<<endl;
            imprimirTablero();
            
            if(colocarReinas(fila+1)) {
                return true;
            }
            
            tablero[fila][columna]=0;
            reinas[fila]=-1;
        }
    }
    return false;
}

void TableroReinas::guardarEstado(int reinaActual) {
    string nombreArchivo="tablero_reina_"+to_string(reinaActual)+".txt";
    ofstream archivo(nombreArchivo);
    
    if(archivo.is_open()) {
        archivo<<"Tablero "<<n<<"x"<<n<<endl;
        archivo<<"Reina actual: "<<reinaActual<<endl;
        archivo<<"Intentos realizados: "<<intentos<<endl;
        archivo<<"-------------------"<<endl;
        
        for(int i=0;i<n;i++) {
            for(int j=0;j<n;j++) {
                archivo<<tablero[i][j]<<" ";
            }
            archivo<<endl;
        }
        
        archivo<<"-------------------"<<endl;
        archivo<<"Posiciones de reinas: ";
        for(int i=0;i<n;i++) {
            if(reinas[i]!=-1) {
                archivo<<"("<<i<<","<<reinas[i]<<") ";
            }
        }
        archivo<<endl;
        
        archivo.close();
    }
}

void TableroReinas::imprimirTableroSimple() {
    cout<<"    ";
    for(int j=0;j<n;j++) {
        cout<<" "<<j<<"  ";
    }
    cout<<endl;
    
    cout<<"    +";
    for(int j=0;j<n;j++) {
        cout<<"---+";
    }
    cout<<endl;
    
    for(int i=0;i<n;i++) {
        cout<<" "<<i<<" |";
        
        for(int j=0;j<n;j++) {
            if(tablero[i][j]==1) {
                cout<<"  |";
            } else {
                if((i+j)%2==0) {
                    cout<<"   |";
                } else {
                    cout<<"   |";
                }
            }
        }
        cout<<endl;
        
        cout<<"    +";
        for(int j=0;j<n;j++) {
            cout<<"---+";
        }
        cout<<endl;
    }
    cout<<endl;
}

void TableroReinas::imprimirTablero() {
    imprimirTableroSimple();
}

bool TableroReinas::resolver() {
    cout<<"Intentando colocar "<<n<<" reinas en un tablero "<<n<<"x"<<n<<endl;
    cout<<"========================================"<<endl;
    
    bool exito=colocarReinas(0);
    
    cout<<"========================================"<<endl;
    cout<<"Procesos/intentos realizados: "<<intentos<<endl;
    cout<<"Soluciones encontradas: "<<solucionesEncontradas<<endl;
    
    if(exito) {
        cout<<"¡Solucion encontrada!"<<endl;
        cout<<"Tablero final:"<<endl;
        imprimirTablero();
    } else {
        cout<<"No se encontro solucion."<<endl;
    }
    
    return exito;
}

int TableroReinas::getIntentos() const {
    return intentos;
}

void TableroReinas::generarArchivoTablero() {
    string nombreArchivo = "tablero_" + to_string(n) + "x" + to_string(n) + ".txt";
    ofstream archivo(nombreArchivo);
    
    if (archivo.is_open()) {
        archivo << "Tablero " << n << "x" << n << " (0=blanco, 1=negro)" << endl;
        archivo << "=========================================" << endl;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Alternar colores: 0 para blanco, 1 para negro
                if ((i + j) % 2 == 0) {
                    archivo << "0 ";
                } else {
                    archivo << "1 ";
                }
            }
            archivo << endl;
        }
        
        archivo.close();
        cout << "Archivo '" << nombreArchivo << "' generado exitosamente." << endl;
    } else {
        cout << "Error al generar el archivo." << endl;
    }
}

// NUEVA FUNCIÓN: Leer archivo y dibujar tablero con colores
void TableroReinas::leerYDibujarTablero(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo '" << nombreArchivo << "'" << endl;
        return;
    }
    
    // Leer las dos primeras líneas (encabezado)
    string linea;
    getline(archivo, linea);  // Primera línea: "Tablero NxN (0=blanco, 1=negro)"
    getline(archivo, linea);  // Segunda línea: "========================================="
    
    // Leer la matriz
    vector<vector<int>> matriz;
    int fila = 0;
    
    while (getline(archivo, linea) && fila < n) {
        vector<int> filaMatriz;
        for (char c : linea) {
            if (c == '0' || c == '1') {
                filaMatriz.push_back(c - '0');  // Convertir char a int
            }
        }
        if (!filaMatriz.empty()) {
            matriz.push_back(filaMatriz);
            fila++;
        }
    }
    
    archivo.close();
    
    // Verificar que la matriz tenga el tamaño correcto
    if (matriz.size() != n || matriz[0].size() != n) {
        cout << "Error: El archivo no contiene una matriz " << n << "x" << n << endl;
        return;
    }
    
    // Dibujar el tablero con colores
    dibujarTableroDesdeMatriz(matriz);
}

// Función auxiliar para dibujar el tablero desde una matriz
void dibujarTableroDesdeMatriz(const vector<vector<int>>& matriz) {
    #ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #endif
    
    int n = matriz.size();
    
    cout << "\nTablero " << n << "x" << n << " con colores:" << endl;
    cout << "========================================" << endl;
    
    // Mostrar números de columna
    cout << "    ";
    for (int j = 0; j < n; j++) {
        cout << " " << j << "  ";
    }
    cout << endl;
    
    cout << "   +";
    for (int j = 0; j < n; j++) {
        cout << "---+";
    }
    cout << endl;
    
    for (int i = 0; i < n; i++) {
        cout << " " << i << " |";
        
        for (int j = 0; j < n; j++) {
            #ifdef _WIN32
            // Configurar colores según el valor (0=blanco, 1=negro/café)
            if (matriz[i][j] == 0) {
                // Blanco/beige
                SetConsoleTextAttribute(hConsole, 15);  // Fondo blanco
                cout << "   ";
            } else {
                // Negro/café
                SetConsoleTextAttribute(hConsole, 8);   // Fondo gris oscuro
                cout << "   ";
            }
            // Resetear color
            SetConsoleTextAttribute(hConsole, 7);
            #else
            // Versión sin colores (Linux/Mac)
            if (matriz[i][j] == 0) {
                cout << "   ";
            } else {
                cout << "###";
            }
            #endif
            cout << "|";
        }
        cout << endl;
        
        cout << "   +";
        for (int j = 0; j < n; j++) {
            cout << "---+";
        }
        cout << endl;
    }
    cout << endl;
    
    // Mostrar leyenda
    #ifdef _WIN32
    SetConsoleTextAttribute(hConsole, 15);
    cout << "█ ";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "= Blanco (0)   ";
    
    SetConsoleTextAttribute(hConsole, 8);
    cout << "█ ";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "= Negro/Café (1)" << endl;
    #endif
}