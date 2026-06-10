// Equipo 4 - Actividad Integradora 2
// Algoritmos: MST (Prim), TSP (Held-Karp / Greedy 2-opt),
// Flujo maximo (Dinic), Diagramas de Voronoi.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;
const double EPS = 1e-9;

struct Punto {
    double x;
    double y;
};

struct AristaFlujo {
    int to;
    int rev;
    long long cap;
};

string nombreNodo(int index) {
    string nombre;
    index++;
    while (index > 0) {
        index--;
        nombre += char('A' + (index % 26));
        index /= 26;
    }
    reverse(nombre.begin(), nombre.end());
    return nombre;
}

Punto leerPunto() {
    string texto;
    char c;
    while (cin >> c) {
        if (c == '(') {
            break;
        }
    }
    getline(cin, texto, ')');
    replace(texto.begin(), texto.end(), ',', ' ');
    stringstream ss(texto);
    Punto p;
    ss >> p.x >> p.y;
    return p;
}

static int encontrarMinimo(const vector<long long>& mejor,
                            const vector<bool>& usado, int n) {
    int u = -1;
    for (int i = 0; i < n; i++) {
        if (!usado[i] && (u == -1 || mejor[i] < mejor[u])) {
            u = i;
        }
    }
    return u;
}

static void relajarVecinos(int u, const vector<vector<long long>>& dist,
                            vector<long long>& mejor, vector<int>& padre,
                            const vector<bool>& usado) {
    int n = static_cast<int>(dist.size());
    for (int v = 0; v < n; v++) {
        if (!usado[v] && u != v && dist[u][v] > 0 && dist[u][v] < mejor[v]) {
            mejor[v] = dist[u][v];
            padre[v] = u;
        }
    }
}

vector<pair<int, int>> prim(const vector<vector<long long>>& dist) {
    int n = static_cast<int>(dist.size());
    vector<long long> mejor(n, INF);
    vector<int> padre(n, -1);
    vector<bool> usado(n, false);
    vector<pair<int, int>> ans;
    mejor[0] = 0;
    for (int paso = 0; paso < n; paso++) {
        int u = encontrarMinimo(mejor, usado, n);
        if (u == -1 || mejor[u] == INF) {
            break;
        }
        usado[u] = true;
        if (padre[u] != -1) {
            ans.push_back({padre[u], u});
        }
        relajarVecinos(u, dist, mejor, padre, usado);
    }
    return ans;
}

static vector<int> reconstruirTsp(const vector<vector<int>>& padre,
                                   int completo, int ultimo) {
    vector<int> ruta;
    int mask = completo;
    int actual = ultimo;
    while (actual != -1) {
        ruta.push_back(actual);
        int prev = padre[mask][actual];
        mask ^= (1 << actual);
        actual = prev;
    }
    reverse(ruta.begin(), ruta.end());
    ruta.push_back(0);
    return ruta;
}

static int encontrarUltimoTsp(const vector<vector<long long>>& dp,
                               const vector<vector<long long>>& dist,
                               int completo, int n) {
    long long mejor = INF;
    int ultimo = -1;
    for (int u = 1; u < n; u++) {
        if (dist[u][0] > 0 && dp[completo][u] != INF) {
            long long costo = dp[completo][u] + dist[u][0];
            if (costo < mejor) {
                mejor = costo;
                ultimo = u;
            }
        }
    }
    return ultimo;
}

static void procesarTransiciones(int mask, int u,
                                  const vector<vector<long long>>& dist,
                                  vector<vector<long long>>& dp,
                                  vector<vector<int>>& padre, int n) {
    for (int v = 0; v < n; v++) {
        if ((mask & (1 << v)) != 0 || dist[u][v] <= 0) {
            continue;
        }
        int sig = mask | (1 << v);
        long long costo = dp[mask][u] + dist[u][v];
        if (costo < dp[sig][v]) {
            dp[sig][v] = costo;
            padre[sig][v] = u;
        }
    }
}

