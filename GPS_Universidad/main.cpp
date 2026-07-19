#include "include/InterfazGrafica.h"
#include <iostream>
#include <cstdlib>

using namespace std;

const char* ARCHIVO_EDIFICIOS="data/edificios.txt";
const char* ARCHIVO_CONEXIONES="data/conexiones.txt";
const char* ARCHIVO_CONFIGURACION="data/configuracion.txt";

int main(int argc, char* argv[]) {
    std::setlocale(LC_ALL, "en_US.UTF-8");
    if (argc>1) {
        for (int i=1; i<argc; i++) {
            if (string(argv[i])=="--help" || string(argv[i])=="-h") {
                cout<<"=================================================="<<endl;
                cout<<"    SISTEMA DE NAVEGACIÓN GPS - CAMPUS UNIVERSITARIO"<<endl;
                cout<<"=================================================="<<endl;
                cout<<endl;
                cout<<"Uso: ./ProyectoGPS [opciones]"<<endl;
                cout<<endl;
                cout<<"Opciones:"<<endl;
                cout<<"  --help, -h     Mostrar esta ayuda"<<endl;
                cout<<"  --version, -v  Mostrar versión del sistema"<<endl;
                cout<<"  --no-color     Desactivar colores ANSI"<<endl;
                cout<<endl;
                cout<<"Archivos de datos:"<<endl;
                cout<<"  "<<ARCHIVO_EDIFICIOS<<"  - Edificios y ubicaciones"<<endl;
                cout<<"  "<<ARCHIVO_CONEXIONES<<" - Conexiones entre ubicaciones"<<endl;
                cout<<"  "<<ARCHIVO_CONFIGURACION<<" - Configuración del sistema"<<endl;
                cout<<endl;
                cout<<"Ejemplo:"<<endl;
                cout<<"  ./ProyectoGPS --no-color"<<endl;
                return 0;
            }
            if (string(argv[i])=="--version" || string(argv[i])=="-v") {
                cout<<"Sistema de Navegación GPS - Campus Universitario"<<endl;
                cout<<"Versión: 1.0.0"<<endl;
                cout<<"Compilado con C++17"<<endl;
                cout<<"Fecha: "<<__DATE__<<" "<<__TIME__<<endl;
                return 0;
            }
        }
    }
    cout<<"=================================================="<<endl;
    cout<<"    SISTEMA DE NAVEGACIÓN GPS - CAMPUS UNIVERSITARIO"<<endl;
    cout<<"=================================================="<<endl;
    cout<<endl;
    cout<<"Desarrollado con C++17"<<endl;
    cout<<"Algoritmo de Dijkstra para rutas óptimas"<<endl;
    cout<<"Integración con Google Maps"<<endl;
    cout<<endl;
    InterfazGrafica interfaz;
    bool usarColor=true;
    for (int i=1; i<argc; i++) {
        if (string(argv[i])=="--no-color") {
            usarColor=false;
            break;
        }
    }
    cout<<"Inicializando sistema..."<<endl;
    cout<<"   Cargando datos desde:"<<endl;
    cout<<"   - "<<ARCHIVO_EDIFICIOS<<endl;
    cout<<"   - "<<ARCHIVO_CONEXIONES<<endl;
    cout<<"   - "<<ARCHIVO_CONFIGURACION<<endl;
    cout<<endl;
    bool exito=interfaz.inicializar(ARCHIVO_EDIFICIOS, ARCHIVO_CONEXIONES);
    if (!exito) {
        cerr<<"ERROR: No se pudo inicializar el sistema"<<endl;
        cerr<<endl;
        cerr<<"Posibles causas:"<<endl;
        cerr<<"  1. Los archivos de datos no existen en la carpeta 'data/'"<<endl;
        cerr<<"  2. Los archivos tienen formato incorrecto"<<endl;
        cerr<<"  3. No hay permisos de lectura para los archivos"<<endl;
        cerr<<endl;
        cerr<<"Solución:"<<endl;
        cerr<<"  Verifique que los archivos existan en la ubicación correcta"<<endl;
        cerr<<"  y tengan el formato esperado."<<endl;
        cerr<<endl;
        cerr<<"Para ayuda, ejecute: ./ProyectoGPS --help"<<endl;
        return 1;
    }
    if (!usarColor) {
        cout<<"Modo sin colores activado"<<endl;
        cout<<endl;
    }
    cout<<"¼ Sistema inicializado correctamente"<<endl;
    cout<<endl;
    cout<<"Estadísticas del sistema:"<<endl;
    cout<<endl;
    cout<<"Presione Enter para continuar...";
    cin.get();
    interfaz.ejecutar();
    cout<<endl;
    cout<<"=================================================="<<endl;
    cout<<"    ¡GRACIAS POR USAR EL SISTEMA DE NAVEGACIÓN!"<<endl;
    cout<<"=================================================="<<endl;
    cout<<endl;
    cout<<"Para más información, consulte la documentación en docs/"<<endl;
    cout<<"Reporte problemas en: github.com/proyecto-campus-gps/issues"<<endl;
    cout<<endl;
    cout<<"El sistema se ha cerrado correctamente."<<endl;
    return 0;
}