/*
 * Copyright (C) 2026 Equipo 4 - Actividad Integradora 2.
 * All rights reserved.
 */

#include "algorithms.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

static std::vector<std::vector<long long>> parse_matriz(const json& m) {
    int n = static_cast<int>(m.size());
    std::vector<std::vector<long long>> mat(n, std::vector<long long>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = m.at(i).at(j).get<long long>();
        }
    }
    return mat;
}

static long long costo_ruta(const std::vector<int>& ruta,
                              const std::vector<std::vector<long long>>& dist) {
    long long total = 0;
    for (int i = 0; i + 1 < static_cast<int>(ruta.size()); i++) {
        total += dist[ruta[i]][ruta[i + 1]];
    }
    return total;
}

static json ejecutar_solve(const std::string& body_str) {
    auto body = json::parse(body_str);
    auto dist = parse_matriz(body.at("distancias"));
    auto cap  = parse_matriz(body.at("capacidades"));

    std::vector<Punto> centrales;
    for (auto& c : body.at("centrales")) {
        centrales.push_back({c.at(0).get<double>(), c.at(1).get<double>()});
    }

    auto arcos = kruskal(dist);
    json mst_json = json::array();
    for (auto& [u, v] : arcos) {
        mst_json.push_back({nombre_nodo(u), nombre_nodo(v)});
    }

    auto ruta = tsp(dist);
    json tsp_json;
    if (ruta.empty()) {
        tsp_json["ruta"]  = json::array();
        tsp_json["costo"] = 0;
    } else {
        json ruta_json = json::array();
        for (int idx : ruta) {
            ruta_json.push_back(nombre_nodo(idx));
        }
        tsp_json["ruta"]  = ruta_json;
        tsp_json["costo"] = costo_ruta(ruta, dist);
    }

    long long flujo = flujo_maximo(cap);

    auto poligonos = voronoi(centrales);
    json voronoi_json = json::array();
    for (auto& poly : poligonos) {
        json poly_json = json::array();
        for (auto& pt : poly) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2)
                << "(" << limpiar_cero(pt.x) << "," << limpiar_cero(pt.y) << ")";
            poly_json.push_back(oss.str());
        }
        voronoi_json.push_back(poly_json);
    }

    json result;
    result["mst"]         = mst_json;
    result["tsp"]         = tsp_json;
    result["flujoMaximo"] = flujo;
    result["voronoi"]     = voronoi_json;
    return result;
}

int main() {
    httplib::Server svr;

    svr.Get("/status", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status":"ok"})", "application/json");
    });

    svr.Post("/solve", [](const httplib::Request& req, httplib::Response& res) {
        try {
            res.set_content(ejecutar_solve(req.body).dump(2), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}
