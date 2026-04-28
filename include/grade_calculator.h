#pragma once

#include <vector>

class GradeCalculator {
public:
    static double average(const std::vector<double>& grades);
    static bool passed(const std::vector<double>& grades, double minPassing = 70.0);
};
