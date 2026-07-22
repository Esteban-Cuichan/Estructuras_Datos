#ifndef NODO_H
#define NODO_H
#include <string>
using namespace std;
class ListaAristas;
class Nodo{
    private:
         int id;
         string nombre;
         bool visitado;
         double tiempoAcumulado;
         double distanciaAcumulada;
         Nodo* anterior;
         ListaAristas* aristas;
    public:
        Nodo(int id,const string& nombre);
        ~Nodo();
        int getId() const;
        string getNombre() const;
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