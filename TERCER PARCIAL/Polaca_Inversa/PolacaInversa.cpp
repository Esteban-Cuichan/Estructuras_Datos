#include <iostream>
#include <stack>
#include <algorithm>
#include <cstring>
#include <limits>
#include <queue>
#include <cctype>
#include <cmath>
#include <map>
#include <fstream>
#include <cstdlib>
using namespace std;

struct NodoArbol {
    string valor;
    NodoArbol* izquierdo;
    NodoArbol* derecho;
    
    NodoArbol(string val) : valor(val), izquierdo(nullptr), derecho(nullptr) {}
    NodoArbol(string val, NodoArbol* izq, NodoArbol* der) : 
        valor(val), izquierdo(izq), derecho(der) {}
};

int longitud(const char* expresion) {
    int contador=0;
    while (*(expresion+contador)!='\0') {
        contador++;
    }
    return contador;
}

bool esFuncionTrigonometrica(const char* expresion, int posicion) {
    if (expresion[posicion]=='s' && expresion[posicion+1]=='e' && expresion[posicion+2]=='n') {
        return true;
    }
    if (expresion[posicion]=='c' && expresion[posicion+1]=='o' && expresion[posicion+2]=='s') {
        return true;
    }
    if (expresion[posicion]=='t' && expresion[posicion+1]=='a' && expresion[posicion+2]=='n') {
        return true;
    }
    return false;
}

int longitudFuncion(const char* expresion, int posicion) {
    if (expresion[posicion]=='s' && expresion[posicion+1]=='e' && expresion[posicion+2]=='n') return 3;
    if (expresion[posicion]=='c' && expresion[posicion+1]=='o' && expresion[posicion+2]=='s') return 3;
    if (expresion[posicion]=='t' && expresion[posicion+1]=='a' && expresion[posicion+2]=='n') return 3;
    return 0;
}

bool esOperador(const char* expresion, int posicion) {
    char actual=expresion[posicion];
    if (actual=='+' || actual=='-' || actual=='*' || actual=='/' || actual=='^') {
        return true;
    }
    if (esFuncionTrigonometrica(expresion, posicion)) {
        return true;
    }
    return false;
}

int precedencia(const char* expresion, int posicion) {
    char actual=expresion[posicion];
    if (esFuncionTrigonometrica(expresion, posicion)) return 4;
    if (actual=='^') return 3;
    if (actual=='*' || actual=='/') return 2;
    if (actual=='+' || actual=='-') return 1;
    return 0;
}

