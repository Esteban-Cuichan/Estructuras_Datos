#ifndef AUDIO_H
#define AUDIO_H
#include <string>
using namespace std;

class Conexion;

class Audio {
private:
    Conexion* conexion;
    string urlEncode(const string& value) const;

public:
    Audio(Conexion* conn);
    bool reproducirTexto(const string& texto) const;
    ~Audio();
};

#endif