#ifndef NODO_H
#define NODO_H

#include "Color.h"

class Nodo
{
private:
    int valor;
    Color color;
    Nodo* padre;
    Nodo* izquierdo;
    Nodo* derecho;

public:
    // Constructor
    Nodo(int valor);

    // Destructor
    ~Nodo();

    // Getters
    int getValor() const;
    Color getColor() const;
    Nodo* getPadre() const;
    Nodo*& getIzquierdo();
    Nodo*& getDerecho();

    // Setters
    void setValor(int valor);
    void setColor(Color color);
    void setPadre(Nodo* padre);
    void setIzquierdo(Nodo* izquierdo);
    void setDerecho(Nodo* derecho);

    // Métodos auxiliares
    bool esRojo() const;
    bool esNegro() const;
    bool esHijoIzquierdo() const;
    bool esHijoDerecho() const;
    Nodo* getHermano() const;
    Nodo* getTio() const;
    Nodo* getAbuelo() const;
};

#endif