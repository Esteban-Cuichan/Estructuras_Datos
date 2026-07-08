#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Variables globales para las dimensiones
int N = 8; 
const int TAM_CASILLA = 60; 

// Matriz global donde guardaremos lo que leamos del archivo de texto
vector<vector<int>> matriz_tablero;

// Paso 1: Función que crea el archivo de texto simulando el tablero
void crearArchivoTablero(int tamano) {
    ofstream archivo("tablero.txt");
    
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo de texto.\n";
        return;
    }

    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            // Intercalamos 0 y 1 para hacer el patrón de ajedrez
            // 0 = Negro, 1 = Blanco
            if ((i + j) % 2 == 0) {
                archivo << "1 "; // Casilla blanca
            } else {
                archivo << "0 "; // Casilla negra
            }
        }
        archivo << "\n"; // Siguiente fila
    }

    archivo.close();
    cout << "[OK] Archivo 'tablero.txt' generado con exito.\n";
}

// Paso 2: Función que lee el archivo de texto y lo carga en memoria
void leerArchivoTablero() {
    ifstream archivo("tablero.txt");
    
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo 'tablero.txt' para lectura.\n";
        return;
    }

    matriz_tablero.clear();
    int valor;
    
    for (int i = 0; i < N; i++) {
        vector<int> fila;
        for (int j = 0; j < N; j++) {
            archivo >> valor;
            fila.push_back(valor);
        }
        matriz_tablero.push_back(fila);
    }

    archivo.close();
    cout << "[OK] Archivo 'tablero.txt' leido correctamente.\n";
}

// Paso 3: Función que dibuja basándose estrictamente en lo leído del TXT
void DibujarTableroDesdeMatriz(HWND hwnd, HDC hdc) {
    // Creamos los pinceles de color Negro y Blanco tal como solicitaste
    HBRUSH colorBlanco = CreateSolidBrush(RGB(255, 255, 255)); 
    HBRUSH colorNegro  = CreateSolidBrush(RGB(0, 0, 0));  

    for (int fila = 0; fila < N; fila++) {
        for (int col = 0; col < N; col++) {
            RECT casilla;
            casilla.left   = col * TAM_CASILLA + 20;  
            casilla.top    = fila * TAM_CASILLA + 20; 
            casilla.right  = casilla.left + TAM_CASILLA;
            casilla.bottom = casilla.top + TAM_CASILLA;

            // Evaluamos el valor del archivo de texto cargado en la matriz
            if (matriz_tablero[fila][col] == 1) {
                FillRect(hdc, &casilla, colorBlanco); // 1 = Blanco
            } else {
                FillRect(hdc, &casilla, colorNegro);  // 0 = Negro
            }
        }
    }

    // Liberar memoria
    DeleteObject(colorBlanco);
    DeleteObject(colorNegro);
}

// Procedimiento de la Ventana Nativa
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Dibujamos usando la matriz que leímos del archivo
            DibujarTableroDesdeMatriz(hwnd, hdc);
            
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int main() {
    cout << "=========================================\n";
    cout << " GENERADOR DE TABLAS POR ARCHIVO DE TEXTO \n";
    cout << "=========================================\n";
    cout << "Ingrese el tamano de la tabla (1 a 8): ";
    cin >> N;

    if (N < 1 || N > 10) {
        cout << "Por favor, ingresa un numero entre 1 y 10.\n";
        return 1;
    }

    // Ejecución de la lógica solicitada
    crearArchivoTablero(N);
    leerArchivoTablero();

    // Configuración y registro de la ventana gráfica de Windows
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("ClaseTablaTxt");
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

    if (!RegisterClass(&wc)) {
        cout << "Error al registrar la ventana.\n";
        return 1;
    }

    // Ajustar tamaño de ventana
    int anchoVentana = (N * TAM_CASILLA) + 60;
    int altoVentana = (N * TAM_CASILLA) + 80;

    HWND hwnd = CreateWindowEx(
        0, 
        TEXT("ClaseTablaTxt"), 
        TEXT("Tablero cargado desde TXT (0=Negro, 1=Blanco)"),
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 
        CW_USEDEFAULT, CW_USEDEFAULT, anchoVentana, altoVentana,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Bucle para mantener viva la ventana
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}