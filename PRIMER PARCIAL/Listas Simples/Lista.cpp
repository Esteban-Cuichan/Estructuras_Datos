#include "Lista.h"
#include "Operaciones.h"
#include <iostream>

Lista::Lista(){
    this->cabeza=nullptr;
    this->cola=nullptr;
}

void Lista::insertar(string cedula,string nombre){
    Nodo* nuevo=new Nodo(cedula,nombre,nullptr);
    if(cabeza==nullptr){
        cabeza=nuevo;
        cola=nuevo;
    }else{
        nuevo->setSiguiente(cabeza);
        cabeza=nuevo;
    }
}

void Lista::imprimir(){
    if(cabeza==nullptr){
        cout<<"No se ha podido encontrar ningun dato ingresado!"<<endl;
        cout<<"LISTA VACÍA!"<<endl;
    }else{
        Nodo* aux=cabeza;
        cout<<"Lista de registros:"<<endl;
        while(aux!=nullptr){
            cout<<"Cedula: "<<aux->getCedula()<<"| Nombre: "<<aux->getNombre()<<endl;
            aux=aux->getSiguiente();
        }
    }
}

Nodo* Lista::buscar(string cedula){
    string cedula_buscar=cedula;
    Nodo* aux=cabeza;
    while(aux!=nullptr){
        if(aux->getCedula()==cedula_buscar){
            return aux;
        }else{
            aux=aux->getSiguiente();
        }
    }
    return nullptr;
}

void Lista::eliminar(string cedula){
    Nodo* actual=cabeza;
    Nodo* anterior=nullptr;
    while(actual!=nullptr){
        if(actual->getCedula()==cedula){
            if(actual==cabeza){
                cabeza=cabeza->getSiguiente();
            }else{
                anterior->setSiguiente(actual->getSiguiente());
            }
            if(actual==cola){
                cola=anterior;
            }
            delete actual;
            return;
        }
        anterior=actual;
        actual=actual->getSiguiente();
    }
    cout<<"No se encontro ningun dato que coincida!"<<endl;
}

void Lista::setCabeza(Nodo* cabeza){
    this->cabeza=cabeza;
}

void Lista::setCola(Nodo* cola){
    this->cola=cola;
}

Nodo* Lista::getCabeza(){
    return this->cabeza;
}

Nodo* Lista::getCola(){
    return this->cola;
}