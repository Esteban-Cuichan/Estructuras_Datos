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
#include <windows.h>
#include <conio.h>
#define DONOT_DEFINE_BYTE
#include "ListaDCE.h"
#include "GestorMenu.h"
using namespace std;
int main(){
    ListaDCE sistemaTurnos;
    GestorMenu gestor(sistemaTurnos);
    int seleccionActiva=1;
    int totalOpciones=7;
    bool ejecutado=true;
    while(ejecutado){
        system("cls");
        gestor.pintarMenuPrincipal(seleccionActiva);
        bool accionDetectada=false;
        while(!accionDetectada){
            if(_kbhit()){
                int tecla=_getch();
                if(tecla==0||tecla==224){
                    tecla=_getch();
                    if(tecla==72){
                        seleccionActiva=(seleccionActiva==1)?totalOpciones:seleccionActiva-1;
                        accionDetectada=true;
                    }else if(tecla==80){
                        seleccionActiva=(seleccionActiva==totalOpciones)?1:seleccionActiva+1;
                        accionDetectada=true;
                    }
                }else if(tecla==13){
                    accionDetectada=true;
                    system("cls");
                    switch(seleccionActiva){
                        case 1:gestor.ejecutarAgendamiento();break;
                        case 2:sistemaTurnos.mostrarLista();system("pause");break;
                        case 3:{
                            int idBuscar=0,d=0,m=0,a=0,nh=0,nm=0;
                            cout<<"\n=== MODIFICAR TURNO POR ID ==="<<endl;
                            cout<<"Ingrese el ID de 4 digitos a reprogramar: ";cin>>idBuscar;
                            cout<<"Nuevo Anio: ";cin>>a;
                            cout<<"Nuevo Mes: ";cin>>m;
                            cout<<"Nuevo Dia: ";cin>>d;
                            cout<<"Nueva Hora: ";cin>>nh;
                            cout<<"Nuevos Minutos: ";cin>>nm;
                            Fecha nuevaFecha(d,m,a,nh,nm);
                            sistemaTurnos.modificar(idBuscar,nuevaFecha);
                            system("pause");
                            break;
                        }
                        case 4:{
                            int idBuscar=0;
                            cout<<"\n=== ELIMINAR / CANCELAR TURNO POR ID ==="<<endl;
                            cout<<"Ingrese el ID a eliminar: ";cin>>idBuscar;
                            sistemaTurnos.eliminarPorID(idBuscar);
                            system("pause");
                            break;
                        }
                        case 5:gestor.ejecutarOrdenamientoTemplates();break;
                        case 6:sistemaTurnos.cifrarPlacas();system("pause");break;
                        case 7:ejecutado=false;break;
                        default:break;
                    }
                }
            }
            Sleep(30);
        }
    }
    system("cls");
    cout<<"============================================"<<endl;
    cout<<"        Sistema Cerrado de forma Segura.    "<<endl;
    cout<<"============================================"<<endl;
    return 0;
}