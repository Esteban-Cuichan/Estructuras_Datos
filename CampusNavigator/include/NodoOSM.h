#ifndef NODOOSM_H
#define NODOOSM_H

class NodoOSM
{
private:

    long long id;

    double latitud;

    double longitud;

public:

    NodoOSM(
        long long id,
        double latitud,
        double longitud
    );

    long long getId() const;

    double getLatitud() const;

    double getLongitud() const;
};

#endif