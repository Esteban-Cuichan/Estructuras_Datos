#include "sacarInfPantalla.h"
#include <windows.h>

void obtenerYMostrarInfoPantalla(int& ancho, int& alto) {
    ancho = GetSystemMetrics(SM_CXSCREEN);
    alto = GetSystemMetrics(SM_CYSCREEN);
}