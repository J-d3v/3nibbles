#include <gtest/gtest.h>

#include "grade_calculator.h"

TEST(GradeCalculatorTest, AverageShouldReturnCorrectValue) {
    const std::vector<double> grades{80, 90, 100};
    EXPECT_DOUBLE_EQ(GradeCalculator::average(grades), 90.0);
}

TEST(GradeCalculatorTest, PassedShouldReturnTrueWhenAverageIsEnough) {
    const std::vector<double> grades{70, 75, 80};
    EXPECT_TRUE(GradeCalculator::passed(grades));
}

TEST(GradeCalculatorTest, PassedShouldReturnFalseWhenAverageIsLow) {
    const std::vector<double> grades{50, 60, 65};
    EXPECT_FALSE(GradeCalculator::passed(grades));
}

TEST(GradeCalculatorTest, AverageShouldThrowOnEmptyInput) {
    const std::vector<double> grades{};
    EXPECT_THROW(GradeCalculator::average(grades), std::invalid_argument);
}

TEST(GradeCalculatorTest, AverageShouldThrowOnInvalidGrade) {
    const std::vector<double> grades{90, 110};
    EXPECT_THROW(GradeCalculator::average(grades), std::invalid_argument);
}
