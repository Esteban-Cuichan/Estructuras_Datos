#include "ListaDCE.h"

ListaDCE::ListaDCE(){
    this->cabeza=nullptr;
    this->cola=nullptr;
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

