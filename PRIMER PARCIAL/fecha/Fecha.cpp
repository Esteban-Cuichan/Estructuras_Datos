#include "Fecha.h"
#include "fstream"
#include "sstream"

Fecha::Fecha(int dia,int mes,int year,int hora,int minutos){
    this->dia=dia;
    this->mes=mes;
    this->year=year;
    this->hora=hora;
    this->minutos=minutos;
}

Fecha::Fecha(){
    this->dia=0;
    this->mes=0;
    this->year=0;
    this->hora=0;
    this->minutos=0;
}

int Fecha::getDia(){
    return dia;
}

int Fecha::getMes(){
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

void Fecha::setMes(int mes){
    this->mes=mes;
}

string Fecha::calcularDiaSemana(){
    ifstream archivo("dias.txt");
    if(!archivo.is_open()){
        cout<<"No se ha encontrado el archivo mencionado!"<<endl;
    }
    string nomDia,diaarchivo;
    const int* c=(const int[]){0,3,2,5,0,3,5,1,4,6,2,4};
    int y=year-(mes<3);
    int diaSemana=(y+(y/4)-(y/100)+(y/400)+*(c+(mes-1))+dia) % 7;
    while(getline(archivo,diaarchivo,',')){
        if(getline(archivo,nomDia)){
            if(diaarchivo==to_string(diaSemana)){
                return nomDia;
            }
        }
    }
    archivo.close();
}

bool Fecha::esBisiesto(int year){
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

int Fecha::obtenerDiadelYear(int dia, int mes, int year){
    int diasAcumulador=0;
    for(int i=1;i<mes;i++){
        if(i==2){
            diasAcumulador+=(esBisiesto(year) ? 29 : 28);
        }else if (i<=7){
            if(i % 2 != 0){
                diasAcumulador+=31;
            }else{
                diasAcumulador+=30;
            }
        }else{
            if(i % 2 == 0){
                diasAcumulador+=31;
            }else{
                diasAcumulador+=30;
            }
        }
    }
    return diasAcumulador+dia;
}

bool Fecha::esFeriadoMovil(int diaIngresado, int mesIngresado, int yearIngresado){
    int a = yearIngresado % 19;
    int b = yearIngresado % 4;
    int c = yearIngresado % 7;
    int M =24;
    int N = 5;
    int d = ((19*a)+M)%30;
    int e = ((2*b)+(4*c)+(6*d)+N) % 7;
    int diasPasados = d + e;
    int diaDomingo = 0;
    int mesDomingo = 0;
    if(diasPasados<10){
        diaDomingo=diasPasados+22;
        mesDomingo=3;
    }else{
        diaDomingo=diasPasados-9;
        mesDomingo=4;
        if(diaDomingo==26) diaDomingo=19;
        if(diaDomingo==25 && d==28 && a>10) diaDomingo=18;
    }

    int diaYearIngresado = obtenerDiadelYear(diaIngresado,mesIngresado,yearIngresado);
    int diaYearDomingo = obtenerDiadelYear(diaDomingo,mesDomingo,yearIngresado);
    int diaYearLunesCarnaval = diaYearDomingo-48;
    int diaYearMartesCarnaval = diaYearDomingo - 47;
    int diaYearViernesSanto = diaYearDomingo-2;

    if(diaYearIngresado == diaYearLunesCarnaval || diaYearIngresado == diaYearMartesCarnaval || diaYearIngresado == diaYearViernesSanto){
        return true;
    }
    return false;
}

string Fecha::mesString(int mes){
    ifstream archivo("mes.txt");
    string numMes="",nomMes="",resultado="Mes no encontrado";
    if(!archivo.is_open()){
        cout<<"No se ha encontrado el archivo mencionado!"<<endl;
        return resultado;
    }

    while(getline(archivo,numMes,',')){
        if(getline(archivo,nomMes)){
            if(numMes==to_string(mes)){
                if(!nomMes.empty()){
                    nomMes.front()=toupper(nomMes.front());
                    for(auto it=nomMes.begin()+1;it!=nomMes.end();it++){
                        *it=tolower(*it);
                    }
                }
                resultado=nomMes;
                break;
            }
        }
    }
    archivo.close();
    return resultado;
}

string Fecha::Feriado(int dia,int mes,int year){
    ifstream archivo("feriados.txt");
    string diaArchivo,mesArchivo,nomFeriado;
    if(!archivo.is_open()){
        return "Archivo no encontrado!";
    }
    while(getline(archivo,diaArchivo,',')){
        if(getline(archivo,mesArchivo,',')){
            if(getline(archivo,nomFeriado)){
                if(diaArchivo==to_string(dia) && mesArchivo==to_string(mes)){
                    archivo.close();
                    return nomFeriado;
                }
            }
        }
    }
    archivo.close();
    return "";
}

string Fecha::validarFechaYObtenerFeriado(int dia,int mes,int year){
    string diaSemana=calcularDiaSemana();
    if(diaSemana=="Sabado" || diaSemana=="Domingo"){
        return "Fin de semana (Oficinas cerradas)";
    }
    if(esFeriadoMovil(dia,mes,year)){
        return "Feriado de Carnaval o Semana Santa";
    }

    string FeriadoDirecto=Feriado(dia,mes,year);
    if(!FeriadoDirecto.empty()){
        if(diaSemana=="Lunes" || diaSemana=="Viernes"){
            return FeriadoDirecto;
        }
        if((mes==1 && dia==1) || (mes==12 && dia==25)){
            return FeriadoDirecto;
        }
    }
    int diaActualYear=obtenerDiadelYear(dia,mes,year);
    if(diaSemana=="Lunes"){
        for (int m=1;m<=12;m++) {
            for (int d=1;d<=31; d++) {
                if(obtenerDiadelYear(d, m, year) == (diaActualYear-1)) {
                    string feriadoAyer = Feriado(d, m, year);
                    if(!feriadoAyer.empty()){
                        return feriadoAyer+" (Trasladado de Domingo a Lunes)";
                    }
                }
                if(obtenerDiadelYear(d, m, year) == (diaActualYear + 1)) {
                    string feriadoManana = Feriado(d, m, year);
                    if(!feriadoManana.empty()) {
                        return feriadoManana+" (Trasladado de Martes a Lunes)";
                    }
                }
            }
        }
    }
    if(diaSemana=="Viernes"){
        for (int m = 1; m <= 12; m++) {
            for (int d = 1; d <= 31; d++) {
                int diaAnioEvaluado = obtenerDiadelYear(d, m, year);
                string feriadoEval = Feriado(d, m, year);
                
                if (!feriadoEval.empty()) {
                    if (diaAnioEvaluado == (diaActualYear - 1) ||
                        diaAnioEvaluado == (diaActualYear - 2) ||
                        diaAnioEvaluado == (diaActualYear + 1)){
                        return feriadoEval + " (Trasladado al Viernes)";
                    }
                }
            }
        }
    }

    return "DISPONIBLE";
}