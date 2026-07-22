#include "MapView.h"
#include <cmath>
#include <limits>

namespace gps {

MapView::MapView(float ancho, float alto)
    : ancho_(ancho), alto_(alto), minLatitud_(0.0), maxLatitud_(0.0),
      minLongitud_(0.0), maxLongitud_(0.0), escala_(1.0f), margen_(40.0f) {
    vista_.setSize(ancho_, alto_);
    vista_.setCenter(ancho_ / 2.0f, alto_ / 2.0f);
}

MapView::~MapView() {
}

void MapView::inicializar(const Grafo &grafo) {
    if (grafo.obtenerCantidadNodos() == 0) {
        return;
    }

    minLatitud_ = std::numeric_limits<double>::max();
    maxLatitud_ = std::numeric_limits<double>::lowest();
    minLongitud_ = std::numeric_limits<double>::max();
    maxLongitud_ = std::numeric_limits<double>::lowest();

    for (int i = 0; i < grafo.obtenerCantidadNodos(); ++i) {
        const Nodo *nodo = grafo.obtenerNodoPorIndice(i);
        if (nodo == nullptr) {
            continue;
        }
        minLatitud_ = std::min(minLatitud_, nodo->obtenerLatitud());
        maxLatitud_ = std::max(maxLatitud_, nodo->obtenerLatitud());
        minLongitud_ = std::min(minLongitud_, nodo->obtenerLongitud());
        maxLongitud_ = std::max(maxLongitud_, nodo->obtenerLongitud());
    }

    if (maxLongitud_ - minLongitud_ <= 0.0 || maxLatitud_ - minLatitud_ <= 0.0) {
        escala_ = 1.0f;
    } else {
        float rangoX = static_cast<float>(maxLongitud_ - minLongitud_);
        float rangoY = static_cast<float>(maxLatitud_ - minLatitud_);
        escala_ = std::min((ancho_ - margen_ * 2.0f) / rangoX,
                           (alto_ - margen_ * 2.0f) / rangoY);
    }

    update:
    actualizarTransformacion();
}

sf::Vector2f MapView::proyectar(double latitud, double longitud) const {
    return proyectarInterno(latitud, longitud);
}

sf::Vector2f MapView::convertirPantallaAGrafo(const sf::Vector2f &posicionPantalla) const {
    return windowToCoords(posicionPantalla); // compile error! needs fix
}

int MapView::seleccionarNodoCercano(const Grafo &grafo, const sf::Vector2f &posicion, float tolerancia) const {
    int seleccionado = -1;
    float distanciaMinima = tolerancia;
    for (int i = 0; i < grafo.obtenerCantidadNodos(); ++i) {
        const Nodo *nodo = grafo.obtenerNodoPorIndice(i);
        if (nodo == nullptr) {
            continue;
        }
        sf::Vector2f coordenada = proyectar(nodo->obtenerLatitud(), nodo->obtenerLongitud());
        float dx = coordenada.x - posicion.x;
        float dy = coordenada.y - posicion.y;
        float distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia < distanciaMinima) {
            distanciaMinima = distancia;
            seleccionado = nodo->obtenerId();
        }
    }
    return seleccionado;
}

void MapView::mover(const sf::Vector2f &delta) {
    vista_.move(-delta);
}

void MapView::aplicarZoom(float factor) {
    vista_.zoom(factor);
}

void MapView::aplicar(sf::RenderWindow &ventana) const {
    ventana.setView(vista_);
}

void MapView::restaurarView(sf::RenderWindow &ventana) const {
    sf::View defaultView(sf::FloatRect(0, 0, ancho_, alto_));
    ventana.setView(defaultView);
}

void MapView::dibujarGrafo(sf::RenderWindow &ventana, const Grafo &grafo, const std::vector<int> &ruta,
                           int origenId, int destinoId) const {
    for (int i = 0; i < grafo.obtenerCantidadNodos(); ++i) {
        const ListaAdyacencia *adyacencia = grafo.obtenerAdyacenciaPorIndice(i);
        const Nodo *nodo = grafo.obtenerNodoPorIndice(i);
        if (adyacencia == nullptr || nodo == nullptr) {
            continue;
        }
        sf::Vector2f origen = proyectar(nodo->obtenerLatitud(), nodo->obtenerLongitud());
        const ListaAdyacencia::NodoAdyacente *nodoAdy = adyacencia->obtenerCabeza();
        while (nodoAdy != nullptr) {
            const Nodo *destino = grafo.obtenerNodoPorId(nodoAdy->arista->obtenerDestinoId());
            if (destino != nullptr) {
                sf::Vector2f destinoPos = proyectar(destino->obtenerLatitud(), destino->obtenerLongitud());
                sf::Vertex linea[] = {
                    sf::Vertex(origen, sf::Color(150, 150, 150)),
                    sf::Vertex(destinoPos, sf::Color(150, 150, 150))
                };
                ventana.draw(linea, 2, sf::Lines);
            }
            nodoAdy = nodoAdy->siguiente;
        }
    }

    for (size_t i = 0; i < ruta.size(); ++i) {
        const Nodo *nodo = grafo.obtenerNodoPorId(ruta[i]);
        if (nodo == nullptr) {
            continue;
        }
        sf::CircleShape circulo(5.0f);
        circulo.setOrigin(5.0f, 5.0f);
        circulo.setPosition(proyectar(nodo->obtenerLatitud(), nodo->obtenerLongitud()));
        circulo.setFillColor(sf::Color::Green);
        ventana.draw(circulo);
    }

    for (size_t i = 1; i < ruta.size(); ++i) {
        const Nodo *anterior = grafo.obtenerNodoPorId(ruta[i - 1]);
        const Nodo *actual = grafo.obtenerNodoPorId(ruta[i]);
        if (anterior == nullptr || actual == nullptr) {
            continue;
        }
        sf::Vertex linea[] = {
            sf::Vertex(proyectar(anterior->obtenerLatitud(), anterior->obtenerLongitud()), sf::Color::Green),
            sf::Vertex(proyectar(actual->obtenerLatitud(), actual->obtenerLongitud()), sf::Color::Green)
        };
        ventana.draw(linea, 2, sf::Lines);
    }

    for (int i = 0; i < grafo.obtenerCantidadNodos(); ++i) {
        const Nodo *nodo = grafo.obtenerNodoPorIndice(i);
        if (nodo == nullptr) {
            continue;
        }
        sf::CircleShape circulo(4.0f);
        circulo.setOrigin(4.0f, 4.0f);
        circulo.setPosition(proyectar(nodo->obtenerLatitud(), nodo->obtenerLongitud()));
        if (nodo->obtenerId() == origenId) {
            circulo.setFillColor(sf::Color::Blue);
        } else if (nodo->obtenerId() == destinoId) {
            circulo.setFillColor(sf::Color::Red);
        } else {
            circulo.setFillColor(sf::Color(200, 100, 100));
        }
        ventana.draw(circulo);
    }
}

void MapView::actualizarTransformacion() {
    vista_.setSize(ancho_, alto_);
    vista_.setCenter(ancho_ / 2.0f, alto_ / 2.0f);
}

sf::Vector2f MapView::proyectarInterno(double latitud, double longitud) const {
    float x = static_cast<float>((longitud - minLongitud_) * escala_ + margen_);
    float y = static_cast<float>((maxLatitud_ - latitud) * escala_ + margen_);
    return sf::Vector2f(x, y);
}

} // namespace gps
