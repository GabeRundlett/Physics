#include "../physics.hpp"

namespace physics {
    Units Units::operator*(const Units &u) const {
        Units result = {units};
        result.units.insert(result.units.end(), u.units.begin(), u.units.end());
        for (unsigned int i = 0; i < result.units.size(); ++i) {
            for (unsigned int j = i; j > 0; --j) {
                if (result.units[i].type == result.units[j - 1].type) {
                    result.units[j - 1].pow += result.units[i].pow;
                    result.units.erase(result.units.begin() + i);
                    --i;
                }
            }
        }
        return result;
    }
} // namespace physics

physics::Unit operator^(const Unit &lhs, const double rhs) { return physics::Unit{lhs, rhs}; }
