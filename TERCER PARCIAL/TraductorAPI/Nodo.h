#ifndef NODO_H
#define NODO_H
#include <string>
using namespace std;

class Nodo {
private:
    string palabraEspanol;
    string palabraIngles;
    Nodo* siguiente;

public:
    Nodo(const string& esp, const string& eng);
    string getPalabraEspanol() const;
    string getPalabraIngles() const;
    Nodo* getSiguiente() const;
    void setPalabraEspanol(const string& esp);
    void setPalabraIngles(const string& eng);
    void setSiguiente(Nodo* sig);
    ~Nodo();
};

#endif