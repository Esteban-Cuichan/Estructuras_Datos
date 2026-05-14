#ifndef NODO_H
#define NODO_H
using namespace std;

class Nodo{
    private:
        Nodo* siguiente;
        Nodo* anterior;
        //Turno turno;
    public:
        Nodo();
        Nodo(Nodo*,Nodo*);
        ~Nodo(){};
        Nodo* getSiguiente();
        Nodo* getAnterior();
        //Turno getTurno();
        void setSiguiente(Nodo*);
        void setAnterior(Nodo*);
        //void setTurno(Turno);
};
#endif