vector<int> tspDp(const vector<vector<long long>>& dist) {
    int n = static_cast<int>(dist.size());
    int total = 1 << n;
    vector<vector<long long>> dp(total, vector<long long>(n, INF));
    vector<vector<int>> padre(total, vector<int>(n, -1));
    dp[1][0] = 0;
    for (int mask = 1; mask < total; mask++) {
        if ((mask & 1) == 0) {
            continue;
        }
        for (int u = 0; u < n; u++) {
            if ((mask & (1 << u)) == 0 || dp[mask][u] == INF) {
                continue;
            }
            procesarTransiciones(mask, u, dist, dp, padre, n);
        }
    }
    int completo = total - 1;
    int ultimo = encontrarUltimoTsp(dp, dist, completo, n);
    if (ultimo == -1) {
        return vector<int>();
    }
    return reconstruirTsp(padre, completo, ultimo);
}

static int encontrarVecinoMasCercano(int u, const vector<vector<long long>>& dist,
                                      const vector<bool>& usado, int n) {
    int mejor = -1;
    for (int v = 0; v < n; v++) {
        if (!usado[v] && dist[u][v] > 0 &&
                (mejor == -1 || dist[u][v] < dist[u][mejor])) {
            mejor = v;
        }
    }
    return mejor;
}

static bool aplicar2opt(vector<int>& ruta,
                         const vector<vector<long long>>& dist, int n) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int a = ruta[i - 1];
            int b = ruta[i];
            int c = ruta[j];
            int d = ruta[j + 1];
            if (dist[a][c] <= 0 || dist[b][d] <= 0) {
                continue;
            }
            long long actual = dist[a][b] + dist[c][d];
            long long nuevo = dist[a][c] + dist[b][d];
            if (nuevo < actual) {
                reverse(ruta.begin() + i, ruta.begin() + j + 1);
                return true;
            }
        }
    }
    return false;
}

vector<int> tspGreedy(const vector<vector<long long>>& dist) {
    int n = static_cast<int>(dist.size());
    vector<bool> usado(n, false);
    vector<int> ruta;
    ruta.push_back(0);
    usado[0] = true;
    for (int paso = 1; paso < n; paso++) {
        int u = ruta.back();
        int mejor = encontrarVecinoMasCercano(u, dist, usado, n);
        if (mejor == -1) {
            return vector<int>();
        }
        usado[mejor] = true;
        ruta.push_back(mejor);
    }
    if (dist[ruta.back()][0] <= 0) {
        return vector<int>();
    }
    ruta.push_back(0);
    bool cambio = true;
    while (cambio) {
        cambio = aplicar2opt(ruta, dist, n);
    }
    return ruta;
}

vector<int> tsp(const vector<vector<long long>>& dist) {
    int n = static_cast<int>(dist.size());
    if (n <= 20) {
        return tspDp(dist);
    }
    return tspGreedy(dist);
}

class Dinic {
private:
    vector<vector<AristaFlujo>> grafo;
    vector<int> nivel;
    vector<int> trabajo;

public:
    explicit Dinic(int tam) : grafo(tam), nivel(tam, 0), trabajo(tam, 0) {}

    void AddEdge(int from, int to, long long cap) {
        AristaFlujo a = {to, static_cast<int>(grafo[to].size()), cap};
        AristaFlujo b = {from, static_cast<int>(grafo[from].size()), 0};
        grafo[from].push_back(a);
        grafo[to].push_back(b);
    }

