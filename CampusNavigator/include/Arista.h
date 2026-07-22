#ifndef ARISTA_H
#define ARISTA_H

class Nodo;

class Arista{
    private:
        Nodo* destino;
        double distancia;
        double tiempo;
    public:
        Arista(Nodo* destino,double distancia,double tiempo);
        Nodo* getDestino() const;
        double getDistancia() const;
        double getTiempo() const;
        void setDistancia(double distancia);
        void setTiempo(double tiempo); 
};
#endif