#include "ArbolRojoNegro.h"
#include <algorithm>

ArbolRojoNegro::ArbolRojoNegro() : raiz(nullptr), ultimoInsertado(nullptr), 
                                  cantidadNodos(0), cantidadRojos(0), cantidadNegros(0) {}

ArbolRojoNegro::~ArbolRojoNegro()
{
    limpiar();
}

void ArbolRojoNegro::limpiar()
{
    destruirArbol(raiz);
    raiz = nullptr;
    ultimoInsertado = nullptr;
    cantidadNodos = 0;
    cantidadRojos = 0;
    cantidadNegros = 0;
}

void ArbolRojoNegro::destruirArbol(Nodo* nodo)
{
    if (nodo == nullptr)
        return;
    destruirArbol(nodo->getIzquierdo());
    destruirArbol(nodo->getDerecho());
    delete nodo;
}

void ArbolRojoNegro::insertar(int valor)
{
    Nodo* nuevoNodo = new Nodo(valor);
    
    // Insertar como en BST normal
    insertarBST(raiz, nuevoNodo);
    
    // Actualizar contadores
    cantidadNodos++;
    
    // Arreglar propiedades del árbol rojo-negro
    arreglarInsercion(nuevoNodo);
    
    // La raíz siempre debe ser negra
    if (raiz != nullptr)
        raiz->setColor(NEGRO);
    
    // Actualizar estadísticas
    actualizarContadores();
    
    ultimoInsertado = nuevoNodo;
}

void ArbolRojoNegro::insertarBST(Nodo* nodoRaiz, Nodo* nodo)
{
    if (nodoRaiz == nullptr)
    {
        nodoRaiz = nodo;
        return;
    }
    
    if (nodo->getValor() < nodoRaiz->getValor())
    {
        if (nodoRaiz->getIzquierdo() == nullptr)
        {
            nodoRaiz->setIzquierdo(nodo);
            nodo->setPadre(nodoRaiz);
        }
        else
        {
            insertarBST(nodoRaiz->getIzquierdo(), nodo);
        }
    }
    else if (nodo->getValor() > nodoRaiz->getValor())
    {
        if (nodoRaiz->getDerecho() == nullptr)
        {
            nodoRaiz->setDerecho(nodo);
            nodo->setPadre(nodoRaiz);
        }
        else
        {
            insertarBST(nodoRaiz->getDerecho(), nodo);
        }
    }
    else
    {
        // Valor duplicado, eliminar el nodo creado
        delete nodo;
        return;
    }
}

void ArbolRojoNegro::arreglarInsercion(Nodo* nodo)
{
    if (nodo == nullptr || nodo->getPadre() == nullptr)
        return;
    
    // Si el padre es negro, no hay problema
    if (nodo->getPadre()->esNegro())
        return;
    
    Nodo* padre = nodo->getPadre();
    Nodo* abuelo = nodo->getAbuelo();
    
    if (abuelo == nullptr)
        return;
    
    Nodo* tio = nodo->getTio();
    
    // CASO 1: El tío es rojo
    if (tio != nullptr && tio->esRojo())
    {
        // Cambiar colores
        padre->setColor(NEGRO);
        tio->setColor(NEGRO);
        abuelo->setColor(ROJO);
        
        // Recursivamente arreglar el abuelo
        arreglarInsercion(abuelo);
        return;
    }
    
    // CASO 2: El tío es negro y el nodo es hijo interno
    if (nodo->esHijoDerecho() && padre->esHijoIzquierdo())
    {
        rotacionIzquierda(padre);
        nodo = padre; // Ahora el nodo es el antiguo padre
        padre = nodo->getPadre();
    }
    else if (nodo->esHijoIzquierdo() && padre->esHijoDerecho())
    {
        rotacionDerecha(padre);
        nodo = padre;
        padre = nodo->getPadre();
    }
    
    // CASO 3: El tío es negro y el nodo es hijo externo
    if (nodo->esHijoIzquierdo() && padre->esHijoIzquierdo())
    {
        rotacionDerecha(abuelo);
        padre->setColor(NEGRO);
        abuelo->setColor(ROJO);
    }
    else if (nodo->esHijoDerecho() && padre->esHijoDerecho())
    {
        rotacionIzquierda(abuelo);
        padre->setColor(NEGRO);
        abuelo->setColor(ROJO);
    }
}

