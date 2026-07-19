#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <map>
#include <climits>
#include <thread>
#include <atomic>
#include "Nodo.h"
#include "sacarInfPantalla.h"

using namespace std;

// Variables globales para comunicación entre hilos
atomic<bool> programaActivo(true);
atomic<bool> entradaDisponible(false);
atomic<int> valorIngresado(0);
Nodo* raizGlobal = nullptr;
Nodo* ultimoNodoGlobal = nullptr;

// Funciones de árbol
Nodo* insertar(Nodo* raiz, int valor) {
    if (raiz == nullptr) {
        return new Nodo(valor);
    }
    if (valor < raiz->getValor()) {
        raiz->setIzquierdo(insertar(raiz->getIzquierdo(), valor));
    } else {
        raiz->setDerecho(insertar(raiz->getDerecho(), valor));
    }
    return raiz;
}

int contarNodos(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    return 1 + contarNodos(raiz->getIzquierdo()) + contarNodos(raiz->getDerecho());
}

int calcularAltura(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    int altIzq = calcularAltura(raiz->getIzquierdo());
    int altDer = calcularAltura(raiz->getDerecho());
    return 1 + max(altIzq, altDer);
}

void calcularPosiciones(Nodo* raiz, int nivel, map<int, int>& nextX, map<Nodo*, int>& posX) {
    if (raiz == nullptr) return;
    
    const int separacion = 110;  // CONSTANTE, no varía por nivel
    
    if (nextX.find(nivel) == nextX.end()) nextX[nivel] = 0;
    
    calcularPosiciones(raiz->getIzquierdo(), nivel + 1, nextX, posX);
    int xIzq = (raiz->getIzquierdo() != nullptr) ? posX[raiz->getIzquierdo()] : -1;
    
    int slotActual = nextX[nivel];
    nextX[nivel] += separacion;
    
    calcularPosiciones(raiz->getDerecho(), nivel + 1, nextX, posX);
    int xDer = (raiz->getDerecho() != nullptr) ? posX[raiz->getDerecho()] : -1;
    
    if (xIzq != -1 && xDer != -1) {
        posX[raiz] = (xIzq + xDer) / 2;
    } else {
        posX[raiz] = slotActual;
    }
}

int calcularOffsetOptimo(map<Nodo*, int>& posX, int anchoPantalla) {
    if (posX.empty()) return 0;
    
    int minX = INT_MAX, maxX = INT_MIN;
    for (auto& par : posX) {
        minX = min(minX, par.second);
        maxX = max(maxX, par.second);
    }
    
    int centroArbol = (minX + maxX) / 2;
    int centroPantalla = anchoPantalla / 2;
    int offsetX = centroPantalla - centroArbol;
    
    int margen = 40;
    if (offsetX + minX < margen) {
        offsetX = margen - minX;
    }
    if (offsetX + maxX > anchoPantalla - margen) {
        offsetX = anchoPantalla - margen - maxX;
    }
    
    return offsetX;
}

void dibujarArbolRT(HDC hdc, Nodo* raiz, int nivel, int espacioVertical,
                    map<Nodo*, int>& posX, int offsetX, int yInicial, Nodo* ultimoNodo) {
    if (raiz == nullptr) return;
    
    int radio = 20;
    int x = posX[raiz] + offsetX;
    int y = yInicial + nivel * espacioVertical;
    
    if (raiz->getIzquierdo() != nullptr) {
        int xHijo = posX[raiz->getIzquierdo()] + offsetX;
        int yHijo = yInicial + (nivel + 1) * espacioVertical;
        MoveToEx(hdc, x, y, nullptr);
        LineTo(hdc, xHijo, yHijo);
        dibujarArbolRT(hdc, raiz->getIzquierdo(), nivel + 1, espacioVertical,
                      posX, offsetX, yInicial, ultimoNodo);
    }
    
    if (raiz->getDerecho() != nullptr) {
        int xHijo = posX[raiz->getDerecho()] + offsetX;
        int yHijo = yInicial + (nivel + 1) * espacioVertical;
        MoveToEx(hdc, x, y, nullptr);
        LineTo(hdc, xHijo, yHijo);
        dibujarArbolRT(hdc, raiz->getDerecho(), nivel + 1, espacioVertical,
                      posX, offsetX, yInicial, ultimoNodo);
    }
    
    HBRUSH fondoNodo;
    HPEN bordeNodo;
    
    if (raiz == ultimoNodo) {
        fondoNodo = CreateSolidBrush(RGB(255, 255, 255));
        bordeNodo = CreatePen(PS_SOLID, 3, RGB(255, 200, 0));
        SetTextColor(hdc, RGB(0, 0, 0));
    } else {
        fondoNodo = CreateSolidBrush(RGB(0, 102, 204));
        bordeNodo = CreatePen(PS_SOLID, 2, RGB(51, 153, 255));
        SetTextColor(hdc, RGB(255, 255, 255));
    }
    
    SelectObject(hdc, fondoNodo);
    SelectObject(hdc, bordeNodo);
    Ellipse(hdc, x - radio, y - radio, x + radio, y + radio);
    
    string texto = to_string(raiz->getValor());
    SetBkMode(hdc, TRANSPARENT);
    int offsetTexto = (texto.length() > 1) ? 8 : 4;
    TextOutA(hdc, x - offsetTexto, y - 7, texto.c_str(), texto.length());
    
    DeleteObject(fondoNodo);
    DeleteObject(bordeNodo);
}