    bool BreadthFirstSearch(int source, int sink) {
        fill(nivel.begin(), nivel.end(), -1);
        queue<int> q;
        nivel[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const AristaFlujo& e : grafo[u]) {
                if (e.cap > 0 && nivel[e.to] == -1) {
                    nivel[e.to] = nivel[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return nivel[sink] != -1;
    }

    long long DepthFirstSearch(int u, int sink, long long flow) {
        if (u == sink) {
            return flow;
        }
        for (int& i = trabajo[u]; i < static_cast<int>(grafo[u].size()); i++) {
            AristaFlujo& e = grafo[u][i];
            if (e.cap <= 0 || nivel[e.to] != nivel[u] + 1) {
                continue;
            }
            long long pushed = DepthFirstSearch(e.to, sink, min(flow, e.cap));
            if (pushed > 0) {
                e.cap -= pushed;
                grafo[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
        return 0;
    }

    long long MaxFlow(int source, int sink) {
        long long ans = 0;
        while (BreadthFirstSearch(source, sink)) {
            fill(trabajo.begin(), trabajo.end(), 0);
            while (true) {
                long long pushed = DepthFirstSearch(source, sink, INF);
                if (pushed == 0) {
                    break;
                }
                ans += pushed;
            }
        }
        return ans;
    }
};

long long flujoMaximo(const vector<vector<long long>>& cap) {
    int n = static_cast<int>(cap.size());
    Dinic red(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && cap[i][j] > 0) {
                red.AddEdge(i, j, cap[i][j]);
            }
        }
    }
    return red.MaxFlow(0, n - 1);
}

Punto interseccion(Punto a, Punto b, double A, double B, double C) {
    double va = A * a.x + B * a.y - C;
    double vb = A * b.x + B * b.y - C;
    double t = va / (va - vb);
    return {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
}

vector<Punto> cortarPoligono(const vector<Punto>& poly,
                              double A, double B, double C) {
    vector<Punto> ans;
    if (poly.empty()) {
        return ans;
    }
    for (int i = 0; i < static_cast<int>(poly.size()); i++) {
        Punto actual = poly[i];
        Punto prev = poly[(i + poly.size() - 1) % poly.size()];
        bool inActual = A * actual.x + B * actual.y <= C + EPS;
        bool inPrev = A * prev.x + B * prev.y <= C + EPS;
        if (inActual != inPrev) {
            ans.push_back(interseccion(prev, actual, A, B, C));
        }
        if (inActual) {
            ans.push_back(actual);
        }
    }
    return ans;
}

static void calcularBounds(const vector<Punto>& puntos,
                            double& minx, double& maxx,
                            double& miny, double& maxy) {
    minx = puntos[0].x;
    maxx = puntos[0].x;
    miny = puntos[0].y;
    maxy = puntos[0].y;
    for (const Punto& p : puntos) {
        minx = min(minx, p.x);
        maxx = max(maxx, p.x);
        miny = min(miny, p.y);
        maxy = max(maxy, p.y);
    }
}

vector<vector<Punto>> voronoi(const vector<Punto>& puntos) {
    int n = static_cast<int>(puntos.size());
    double minx, maxx, miny, maxy;
    calcularBounds(puntos, minx, maxx, miny, maxy);
    double span = max(maxx - minx, maxy - miny);
    if (span < 1.0) {
        span = 1.0;
    }
    double margen = span * 2.0 + 10.0;
    minx -= margen;
    maxx += margen;
    miny -= margen;
    maxy += margen;
    vector<Punto> caja = {{minx, miny}, {maxx, miny}, {maxx, maxy}, {minx, maxy}};
    vector<vector<Punto>> ans;
    for (int i = 0; i < n; i++) {
        vector<Punto> celda = caja;
        for (int j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            double A = 2.0 * (puntos[j].x - puntos[i].x);
            double B = 2.0 * (puntos[j].y - puntos[i].y);
            double C = puntos[j].x * puntos[j].x + puntos[j].y * puntos[j].y
                     - puntos[i].x * puntos[i].x - puntos[i].y * puntos[i].y;
            celda = cortarPoligono(celda, A, B, C);
        }
        ans.push_back(celda);
    }
    return ans;
}

void imprimirArcos(const vector<pair<int, int>>& arcos) {
    for (int i = 0; i < static_cast<int>(arcos.size()); i++) {
        cout << "(" << nombreNodo(arcos[i].first)
             << "," << nombreNodo(arcos[i].second) << ")" << '\n';
    }
}

void imprimirRuta(const vector<int>& ruta) {
    if (ruta.empty()) {
        cout << "no existe ruta" << '\n';
        return;
    }
    for (int i = 0; i < static_cast<int>(ruta.size()); i++) {
        if (i > 0) {
            cout << " ";
        }
        cout << nombreNodo(ruta[i]);
    }
    cout << '\n';
}

double limpiarCero(double x) {
    if (fabs(x) < 0.0005) {
        return 0.0;
    }
    return x;
}

void imprimirPoligonos(const vector<vector<Punto>>& poligonos) {
    cout << fixed << setprecision(2);
    for (int i = 0; i < static_cast<int>(poligonos.size()); i++) {
        cout << "[";
        for (int j = 0; j < static_cast<int>(poligonos[i].size()); j++) {
            if (j > 0) {
                cout << " ";
            }
            cout << "(" << limpiarCero(poligonos[i][j].x)
                 << "," << limpiarCero(poligonos[i][j].y) << ")";
        }
        cout << "]" << '\n';
    }
}

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
