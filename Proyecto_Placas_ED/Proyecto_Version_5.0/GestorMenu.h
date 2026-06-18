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

// Estructura auxiliar para manejar el catálogo dinámico de intervalos de tiempo
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

        // FUNCIÓN CLAVE: Verifica en la lista doblemente circular enlazada si la fecha y hora ya están tomadas
        bool verificarCitaOcupada(int d, int m, int a, int hh, int mm) {
            if (sistemaTurnos.getCabeza() == nullptr) return false;

            Nodo* actual = sistemaTurnos.getCabeza();
            do {
                Turno* t = actual->getTurno();
                if (t != nullptr && t->getEstado()) { // Si el turno está activo
                    Fecha f = t->getFecha();
                    if (f.getDia() == d && f.getMes() == m && f.getYear() == a &&
                        f.getHora() == hh && f.getMinutos() == mm) {
                        return true; // Encontrado, el bloque está ocupado
                    }
                }
                actual = actual->getSiguiente();
            } while (actual != sistemaTurnos.getCabeza());

            return false;
        }

        // NUEVO MÉTODO INTERACTIVO: Permite seleccionar el intervalo de 15 minutos usando flechas
        bool seleccionarBloqueHorario(int d, int m, int a, int anioActual, int mesActual, int diaActual, int horaActual, int minActual, int& horaSeleccionada, int& minutoSeleccionado) {
            mostrarCursorTexto(false);
            
            // 1. Generar los 32 bloques de intervalos de 15 minutos (08:00 a 15:45)
            const int TOTAL_INTERVALOS = 32;
            IntervaloTiempo bloques[TOTAL_INTERVALOS];
            int indice = 0;

            for (int h = 8; h <= 15; ++h) {
                for (int min = 0; min < 60; min += 15) {
                    bloques[indice].hora = h;
                    bloques[indice].minuto = min;
                    
                    // Formatear texto visual en formato de 2 dígitos
                    string strH = (h < 10 ? "0" : "") + to_string(h);
                    string strM = (min == 0 ? "00" : to_string(min));
                    bloques[indice].textoVisual = strH + ":" + strM;

                    // Evaluar disponibilidad inicial basándose en la persistencia / base de datos en memoria
                    bool yaOcupado = verificarCitaOcupada(d, m, a, h, min);
                    
                    // Evaluar si el intervalo ya pasó en el tiempo físico (si es el día de hoy)
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

                // Renderizado de la cuadrícula/lista de turnos en consola
                for (int i = 0; i < TOTAL_INTERVALOS; ++i) {
                    string estadoStr = bloques[i].disponible ? "[ DISPONIBLE ]" : "[   OCUPADO  ]";
                    
                    if (seleccionActiva == i) {
                        if (bloques[i].disponible) {
                            cambiarColor(0, 11); // Fondo cian si está seleccionado y disponible
                        } else {
                            cambiarColor(12, 15); // Fondo blanco con letras rojas si está ocupado
                        }
                        cout << " -> " << bloques[i].textoVisual << " h   " << estadoStr << " \n";
                        cambiarColor(7, 0);
                    } else {
                        if (bloques[i].disponible) {
                            cambiarColor(10, 0); // Texto verde si está disponible
                            cout << "    " << bloques[i].textoVisual << " h   " << estadoStr << " \n";
                        } else {
                            cambiarColor(8, 0); // Texto gris oscuro si ya está ocupado
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
                    // Validar si el bloque que el usuario quiere seleccionar está disponible
                    if (bloques[seleccionActiva].disponible) {
                        horaSeleccionada = bloques[seleccionActiva].hora;
                        minutoSeleccionado = bloques[seleccionActiva].minuto;
                        mostrarCursorTexto(true);
                        return true; // Selección exitosa
                    } else {
                        cout << "\a"; // Pitido de advertencia del sistema
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

        void pintarMenuPrincipal(int seleccionActiva) {
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
                        case 3: {
                            cout << "\n=== MODIFICAR TURNO POR ID ===" << endl;
                            int idBuscar = leerEntero("Ingrese el ID del turno a modificar: ");
                            
                            // Reutiliza la lógica para actualizar la fecha
                            cout << "\n--- Ingrese los datos de la NUEVA fecha ---" << endl;
                            int a = leerEntero("Nuevo Anio: ");
                            int m = leerEntero("Nuevo Mes (1-12): ");
                            int d = leerEntero("Nuevo Dia: ");
                            
                            // Obtener tiempos actuales para validar en la subselección
                            time_t tiempoActual = time(nullptr);
                            tm* fechaSistema = localtime(&tiempoActual);
                            int anioActual = fechaSistema->tm_year + 1900;
                            int mesActual = fechaSistema->tm_mon + 1;
                            int diaActual = fechaSistema->tm_mday;
                            int horaActual = fechaSistema->tm_hour;
                            int minActual = fechaSistema->tm_min;

                            int nh = 0, nm = 0;
                            if (seleccionarBloqueHorario(d, m, a, anioActual, mesActual, diaActual, horaActual, minActual, nh, nm)) {
                                Fecha nuevaFecha(d, m, a, nh, nm);
                                sistemaTurnos.modificar(idBuscar, nuevaFecha);
                                cout << "\n[EXITO] Turno modificado correctamente." << endl;
                            } else {
                                cout << "\n[INFO] Modificacion cancelada." << endl;
                            }
                            system("pause");
                            break;
                        }
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
            cout << "        Sistema Cerrado Correctamente.      " << endl;
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
                    "1. Ordenar por Nombre del Propietario (A-Z)",
                    "2. Ordenar por Apellido del Propietario (A-Z)",
                    "3. Ordenar por Placa del Vehiculo",
                    "4. Ordenar por Fecha del Turno",
                    "5. [CONSULTA] Realizar Busqueda Binaria Rapida",
                    "6. Volver al Menu Principal"
                };
                for (int i = 0; i < 6; ++i) {
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
                        if (opcionSub < 1) opcionSub = 6;
                        pintarSubMenu(opcionSub);
                    } else if (tecla == TECLA_ABAJO) {
                        opcionSub++;
                        if (opcionSub > 6) opcionSub = 1;
                        pintarSubMenu(opcionSub);
                    }
                } else if (tecla == TECLA_ENTER) {
                    mostrarCursorTexto(true);
                    system("cls");
                    if (opcionSub == 6) {
                        subMenuActivo = false;
                        break;
                    }
                    if (opcionSub == 5) {
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

        void ejecutarBusquedaBinariaTemplates() {
            system("cls");
            if (sistemaTurnos.getCabeza() == nullptr) {
                cout << "[INFO] El sistema esta vacio. No se puede realizar busquedas binarias.\n" << endl;
                system("pause");
                return;
            }
            cout << "==========================================================" << endl;
            cout << "       EJERCICIOS COMPLEMENTARIOS: BUSQUEDA BINARIA       " << endl;
            cout << "==========================================================" << endl;
            cout << "1. Ejercicio 1: Buscar Turno por Nombre exacto" << endl;
            cout << "2. Ejercicio 2: Buscar Turno por Cedula del Propietario" << endl;
            cout << "3. Ejercicio 3: Buscar Turno por Placa del Vehiculo" << endl;
            int opcBusqueda = leerEntero("Seleccione el ejercicio de busqueda a evaluar: ");
            if (opcBusqueda < 1 || opcBusqueda > 3) {
                cout << "[ERROR] Opcion no valida." << endl;
                system("pause");
                return;
            }
            string valorBuscar = "";
            cout << "Ingrese el valor exacto a buscar: ";
            getline(cin, valorBuscar);
            typedef string(*ExtractorBusqueda)(Turno*);
            ExtractorBusqueda b1 = [](Turno* t) {return t->getUsuario().getNombre();};
            ExtractorBusqueda b2 = [](Turno* t) {return t->getUsuario().getCI();};
            ExtractorBusqueda b3 = [](Turno* t) {return t->getVehiculo().getPlaca();};
            ExtractorBusqueda tablaBusquedas[] = {b1, b2, b3};
            
            cout << "\n[PRE-REQUISITO] Alineando y ordenando la estructura para busqueda binaria..." << endl;
            sistemaTurnos.ordenamiento(*(tablaBusquedas + (opcBusqueda - 1)));
            
            cout << "[PROCESANDO] Ejecutando algoritmo logaritmico O(log N)..." << endl;
            Turno* encontrado = sistemaTurnos.busquedaBinaria(*(tablaBusquedas + (opcBusqueda - 1)), valorBuscar);
            if (encontrado != nullptr) {
                cout << "\n========================================================" << endl;
                cout << "           RESULTADO DE LA BUSQUEDA BINARIA             " << endl;
                cout << "========================================================" << endl;
                cout << "  [+] ID Asignado : " << encontrado->getIDturno() << endl;
                cout << "  [+] Propietario : " << encontrado->getUsuario().getNombre() << " " << encontrado->getUsuario().getApellido() << endl;
                cout << "  [+] Cedula      : " << encontrado->getUsuario().getCI() << endl;
                cout << "  [+] Vehiculo    : Placa [" << encontrado->getVehiculo().getPlaca() << "] - Color [" << encontrado->getVehiculo().getColor() << "]" << endl;
                cout << "  [+] Cita Agendada: " << encontrado->getFecha().getDia() << "/" << encontrado->getFecha().getMes() << "/" << encontrado->getFecha().getYear() << endl;
                cout << "========================================================" << endl;
            } else {
                cout << "\n[ERROR] Busqueda Binaria concluida sin coincidencias. El registro no existe." << endl;
            }
            system("pause");
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
            
            cin.clear();
            cout << "\n=== AGENDAR NUEVO TURNO ===" << endl;
            while (true) {
                a = leerEntero("Ingrese el anio (Actual o Siguiente): ");
                if (a >= anioActual && a <= (anioActual + 1)) break;
                cout << "[RECHAZADO] El anio debe ser el actual (" << anioActual << ") o el siguiente (" << anioActual + 1 << "). Intente de nuevo.\n" << endl;
            }
            while (true) {
                while (true) {
                    m = leerEntero("Ingrese el mes (1-12): ");
                    if (m < 1 || m > 12) {
                        cout << "[RECHAZADO] El mes debe estar estrictamente entre 1 and 12.\n" << endl;
                        continue;
                    }
                    if (a == anioActual && m < mesActual) {
                        cout << "[RECHAZADO] El mes ingresado ya transcurrio en el presente anio (" << mesActual << ").\n" << endl;
                        continue;
                    }
                    break;
                }
                bool regresarAMes = false;
                while (true) {
                    cout << "--- (O si desea cambiar de mes, ingrese 0) ---" << endl;
                    d = leerEntero("Ingrese el dia: ");
                    if (d == 0) {
                        regresarAMes = true;
                        cout << "\n[INFO] Regresando a la seleccion de mes...\n" << endl;
                        break;
                    }
                    if (d < 1 || d > 31) {
                        cout << "[RECHAZADO] El dia debe estar entre 1 and 31.\n" << endl;
                        continue;
                    }
                    if (a == anioActual && m == mesActual && d < diaActual) {
                        cout << "[RECHAZADO] El dia ingresado ya paso. Hoy es dia " << diaActual << ".\n" << endl;
                        continue;
                    }
                    Fecha fechaAuxiliar(d, m, a, 0, 0); 
                    string resultadoFeriado = fechaAuxiliar.validarFechaYObtenerFeriado(d, m, a);
                    if (resultadoFeriado != "DISPONIBLE") {
                        cout << "[RECHAZADO]: " << resultadoFeriado << " Intente con otra fecha u otra casilla.\n" << endl;
                        continue; 
                    }
                    break;
                }
                if (!regresarAMes) break; 
            }

            // =========================================================================
            // AQUÍ SE INTEGRÓ LA NUEVA SELECCIÓN INTERACTIVA CADA 15 MINUTOS
            // =========================================================================
            cout << "\n[PROCESANDO] Cargando la disponibilidad de horas desde el sistema..." << endl;
            Sleep(500);
            
            if (!seleccionarBloqueHorario(d, m, a, anioActual, mesActual, diaActual, horaActual, minActual, hh, mm)) {
                cout << "[INFO] Agendamiento cancelado por falta de horarios." << endl;
                system("pause");
                return;
            }

            Fecha fechaTentativa(d, m, a, hh, mm);
            cout << "\n--- DATOS DEL PROPIETARIO ---" << endl;
            do {
                cout << "Ingrese la cedula del dueno (10 digitos): "; 
                getline(cin, cedula);
                if (!validadorCedula.validar(cedula.c_str())) {
                    cout << "[ERROR] La cedula es invalida (Fallo el algoritmo de verificacion ecuatoriano).\n" << endl;
                }
            } while (!validadorCedula.validar(cedula.c_str()));
            do {
                cout << "Ingrese el nombre (Solo letras): "; 
                getline(cin, nombre);
                if (!validador.validarNombre(nombre)) {
                    cout << "[ERROR] Nombre invalido. No se permiten numeros ni simbolos.\n" << endl;
                }
            } while (!validador.validarNombre(nombre));
            do {
                cout << "Ingrese el apellido (Solo letras): "; 
                getline(cin, apellido);
                if (!validador.validarNombre(apellido)) {
                    cout << "[ERROR] Apellido invalido. No se permiten numeros ni simbolos.\n" << endl;
                }
            } while (!validador.validarNombre(apellido));
            do {
                cout << "Ingrese su numero de telefono (10 digitos): "; 
                getline(cin, telefono);
                if (!validador.validarTelefono(telefono)) {
                    cout << "[ERROR] Telefono invalido. Deben ser exactamente 10 digitos numericos.\n" << endl;
                }
            } while (!validador.validarTelefono(telefono));
            do {
                cout << "Ingrese su correo electronico (ejemplo@dominio.com): "; 
                getline(cin, correo);
                if (!validador.validarCorreo(correo)) {
                    cout << "[ERROR] Formato de correo electronico incorrecto.\n" << endl;
                }
            } while (!validador.validarCorreo(correo));
            Persona propietario(nombre, apellido, cedula, telefono, correo);
            cout << "\n==== REGISTRO DE VEHICULO ====" << endl;
            while (true) {
                yearAuto = leerEntero("Ingrese el anio del vehiculo (1950 - posterior): ");
                if (yearAuto >= 1950 && yearAuto <= (anioActual + 1)) break;
                cout << "[ERROR] Anio fuera de rango permitido (1950 - " << (anioActual + 1) << ").\n" << endl;
            }
            do {
                cout << "Ingrese la placa (Ej: ABC1234): "; 
                getline(cin, placa);
                for (char& c : placa) c = toupper(c);
                if (!validador.validarPlaca(placa)) {
                    cout << "[ERROR] Formato de placa invalido (Deben ser 3 letras mayusculas seguidas de 3 o 4 numeros).\n" << endl;
                }
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
                cout << "Ingrese el color del vehiculo: "; 
                getline(cin, color);
                if (!validador.validarNombre(color)) {
                    cout << "[ERROR] Color invalido. Solo se aceptan caracteres alfabeticos.\n" << endl;
                }
            } while (!validador.validarNombre(color));
            Vehiculo carro(color, placa, yearAuto);
            int idTurno = sistemaTurnos.generarSiguienteID();
            Turno* nuevoTurno = new Turno(propietario, carro, fechaTentativa, idTurno, true);
            sistemaTurnos.insertar(nuevoTurno);
            cout << "\n[EXITO] Registro completado de forma segura. ID Asignado: " << idTurno << endl;
            system("pause");
        }
};

#endif