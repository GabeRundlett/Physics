#include "physics.hpp"

int main() {
    physics::Vec2 v1 = {3, 4, 1_m / 2_s};
    physics::Vec2 v2 = {-4, -7, 1_kg * 2_m / 2_s};
    v1.len().print();
    v2.len().print();
    auto v3 = v1 * v2;
    auto v4 = v1.cross(v2);
    v3.print();
    v4.print();
}
