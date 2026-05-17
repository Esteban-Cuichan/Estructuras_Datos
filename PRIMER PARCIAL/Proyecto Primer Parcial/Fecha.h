#ifndef FECHA_H
#define FECHA_H
#include <iostream>
#include <string>
using namespace std;

class Fecha{
    private:
        int dia;
        int mes;
        int year;
        int hora;
        int minutos;
    public:
        Fecha(int,int,int,int,int);
        Fecha();
        ~Fecha() {}
        int getDia();
        int getMes();
        int getYear();
        int getHora();
        int getMinutos();
        void setDia(int);
        void setMes(int);
        void setYear(int);
        void setHora(int);
        void setMinutos(int);
        string calcularDiaSemana();
        bool esBisiesto(int);
        int obtenerDiadelYear(int,int,int);
        bool esFeriadoMovil(int,int,int);
        string mesString(int);
        string Feriado(int,int,int);
        string validarFechaYObtenerFeriado(int,int,int);
        string fechaFormateada();
        bool esHorarioLaboral();
};
#endif