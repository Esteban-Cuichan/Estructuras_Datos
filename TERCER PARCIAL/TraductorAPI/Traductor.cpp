#include "Traductor.h"
#include "Conexion.h"
#include <stdexcept>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

Traductor::Traductor(Conexion* conn, const string& apiEndpoint)
    : conexion(conn), endpoint(apiEndpoint) {}

static string extraerTraducido(const string& json) {
    const string key1="translatedText";
    size_t pos=json.find(key1);
    if (pos==string::npos) {
        size_t q1=json.find('"');
        size_t q2=string::npos;
        if (q1!=string::npos) q2=json.find('"', q1+1);
        if (q1!=string::npos && q2!=string::npos && q2>q1)
            return json.substr(q1+1, q2-q1-1);
        return string();
    }
    size_t colon=json.find(':', pos);
    if (colon==string::npos) return string();
    size_t start=json.find('"', colon);
    if (start==string::npos) return string();
    size_t end=json.find('"', start+1);
    if (end==string::npos) return string();
    return json.substr(start+1, end-start-1);
}

string Traductor::traducir(const string& palabra) const {
    if (!conexion) throw runtime_error("Conexion no inicializada");

    string payload="{\"q\":\""+palabra+"\",\"source\":\"es\",\"target\":\"en\",\"format\":\"text\"}";
    string respuesta;
    try {
        respuesta=conexion->postJson(endpoint, payload);
    } catch (...) {
        respuesta.clear();
    }

    string traducido=extraerTraducido(respuesta);

    if (traducido.empty()) {
        auto urlencode=[](const string& value) {
            ostringstream escaped;
            escaped.fill('0');
            escaped<<hex;
            for (unsigned char c : value) {
                if (isalnum(c) || c=='-' || c=='_' || c=='.' || c=='~') {
                    escaped<<c;
                } else if (c==' ') {
                    escaped<<'+';
                } else {
                    escaped<<'%'<<uppercase<<setw(2)<<int(c)<<nouppercase;
                }
            }
            return escaped.str();
        };

        try {
            string url="https://api.mymemory.translated.net/get?q="+urlencode(palabra)+"&langpair=es|en";
            string resp2=conexion->getUrl(url);
            const string key="translatedText";
            size_t pos=resp2.find(key);
            if (pos!=string::npos) {
                size_t colon=resp2.find(':', pos);
                size_t start=resp2.find('"', colon);
                if (start!=string::npos) {
                    size_t end=resp2.find('"', start+1);
                    if (end!=string::npos) {
                        traducido=resp2.substr(start+1, end-start-1);
                    }
                }
            }
        } catch (...) {
        }
    }

    traducido.erase(remove(traducido.begin(), traducido.end(), '\n'), traducido.end());
    return traducido;
}

Traductor::~Traductor() {}