bool esNumero(const string& str) {
    if (str.empty()) return false;
    bool puntoDecimal=false;
    for (char c : str) {
        if (c=='.') {
            if (puntoDecimal) return false;
            puntoDecimal=true;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

char* sacarOperador(const char* expresion, int &totalOperadores) {
    totalOperadores=0;
    int len=longitud(expresion);
    int i=0;
    
    while (i<len) {
        if (esOperador(expresion, i)) {
            totalOperadores++;
            if (esFuncionTrigonometrica(expresion, i)) {
                i+=3;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
    
    char* operadores=new char[totalOperadores*4+1];
    int indice=0;
    i=0;
    
    while (i<len) {
        if (esOperador(expresion, i)) {
            if (esFuncionTrigonometrica(expresion, i)) {
                int longFunc=longitudFuncion(expresion, i);
                for (int j=0; j<longFunc; j++) {
                    operadores[indice++]=expresion[i+j];
                }
                operadores[indice++]=' ';
                i+=longFunc;
            } else {
                operadores[indice++]=expresion[i];
                operadores[indice++]=' ';
                i++;
            }
        } else {
            i++;
        }
    }
    operadores[indice]='\0';
    return operadores;
}

char* expresionPostFija(const char* expresion) {
    int len=longitud(expresion);
    char* resultado=new char[len*4+1];
    stack<string> pila;
    int indiceResultado=0;
    int i=0;
    
    while (i<len) {
        char actual=expresion[i];
        
        if (isdigit(actual)) {
            string numero="";
            while (i<len && (isdigit(expresion[i]) || expresion[i]=='.')) {
                numero+=expresion[i];
                i++;
            }
            for (char c : numero) {
                resultado[indiceResultado++]=c;
            }
            resultado[indiceResultado++]=' ';
            continue;
        }
        else if (isalpha(actual) && !esFuncionTrigonometrica(expresion, i)) {
            resultado[indiceResultado++]=actual;
            resultado[indiceResultado++]=' ';
            i++;
        }
        else if (esFuncionTrigonometrica(expresion, i)) {
            int longFunc=longitudFuncion(expresion, i);
            string funcName;
            for (int j=0; j<longFunc; j++) {
                funcName+=expresion[i+j];
            }
            pila.push(funcName);
            i+=longFunc;
        }
        else if (actual=='(') {
            pila.push("(");
            i++;
        }
        else if (actual==')') {
            while (!pila.empty() && pila.top()!="(") {
                string top=pila.top();
                pila.pop();
                for (char c : top) {
                    resultado[indiceResultado++]=c;
                }
                resultado[indiceResultado++]=' ';
            }
            if (!pila.empty() && pila.top()=="(") {
                pila.pop();
            }
            i++;
        }
        else if (actual=='+' || actual=='-' || actual=='*' || actual=='/' || actual=='^') {
            string op(1, actual);
            while (!pila.empty() && pila.top()!="(" && 
                   precedencia(expresion, i)<=precedencia(pila.top().c_str(), 0)) {
                string top=pila.top();
                pila.pop();
                for (char c : top) {
                    resultado[indiceResultado++]=c;
                }
                resultado[indiceResultado++]=' ';
            }
            pila.push(op);
            i++;
        }
        else {
            i++;
        }
    }
    
    while (!pila.empty()) {
        string top=pila.top();
        pila.pop();
        for (char c : top) {
            resultado[indiceResultado++]=c;
        }
        resultado[indiceResultado++]=' ';
    }
    
    resultado[indiceResultado]='\0';
    return resultado;
}

NodoArbol* construirArbolDesdePostfija(const char* postfija) {
    stack<NodoArbol*> pila;
    string token;
    int i=0;
    int len=longitud(postfija);
    
    while (i<len) {
        while (i<len && postfija[i]==' ') i++;
        if (i>=len) break;
        
        token="";
        while (i<len && postfija[i]!=' ') {
            token+=postfija[i];
            i++;
        }
        
        bool esOperadorToken=false;
        if (token=="+" || token=="-" || token=="*" || token=="/" || token=="^") {
            esOperadorToken=true;
        } else if (token=="sen" || token=="cos" || token=="tan") {
            esOperadorToken=true;
        }
        
        if (esOperadorToken) {
            if (token=="+" || token=="-" || token=="*" || token=="/" || token=="^") {
                if (pila.size()<2) {
                    cerr<<"Error: Faltan operandos para "<<token<<endl;
                    return nullptr;
                }
                NodoArbol* derecho=pila.top(); pila.pop();
                NodoArbol* izquierdo=pila.top(); pila.pop();
                pila.push(new NodoArbol(token, izquierdo, derecho));
            }
            else if (token=="sen" || token=="cos" || token=="tan") {
                if (pila.empty()) {
                    cerr<<"Error: Falta argumento para "<<token<<endl;
                    return nullptr;
                }
                NodoArbol* argumento=pila.top(); pila.pop();
                pila.push(new NodoArbol(token, argumento, nullptr));
            }
        } else {
            pila.push(new NodoArbol(token));
        }
    }
    
    if (pila.size()!=1) {
        cerr<<"Error: Pila tiene "<<pila.size()<<" elementos"<<endl;
        return nullptr;
    }
    
    return pila.top();
}

void recorridoInorden(NodoArbol* nodo, string &resultado) {
    if (nodo==nullptr) return;
    
    if (nodo->valor=="sen" || nodo->valor=="cos" || nodo->valor=="tan") {
        resultado+=nodo->valor+"(";
        recorridoInorden(nodo->izquierdo, resultado);
        resultado+=")";
    }
    else if (nodo->valor=="+" || nodo->valor=="-" || nodo->valor=="*" || 
             nodo->valor=="/" || nodo->valor=="^") {
        resultado+="(";
        recorridoInorden(nodo->izquierdo, resultado);
        resultado+=" "+nodo->valor+" ";
        recorridoInorden(nodo->derecho, resultado);
        resultado+=")";
    }
    else {
        resultado+=nodo->valor;
    }
}

void recorridoPreorden(NodoArbol* nodo, string &resultado) {
    if (nodo==nullptr) return;
    
    resultado+=nodo->valor;
    if (nodo->izquierdo!=nullptr) {
        resultado+=" ";
        recorridoPreorden(nodo->izquierdo, resultado);
    }
    if (nodo->derecho!=nullptr) {
        resultado+=" ";
        recorridoPreorden(nodo->derecho, resultado);
    }
}

void recorridoPostorden(NodoArbol* nodo, string &resultado) {
    if (nodo==nullptr) return;
    
    if (nodo->izquierdo!=nullptr) {
        recorridoPostorden(nodo->izquierdo, resultado);
        resultado+=" ";
    }
    if (nodo->derecho!=nullptr) {
        recorridoPostorden(nodo->derecho, resultado);
        resultado+=" ";
    }
    resultado+=nodo->valor;
}

void visualizarArbol(NodoArbol* nodo, string prefijo="", bool esIzquierdo=true) {
    if (nodo==nullptr) return;
    
    cout<<prefijo;
    cout<<(esIzquierdo?"+--":"+--");
    cout<<nodo->valor<<endl;
    
    string nuevoPrefijo=prefijo+(esIzquierdo?"|   ":"    ");
    
    if (nodo->izquierdo!=nullptr && nodo->derecho==nullptr) {
        visualizarArbol(nodo->izquierdo, nuevoPrefijo, true);
    } else {
        if (nodo->izquierdo) visualizarArbol(nodo->izquierdo, nuevoPrefijo, true);
        if (nodo->derecho) visualizarArbol(nodo->derecho, nuevoPrefijo, false);
    }
}

void recorridoPorNiveles(NodoArbol* raiz) {
    if (raiz==nullptr) return;
    
    queue<NodoArbol*> cola;
    cola.push(raiz);
    int nivel=0;
    
    cout<<"\n=== ARBOL DE EXPRESION (POR NIVELES) ===\n";
    while (!cola.empty()) {
        int size=cola.size();
        cout<<"Nivel "<<nivel++<<": ";
        
        for (int i=0; i<size; i++) {
            NodoArbol* actual=cola.front(); cola.pop();
            cout<<actual->valor;
            
            if (i<size-1) cout<<", ";
            
            if (actual->izquierdo) cola.push(actual->izquierdo);
            if (actual->derecho) cola.push(actual->derecho);
        }
        cout<<endl;
    }
}

double evaluarArbol(NodoArbol* nodo, map<string, double>& variables) {
    if (nodo==nullptr) return 0;
    
    if (nodo->izquierdo==nullptr && nodo->derecho==nullptr) {
        if (esNumero(nodo->valor)) {
            return stod(nodo->valor);
        } else {
            auto it=variables.find(nodo->valor);
            if (it!=variables.end()) {
                return it->second;
            } else {
                cout<<"Advertencia: Variable '"<<nodo->valor<<"' sin valor asignado (usando 0)"<<endl;
                return 0;
            }
        }
    }
    
    double izq=evaluarArbol(nodo->izquierdo, variables);
    double der=nodo->derecho?evaluarArbol(nodo->derecho, variables):0;
    
    if (nodo->valor=="+") return izq+der;
    if (nodo->valor=="-") return izq-der;
    if (nodo->valor=="*") return izq*der;
    if (nodo->valor=="/") {
        if (der==0) {
            cerr<<"Error: Division por cero"<<endl;
            return 0;
        }
        return izq/der;
    }
    if (nodo->valor=="^") return pow(izq, der);
    if (nodo->valor=="sen") return sin(izq);
    if (nodo->valor=="cos") return cos(izq);
    if (nodo->valor=="tan") return tan(izq);
    
    return 0;
}

void exportarGraphviz(NodoArbol* nodo, ofstream& archivo, int& id) {
    if (nodo==nullptr) return;
    
    int actual=id++;
    archivo<<"    node"<<actual<<" [label=\""<<nodo->valor<<"\"];\n";
    
    if (nodo->izquierdo) {
        int hijoId=id;
        exportarGraphviz(nodo->izquierdo, archivo, id);
        archivo<<"    node"<<actual<<" -> node"<<hijoId<<";\n";
    }
    if (nodo->derecho) {
        int hijoId=id;
        exportarGraphviz(nodo->derecho, archivo, id);
        archivo<<"    node"<<actual<<" -> node"<<hijoId<<";\n";
    }
}

void generarArchivoGraphviz(NodoArbol* raiz, const string& nombreArchivo="arbol.dot") {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr<<"Error: No se pudo crear el archivo "<<nombreArchivo<<endl;
        return;
    }
    
    archivo<<"digraph ArbolExpression {\n";
    archivo<<"    node [shape=circle, style=filled, fillcolor=lightblue];\n";
    int id=0;
    exportarGraphviz(raiz, archivo, id);
    archivo<<"}\n";
    
    archivo.close();
    cout<<"\nArchivo Graphviz generado: "<<nombreArchivo<<endl;
    cout<<"Para visualizar, instala Graphviz y ejecuta: dot -Tpng "<<nombreArchivo<<" -o arbol.png"<<endl;
}

void liberarArbol(NodoArbol* nodo) {
    if (nodo==nullptr) return;
    liberarArbol(nodo->izquierdo);
    liberarArbol(nodo->derecho);
    delete nodo;
}

void asignarVariables(map<string, double>& variables) {
    cout<<"\n--- ASIGNAR VALORES A VARIABLES ---\n";
    cout<<"Ingresa valores para las variables (escribe 'fin' para terminar):\n";
    
    string variable;
    double valor;
    
    while (true) {
        cout<<"Variable (o 'fin' para terminar): ";
        cin>>variable;
        
        if (variable=="fin") break;
        
        cout<<"Valor para "<<variable<<": ";
        cin>>valor;
        
        variables[variable]=valor;
        cout<<variable<<" = "<<valor<<" asignado"<<endl;
    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    cout<<"========================================\n";
    cout<<"   CONVERSOR DE EXPRESIONES CON ARBOLES\n";
    cout<<"========================================\n\n";
    
    string input;
    cout<<"Ingrese la expresion matematica: ";
    getline(cin, input);
    
    if (input.empty()) {
        cout<<"Expresion vacia. Saliendo...\n";
        system("pause");
        return 0;
    }
    
    char* expresion=new char[input.length()+1];
    strcpy(expresion, input.c_str());
    
    int cantidadOperadores=0;
    char* operadores=sacarOperador(expresion, cantidadOperadores);
    
    char* postfija=expresionPostFija(expresion);
    
    NodoArbol* raiz=construirArbolDesdePostfija(postfija);
    
    if (raiz!=nullptr) {
        string inorden, preorden, postorden;
        recorridoInorden(raiz, inorden);
        recorridoPreorden(raiz, preorden);
        recorridoPostorden(raiz, postorden);
        
        cout<<"\n========================================\n";
        cout<<"               RESULTADOS\n";
        cout<<"========================================\n\n";
        
        cout<<"1. EXPRESION ORIGINAL:\n";
        cout<<"   "<<expresion<<"\n\n";
        
        cout<<"2. OPERADORES ENCONTRADOS:\n";
        cout<<"   "<<operadores<<"\n";
        cout<<"   Cantidad: "<<cantidadOperadores<<"\n\n";
        
        cout<<"3. EXPRESION POSTFIJA:\n";
        cout<<"   "<<postfija<<"\n\n";
        
        cout<<"4. EXPRESION PREFIJA:\n";
        cout<<"   "<<preorden<<"\n\n";
        
        cout<<"5. RECORRIDOS DEL ARBOL:\n";
        cout<<"   Inorden (infijo): "<<inorden<<"\n";
        cout<<"   Preorden (prefijo): "<<preorden<<"\n";
        cout<<"   Postorden (postfijo): "<<postorden<<"\n\n";
        
        cout<<"6. ESTRUCTURA DEL ARBOL:\n";
        visualizarArbol(raiz);
        cout<<"\n";
        
        cout<<"7. ARBOL POR NIVELES:\n";
        recorridoPorNiveles(raiz);
        cout<<"\n";
        
        cout<<"========================================\n";
        cout<<"¿Desea evaluar la expresion? (s/n): ";
        char evaluar;
        cin>>evaluar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (evaluar=='s' || evaluar=='S') {
            map<string, double> variables;
            asignarVariables(variables);
            
            double resultado=evaluarArbol(raiz, variables);
            cout<<"\nRESULTADO DE LA EXPRESION: "<<resultado<<endl;
        }
        
        cout<<"\n========================================\n";
        cout<<"¿Desea exportar a Graphviz? (s/n): ";
        char exportar;
        cin>>exportar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (exportar=='s' || exportar=='S') {
            generarArchivoGraphviz(raiz);
        }
        
        liberarArbol(raiz);
    } else {
        cout<<"\nError al procesar la expresion. Verifique la sintaxis."<<endl;
    }
    
    delete[] expresion;
    delete[] postfija;
    delete[] operadores;
    
    cout<<"\n========================================\n";
    system("pause");
    
    return 0;
}