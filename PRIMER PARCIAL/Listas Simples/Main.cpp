#include "Lista.h"
#include "validarCedula.h"
#include "validarExpresion.h"
#include <iostream>
#include <string>
using namespace std;

auto deseaContinuar = [](char respuesta) -> bool{
    respuesta = tolower(respuesta);
    if(respuesta=='y') return true;
    if(respuesta=='n') return false;
    cout<<"Operador invalido!\n Asumiendo salida por seguridad..."<<endl;
    return false;
};

int main(){
    string cedula,nombre,apellido;
    const char* charcedula;
    char aux;
    bool continuar=true;
    int op;
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
            if(validarExpresion::validarNombre(nombre)){
                cout<<"Ingrese su apellido: ";
                cin.ignore();
                getline(cin,apellido);
                if(validarExpresion::validarNombre(apellido)){
                    miLista->insertar(cedula,nombre,apellido);
                    miLista->guardarArchivoIngreso(nombre,cedula,apellido);
                    cout<<"Desea agregar otro usuario? [Y/n]: ";
                    cin>>aux;
                    continuar=deseaContinuar(aux);
                    if(!continuar && (tolower(aux)!='n')){
                        return 1;
                    }
                    miLista->limpiarpantalla();
                }else{
                    cout<<"El apellido ingresado es incorrecto!"<<endl;
                    cout<<"Intente ingresar otros datos!"<<endl;
                }
            }else{
                cout<<"El nombre ingresado es incorrecto!"<<endl;
                cout<<"Intente ingresar otros datos!"<<endl;
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
    /*cout<<"Empezando a contar cedulas por provincia!"<<endl;
    miLista->conteoGeneral();
    cout<<"Eliminacion de digitos impares :)"<<endl;
    miLista->eliminarDigito();*/
    delete miLista;
    return 0;
}