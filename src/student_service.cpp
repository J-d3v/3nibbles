#include "student_service.h"
#include "grade_calculator.h"

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <vector>

using json = nlohmann::json;

std::string StudentService::health() {
    const json response = {
        {"status", "UP"},
        {"service", "student-grade-service"}
    };

    return response.dump();
}

std::string StudentService::calculateFinalGradeJson(const std::string& body) {
    const auto request = json::parse(body);

    if (!request.contains("grades") || !request["grades"].is_array()) {
        throw std::invalid_argument("grades array is required");
    }

    const auto grades = request["grades"].get<std::vector<double>>();
    const double avg = GradeCalculator::average(grades);
    const bool pass = GradeCalculator::passed(grades);

    const json response = {
        {"average", avg},
        {"passed", pass}
    };

    return response.dump();
}
