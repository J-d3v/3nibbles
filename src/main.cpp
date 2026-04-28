#include "student_service.h"

#include <httplib.h>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(StudentService::health(), "application/json");
        res.status = 200;
    });

    server.Post("/api/v1/grades/final", [](const httplib::Request& req, httplib::Response& res) {
        try {
            const auto result = StudentService::calculateFinalGradeJson(req.body);
            res.set_content(result, "application/json");
            res.status = 200;
        } catch (const std::exception& ex) {
            const std::string body = std::string("{\"error\":\"") + ex.what() + "\"}";
            res.set_content(body, "application/json");
            res.status = 400;
        }
    });

    const char* portEnv = std::getenv("PORT");
    const int port = portEnv ? std::stoi(portEnv) : 8080;

    std::cout << "Server running on http://0.0.0.0:" << port << std::endl;
    server.listen("0.0.0.0", port);
    return 0;
}
