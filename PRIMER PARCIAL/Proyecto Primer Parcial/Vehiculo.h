#pragma once
#include <string>
using namespace std;

class Vehiculo {
    private:
        string color;
        string placa;
        int anio;
    public:
        Vehiculo();
        ~Vehiculo(){};
        Vehiculo(string,string,int);
        string getColor();
        string getPlaca();
        int getAnio();
        void setColor(string);
        void setPlaca(string);
        void setAnio(int);
};