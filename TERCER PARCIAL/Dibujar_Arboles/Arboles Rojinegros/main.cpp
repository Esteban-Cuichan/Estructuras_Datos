#include <windows.h>
#include <string>
#include <map>
#include <climits>
#include <vector>
#include <algorithm>
using namespace std;

// ==================== CLASE NODO ROJO-NEGRO ====================
enum Color { ROJO, NEGRO };

class Nodo {
private:
    int valor;
    Color color;
    Nodo* izquierdo;
    Nodo* derecho;
    Nodo* padre;

public:
    // Constructor: los nodos nuevos siempre son ROJOS
    Nodo(int val) : valor(val), color(ROJO), izquierdo(nullptr), derecho(nullptr), padre(nullptr) {}
    
    // Getters (const)
    int getValor() const { return valor; }
    Color getColor() const { return color; }
    Nodo* getIzquierdo() const { return izquierdo; }
    Nodo* getDerecho() const { return derecho; }
    Nodo* getPadre() const { return padre; }
    
    // Setters (normales)
    void setValor(int val) { valor = val; }
    void setColor(Color c) { color = c; }
    void setIzquierdo(Nodo* nodo) { 
        izquierdo = nodo; 
        if (nodo) nodo->padre = this; 
    }
    void setDerecho(Nodo* nodo) { 
        derecho = nodo; 
        if (nodo) nodo->padre = this; 
    }
    void setPadre(Nodo* nodo) { padre = nodo; }
    
    // ===== REFERENCIAS PARA ROTACIONES =====
    // Estas permiten modificar los punteros directamente sin romper encapsulación
    Nodo*& refIzquierdo() { return izquierdo; }
    Nodo*& refDerecho() { return derecho; }
    Nodo*& refPadre() { return padre; }
    
    // Funciones auxiliares para el RBT
    bool esRojo() const { return color == ROJO; }
    bool esNegro() const { return color == NEGRO; }
    
    // Obtener abuelo (si existe)
    Nodo* getAbuelo() const {
        if (padre != nullptr && padre->padre != nullptr)
            return padre->padre;
        return nullptr;
    }
    
    // Obtener tío (si existe)
    Nodo* getTio() const {
        Nodo* abuelo = getAbuelo();
        if (abuelo == nullptr) return nullptr;
        if (padre == abuelo->izquierdo)
            return abuelo->derecho;
        else
            return abuelo->izquierdo;
    }
    
    // Obtener hermano (si existe)
    Nodo* getHermano() const {
        if (padre == nullptr) return nullptr;
        if (this == padre->izquierdo)
            return padre->derecho;
        else
            return padre->izquierdo;
    }
    
    // Verificar si es hijo izquierdo o derecho
    bool esHijoIzquierdo() const {
        return (padre != nullptr && this == padre->izquierdo);
    }
    
    bool esHijoDerecho() const {
        return (padre != nullptr && this == padre->derecho);
    }
    
    // Destructor recursivo
    ~Nodo() {
        delete izquierdo;
        delete derecho;
    }
};

// ==================== VARIABLES GLOBALES ====================
Nodo* raizGlobal = nullptr;
Nodo* ultimoNodoGlobal = nullptr;
wstring entradaUsuario = L"";
bool programaActivo = true;
HWND hwndPrincipal = nullptr;

// ==================== FUNCIONES DEL ÁRBOL ROJO-NEGRO ====================

/**
 * Rotación a la izquierda
 * @param raiz Puntero a la raíz del árbol (se pasa por referencia)
 * @param nodo Nodo a rotar
 */
void rotacionIzquierda(Nodo*& raiz, Nodo* nodo) {
    if (nodo == nullptr) return;
    
    Nodo* hijoDerecho = nodo->getDerecho();
    if (hijoDerecho == nullptr) return;
    
    // PASO 1: El hijo izquierdo del hijo derecho se convierte en el hijo derecho del nodo
    nodo->refDerecho() = hijoDerecho->getIzquierdo();
    if (nodo->getDerecho() != nullptr) {
        nodo->getDerecho()->refPadre() = nodo;
    }
    
    // PASO 2: El hijo derecho asciende
    hijoDerecho->refPadre() = nodo->getPadre();
    
    // PASO 3: Actualizar el padre del nodo original
    if (nodo->getPadre() == nullptr) {
        raiz = hijoDerecho;
    } else if (nodo == nodo->getPadre()->getIzquierdo()) {
        nodo->getPadre()->refIzquierdo() = hijoDerecho;
    } else {
        nodo->getPadre()->refDerecho() = hijoDerecho;
    }
    
    // PASO 4: El nodo original desciende como hijo izquierdo
    hijoDerecho->refIzquierdo() = nodo;
    nodo->refPadre() = hijoDerecho;
}

