#ifndef LISTA_H
#define LISTA_H
#include "Nodo.h"
using namespace std;

class Lista {
private:
    Nodo* cabeza;

public:
    Lista();
    void insertar(const string& esp, const string& eng);
    void mostrar() const;
    Nodo* buscar(const string& esp) const;
    bool eliminar(const string& esp);
    void liberarMemoria();
    ~Lista();
};

#endif