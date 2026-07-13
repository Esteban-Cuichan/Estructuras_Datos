#include "VisualizadorRedBlack.h"
#include <sstream>
#include <cmath>
#include <algorithm>

VisualizadorRedBlack::VisualizadorRedBlack(ArbolRojoNegro* arbol) 
    : arbol(arbol), hwnd(nullptr), hdc(nullptr), 
      anchoVentana(800), altoVentana(600), 
      radioNodo(20), separacionHorizontal(50), separacionVertical(60)
{
    colorFondo = RGB(10, 10, 40);      // Azul oscuro
    colorRama = RGB(180, 180, 180);    // Gris claro
}

VisualizadorRedBlack::~VisualizadorRedBlack() {}

void VisualizadorRedBlack::setHWND(HWND hwnd)
{
    this->hwnd = hwnd;
}

void VisualizadorRedBlack::setHDC(HDC hdc)
{
    this->hdc = hdc;
}

void VisualizadorRedBlack::setDimensiones(int ancho, int alto)
{
    this->anchoVentana = ancho;
    this->altoVentana = alto;
}

int VisualizadorRedBlack::obtenerProfundidad(Nodo* nodo) const
{
    if (nodo == nullptr)
        return 0;
    return 1 + std::max(obtenerProfundidad(nodo->getIzquierdo()), 
                       obtenerProfundidad(nodo->getDerecho()));
}

void VisualizadorRedBlack::calcularPosiciones(Nodo* nodo, int x, int y, int nivel, 
                                             std::map<int, std::pair<int, int>>& posiciones)
{
    if (nodo == nullptr)
        return;
    
    posiciones[nodo->getValor()] = std::make_pair(x, y);
    
    // Calcular separación basada en el nivel
    int separacion = separacionHorizontal / (nivel + 1);
    if (separacion < 20) separacion = 20;
    
    if (nodo->getIzquierdo() != nullptr)
    {
        calcularPosiciones(nodo->getIzquierdo(), x - separacion, y + separacionVertical, 
                          nivel + 1, posiciones);
    }
    
    if (nodo->getDerecho() != nullptr)
    {
        calcularPosiciones(nodo->getDerecho(), x + separacion, y + separacionVertical, 
                          nivel + 1, posiciones);
    }
}

