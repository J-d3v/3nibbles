#include "grade_calculator.h"

#include <numeric>
#include <stdexcept>


double GradeCalculator::average(const std::vector<double>& grades) {
    if (grades.empty()) {
        throw std::invalid_argument("grades cannot be empty");
    }

    for (double grade : grades) {
        if (grade < 0.0 || grade > 100.0) {
            throw std::invalid_argument("grade out of range");
        }
    }

    const double sum = std::accumulate(grades.begin(), grades.end(), 0.0);
    return sum / static_cast<double>(grades.size());
}

bool GradeCalculator::passed(const std::vector<double>& grades, double minPassing) {
    return average(grades) >= minPassing;
}
