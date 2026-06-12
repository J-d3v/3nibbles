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

int main() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status":"ok"})", "application/json");
    });

    svr.Post("/api/prim", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            auto dist = parse_matriz(body.at("dist"));
            auto arcos = prim(dist);
            json result;
            result["arcos"] = json::array();
            for (auto& [u, v] : arcos) {
                result["arcos"].push_back(
                    "(" + nombre_nodo(u) + "," + nombre_nodo(v) + ")");
            }
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    svr.Post("/api/tsp", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            auto dist = parse_matriz(body.at("dist"));
            auto ruta = tsp(dist);
            json result;
            if (ruta.empty()) {
                result["ruta"] = "no existe ruta";
            } else {
                std::string s;
                for (int i = 0; i < static_cast<int>(ruta.size()); i++) {
                    if (i > 0) {
                        s += " ";
                    }
                    s += nombre_nodo(ruta[i]);
                }
                result["ruta"] = s;
            }
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    svr.Post("/api/flujo", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            auto cap = parse_matriz(body.at("cap"));
            long long flujo = flujo_maximo(cap);
            json result;
            result["flujo"] = flujo;
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    svr.Post("/api/voronoi", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            auto pts_json = body.at("puntos");
            std::vector<Punto> puntos;
            for (auto& p : pts_json) {
                puntos.push_back({p.at("x").get<double>(), p.at("y").get<double>()});
            }
            auto poligonos = voronoi(puntos);
            json result;
            result["poligonos"] = json::array();
            for (auto& poly : poligonos) {
                json poly_json = json::array();
                for (auto& pt : poly) {
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(2)
                        << "(" << limpiar_cero(pt.x) << "," << limpiar_cero(pt.y) << ")";
                    poly_json.push_back(oss.str());
                }
                result["poligonos"].push_back(poly_json);
            }
            res.set_content(result.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}
