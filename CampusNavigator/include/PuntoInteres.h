#ifndef PUNTOINTERES_H
#define PUNTOINTERES_H

#include <string>

using namespace std;

class PuntoInteres
{
private:
    long long idOSM;

    string nombre;
    string tipo;

    double latitud;
    double longitud;

public:
    PuntoInteres();

    PuntoInteres(
        long long idOSM,
        const string& nombre,
        const string& tipo,
        double latitud,
        double longitud
    );

    ~PuntoInteres();

    long long getIdOSM() const;

    string getNombre() const;

    string getTipo() const;

    double getLatitud() const;

    double getLongitud() const;

    void setNombre(
        const string& nombre
    );

    void setTipo(
        const string& tipo
    );

    void setLatitud(
        double latitud
    );

    void setLongitud(
        double longitud
    );

    void mostrar() const;
};

#endif