#include <gtest/gtest.h>

#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(IntegrationTest, HealthEndpointShouldReturnUp) {
    httplib::Client client("localhost", 8080);
    const auto res = client.Get("/health");

    ASSERT_TRUE(res != nullptr);
    EXPECT_EQ(res->status, 200);

    const auto body = json::parse(res->body);
    EXPECT_EQ(body["status"], "UP");
}

TEST(IntegrationTest, FinalGradeEndpointShouldCalculateAverage) {
    httplib::Client client("localhost", 8080);
    const json request = {
        {"grades", {90, 80, 70}}
    };

    const auto res = client.Post("/api/v1/grades/final", request.dump(), "application/json");

    ASSERT_TRUE(res != nullptr);
    EXPECT_EQ(res->status, 200);

    const auto body = json::parse(res->body);
    EXPECT_DOUBLE_EQ(body["average"], 80.0);
    EXPECT_EQ(body["passed"], true);
}
