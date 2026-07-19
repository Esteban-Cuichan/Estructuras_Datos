#include <iostream>
#include <string>
#include <exception>

#include "Lista.h"
#include "Conexion.h"
#include "Traductor.h"
#include "Audio.h"

using namespace std;

int main() {
    cout<<"==========================\n";
    cout<<"TRADUCTOR ESPANOL - INGLES\n";
    cout<<"==========================\n";

    Conexion* conexion=nullptr;
    Traductor* traductor=nullptr;
    Audio* audio=nullptr;
    Lista* lista=nullptr;

    try {
        conexion=new Conexion();
        string endpoint="https://libretranslate.de/translate";
        traductor=new Traductor(conexion, endpoint);
        audio=new Audio(conexion);
        lista=new Lista();
    } catch (const exception& ex) {
        cerr<<"Error al inicializar componentes: "<<ex.what()<<endl;
        delete conexion;
        delete traductor;
        delete audio;
        delete lista;
        return 1;
    }

    bool running=true;
    while (running) {
        cout<<"\nIngrese una palabra en espanol (o escriba 'menu' para opciones):\n";
        string palabra;
        getline(cin, palabra);
        if (palabra.empty()) continue;
        if (palabra=="menu") {
            int opcion=0;
            while (true) {
                cout<<"\nMENU\n";
                cout<<"1. Traducir otra palabra\n";
                cout<<"2. Mostrar historial\n";
                cout<<"3. Buscar traduccion\n";
                cout<<"4. Eliminar traduccion\n";
                cout<<"5. Salir\n";
                cout<<"Seleccione una opcion: ";
                string linea;
                getline(cin, linea);
                if (linea.empty()) continue;
                try {
                    opcion=stoi(linea);
                } catch (...) {
                    cout<<"Opcion invalida."<<endl;
                    continue;
                }

                if (opcion==1) break; 
                else if (opcion==2) {
                    lista->mostrar();
                } else if (opcion==3) {
                    cout<<"Ingrese palabra a buscar: ";
                    string clave;
                    getline(cin, clave);
                    if (clave.empty()) continue;
                    Nodo* res=lista->buscar(clave);
                    if (res) {
                        cout<<res->getPalabraEspanol()<<" -> "<<res->getPalabraIngles()<<endl;
                    } else {
                        cout<<"No encontrada en el historial."<<endl;
                    }
                } else if (opcion==4) {
                    cout<<"Ingrese palabra a eliminar: ";
                    string clave;
                    getline(cin, clave);
                    if (clave.empty()) continue;
                    bool ok=lista->eliminar(clave);
                    if (ok) cout<<"Eliminado."<<endl;
                    else cout<<"No encontrada."<<endl;
                } else if (opcion==5) {
                    running=false;
                    break;
                } else {
                    cout<<"Opcion invalida."<<endl;
                }
            }
            if (!running) break;
            continue;
        }

        try {
            cout<<"\nConsultando traduccion...\n";
            string traducido=traductor->traducir(palabra);
            if (traducido.empty()) {
                cout<<"No se obtuvo traduccion."<<endl;
            } else {
                cout<<"Traduccion:\n"<<traducido<<endl;
                lista->insertar(palabra, traducido);

                cout<<"Reproduciendo pronunciacion...\n";
                bool okAudio=audio->reproducirTexto(traducido);
                if (!okAudio) cout<<"No se pudo reproducir audio."<<endl;
            }
        } catch (const exception& ex) {
            cerr<<"Error durante la traduccion o reproduccion: "<<ex.what()<<endl;
        }
    }

    if (lista) {
        lista->liberarMemoria();
    }
    delete lista;
    delete audio;
    delete traductor;
    delete conexion;

    cout<<"Saliendo. Memoria liberada."<<endl;
    return 0;
}