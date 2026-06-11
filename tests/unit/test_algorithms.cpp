// Equipo 4 - Pruebas unitarias de algoritmos

#include <gtest/gtest.h>
#include "algorithms.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ==================== nombreNodo ====================

TEST(NombreNodoTest, FirstNode) {
    EXPECT_EQ(nombreNodo(0), "A");
}

TEST(NombreNodoTest, SecondNode) {
    EXPECT_EQ(nombreNodo(1), "B");
}

TEST(NombreNodoTest, LastSingleChar) {
    EXPECT_EQ(nombreNodo(25), "Z");
}

TEST(NombreNodoTest, FirstDoubleChar) {
    EXPECT_EQ(nombreNodo(26), "AA");
}

TEST(NombreNodoTest, SecondDoubleChar) {
    EXPECT_EQ(nombreNodo(27), "AB");
}

// ==================== leerPunto ====================

TEST(LeerPuntoTest, SimplePoint) {
    istringstream input("(1.5, 2.5)");
    streambuf* orig = cin.rdbuf(input.rdbuf());
    Punto p = leerPunto();
    cin.rdbuf(orig);
    EXPECT_NEAR(p.x, 1.5, 1e-9);
    EXPECT_NEAR(p.y, 2.5, 1e-9);
}

TEST(LeerPuntoTest, NegativeCoords) {
    istringstream input("(-3.0,-4.5)");
    streambuf* orig = cin.rdbuf(input.rdbuf());
    Punto p = leerPunto();
    cin.rdbuf(orig);
    EXPECT_NEAR(p.x, -3.0, 1e-9);
    EXPECT_NEAR(p.y, -4.5, 1e-9);
}

TEST(LeerPuntoTest, ZeroPoint) {
    istringstream input("(0,0)");
    streambuf* orig = cin.rdbuf(input.rdbuf());
    Punto p = leerPunto();
    cin.rdbuf(orig);
    EXPECT_NEAR(p.x, 0.0, 1e-9);
    EXPECT_NEAR(p.y, 0.0, 1e-9);
}

// ==================== prim ====================

TEST(PrimTest, FourNodeGraph) {
    vector<vector<long long>> dist = {
        {0, 2, 0, 6},
        {2, 0, 3, 8},
        {0, 3, 0, 7},
        {6, 8, 7, 0}
    };
    auto result = prim(dist);
    EXPECT_EQ(static_cast<int>(result.size()), 3);
}

TEST(PrimTest, SingleNode) {
    vector<vector<long long>> dist = {{0}};
    auto result = prim(dist);
    EXPECT_TRUE(result.empty());
}

TEST(PrimTest, TwoNodes) {
    vector<vector<long long>> dist = {
        {0, 5},
        {5, 0}
    };
    auto result = prim(dist);
    EXPECT_EQ(static_cast<int>(result.size()), 1);
}

TEST(PrimTest, DisconnectedGraph) {
    vector<vector<long long>> dist = {
        {0, 0},
        {0, 0}
    };
    auto result = prim(dist);
    EXPECT_TRUE(result.empty());
}

TEST(PrimTest, FullyConnected) {
    vector<vector<long long>> dist = {
        {0, 1, 4, 3},
        {1, 0, 2, 5},
        {4, 2, 0, 1},
        {3, 5, 1, 0}
    };
    auto result = prim(dist);
    EXPECT_EQ(static_cast<int>(result.size()), 3);
}

// ==================== tspDp ====================

TEST(TspDpTest, FourNodeComplete) {
    vector<vector<long long>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    auto result = tspDp(dist);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.front(), 0);
    EXPECT_EQ(result.back(), 0);
}

TEST(TspDpTest, NoReturnPath) {
    vector<vector<long long>> dist = {
        {0, 1, 0},
        {0, 0, 1},
        {0, 0, 0}
    };
    auto result = tspDp(dist);
    EXPECT_TRUE(result.empty());
}

TEST(TspDpTest, TwoNodes) {
    vector<vector<long long>> dist = {
        {0, 5},
        {5, 0}
    };
    auto result = tspDp(dist);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.front(), 0);
    EXPECT_EQ(result.back(), 0);
}

TEST(TspDpTest, SkipImpossibleTransitions) {
    vector<vector<long long>> dist = {
        {0, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 1, 1, 0}
    };
    auto result = tspDp(dist);
    EXPECT_FALSE(result.empty());
}