void dibujarEscena(HWND hwnd, HDC hdc, Nodo* raiz, int anchoPantalla, Nodo* ultimoNodo){
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    HBRUSH fondo = CreateSolidBrush(RGB(20, 20, 40));
    FillRect(hdc, &rect, fondo);
    DeleteObject(fondo);
    
    if (raiz != nullptr) {
        map<int, int> nextX;
        map<Nodo*, int> posX;
        
        calcularPosiciones(raiz, 0, nextX, posX);
        int offsetX = calcularOffsetOptimo(posX, anchoPantalla);
        
        HPEN lapizRamas = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
        SelectObject(hdc, lapizRamas);
        
        dibujarArbolRT(hdc, raiz, 0, 80, posX, offsetX, 120, ultimoNodo);
        
        DeleteObject(lapizRamas);
    }
}


void leerEntrada() {
    int valor;
    while (programaActivo) {
        // cin bloqueante, sin interferencia del hilo principal
        if (cin >> valor) {
            if (valor == -1) {
                programaActivo = false;
                break;
            }
            valorIngresado = valor;
            entradaDisponible = true;
            
            // Esperar a que el principal procese antes de leer otro
            while (entradaDisponible && programaActivo) {
                Sleep(10);
            }
        } else {
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

Nodo* buscarNodo(Nodo* raiz, int valor) {
    if (raiz == nullptr) return nullptr;
    if (raiz->getValor() == valor) return raiz;
    if (valor < raiz->getValor()) return buscarNodo(raiz->getIzquierdo(), valor);
    return buscarNodo(raiz->getDerecho(), valor);
}

void procesarEntrada() {
    if (entradaDisponible) {
        int valor = valorIngresado;
        raizGlobal = insertar(raizGlobal, valor);
        ultimoNodoGlobal = buscarNodo(raizGlobal, valor);
        entradaDisponible = false;
    }
}

int main() {
    // Configurar codificación
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    system("cls");
    
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE);
    
    int anchoRealPantalla = GetSystemMetrics(SM_CXSCREEN);
    int altoRealPantalla = GetSystemMetrics(SM_CYSCREEN);
    
    cout << "========================================" << endl;
    cout << "       ARBOL BINARIO DE BUSQUEDA        " << endl;
    cout << "========================================" << endl;
    cout << "Resolucion: " << anchoRealPantalla << " x " << altoRealPantalla << " pixeles" << endl;
    cout << "========================================" << endl;
    
    cout << "\nPresiona ENTER para iniciar...";
    cin.get();
    
    system("cls");
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true; // Mantener cursor visible para entrada
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    DWORD modoEntrada;
    if (GetConsoleMode(hInput, &modoEntrada)) {
        SetConsoleMode(hInput, modoEntrada & ~ENABLE_ECHO_INPUT);
    }
    
    HDC hdc = GetDC(hwnd);
    raizGlobal = nullptr;

    // Crear hilo para leer entrada
    thread hiloEntrada(leerEntrada);
    
    int frameCount = 0;
    while (programaActivo) {
        procesarEntrada();
        
        RECT rectVentana;
        GetClientRect(hwnd, &rectVentana);
        int anchoConsola = rectVentana.right - rectVentana.left;
        
        dibujarEscena(hwnd, hdc, raizGlobal, anchoConsola, ultimoNodoGlobal);
        
        // Info del árbol dibujada con GDI, NO con cout
        // (así no interfiere con cin)
        string info = "";
        if (raizGlobal != nullptr) {
            string ultimoValor = (ultimoNodoGlobal != nullptr) ? to_string(ultimoNodoGlobal->getValor()) : "-";
            info = "Nodos: " + to_string(contarNodos(raizGlobal)) +
                "  Altura: " + to_string(calcularAltura(raizGlobal)) +
                "  Ultimo: " + ultimoValor;
        } else {
            info = "Arbol vacio";
        }
        
        // Dibujar info con GDI en la esquina superior izquierda
        SetBkMode(hdc, OPAQUE);
        SetBkColor(hdc, RGB(20, 20, 40));
        SetTextColor(hdc, RGB(200, 200, 200));
        TextOutA(hdc, 10, 10, info.c_str(), info.length());

        string prompt = "Numero (-1 para salir): ";
        TextOutA(hdc, 10, 35, prompt.c_str(), prompt.length());
        
        Sleep(50);
    }
    
    // Esperar a que el hilo de entrada termine
    if (hiloEntrada.joinable()) {
        hiloEntrada.join();
    }
    
    ReleaseDC(hwnd, hdc);
    if (raizGlobal != nullptr) delete raizGlobal;
    
    cout << "\nPrograma finalizado." << endl;
    system("pause");
    
    return 0;
}

