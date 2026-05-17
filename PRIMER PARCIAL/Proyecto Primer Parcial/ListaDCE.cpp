#include "ListaDCE.h"
#include <fstream>

ListaDCE::ListaDCE(){
    this->cabeza=nullptr;
    this->cola=nullptr;
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

ListaDCE::ListaDCE(Nodo* cabeza, Nodo* cola){
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
    if(nuevoTurno==nullptr) return;
    Fecha fechaNueva=nuevoTurno->getFecha();
    if(!fechaNueva.esHorarioLaboral()){
        cout<<"Error: El horario ingresado ("
        <<fechaNueva.getHora()<<":"<<fechaNueva.getMinutos()
        <<") esta fuera del horario de atencion (08:00 a 16:30)."<<endl;
        delete nuevoTurno;
        return;
    }
    if(cabeza!=nullptr) {
        Nodo* actual=cabeza;
        do{
            Turno* t=actual->getTurno();
            if(t!=nullptr){
                Fecha fechaExistente=t->getFecha();
                if(fechaExistente.getYear() == fechaNueva.getYear() &&
                    fechaExistente.getMes() == fechaNueva.getMes() &&
                    fechaExistente.getDia() == fechaNueva.getDia() &&
                    fechaExistente.getHora() == fechaNueva.getHora() &&
                    fechaExistente.getMinutos() == fechaNueva.getMinutos()){
                        cout<<"Error: Ya existe un turno agentado para el "
                        <<fechaNueva.fechaFormateada()<<". Intente con otra hora."<<endl;
                        delete nuevoTurno;
                        return;
                    }
            }
            actual=actual->getSiguiente();
        }while(actual!=nullptr);
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
    guardarListaEnArchivo();
    cout<<"Turno agendado correctamente!"<<endl;
}

Nodo* ListaDCE::buscarPorFecha(int dia,int mes,int year){
    if(cabeza==nullptr) return nullptr;
    Nodo* actual=cabeza;
    do{
        Turno* t=actual->getTurno();
        Fecha f=t->getFecha();
        if(f.getDia()==dia && f.getMes()==mes && f.getYear()==year){
            return actual;
        }
        actual=actual->getSiguiente();
    }while(actual!=cabeza);
    return nullptr;
}

bool ListaDCE::modificar(int dia, int mes, int year,Fecha nuevaFecha){
    Nodo* nodoEncontrado=buscarPorFecha(dia,mes,year);
    if(nodoEncontrado==nullptr){
        cout<<"No existe ningun turno registrado el: "<<dia<<" / "<<mes<<" / "<<year<<endl;
        return false;
    }
    if(!nuevaFecha.esHorarioLaboral()){
        cout<<"Error: El nuevo Horario ("
            <<nuevaFecha.getHora()<<":"<<nuevaFecha.getMinutos()
            <<") esta fuera del horario de atencion (08:00 a 16:30)."<<endl;
        return false;
    }
    if(cabeza!=nullptr) {
        Nodo* actual=cabeza;
        do{
            Turno* t=actual->getTurno();
            if(t!=nullptr){
                Fecha fechaExistente=t->getFecha();
                if(fechaExistente.getYear() == nuevaFecha.getYear() &&
                    fechaExistente.getMes() == nuevaFecha.getMes() &&
                    fechaExistente.getDia() == nuevaFecha.getDia() &&
                    fechaExistente.getHora() == nuevaFecha.getHora() &&
                    fechaExistente.getMinutos() == nuevaFecha.getMinutos()){
                        cout<<"Error: Ya existe un turno agentado para el "
                        <<nuevaFecha.fechaFormateada()<<". Intente con otra hora."<<endl;
                        return false;
                    }
            }
            actual=actual->getSiguiente();
        }while(actual!=cabeza);
    }
    nodoEncontrado->getTurno()->setFecha(nuevaFecha);
    guardarListaEnArchivo();
    cout<<"El turno ha sido modificado correctamente a la fecha: "<<nuevaFecha.fechaFormateada()<<endl;
    return true;
}

bool ListaDCE::eliminarPorFecha(int dia,int mes,int year){
    if(cabeza==nullptr) return false;
    Nodo* nodoEliminar=buscarPorFecha(dia,mes,year);
    if(nodoEliminar==nullptr){
        cout<<"No se puede eliminar: No se encontro un turno con esa fecha!"<<endl;
        return false;
    }

    if(cabeza==cola && cabeza==nodoEliminar){
        cabeza==nullptr;
        cola==nullptr;
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
    cout<<"Turno cancelado y eliminado del sistema."<<endl;
    return true;
}

void ListaDCE::mostrarLista(){
    if(cabeza==nullptr){
        cout<<"No hay turnos registrados en el sistema!"<<endl;
        return;
    }
    Nodo* actual=cabeza;
    cout<<"\n======== TURNOS REGISTRADOS ========"<<endl;
    do{
        Turno* t=actual->getTurno();
        cout<<"ID: "<<t->getIDturno()
        <<" | Fecha: "<<t->getFecha().getDia()<<"/"<<t->getFecha().getMes()
        <<" | Placa: "<<t->getVehiculo().getPlaca()<<endl;
        actual=actual->getSiguiente();
    }while(actual!=cabeza);
    cout<<"=====================================\n"<<endl;
}

void ListaDCE::guardarListaEnArchivo() {
    ofstream archivoOut("turnos.txt",ios::trunc);
    if (!archivoOut.is_open()) return;

    if (cabeza != nullptr) {
        Nodo* actual = cabeza;
        do {
            Turno* t = actual->getTurno();
            archivoOut << t->getIDturno() << ","
                       << t->getUsuario().getCI() << ","
                       << t->getVehiculo().getPlaca() << ","
                       << t->getFecha().getDia() << ","
                       << t->getFecha().getMes() << ","
                       << t->getFecha().getYear() << ","
                       << t->getEstado() << "\n";
            actual = actual->getSiguiente();
        } while (actual != cabeza);
    }
    archivoOut.close();
}