#include "Physics.hpp"
#include <iostream>

namespace Physics {
    namespace Detail {
        static constexpr const char *print_unit(const ::Unit u) {
            switch (u) {
            case ::Unit::Meters: return "m";
            case ::Unit::Kilograms: return "kg";
            case ::Unit::Seconds: return "s";
            default: return " ";
            }
        }
        static constexpr const char *print_unit(const Unit &u) { return print_unit(u.type); }
    } // namespace Detail

    void Scalar::print() const {
        std::cout << value << ' ';
        for (unsigned int i = 0; i < units.units.size(); ++i) {
            const Unit &u = units.units[i];
            std::cout << Detail::print_unit(u) << u.pow << ' ';
        }
        std::cout << '\n';
    }
} // namespace Physics