/**
 * Rotación a la derecha
 * @param raiz Puntero a la raíz del árbol (se pasa por referencia)
 * @param nodo Nodo a rotar
 */
void rotacionDerecha(Nodo*& raiz, Nodo* nodo) {
    if (nodo == nullptr) return;
    
    Nodo* hijoIzquierdo = nodo->getIzquierdo();
    if (hijoIzquierdo == nullptr) return;
    
    // PASO 1: El hijo derecho del hijo izquierdo se convierte en el hijo izquierdo del nodo
    nodo->refIzquierdo() = hijoIzquierdo->getDerecho();
    if (nodo->getIzquierdo() != nullptr) {
        nodo->getIzquierdo()->refPadre() = nodo;
    }
    
    // PASO 2: El hijo izquierdo asciende
    hijoIzquierdo->refPadre() = nodo->getPadre();
    
    // PASO 3: Actualizar el padre del nodo original
    if (nodo->getPadre() == nullptr) {
        raiz = hijoIzquierdo;
    } else if (nodo == nodo->getPadre()->getIzquierdo()) {
        nodo->getPadre()->refIzquierdo() = hijoIzquierdo;
    } else {
        nodo->getPadre()->refDerecho() = hijoIzquierdo;
    }
    
    // PASO 4: El nodo original desciende como hijo derecho
    hijoIzquierdo->refDerecho() = nodo;
    nodo->refPadre() = hijoIzquierdo;
}

/**
 * Inserta un nodo en el árbol usando el algoritmo BST estándar
 * @param raiz Puntero a la raíz del árbol
 * @param nodo Nodo a insertar
 */
void insertarBST(Nodo*& raiz, Nodo* nodo) {
    if (raiz == nullptr) {
        raiz = nodo;
        return;
    }
    
    Nodo* actual = raiz;
    Nodo* padre = nullptr;
    
    while (actual != nullptr) {
        padre = actual;
        if (nodo->getValor() < actual->getValor()) {
            actual = actual->getIzquierdo();
        } else {
            actual = actual->getDerecho();
        }
    }
    
    nodo->setPadre(padre);
    if (nodo->getValor() < padre->getValor()) {
        padre->setIzquierdo(nodo);
    } else {
        padre->setDerecho(nodo);
    }
}

/**
 * Corrige las violaciones de las propiedades del Árbol Rojo-Negro después de la inserción
 * @param raiz Puntero a la raíz del árbol (se pasa por referencia)
 * @param nodo Nodo recién insertado
 */
