#ifndef TRADUCTOR_H
#define TRADUCTOR_H
#include <string>
using namespace std;

class Conexion;

class Traductor {
private:
    Conexion* conexion;
    string endpoint;

public:
    Traductor(Conexion* conn, const string& apiEndpoint);
    string traducir(const string& palabra) const;
    ~Traductor();
};

#endif