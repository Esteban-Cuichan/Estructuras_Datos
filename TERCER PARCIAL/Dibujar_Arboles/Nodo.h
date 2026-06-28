#ifndef NODO_H
#define NODO_H

class Nodo{
    private:
        int valor;
        Nodo* izquierdo;
        Nodo* derecho;
    public:
        Nodo(int val);
        ~Nodo();
        int getValor() const;
        void setValor(int val);
        Nodo* getIzquierdo() const;
        void setIzquierdo(Nodo* izq);
        Nodo* getDerecho() const;
        void setDerecho(Nodo* der);
};

#endif