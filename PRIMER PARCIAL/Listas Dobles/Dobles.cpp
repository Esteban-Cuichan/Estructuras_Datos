#include <iostream>
using namespace std;

class Nodo {
public:
    string cedula;
    string nombre;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(string c, string n) {
        cedula = c;
        nombre = n;
        siguiente = nullptr;
        anterior = nullptr;
    }
};

class ListaDoble {
private:
    Nodo* cabeza;

public:
    ListaDoble() {
        cabeza = nullptr;
    }

    void insertarInicio(string c, string n) {
        Nodo* nuevo = new Nodo(c, n);

        if (!cabeza) {
            cabeza = nuevo;
            return;
        }

        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
        cabeza = nuevo;
    }

    void insertarFinal(string c, string n) {
        Nodo* nuevo = new Nodo(c, n);

        if (!cabeza) {
            cabeza = nuevo;
            return;
        }

        Nodo* aux = cabeza;

        while (aux->siguiente)
            aux = aux->siguiente;

        aux->siguiente = nuevo;
        nuevo->anterior = aux;
    }

    void insertarEntre(string c, string n, string ref) {
        Nodo* nuevo = new Nodo(c, n);
        Nodo* aux = cabeza;

        while (aux && aux->cedula != ref)
            aux = aux->siguiente;

        if (!aux) return;

        nuevo->siguiente = aux->siguiente;
        nuevo->anterior = aux;

        if (aux->siguiente)
            aux->siguiente->anterior = nuevo;

        aux->siguiente = nuevo;
    }

    bool buscar(string c) {
        Nodo* aux = cabeza;

        while (aux) {
            if (aux->cedula == c)
                return true;
            aux = aux->siguiente;
        }

        return false;
    }

    void eliminar(string c) {
        Nodo* aux = cabeza;

        while (aux && aux->cedula != c)
            aux = aux->siguiente;

        if (!aux) return;

        if (aux->anterior)
            aux->anterior->siguiente = aux->siguiente;
        else
            cabeza = aux->siguiente;

        if (aux->siguiente)
            aux->siguiente->anterior = aux->anterior;

        delete aux;
    }

    void mostrar() {
        Nodo* aux = cabeza;

        while (aux) {
            cout << aux->cedula << " - " << aux->nombre << endl;
            aux = aux->siguiente;
        }
    }
};

int main() {
    ListaDoble lista;

    lista.insertarInicio("1", "Juan");
    lista.insertarFinal("2", "Maria");
    lista.insertarFinal("3", "Pedro");

    lista.insertarEntre("4", "Ana", "2");

    cout << "LISTA DOBLE:\n";
    lista.mostrar();

    cout << "\nBuscar 3: " << (lista.buscar("3") ? "SI" : "NO") << endl;

    lista.eliminar("2");

    cout << "\nDespues de eliminar 2:\n";
    lista.mostrar();

    return 0;
}