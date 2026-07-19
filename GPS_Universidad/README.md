# 🗺️ Sistema Inteligente de Navegación para Campus Universitario

**ProyectoGPS** es un motor de localización y navegación escrito en C++17 que modela un campus universitario como un grafo dirigido correlacionado geométricamente. El sistema calcula trayectorias óptimas utilizando el algoritmo de Dijkstra (con búsqueda lineal y optimizaciones mediante Min-Heaps binarios), integrando además interfaces de visualización en terminal (ANSI), web interactiva (D3.js) y exportación cartográfica (Google Maps y KML).

---

## 📋 Tabla de Contenidos
- [Características](#-características)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Descripción de Módulos (Arquitectura)](#-descripción-de-módulos-arquitectura)
- [Requisitos del Sistema](#%EF%B8%8F-requisitos-del-sistema)
- [Instalación y Compilación](#-instalación-y-compilación)
- [Uso del Sistema](#-uso-del-sistema)
- [Formato de Especificación de Datos](#-formato-de-especificación-de-datos)
- [Contribuciones](#-contribuciones)
- [Licencia](#-licencia)

---

## 🚀 Características

*   **Modelado Estricto de Grafos:** Representación dinámica mediante arreglos indexados y listas de adyacencia de nodos (Puntos de Interés como Edificios, Laboratorios, Garitas o Cruces) y aristas (Caminos Peatonales, Vehiculares, Rampas o Escaleras).
*   **Algoritmo de Dijkstra Optimizado:** Motor dual de cálculo. Implementación estándar $O(V^2)$ e implementación optimizada mediante colas de prioridad (`std::priority_queue`) con complejidad de tiempo reducida a $O(E \log V)$.
*   **Restricciones de Estado de Vías:** El motor ignora o penaliza tramos en tiempo de ejecución si se encuentran bajo estados de `CERRADO` o `MANTENIMIENTO`.
*   **Reconstrucción Dinámica de Trayectorias:** Reversión de vectores predecesores para proveer telemetría por tramo, distancias acumuladas e instrucciones paso a paso.
*   **Doble Capa de Visualización:**
    *   *Terminal:* Renderizado ASCII que proyecta una aproximación espacial topológica y marcas ANSI colorizadas en base al estado de la vía y el tipo de nodo.
    *   *Navegador (Web):* Exportación automatizada a un entorno interactivo HTML5 apoyado sobre `D3.js` con soporte para Force-Directed Graphs, manipulación de zoom/arrastre y resaltado de la ruta activa.
*   **Pasarela de Mapas Externa:** Generación en tiempo real de cadenas URL sintácticamente válidas para la API de rutas de Google Maps (en modo peatonal/walking) y estructuración de archivos `.kml` nativos para Google Earth.

---

## 📁 Estructura del Proyecto

```text
ProyectoGPS/
├── include/
│   ├── Nodo.h               ├── Arista.h             ├── Grafo.h
│   ├── ListaAdyacencia.h    ├── CargadorDatos.h      ├── Dijkstra.h
│   ├── ReconstructorRutas.h ├── Renderizador.h       ├── Optimizador.h
│   ├── IntegracionGoogleMaps.h └── InterfazGrafica.h
├── src/
│   ├── Nodo.cpp             ├── Arista.cpp           ├── Grafo.cpp
│   ├── ListaAdyacencia.cpp  ├── CargadorDatos.cpp    ├── Dijkstra.cpp
│   ├── ReconstructorRutas.cpp ├── Renderizador.cpp   ├── Optimizador.cpp
│   ├── IntegracionGoogleMaps.cpp └── InterfazGrafica.cpp
├── data/
│   ├── edificios.txt
│   ├── conexiones.txt
│   └── configuracion.txt
├── main.cpp
└── README.md