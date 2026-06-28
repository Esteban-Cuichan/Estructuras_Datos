#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h> // Necesario para _getch()
#include "Nodo.h"
#include "sacarInfPantalla.h"

using namespace std;

// Códigos de teclas en Windows
#define ARRIBA 72
#define ABAJO 80
#define ENTER 13

Nodo* insertar(Nodo* raiz, int valor) {
    if (raiz == nullptr) {
        return new Nodo(valor);
    }
    if (valor < raiz->getValor()) {
        raiz->setIzquierdo(insertar(raiz->getIzquierdo(), valor));
    } else if (valor > raiz->getValor()) {
        raiz->setDerecho(insertar(raiz->getDerecho(), valor));
    }
    return raiz;
}

void dibujarArbol(HDC hdc, Nodo* raiz, int x, int y, int desviamientosHorizontal) {
    if (raiz == nullptr) return;

    int radio = 18;
    int espacioVertical = 60;

    if (raiz->getIzquierdo() != nullptr) {
        MoveToEx(hdc, x, y, nullptr);
        LineTo(hdc, x - desviamientosHorizontal, y + espacioVertical);
        dibujarArbol(hdc, raiz->getIzquierdo(), x - desviamientosHorizontal, y + espacioVertical, desviamientosHorizontal / 2);
    }
    
    if (raiz->getDerecho() != nullptr) {
        MoveToEx(hdc, x, y, nullptr);
        LineTo(hdc, x + desviamientosHorizontal, y + espacioVertical);
        dibujarArbol(hdc, raiz->getDerecho(), x + desviamientosHorizontal, y + espacioVertical, desviamientosHorizontal / 2);
    }

    SelectObject(hdc, GetStockObject(BLACK_BRUSH)); 
    Ellipse(hdc, x - radio, y - radio, x + radio, y + radio);

    string texto = to_string(raiz->getValor());
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    int offset = (texto.length() > 1) ? 8 : 4;
    TextOutA(hdc, x - offset, y - 7, texto.c_str(), texto.length());
}

int main() {
    int anchoPantalla = 0;
    int altoPantalla = 0;

    obtenerYMostrarInfoPantalla(anchoPantalla, altoPantalla);
    cout << "\nPresiona ENTER para iniciar el programa interactivo...";
    cin.get();

    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);

    HPEN lapiz = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, lapiz);

    Nodo* raiz = nullptr;
    int opcionSeleccionada = 1; // 1 = Insertar, 2 = Salir
    int tecla = 0;
    int valor = 0;
    bool ejecutar = true;

    while (ejecutar) {
        system("cls");
        InvalidateRect(hwnd, nullptr, TRUE);
        UpdateWindow(hwnd);
        Sleep(50); 

        int xInicial = anchoPantalla / 3; 
        int yInicial = 220; 
        int dispersionInicial = anchoPantalla / 12;

        if (raiz != nullptr) {
            dibujarArbol(hdc, raiz, xInicial, yInicial, dispersionInicial);
        }

        cout << "========================================" << endl;
        cout << "    MENU ARBOL BINARIO INTERACTIVO      " << endl;
        cout << "  (Usa las flechas y presiona ENTER)   " << endl;
        cout << "========================================" << endl;
        
        // Renderizado condicional del puntero '>'
        if (opcionSeleccionada == 1) {
            cout << " > [1] Insertar Nodo" << endl;
            cout << "   [2] Salir" << endl;
        } else {
            cout << "   [1] Insertar Nodo" << endl;
            cout << " > [2] Salir" << endl;
        }
        cout << "========================================" << endl;

        // Captura de teclado síncrona sin necesidad de usar cin >>
        tecla = _getch();

        if (tecla == 0 || tecla == 224) { // Es una tecla especial (flechas)
            tecla = _getch(); // Lee el código real de la flecha
            if (tecla == ARRIBA) {
                opcionSeleccionada = 1;
            } else if (tecla == ABAJO) {
                opcionSeleccionada = 2;
            }
        } else if (tecla == ENTER) {
            if (opcionSeleccionada == 1) {
                cout << "\nIngrese el valor entero: ";
                if (cin >> valor) {
                    raiz = insertar(raiz, valor);
                } else {
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            } else if (opcionSeleccionada == 2) {
                ejecutar = false;
            }
        }
    }

    DeleteObject(lapiz);
    ReleaseDC(hwnd, hdc);
    
    if (raiz != nullptr) {
        delete raiz;
    }

    return 0;
}