void arreglarInsercion(Nodo*& raiz, Nodo* nodo) {
    // Caso 1: El nodo es la raíz
    if (nodo->getPadre() == nullptr) {
        nodo->setColor(NEGRO);
        return;
    }
    
    // Caso 2: El padre es negro -> no hay violación
    if (nodo->getPadre()->esNegro()) {
        return;
    }
    
    // Caso 3: El padre y el tío son rojos
    Nodo* padre = nodo->getPadre();
    Nodo* abuelo = nodo->getAbuelo();
    Nodo* tio = nodo->getTio();
    
    if (tio != nullptr && tio->esRojo()) {
        // Cambiar colores
        padre->setColor(NEGRO);
        tio->setColor(NEGRO);
        abuelo->setColor(ROJO);
        
        // Recursivamente arreglar el abuelo
        arreglarInsercion(raiz, abuelo);
        return;
    }
    
    // Caso 4: El tío es negro o nulo
    // Caso 4.1: El nodo es hijo derecho y el padre es hijo izquierdo (caso zig-zag)
    if (nodo->esHijoDerecho() && padre->esHijoIzquierdo()) {
        rotacionIzquierda(raiz, padre);
        // Ahora el nodo original es el padre y el padre original es su hijo
        nodo = padre;
        padre = nodo->getPadre();
    }
    // Caso 4.2: El nodo es hijo izquierdo y el padre es hijo derecho (caso zig-zag espejo)
    else if (nodo->esHijoIzquierdo() && padre->esHijoDerecho()) {
        rotacionDerecha(raiz, padre);
        nodo = padre;
        padre = nodo->getPadre();
    }
    
    // Caso 5: El padre y el nodo están alineados
    // Caso 5.1: El nodo es hijo izquierdo y el padre es hijo izquierdo (caso zig-zig)
    if (nodo->esHijoIzquierdo() && padre->esHijoIzquierdo()) {
        padre->setColor(NEGRO);
        if (abuelo != nullptr) {
            abuelo->setColor(ROJO);
            rotacionDerecha(raiz, abuelo);
        }
    }
    // Caso 5.2: El nodo es hijo derecho y el padre es hijo derecho (caso zig-zig espejo)
    else if (nodo->esHijoDerecho() && padre->esHijoDerecho()) {
        padre->setColor(NEGRO);
        if (abuelo != nullptr) {
            abuelo->setColor(ROJO);
            rotacionIzquierda(raiz, abuelo);
        }
    }
    
    // Asegurar que la raíz siempre sea negra
    if (raiz != nullptr) {
        raiz->setColor(NEGRO);
    }
}

/**
 * Inserta un valor en el Árbol Rojo-Negro
 * @param raiz Puntero a la raíz del árbol (se pasa por referencia)
 * @param valor Valor a insertar
 * @return Puntero al nodo insertado (para resaltarlo en la interfaz)
 */
Nodo* insertar(Nodo*& raiz, int valor) {
    // Crear nuevo nodo (siempre ROJO)
    Nodo* nuevoNodo = new Nodo(valor);
    
    // Insertar usando BST estándar
    insertarBST(raiz, nuevoNodo);
    
    // Corregir violaciones del RBT
    arreglarInsercion(raiz, nuevoNodo);
    
    // Asegurar que la raíz sea negra (por si acaso)
    if (raiz != nullptr) {
        raiz->setColor(NEGRO);
    }
    
    return nuevoNodo;
}

/**
 * Busca un nodo por su valor
 * @param raiz Raíz del árbol
 * @param valor Valor a buscar
 * @return Puntero al nodo encontrado, o nullptr si no existe
 */
Nodo* buscarNodo(Nodo* raiz, int valor) {
    if (raiz == nullptr) return nullptr;
    if (raiz->getValor() == valor) return raiz;
    if (valor < raiz->getValor()) return buscarNodo(raiz->getIzquierdo(), valor);
    return buscarNodo(raiz->getDerecho(), valor);
}

/**
 * Cuenta la cantidad de nodos en el árbol
 */
int contarNodos(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    return 1 + contarNodos(raiz->getIzquierdo()) + contarNodos(raiz->getDerecho());
}

/**
 * Calcula la altura del árbol (máximo nivel)
 */
int calcularAltura(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    int altIzq = calcularAltura(raiz->getIzquierdo());
    int altDer = calcularAltura(raiz->getDerecho());
    return 1 + max(altIzq, altDer);
}

/**
 * Cuenta la cantidad de nodos rojos en el árbol
 */
int contarRojos(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    int rojos = (raiz->esRojo()) ? 1 : 0;
    return rojos + contarRojos(raiz->getIzquierdo()) + contarRojos(raiz->getDerecho());
}

/**
 * Cuenta la cantidad de nodos negros en el árbol
 */
int contarNegros(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    int negros = (raiz->esNegro()) ? 1 : 0;
    return negros + contarNegros(raiz->getIzquierdo()) + contarNegros(raiz->getDerecho());
}

// ==================== FUNCIONES DE POSICIONAMIENTO ====================

