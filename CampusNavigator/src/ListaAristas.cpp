#include "../include/ListaAristas.h"
#include "../include/ElementoArista.h"
#include "../include/Arista.h"
#include "../include/Nodo.h"

ListaAristas::ListaAristas()
    : primero(nullptr),
      ultimo(nullptr),
      cantidad(0)
{
}

ListaAristas::~ListaAristas(){
    vaciar();
}

void ListaAristas::insertar(Nodo* destino,double distancia,double tiempo){
    Arista* nuevaArista = new Arista(
        destino,
        distancia,
        tiempo
    );

    insertar(nuevaArista);
}

void ListaAristas::insertar(Arista* arista){
    if (arista == nullptr){
        return;
    }

    ElementoArista* nuevoElemento =
        new ElementoArista(arista);

    if (primero == nullptr){
        primero = nuevoElemento;
        ultimo = nuevoElemento;
    }else{
        ultimo->setSiguiente(nuevoElemento);
        ultimo = nuevoElemento;
    }

    cantidad++;
}

ElementoArista* ListaAristas::getPrimero() const{
    return primero;
}

ElementoArista* ListaAristas::getUltimo() const{
    return ultimo;
}

int ListaAristas::getCantidad() const{
    return cantidad;
}

bool ListaAristas::estaVacia() const{
    return primero == nullptr;
}

void ListaAristas::vaciar(){
    ElementoArista* actual = primero;

    while (actual != nullptr){
        ElementoArista* siguiente =
            actual->getSiguiente();

        delete actual;
        actual = siguiente;
    }

    primero = nullptr;
    ultimo = nullptr;
    cantidad = 0;
}