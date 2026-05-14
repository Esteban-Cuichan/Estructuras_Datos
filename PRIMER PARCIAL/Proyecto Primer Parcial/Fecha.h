#ifndef FECHA_H
#define FECHA_H
#include <iostream>
#include <string>
using namespace std;

class Fecha{
    private:
        int dia;
        string mes;
        int year;
        int hora;
        int minutos;
    public:
        Fecha(int,string,int,int,int);
        Fecha() {}
        int getDia();
        string getMes();
        int getYear();
        int getHora();
        int getMinutos();
        void setDia(int);
        void setMes(string);
        void setYear(int);
        void setHora(int);
        void setMinutos(int);

};
#endif