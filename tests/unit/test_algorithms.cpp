/*
* Copyright (C) 2026 Equipo 4 - Actividad Integradora 2.
 * All rights reserved.
 */

#include <gtest/gtest.h>
#include "algorithms.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ==================== nombre_nodo ====================

TEST(NombreNodoTest, FirstNode) {
    EXPECT_EQ(nombre_nodo(0), "A");
}

TEST(NombreNodoTest, SecondNode) {
    EXPECT_EQ(nombre_nodo(1), "B");
}

TEST(NombreNodoTest, LastSingleChar) {
    EXPECT_EQ(nombre_nodo(25), "Z");
}

TEST(NombreNodoTest, FirstDoubleChar) {
    EXPECT_EQ(nombre_nodo(26), "AA");
}

TEST(NombreNodoTest, SecondDoubleChar) {
    EXPECT_EQ(nombre_nodo(27), "AB");
}

// ==================== leer_punto ====================

TEST(LeerPuntoTest, SimplePoint) {
    istringstream input("(1.5, 2.5)");
    streambuf* orig = cin.rdbuf(input.rdbuf());
    Punto p = leer_punto();
    cin.rdbuf(orig);
    EXPECT_NEAR(p.x, 1.5, 1e-9);
    EXPECT_NEAR(p.y, 2.5, 1e-9);
}

TEST(LeerPuntoTest, NegativeCoords) {
    istringstream input("(-3.0,-4.5)");
    streambuf* orig = cin.rdbuf(input.rdbuf());
    Punto p = leer_punto();
    cin.rdbuf(orig);
    EXPECT_NEAR(p.x, -3.0, 1e-9);
    EXPECT_NEAR(p.y, -4.5, 1e-9);
}

TEST(LeerPuntoTest, ZeroPoint) {
    istringstream input("(0,0)");
    streambuf* orig = cin.rdbuf(input.rdbuf());
    Punto p = leer_punto();
    cin.rdbuf(orig);
    EXPECT_NEAR(p.x, 0.0, 1e-9);
    EXPECT_NEAR(p.y, 0.0, 1e-9);
}

// ==================== kruskal ====================

TEST(KruskalTest, FourNodeGraph) {
    vector<vector<long long>> dist = {
        {0, 2, 0, 6},
        {2, 0, 3, 8},
        {0, 3, 0, 7},
        {6, 8, 7, 0}
    };
    auto result = kruskal(dist);
    EXPECT_EQ(static_cast<int>(result.size()), 3);
}

TEST(KruskalTest, SingleNode) {
    vector<vector<long long>> dist = {{0}};
    auto result = kruskal(dist);
    EXPECT_TRUE(result.empty());
}

TEST(KruskalTest, TwoNodes) {
    vector<vector<long long>> dist = {
        {0, 5},
        {5, 0}
    };
    auto result = kruskal(dist);
    EXPECT_EQ(static_cast<int>(result.size()), 1);
}

TEST(KruskalTest, DisconnectedGraph) {
    vector<vector<long long>> dist = {
        {0, 0},
        {0, 0}
    };
    auto result = kruskal(dist);
    EXPECT_TRUE(result.empty());
}

TEST(KruskalTest, FullyConnected) {
    vector<vector<long long>> dist = {
        {0, 1, 4, 3},
        {1, 0, 2, 5},
        {4, 2, 0, 1},
        {3, 5, 1, 0}
    };
    auto result = kruskal(dist);
    EXPECT_EQ(static_cast<int>(result.size()), 3);
}

TEST(KruskalTest, TestCase1) {
    vector<vector<long long>> dist = {
        {0, 16, 45, 32},
        {16, 0, 18, 21},
        {45, 18, 0, 7},
        {32, 21, 7, 0}
    };
    auto result = kruskal(dist);
    ASSERT_EQ(static_cast<int>(result.size()), 3);
    // Expected edges: (C,D)=7, (A,B)=16, (B,C)=18
    EXPECT_EQ(result[0], make_pair(2, 3));
    EXPECT_EQ(result[1], make_pair(0, 1));
    EXPECT_EQ(result[2], make_pair(1, 2));
}

TEST(KruskalTest, TestCase2) {
    vector<vector<long long>> dist = {
        {0, 2, 9, 10, 7},
        {2, 0, 6, 4, 3},
        {9, 6, 0, 8, 5},
        {10, 4, 8, 0, 6},
        {7, 3, 5, 6, 0}
    };
    auto result = kruskal(dist);
    ASSERT_EQ(static_cast<int>(result.size()), 4);
    EXPECT_EQ(result[0], make_pair(0, 1));
    EXPECT_EQ(result[1], make_pair(1, 4));
    EXPECT_EQ(result[2], make_pair(1, 3));
    EXPECT_EQ(result[3], make_pair(2, 4));
}

TEST(KruskalTest, EdgesOrderedByWeight) {
    vector<vector<long long>> dist = {
        {0, 10, 1},
        {10, 0, 5},
        {1, 5, 0}
    };
    auto result = kruskal(dist);
    ASSERT_EQ(static_cast<int>(result.size()), 2);
    // Minimum edge (0,2)=1 should come first
    EXPECT_EQ(result[0], make_pair(0, 2));
}

// ==================== tsp_dp ====================

