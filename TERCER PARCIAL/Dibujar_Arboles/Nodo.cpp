#include "Nodo.h"

Nodo::Nodo(int val){
    valor = val;
    izquierdo = nullptr;
    derecho = nullptr;
}

Nodo::~Nodo(){
    if (izquierdo != nullptr) delete izquierdo;
    if (derecho != nullptr) delete derecho;
}

int Nodo::getValor() const{return valor;}

void Nodo::setValor(int val){valor = val;}

Nodo* Nodo::getIzquierdo() const{return izquierdo;}

void Nodo::setIzquierdo(Nodo* izq){izquierdo = izq;}

Nodo* Nodo::getDerecho() const{return derecho;}

void Nodo::setDerecho(Nodo* der){derecho = der;}