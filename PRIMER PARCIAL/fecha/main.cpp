#include "Fecha.h"
#include "fstream"
#include "sstream"

int main(void){
    int dia,mes,year;
    cout<<"Ingrese el dia: ";
    cin>>dia;
    cout<<"Ingrese el mes: ";
    cin>>mes;
    cout<<"Ingrese el año: ";
    cin>>year;
    Fecha fecha;
    fecha.setDia(dia);
    fecha.setMes(mes);
    fecha.setYear(year);
    string estadoDia=fecha.validarFechaYObtenerFeriado(dia,mes,year);
    cout<<fecha.calcularDiaSemana()<<", "<<dia<<" de "<<fecha.mesString(mes)<<" de "<<year<<endl;
    if(estadoDia!="DISPONIBLE"){
        cout<<"El dia que intenta agendar no esta disponible. Motivo: "<<estadoDia;
    }else{
        cout<<"Fecha Disponible. Registrando Turno...."<<endl;
    }
}

/*cd "c:\Users\Esteban\Documents\Documentos\PREGRADO SI-ABR26-AGO26\ESTRUCTURAS DE DATOS\PRIMER PARCIAL\fecha\" ; if ($?) { g++ main.cpp Fecha.cpp -o main } ; if ($?) { .\main }*/