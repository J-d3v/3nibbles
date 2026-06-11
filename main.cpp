// Equipo 4 - Actividad Integradora 2

#include "algorithms.h"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<long long>> dist(n, vector<long long>(n));
    vector<vector<long long>> cap(n, vector<long long>(n));
    vector<Punto> centrales(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> dist[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> cap[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        centrales[i] = leerPunto();
    }
    vector<pair<int, int>> arcos = prim(dist);
    vector<int> ruta = tsp(dist);
    long long flujo = flujoMaximo(cap);
    vector<vector<Punto>> poligonos = voronoi(centrales);
    imprimirArcos(arcos);
    imprimirRuta(ruta);
    cout << flujo << '\n';
    imprimirPoligonos(poligonos);
    return 0;
}
