#ifndef PUNTOINTERES_H
#define PUNTOINTERES_H

#include <string>

using namespace std;

class PuntoInteres
{
private:
    long long idOSM;

    string nombre;
    string categoria;
    string subtipo;

    double latitud;
    double longitud;

public:
    PuntoInteres();

    PuntoInteres(
    long long idOSM,
    const string& nombre,
    const string& categoria,
    const string& subtipo,
    double latitud,
    double longitud
);

    ~PuntoInteres();

    long long getIdOSM() const;

    string getNombre() const;

    string getCategoria() const;

    string getSubtipo() const;

    double getLatitud() const;

    double getLongitud() const;

    void setNombre(
        const string& nombre
    );

    void setCategoria(
        const string& categoria
    );

    void setSubtipo(
        const string& subtipo
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