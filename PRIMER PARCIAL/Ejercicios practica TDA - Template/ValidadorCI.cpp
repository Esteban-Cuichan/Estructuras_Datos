#include <iostream>
#include <string>
#include "ValidarCI.h"
using namespace std;

int main(){
	ValidarCI validador;
	string cedula;
	cout<<"Ingrese el numero de cedula> ";
	cin>>cedula;
	if(validador.validar(cedula)){
		cout<<"La cedula es VALIDA"<<endl;
	} else { 
		cout<<"La cedula es INVALIDA"<<endl;
	}
	return 0;
}