// ==================== tspGreedy ====================

TEST(TspGreedyTest, FourNodeGraph) {
    vector<vector<long long>> dist = {
        {0, 1, 2, 3},
        {1, 0, 1, 2},
        {2, 1, 0, 1},
        {3, 2, 1, 0}
    };
    auto result = tspGreedy(dist);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.front(), 0);
    EXPECT_EQ(result.back(), 0);
}

TEST(TspGreedyTest, NoReturnToStart) {
    vector<vector<long long>> dist = {
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 0}
    };
    auto result = tspGreedy(dist);
    EXPECT_TRUE(result.empty());
}

TEST(TspGreedyTest, NoNeighbor) {
    vector<vector<long long>> dist = {
        {0, 0},
        {0, 0}
    };
    auto result = tspGreedy(dist);
    EXPECT_TRUE(result.empty());
}

TEST(TspGreedyTest, With2OptImprovement) {
    vector<vector<long long>> dist = {
        {0, 2, 9, 10},
        {1, 0, 6,  4},
        {15, 7, 0, 8},
        {6,  3, 12, 0}
    };
    auto result = tspGreedy(dist);
    EXPECT_FALSE(result.empty());
}

// ==================== tsp (routing) ====================

TEST(TspTest, SmallUsesExact) {
    vector<vector<long long>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    auto result = tsp(dist);
    EXPECT_FALSE(result.empty());
}

TEST(TspTest, LargeUsesGreedy) {
    int n = 21;
    vector<vector<long long>> dist(n, vector<long long>(n, 1));
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }
    auto result = tsp(dist);
    EXPECT_FALSE(result.empty());
}

// ==================== Dinic / flujoMaximo ====================

TEST(DinicTest, SimpleFlow) {
    Dinic d(4);
    d.AddEdge(0, 1, 3);
    d.AddEdge(0, 2, 2);
    d.AddEdge(1, 3, 2);
    d.AddEdge(2, 3, 3);
    EXPECT_EQ(d.MaxFlow(0, 3), 4);
}

TEST(DinicTest, NoPath) {
    Dinic d(3);
    d.AddEdge(0, 1, 5);
    EXPECT_EQ(d.MaxFlow(0, 2), 0);
}

TEST(DinicTest, SingleEdge) {
    Dinic d(2);
    d.AddEdge(0, 1, 7);
    EXPECT_EQ(d.MaxFlow(0, 1), 7);
}

TEST(DinicTest, MultipleAugmentingPaths) {
    Dinic d(6);
    d.AddEdge(0, 1, 10);
    d.AddEdge(0, 2, 10);
    d.AddEdge(1, 3, 10);
    d.AddEdge(2, 4, 10);
    d.AddEdge(3, 5, 10);
    d.AddEdge(4, 5, 10);
    EXPECT_EQ(d.MaxFlow(0, 5), 20);
}

TEST(FlujoMaximoTest, SimpleNetwork) {
    vector<vector<long long>> cap = {
        {0, 10, 10, 0},
        {0, 0,  2,  4},
        {0, 0,  0,  8},
        {0, 0,  0,  0}
    };
    EXPECT_EQ(flujoMaximo(cap), 12);
}

TEST(FlujoMaximoTest, NoEdges) {
    vector<vector<long long>> cap = {
        {0, 0},
        {0, 0}
    };
    EXPECT_EQ(flujoMaximo(cap), 0);
}

TEST(FlujoMaximoTest, SingleEdge) {
    vector<vector<long long>> cap = {
        {0, 5},
        {0, 0}
    };
    EXPECT_EQ(flujoMaximo(cap), 5);
}

// ==================== Geometria ====================

TEST(InterseccionTest, DiagonalWithVertical) {
    Punto a = {0.0, 0.0};
    Punto b = {2.0, 2.0};
    Punto result = interseccion(a, b, 1.0, 0.0, 1.0);
    EXPECT_NEAR(result.x, 1.0, 1e-9);
    EXPECT_NEAR(result.y, 1.0, 1e-9);
}

TEST(InterseccionTest, HorizontalLine) {
    Punto a = {0.0, 0.0};
    Punto b = {0.0, 4.0};
    Punto result = interseccion(a, b, 0.0, 1.0, 2.0);
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 2.0, 1e-9);
}

TEST(CortarPoligonoTest, EmptyPolygon) {
    vector<Punto> poly = {};
    auto result = cortarPoligono(poly, 1.0, 0.0, 0.0);
    EXPECT_TRUE(result.empty());
}

