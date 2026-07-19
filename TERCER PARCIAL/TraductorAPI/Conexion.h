#ifndef CONEXION_H
#define CONEXION_H
#include <string>
using namespace std;

class Conexion {
public:
    Conexion();
    string postJson(const string& url, const string& jsonPayload);
    string getUrl(const string& url);
    bool descargarArchivo(const string& url, const string& rutaDestino);
    ~Conexion();
};

#endif