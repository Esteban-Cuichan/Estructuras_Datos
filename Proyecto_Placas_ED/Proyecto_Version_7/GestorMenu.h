#ifndef GESTORMENU_H
#define GESTORMENU_H
#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <sstream>
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
class GestorMenu{
    private:
        ListaDCE& sistemaTurnos;
        validarCedula validadorCedula;
        validarExpresion validador;
        void cambiarColor(int texto,int fondo){
            HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole,(fondo<<4)|texto);
        }
        void mostrarCursorTexto(bool mostrar){
            HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(hOutput,&cursorInfo);
            cursorInfo.bVisible=mostrar;
            SetConsoleCursorInfo(hOutput,&cursorInfo);
        }
        unsigned long generarHashDJB2(const string& cadena){
            unsigned long hash=5381;
            const char* ptr=cadena.c_str();
            while(*ptr){
                hash=((hash<<5)+hash)+*ptr;
                ptr++;
            }
            return hash;
        }
        int leerEntero(const string& mensaje){
            string entrada;
            int valor;
            while(true){
                cout<<mensaje;
                if(!getline(cin,entrada)){
                    cin.clear();
                    continue;
                }
                if(entrada.empty()){
                    cout<<"[ERROR] No puede dejar el campo vacio. Ingrese un numero.\n"<<endl;
                    continue;
                }
                bool esNumeroPuro=true;
                for(char c:entrada){
                    if(!isdigit(c)){
                        esNumeroPuro=false;
                        break;
                    }
                }
                if(!esNumeroPuro){
                    cout<<"[ERROR] Entrada invalida. Ingrese UNICAMENTE los numeros enteros.\n"<<endl;
                    continue;
                }
                stringstream ss(entrada);
                ss>>valor;
                return valor;
            }
        }
    public:
        GestorMenu(ListaDCE& sistema):sistemaTurnos(sistema){}
        void pintarMenuPrincipal(int seleccionActiva){
            HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
            COORD coord={0,0};
            SetConsoleCursorPosition(hOutput,coord);
            cout<<"============================================"<<endl;
            cout<<"     SISTEMA DE MATRICULACION VEHICULAR     "<<endl;
            cout<<"     (Use Flechas ARRIBA/ABAJO y ENTER)     "<<endl;
            cout<<"============================================"<<endl;
            const char* menu[]={
                "1. Agendar Nuevo Turno (Propietario y Vehiculo)",
                "2. Mostrar Turnos Registrados",
                "3. Modificar Turno por ID",
                "4. Cancelar/Eliminar Turno por ID",
                "5. Ordenar y Filtrar Reportes (Templates)",
                "6. Lista de placas cifradas (Hash)",
                "7. Salir del Sistema"
            };
            for(int i=0;i<7;++i){
                if((seleccionActiva-1)==i){
                    cambiarColor(0,11);
                    cout<<" -> "<<*(menu+i)<<"   "<<endl;
                    cambiarColor(7,0);
                }else{
                    cout<<"    "<<*(menu+i)<<"   "<<endl;
                }
            }
            cout<<"============================================"<<endl;
        }
        void ejecutarBusquedaBinariaTemplates(){
            system("cls");
            if(sistemaTurnos.getCabeza()==nullptr){
                cout<<"[INFO] El sistema esta vacio. No se puede realizar busquedas binarias.\n"<<endl;
                system("pause");
                return;
            }
            cout<<"=========================================================="<<endl;
            cout<<"       EJERCICIOS COMPLEMENTARIOS: BUSQUEDA BINARIA       "<<endl;
            cout<<"=========================================================="<<endl;
            cout<<"1. Ejercicio 1: Buscar Turno por Nombre exacto"<<endl;
            cout<<"2. Ejercicio 2: Buscar Turno por Cedula del Propietario"<<endl;
            cout<<"3. Ejercicio 3: Buscar Turno por Placa del Vehiculo"<<endl;
            int opcBusqueda=leerEntero("Seleccione el ejercicio de busqueda a evaluar: ");
            if(opcBusqueda<1||opcBusqueda>3){
                cout<<"[ERROR] Opcion no valida."<<endl;
                system("pause");
                return;
            }
            string valorBuscar="";
            cout<<"Ingrese el valor exacto a buscar: ";
            getline(cin,valorBuscar);
            typedef string(*ExtractorBusqueda)(Turno*);
            ExtractorBusqueda b1=[](Turno* t){return t->getUsuario().getNombre();};
            ExtractorBusqueda b2=[](Turno* t){return t->getUsuario().getCI();};
            ExtractorBusqueda b3=[](Turno* t){return t->getVehiculo().getPlaca();};
            ExtractorBusqueda tablaBusquedas[]={b1,b2,b3};
            cout<<"\n[PRE-REQUISITO] Alineando y ordenando la estructura para busqueda binaria..."<<endl;
            sistemaTurnos.ordenamiento(*(tablaBusquedas+(opcBusqueda-1)));
            cout<<"[PROCESANDO] Ejecutando algoritmo logaritmico O(log N)..."<<endl;
            Turno* encontrado=sistemaTurnos.busquedaBinaria(*(tablaBusquedas+(opcBusqueda-1)),valorBuscar);
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
        }
        void ejecutarOrdenamientoTemplates(){
            mostrarCursorTexto(false);
            int opcionSub=1;
            bool subMenuActivo=true;
            auto pintarSubMenu=[&](int seleccion){
                HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
                COORD coord={0,0};
                SetConsoleCursorPosition(hOutput,coord);
                cout<<"=========================================================="<<endl;
                cout<<"   === ORDENAR REPORTES DE TURNOS (BUBBLE SORT) ==="<<endl;
                cout<<"          (Use Flechas ARRIBA/ABAJO y ENTER)              "<<endl;
                cout<<"=========================================================="<<endl;
                const char* opcionesSub[]={
                    "1. Ordenar por Nombre del Propietario (A-Z)",
                    "2. Ordenar por Apellido del Propietario (A-Z)",
                    "3. Ordenar por Placa del Vehiculo",
                    "4. Ordenar Cronologicamente (Fecha y Hora)",
                    "5. [CONSULTA] Realizar Busqueda Binaria Rapida",
                    "6. Volver al Menu Principal"
                };
                for(int i=0;i<6;++i){
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
            system("cls");
            pintarSubMenu(opcionSub);
            while(subMenuActivo){
                int tecla=_getch();
                if(tecla==0||tecla==224){
                    tecla=_getch();
                    if(tecla==TECLA_ARRIBA){
                        opcionSub--;
                        if(opcionSub<1)opcionSub=6;
                        pintarSubMenu(opcionSub);
                    }else if(tecla==TECLA_ABAJO){
                        opcionSub++;
                        if(opcionSub>6)opcionSub=1;
                        pintarSubMenu(opcionSub);
                    }
                }else if(tecla==TECLA_ENTER){
                    mostrarCursorTexto(true);
                    system("cls");
                    if(opcionSub==6){
                        subMenuActivo=false;
                        break;
                    }
                    if(opcionSub==5){
                        ejecutarBusquedaBinariaTemplates();
                    }else if(opcionSub==4){
                        cout<<"\n[+] Generando reporte cronologico..."<<endl;
                        if(sistemaTurnos.getCabeza()==nullptr){
                            cout<<"[INFO] No hay turnos registrados para generar el reporte."<<endl;
                            system("pause");
                        }else{
                            int totalTurnos=0;
                            Nodo* actual=sistemaTurnos.getCabeza();
                            do{
                                if(actual->getTurno()!=nullptr){totalTurnos++;}
                                actual=actual->getSiguiente();
                            }while(actual!=sistemaTurnos.getCabeza());
                            Turno** turnosTemporales=new Turno*[totalTurnos];
                            int indice=0;
                            actual=sistemaTurnos.getCabeza();
                            do{
                                if(actual->getTurno()!=nullptr){
                                    *(turnosTemporales+indice)=actual->getTurno();
                                    indice++;
                                }
                                actual=actual->getSiguiente();
                            }while(actual!=sistemaTurnos.getCabeza());
                            for(int i=0;i<totalTurnos-1;i++){
                                for(int j=0;j<totalTurnos-i-1;j++){
                                    Fecha f1=(*(turnosTemporales+j))->getFecha();
                                    Fecha f2=(*(turnosTemporales+j+1))->getFecha();
                                    bool intercambiar=false;
                                    if(f1.getYear()!=f2.getYear())intercambiar=(f1.getYear()>f2.getYear());
                                    else if(f1.getMes()!=f2.getMes())intercambiar=(f1.getMes()>f2.getMes());
                                    else if(f1.getDia()!=f2.getDia())intercambiar=(f1.getDia()>f2.getDia());
                                    else if(f1.getHora()!=f2.getHora())intercambiar=(f1.getHora()>f2.getHora());
                                    else intercambiar=(f1.getMinutos()>f2.getMinutos());
                                    if(intercambiar){
                                        Turno* aux=*(turnosTemporales+j);
                                        *(turnosTemporales+j)=*(turnosTemporales+j+1);
                                        *(turnosTemporales+j+1)=aux;
                                    }
                                }
                            }
                            ofstream archivoReporte("reporte_cronologico_placas.txt",ios::out);
                            cout<<"\n       LISTADO DE PLACAS ORDENADO CRONOLOGICAMENTE      "<<endl;
                            cout<<"========================================================"<<endl;
                            if(archivoReporte.is_open()){
                                for(int i=0;i<totalTurnos;i++){
                                    Fecha f=(*(turnosTemporales+i))->getFecha();
                                    string placa=(*(turnosTemporales+i))->getVehiculo().getPlaca();
                                    string minStr=(f.getMinutos()<10?"0":"")+to_string(f.getMinutos());
                                    stringstream ss;
                                    ss<<f.getDia()<<"/"<<f.getMes()<<"/"<<f.getYear()<<" - "<<f.getHora()<<":"<<minStr<<"h - "<<placa;
                                    cout<<"  - "<<ss.str()<<endl;
                                    archivoReporte<<ss.str()<<"\n";
                                }
                                cout<<"========================================================"<<endl;
                                archivoReporte.close();
                                cout<<"\n[EXITO] Reporte guardado en 'reporte_cronologico_placas.txt'."<<endl;
                            }else{
                                cout<<"\n[ERROR] No se pudo crear el archivo de reporte externo."<<endl;
                            }
                            delete[] turnosTemporales;
                            system("pause");
                        }
                    }else{
                        typedef string(*ExtractorAtributo)(Turno*);
                        ExtractorAtributo c1=[](Turno* t){return t->getUsuario().getNombre();};
                        ExtractorAtributo c2=[](Turno* t){return t->getUsuario().getApellido();};
                        ExtractorAtributo c3=[](Turno* t){return t->getVehiculo().getPlaca();};
                        ExtractorAtributo tablaCriterios[]={c1,c2,c3};
                        const char* mensajesSub[]={
                            "\n[+] Ordenando por Nombre del Propietario...",
                            "\n[+] Ordenando por Apellido del Propietario...",
                            "\n[+] Ordenando por Placa del Vehiculo..."
                        };
                        cout<<*(mensajesSub+(opcionSub-1))<<endl;
                        sistemaTurnos.ordenamiento(*(tablaCriterios+(opcionSub-1)));
                        sistemaTurnos.mostrarLista();
                        system("pause");
                    }
                    mostrarCursorTexto(false);
                    system("cls");
                    pintarSubMenu(opcionSub);
                }
            }
        }
        void iniciarMenuInteractivo(){
            mostrarCursorTexto(false);
            int opcionActual=1;
            bool ejecutando=true;
            system("cls");
            pintarMenuPrincipal(opcionActual);
            while(ejecutando){
                int tecla=_getch();
                if(tecla==0||tecla==224){
                    tecla=_getch();
                    if(tecla==TECLA_ARRIBA){
                        opcionActual--;
                        if(opcionActual<1)opcionActual=7;
                        pintarMenuPrincipal(opcionActual);
                    }else if(tecla==TECLA_ABAJO){
                        opcionActual++;
                        if(opcionActual>7)opcionActual=1;
                        pintarMenuPrincipal(opcionActual);
                    }
                }else if(tecla==TECLA_ENTER){
                    mostrarCursorTexto(true);
                    system("cls");
                    switch(opcionActual){
                        case 1:ejecutarAgendamiento();break;
                        case 2:
                            cout<<"\n=== TURNOS REGISTRADOS ==="<<endl;
                            sistemaTurnos.mostrarLista();
                            system("pause");
                            break;
                        case 3:{
                            int idBuscar=0,a=0,m=0,d=0,nh=0,nm=0;
                            cout<<"\n=== MODIFICAR TURNO POR ID ==="<<endl;
                            idBuscar=leerEntero("Ingrese el ID del turno a modificar: ");
                            cout<<"\n--- Ingrese los datos de la NUEVA cita ---"<<endl;
                            a=leerEntero("Nuevo Anio: ");
                            m=leerEntero("Nuevo Mes (1-12): ");
                            d=leerEntero("Nuevo Dia: ");
                            nh=leerEntero("Nueva Hora (08-16): ");
                            nm=leerEntero("Nuevos Minutos: ");
                            Fecha nuevaFecha(d,m,a,nh,nm);
                            sistemaTurnos.modificar(idBuscar,nuevaFecha);
                            system("pause");
                            break;
                        }
                        case 4:{
                            int idBuscar=0;
                            cout<<"\n=== ELIMINAR / CANCELAR TURNO POR ID ==="<<endl;
                            idBuscar=leerEntero("Ingrese el ID del turno a eliminar: ");
                            sistemaTurnos.eliminarPorID(idBuscar);
                            system("pause");
                            break;
                        }
                        case 5:ejecutarOrdenamientoTemplates();break;
                        case 6:{
                            cout<<"\n=== LISTA DE PLACAS CON INTEGRIDAD HASH (DJB2) ==="<<endl;
                            if(sistemaTurnos.getCabeza()==nullptr){
                                cout<<"[INFO] No hay vehiculos registrados para procesar."<<endl;
                            }else{
                                Nodo* actual=sistemaTurnos.getCabeza();
                                do{
                                    string placa=actual->getTurno()->getVehiculo().getPlaca();
                                    unsigned long codigoHash=generarHashDJB2(placa);
                                    cout<<"  [+] Vehiculo Placa: "<<placa<<" -> HASH Digital Unico: "<<codigoHash<<endl;
                                    actual=actual->getSiguiente();
                                }while(actual!=sistemaTurnos.getCabeza());
                            }
                            system("pause");
                            break;
                        }
                        case 7:ejecutando=false;break;
                    }
                    if(ejecutando){
                        mostrarCursorTexto(false);
                        system("cls");
                        pintarMenuPrincipal(opcionActual);
                    }
                }
            }
            mostrarCursorTexto(true);
            system("cls");
            cout<<"============================================"<<endl;
            cout<<"        Sistema Cerrado Correctamente.      "<<endl;
            cout<<"============================================"<<endl;
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
            cout<<"\n=== AGENDAR NUEVO TURNOS ==="<<endl;
            bool fechaValida=false;
            do{
                a=leerEntero("Ingrese el anio: ");
                m=leerEntero("Ingrese el mes (1-12): ");
                d=leerEntero("Ingrese el dia: ");
                hh=leerEntero("Ingrese la hora (08-16): ");
                mm=leerEntero("Ingrese los minutos: ");
                if(a<anioActual||a>(anioActual+1)){
                    cout<<"[RECHAZADO] El anio debe ser el actual o el siguiente.\n"<<endl;
                    continue;
                }
                if(a==anioActual){
                    if(m<mesActual){cout<<"[RECHAZADO] El mes ya paso.\n"<<endl;continue;}
                    if(m==mesActual&&d<diaActual){cout<<"[RECHAZADO] El dia ya paso.\n"<<endl;continue;}
                    if(m==mesActual&&d==diaActual){
                        if(hh<horaActual||(hh==horaActual&&mm<=minActual)){
                            cout<<"[RECHAZADO] La hora ya paso.\n"<<endl;
                            continue;
                        }
                    }
                }
                fechaValida=true;
            }while(!fechaValida);
            Fecha fechaTentativa(d,m,a,hh,mm);
            string resultadoFeriado=fechaTentativa.validarFechaYObtenerFeriado(d,m,a);
            if(resultadoFeriado!="DISPONIBLE"){
                cout<<"\n[RECHAZADO]: "<<resultadoFeriado<<endl;
                system("pause");
                return;
            }
            do{
                cout<<"Ingrese la cedula del dueno: ";getline(cin,cedula);
            }while(!validadorCedula.validar(cedula.c_str()));
            do{
                cout<<"Ingrese el nombre: ";getline(cin,nombre);
            }while(!validador.validarNombre(nombre));
            do{
                cout<<"Ingrese el apellido: ";getline(cin,apellido);
            }while(!validador.validarNombre(apellido));
            do{
                cout<<"Ingrese su numero de telefono: ";getline(cin,telefono);
            }while(!validador.validarTelefono(telefono));
            do{
                cout<<"Ingrese su correo electronico: ";getline(cin,correo);
            }while(!validador.validarCorreo(correo));
            Persona propietario(nombre,apellido,cedula,telefono,correo);
            cout<<"==== REGISTRO DE VEHICULO ===="<<endl;
            while(true){
                yearAuto=leerEntero("Ingrese el anio del vehiculo: ");
                if(yearAuto>=1950&&yearAuto<=(anioActual+1))break;
            }
            do{
                cout<<"Ingrese la placa: ";getline(cin,placa);
                for(char& c:placa)c=toupper(c);
            }while(!validador.validarPlaca(placa));
            int indicador=-1;
            for(auto it=placa.rbegin();it!=placa.rend();++it){
                if(isdigit(*it)){indicador=*it-'0';break;}
            }
            if(indicador!=-1){
                int mesCorrecto=0;
                if(indicador==1)mesCorrecto=2;
                else if(indicador==2)mesCorrecto=3;
                else if(indicador==3)mesCorrecto=4;
                else if(indicador==4)mesCorrecto=5;
                else if(indicador==5)mesCorrecto=6;
                else if(indicador==6)mesCorrecto=7;
                else if(indicador==7)mesCorrecto=8;
                else if(indicador==8)mesCorrecto=9;
                else if(indicador==9)mesCorrecto=10;
                else if(indicador==0)mesCorrecto=11;
                if(m!=mesCorrecto&&m!=12){
                    cout<<"\n[RECHAZADO] Calendarizacion ANT: Placas terminadas en "<<indicador<<" deben matricularse en el mes "<<mesCorrecto<<"."<<endl;
                    system("pause");
                    return;
                }
            }
            do{
                cout<<"Ingrese el color: ";getline(cin,color);
            }while(!validador.validarNombre(color));
            Vehiculo carro(color,placa,yearAuto);
            int idTurno=sistemaTurnos.generarSiguienteID();
            Turno* nuevoTurno=new Turno(propietario,carro,fechaTentativa,idTurno,true);
            sistemaTurnos.insertar(nuevoTurno);
            cout<<"[EXITO] Registro completado. ID Asignado: "<<idTurno<<endl;
            system("pause");
        }
};
#endif