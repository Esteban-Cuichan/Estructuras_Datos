#include "sacarInfPantalla.h"
#include <iostream>
#include <windows.h>
#include <dxgi.h>
#include <string>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "user32.lib")

using namespace std;

void obtenerYMostrarInfoPantalla(int& ancho, int& alto) {
    cout << "=== Informacion de la pantalla ===" << endl;
    ancho = GetSystemMetrics(SM_CXSCREEN);
    alto = GetSystemMetrics(SM_CYSCREEN);
    cout << "Resolucion de la pantalla actual: " << ancho << " x " << alto << " pixeles" << endl;
    cout << "===================================" << endl;

    IDXGIFactory* pFactory = nullptr;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
        cout << "No se pudo inicializar DXGI para leer la GPU." << endl;
        return;
    }

    IDXGIAdapter* pAdapter = nullptr;
    if (pFactory->EnumAdapters(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC adapterDesc;
        pAdapter->GetDesc(&adapterDesc);
        wstring ws(adapterDesc.Description);
        string gpuName(ws.begin(), ws.end());
        cout << "Modelo de la tarjeta de video: " << gpuName << endl;
        double vramGB = static_cast<double>(adapterDesc.DedicatedVideoMemory) / (1024.0 * 1024.0 * 1024.0);
        double sharedMemGB = static_cast<double>(adapterDesc.SharedSystemMemory) / (1024.0 * 1024.0 * 1024.0);
        cout << "Memoria de video dedicada: " << vramGB << " GB" << endl;
        cout << "Memoria de sistema compartida: " << sharedMemGB << " GB" << endl;
        pAdapter->Release();
    } else {
        cerr << "No se detecto ninguna tarjeta de video compatible con DXGI." << endl;
    }
    pFactory->Release();
}