void VisualizadorRedBlack::dibujarRama(int x1, int y1, int x2, int y2)
{
    if (hdc == nullptr)
        return;
    
    HPEN pen = CreatePen(PS_SOLID, 2, colorRama);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    
    MoveToEx(hdc, x1, y1 + radioNodo, NULL);
    LineTo(hdc, x2, y2 - radioNodo);
    
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void VisualizadorRedBlack::dibujarNodo(int valor, Color color, int x, int y, bool esUltimo)
{
    if (hdc == nullptr)
        return;
    
    // Determinar colores según el estado del nodo
    COLORREF colorFondoNodo;
    COLORREF colorBordeNodo;
    COLORREF colorTexto;
    
    if (esUltimo)
    {
        // Último nodo insertado: amarillo
        colorFondoNodo = RGB(255, 255, 0);
        colorBordeNodo = RGB(255, 215, 0);
        colorTexto = RGB(0, 0, 0);
    }
    else if (color == ROJO)
    {
        colorFondoNodo = RGB(255, 0, 0);
        colorBordeNodo = RGB(200, 0, 0);
        colorTexto = RGB(255, 255, 255);
    }
    else // NEGRO
    {
        colorFondoNodo = RGB(0, 0, 0);
        colorBordeNodo = RGB(128, 128, 128);
        colorTexto = RGB(255, 255, 255);
    }
    
    // Sombra
    HBRUSH shadowBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, shadowBrush);
    HPEN shadowPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, shadowPen);
    Ellipse(hdc, x - radioNodo + 3, y - radioNodo + 3, x + radioNodo + 3, y + radioNodo + 3);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(shadowBrush);
    DeleteObject(shadowPen);
    
    // Círculo principal
    HBRUSH brush = CreateSolidBrush(colorFondoNodo);
    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    
    HPEN pen = CreatePen(PS_SOLID, 2, colorBordeNodo);
    oldPen = (HPEN)SelectObject(hdc, pen);
    
    Ellipse(hdc, x - radioNodo, y - radioNodo, x + radioNodo, y + radioNodo);
    
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
    
    // Dibujar texto
    std::string texto = std::to_string(valor);
    SetTextColor(hdc, colorTexto);
    SetBkMode(hdc, TRANSPARENT);
    
    RECT rect;
    rect.left = x - radioNodo;
    rect.top = y - radioNodo;
    rect.right = x + radioNodo;
    rect.bottom = y + radioNodo;
    
    DrawTextA(hdc, texto.c_str(), texto.length(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void VisualizadorRedBlack::dibujarInformacion()
{
    if (hdc == nullptr)
        return;
    
    HFONT font = CreateFontA(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    
    // Fondo semitransparente para la información
    RECT rectFondo = {10, 10, 600, 50};
    HBRUSH brushFondo = CreateSolidBrush(RGB(0, 0, 0));
    SetBkMode(hdc, OPAQUE);
    FillRect(hdc, &rectFondo, brushFondo);
    DeleteObject(brushFondo);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    std::stringstream ss;
    ss << "Nodos: " << arbol->getCantidadNodos()
       << " | Altura: " << arbol->getAltura()
       << " | Ultimo: " << (arbol->getUltimoInsertado() != nullptr ? 
                           std::to_string(arbol->getUltimoInsertado()->getValor()) : "N/A")
       << " | Rojos: " << arbol->getCantidadRojos()
       << " | Negros: " << arbol->getCantidadNegros();
    
    std::string info = ss.str();
    TextOutA(hdc, 20, 20, info.c_str(), info.length());
    
    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

void VisualizadorRedBlack::redibujar()
{
    if (hdc == nullptr)
    {
        if (hwnd != nullptr)
        {
            hdc = GetDC(hwnd);
            if (hdc == nullptr)
                return;
        }
        else
        {
            return;
        }
    }
    
    // Limpiar fondo
    HBRUSH brush = CreateSolidBrush(colorFondo);
    RECT rect = {0, 0, anchoVentana, altoVentana};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    
    // Dibujar información
    dibujarInformacion();
    
    // Dibujar árbol si existe
    Nodo* raiz = arbol->getRaiz();
    if (raiz != nullptr)
    {
        std::map<int, std::pair<int, int>> posiciones;
        calcularPosiciones(raiz, anchoVentana / 2, 80, 1, posiciones);
        
        dibujarRamas(raiz, posiciones);
        dibujarNodos(raiz, posiciones);
    }
}

void VisualizadorRedBlack::dibujarRamas(Nodo* nodo, std::map<int, std::pair<int, int>>& posiciones)
{
    if (nodo == nullptr)
        return;
    
    int x = posiciones[nodo->getValor()].first;
    int y = posiciones[nodo->getValor()].second;
    
    if (nodo->getIzquierdo() != nullptr)
    {
        Nodo* hijoIzq = nodo->getIzquierdo();
        int xIzq = posiciones[hijoIzq->getValor()].first;
        int yIzq = posiciones[hijoIzq->getValor()].second;
        dibujarRama(x, y, xIzq, yIzq);
        dibujarRamas(hijoIzq, posiciones);
    }
    
    if (nodo->getDerecho() != nullptr)
    {
        Nodo* hijoDer = nodo->getDerecho();
        int xDer = posiciones[hijoDer->getValor()].first;
        int yDer = posiciones[hijoDer->getValor()].second;
        dibujarRama(x, y, xDer, yDer);
        dibujarRamas(hijoDer, posiciones);
    }
}

void VisualizadorRedBlack::dibujarNodos(Nodo* nodo, std::map<int, std::pair<int, int>>& posiciones)
{
    if (nodo == nullptr)
        return;
    
    int x = posiciones[nodo->getValor()].first;
    int y = posiciones[nodo->getValor()].second;
    
    bool esUltimo = (arbol->getUltimoInsertado() == nodo);
    dibujarNodo(nodo->getValor(), nodo->getColor(), x, y, esUltimo);
    
    dibujarNodos(nodo->getIzquierdo(), posiciones);
    dibujarNodos(nodo->getDerecho(), posiciones);
}