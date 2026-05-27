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
using namespace std;

class GestorMenu {
    private:
        ListaDCE& sistemaTurnos;
        validarCedula validadorCedula;
        validarExpresion validador;
        void cambiarColor(int texto,int fondo){
            HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole,(fondo<<4)|texto);
        }

    public:
        GestorMenu(ListaDCE& sistema):sistemaTurnos(sistema){}
        void activarMouse(){
            HANDLE hInput=GetStdHandle(STD_INPUT_HANDLE);
            DWORD modo;
            GetConsoleMode(hInput,&modo);
            SetConsoleMode(hInput,modo|ENABLE_MOUSE_INPUT|ENABLE_EXTENDED_FLAGS);
        }

        void pintarMenuPrincipal(int seleccionActiva){
            cout<<"============================================"<<endl;
            cout<<"     SISTEMA DE MATRICULACION VEHICULAR     "<<endl;
            cout<<"  (Use Flechas del Teclado o Clic del Mouse) "<<endl;
            cout<<"============================================"<<endl;
            const char* menu[]={"1. Agendar Nuevo Turno (Propietario y Vehiculo)",
                                "2. Mostrar Turnos Registrados",
                                "3. Modificar Turno por ID",
                                "4. Cancelar/Eliminar Turno por ID",
                                "5. Ordenar y Filtrar Reportes (Templates)",
                                "6. Lista de placas cifradas",
                                "7. Salir del Sistema"
            };

            for(int i=0;i<7;++i){
                if((seleccionActiva-1)==i){
                    cambiarColor(0,11);
                    cout<<" -> "<<*(menu+i)<<endl;
                    cambiarColor(7,0);
                }else{
                    cout<<"    "<<*(menu+i)<<endl;
                }
            }
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

            cout<<"\n=== AGENDAR NUEVO TURNO ==="<<endl;
            bool fechaValida=false;
            do{
                cout<<"Ingrese el anio: ";cin>>a;
                cout<<"Ingrese el mes (1-12): ";cin>>m;
                cout<<"Ingrese el dia: ";cin>>d;
                cout<<"Ingrese la hora (08-16): ";cin>>hh;
                cout<<"Ingrese los minutos: ";cin>>mm;
                if(a<anioActual||a>(anioActual+1)){
                    cout<<"[RECHAZADO] El anio del turno debe ser el actual o maximo el siguiente.\n"<<endl;
                    continue;
                }
                if(a==anioActual){
                    if(m<mesActual){cout<<"[RECHAZADO] El mes ya paso.\n"<<endl;continue;}
                    if(m==mesActual&&d<diaActual){cout<<"[RECHAZADO] El dia ya paso.\n"<<endl;continue;}
                    if(m==mesActual&&d==diaActual){
                        if(hh<horaActual||(hh==horaActual&&mm<=minActual)){
                            cout<<"[RECHAZADO] La hora ingresada ya transcurrio.\n"<<endl;
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

            cin.ignore();
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
            do{
                cout<<"Ingrese el anio del vehiculo: ";cin>>yearAuto;
            }while(yearAuto<1950||yearAuto>(anioActual+1));

            cin.ignore();
            do{
                cout<<"Ingrese la placa: ";getline(cin,placa);
                for(char &c:placa)c=toupper(c);
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

        void ejecutarOrdenamientoTemplates(){
            int opcOrden=0;
            cout<<"\n=== ORDENAR REPORTES DE TURNOS (BUBBLE SORT TEMPLATES) ==="<<endl;
            cout<<"1. Ordenar por Nombre del Propietario (A-Z)"<<endl;
            cout<<"2. Ordenar por Apellido del Propietario (A-Z)"<<endl;
            cout<<"3. Ordenar por Placa del Vehiculo"<<endl;
            cout<<"Seleccione su criterio: ";cin>>opcOrden;

            if(opcOrden==1){
                auto porNombre=[](Turno* a,Turno* b){return a->getUsuario().getNombre()>b->getUsuario().getNombre();};
                sistemaTurnos.ordenamiento(porNombre);
            }else if(opcOrden==2){
                auto porApellido=[](Turno* a,Turno* b){return a->getUsuario().getApellido()>b->getUsuario().getApellido();};
                sistemaTurnos.ordenamiento(porApellido);
            }else if(opcOrden==3){
                auto porPlaca=[](Turno* a,Turno* b){return a->getVehiculo().getPlaca()>b->getVehiculo().getPlaca();};
                sistemaTurnos.ordenamiento(porPlaca);
            }
            sistemaTurnos.mostrarLista();
            system("pause");
        }
};

#endif