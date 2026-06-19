#ifndef GESTORMENU_H
#define GESTORMENU_H

#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <sstream>
#include <fstream>
#include "ListaDCE.h"
#include "Persona.h"
#include "Vehiculo.h"
#include "Fecha.h"
#include "Turno.h"
#include "validarCedula.h"
#include "validarExpresion.h"

#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_ENTER 13

using namespace std;

struct IntervaloTiempo {
    int hora;
    int minuto;
    bool disponible;
    string textoVisual;
};

class GestorMenu {
    private:
        ListaDCE& sistemaTurnos;
        validarCedula validadorCedula;
        validarExpresion validador;

        void cambiarColor(int texto, int fondo) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, (fondo << 4) | texto);
        }

        void mostrarCursorTexto(bool mostrar) {
            HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(hOutput, &cursorInfo);
            cursorInfo.bVisible = mostrar;
            SetConsoleCursorInfo(hOutput, &cursorInfo);
        }

        string calcularHashArchivo(const string& ruta) {
            ifstream archivo(ruta, ios::in | ios::binary);
            if (!archivo.is_open()) return "0";
            unsigned long hash = 5381;
            char caracter;
            while (archivo.get(caracter)) {
                hash = ((hash << 5) + hash) + caracter;
            }
            archivo.close();
            return to_string(hash);
        }

        int leerEntero(const string& mensaje) {
            string entrada;
            int valor;
            while (true) {
                cout << mensaje;
                if (!getline(cin, entrada)) {
                    cin.clear();
                    continue;
                }
                if (entrada.empty()) {
                    cout << "[ERROR] No puede dejar el campo vacio. Ingrese un numero.\n" << endl;
                    continue;
                }
                bool esNumeroPuro = true;
                for (char c : entrada) {
                    if (!isdigit(c)) {
                        esNumeroPuro = false;
                        break;
                    }
                }
                if (!esNumeroPuro) {
                    cout << "[ERROR] Entrada invalida. Ingrese UNICAMENTE los numeros enteros.\n" << endl;
                    continue;
                }
                stringstream ss(entrada);
                ss >> valor;
                return valor;
            }
        }

        string leerSoloNumeros(const string& mensaje, size_t longitudMaxima) {
            cout << mensaje;
            string entrada = "";
            while (true) {
                int tecla = _getch();
                if (tecla == 13) { 
                    if (entrada.empty()) continue;
                    break;
                }
                else if (tecla == 8) { 
                    if (!entrada.empty()) {
                        entrada.pop_back();
                        cout << "\b \b";
                    }
                }
                else if (tecla >= '0' && tecla <= '9') { 
                    if (entrada.length() < longitudMaxima) {
                        entrada.push_back(static_cast<char>(tecla));
                        cout << static_cast<char>(tecla);
                    }
                }
            }
            cout << endl;
            return entrada;
        }

        string leerSoloLetras(const string& mensaje, size_t longitudMaxima) {
            cout << mensaje;
            string entrada = "";
            while (true) {
                int tecla = _getch();
                if (tecla == 13) { 
                    if (entrada.empty()) continue;
                    break;
                }
                else if (tecla == 8) { 
                    if (!entrada.empty()) {
                        entrada.pop_back();
                        cout << "\b \b";
                    }
                }
                else if ((tecla >= 'A' && tecla <= 'Z') || (tecla >= 'a' && tecla <= 'z') || tecla == 32) { 
                    if (entrada.length() < longitudMaxima) {
                        entrada.push_back(static_cast<char>(tecla));
                        cout << static_cast<char>(tecla);
                    }
                }
            }
            cout << endl;
            return entrada;
        }

        int leerEnteroBloqueado(const string& mensaje, size_t longitudMaxima) {
            string numeroStr = leerSoloNumeros(mensaje, longitudMaxima);
            stringstream ss(numeroStr);
            int valor;
            ss >> valor;
            return valor;
        }

        bool verificarCitaOcupada(int d, int m, int a, int hh, int mm) {
            if (sistemaTurnos.getCabeza() == nullptr) return false;
            Nodo* actual = sistemaTurnos.getCabeza();
            do {
                Turno* t = actual->getTurno();
                if (t != nullptr && t->getEstado()) { 
                    Fecha f = t->getFecha();
                    if (f.getDia() == d && f.getMes() == m && f.getYear() == a &&
                        f.getHora() == hh && f.getMinutos() == mm) {
                        return true;
                    }
                }
                actual = actual->getSiguiente();
            } while (actual != sistemaTurnos.getCabeza());
            return false;
        }

        bool seleccionarBloqueHorario(int d, int m, int a, int anioActual, int mesActual, int diaActual, int horaActual, int minActual, int& horaSeleccionada, int& minutoSeleccionado) {
            mostrarCursorTexto(false);
            const int TOTAL_INTERVALOS = 32;
            IntervaloTiempo bloques[TOTAL_INTERVALOS];
            int indice = 0;
            for (int h = 8; h <= 15; ++h) {
                for (int min = 0; min < 60; min += 15) {
                    bloques[indice].hora = h;
                    bloques[indice].minuto = min;
                    string strH = (h < 10 ? "0" : "") + to_string(h);
                    string strM = (min == 0 ? "00" : to_string(min));
                    bloques[indice].textoVisual = strH + ":" + strM;
                    bool yaOcupado = verificarCitaOcupada(d, m, a, h, min);
                    bool yaPasoHoy = false;
                    if (a == anioActual && m == mesActual && d == diaActual) {
                        if (h < horaActual || (h == horaActual && min <= minActual)) {
                            yaPasoHoy = true;
                        }
                    }
                    if (yaOcupado || yaPasoHoy) {
                        bloques[indice].disponible = false;
                    } else {
                        bloques[indice].disponible = true;
                    }
                    indice++;
                }
            }
            int seleccionActiva = 0;
            bool interactuando = true;
            while (interactuando) {
                system("cls");
                cout << "==========================================================" << endl;
                cout << "         SELECCIONA EL HORARIO DISPONIBLE DE TU CITA     " << endl;
                cout << "     Fecha elegida: " << d << "/" << m << "/" << a << " | (Use Flechas y ENTER)   " << endl;
                cout << "==========================================================" << endl;
                for (int i = 0; i < TOTAL_INTERVALOS; ++i) {
                    string estadoStr = bloques[i].disponible ? "[ DISPONIBLE ]" : "[   OCUPADO   ]";
                    if (seleccionActiva == i) {
                        if (bloques[i].disponible) {
                            cambiarColor(0, 11);
                        } else {
                            cambiarColor(12, 15);
                        }
                        cout << " -> " << bloques[i].textoVisual << " h   " << estadoStr << " \n";
                        cambiarColor(7, 0);
                    } else {
                        if (bloques[i].disponible) {
                            cambiarColor(10, 0);
                            cout << "    " << bloques[i].textoVisual << " h   " << estadoStr << " \n";
                        } else {
                            cambiarColor(8, 0);
                            cout << "    " << bloques[i].textoVisual << " h   " << estadoStr << " \n";
                        }
                        cambiarColor(7, 0);
                    }
                }
                cout << "==========================================================" << endl;
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) {
                    tecla = _getch();
                    if (tecla == TECLA_ARRIBA) {
                        seleccionActiva--;
                        if (seleccionActiva < 0) seleccionActiva = TOTAL_INTERVALOS - 1;
                    } else if (tecla == TECLA_ABAJO) {
                        seleccionActiva++;
                        if (seleccionActiva >= TOTAL_INTERVALOS) seleccionActiva = 0;
                    }
                } else if (tecla == TECLA_ENTER) {
                    if (bloques[seleccionActiva].disponible) {
                        horaSeleccionada = bloques[seleccionActiva].hora;
                        minutoSeleccionado = bloques[seleccionActiva].minuto;
                        mostrarCursorTexto(true);
                        return true;
                    } else {
                        cout << "\a";
                        cout << "\n[RECHAZADO] Este horario ya no esta disponible o ya transcurrio. Elija otro." << endl;
                        Sleep(1200);
                    }
                }
            }
            mostrarCursorTexto(true);
            return false;
        }

    public:
        GestorMenu(ListaDCE& sistema) : sistemaTurnos(sistema) {}

        void registrarFirmaSeguridad(const string& rutaDatos, const string& rutaFirma) {
            string nuevoHash = calcularHashArchivo(rutaDatos);
            ofstream archivoFirma(rutaFirma, ios::out | ios::trunc);
            if (archivoFirma.is_open()) {
                archivoFirma << nuevoHash;
                archivoFirma.close();
            }
        }

        void pintarMenuPrincipal(int seleccionActiva) {
            HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD coord = {0, 0};
            SetConsoleCursorPosition(hOutput, coord);
            cout << "============================================" << endl;
            cout << "     SISTEMA DE MATRICULACION VEHICULAR     " << endl;
            cout << "     (Use Flechas ARRIBA/ABAJO y ENTER)     " << endl;
            cout << "============================================" << endl;
            const char* menu[] = {
                "Agendar Nuevo Turno (Propietario y Vehiculo)",
                "Mostrar Turnos Registrados",
                "Modificar Turno por ID",
                "Cancelar/Eliminar Turno por ID",
                "Ordenar y Filtrar Reportes (Templates)",
                "Lista de placas cifradas",
                "Salir del Sistema"
            };
            for (int i = 0; i < 7; ++i) {
                if ((seleccionActiva - 1) == i) {
                    cambiarColor(0, 11);
                    cout << " -> " << *(menu + i) << "   " << endl;
                    cambiarColor(7, 0);
                } else {
                    cout << "    " << *(menu + i) << "   " << endl;
                }
            }
            cout << "============================================" << endl;
        }

        void iniciarMenuInteractivo() {
            mostrarCursorTexto(false);
            int opcionActual = 1; 
            bool ejecutando = true;
            system("cls");
            pintarMenuPrincipal(opcionActual);
            while (ejecutando) {
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) {
                    tecla = _getch();
                    if (tecla == TECLA_ARRIBA) {
                        opcionActual--;
                        if (opcionActual < 1) opcionActual = 7;
                        pintarMenuPrincipal(opcionActual);
                    } else if (tecla == TECLA_ABAJO) {
                        opcionActual++;
                        if (opcionActual > 7) opcionActual = 1;
                        pintarMenuPrincipal(opcionActual);
                    }
                } else if (tecla == TECLA_ENTER) {
                    mostrarCursorTexto(true);
                    system("cls");
                    switch (opcionActual) {
                        case 1: ejecutarAgendamiento(); break;
                        case 2:
                            cout << "\n=== TURNOS REGISTRADOS ===" << endl;
                            sistemaTurnos.mostrarLista();
                            system("pause");
                            break;
                        case 3:
                            ejecutarModificacionPorID();
                            break;
                        case 4: {
                            cout << "\n=== ELIMINAR / CANCELAR TURNO POR ID ===" << endl;
                            int idBuscar = leerEntero("Ingrese el ID del turno a eliminar: ");
                            sistemaTurnos.eliminarPorID(idBuscar);
                            system("pause");
                            break;
                        }
                        case 5: ejecutarOrdenamientoTemplates(); break;
                        case 6:
                            cout << "\n=== LISTA DE PLACAS CIFRADAS ===" << endl;
                            sistemaTurnos.cifrarPlacas();
                            system("pause");
                            break;
                        case 7: ejecutando = false; break;
                    }
                    if (ejecutando) {
                        mostrarCursorTexto(false);
                        system("cls");
                        pintarMenuPrincipal(opcionActual);
                    }
                }
            }
            mostrarCursorTexto(true);
            system("cls");
            cout << "============================================" << endl;
            cout << "        Sistema Closed Correctamente.      " << endl;
            cout << "============================================" << endl;
        }

        void ejecutarOrdenamientoTemplates() {
            mostrarCursorTexto(false);
            int opcionSub = 1;
            bool subMenuActivo = true;
            auto pintarSubMenu = [&](int seleccion) {
                HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
                COORD coord = {0, 0};
                SetConsoleCursorPosition(hOutput, coord);
                cout << "==========================================================" << endl;
                cout << "   === ORDENAR REPORTES DE TURNOS (BUBBLE SORT) ===" << endl;
                cout << "          (Use Flechas ARRIBA/ABAJO y ENTER)              " << endl;
                cout << "==========================================================" << endl;
                const char* opcionesSub[] = {
                    "Ordenar por Nombre del Propietario (A-Z)",
                    "Ordenar por Apellido del Propietario (A-Z)",
                    "Ordenar por Placa del Vehiculo",
                    "Ordenar por Fecha del Turno",
                    "[CONSULTA] Realizar Busqueda Binaria Rapida",
                    "[CONSULTA] Realizar Busqueda por Funcion Hash",
                    "Volver al Menu Principal"
                };
                for (int i = 0; i < 7; ++i) {
                    if ((seleccion - 1) == i) {
                        cambiarColor(0, 11);
                        cout << " -> " << *(opcionesSub + i) << "   " << endl;
                        cambiarColor(7, 0);
                    } else {
                        cout << "    " << *(opcionesSub + i) << "   " << endl;
                    }
                }
                cout << "==========================================================" << endl;
            };
            system("cls");
            pintarSubMenu(opcionSub);
            while (subMenuActivo) {
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) {
                    tecla = _getch();
                    if (tecla == TECLA_ARRIBA) {
                        opcionSub--;
                        if (opcionSub < 1) opcionSub = 7;
                        pintarSubMenu(opcionSub);
                    } else if (tecla == TECLA_ABAJO) {
                        opcionSub++;
                        if (opcionSub > 7) opcionSub = 1;
                        pintarSubMenu(opcionSub);
                    }
                } else if (tecla == TECLA_ENTER) {
                    mostrarCursorTexto(true);
                    system("cls");
                    if (opcionSub == 7) {
                        subMenuActivo = false;
                        break;
                    }
                    if (opcionSub == 6) {
                        ejecutarBusquedaHashTemplates();
                    } else if (opcionSub == 5) {
                        ejecutarBusquedaBinariaTemplates();
                    } else if (opcionSub == 4) {
                        cout << "\n[+] Generando reporte cronologico y mostrando en pantalla..." << endl;
                        if (sistemaTurnos.getCabeza() == nullptr) {
                            cout << "[INFO] No hay turnos registrados para generar el reporte." << endl;
                            system("pause");
                        } else {
                            int totalTurnos = 0;
                            Nodo* actual = sistemaTurnos.getCabeza();
                            do {
                                if (actual->getTurno() != nullptr) totalTurnos++;
                                actual = actual->getSiguiente();
                            } while (actual != sistemaTurnos.getCabeza());
                            Turno** turnosTemporales = new Turno*[totalTurnos];
                            int indice = 0;
                            actual = sistemaTurnos.getCabeza();
                            do {
                                if (actual->getTurno() != nullptr) {
                                    *(turnosTemporales + indice) = actual->getTurno();
                                    indice++;
                                }
                                actual = actual->getSiguiente();
                            } while (actual != sistemaTurnos.getCabeza());
                            for (int i = 0; i < totalTurnos - 1; i++) {
                                for (int j = 0; j < totalTurnos - i - 1; j++) {
                                    Fecha f1 = (*(turnosTemporales + j))->getFecha();
                                    Fecha f2 = (*(turnosTemporales + j + 1))->getFecha();
                                    bool intercambiar = false;
                                    if (f1.getYear() != f2.getYear()) intercambiar = (f1.getYear() > f2.getYear());
                                    else if (f1.getMes() != f2.getMes()) intercambiar = (f1.getMes() > f2.getMes());
                                    else if (f1.getDia() != f2.getDia()) intercambiar = (f1.getDia() > f2.getDia());
                                    else if (f1.getHora() != f2.getHora()) intercambiar = (f1.getHora() > f2.getHora());
                                    else intercambiar = (f1.getMinutos() > f2.getMinutos());
                                    if (intercambiar) {
                                        Turno* aux = *(turnosTemporales + j);
                                        *(turnosTemporales + j) = *(turnosTemporales + j + 1);
                                        *(turnosTemporales + j + 1) = aux;
                                    }
                                }
                            }
                            ofstream archivoReporte("reporte_cronologico_placas.txt", ios::out);
                            cout << "\n========================================================" << endl;
                            cout << "       LISTADO DE PLACAS ORDENADO CRONOLOGICAMENTE      " << endl;
                            cout << "========================================================" << endl;
                            for (int i = 0; i < totalTurnos; i++) {
                                Fecha f = (*(turnosTemporales + i))->getFecha();
                                string placa = (*(turnosTemporales + i))->getVehiculo().getPlaca();
                                string minStr = (f.getMinutos() < 10 ? "0" : "") + to_string(f.getMinutos());
                                cout << "  [+] " << f.getDia() << "/" << f.getMes() << "/" << f.getYear() << " - " << f.getHora() << ":" << minStr << "h - " << placa << endl;
                                if (archivoReporte.is_open()) {
                                    archivoReporte << f.getDia() << "/" << f.getMes() << "/" << f.getYear() << " - " << f.getHora() << ":" << minStr << "h - " << placa << "\n";
                                }
                            }
                            cout << "========================================================" << endl;
                            if (archivoReporte.is_open()) archivoReporte.close();
                            cout << "\n[EXITO] Reporte guardado en 'reporte_cronologico_placas.txt'." << endl;
                            delete[] turnosTemporales;
                            system("pause");
                        }
                    } else {
                        typedef string(*ExtractorAtributo)(Turno*);
                        ExtractorAtributo c1 = [](Turno* t) {return t->getUsuario().getNombre();};
                        ExtractorAtributo c2 = [](Turno* t) {return t->getUsuario().getApellido();};
                        ExtractorAtributo c3 = [](Turno* t) {return t->getVehiculo().getPlaca();};
                        ExtractorAtributo tablaCriterios[] = {c1, c2, c3};
                        const char* mensajesSub[] = {
                            "\n[+] Ordenando por Nombre del Propietario...",
                            "\n[+] Ordenando por Apellido del Propietario...",
                            "\n[+] Ordenando por Placa del Vehiculo..."
                        };
                        cout << *(mensajesSub + (opcionSub - 1)) << endl;
                        sistemaTurnos.ordenamiento(*(tablaCriterios + (opcionSub - 1)));
                        sistemaTurnos.mostrarLista();
                        system("pause");
                    }
                    mostrarCursorTexto(false);
                    system("cls");
                    pintarSubMenu(opcionSub);
                }
            }
        }

        void ejecutarBusquedaBinariaTemplates(){
            system("cls");
            if(sistemaTurnos.getCabeza()==nullptr){
                cout<<"[INFO] El sistema esta vacio. No se puede realizar busquedas binarias.\n"<<endl;
                system("pause");
                return;
            }
            mostrarCursorTexto(false);
            int seleccionSub=1;
            bool subMenuActivo=true;
            auto pintarMenuBusqueda=[&](int seleccion){
                HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
                COORD coord={0,0};
                SetConsoleCursorPosition(hOutput,coord);
                cout<<"=========================================================="<<endl;
                cout<<"       EJERCICIOS COMPLEMENTARIOS: BUSQUEDA BINARIA       "<<endl;
                cout<<"          (Use Flechas ARRIBA/ABAJO y ENTER)              "<<endl;
                cout<<"=========================================================="<<endl;
                const char* opcionesSub[]={
                    "Buscar Turno por Nombre exacto del Propietario",
                    "Buscar Turno por Apellido exacto del Propietario",
                    "Buscar Turno por Cedula del Propietario",
                    "Buscar Turno por Placa del Vehiculo",
                    "Volver al Menu Anterior"
                };
                for(int i=0;i<5;++i){
                    if((seleccion-1)==i){
                        cambiarColor(0,11);
                        cout<<" -> "<<*(opcionesSub+i)<<"   "<<endl;
                        cambiarColor(7,0);
                    }else{
                        cout<<"    "<<*(opcionesSub+i)<<"   "<<endl;
                    }
                }
                cout<<"=========================================================="<<endl;
            };
            pintarMenuBusqueda(seleccionSub);
            while(subMenuActivo){
                int tecla=_getch();
                if(tecla==0||tecla==224){
                    tecla=_getch();
                    if(tecla==TECLA_ARRIBA){
                        seleccionSub--;
                        if(seleccionSub<1)seleccionSub=5;
                        pintarMenuBusqueda(seleccionSub);
                    }else if(tecla==TECLA_ABAJO){
                        seleccionSub++;
                        if(seleccionSub>5)seleccionSub=1;
                        pintarMenuBusqueda(seleccionSub);
                    }
                }else if(tecla==TECLA_ENTER){
                    mostrarCursorTexto(true);
                    system("cls");
                    if(seleccionSub==5){
                        subMenuActivo=false;
                        break;
                    }
                    string valorBuscar="";
                    if(seleccionSub==1){
                        valorBuscar=leerSoloLetras("Ingrese el Nombre exacto a buscar: ",30);
                    }else if(seleccionSub==2){
                        valorBuscar=leerSoloLetras("Ingrese el Apellido exacto a buscar: ",30);
                    }else if(seleccionSub==3){
                        valorBuscar=leerSoloNumeros("Ingrese la Cedula exacta a buscar: ",10);
                    }else if(seleccionSub==4){
                        cout<<"Ingrese la Placa exacta a buscar (Ej: ABC1234): ";
                        getline(cin,valorBuscar);
                        for(char& c:valorBuscar)c=toupper(c);
                    }
                    typedef string(*ExtractorBusqueda)(Turno*);
                    ExtractorBusqueda b1=[](Turno* t){return t->getUsuario().getNombre();};
                    ExtractorBusqueda b2=[](Turno* t){return t->getUsuario().getApellido();};
                    ExtractorBusqueda b3=[](Turno* t){return t->getUsuario().getCI();};
                    ExtractorBusqueda b4=[](Turno* t){return t->getVehiculo().getPlaca();};
                    ExtractorBusqueda tablaBusquedas[]={b1,b2,b3,b4};
                    cout<<"\n[PRE-REQUISITO] Alineando y ordenando la estructura para busqueda binaria..."<<endl;
                    sistemaTurnos.ordenamiento(*(tablaBusquedas+(seleccionSub-1)));
                    cout<<"[PROCESANDO] Ejecutando algoritmo logaritmico O(log N)..."<<endl;
                    Turno* encontrado=sistemaTurnos.busquedaBinaria(*(tablaBusquedas+(seleccionSub-1)),valorBuscar);
                    if(encontrado!=nullptr){
                        cout<<"\n========================================================"<<endl;
                        cout<<"          RESULTADO DE LA BUSQUEDA BINARIA              "<<endl;
                        cout<<"========================================================"<<endl;
                        cout<<"  [+] ID Asignado : "<<encontrado->getIDturno()<<endl;
                        cout<<"  [+] Propietario : "<<encontrado->getUsuario().getNombre()<<" "<<encontrado->getUsuario().getApellido()<<endl;
                        cout<<"  [+] Cedula      : "<<encontrado->getUsuario().getCI()<<endl;
                        cout<<"  [+] Vehiculo    : Placa ["<<encontrado->getVehiculo().getPlaca()<<"] - Color ["<<encontrado->getVehiculo().getColor()<<"]"<<endl;
                        cout<<"  [+] Cita Agendada: "<<encontrado->getFecha().getDia()<<"/"<<encontrado->getFecha().getMes()<<"/"<<encontrado->getFecha().getYear()<<endl;
                        cout<<"========================================================"<<endl;
                    }else{
                        cout<<"\n[ERROR] Busqueda Binaria concluida sin coincidencias. El registro no existe."<<endl;
                    }
                    system("pause");
                    mostrarCursorTexto(false);
                    system("cls");
                    pintarMenuBusqueda(seleccionSub);
                }
            }
        }

        void ejecutarBusquedaHashTemplates(){
            system("cls");
            if(sistemaTurnos.getCabeza()==nullptr){
                cout<<"[INFO] El sistema esta vacio. No se puede realizar busquedas por Hash.\n"<<endl;
                system("pause");
                return;
            }
            mostrarCursorTexto(false);
            int seleccionSub=1;
            bool subMenuActivo=true;
            auto pintarMenuHash=[&](int seleccion){
                HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
                COORD coord={0,0};
                SetConsoleCursorPosition(hOutput,coord);
                cout<<"=========================================================="<<endl;
                cout<<"       EJERCICIOS COMPLEMENTARIOS: BUSQUEDA HASH (O(1))   "<<endl;
                cout<<"          (Use Flechas ARRIBA/ABAJO y ENTER)              "<<endl;
                cout<<"=========================================================="<<endl;
                const char* opcionesSub[]={
                    "Buscar Turno por Nombre con Hash DJB2",
                    "Buscar Turno por Apellido con Hash DJB2",
                    "Buscar Turno por Cedula con Hash DJB2",
                    "Buscar Turno por Placa con Hash DJB2",
                    "Volver al Menu Anterior"
                };
                for(int i=0;i<5;++i){
                    if((seleccion-1)==i){
                        cambiarColor(0,11);
                        cout<<" -> "<<*(opcionesSub+i)<<"   "<<endl;
                        cambiarColor(7,0);
                    }else{
                        cout<<"    "<<*(opcionesSub+i)<<"   "<<endl;
                    }
                }
                cout<<"=========================================================="<<endl;
            };
            pintarMenuHash(seleccionSub);
            while(subMenuActivo){
                int tecla=_getch();
                if(tecla==0||tecla==224){
                    tecla=_getch();
                    if(tecla==TECLA_ARRIBA){
                        seleccionSub--;
                        if(seleccionSub<1)seleccionSub=5;
                        pintarMenuHash(seleccionSub);
                    }else if(tecla==TECLA_ABAJO){
                        seleccionSub++;
                        if(seleccionSub>5)seleccionSub=1;
                        pintarMenuHash(seleccionSub);
                    }
                }else if(tecla==TECLA_ENTER){
                    mostrarCursorTexto(true);
                    system("cls");
                    if(seleccionSub==5){
                        subMenuActivo=false;
                        break;
                    }
                    string valorBuscar="";
                    if(seleccionSub==1){
                        valorBuscar=leerSoloLetras("Ingrese el Nombre exacto a buscar: ",30);
                    }else if(seleccionSub==2){
                        valorBuscar=leerSoloLetras("Ingrese el Apellido exacto a buscar: ",30);
                    }else if(seleccionSub==3){
                        valorBuscar=leerSoloNumeros("Ingrese la Cedula exacta a buscar: ",10);
                    }else if(seleccionSub==4){
                        cout<<"Ingrese la Placa exacta a buscar (Ej: ABC1234): ";
                        getline(cin,valorBuscar);
                        for(char& c:valorBuscar)c=toupper(c);
                    }
                    typedef string(*ExtractorHash)(Turno*);
                    ExtractorHash b1=[](Turno* t){return t->getUsuario().getNombre();};
                    ExtractorHash b2=[](Turno* t){return t->getUsuario().getApellido();};
                    ExtractorHash b3=[](Turno* t){return t->getUsuario().getCI();};
                    ExtractorHash b4=[](Turno* t){return t->getVehiculo().getPlaca();};
                    ExtractorHash tablaBusquedas[]={b1,b2,b3,b4};
                    
                    cout<<"\n[PROCESANDO] Calculando cubeta aritmetica mediante funcion Hash..."<<endl;
                    Turno* encontrado=sistemaTurnos.busquedaHash(*(tablaBusquedas+(seleccionSub-1)),valorBuscar);
                    if(encontrado!=nullptr){
                        cout<<"\n========================================================"<<endl;
                        cout<<"            RESULTADO DE LA BUSQUEDA HASH               "<<endl;
                        cout<<"========================================================"<<endl;
                        cout<<"  [+] ID Asignado : "<<encontrado->getIDturno() << endl;
                        cout<<"  [+] Propietario : "<<encontrado->getUsuario().getNombre()<<" "<<encontrado->getUsuario().getApellido()<<endl;
                        cout<<"  [+] Cedula      : "<<encontrado->getUsuario().getCI()<<endl;
                        cout<<"  [+] Vehiculo    : Placa ["<<encontrado->getVehiculo().getPlaca()<<"] - Color ["<<encontrado->getVehiculo().getColor()<<"]"<<endl;
                        cout<<"  [+] Cita Agendada: "<<encontrado->getFecha().getDia()<<"/"<<encontrado->getFecha().getMes()<<"/"<<encontrado->getFecha().getYear()<<endl;
                        cout<<"========================================================"<<endl;
                    }else{
                        cout<<"\n[ERROR] Tabla de dispersion recorrida. El registro buscado no existe."<<endl;
                    }
                    system("pause");
                    mostrarCursorTexto(false);
                    system("cls");
                    pintarMenuHash(seleccionSub);
                }
            }
        }

        void ejecutarAgendamiento(){
            int d=0,m=0,a=0,hh=0,mm=0,yearAuto=0;
            string cedula="",nombre="",apellido="",telefono="",correo="";
            string placa="",color="";
            time_t tiempoActual=time(nullptr);
            tm* fechaSistema=localtime(&tiempoActual);
            int anioActual=fechaSistema->tm_year+1900;
            int mesActual=fechaSistema->tm_mon+1;
            int diaActual=fechaSistema->tm_mday;
            int horaActual=fechaSistema->tm_hour;
            int minActual=fechaSistema->tm_min;
            cin.clear();
            cout<<"\n=== AGENDAR NUEVO TURNO ==="<<endl;
            while(true){
                a=leerEnteroBloqueado("Ingrese el anio (Actual o Siguiente): ",4);
                if(a>=anioActual&&a<=(anioActual+1))break;
                cout<<"[RECHAZADO] El anio debe ser el actual ("<<anioActual<<") o el siguiente ("<<anioActual+1<<"). Intente de nuevo.\n"<<endl;
            }
            while(true){
                while(true){
                    m=leerEnteroBloqueado("Ingrese el mes (1-12): ",2);
                    if(m<1||m>12){
                        cout<<"[RECHAZADO] El mes debe estar estrictamente entre 1 and 12.\n"<<endl;
                        continue;
                    }
                    if(a==anioActual&&m<mesActual){
                        cout<<"[RECHAZADO] El mes ingresado ya transcurrio en el presente anio ("<<mesActual<<").\n"<<endl;
                        continue;
                    }
                    break;
                }
                bool regresarAMes=false;
                while(true){
                    cout<<"--- (O si desea cambiar de mes, ingrese 0) ---"<<endl;
                    d=leerEnteroBloqueado("Ingrese el dia: ",2);
                    if(d==0){
                        regresarAMes=true;
                        cout<<"\n[INFO] Regresando a la seleccion de mes...\n"<<endl;
                        break;
                    }
                    if(d<1||d>31){
                        cout<<"[RECHAZADO] El dia debe estar entre 1 and 31.\n"<<endl;
                        continue;
                    }
                    if(a==anioActual&&m==mesActual&&d<diaActual){
                        cout<<"[RECHAZADO] El dia ingresado ya paso. Hoy es dia "<<diaActual<<".\n"<<endl;
                        continue;
                    }
                    Fecha fechaAuxiliar(d,m,a,0,0);
                    string resultadoFeriado=fechaAuxiliar.validarFechaYObtenerFeriado(d,m,a);
                    if(resultadoFeriado!="DISPONIBLE"){
                        cout<<"[RECHAZADO]: "<<resultadoFeriado<<" Intente con otra fecha u otra casilla.\n"<<endl;
                        continue;
                    }
                    break;
                }
                if(!regresarAMes)break;
            }
            cout<<"\n[PROCESANDO] Cargando la disponibilidad de horas desde el sistema..."<<endl;
            Sleep(500);
            if(!seleccionarBloqueHorario(d,m,a,anioActual,mesActual,diaActual,horaActual,minActual,hh,mm)){
                cout<<"[INFO] Agendamiento cancelado por falta de horarios."<<endl;
                system("pause");
                return;
            }
            Fecha fechaTentativa(d,m,a,hh,mm);
            cout<<"\n--- DATOS DEL PROPIETARIO ---"<<endl;
            do{
                cedula=leerSoloNumeros("Ingrese la cedula del dueno (10 digitos): ",10);
                if(!validadorCedula.validar(cedula.c_str())){
                    cout<<"[ERROR] La cedula es invalida (Fallo el algoritmo de verificacion ecuatoriano).\n"<<endl;
                }
            }while(!validadorCedula.validar(cedula.c_str()));
            do{
                nombre=leerSoloLetras("Ingrese el nombre (Solo letras): ",30);
                if(!validador.validarNombre(nombre)){
                    cout<<"[ERROR] Nombre invalido. Intente de nuevo.\n"<<endl;
                }
            }while(!validador.validarNombre(nombre));
            do{
                apellido=leerSoloLetras("Ingrese el apellido (Solo letras): ",30);
                if(!validador.validarNombre(apellido)){
                    cout<<"[ERROR] Apellido invalido. Intente de nuevo.\n"<<endl;
                }
            }while(!validador.validarNombre(apellido));
            do{
                telefono=leerSoloNumeros("Ingrese su numero de telefono (10 digitos): ",10);
                if(!validador.validarTelefono(telefono)){
                    cout<<"[ERROR] Telefono invalido. Deben ser exactamente 10 digitos numericos.\n"<<endl;
                }
            }while(!validador.validarTelefono(telefono));
            do{
                cout<<"Ingrese su correo electronico (ejemplo@dominio.com): ";
                getline(cin,correo);
                if(!validador.validarCorreo(correo)){
                    cout<<"[ERROR] Formato de correo electronico incorrecto.\n"<<endl;
                }
            }while(!validador.validarCorreo(correo));
            Persona propietario(nombre, apellido, cedula, telefono, correo);
            cout << "\n==== REGISTRO DE VEHICULO ====" << endl;
            while (true) {
                yearAuto = leerEnteroBloqueado("Ingrese el anio del vehiculo (1950 - posterior): ", 4);
                if (yearAuto >= 1950 && yearAuto <= (anioActual + 1)) break;
                cout << "[ERROR] Anio fuera de rango permitido (1950 - " << (anioActual + 1) << ").\n" << endl;
            }
            while (true) {
                cout << "Ingrese la placa (Ej: ABC1234): "; 
                getline(cin, placa);
                for (char& c : placa) c = toupper(c);
                if (!validador.validarPlaca(placa)) {
                    cout << "[ERROR] Formato de placa invalido (Deben ser 3 letras mayusculas seguidas de 3 o 4 numeros).\n" << endl;
                    continue;
                }
                bool yaMatriculadoEsteAnio = false;
                if (sistemaTurnos.getCabeza() != nullptr) {
                    Nodo* temp = sistemaTurnos.getCabeza();
                    do {
                        if (temp->getTurno() != nullptr && temp->getTurno()->getVehiculo().getPlaca() == placa) {
                            if (temp->getTurno()->getFecha().getYear() == a) {
                                yaMatriculadoEsteAnio = true;
                                break;
                            }
                        }
                        temp = temp->getSiguiente();
                    } while (temp != sistemaTurnos.getCabeza());
                }
                if (yaMatriculadoEsteAnio) {
                    cout << "[RECHAZADO] Error: El vehiculo con placa " << placa << " ya registra un estado MATRICULADO para el anio " << a << ".\n" << endl;
                    continue; 
                }
                if (a > anioActual) {
                    cout << "[RECHAZADO] Error: No se puede agendar turnos para el periodo fiscal " << a << " de forma anticipada." << endl;
                    cout << "El sistema habilitara el agendamiento a partir del 1 de enero de " << a << ".\n" << endl;
                    continue;
                }
                break; 
            }
            color = leerSoloLetras("Ingrese el color del vehiculo: ", 20);
            Vehiculo carro(color, placa, yearAuto);
            int idTurno = sistemaTurnos.generarSiguienteID();
            Turno* nuevoTurno = new Turno(propietario, carro, fechaTentativa, idTurno, true);
            sistemaTurnos.insertar(nuevoTurno);
            registrarFirmaSeguridad("turnos.txt", "checksum.ptr");
            cout << "\n[EXITO] Registro completado de forma segura. ID Asignado: " << idTurno << endl;
            system("pause");
        }

        void ejecutarModificacionPorID(){
            system("cls");
            if(sistemaTurnos.getCabeza()==nullptr){
                cout<<"[INFO] El sistema esta vacio. No hay turnos para modificar.\n"<<endl;
                system("pause");
                return;
            }
            cout<<"=========================================================="<<endl;
            cout<<"               MODIFICAR TURNO POR ID                    "<<endl;
            cout<<"=========================================================="<<endl;
            int idBuscar=leerEnteroBloqueado("Ingrese el ID del turno a modificar: ",6);
            Nodo* actual=sistemaTurnos.getCabeza();
            Turno* encontrado=nullptr;
            do{
                if(actual->getTurno()!=nullptr&&actual->getTurno()->getIDturno()==idBuscar){
                    encontrado=actual->getTurno();
                    break;
                }
                actual=actual->getSiguiente();
            }while(actual!=sistemaTurnos.getCabeza());
            if(encontrado==nullptr){
                cout<<"\n[ERROR] No se encontro ningun turno con el ID especificado."<<endl;
                system("pause");
                return;
            }
            cout<<"\n[TURNO ENCONTRADO] Datos actuales del registro:"<<endl;
            cout<<" -> Propietario: "<<encontrado->getUsuario().getNombre()<<" "<<encontrado->getUsuario().getApellido()<<endl;
            cout<<" -> Vehiculo   : Placa ["<<encontrado->getVehiculo().getPlaca()<<"]"<<endl;
            cout<<" -> Fecha/Hora : "<<encontrado->getFecha().getDia()<<"/"<<encontrado->getFecha().getMes()<<"/"<<encontrado->getFecha().getYear()<<endl;
            cout<<"=========================================================="<<endl;
            int d=0,m=0,a=0,hh=0,mm=0,yearAuto=0;
            string cedula="",nombre="",apellido="",telefono="",correo="";
            string placa="",color="";
            time_t tiempoActual=time(nullptr);
            tm* fechaSistema=localtime(&tiempoActual);
            int anioActual=fechaSistema->tm_year+1900;
            int mesActual=fechaSistema->tm_mon+1;
            int diaActual=fechaSistema->tm_mday;
            int horaActual=fechaSistema->tm_hour;
            int minActual=fechaSistema->tm_min;
            cout<<"\n--- NUEVA FECHA DE AGENDAMIENTO ---"<<endl;
            while(true){
                a=leerEnteroBloqueado("Ingrese el nuevo anio (Actual o Siguiente): ",4);
                if(a>=anioActual&&a<=(anioActual+1))break;
                cout<<"[RECHAZADO] El anio debe ser el actual ("<<anioActual<<") o el siguiente ("<<anioActual+1<<").\n"<<endl;
            }
            while(true){
                while(true){
                    m=leerEnteroBloqueado("Ingrese el nuevo mes (1-12): ",2);
                    if(m<1||m>12){
                        cout<<"[RECHAZADO] El mes debe estar estrictamente entre 1 y 12.\n"<<endl;
                        continue;
                    }
                    if(a==anioActual&&m<mesActual){
                        cout<<"[RECHAZADO] El mes ingresado ya transcurrio.\n"<<endl;
                        continue;
                    }
                    break;
                }
                bool regresarAMes=false;
                while(true){
                    cout<<"--- (O si desea cambiar de mes, ingrese 0) ---"<<endl;
                    d=leerEnteroBloqueado("Ingrese el nuevo dia: ",2);
                    if(d==0){
                        regresarAMes=true;
                        cout<<"\n[INFO] Regresando a la seleccion de mes...\n"<<endl;
                        break;
                    }
                    if(d<1||d>31){
                        cout<<"[RECHAZADO] El dia debe estar entre 1 y 31.\n"<<endl;
                        continue;
                    }
                    if(a==anioActual&&m==mesActual&&d<diaActual){
                        cout<<"[RECHAZADO] El dia ingresado ya paso. Hoy es dia "<<diaActual<<".\n"<<endl;
                        continue;
                    }
                    Fecha fechaAuxiliar(d,m,a,0,0);
                    string resultadoFeriado=fechaAuxiliar.validarFechaYObtenerFeriado(d,m,a);
                    if(resultadoFeriado!="DISPONIBLE"){
                        cout<<"[RECHAZADO]: "<<resultadoFeriado<<" Intente con otra fecha.\n"<<endl;
                        continue;
                    }
                    break;
                }
                if(!regresarAMes)break;
            }
            cout<<"\n[PROCESANDO] Cargando la disponibilidad de horas para la nueva fecha..."<<endl;
            Sleep(500);
            if(!seleccionarBloqueHorario(d,m,a,anioActual,mesActual,diaActual,horaActual,minActual,hh,mm)){
                cout<<"[INFO] Modificacion cancelada por falta de horarios."<<endl;
                system("pause");
                return;
            }
            Fecha nuevaFecha(d,m,a,hh,mm);
            cout<<"\n--- NUEVOS DATOS DEL PROPIETARIO ---"<<endl;
            do{
                cedula=leerSoloNumeros("Ingrese la cedula del dueno (10 digitos): ",10);
                if(!validadorCedula.validar(cedula.c_str())){
                    cout<<"[ERROR] La cedula es invalida (Algoritmo de verificacion ecuatoriano).\n"<<endl;
                }
            }while(!validadorCedula.validar(cedula.c_str()));
            nombre=leerSoloLetras("Ingrese el nombre (Solo letras): ",30);
            apellido=leerSoloLetras("Ingrese el apellido (Solo letras): ",30);
            do{
                telefono=leerSoloNumeros("Ingrese su numero de telefono (10 digitos): ",10);
                if(!validador.validarTelefono(telefono)){
                    cout<<"[ERROR] Telefono invalido. Deben ser exactamente 10 digitos numericos.\n"<<endl;
                }
            }while(!validador.validarTelefono(telefono));
            do{
                cout<<"Ingrese su correo electronico (ejemplo@dominio.com): ";
                getline(cin,correo);
                if(!validador.validarCorreo(correo)){
                    cout<<"[ERROR] Formato de correo electronico incorrecto.\n"<<endl;
                }
            }while(!validador.validarCorreo(correo));
            Persona nuevoPropietario(nombre,apellido,cedula,telefono,correo);
            cout<<"\n==== NUEVO REGISTRO DE VEHICULO ===="<<endl;
            while(true){
                yearAuto=leerEnteroBloqueado("Ingrese el anio del vehiculo (1950 - posterior): ",4);
                if(yearAuto>=1950&&yearAuto<=(anioActual+1))break;
                cout<<"[ERROR] Anio fuera de rango permitido (1950 - "<<(anioActual+1)<<").\n"<<endl;
            }
            do{
                cout<<"Ingrese la placa (Ej: ABC1234): ";
                getline(cin,placa);
                for(char& c:placa)c=toupper(c);
                if(!validador.validarPlaca(placa)){
                    cout<<"[ERROR] Formato de placa invalido.\n"<<endl;
                }
            }while(!validador.validarPlaca(placa));
            color=leerSoloLetras("Ingrese el color del vehiculo: ",20);
            Vehiculo nuevoCarro(color,placa,yearAuto);
            encontrado->setFecha(nuevaFecha);
            encontrado->setUsuario(nuevoPropietario);
            encontrado->setVehiculo(nuevoCarro);
            registrarFirmaSeguridad("turnos.txt", "checksum.ptr");
            cout<<"\n[EXITO] Turno ID "<<idBuscar<<" modificado correctamente con todas las politicas de seguridad."<<endl;
            system("pause");
        }

        bool verificarIntegridadDatos(const string& rutaDatos, const string& rutaFirma) {
            ifstream archivoDatos(rutaDatos);
            if (!archivoDatos.is_open()) return true;
            archivoDatos.close();
            string hashActual = calcularHashArchivo(rutaDatos);
            string hashGuardado = "";
            ifstream archivoFirma(rutaFirma);
            if (archivoFirma.is_open()) {
                archivoFirma >> hashGuardado;
                archivoFirma.close();
            }
            if (hashGuardado.empty()) {
                registrarFirmaSeguridad(rutaDatos, rutaFirma);
                return true;
            }
            if (hashActual != hashGuardado) {
                cambiarColor(12, 0);
                cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                cout << " [ALERTA DE SEGURIDAD] CRITICAL ERROR: INTEGRIDAD COMPROMETIDA" << endl;
                cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                cout << " Se detecto que el archivo de base de datos '" << rutaDatos << "'" << endl;
                cout << " fue modificado o alterado de forma externa fuera del sistema." << endl;
                cout << " Firma de confianza esperada: " << hashGuardado << endl;
                cout << " Firma corrupta calculada   : " << hashActual << endl;
                cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << endl;
                cambiarColor(7, 0);
                system("pause");
                return false;
            }
            return true;
        }
};

#endif