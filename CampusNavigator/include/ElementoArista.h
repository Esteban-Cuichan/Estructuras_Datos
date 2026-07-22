#ifndef ELEMENTO_ARISTA_H
#define ELEMENTO_ARISTA_H

class Arista;

class ElementoArista {
private:
    Arista* arista;
    ElementoArista* siguiente;

public:
    ElementoArista(Arista* arista);
    ~ElementoArista();

    Arista* getArista() const;
    ElementoArista* getSiguiente() const;

    void setArista(Arista* arista);
    void setSiguiente(ElementoArista* siguiente);
};

#endif