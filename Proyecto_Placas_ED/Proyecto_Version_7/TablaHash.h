#ifndef TABLAHASH_H
#define TABLAHASH_H
#include "Turno.h"
#include <iostream>

class TablaHash{
    private:
        int capacidad;
        Turno** tabla;
        int funcionHash(int clave){
            return clave%capacidad;
        }

    public:
        TablaHash(int tamaniov=101){
            capacidad=tamaniov;
            tabla=new Turno*[capacidad];
            for(int i=0;i<capacidad;++i){
                *(tabla+i)=nullptr;
            }
        }

        ~TablaHash(){
            delete[] tabla;
        }

        void insertar(Turno* nuevoTurno){
            if(nuevoTurno==nullptr)return;
            int indice=funcionHash(nuevoTurno->getIDturno());
            nuevoTurno->setSiguiente(*(tabla+indice));
            if(*(tabla+indice)!=nullptr){
                (*(tabla+indice))->setAnterior(nuevoTurno);
            }
            nuevoTurno->setAnterior(nullptr);
            *(tabla+indice)=nuevoTurno;
        }

        Turno* buscar(int idBuscado){
            int indice=funcionHash(idBuscado);
            Turno* actual=*(tabla+indice);
            while(actual!=nullptr){
                if(actual->getIDturno()==idBuscado){
                    return actual;
                }
                actual=actual->getSiguiente();
            }
            return nullptr;
        }

        void eliminar(int idEliminar){
            int indice=funcionHash(idEliminar);
            Turno* actual=*(tabla+indice);
            while(actual!=nullptr){
                if(actual->getIDturno()==idEliminar){
                    Turno* ant=actual->getAnterior();
                    Turno* sig=actual->getSiguiente();
                    if(ant!=nullptr)ant->setSiguiente(sig);
                    else *(tabla+indice)=sig;
                    if(sig!=nullptr)sig->setAnterior(ant);
                    break;
                }
                actual=actual->getSiguiente();
            }
        }
};
#endif