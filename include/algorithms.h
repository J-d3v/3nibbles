// Equipo 4 - Actividad Integradora 2
// Declaraciones de algoritmos: MST (Prim), TSP (Held-Karp/Greedy),
// Flujo maximo (Dinic), Diagramas de Voronoi.

#pragma once

#include <string>
#include <utility>
#include <vector>

struct Punto {
    double x;
    double y;
};

struct AristaFlujo {
    int to;
    int rev;
    long long cap;
};

class Dinic {
private:
    std::vector<std::vector<AristaFlujo>> grafo;
    std::vector<int> nivel;
    std::vector<int> trabajo;

    bool BreadthFirstSearch(int source, int sink);
    long long DepthFirstSearch(int u, int sink, long long flow);

public:
    explicit Dinic(int tam);
    void AddEdge(int from, int to, long long cap);
    long long MaxFlow(int source, int sink);
};

std::string nombreNodo(int index);
Punto leerPunto();

std::vector<std::pair<int, int>> prim(
    const std::vector<std::vector<long long>>& dist);

std::vector<int> tspDp(const std::vector<std::vector<long long>>& dist);
std::vector<int> tspGreedy(const std::vector<std::vector<long long>>& dist);
std::vector<int> tsp(const std::vector<std::vector<long long>>& dist);

long long flujoMaximo(const std::vector<std::vector<long long>>& cap);

Punto interseccion(Punto a, Punto b, double A, double B, double C);
std::vector<Punto> cortarPoligono(
    const std::vector<Punto>& poly, double A, double B, double C);
std::vector<std::vector<Punto>> voronoi(const std::vector<Punto>& puntos);

void imprimirArcos(const std::vector<std::pair<int, int>>& arcos);
void imprimirRuta(const std::vector<int>& ruta);
double limpiarCero(double x);
void imprimirPoligonos(const std::vector<std::vector<Punto>>& poligonos);
