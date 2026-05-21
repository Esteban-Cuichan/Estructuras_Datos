#define _WINSOCKAPI_
#define NOWINBASEINTERLOCKS
#ifndef NOMINMAX
    #define NOMINMAX
#endif
#define TYPE_ALIGNMENT 1
#define _GDI32_
#define DBG 0
#define fdesc unsigned long
#define _RPCNDR_H_
#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#define DONOT_DEFINE_BYTE
#include <windows.h>
#include "ListaDCE.h"
#include "Persona.h"
#include "Vehiculo.h"
#include "Fecha.h"
#include "Turno.h"
#include "validarCedula.h"
#include "validarExpresion.h"

void cambiarColor(int texto, int fondo){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,(fondo<<4) | texto);
}

int main(){
    ListaDCE sistemaTurnos;
    validarCedula validadorCedula;
    validarExpresion validador;
    int seleccionActiva=1;
    int totalOpciones=5;
    bool ejecutado=true;
    while(ejecutado){
        system("cls");
        cout<<"============================================"<<endl;
        cout<<"     SISTEMA DE MATRICULACION VEHICULAR     "<<endl;
        cout<<"============================================"<<endl;
        if(seleccionActiva==1){
            cambiarColor(0,11);
            cout<<" -> 1. Agendar Nuevo Turno (Propietario y Vehiculo) "<<endl;
            cambiarColor(7,0);
        }else{
            cout<<"    1. Agendar Nuevo Turno (Propietario y Vechiculo) "<<endl;
        }

        if(seleccionActiva==2){
            cambiarColor(0,11);
            cout<<" -> 2. Mostrar Turnos Registrados "<<endl;
            cambiarColor(7,0);
        }else{
            cout<<"    2. Mostrar Turnos Registrados "<<endl;
        }
        if(seleccionActiva==3){
            cambiarColor(0,11);
            cout<<" -> 3. Modificar Turno por ID "<<endl;
            cambiarColor(7,0);
        }else{
            cout<<"    3. Modificar Turno por ID "<<endl;
        }
        if(seleccionActiva==4){
            cambiarColor(0,11);
            cout<<" -> 4. Cancelar/Eliminar Turno por ID "<<endl;
            cambiarColor(7,0);
        }else{
            cout<<"    4. Cancelar/Eliminar Turno por ID "<<endl;
        }
        if(seleccionActiva==5){
            cambiarColor(0,11);
            cout<<" -> 5. Salir del Sistema "<<endl;
            cambiarColor(7,0);
        }else{
            cout<<"    5. Salir del Sistema "<<endl;
        }
        cout<<"============================================"<<endl;
        char tecla=_getch();
        if(tecla==0 || tecla==-32){
            tecla=_getch();
            if(tecla==72){
                seleccionActiva=(seleccionActiva==1) ? totalOpciones:seleccionActiva-1;
            }else if(tecla==80){
                seleccionActiva=(seleccionActiva==totalOpciones) ? 1 :seleccionActiva + 1;
            }
        }else if(tecla==13){
            system("cls");
            switch(seleccionActiva){
                case 1:{
                    int d=0,m=0,a=0,hh=0,mm=0,yearAuto=0;
                    string cedula="",nombre="",apellido="",telefono="",correo="";
                    string placa="",color="";
                    time_t tiempoActual=time(nullptr);
                    tm* fechaSistema=localtime(&tiempoActual);
                    int anioActual = fechaSistema->tm_year + 1900;
                    int mesActual = fechaSistema->tm_mon + 1;
                    int diaActual = fechaSistema->tm_mday;
                    int horaActual = fechaSistema->tm_hour;
                    int minActual = fechaSistema->tm_min;
                    cout<<"\n=== AGENDAR NUEVO TURNO ==="<<endl;
                    bool fechaValida=false;
                    do{
                        cout<<"Ingrese el anio: ";
                        cin>>a;
                        cout<<"Ingrese el mes (1-12): ";
                        cin>>m;
                        cout<<"Ingrese el dia: ";
                        cin>>d;
                        cout<<"Ingrese la hora (08-16): ";
                        cin>>hh;
                        cout<<"Ingrese los minutos: ";
                        cin>>mm;
                        if(a<anioActual || a>(anioActual+1)){
                            cout<<"[RECHAZADO] El anio del turno debe ser el actual o maximo el siguiente.\n"<<endl;
                            continue;
                        }
                        if (a==anioActual){
                            if (m<mesActual){
                                cout<<"[RECHAZADO] No puede agendar en un mes que ya paso.\n"<<endl;
                                continue;
                            }
                            if (m==mesActual && d<diaActual){
                                cout<<"[RECHAZADO] No puede agendar en un dia del pasado.\n"<<endl;
                                continue;
                            }
                            if(m==mesActual && d==diaActual){
                                if (hh<horaActual || (hh==horaActual && mm<=minActual)){
                                    cout<<"[RECHAZADO] La hora ingresada ya transcurrio el dia de hoy.\n"<<endl;
                                    continue;
                                }
                            }
                        }

                        if (a==(anioActual + 1)){
                            if (m>mesActual || (m==mesActual && d>diaActual)) {
                                cout<<"[RECHAZADO] El turno no puede superar el limite de un anio de anticipacion.\n"<<endl;
                                continue;
                            }
                        }
                        fechaValida = true;
                    }while(!fechaValida);
                    Fecha fechaTentativa(d,m,a,hh,mm);
                    string resultadoFeriado=fechaTentativa.validarFechaYObtenerFeriado(d,m,a);
                    if(resultadoFeriado!="DISPONIBLE"){
                        cout<<"\n[RECHAZADO]: "<<resultadoFeriado<<endl;
                        system("pause");
                        break;
                    }
                    if(!fechaTentativa.esHorarioLaboral()){
                        cout<<"\n[RECHAZADO]: Fuera de horario laboral"<<endl;
                        break;
                    }
                    cin.ignore();
                    do{
                        cout<<"Ingrese la cedula del dueno: ";
                        getline(cin,cedula);
                        if(!validadorCedula.validar(cedula.c_str())){
                            cout<<"[ERROR] La cedula ingresada es invalida. Intente de nuevo.\n"<<endl;
                        }
                    }while(!validadorCedula.validar(cedula.c_str()));
                    do{
                        cout<<"Ingrese el nombre: ";
                        getline(cin,nombre);
                        if(!validador.validarNombre(nombre)){
                            cout<<"[ERROR] El nombre solo debe contener letras. Intente de nuevo.\n"<<endl;
                        }
                    }while(!validador.validarNombre(nombre));
                    do{
                        cout<<"Ingrese el apellido: ";
                        getline(cin,apellido);
                        if(!validador.validarNombre(apellido)){
                                cout<<"[ERROR] El apellido solo debe contener letras. Intente de nuevo.\n"<<endl;
                        }
                    }while(!validador.validarNombre(apellido));
                    do{
                        cout<<"Ingrese su numero de telefono: ";
                        getline(cin,telefono);
                        if(!validador.validarTelefono(telefono)){
                            cout<<"[ERROR] Formato de telefono invalido (Ej: 0987654321). Intente de nuevo.\n"<<endl;
                        }
                    }while(!validador.validarTelefono(telefono));
                    do{
                        cout<<"Ingrese su correo electronico: ";
                        getline(cin,correo);
                        if(!validador.validarCorreo(correo)){
                            cout<<"[ERROR] Formato de correo incorrecto (Ej: usuario@dominio.com). Intente de nuevo.\n"<<endl;
                        }
                    }while(!validador.validarCorreo(correo));
                    Persona propietario(nombre,apellido,cedula,telefono,correo);
                    cout<<"==== REGISTRO DE VEHICULO ===="<<endl;
                    do{
                        cout<<"Ingrese el anio del vehiculo: ";
                        cin>>yearAuto;
                        if(yearAuto<1950 || yearAuto>(anioActual+1)){
                            cout<<"[ERROR] Anio de vehiculo invalido o fuera de rango (1950 - "<<(anioActual + 1)<<").\n"<<endl;
                        }
                    }while((yearAuto<1950 || yearAuto>(anioActual+1)));
                    cin.ignore();
                    do{
                        cout<<"Ingrese la placa: ";
                        getline(cin,placa);
                        for(char &c:placa){
                            c=toupper(c);
                        }
                        if(!validador.validarPlaca(placa)){
                            cout<<"[ERROR] Formato de placa ecuatoriana invalido. Intente de nuevo.\n"<<endl;
                        }
                    }while(!validador.validarPlaca(placa));
                    int indicador = -1;
                    for (auto it = placa.rbegin(); it != placa.rend(); ++it) {
                        if (isdigit(*it)) {
                            indicador = *it - '0';
                            break;
                        }
                    }
                    if(indicador!=-1){
                        int mesCorrecto=0;
                        if (indicador == 1) mesCorrecto = 2;
                        else if (indicador == 2) mesCorrecto = 3;
                        else if (indicador == 3) mesCorrecto = 4;
                        else if (indicador == 4) mesCorrecto = 5;
                        else if (indicador == 5) mesCorrecto = 6;
                        else if (indicador== 6) mesCorrecto = 7;
                        else if (indicador == 7) mesCorrecto = 8;
                        else if (indicador == 8) mesCorrecto = 9;
                        else if (indicador == 9) mesCorrecto = 10;
                        else if (indicador == 0) mesCorrecto = 11;
                        if(m!=mesCorrecto && m!=12) {
                            cout<<"\n[RECHAZADO] Calendarizacion: Los vehiculos con placa terminada en " 
                                      <<indicador<<" deben matricularse obligatoriamente en el mes " 
                                      <<mesCorrecto<<" o en Diciembre (Rezagados)."<<endl;
                            system("pause");
                            break;
                        }
                    }
                    do{
                        cout<<"Ingrese el color: ";
                        getline(cin,color);
                        if(!validador.validarNombre(color)){
                            cout<<"[ERROR] El color solo debe contener letras. Intente de nuevo.\n"<<endl;
                        }
                    }while(!validador.validarNombre(color));
                    Vehiculo carro(color,placa,yearAuto);
                    int idTurno=sistemaTurnos.generarSiguienteID();
                    cout<<"\n[PROCESANDO] Asignando ID de turno secuencial..."<<endl;
                    Turno* nuevoTurno=new Turno(propietario,carro,fechaTentativa,idTurno,true);
                    sistemaTurnos.insertar(nuevoTurno);
                    cout<<"[EXITO] Registro completado. ID de Turno Asignado: " << idTurno << endl;
                    system("pause");
                    break;
                }
                case 2:{
                    sistemaTurnos.mostrarLista();
                    system("pause");
                    break;
                }
                case 3:{
                    int idBuscar=0,d=0,m=0,a=0,nh=0,nm=0;
                    cout<<"\n=== MODIFICAR TURNO POR ID ==="<<endl;
                    cout<<"Ingrese el ID del turno de 4 digitos que desea reprogramar: ";
                    cin>>idBuscar;
                    cout<<"\n--- Ingrese los datos de la NUEVA cita ---"<<endl;
                    cout<<"Nuevo Anio: ";
                    cin>>a;
                    cout<<"Nuevo Mes (1-12): ";
                    cin>>m;
                    cout<<"Nuevo Dia: ";
                    cin>>d;
                    cout<<"Nueva Hora (08-16): ";
                    cin>>nh;
                    cout<<"Nuevos Minutos: ";
                    cin>>nm;
                    Fecha nuevaFecha(d,m,a,nh,nm);
                    sistemaTurnos.modificar(idBuscar,nuevaFecha);
                    system("pause");
                    break;
                }
                case 4:{
                    int idBuscar=0;
                    cout<<"\n=== ELIMINAR / CANCELAR TURNO POR ID ==="<<endl;
                    cout<<"Ingrese el ID del turno de 4 digitos a eliminar: ";
                    cin>>idBuscar;
                    sistemaTurnos.eliminarPorID(idBuscar);
                    system("pause");
                    break;
                }
                case 5:{
                    ejecutado=false;
                    break;
                }
                default:
                    break;
            }
        }
    }
    system("cls");
    cout<<"============================================"<< endl;
    cout<<"        Sistema Cerrado de forma Segura.    "<< endl;
    cout<<"============================================"<<endl;
    return 0;
}