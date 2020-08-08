#pragma once
#include <cmath>
#include <vector>

enum class Unit : unsigned char {
    Meters,
    Kilograms,
    Seconds,
};

namespace Physics {
    struct Units;

    struct Unit {
        double pow = 1.0;
        ::Unit type;
        constexpr Unit operator^(const double p) {
            pow = p;
            return *this;
        }
        Units operator*(const Unit u);
        Units operator/(Unit u);
    };

    struct Units {
        std::vector<Unit> units;
        Units operator*(const Units u) const;
        template <typename... T>
        Units(T... args) : units{args...} {}
        Units operator*(const Unit u) {
            units.push_back(u);
            return *this;
        }
        Units operator/(Unit &u) {
            u.pow *= -1;
            units.push_back(u);
            return *this;
        }
    };

    struct Scalar {
        float value;
        Units units;
        void print() const;
    };

    struct Vec2 {
        Units units;
        float x, y;
        Scalar operator*(const Vec2 &v) const { return {x * v.x + y * v.y, units * v.units}; }
        Scalar cross(const Vec2 &v) const { return {x * v.y + y * v.x, units * v.units}; }
        Scalar len() const { return {std::sqrt(x * x + y * y), units}; }
    };
} // namespace Physics

inline constexpr Physics::Unit operator^(const Unit &lhs, const double rhs) { return {rhs, lhs}; }
inline constexpr Physics::Unit operator""_m(long double d) { return {static_cast<double>(d), Unit::Meters}; }
inline constexpr Physics::Unit operator""_m(unsigned long long d) { return {static_cast<double>(d), Unit::Meters}; }
inline constexpr Physics::Unit operator""_kg(long double d) { return {static_cast<double>(d), Unit::Kilograms}; }
inline constexpr Physics::Unit operator""_kg(unsigned long long d) { return {static_cast<double>(d), Unit::Kilograms}; }
inline constexpr Physics::Unit operator""_s(long double d) { return {static_cast<double>(d), Unit::Seconds}; }
inline constexpr Physics::Unit operator""_s(unsigned long long d) { return {static_cast<double>(d), Unit::Seconds}; }
