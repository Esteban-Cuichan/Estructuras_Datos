#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include "Grafo.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace gps {

class MapView {
public:
    MapView(float ancho, float alto);
    ~MapView();

    void inicializar(const Grafo &grafo);
    sf::Vector2f proyectar(double latitud, double longitud) const;
    sf::Vector2f convertirPantallaAGrafo(const sf::Vector2f &posicionPantalla) const;
    int seleccionarNodoCercano(const Grafo &grafo, const sf::Vector2f &posicion, float tolerancia) const;

    void mover(const sf::Vector2f &delta);
    void aplicarZoom(float factor);
    void aplicar(sf::RenderWindow &ventana) const;
    void restaurarView(sf::RenderWindow &ventana) const;
    void dibujarGrafo(sf::RenderWindow &ventana, const Grafo &grafo, const std::vector<int> &ruta,
                      int origenId, int destinoId) const;

private:
    sf::View vista_;
    float ancho_; 
    float alto_; 
    double minLatitud_; 
    double maxLatitud_; 
    double minLongitud_; 
    double maxLongitud_; 
    float escala_; 
    float margen_; 

    void actualizarTransformacion();
    sf::Vector2f proyectarInterno(double latitud, double longitud) const;
};

} // namespace gps

#endif // MAP_VIEW_H
