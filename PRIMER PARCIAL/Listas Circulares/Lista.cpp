#include "Lista.h"
#include "Operaciones.h"
#include <iostream>
using namespace std;

Lista::Lista(){
    this->cabeza=nullptr;
    this->cola=nullptr;
}

void Lista::insertar(string cedula,string nombre){
    Nodo* nuevo=new Nodo(cedula,nombre,nullptr,nullptr);
    if(cabeza==nullptr){
        cabeza=nuevo;
        cola=nuevo;
        cola->setSiguiente(cabeza);
    }else{
        nuevo->setSiguiente(cabeza);
        cabeza=nuevo;
        cola->setSiguiente(cabeza);
    }
}

void Lista::imprimir(){
    if(cabeza==nullptr){
        cout<<"No se ha podido encontrar ningun dato ingresado!"<<endl;
        cout<<"LISTA VACÍA!"<<endl;
    }else{
        Nodo* aux=cabeza;
        cout<<"Lista de registros:"<<endl;
        do{
            cout<<"Cedula: "<<aux->getCedula()<<"| Nombre: "<<aux->getNombre()<<endl;
            aux=aux->getSiguiente();
        }while(aux!=cabeza);
    }
}

Nodo* Lista::buscar(string cedula){
    string cedula_buscar=cedula;
    Nodo* aux=cabeza;
    do{
        if(aux->getCedula()==cedula_buscar){
            return aux;
        }
        aux=aux->getSiguiente();
    }while(aux!=cabeza);
    return nullptr;
}

void Lista::eliminar(string cedula){
    if(cabeza==nullptr) return;
    Nodo* actual=cabeza;
    Nodo* anterior=cola;
    bool encontrado=false;
    do{
        if(actual->getCedula()==cedula){
            encontrado=true;
            if(actual==cabeza && actual==cola){
                cabeza==nullptr;
                cola==nullptr;
                if(cabeza!=nullptr){
                    cabeza->setAnterior(nullptr);
                }
            }else if(actual==cabeza){
                cabeza=cabeza->getSiguiente();
                cola->setSiguiente(cabeza);
            }else{
                anterior->setSiguiente(actual->getSiguiente());
                if(actual==cola){
                    cola=anterior;
                }
            }
            delete actual;
            cout<<"Registro eliminado con exito"<<endl;
            return;
        }
        anterior=actual;
        actual=actual->getSiguiente();
    }while(actual!=cabeza);

    if(!encontrado){
        cout<<"No se encontro ningun dato que coincida!"<<endl;
    } 
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