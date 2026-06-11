// Copyright (c) 2026 Equipo 4 - Actividad Integradora 2. All rights reserved.

/**
 * @file algorithms.h
 * @brief Declaraciones de algoritmos: MST (Prim), TSP, Flujo maximo, Voronoi.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

/** @brief Punto en el plano con coordenadas cartesianas. */
struct Punto {
    double x; /**< Coordenada horizontal. */
    double y; /**< Coordenada vertical. */
};

/** @brief Arista de la red de flujo usada por el algoritmo de Dinic. */
struct AristaFlujo {
    int to;        /**< Nodo destino. */
    int rev;       /**< Indice de la arista inversa en el grafo. */
    long long cap; /**< Capacidad residual de la arista. */
};

/** @brief Implementacion del algoritmo de flujo maximo de Dinic. */
class Dinic {
private:
    std::vector<std::vector<AristaFlujo>> grafo;
    std::vector<int> nivel;
    std::vector<int> trabajo;

    bool BreadthFirstSearch(int source, int sink);
    long long DepthFirstSearch(int u, int sink, long long flow);

public:
    /** @brief Construye la red de flujo con @p tam nodos. */
    explicit Dinic(int tam);

    /** @brief Agrega una arista de @p from a @p to con capacidad @p cap. */
    void AddEdge(int from, int to, long long cap);

    /** @brief Calcula el flujo maximo desde @p source hasta @p sink. */
    long long MaxFlow(int source, int sink);
};

/** @brief Convierte un indice de nodo a su nombre alfabetico (A, B, AA, ...). */
std::string nombre_nodo(int index);

/** @brief Lee un punto en formato "(x,y)" desde la entrada estandar. */
Punto leer_punto();

/** @brief Calcula el Arbol de Expansion Minima con el algoritmo de Prim. */
std::vector<std::pair<int, int>> prim(
    const std::vector<std::vector<long long>>& dist);

/** @brief Resuelve el TSP de forma exacta con DP de bitmask (n <= 20). */
std::vector<int> tsp_dp(const std::vector<std::vector<long long>>& dist);

/** @brief Resuelve el TSP heuristicamente con vecino mas cercano y 2-opt. */
std::vector<int> tsp_greedy(const std::vector<std::vector<long long>>& dist);

/** @brief Selecciona el algoritmo TSP optimo segun el tamano de la instancia. */
std::vector<int> tsp(const std::vector<std::vector<long long>>& dist);

/** @brief Calcula el flujo maximo en una red de capacidades. */
long long flujo_maximo(const std::vector<std::vector<long long>>& cap);

/** @brief Calcula la interseccion de un segmento con la recta Ax+By=C. */
Punto interseccion(Punto a, Punto b, double A, double B, double C);

/** @brief Recorta un poligono convexo con el semiplano Ax+By <= C. */
std::vector<Punto> cortar_poligono(
    const std::vector<Punto>& poly, double A, double B, double C);

/** @brief Genera las celdas del diagrama de Voronoi para los puntos dados. */
std::vector<std::vector<Punto>> voronoi(const std::vector<Punto>& puntos);

/** @brief Imprime los arcos del MST en formato (X,Y) por linea. */
void imprimir_arcos(const std::vector<std::pair<int, int>>& arcos);

/** @brief Imprime la ruta TSP separada por espacios, o "no existe ruta". */
void imprimir_ruta(const std::vector<int>& ruta);

/** @brief Redondea a 0.0 valores cuyo valor absoluto sea menor a 0.0005. */
double limpiar_cero(double x);

/** @brief Imprime las celdas de Voronoi en formato [(x,y) (x,y) ...]. */
void imprimir_poligonos(const std::vector<std::vector<Punto>>& poligonos);
