#include "Nodo.h"

Nodo::Nodo(int valor) : valor(valor), color(ROJO), padre(nullptr), izquierdo(nullptr), derecho(nullptr) {}

Nodo::~Nodo() {}

int Nodo::getValor() const { return valor; }
Color Nodo::getColor() const { return color; }
Nodo* Nodo::getPadre() const { return padre; }
Nodo*& Nodo::getIzquierdo()
{
    return izquierdo;
}

Nodo*& Nodo::getDerecho()
{
    return derecho;
}

void Nodo::setValor(int valor) { this->valor = valor; }
void Nodo::setColor(Color color) { this->color = color; }
void Nodo::setPadre(Nodo* padre) { this->padre = padre; }
void Nodo::setIzquierdo(Nodo* izquierdo) { this->izquierdo = izquierdo; }
void Nodo::setDerecho(Nodo* derecho) { this->derecho = derecho; }

bool Nodo::esRojo() const { return color == ROJO; }
bool Nodo::esNegro() const { return color == NEGRO; }

bool Nodo::esHijoIzquierdo() const
{
    return padre != nullptr && padre->getIzquierdo() == this;
}

bool Nodo::esHijoDerecho() const
{
    return padre != nullptr && padre->getDerecho() == this;
}

Nodo* Nodo::getHermano() const
{
    if (padre == nullptr)
        return nullptr;
    return esHijoIzquierdo() ? padre->getDerecho() : padre->getIzquierdo();
}

Nodo* Nodo::getTio() const
{
    if (padre == nullptr || padre->getPadre() == nullptr)
        return nullptr;
    return padre->esHijoIzquierdo() ? padre->getPadre()->getDerecho() : padre->getPadre()->getIzquierdo();
}

Nodo* Nodo::getAbuelo() const
{
    return padre != nullptr ? padre->getPadre() : nullptr;
}