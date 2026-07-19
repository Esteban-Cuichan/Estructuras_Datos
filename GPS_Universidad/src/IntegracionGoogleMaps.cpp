#include "../include/IntegracionGoogleMaps.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cctype>

const char* IntegracionGoogleMaps::BASE_URL="https://www.google.com/maps/dir/";

IntegracionGoogleMaps::IntegracionGoogleMaps(Grafo* grafo)
    : grafo(grafo), ruta(nullptr), longitudRuta(0) {
    if (grafo==nullptr) {
        std::cerr<<"Error: Grafo nulo en IntegracionGoogleMaps"<<std::endl;
    }
}

IntegracionGoogleMaps::~IntegracionGoogleMaps() {}

void IntegracionGoogleMaps::setRuta(int* ruta, int longitud) {
    this->ruta=ruta;
    this->longitudRuta=longitud;
}

char* IntegracionGoogleMaps::generarEnlaceNodo(Nodo* nodo) const {
    if (nodo==nullptr) {
        char* resultado=new char[100];
        std::strcpy(resultado, "Error: Nodo nulo");
        return resultado;
    }
    double lat=nodo->getLatitud();
    double lon=nodo->getLongitud();
    const char* nombre=nodo->getNombre();
    const int TAMANO=512;
    char* url=new char[TAMANO];
    std::snprintf(url, TAMANO, "https://www.google.com/maps/search/%s/@%f,%f,17z", nombre, lat, lon);
    return url;
}

char* IntegracionGoogleMaps::generarEnlaceRuta() const {
    if (this->grafo==nullptr) {
        char* error=new char[100];
        std::strcpy(error, "Error: Grafo nulo");
        return error;
    }
    if (this->ruta==nullptr || this->longitudRuta<2) {
        char* error=new char[100];
        std::strcpy(error, "Error: No hay ruta definida");
        return error;
    }
    const int TAMANO_INICIAL=1024;
    const int TAMANO_INCREMENTO=256;
    char* url=new char[TAMANO_INICIAL];
    int capacidadActual=TAMANO_INICIAL;
    int posicion=0;
    auto agregarTexto=[&](const char* texto) {
        int longitudTexto=std::strlen(texto);
        if (posicion+longitudTexto+1>=capacidadActual) {
            int nuevaCapacidad=capacidadActual+TAMANO_INCREMENTO;
            char* nuevoUrl=new char[nuevaCapacidad];
            std::strcpy(nuevoUrl, url);
            delete[] url;
            url=nuevoUrl;
            capacidadActual=nuevaCapacidad;
        }
        std::strcpy(url+posicion, texto);
        posicion+=longitudTexto;
    };
    agregarTexto(BASE_URL);
    Nodo* origen=this->grafo->getNodo(this->ruta[0]);
    if (origen!=nullptr) {
        char coord[100];
        std::snprintf(coord, sizeof(coord), "%f,%f", origen->getLatitud(), origen->getLongitud());
        agregarTexto(coord);
    }
    char* waypoints=this->generarWaypoints();
    if (waypoints!=nullptr) {
        agregarTexto(waypoints);
        delete[] waypoints;
    }
    Nodo* destino=this->grafo->getNodo(this->ruta[this->longitudRuta-1]);
    if (destino!=nullptr) {
        agregarTexto("/");
        char coord[100];
        std::snprintf(coord, sizeof(coord), "%f,%f", destino->getLatitud(), destino->getLongitud());
        agregarTexto(coord);
    }
    agregarTexto("?travelmode=walking");
    return url;
}

char* IntegracionGoogleMaps::generarEnlaceGrafo() const {
    if (this->grafo==nullptr) {
        char* error=new char[100];
        std::strcpy(error, "Error: Grafo nulo");
        return error;
    }
    const int TAMANO=2048;
    char* url=new char[TAMANO];
    int posicion=0;
    posicion+=std::snprintf(url+posicion, TAMANO-posicion, "https://www.google.com/maps/search/");
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            if (i>0) {
                posicion+=std::snprintf(url+posicion, TAMANO-posicion, ",");
            }
            posicion+=std::snprintf(url+posicion, TAMANO-posicion, "%f,%f", nodo->getLatitud(), nodo->getLongitud());
        }
    }
    posicion+=std::snprintf(url+posicion, TAMANO-posicion, "/@%f,%f,15z", this->grafo->getNodo(0)->getLatitud(), this->grafo->getNodo(0)->getLongitud());
    return url;
}

