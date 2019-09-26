#include "physics.hpp"

int main() {
    physics::Vec2 v1 = {2, 3, {Unit::Meters ^ 1, Unit::Seconds ^ -2}};
    physics::Vec2 v2 = {-4, -7, {Unit::Kilograms ^ 1, Unit::Seconds ^ -1, Unit::Meters ^ 2, Unit::Seconds ^ -1}};
    auto v3 = v1 * v2;
    auto v4 = v1.cross(v2);
    v3.print();
    v4.print();
}