TEST(CortarPoligonoTest, SquareCutByVertical) {
    vector<Punto> poly = {{-1.0,-1.0},{1.0,-1.0},{1.0,1.0},{-1.0,1.0}};
    auto result = cortarPoligono(poly, 1.0, 0.0, 0.0);
    EXPECT_FALSE(result.empty());
}

TEST(CortarPoligonoTest, AllOutside) {
    vector<Punto> poly = {{1.0,0.0},{2.0,0.0},{2.0,1.0},{1.0,1.0}};
    auto result = cortarPoligono(poly, 1.0, 0.0, -1.0);
    EXPECT_TRUE(result.empty());
}

TEST(CortarPoligonoTest, AllInside) {
    vector<Punto> poly = {{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0}};
    auto result = cortarPoligono(poly, 1.0, 0.0, 5.0);
    EXPECT_EQ(result.size(), poly.size());
}

TEST(CortarPoligonoTest, PartialCross) {
    vector<Punto> poly = {{-2.0,0.0},{2.0,0.0},{2.0,2.0},{-2.0,2.0}};
    auto result = cortarPoligono(poly, 1.0, 0.0, 1.0);
    EXPECT_FALSE(result.empty());
}

TEST(VoronoiTest, TwoPoints) {
    vector<Punto> pts = {{0.0, 0.0}, {4.0, 0.0}};
    auto result = voronoi(pts);
    EXPECT_EQ(static_cast<int>(result.size()), 2);
    EXPECT_FALSE(result[0].empty());
    EXPECT_FALSE(result[1].empty());
}

TEST(VoronoiTest, ThreePoints) {
    vector<Punto> pts = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0}};
    auto result = voronoi(pts);
    EXPECT_EQ(static_cast<int>(result.size()), 3);
}

TEST(VoronoiTest, SmallSpan) {
    vector<Punto> pts = {{0.0, 0.0}, {0.1, 0.0}};
    auto result = voronoi(pts);
    EXPECT_EQ(static_cast<int>(result.size()), 2);
}

// ==================== limpiarCero ====================

TEST(LimpiarCeroTest, SmallPositive) {
    EXPECT_NEAR(limpiarCero(0.0001), 0.0, 1e-9);
}

TEST(LimpiarCeroTest, SmallNegative) {
    EXPECT_NEAR(limpiarCero(-0.0001), 0.0, 1e-9);
}

TEST(LimpiarCeroTest, NotNearZero) {
    EXPECT_NEAR(limpiarCero(1.5), 1.5, 1e-9);
}

TEST(LimpiarCeroTest, ExactZero) {
    EXPECT_NEAR(limpiarCero(0.0), 0.0, 1e-9);
}

// ==================== Funciones de salida ====================

TEST(ImprimirArcosTest, TwoEdges) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimirArcos({{0, 1}, {1, 2}});
    cout.rdbuf(orig);
    string s = out.str();
    EXPECT_NE(s.find("(A,B)"), string::npos);
    EXPECT_NE(s.find("(B,C)"), string::npos);
}

TEST(ImprimirArcosTest, EmptyArcos) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimirArcos({});
    cout.rdbuf(orig);
    EXPECT_TRUE(out.str().empty());
}

TEST(ImprimirRutaTest, ValidRoute) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimirRuta({0, 1, 2, 0});
    cout.rdbuf(orig);
    EXPECT_NE(out.str().find("A"), string::npos);
}

TEST(ImprimirRutaTest, EmptyRoute) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimirRuta({});
    cout.rdbuf(orig);
    EXPECT_NE(out.str().find("no existe ruta"), string::npos);
}

TEST(ImprimirPoligonosTest, SimplePolygon) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    vector<vector<Punto>> polys = {{{1.0, 2.0}, {3.0, 4.0}}};
    imprimirPoligonos(polys);
    cout.rdbuf(orig);
    EXPECT_NE(out.str().find("["), string::npos);
}

TEST(ImprimirPoligonosTest, MultiplePolygons) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    vector<vector<Punto>> polys = {
        {{0.0, 0.0}, {1.0, 0.0}},
        {{2.0, 0.0}, {3.0, 1.0}}
    };
    imprimirPoligonos(polys);
    cout.rdbuf(orig);
    string s = out.str();
    EXPECT_NE(s.find("]"), string::npos);
}
