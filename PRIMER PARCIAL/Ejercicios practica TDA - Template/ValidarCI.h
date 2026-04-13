#ifndef VALIDARCI_H
#define VALIDARCI_H
#include "IValidar.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class ValidarCI : public IValidar<string> {
	public:
		bool validar(string cedula) override{
			if(cedula.length()!=10) return false;
			int region;
			try{
	            region = stoi(cedula.substr(0, 2));
	        }catch (...){return false;}
			if(region<1 || (region>24 && region!=30)) return false;
			int suma = 0;
			for(int i=0;i<9;i++){
				int digito=cedula[i]-'0';
				if(i%2==0){
					digito*=2;
					if(digito>9) digito-=9;
				}
				suma+=digito;
			}
			
			int ultimoDigito=cedula[9]-'0';
			int decenaSuperior=((suma/10)+1)*10;
			if(suma%10==0) decenaSuperior=suma;
			int digitoCalculado=decenaSuperior-suma;
			return digitoCalculado== ultimoDigito;
		}
};

#endif
