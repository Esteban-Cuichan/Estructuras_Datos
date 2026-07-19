#include "Lista.h"
#include <iostream>

Lista::Lista() : cabeza(nullptr) {}

void Lista::insertar(const string& esp, const string& eng) {
    Nodo* nuevo=new Nodo(esp, eng);
    if (cabeza==nullptr) {
        cabeza=nuevo;
    } else {
        Nodo* actual=cabeza;
        while (actual->getSiguiente()!=nullptr) {
            actual=actual->getSiguiente();
        }
        actual->setSiguiente(nuevo);
    }
}

void Lista::mostrar() const {
    Nodo* actual=cabeza;
    if (actual==nullptr) {
        cout<<"Historial vacío."<<endl;
        return;
    }
    int index=1;
    while (actual!=nullptr) {
        cout<<index<<". "<<actual->getPalabraEspanol()<<" -> "
            <<actual->getPalabraIngles()<<endl;
        actual=actual->getSiguiente();
        ++index;
    }
}

Nodo* Lista::buscar(const string& esp) const {
    Nodo* actual=cabeza;
    while (actual!=nullptr) {
        if (actual->getPalabraEspanol()==esp) {
            return actual;
        }
        actual=actual->getSiguiente();
    }
    return nullptr;
}

bool Lista::eliminar(const string& esp) {
    Nodo* actual=cabeza;
    Nodo* anterior=nullptr;
    while (actual!=nullptr) {
        if (actual->getPalabraEspanol()==esp) {
            if (anterior==nullptr) {
                cabeza=actual->getSiguiente();
            } else {
                anterior->setSiguiente(actual->getSiguiente());
            }
            delete actual;
            return true;
        }
        anterior=actual;
        actual=actual->getSiguiente();
    }
    return false;
}

void Lista::liberarMemoria() {
    Nodo* actual=cabeza;
    while (actual!=nullptr) {
        Nodo* siguiente=actual->getSiguiente();
        delete actual;
        actual=siguiente;
    }
    cabeza=nullptr;
}

Lista::~Lista() {
    liberarMemoria();
}