void calcularPosiciones(Nodo* raiz, int nivel, map<int, int>& nextX, map<Nodo*, int>& posX) {
    if (raiz == nullptr) return;
    
    const int separacion = 110;
    
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

// ==================== FUNCIONES DE DIBUJO ====================

void dibujarArbolRT(HDC hdc, Nodo* raiz, int nivel, int espacioVertical,
                    map<Nodo*, int>& posX, int offsetX, int yInicial, Nodo* ultimoNodo) {
    if (raiz == nullptr) return;
    
    int radio = 20;
    int x = posX[raiz] + offsetX;
    int y = yInicial + nivel * espacioVertical;
    
    // Dibujar ramas
    HPEN lapizRamas = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
    SelectObject(hdc, lapizRamas);
    
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
    
    DeleteObject(lapizRamas);
    
    // ===== DIBUJO DEL NODO MODIFICADO PARA RBT =====
    HBRUSH fondoNodo;
    HPEN bordeNodo;
    COLORREF colorTexto;
    
    // Verificar si es el último nodo insertado (prioridad máxima)
    if (raiz == ultimoNodo) {
        // Último nodo insertado: fondo amarillo con borde dorado
        fondoNodo = CreateSolidBrush(RGB(255, 255, 0));      // Amarillo
        bordeNodo = CreatePen(PS_SOLID, 3, RGB(255, 200, 0)); // Dorado
        colorTexto = RGB(0, 0, 0);                           // Texto negro
    } else if (raiz->esRojo()) {
        // Nodo ROJO: fondo rojo, borde rojo oscuro, texto blanco
        fondoNodo = CreateSolidBrush(RGB(255, 0, 0));         // Rojo
        bordeNodo = CreatePen(PS_SOLID, 2, RGB(180, 0, 0));   // Rojo oscuro
        colorTexto = RGB(255, 255, 255);                      // Blanco
    } else { // Nodo NEGRO
        // Nodo NEGRO: fondo negro, borde gris, texto blanco
        fondoNodo = CreateSolidBrush(RGB(0, 0, 0));           // Negro
        bordeNodo = CreatePen(PS_SOLID, 2, RGB(150, 150, 150)); // Gris
        colorTexto = RGB(255, 255, 255);                      // Blanco
    }
    
    SelectObject(hdc, fondoNodo);
    SelectObject(hdc, bordeNodo);
    SetTextColor(hdc, colorTexto);
    
    Ellipse(hdc, x - radio, y - radio, x + radio, y + radio);
    
    // Dibujar el valor del nodo
    string texto = to_string(raiz->getValor());
    SetBkMode(hdc, TRANSPARENT);
    int offsetTexto = (texto.length() > 1) ? 8 : 4;
    TextOutA(hdc, x - offsetTexto, y - 7, texto.c_str(), texto.length());
    
    DeleteObject(fondoNodo);
    DeleteObject(bordeNodo);
}

// ==================== INTERFAZ GRÁFICA ====================

void dibujarInterfaz(HDC hdc, RECT rect) {
    int ancho = rect.right - rect.left;
    int alto = rect.bottom - rect.top;
    
    // Fondo
    HBRUSH fondo = CreateSolidBrush(RGB(20, 20, 40));
    FillRect(hdc, &rect, fondo);
    DeleteObject(fondo);
    
    // Marco de entrada
    HPEN lapizMarco = CreatePen(PS_SOLID, 2, RGB(100, 100, 150));
    HBRUSH fondoMarco = CreateSolidBrush(RGB(40, 40, 60));
    
    RECT rectMarco = {10, 35, 300, 65};
    FillRect(hdc, &rectMarco, fondoMarco);
    SelectObject(hdc, lapizMarco);
    Rectangle(hdc, 10, 35, 300, 65);
    
    DeleteObject(lapizMarco);
    DeleteObject(fondoMarco);
    
    // Etiqueta y entrada
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(200, 200, 200));
    LPCWSTR label = L"Número (-1 para salir):";
    TextOutW(hdc, 15, 42, label, wcslen(label));
    
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOutW(hdc, 215, 42, entradaUsuario.c_str(), entradaUsuario.length());
    
    // ===== INFORMACIÓN DEL ÁRBOL ROJO-NEGRO =====
    wstring info = L"";
    if (raizGlobal != nullptr) {
        int total = contarNodos(raizGlobal);
        int altura = calcularAltura(raizGlobal);
        int rojos = contarRojos(raizGlobal);
        int negros = contarNegros(raizGlobal);
        
        string ultimoValor = (ultimoNodoGlobal != nullptr) ? to_string(ultimoNodoGlobal->getValor()) : "-";
        wstring wUltimoValor(ultimoValor.begin(), ultimoValor.end());
        wstring colorRaiz = (raizGlobal->esRojo()) ? L"ROJO" : L"NEGRO";
        
        info = L"Nodos: " + to_wstring(total) +
               L"  Altura: " + to_wstring(altura) +
               L"  Rojos: " + to_wstring(rojos) +
               L"  Negros: " + to_wstring(negros) +
               L"  Raíz: " + colorRaiz +
               L"  Último: " + wUltimoValor;
    } else {
        info = L"Árbol Rojo-Negro vacío - Ingrese un número";
    }
    SetTextColor(hdc, RGB(150, 150, 200));
    TextOutW(hdc, 10, 10, info.c_str(), info.length());
    
    // Instrucciones
    LPCWSTR ayuda = L"Presione ENTER para agregar | ESC para salir";
    SetTextColor(hdc, RGB(100, 100, 150));
    TextOutW(hdc, 10, alto - 30, ayuda, wcslen(ayuda));
    
    // Dibujar árbol
    if (raizGlobal != nullptr) {
        map<int, int> nextX;
        map<Nodo*, int> posX;
        
        calcularPosiciones(raizGlobal, 0, nextX, posX);
        int offsetX = calcularOffsetOptimo(posX, ancho);
        
        dibujarArbolRT(hdc, raizGlobal, 0, 80, posX, offsetX, 120, ultimoNodoGlobal);
    }
}

