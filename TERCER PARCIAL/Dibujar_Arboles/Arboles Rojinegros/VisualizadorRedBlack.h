#ifndef VISUALIZADOR_REDBLACK_H
#define VISUALIZADOR_REDBLACK_H

#include "ArbolRojoNegro.h"
#include <windows.h>
#include <string>
#include <map>

class VisualizadorRedBlack
{
private:
    ArbolRojoNegro* arbol;
    HWND hwnd;
    HDC hdc;
    int anchoVentana;
    int altoVentana;
    int radioNodo;
    int separacionHorizontal;
    int separacionVertical;

    // Colores
    COLORREF colorFondo;
    COLORREF colorRama;
    
    // Métodos de dibujo
    void dibujarRama(int x1, int y1, int x2, int y2);
    void dibujarNodo(int valor, Color color, int x, int y, bool esUltimo);
    void dibujarInformacion();
    void dibujarRamas(Nodo* nodo, std::map<int, std::pair<int, int>>& posiciones);
    void dibujarNodos(Nodo* nodo, std::map<int, std::pair<int, int>>& posiciones);
    
    // Cálculo de posiciones
    void calcularPosiciones(Nodo* nodo, int x, int y, int nivel, 
                           std::map<int, std::pair<int, int>>& posiciones);
    int obtenerProfundidad(Nodo* nodo) const;

public:
    VisualizadorRedBlack(ArbolRojoNegro* arbol);
    ~VisualizadorRedBlack();
    
    void setHWND(HWND hwnd);
    void setHDC(HDC hdc);
    void setDimensiones(int ancho, int alto);
    
    void redibujar();
};

#endif