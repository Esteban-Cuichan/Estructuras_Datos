#ifndef ARISTA_H
#define ARISTA_H

class Nodo;

enum class TipoCamino{
    PEATONAL,
    VEHICULAR,
    RAMPA,
    ESCALERAS,
    MIXTO
};

enum class EstadoCamino{
    ABIERTO,
    CERRADO,
    MANTENIMIENTO,
    RESTRINGIDO
};

class Arista{
    private:
        int id;
        Nodo* origen;
        Nodo* destino;
        double distancia;
        TipoCamino tipo;
        EstadoCamino estado;
    public:
        Arista(int,Nodo*,Nodo*,double,TipoCamino);
        ~Arista();
        int getId() const;
        Nodo* getOrigen() const;
        void setOrigen(Nodo*);
        Nodo* getDestino() const;
        void setDestino(Nodo*);
        double getDistancia() const;
        void setDistancia(double);
        TipoCamino getTipo() const;
        void setTipo(TipoCamino);
        EstadoCamino getEstado() const;
        void setEstado(EstadoCamino estado);
        bool esIgual(Nodo* origen,Nodo* destino) const;
        char* toString() const;
    private:
        Arista(const Arista& otro);
        Arista& operator=(const Arista& otro);
};
#endif