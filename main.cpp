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

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define endl '\n'
#define pb push_back
#define FOR(i, a, n) for (int i = a; i < n; i++)

const long long inf = numeric_limits<long long>::max() / 4;
const double eps = 1e-9;

struct punto {
    double x;
    double y;
};

struct arista_flujo {
    int to;
    int rev;
    long long cap;
};

string nombre_nodo(int index) {
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

punto leer_punto() {
    string texto;
    char c;

    while (cin >> c) {
        if (c == '(') break;
    }

    getline(cin, texto, ')');
    replace(texto.begin(), texto.end(), ',', ' ');

    stringstream ss(texto);
    punto p;
    ss >> p.x >> p.y;

    return p;
}

vector<pair<int, int> > prim(const vector<vector<long long> > &dist) {
    // cableado minimo con mst
    int n = dist.size();
    vector<long long> mejor(n, inf);
    vector<int> padre(n, -1);
    vector<bool> usado(n, false);
    vector<pair<int, int> > ans;

    mejor[0] = 0;

    FOR(paso, 0, n) {
        int u = -1;

        FOR(i, 0, n) {
            if (!usado[i] && (u == -1 || mejor[i] < mejor[u])) u = i;
        }

        if (u == -1 || mejor[u] == inf) break;

        usado[u] = true;

        if (padre[u] != -1) ans.pb({padre[u], u});

        FOR(v, 0, n) {
            if (!usado[v] && u != v && dist[u][v] > 0 && dist[u][v] < mejor[v]) {
                mejor[v] = dist[u][v];
                padre[v] = u;
            }
        }
    }

    return ans;
}

vector<int> tsp_dp(const vector<vector<long long> > &dist) {
    // tsp exacto para n chico
    int n = dist.size();
    int total = 1 << n;

    vector<vector<long long> > dp(total, vector<long long>(n, inf));
    vector<vector<int> > padre(total, vector<int>(n, -1));

    dp[1][0] = 0;

    FOR(mask, 1, total) {
        if ((mask & 1) == 0) continue;

        FOR(u, 0, n) {
            if ((mask & (1 << u)) == 0 || dp[mask][u] == inf) continue;

            FOR(v, 0, n) {
                if ((mask & (1 << v)) != 0 || dist[u][v] <= 0) continue;

                int sig = mask | (1 << v);
                long long costo = dp[mask][u] + dist[u][v];

                if (costo < dp[sig][v]) {
                    dp[sig][v] = costo;
                    padre[sig][v] = u;
                }
            }
        }
    }

    int completo = total - 1;
    long long mejor = inf;
    int ultimo = -1;

    FOR(u, 1, n) {
        if (dist[u][0] > 0 && dp[completo][u] != inf) {
            long long costo = dp[completo][u] + dist[u][0];
            if (costo < mejor) {
                mejor = costo;
                ultimo = u;
            }
        }
    }

    vector<int> ruta;
    if (ultimo == -1) return ruta;

    int mask = completo;
    int actual = ultimo;

    while (actual != -1) {
        ruta.pb(actual);
        int prev = padre[mask][actual];
        mask ^= (1 << actual);
        actual = prev;
    }

    reverse(ruta.begin(), ruta.end());
    ruta.pb(0);

    return ruta;
}

vector<int> tsp_greedy(const vector<vector<long long> > &dist) {
    // fallback para casos grandes
    int n = dist.size();
    vector<bool> usado(n, false);
    vector<int> ruta;

    ruta.pb(0);
    usado[0] = true;

    FOR(paso, 1, n) {
        int u = ruta.back();
        int mejor = -1;

        FOR(v, 0, n) {
            if (!usado[v] && dist[u][v] > 0 && (mejor == -1 || dist[u][v] < dist[u][mejor])) {
                mejor = v;
            }
        }

        if (mejor == -1) return vector<int>();

        usado[mejor] = true;
        ruta.pb(mejor);
    }

    if (dist[ruta.back()][0] <= 0) return vector<int>();

    ruta.pb(0);

    bool cambio = true;
    while (cambio) {
        cambio = false;

        FOR(i, 1, n - 1) {
            FOR(j, i + 1, n) {
                int a = ruta[i - 1];
                int b = ruta[i];
                int c = ruta[j];
                int d = ruta[j + 1];

                if (dist[a][c] <= 0 || dist[b][d] <= 0) continue;

                long long actual = dist[a][b] + dist[c][d];
                long long nuevo = dist[a][c] + dist[b][d];

                if (nuevo < actual) {
                    reverse(ruta.begin() + i, ruta.begin() + j + 1);
                    cambio = true;
                }
            }
        }
    }

    return ruta;
}

vector<int> tsp(const vector<vector<long long> > &dist) {
    int n = dist.size();

    if (n <= 20) return tsp_dp(dist);

    return tsp_greedy(dist);
}

class dinic {
private:
    int n;
    vector<vector<arista_flujo> > grafo;
    vector<int> nivel;
    vector<int> trabajo;

public:
    dinic(int tam) {
        n = tam;
        grafo.assign(n, vector<arista_flujo>());
        nivel.assign(n, 0);
        trabajo.assign(n, 0);
    }

    void add_edge(int from, int to, long long cap) {
        arista_flujo a = {to, (int)grafo[to].size(), cap};
        arista_flujo b = {from, (int)grafo[from].size(), 0};

        grafo[from].pb(a);
        grafo[to].pb(b);
    }

    bool bfs(int source, int sink) {
        fill(nivel.begin(), nivel.end(), -1);
        queue<int> q;

        nivel[source] = 0;
        q.push(source);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const arista_flujo &e : grafo[u]) {
                if (e.cap > 0 && nivel[e.to] == -1) {
                    nivel[e.to] = nivel[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return nivel[sink] != -1;
    }

    long long dfs(int u, int sink, long long flow) {
        if (u == sink) return flow;

        for (int &i = trabajo[u]; i < (int)grafo[u].size(); i++) {
            arista_flujo &e = grafo[u][i];

            if (e.cap <= 0 || nivel[e.to] != nivel[u] + 1) continue;

            long long pushed = dfs(e.to, sink, min(flow, e.cap));

            if (pushed > 0) {
                e.cap -= pushed;
                grafo[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }

        return 0;
    }

    long long max_flow(int source, int sink) {
        long long ans = 0;

        while (bfs(source, sink)) {
            fill(trabajo.begin(), trabajo.end(), 0);

            while (true) {
                long long pushed = dfs(source, sink, inf);
                if (pushed == 0) break;
                ans += pushed;
            }
        }

        return ans;
    }
};

long long flujo_maximo(const vector<vector<long long> > &cap) {
    // source a, sink ultimo nodo
    int n = cap.size();
    dinic red(n);

    FOR(i, 0, n) {
        FOR(j, 0, n) {
            if (i != j && cap[i][j] > 0) red.add_edge(i, j, cap[i][j]);
        }
    }

    return red.max_flow(0, n - 1);
}

punto interseccion(punto a, punto b, double A, double B, double C) {
    double va = A * a.x + B * a.y - C;
    double vb = A * b.x + B * b.y - C;
    double t = va / (va - vb);

    return {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
}

vector<punto> cortar_poligono(const vector<punto> &poly, double A, double B, double C) {
    vector<punto> ans;

    if (poly.empty()) return ans;

    FOR(i, 0, (int)poly.size()) {
        punto actual = poly[i];
        punto prev = poly[(i + poly.size() - 1) % poly.size()];

        bool in_actual = A * actual.x + B * actual.y <= C + eps;
        bool in_prev = A * prev.x + B * prev.y <= C + eps;

        if (in_actual != in_prev) ans.pb(interseccion(prev, actual, A, B, C));
        if (in_actual) ans.pb(actual);
    }

    return ans;
}

vector<vector<punto> > voronoi(const vector<punto> &puntos) {
    // celdas de voronoi con caja limite
    int n = puntos.size();
    vector<vector<punto> > ans;

    double minx = puntos[0].x;
    double maxx = puntos[0].x;
    double miny = puntos[0].y;
    double maxy = puntos[0].y;

    FOR(i, 0, n) {
        minx = min(minx, puntos[i].x);
        maxx = max(maxx, puntos[i].x);
        miny = min(miny, puntos[i].y);
        maxy = max(maxy, puntos[i].y);
    }

    double span = max(maxx - minx, maxy - miny);
    if (span < 1.0) span = 1.0;

    double margen = span * 2.0 + 10.0;

    minx -= margen;
    maxx += margen;
    miny -= margen;
    maxy += margen;

    vector<punto> caja;
    caja.pb({minx, miny});
    caja.pb({maxx, miny});
    caja.pb({maxx, maxy});
    caja.pb({minx, maxy});

    FOR(i, 0, n) {
        vector<punto> celda = caja;

        FOR(j, 0, n) {
            if (i == j) continue;

            double A = 2.0 * (puntos[j].x - puntos[i].x);
            double B = 2.0 * (puntos[j].y - puntos[i].y);
            double C = puntos[j].x * puntos[j].x + puntos[j].y * puntos[j].y
                     - puntos[i].x * puntos[i].x - puntos[i].y * puntos[i].y;

            celda = cortar_poligono(celda, A, B, C);
        }

        ans.pb(celda);
    }

    return ans;
}

void imprimir_arcos(const vector<pair<int, int> > &arcos) {
    FOR(i, 0, (int)arcos.size()) {
        cout << "(" << nombre_nodo(arcos[i].first) << "," << nombre_nodo(arcos[i].second) << ")" << endl;
    }
}

void imprimir_ruta(const vector<int> &ruta) {
    if (ruta.empty()) {
        cout << "no existe ruta" << endl;
        return;
    }

    FOR(i, 0, (int)ruta.size()) {
        if (i > 0) cout << " ";
        cout << nombre_nodo(ruta[i]);
    }

    cout << endl;
}

double limpiar_cero(double x) {
    if (fabs(x) < 0.0005) return 0.0;
    return x;
}

void imprimir_poligonos(const vector<vector<punto> > &poligonos) {
    cout << fixed << setprecision(2);

    FOR(i, 0, (int)poligonos.size()) {
        cout << "[";

        FOR(j, 0, (int)poligonos[i].size()) {
            if (j > 0) cout << " ";
            cout << "(" << limpiar_cero(poligonos[i][j].x) << "," << limpiar_cero(poligonos[i][j].y) << ")";
        }

        cout << "]" << endl;
    }
}

signed main() { _
    int n;
    cin >> n;

    vector<vector<long long> > dist(n, vector<long long>(n));
    vector<vector<long long> > cap(n, vector<long long>(n));
    vector<punto> centrales(n);

    FOR(i, 0, n) {
        FOR(j, 0, n) cin >> dist[i][j];
    }

    FOR(i, 0, n) {
        FOR(j, 0, n) cin >> cap[i][j];
    }

    FOR(i, 0, n) {
        centrales[i] = leer_punto();
    }

    vector<pair<int, int> > arcos = prim(dist);
    vector<int> ruta = tsp(dist);
    long long flujo = flujo_maximo(cap);
    vector<vector<punto> > poligonos = voronoi(centrales);

    imprimir_arcos(arcos);
    imprimir_ruta(ruta);
    cout << flujo << endl;
    imprimir_poligonos(poligonos);

    return 0;
}