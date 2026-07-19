#include "Nodo.h"

Nodo::Nodo(const string& esp, const string& eng)
    : palabraEspanol(esp), palabraIngles(eng), siguiente(nullptr) {}

string Nodo::getPalabraEspanol() const { return palabraEspanol; }
string Nodo::getPalabraIngles() const { return palabraIngles; }
Nodo* Nodo::getSiguiente() const { return siguiente; }

void Nodo::setPalabraEspanol(const string& esp) { palabraEspanol=esp; }
void Nodo::setPalabraIngles(const string& eng) { palabraIngles=eng; }
void Nodo::setSiguiente(Nodo* sig) { siguiente=sig; }

Nodo::~Nodo() {}