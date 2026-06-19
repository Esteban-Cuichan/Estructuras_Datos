#ifndef NOMINMAX
    #define NOMINMAX
#endif
#define _RPCNDR_H_
#define DONOT_DEFINE_BYTE
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ListaDCE.h"
#include "TablaHash.h"
using namespace std;
ListaDCE::ListaDCE(){
    this->cabeza=nullptr;
    this->cola=nullptr;
    cargarListaDesdeArchivo();
}
ListaDCE::~ListaDCE(){
    if(cabeza!=nullptr){
        Nodo* actual=cabeza;
        Nodo* siguienteNodo=nullptr;
        do{
            siguienteNodo=actual->getSiguiente();
            if(actual->getTurno()!=nullptr){
                delete actual->getTurno();
            }
            delete actual;
            actual=siguienteNodo;
        }while(actual!=cabeza);
    }
}
ListaDCE::ListaDCE(Nodo* cabeza,Nodo* cola){
    this->cabeza=cabeza;
    this->cola=cola;
}
Nodo* ListaDCE::getCabeza(){
    return cabeza;
}
Nodo* ListaDCE::getCola(){
    return cola;
}
void ListaDCE::setCabeza(Nodo*){
    this->cabeza=cabeza;
}
void ListaDCE::setCola(Nodo*){
    this->cola=cola;
}
void ListaDCE::insertar(Turno* nuevoTurno){
    if(nuevoTurno==nullptr)return;
    Fecha fechaNueva=nuevoTurno->getFecha();
    string placaNueva=nuevoTurno->getVehiculo().getPlaca();
    if(!fechaNueva.esHorarioLaboral()){
        cout<<"Error: El horario ingresado ("<<fechaNueva.getHora()<<":"<<fechaNueva.getMinutos()<<") esta fuera del horario de atencion (08:00 a 16:30)."<<endl;
        delete nuevoTurno;
        return;
    }
    if(cabeza!=nullptr){
        Nodo* actual=cabeza;
        do{
            Turno* t=actual->getTurno();
            if(t!=nullptr){
                if(t->getVehiculo().getPlaca()==placaNueva){
                    cout<<"Error: El vehiculo con placa: "<<placaNueva<<" ya tiene un turno asignado en el sistema!"<<endl;
                    delete nuevoTurno;
                    return;
                }
                Fecha fechaExistente=t->getFecha();
                if(fechaExistente.getYear()==fechaNueva.getYear()&&fechaExistente.getMes()==fechaNueva.getMes()&&fechaExistente.getDia()==fechaNueva.getDia()&&fechaExistente.getHora()==fechaNueva.getHora()&&fechaExistente.getMinutos()==fechaNueva.getMinutos()){
                    cout<<"Error: Ya existe un turno agentado para el "<<fechaNueva.fechaFormateada()<<". Intente con otra hora."<<endl;
                    delete nuevoTurno;
                    return;
                }
            }
            actual=actual->getSiguiente();
        }while(actual!=cabeza);
    }
    Nodo* nuevoNodo=new Nodo();
    nuevoNodo->setTurno(nuevoTurno);
    if(cabeza==nullptr){
        cabeza=nuevoNodo;
        cola=nuevoNodo;
        cabeza->setSiguiente(cabeza);
        cabeza->setAnterior(cabeza);
    }else{
        cola->setSiguiente(nuevoNodo);
        nuevoNodo->setAnterior(cola);
        nuevoNodo->setSiguiente(cabeza);
        cabeza->setAnterior(nuevoNodo);
        cola=nuevoNodo;
    }
    indiceHash.insertar(nuevoTurno);
    guardarListaEnArchivo();
    cout<<"Turno agendado correctamente!"<<endl;
}
bool ListaDCE::modificar(int idBuscado,Fecha nuevaFecha){
    Nodo* nodoEncontrado=buscarPorID(idBuscado);
    if(nodoEncontrado==nullptr){
        cout<<"No existe ningun turno con el ID: "<<idBuscado<<endl;
        return false;
    }
    string resultadoFeriado=nuevaFecha.validarFechaYObtenerFeriado(nuevaFecha.getDia(),nuevaFecha.getMes(),nuevaFecha.getYear());
    if(resultadoFeriado!="DISPONIBLE"){
        cout<<"Erro: No se puede reprogramar. Motivo: "<<resultadoFeriado<<endl;
        return false;
    }
    if(!nuevaFecha.esHorarioLaboral()){
        cout<<"Error: El nuevo Horario ("<<nuevaFecha.getHora()<<":"<<nuevaFecha.getMinutos()<<") esta fuera del horario de atencion (08:00 a 16:30)."<<endl;
        return false;
    }
    if(cabeza!=nullptr){
        Nodo* actual=cabeza;
        do{
            if(actual!=nodoEncontrado){
                Turno* t=actual->getTurno();
                if(t!=nullptr){
                    Fecha fechaExistente=t->getFecha();
                    if(fechaExistente.getYear()==nuevaFecha.getYear()&&fechaExistente.getMes()==nuevaFecha.getMes()&&fechaExistente.getDia()==nuevaFecha.getDia()&&fechaExistente.getHora()==nuevaFecha.getHora()&&fechaExistente.getMinutos()==nuevaFecha.getMinutos()){
                        cout<<"Error: Ya existe un turno agentado para el "<<nuevaFecha.fechaFormateada()<<". Intente con otra hora."<<endl;
                        return false;
                    }
                }
            }
            actual=actual->getSiguiente();
        }while(actual!=cabeza);
    }
    nodoEncontrado->getTurno()->setFecha(nuevaFecha);
    guardarListaEnArchivo();
    cout<<"El turno ID  "<<idBuscado<<" ha saido modificado correctamente a la fecha: "<<nuevaFecha.fechaFormateada()<<endl;
    return true;
}
Nodo* ListaDCE::buscarPorID(int idBuscado){
    Turno* tEncontrado=indiceHash.buscar(idBuscado);
    if(tEncontrado==nullptr)return nullptr;
    if(cabeza==nullptr)return nullptr;
    Nodo* actual=cabeza;
    do{
        if(actual->getTurno()==tEncontrado)return actual;
        actual=actual->getSiguiente();
    }while(actual!=cabeza);
    return nullptr;
}
bool ListaDCE::eliminarPorID(int idBuscado){
    if(cabeza==nullptr)return false;
    Nodo* nodoEliminar=buscarPorID(idBuscado);
    if(nodoEliminar==nullptr){
        cout<<"No se puede eliminar: No se encontro el ID: "<<idBuscado<<endl;
        return false;
    }
    indiceHash.eliminar(idBuscado);
    if(cabeza==cola&&cabeza==nodoEliminar){
        cabeza=nullptr;
        cola=nullptr;
    }else{
        Nodo* previo=nodoEliminar->getAnterior();
        Nodo* proximo=nodoEliminar->getSiguiente();
        previo->setSiguiente(proximo);
        proximo->setAnterior(previo);
        if(nodoEliminar==cabeza){cabeza=proximo;}
        if(nodoEliminar==cola){cola=previo;}
    }
    if(nodoEliminar->getTurno()!=nullptr){
        delete nodoEliminar->getTurno();
    }
    delete nodoEliminar;
    guardarListaEnArchivo();
    cout<<"Turno ID: "<<idBuscado<<" eliminado correctamente!"<<endl;
    return true;
}
void ListaDCE::mostrarLista(){
    SetConsoleOutputCP(CP_UTF8);
    if(cabeza==nullptr){
        cout<<"\n[INFO] No hay turnos registrados en el sistema!"<<endl;
        return;
    }
    Nodo* actual=cabeza;
    cout<<"\n====================================================================="<<endl;
    cout<<"                     LISTADO DE TURNOS REGISTRADOS                   "<<endl;
    cout<<"====================================================================="<<endl;
    do{
        Turno* t=actual->getTurno();
        if(t!=nullptr){
            Persona p=t->getUsuario();
            Fecha f=t->getFecha();
            cout<<"  ID TURNO : "<<t->getIDturno()<<" | Estado: "<<(t->getEstado()?"Activo":"Cancelado")<<endl;
            cout<<"  DUEÑO    : "<<p.getNombre()<<" "<<p.getApellido()<<" (C.I: "<<p.getCI()<<")"<<endl;
            cout<<"  CONTACTO : Tel: "<<p.getTelefono()<<" | Correo: "<<p.getCorreo()<<endl;
            cout<<"  FECHA    : "<<f.fechaFormateada()<<endl;
            cout<<"---------------------------------------------------------------------"<<endl;
        }
        actual=actual->getSiguiente();
    }while(actual!=cabeza);
    cout<<"=====================================================================\n"<<endl;
}
void ListaDCE::guardarListaEnArchivo(){
    ofstream archivoOut("turnos.txt",ios::trunc);
    if(!archivoOut.is_open())return;
    if(cabeza!=nullptr){
        Nodo* actual=cabeza;
        do{
            Turno* t=actual->getTurno();
            if(t!=nullptr){
                archivoOut<<t->getIDturno()<<","<<t->getUsuario().getNombre()<<","<<t->getUsuario().getApellido()<<","<<t->getUsuario().getCI()<<","<<t->getUsuario().getTelefono()<<","<<t->getUsuario().getCorreo()<<","<<t->getVehiculo().getColor()<<","<<t->getVehiculo().getPlaca()<<","<<t->getVehiculo().getAnio()<<","<<t->getFecha().getDia()<<","<<t->getFecha().getMes()<<","<<t->getFecha().getYear()<<","<<t->getFecha().getHora()<<","<<t->getFecha().getMinutos()<<","<<t->getEstado()<<"\n";
            }
            actual=actual->getSiguiente();
        }while(actual!=cabeza);
    }
    archivoOut.close();
}
int ListaDCE::generarSiguienteID(){
    if(cabeza==nullptr){
        return 1001;
    }
    int idMaximo=1001;
    Nodo* actual=cabeza;
    do{
        Turno* t=actual->getTurno();
        if(t!=nullptr&&t->getIDturno()>idMaximo){
            idMaximo=t->getIDturno();
        }
        actual=actual->getSiguiente();
    }while(actual!=cabeza);
    return idMaximo+1;
}
int reducirAUnDigito(int numero){
    if(numero==0)return 0;
    int resultado=numero%9;
    return(resultado==0)?9:resultado;
}
void ListaDCE::cifrarPlacas(){
    SetConsoleOutputCP(CP_UTF8);
    if(cabeza==nullptr){
        cout<<"\n[INFO] No hay turnos en el sistema para cifrar placas."<<endl;
        return;
    }
    cout<<"====================================================================="<<endl;
    cout<<"                     CIFRADO DE PLACAS POR TURNO                     "<<endl;
    Nodo* actual=cabeza;
    do{
        Turno* t=actual->getTurno();
        if(t!=nullptr){
            string placa=t->getVehiculo().getPlaca();
            int sumaLetras=0;
            int sumaNumeros=0;
            for(char c:placa){
                if(isalpha(c)){
                    if(c>='A'&&c<='Z'){c=c+32;}
                    sumaLetras+=(int)c;
                }else if(isdigit(c)){
                    sumaNumeros+=(c-'0');
                }
            }
            int digitoLetras=reducirAUnDigito(sumaLetras);
            int digitoNumeros=reducirAUnDigito(sumaNumeros);
            cout<<"  ID TURNO: "<<t->getIDturno()<<" | Placa Original: "<<placa<<" | Placa Cifrada: "<<digitoLetras<<digitoNumeros<<endl;
        }
        actual=actual->getSiguiente();
    }while(actual!=cabeza);
    cout<<"=====================================================================\n"<<endl;
}
void ListaDCE::cargarListaDesdeArchivo(){
    ifstream archivo("turnos.txt");
    if(!archivo.is_open()){
        return;
    }
    string linea;
    while(getline(archivo,linea)){
        if(linea.empty())continue;
        stringstream ss(linea);
        string idStr,nombre,apellido,ci,telefono,correo;
        string placa,color,anioVehiculoStr;
        string diaStr,mesStr,anioFechaStr,horaStr,minStr,estadoStr;
        getline(ss,idStr,',');
        getline(ss,nombre,',');
        getline(ss,apellido,',');
        getline(ss,ci,',');
        getline(ss,telefono,',');
        getline(ss,correo,',');
        getline(ss,color,',');
        getline(ss,placa,',');
        getline(ss,anioVehiculoStr,',');
        getline(ss,diaStr,',');
        getline(ss,mesStr,',');
        getline(ss,anioFechaStr,',');
        getline(ss,horaStr,',');
        getline(ss,minStr,',');
        getline(ss,estadoStr,'\n');
        try{
            if(idStr.empty()||diaStr.empty()||mesStr.empty()||anioFechaStr.empty()){
                continue;
            }
            int id=stoi(idStr);
            int anioV=stoi(anioVehiculoStr);
            int d=stoi(diaStr);
            int m=stoi(mesStr);
            int a=stoi(anioFechaStr);
            int hh=stoi(horaStr);
            int mm=stoi(minStr);
            bool est=(estadoStr=="1");
            Persona prop(nombre,apellido,ci,telefono,correo);
            Vehiculo car(color,placa,anioV); 
            Fecha fec;
            fec.setDia(d);
            fec.setMes(m);
            fec.setYear(a);
            fec.setHora(hh);
            fec.setMinutos(mm);
            Turno* turnoCargado=new Turno(prop,car,fec,id,est);
            Nodo* nuevoNodo=new Nodo();
            nuevoNodo->setTurno(turnoCargado);
            if(cabeza==nullptr){
                cabeza=nuevoNodo;
                cola=nuevoNodo;
                cabeza->setSiguiente(cabeza);
                cabeza->setAnterior(cabeza);
            }else{
                cola->setSiguiente(nuevoNodo);
                nuevoNodo->setAnterior(cola);
                nuevoNodo->setSiguiente(cabeza);
                cabeza->setAnterior(nuevoNodo);
                cola=nuevoNodo;
            }
            // LA CORRECCIÓN CLAVE: Mantiene indexado el Hash con el archivo guardado
            indiceHash.insertar(turnoCargado);
        }catch(const invalid_argument& e){
            continue;
        }
    }
    archivo.close();
}

void ListaDCE::guardarOrdenamiento(){
    ofstream archivoOut("turnosOrdenados.txt",ios::trunc);
    if(!archivoOut.is_open())return;
    if(cabeza!=nullptr){
        Nodo* actual=cabeza;
        do{
            Turno* t=actual->getTurno();
            if(t!=nullptr){
                archivoOut<<t->getIDturno()<<","<<t->getUsuario().getNombre()<<","<<t->getUsuario().getApellido()<<","<<t->getUsuario().getCI()<<","<<t->getUsuario().getTelefono()<<","<<t->getUsuario().getCorreo()<<","<<t->getVehiculo().getColor()<<","<<t->getVehiculo().getPlaca()<<","<<t->getVehiculo().getAnio()<<","<<t->getFecha().getDia()<<","<<t->getFecha().getMes()<<","<<t->getFecha().getYear()<<","<<t->getFecha().getHora()<<","<<t->getFecha().getMinutos()<<","<<t->getEstado()<<"\n";
            }
            actual=actual->getSiguiente();
        }while(actual!=cabeza);
    }
    archivoOut.close();
}