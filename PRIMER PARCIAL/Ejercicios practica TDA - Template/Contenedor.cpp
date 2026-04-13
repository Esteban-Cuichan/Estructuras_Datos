#include "Contenedor.h"
#include <iostream>
#include <string>
using namespace std;

int main(){
	Contenedor<double> numero;
	double dato;
	cout<<"Ingrese el numero que desea guardar> ";
	cin>>dato;
	numero.guardar(dato);
	cout<<"El valor ingresado es> "<<numero.recuperar()<<endl;
	
	Contenedor<string> frase;
	string expresion;
	cout<<"Ingrese la frase que desea guardar> ";
	cin>>expresion;
	frase.guardar(expresion);
	cout<<"La frase ingresada es> "<<frase.recuperar()<<endl;
	return 0;
}
