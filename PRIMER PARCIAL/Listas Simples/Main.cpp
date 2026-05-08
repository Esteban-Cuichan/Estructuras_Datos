#include "Lista.h"
#include "validarCedula.h"
#include <iostream>
#include <string>
using namespace std;

int main(){
    string cedula,nombre;
    const char* charcedula;
    char aux;
    bool continuar=true;
    Lista* miLista=new Lista();
    while(continuar!=false){
        cout<<"-------- INGRESO DE DATOS ---------"<<endl;
        cout<<"Ingrese su cedula: ";
        cin>>cedula;
        charcedula=cedula.c_str();
        if(validarCedula::validar(charcedula)){
            if(miLista->busquedaDuplicado(cedula)){
                cout<<"Error: No se puede ingresar una cedula existente!"<<endl;
                cout<<"Intente nuevamente con datos diferentes!"<<endl;
                miLista->limpiarpantalla();
                continue;
            }
            cout<<"Ingrese su nombre: ";
            cin.ignore();
            getline(cin,nombre);
            miLista->insertar(cedula,nombre);
            miLista->guardarArchivoIngreso(nombre,cedula);
            cout<<"Desea agregar otro usuario? [Y/n]: ";
            cin>>aux;
            miLista->limpiarpantalla();
            if(aux=='n' || aux=='N'){
                continuar=false;
            }else if(aux=='Y' || aux=='y'){
                continuar=true;
            }else{
                cout<<"Operador invalido! Saliendo por seguridad..."<<endl;
                return false;
                miLista->limpiarpantalla();
            }
        }else{
            cout<<"La cedula ingresada no es valida!"<<endl;
            cout<<"Intente nuevamente con diferentes datos!"<<endl;
            miLista->limpiarpantalla();
            continue;
        }
        
    }
    cout<<"Registros en Memoria"<<endl;
    miLista->imprimir();
    cout<<"Empezando a contar cedulas por provincia!"<<endl;
    miLista->conteoGeneral();
    delete miLista;
    return 0;
}