#include <iostream>
#include <string>
#define _RPCNDR_H_
typedef unsigned char std_byte;
#define byte std_byte
#include <windows.h>
#include <psapi.h>
#undef byte
#include "Lista.h"
#include "Nodo.h"
#include "../PRIMER PARCIAL/Librerias/ValidarCedula/validarCedula.h"
#include "../PRIMER PARCIAL/Librerias/validarExpresion/validarExpresion.h"

using namespace std;

auto deseaContinuar = [](char respuesta) -> bool {
    respuesta = tolower(respuesta);
    if(respuesta == 'y') return true;
    if(respuesta == 'n') return false;
    cout << "Operador invalido!\n Asumiendo salida por seguridad..." << endl;
    return false;
};

void mostrarRAMConsumida() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        SIZE_T ramEnBytes = pmc.WorkingSetSize;
        double ramEnKB = ramEnBytes / 1024.0;
        double ramEnMB = ramEnKB / 1024.0;

        cout << "Memoria RAM actual en uso: " << ramEnMB << " MB (" << ramEnBytes << " bytes)" << endl;
    } else {
        cout << "No se pudo obtener el análisis de la RAM física." << endl;
    }
}

int main() {
    string cedula, nombre, apellido;
    const char* charcedula;
    char aux;
    bool continuar = true;
    Lista* miLista = new Lista();
    
    while(continuar != false) {
        cout << "-------- INGRESO DE DATOS ---------" << endl;
        cout << "Ingrese su cedula: ";
        cin >> cedula;
        charcedula = cedula.c_str();
        if(validarCedula::validar(charcedula)) {
            if(miLista->busquedaDuplicado(cedula)) {
                cout << "Error: No se puede ingresar una cedula existente!" << endl;
                cout << "Intente nuevamente con datos diferentes!" << endl;
                miLista->limpiarpantalla();
                continue;
            }
            cout << "Ingrese su nombre: ";
            cin.ignore();
            getline(cin, nombre);
            if(validarExpresion::validarNombre(nombre)) {
                cout << "Ingrese su apellido: ";
                getline(cin, apellido);
                if(validarExpresion::validarNombre(apellido)) {
                    miLista->insertar(cedula, nombre, apellido);
                    miLista->guardarArchivoIngreso(nombre, cedula, apellido);
                    cout << "Desea agregar otro usuario? [Y/n]: ";
                    cin >> aux;
                    continuar = deseaContinuar(aux);
                    if(!continuar && (tolower(aux) != 'n')) {
                        delete miLista;
                        return 1;
                    }
                    miLista->limpiarpantalla();
                } else {
                    cout << "El apellido ingresado es incorrecto!" << endl;
                    cout << "Intente ingresar otros datos!" << endl;
                }
            } else {
                cout << "El nombre ingresado es incorrecto!" << endl;
                cout << "Intente ingresar otros datos!" << endl;
            }
        } else {
            cout << "La cedula ingresada no es valida!" << endl;
            cout << "Intente nuevamente con diferentes datos!" << endl;
            miLista->limpiarpantalla();
            continue;
        }
    }
    
    cout << "\n=== ANALISIS DE MEMORIA RAM ===" << endl;
    cout << "Tamano de un puntero en tu sistema: " << sizeof(void*) << " bytes." << endl;
    cout << "Tamano de un std::string: " << sizeof(string) << " bytes." << endl;
    cout << "Tamano estatico de la clase Nodo: " << sizeof(Nodo) << " bytes." << endl;
    cout << "Tamano estatico de la clase Lista (incluye vptr): " << sizeof(Lista) << " bytes." << endl;
    
    mostrarRAMConsumida(); 
    
    cout << "===============================" << endl;
    cout << "Registros en Memoria" << endl;
    miLista->imprimir();
    miLista->ordenamientoburbuja();
    miLista->imprimir();
    
    delete miLista;
    return 0;
}
    /*cout<<"Empezando a contar cedulas por provincia!"<<endl;
    miLista->conteoGeneral();
    cout<<"Eliminacion de digitos impares :)"<<endl;
    miLista->eliminarDigito();*/