// ==================== PROCEDIMIENTO DE VENTANA ====================

LRESULT CALLBACK VentanaProcedure(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam) {
    switch(mensaje) {
        case WM_CREATE:
            SetFocus(hwnd);
            break;
            
        case WM_KEYDOWN: {
            switch(wParam) {
                case VK_RETURN: {
                    if (!entradaUsuario.empty()) {
                        try {
                            string entradaStr(entradaUsuario.begin(), entradaUsuario.end());
                            int valor = stoi(entradaStr);
                            if (valor == -1) {
                                programaActivo = false;
                                DestroyWindow(hwnd);
                            } else {
                                // Insertar en el Árbol Rojo-Negro
                                ultimoNodoGlobal = insertar(raizGlobal, valor);
                                entradaUsuario = L"";
                                InvalidateRect(hwnd, nullptr, TRUE);
                            }
                        } catch (...) {
                            entradaUsuario = L"";
                            InvalidateRect(hwnd, nullptr, TRUE);
                        }
                    }
                    break;
                }
                
                case VK_ESCAPE: {
                    programaActivo = false;
                    DestroyWindow(hwnd);
                    break;
                }
                
                case VK_BACK: {
                    if (!entradaUsuario.empty()) {
                        entradaUsuario.pop_back();
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                    break;
                }
                
                default: {
                    wchar_t tecla = static_cast<wchar_t>(wParam);
                    if ((tecla >= L'0' && tecla <= L'9') || tecla == L'-') {
                        if (tecla == L'-' && !entradaUsuario.empty()) {
                            break;
                        }
                        entradaUsuario += tecla;
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                    break;
                }
            }
            break;
        }
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            RECT rect;
            GetClientRect(hwnd, &rect);
            
            dibujarInterfaz(hdc, rect);
            
            EndPaint(hwnd, &ps);
            break;
        }
        
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        
        default:
            return DefWindowProc(hwnd, mensaje, wParam, lParam);
    }
    return 0;
}

// ==================== PUNTO DE ENTRADA ====================

int WINAPI wWinMain(HINSTANCE hInst,
                    HINSTANCE hPrevInst,
                    PWSTR lpCmdLine,
                    int nCmdShow) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = VentanaProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = L"ArbolRojoNegro";
    
    RegisterClassExW(&wc);
    
    int ancho = GetSystemMetrics(SM_CXSCREEN);
    int alto = GetSystemMetrics(SM_CYSCREEN);
    
    hwndPrincipal = CreateWindowExW(
        0,
        L"ArbolRojoNegro",
        L"Árbol Rojo-Negro (Red-Black Tree)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, ancho, alto,
        nullptr, nullptr, hInst, nullptr
    );
    
    ShowWindow(hwndPrincipal, SW_MAXIMIZE);
    
    MSG msg;
    while (programaActivo && GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    if (raizGlobal != nullptr) delete raizGlobal;
    
    return 0;
}