void ArbolRojoNegro::rotacionIzquierda(Nodo* nodo)
{
    if (nodo == nullptr || nodo->getDerecho() == nullptr)
        return;
    
    Nodo* hijoDerecho = nodo->getDerecho();
    Nodo* padre = nodo->getPadre();
    
    // Actualizar el padre del nodo
    if (padre == nullptr)
    {
        raiz = hijoDerecho;
    }
    else if (nodo->esHijoIzquierdo())
    {
        padre->setIzquierdo(hijoDerecho);
    }
    else
    {
        padre->setDerecho(hijoDerecho);
    }
    
    // Actualizar punteros
    hijoDerecho->setPadre(padre);
    nodo->setDerecho(hijoDerecho->getIzquierdo());
    if (nodo->getDerecho() != nullptr)
        nodo->getDerecho()->setPadre(nodo);
    hijoDerecho->setIzquierdo(nodo);
    nodo->setPadre(hijoDerecho);
}

void ArbolRojoNegro::rotacionDerecha(Nodo* nodo)
{
    if (nodo == nullptr || nodo->getIzquierdo() == nullptr)
        return;
    
    Nodo* hijoIzquierdo = nodo->getIzquierdo();
    Nodo* padre = nodo->getPadre();
    
    // Actualizar el padre del nodo
    if (padre == nullptr)
    {
        raiz = hijoIzquierdo;
    }
    else if (nodo->esHijoIzquierdo())
    {
        padre->setIzquierdo(hijoIzquierdo);
    }
    else
    {
        padre->setDerecho(hijoIzquierdo);
    }
    
    // Actualizar punteros
    hijoIzquierdo->setPadre(padre);
    nodo->setIzquierdo(hijoIzquierdo->getDerecho());
    if (nodo->getIzquierdo() != nullptr)
        nodo->getIzquierdo()->setPadre(nodo);
    hijoIzquierdo->setDerecho(nodo);
    nodo->setPadre(hijoIzquierdo);
}

bool ArbolRojoNegro::buscar(int valor) const
{
    Nodo* actual = raiz;
    while (actual != nullptr)
    {
        if (valor == actual->getValor())
            return true;
        else if (valor < actual->getValor())
            actual = actual->getIzquierdo();
        else
            actual = actual->getDerecho();
    }
    return false;
}

void ArbolRojoNegro::actualizarContadores()
{
    cantidadRojos = 0;
    cantidadNegros = 0;
    
    // Recolectar todos los nodos y contar colores usando recorrido inorden
    std::vector<int> recorrido;
    inorden(raiz, recorrido);
    
    // Contar colores recorriendo el árbol
    contarColores(raiz);
}

void ArbolRojoNegro::contarColores(Nodo* nodo)
{
    if (nodo == nullptr)
        return;
    
    if (nodo->esRojo())
        cantidadRojos++;
    else
        cantidadNegros++;
    
    contarColores(nodo->getIzquierdo());
    contarColores(nodo->getDerecho());
}

int ArbolRojoNegro::getAltura() const
{
    return obtenerAltura(raiz);
}

int ArbolRojoNegro::obtenerAltura(Nodo* nodo) const
{
    if (nodo == nullptr)
        return 0;
    
    int alturaIzq = obtenerAltura(nodo->getIzquierdo());
    int alturaDer = obtenerAltura(nodo->getDerecho());
    
    return 1 + std::max(alturaIzq, alturaDer);
}

void ArbolRojoNegro::inorden(Nodo* nodo, std::vector<int>& resultado) const
{
    if (nodo == nullptr)
        return;
    inorden(nodo->getIzquierdo(), resultado);
    resultado.push_back(nodo->getValor());
    inorden(nodo->getDerecho(), resultado);
}

// Getters
Nodo* ArbolRojoNegro::getRaiz() const { return raiz; }
Nodo* ArbolRojoNegro::getUltimoInsertado() const { return ultimoInsertado; }
int ArbolRojoNegro::getCantidadNodos() const { return cantidadNodos; }
int ArbolRojoNegro::getCantidadRojos() const { return cantidadRojos; }
int ArbolRojoNegro::getCantidadNegros() const { return cantidadNegros; }