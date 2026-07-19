#include "Audio.h"
#include "Conexion.h"
#include <stdexcept>
#include <sstream>
#include <cstdlib>

Audio::Audio(Conexion* conn) : conexion(conn) {}

string Audio::urlEncode(const string& value) const {
    ostringstream escaped;
    escaped.fill('0');
    escaped<<hex;

    for (char c : value) {
        if (isalnum(static_cast<unsigned char>(c)) || c=='-' || c=='_' || c=='.' || c=='~') {
            escaped<<c;
        } else if (c==' ') {
            escaped<<"%20";
        } else {
            unsigned int byte=static_cast<unsigned char>(c);
            ostringstream hexStr;
            hexStr<<uppercase<<hex<<(byte>>4);
            hexStr<<uppercase<<hex<<(byte&0xF);
            string hs=hexStr.str();
            if (hs.size()<2) hs=string("0")+hs;
            escaped<<'%'<<hs;
        }
    }

    return escaped.str();
}

bool Audio::reproducirTexto(const string& texto) const {
    if (!conexion) throw runtime_error("Conexion no inicializada en Audio");

    string base="https://translate.google.com/translate_tts?ie=UTF-8&tl=en&client=tw-ob&q=";
    string q=urlEncode(texto);
    string url=base+q;
    string archivo="tts_output.mp3";

    bool ok=conexion->descargarArchivo(url, archivo);
    if (!ok) return false;

#ifdef _WIN32
    string comando="start \"\" \""+archivo+"\"";
#else
    string comando="xdg-open \""+archivo+"\" &";
#endif
    int rc=system(comando.c_str());
    (void)rc;
    return true;
}

Audio::~Audio() {}