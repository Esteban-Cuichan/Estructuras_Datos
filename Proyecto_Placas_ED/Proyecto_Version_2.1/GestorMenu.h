#ifndef GESTORMENU_H
#define GESTORMENU_H

#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include "ListaDCE.h"
#include "Persona.h"
#include "Vehiculo.h"
#include "Fecha.h"
#include "Turno.h"
#include "validarCedula.h"
#include "validarExpresion.h"

// Definición de las teclas especiales en Windows
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_ENTER 13

using namespace std;

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

    public:
        GestorMenu(ListaDCE& sistema) : sistemaTurnos(sistema) {}

        void pintarMenuPrincipal(int seleccionActiva) {
            // Mover el cursor al inicio evita el parpadeo de pantalla (flicker)
            HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD coord = {0, 0};
            SetConsoleCursorPosition(hOutput, coord);

            cout << "============================================" << endl;
            cout << "     SISTEMA DE MATRICULACION VEHICULAR     " << endl;
            cout << "     (Use Flechas ARRIBA/ABAJO y ENTER)     " << endl;
            cout << "============================================" << endl;
            
            const char* menu[] = {
                "1. Agendar Nuevo Turno (Propietario y Vehiculo)",
                "2. Mostrar Turnos Registrados",
                "3. Modificar Turno por ID",
                "4. Cancelar/Eliminar Turno por ID",
                "5. Ordenar y Filtrar Reportes (Templates)",
                "6. Lista de placas cifradas",
                "7. Salir del Sistema"
            };

            for (int i = 0; i < 7; ++i) {
                if ((seleccionActiva - 1) == i) {
                    cambiarColor(0, 11); // Fondo Aqua, letras negras
                    cout << " -> " << *(menu + i) << "   " << endl;
                    cambiarColor(7, 0);  // Restaurar colores por defecto
                } else {
                    cout << "    " << *(menu + i) << "   " << endl;
                }
            }
            cout << "============================================" << endl;
        }

        // CONTROLADOR DEL MENÚ PRINCIPAL MEDIANTE TECLADO
        void iniciarMenuInteractivo() {
            mostrarCursorTexto(false); // Ocultar el cursor parpadeante
            int opcionActual = 1; 
            bool ejecutando = true;

            system("cls"); // Limpieza inicial fija
            pintarMenuPrincipal(opcionActual);

            while (ejecutando) {
                // _getch() lee la tecla pulsada inmediatamente sin esperar un Enter
                int tecla = _getch();

                // Las flechas del teclado envían dos caracteres, el primero suele ser 0 o 224
                if (tecla == 0 || tecla == 224) {
                    tecla = _getch(); // Capturar el código real de la flecha
                    
                    if (tecla == TECLA_ARRIBA) {
                        opcionActual--;
                        if (opcionActual < 1) opcionActual = 7; // Bucle hacia abajo
                        pintarMenuPrincipal(opcionActual);
                    } 
                    else if (tecla == TECLA_ABAJO) {
                        opcionActual++;
                        if (opcionActual > 7) opcionActual = 1; // Bucle hacia arriba
                        pintarMenuPrincipal(opcionActual);
                    }
                } 
                else if (tecla == TECLA_ENTER) {
                    mostrarCursorTexto(true); // Activar cursor para el ingreso de datos
                    system("cls");

                    switch (opcionActual) {
                        case 1: 
                            ejecutarAgendamiento(); 
                            break;
                        case 2:
                            cout << "\n=== TURNOS REGISTRADOS ===" << endl;
                            sistemaTurnos.mostrarLista();
                            system("pause");
                            break;
                        case 3: {
                            int idBuscar = 0, a = 0, m = 0, d = 0, nh = 0, nm = 0;
                            cout << "\n=== MODIFICAR TURNO POR ID ===" << endl;
                            cout << "Ingrese el ID del turno a modificar: "; cin >> idBuscar;
                            cout << "\n--- Ingrese los datos de la NUEVA cita ---" << endl;
                            cout << "Nuevo Anio: "; cin >> a;
                            cout << "Nuevo Mes (1-12): "; cin >> m;
                            cout << "Nuevo Dia: "; cin >> d;
                            cout << "Nueva Hora (08-16): "; cin >> nh;
                            cout << "Nuevos Minutos: "; cin >> nm;
                            Fecha nuevaFecha(d, m, a, nh, nm);
                            sistemaTurnos.modificar(idBuscar, nuevaFecha);
                            system("pause");
                            break;
                        }
                        case 4: {
                            int idBuscar = 0;
                            cout << "\n=== ELIMINAR / CANCELAR TURNO POR ID ===" << endl;
                            cout << "Ingrese el ID del turno a eliminar: "; cin >> idBuscar;
                            sistemaTurnos.eliminarPorID(idBuscar);
                            system("pause");
                            break;
                        }
                        case 5: 
                            ejecutarOrdenamientoTemplates(); // Abre el submenú interactivo
                            break;
                        case 6:
                            cout << "\n=== LISTA DE PLACAS CIFRADAS ===" << endl;
                            system("pause");
                            break;
                        case 7: 
                            ejecutando = false; 
                            break;
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
            cout << "        Sistema Cerrado Correctamente.      " << endl;
            cout << "============================================" << endl;
        }

        // CONTROLADOR INTERACTIVO CON TECLADO PARA LA OPCIÓN 5
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
                    "1. Ordenar por Nombre del Propietario (A-Z)",
                    "2. Ordenar por Apellido del Propietario (A-Z)",
                    "3. Ordenar por Placa del Vehiculo",
                    "4. Volver al Menu Principal"
                };

                for (int i = 0; i < 4; ++i) {
                    if ((seleccion - 1) == i) {
                        cambiarColor(0, 11);
                        cout << " -> " << opcionesSub[i] << "   " << endl;
                        cambiarColor(7, 0);
                    } else {
                        cout << "    " << opcionesSub[i] << "   " << endl;
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
                        if (opcionSub < 1) opcionSub = 4; // Ciclar al último
                        pintarSubMenu(opcionSub);
                    } 
                    else if (tecla == TECLA_ABAJO) {
                        opcionSub++;
                        if (opcionSub > 4) opcionSub = 1; // Ciclar al primero
                        pintarSubMenu(opcionSub);
                    }
                } 
                else if (tecla == TECLA_ENTER) {
                    mostrarCursorTexto(true);
                    system("cls");

                    if (opcionSub == 1) {
                        cout << "\n[+] Ordenando por Nombre del Propietario..." << endl;
                        auto porNombre = [](Turno* a, Turno* b) { return a->getUsuario().getNombre() > b->getUsuario().getNombre(); };
                        sistemaTurnos.ordenamiento(porNombre);
                        sistemaTurnos.mostrarLista();
                        system("pause");
                    } 
                    else if (opcionSub == 2) {
                        cout << "\n[+] Ordenando por Apellido del Propietario..." << endl;
                        auto porApellido = [](Turno* a, Turno* b) { return a->getUsuario().getApellido() > b->getUsuario().getApellido(); };
                        sistemaTurnos.ordenamiento(porApellido);
                        sistemaTurnos.mostrarLista();
                        system("pause");
                    } 
                    else if (opcionSub == 3) {
                        cout << "\n[+] Ordenando por Placa del Vehiculo..." << endl;
                        auto porPlaca = [](Turno* a, Turno* b) { return a->getVehiculo().getPlaca() > b->getVehiculo().getPlaca(); };
                        sistemaTurnos.ordenamiento(porPlaca);
                        sistemaTurnos.mostrarLista();
                        system("pause");
                    } 
                    else if (opcionSub == 4) {
                        subMenuActivo = false;
                        break; // Sale del submenú
                    }

                    mostrarCursorTexto(false);
                    system("cls");
                    pintarSubMenu(opcionSub);
                }
            }
        }

        void ejecutarAgendamiento() {
            int d = 0, m = 0, a = 0, hh = 0, mm = 0, yearAuto = 0;
            string cedula = "", nombre = "", apellido = "", telefono = "", correo = "";
            string placa = "", color = "";
            time_t tiempoActual = time(nullptr);
            tm* fechaSistema = localtime(&tiempoActual);
            int anioActual = fechaSistema->tm_year + 1900;
            int mesActual = fechaSistema->tm_mon + 1;
            int diaActual = fechaSistema->tm_mday;
            int horaActual = fechaSistema->tm_hour;
            int minActual = fechaSistema->tm_min;

            cout << "\n=== AGENDAR NUEVO TURNOS ===" << endl;
            bool fechaValida = false;
            do {
                cout << "Ingrese el anio: "; cin >> a;
                cout << "Ingrese el mes (1-12): "; cin >> m;
                cout << "Ingrese el dia: "; cin >> d;
                cout << "Ingrese la hora (08-16): "; cin >> hh;
                cout << "Ingrese los minutos: "; cin >> mm;
                if (a < anioActual || a > (anioActual + 1)) {
                    cout << "[RECHAZADO] El anio debe ser el actual o el siguiente.\n" << endl;
                    continue;
                }
                if (a == anioActual) {
                    if (m < mesActual) { cout << "[RECHAZADO] El mes ya paso.\n" << endl; continue; }
                    if (m == mesActual && d < diaActual) { cout << "[RECHAZADO] El dia ya paso.\n" << endl; continue; }
                    if (m == mesActual && d == diaActual) {
                        if (hh < horaActual || (hh == horaActual && mm <= minActual)) {
                            cout << "[RECHAZADO] La hora ya paso.\n" << endl;
                            continue;
                        }
                    }
                }
                fechaValida = true;
            } while (!fechaValida);

            Fecha fechaTentativa(d, m, a, hh, mm);
            string resultadoFeriado = fechaTentativa.validarFechaYObtenerFeriado(d, m, a);
            if (resultadoFeriado != "DISPONIBLE") {
                cout << "\n[RECHAZADO]: " << resultadoFeriado << endl;
                system("pause");
                return;
            }

            cin.ignore();
            do {
                cout << "Ingrese la cedula del dueno: "; getline(cin, cedula);
            } while (!validadorCedula.validar(cedula.c_str()));
            do {
                cout << "Ingrese el nombre: "; getline(cin, nombre);
            } while (!validador.validarNombre(nombre));
            do {
                cout << "Ingrese el apellido: "; getline(cin, apellido);
            } while (!validador.validarNombre(apellido));
            do {
                cout << "Ingrese su numero de telefono: "; getline(cin, telefono);
            } while (!validador.validarTelefono(telefono));
            do {
                cout << "Ingrese su correo electronico: "; getline(cin, correo);
            } while (!validador.validarCorreo(correo));

            Persona propietario(nombre, apellido, cedula, telefono, correo);
            cout << "==== REGISTRO DE VEHICULO ====" << endl;
            do {
                cout << "Ingrese el anio del vehiculo: "; cin >> yearAuto;
            } while (yearAuto < 1950 || yearAuto > (anioActual + 1));

            cin.ignore();
            do {
                cout << "Ingrese la placa: "; getline(cin, placa);
                for (char& c : placa) c = toupper(c);
            } while (!validador.validarPlaca(placa));

            int indicador = -1;
            for (auto it = placa.rbegin(); it != placa.rend(); ++it) {
                if (isdigit(*it)) { indicador = *it - '0'; break; }
            }
            if (indicador != -1) {
                int mesCorrecto = 0;
                if (indicador == 1) mesCorrecto = 2;
                else if (indicador == 2) mesCorrecto = 3;
                else if (indicador == 3) mesCorrecto = 4;
                else if (indicador == 4) mesCorrecto = 5;
                else if (indicador == 5) mesCorrecto = 6;
                else if (indicador == 6) mesCorrecto = 7;
                else if (indicador == 7) mesCorrecto = 8;
                else if (indicador == 8) mesCorrecto = 9;
                else if (indicador == 9) mesCorrecto = 10;
                else if (indicador == 0) mesCorrecto = 11;
                if (m != mesCorrecto && m != 12) {
                    cout << "\n[RECHAZADO] Calendarizacion ANT: Placas terminadas en " << indicador << " deben matricularse en el mes " << mesCorrecto << "." << endl;
                    system("pause");
                    return;
                }
            }
            do {
                cout << "Ingrese el color: "; getline(cin, color);
            } while (!validador.validarNombre(color));

            Vehiculo carro(color, placa, yearAuto);
            int idTurno = sistemaTurnos.generarSiguienteID();
            Turno* nuevoTurno = new Turno(propietario, carro, fechaTentativa, idTurno, true);
            sistemaTurnos.insertar(nuevoTurno);
            cout << "[EXITO] Registro completado. ID Asignado: " << idTurno << endl;
            system("pause");
        }
};

#endif