TEST(TspDpTest, FourNodeComplete) {
    vector<vector<long long>> dist = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    auto result = tsp_dp(dist);
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
    auto result = tsp_dp(dist);
    EXPECT_TRUE(result.empty());
}

TEST(TspDpTest, TwoNodes) {
    vector<vector<long long>> dist = {
        {0, 5},
        {5, 0}
    };
    auto result = tsp_dp(dist);
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
    auto result = tsp_dp(dist);
    EXPECT_FALSE(result.empty());
}

// ==================== tsp_greedy ====================

TEST(TspGreedyTest, FourNodeGraph) {
    vector<vector<long long>> dist = {
        {0, 1, 2, 3},
        {1, 0, 1, 2},
        {2, 1, 0, 1},
        {3, 2, 1, 0}
    };
    auto result = tsp_greedy(dist);
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
    auto result = tsp_greedy(dist);
    EXPECT_TRUE(result.empty());
}

TEST(TspGreedyTest, NoNeighbor) {
    vector<vector<long long>> dist = {
        {0, 0},
        {0, 0}
    };
    auto result = tsp_greedy(dist);
    EXPECT_TRUE(result.empty());
}

TEST(TspGreedyTest, With2OptImprovement) {
    vector<vector<long long>> dist = {
        {0, 2, 9, 10},
        {1, 0, 6,  4},
        {15, 7, 0, 8},
        {6,  3, 12, 0}
    };
    auto result = tsp_greedy(dist);
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

// ==================== Dinic / flujo_maximo ====================

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
    EXPECT_EQ(flujo_maximo(cap), 12);
}

TEST(FlujoMaximoTest, NoEdges) {
    vector<vector<long long>> cap = {
        {0, 0},
        {0, 0}
    };
    EXPECT_EQ(flujo_maximo(cap), 0);
}

TEST(FlujoMaximoTest, SingleEdge) {
    vector<vector<long long>> cap = {
        {0, 5},
        {0, 0}
    };
    EXPECT_EQ(flujo_maximo(cap), 5);
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
    auto result = cortar_poligono(poly, 1.0, 0.0, 0.0);
    EXPECT_TRUE(result.empty());
}

TEST(CortarPoligonoTest, SquareCutByVertical) {
    vector<Punto> poly = {{-1.0,-1.0},{1.0,-1.0},{1.0,1.0},{-1.0,1.0}};
    auto result = cortar_poligono(poly, 1.0, 0.0, 0.0);
    EXPECT_FALSE(result.empty());
}

TEST(CortarPoligonoTest, AllOutside) {
    vector<Punto> poly = {{1.0,0.0},{2.0,0.0},{2.0,1.0},{1.0,1.0}};
    auto result = cortar_poligono(poly, 1.0, 0.0, -1.0);
    EXPECT_TRUE(result.empty());
}

TEST(CortarPoligonoTest, AllInside) {
    vector<Punto> poly = {{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0}};
    auto result = cortar_poligono(poly, 1.0, 0.0, 5.0);
    EXPECT_EQ(result.size(), poly.size());
}

TEST(CortarPoligonoTest, PartialCross) {
    vector<Punto> poly = {{-2.0,0.0},{2.0,0.0},{2.0,2.0},{-2.0,2.0}};
    auto result = cortar_poligono(poly, 1.0, 0.0, 1.0);
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

// ==================== limpiar_cero ====================

TEST(LimpiarCeroTest, SmallPositive) {
    EXPECT_NEAR(limpiar_cero(0.0001), 0.0, 1e-9);
}

TEST(LimpiarCeroTest, SmallNegative) {
    EXPECT_NEAR(limpiar_cero(-0.0001), 0.0, 1e-9);
}

TEST(LimpiarCeroTest, NotNearZero) {
    EXPECT_NEAR(limpiar_cero(1.5), 1.5, 1e-9);
}

TEST(LimpiarCeroTest, ExactZero) {
    EXPECT_NEAR(limpiar_cero(0.0), 0.0, 1e-9);
}

// ==================== Funciones de salida ====================

TEST(ImprimirArcosTest, TwoEdges) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimir_arcos({{0, 1}, {1, 2}});
    cout.rdbuf(orig);
    string s = out.str();
    EXPECT_NE(s.find("(A,B)"), string::npos);
    EXPECT_NE(s.find("(B,C)"), string::npos);
}

TEST(ImprimirArcosTest, EmptyArcos) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimir_arcos({});
    cout.rdbuf(orig);
    EXPECT_TRUE(out.str().empty());
}

TEST(ImprimirRutaTest, ValidRoute) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimir_ruta({0, 1, 2, 0});
    cout.rdbuf(orig);
    EXPECT_NE(out.str().find("A"), string::npos);
}

TEST(ImprimirRutaTest, EmptyRoute) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    imprimir_ruta({});
    cout.rdbuf(orig);
    EXPECT_NE(out.str().find("no existe ruta"), string::npos);
}

TEST(ImprimirPoligonosTest, SimplePolygon) {
    ostringstream out;
    streambuf* orig = cout.rdbuf(out.rdbuf());
    vector<vector<Punto>> polys = {{{1.0, 2.0}, {3.0, 4.0}}};
    imprimir_poligonos(polys);
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
    imprimir_poligonos(polys);
    cout.rdbuf(orig);
    string s = out.str();
    EXPECT_NE(s.find("]"), string::npos);
}
