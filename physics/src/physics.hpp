#pragma once
#include <vector>

enum class Unit : unsigned char { Meters, Kilograms, Seconds };

namespace physics {
    struct Unit {
        ::Unit type;
        double pow = 1.0;
        Unit operator^(const double p) {
            pow = p;
            return *this;
        }
    };

    struct Units {
        std::vector<Unit> units;
        Units operator*(const Units &u) const;
        template <typename... T> Units(T... args) : units{args...} {}
    };

    struct Scalar {
        float value;
        Units units;
        void print() const;
    };

    struct Vec2 {
        float x, y;
        Units units;
        Scalar operator*(const Vec2 &v) const { return {x * v.x + y * v.y, units * v.units}; }
        Scalar cross(const Vec2 &v) const { return {x * v.y + y * v.x, units * v.units}; }
    };
} // namespace physics

physics::Unit operator^(const ::Unit &lhs, const double rhs);
