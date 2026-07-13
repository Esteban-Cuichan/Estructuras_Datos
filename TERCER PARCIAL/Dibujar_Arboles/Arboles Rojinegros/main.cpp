#include "ArbolRojoNegro.h"
#include "VisualizadorRedBlack.h"
#include "ConsolaUtils.h"
#include <windows.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <conio.h>

// Variables globales
ArbolRojoNegro arbol;
VisualizadorRedBlack visualizador(&arbol);
std::atomic<bool> ejecutando(true);
HWND hwndConsola;
HDC hdcConsola;
int anchoVentana = 800;
int altoVentana = 600;

// Función para procesar la entrada del usuario
void procesarEntrada()
{
    std::cout << "\n==================================================" << std::endl;
    std::cout << "     ARBOL ROJO-NEGRO CON VISUALIZACION GRAFICA    " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Ingrese numeros enteros para insertar." << std::endl;
    std::cout << "Escriba -1 para salir." << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "> ";
    
    while (ejecutando)
    {
        int valor;
        std::cin >> valor;
        
        if (!ejecutando)
            break;
            
        if (valor == -1)
        {
            ejecutando = false;
            break;
        }
        
        // Insertar el valor en el árbol
        arbol.insertar(valor);
        
        // Forzar un redibujado inmediato
        visualizador.setHDC(hdcConsola);
        visualizador.setDimensiones(anchoVentana, altoVentana);
        visualizador.redibujar();
        
        std::cout << "Insertado: " << valor << " | Nodos: " << arbol.getCantidadNodos() << std::endl;
        std::cout << "> ";
    }
}

// Función principal de dibujo
void dibujar()
{
    while (ejecutando)
    {
        hwndConsola = GetConsoleHwnd();
        if (hwndConsola != NULL)
        {
            hdcConsola = GetDC(hwndConsola);
            if (hdcConsola != NULL)
            {
                RECT rect;
                GetClientRect(hwndConsola, &rect);
                anchoVentana = rect.right;
                altoVentana = rect.bottom;
                
                visualizador.setHWND(hwndConsola);
                visualizador.setHDC(hdcConsola);
                visualizador.setDimensiones(anchoVentana, altoVentana);
                visualizador.redibujar();
                
                ReleaseDC(hwndConsola, hdcConsola);
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main()
{
    // Configurar la consola
    system("cls");
    
    // Maximizar la ventana
    HWND console = GetConsoleHwnd();
    if (console != NULL)
    {
        ShowWindow(console, SW_MAXIMIZE);
    }
    
    // Ocultar el cursor
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // Crear los hilos
    std::thread hiloEntrada(procesarEntrada);
    std::thread hiloDibujo(dibujar);
    
    // Esperar a que termine
    hiloEntrada.join();
    ejecutando = false;
    if (hiloDibujo.joinable())
    {
        hiloDibujo.join();
    }
    
    // Liberar recursos
    std::cout << "\nLiberando memoria..." << std::endl;
    arbol.limpiar();
    
    // Restaurar el cursor
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    std::cout << "Programa finalizado correctamente." << std::endl;
    std::cout << "Presione cualquier tecla para salir...";
    _getch();
    
    return 0;
}