#include "Fecha.h"

Fecha::Fecha(int dia,string mes,int year,int hora,int minutos){
    this->dia=dia;
    this->mes=mes;
    this->year=year;
    this->hora=hora;
    this->minutos=minutos;
}

int Fecha::getDia(){
    return dia;
}

string Fecha::getMes(){
    return mes;
}

int Fecha::getYear(){
    return year;
}

int Fecha::getHora(){
    return hora;
}

int Fecha::getMinutos(){
    return minutos;
}

void Fecha::setDia(int dia){
    this->dia=dia;
}

void Fecha::setHora(int hora){
    this->hora=hora;
}

void Fecha::setMinutos(int minutos){
    this->minutos=minutos;
}

void Fecha::setYear(int year){
    this->year=year;
}

void Fecha::setMes(string mes){
    this->mes=mes;
}