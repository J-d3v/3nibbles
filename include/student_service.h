#pragma once

#include <string>

class StudentService {
public:
    static std::string health();
    static std::string calculateFinalGradeJson(const std::string& body);
};
