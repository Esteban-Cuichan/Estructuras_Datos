#include <iostream>
using namespace std;

class Nodo{
    private:
        int clave;
        Nodo* siguiente;
    public:
        Nodo(int c) : clave(c), siguiente(nullptr) {}
        int getClave() const { return clave; }
        Nodo* getSiguiente() const { return siguiente; }
        void setSiguiente(Nodo* sig) { siguiente = sig; }
};

class TablaHash{
    private:
        Nodo** tabla;
        int capacidad;
        int totalClave;
        int funcionHash(int clave) const{
            int residuo = clave % capacidad;
            return (residuo < 0) ? residuo + capacidad : residuo;
        }
    public:
        TablaHash(int tam) : capacidad(tam), totalClave(0){
            tabla=static_cast<Nodo**>(::operator new(sizeof(Nodo*)*capacidad));
            for(int i=0;i<capacidad;i++){
                *(tabla+i)=nullptr;
            }
        }

        ~TablaHash(){
            for(int i=0;i<capacidad;i++){
                Nodo* actual=*(tabla+i);
                while(actual!=nullptr){
                    Nodo* aEliminar=actual;
                    actual=actual->getSiguiente();
                    delete aEliminar;
                }
            }
            ::operator delete(tabla);
        }

        void insertar(int clave){
            int indice=funcionHash(clave);
            Nodo* nuevo=new Nodo(clave);
            Nodo** cubetaActual=tabla+indice;
            nuevo->setSiguiente(*cubetaActual);
            *cubetaActual=nuevo;
            totalClave++;
        }

        bool buscar(int clave) const{
            int indice=funcionHash(clave);
            Nodo* actual=*(tabla+indice);
            while(actual!=nullptr){
                if(actual->getClave()==clave){
                    return true;
                }
                actual=actual->getSiguiente();
            }
            return false;
        }

        void mostrarTabla() const{
            cout<<"\t\t========== TABLA DE HASH ==========\n";
            for(int i=0;i<capacidad;i++){
                cout<<"Cubeta ["<<i<<"]: ";
                Nodo* actual=*(tabla+i);
                if(actual==nullptr){
                    cout<<" vacia";
                }else{
                    while(actual!=nullptr){
                        cout<<"->["<<actual->getClave()<<"]";
                        actual=actual->getSiguiente();
                    }
                }
                cout<<"\n";
            }
            cout<<"Factor de carga: "<<static_cast<double>(totalClave)/capacidad<<"\n";
        }
};

int main(){
    const int tam=5;
    TablaHash hash(tam);
    hash.insertar(10);
    hash.insertar(-4);
    hash.insertar(5);
    hash.insertar(6);
    hash.insertar(11);
    hash.mostrarTabla();
    int busqueda=6;
    if(hash.buscar(busqueda)){
        cout<<"Clave "<<busqueda<<" encontrada en la tabla hash.\n";
    } else {
        cout<<"Clave "<<busqueda<<" no encontrada en la tabla hash.\n";
    }
        return 0;
}