char* IntegracionGoogleMaps::generarURLCoordenadas(double lat, double lon) const {
    const int TAMANO=256;
    char* url=new char[TAMANO];
    std::snprintf(url, TAMANO, "https://www.google.com/maps/search/%f,%f", lat, lon);
    return url;
}

char* IntegracionGoogleMaps::generarKMLRuta() const {
    if (this->grafo==nullptr || this->ruta==nullptr || this->longitudRuta<2) {
        char* error=new char[100];
        std::strcpy(error, "Error: No hay ruta definida");
        return error;
    }
    const int TAMANO_INICIAL=2048;
    const int TAMANO_INCREMENTO=512;
    char* kml=new char[TAMANO_INICIAL];
    int capacidadActual=TAMANO_INICIAL;
    int posicion=0;
    auto agregarTexto=[&](const char* texto) {
        int longitudTexto=std::strlen(texto);
        if (posicion+longitudTexto+1>=capacidadActual) {
            int nuevaCapacidad=capacidadActual+TAMANO_INCREMENTO;
            char* nuevoKml=new char[nuevaCapacidad];
            std::strcpy(nuevoKml, kml);
            delete[] kml;
            kml=nuevoKml;
            capacidadActual=nuevaCapacidad;
        }
        std::strcpy(kml+posicion, texto);
        posicion+=longitudTexto;
    };
    agregarTexto("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    agregarTexto("<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
    agregarTexto("  <Document>\n");
    agregarTexto("    <name>Ruta del Campus</name>\n");
    agregarTexto("    <Style id=\"rutaStyle\">\n");
    agregarTexto("      <LineStyle>\n");
    agregarTexto("        <color>ff0000ff</color>\n");
    agregarTexto("        <width>4</width>\n");
    agregarTexto("      </LineStyle>\n");
    agregarTexto("      <PolyStyle>\n");
    agregarTexto("        <color>7f0000ff</color>\n");
    agregarTexto("      </PolyStyle>\n");
    agregarTexto("    </Style>\n");
    for (int i=0; i<this->longitudRuta; i++) {
        Nodo* nodo=this->grafo->getNodo(this->ruta[i]);
        if (nodo!=nullptr) {
            agregarTexto("    <Placemark>\n");
            agregarTexto("      <name>");
            agregarTexto(nodo->getNombre());
            agregarTexto("</name>\n");
            agregarTexto("      <Point>\n");
            agregarTexto("        <coordinates>");
            char coord[100];
            std::snprintf(coord, sizeof(coord), "%f,%f,0", nodo->getLongitud(), nodo->getLatitud());
            agregarTexto(coord);
            agregarTexto("</coordinates>\n");
            agregarTexto("      </Point>\n");
            agregarTexto("    </Placemark>\n");
        }
    }
    agregarTexto("    <Placemark>\n");
    agregarTexto("      <name>Ruta Óptima</name>\n");
    agregarTexto("      <styleUrl>#rutaStyle</styleUrl>\n");
    agregarTexto("      <LineString>\n");
    agregarTexto("        <coordinates>\n");
    for (int i=0; i<this->longitudRuta; i++) {
        Nodo* nodo=this->grafo->getNodo(this->ruta[i]);
        if (nodo!=nullptr) {
            char coord[100];
            std::snprintf(coord, sizeof(coord), "          %f,%f,0\n", nodo->getLongitud(), nodo->getLatitud());
            agregarTexto(coord);
        }
    }
    agregarTexto("        </coordinates>\n");
    agregarTexto("      </LineString>\n");
    agregarTexto("    </Placemark>\n");
    agregarTexto("  </Document>\n");
    agregarTexto("</kml>\n");
    return kml;
}

bool IntegracionGoogleMaps::exportarCoordenadas(const char* archivo) const {
    if (this->grafo==nullptr) {
        return false;
    }
    FILE* file=fopen(archivo, "w");
    if (file==nullptr) {
        return false;
    }
    fprintf(file, "# Exportación de coordenadas del campus\n");
    fprintf(file, "# Formato: ID|Nombre|Latitud|Longitud|Tipo\n");
    fprintf(file, "# Generado por Sistema de Navegación GPS\n\n");
    for (int i=0; i<this->grafo->getNumNodos(); i++) {
        Nodo* nodo=this->grafo->getNodo(i);
        if (nodo!=nullptr) {
            fprintf(file, "%d|%s|%f|%f|%d\n", nodo->getId(), nodo->getNombre(), nodo->getLatitud(), nodo->getLongitud(), static_cast<int>(nodo->getTipo()));
        }
    }
    fclose(file);
    return true;
}

void IntegracionGoogleMaps::mostrarEnlaceRuta() const {
    char* url=this->generarEnlaceRuta();
    if (url!=nullptr) {
        std::cout<<"\n╔═══════════════════════════════════════════════════╗"<<std::endl;
        std::cout<<"║            ENLACE A GOOGLE MAPS                   ║"<<std::endl;
        std::cout<<"╚═══════════════════════════════════════════════════╝"<<std::endl;
        std::cout<<"\nPara ver la ruta en Google Maps, copie y pegue el siguiente enlace:"<<std::endl;
        std::cout<<"\n"<<url<<std::endl;
        std::cout<<"\nSugerencia: Abra el enlace en su navegador para visualizar la ruta."<<std::endl;
        std::cout<<"   El modo de navegación está configurado como 'peatonal'."<<std::endl;
        delete[] url;
    }
}

char* IntegracionGoogleMaps::generarWaypoints() const {
    if (this->ruta==nullptr || this->longitudRuta<3) {
        char* empty=new char[1];
        empty[0]='\0';
        return empty;
    }
    const int TAMANO=1024;
    char* waypoints=new char[TAMANO];
    int posicion=0;
    for (int i=1; i<this->longitudRuta-1; i++) {
        Nodo* nodo=this->grafo->getNodo(this->ruta[i]);
        if (nodo!=nullptr) {
            if (i==1) {
                posicion+=std::snprintf(waypoints+posicion, TAMANO-posicion, "/");
            } else {
                posicion+=std::snprintf(waypoints+posicion, TAMANO-posicion, ",");
            }
            posicion+=std::snprintf(waypoints+posicion, TAMANO-posicion, "%f,%f", nodo->getLatitud(), nodo->getLongitud());
        }
    }
    return waypoints;
}

char* IntegracionGoogleMaps::codificarURL(const char* cadena) const {
    if (cadena==nullptr) {
        char* vacio=new char[1];
        vacio[0]='\0';
        return vacio;
    }
    const int TAMANO=512;
    char* codificada=new char[TAMANO];
    int posicion=0;
    for (int i=0; cadena[i]!='\0' && posicion<TAMANO-4; i++) {
        char c=cadena[i];
        if (std::isalnum(c) || c=='-' || c=='_' || c=='.' || c=='~') {
            codificada[posicion++]=c;
        } else if (c==' ') {
            codificada[posicion++]='+';
        } else {
            posicion+=std::snprintf(codificada+posicion, 4, "%%%02X", static_cast<unsigned char>(c));
        }
    }
    codificada[posicion]='\0';
    return codificada;
}

const char* IntegracionGoogleMaps::getNombreNodo(int id) const {
    if (this->grafo==nullptr) {
        return "Desconocido";
    }
    Nodo* nodo=this->grafo->getNodo(id);
    if (nodo==nullptr) {
        return "Desconocido";
    }
    return nodo->getNombre();
}

IntegracionGoogleMaps::IntegracionGoogleMaps(const IntegracionGoogleMaps& otro) {}

IntegracionGoogleMaps& IntegracionGoogleMaps::operator=(const IntegracionGoogleMaps& otro) {
    return *this;
}