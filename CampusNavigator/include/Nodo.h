#ifndef NODO_H
#define NODO_H

#include <string>

using namespace std;

class ListaAristas;

class Nodo
{
private:
    int id;
    string nombre;

    double latitud;
    double longitud;

    bool visitado;
    double tiempoAcumulado;
    double distanciaAcumulada;

    Nodo* anterior;
    ListaAristas* aristas;

public:
    Nodo(
        int id,
        const string& nombre,
        double latitud,
        double longitud
    );

    ~Nodo();

    int getId() const;
    string getNombre() const;

    double getLatitud() const;
    double getLongitud() const;

    bool estaVisitado() const;
    void setVisitado(bool estado);

    double getTiempoAcumulado() const;
    void setTiempoAcumulado(double tiempo);

    double getDistanciaAcumulada() const;
    void setDistanciaAcumulada(double distancia);

    Nodo* getAnterior() const;
    void setAnterior(Nodo* nodo);

    ListaAristas* getConexiones() const;

    void reiniciar();
};

#endif