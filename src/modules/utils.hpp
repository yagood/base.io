#pragma once
#include "json.hpp"    // json, toRadius, splitStr, smart pointers
#include <random>      // rand()
#include "vec2.hpp" // randomPosition()

// Stuff I dont feel like typing out
using json = nlohmann::json;
using e_ptr = std::shared_ptr<class Entity>;
using e_vec = std::vector<e_ptr>;

// Constants
#define MATH_PI 3.141592653589793238462643383279502884L
#define INV_SQRT_2 1 / std::sqrt(2)

// Externals
extern json config;

// Utilities
namespace utils {

struct Color {
    unsigned char r, g, b;

    Color();
    Color(const json::value_type &j);
    Color(const json &j0, const json &j1, const json &j2);
    Color(unsigned char _r, unsigned char _g, unsigned char _b);

    std::string toString();

    bool operator==(const Color &other);
    bool operator!=(const Color &other);
};

extern std::vector<std::string> splitStr(const std::string &str, char delimiter);

extern Color randomColor() noexcept;

extern Vec2 randomPosition() noexcept;

extern inline double rand(double min, double max) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<> distr(min, max);
    return distr(eng);
}

extern inline int rand(int min, int max) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(eng);
}

extern inline double toRadius(double mass) noexcept {
    return std::sqrt(mass * 100);
}
extern inline double toMass(double radius) noexcept {
    return radius * radius / 100;
}

} // namespace utils

using namespace utils;