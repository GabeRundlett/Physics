#include <stdio.h>

enum class UnitTypeID { none, length, angle };
template <typename T> struct Unit {
    static constexpr UnitTypeID get_type() { return UnitTypeID::none; }
    template <typename U> constexpr Unit<T> operator+(const Unit<U>) { return Unit<T>(); }
};

#define _IMPL_LITERAL_SUFFIX(type, singular_name, abbreviation)                                                                \
    constexpr Unit<type::singular_name> operator""_##abbreviation(const long double lhs) {                                     \
        return {static_cast<double>(lhs)};                                                                                     \
    }                                                                                                                          \
    constexpr Unit<type::singular_name> operator""_##abbreviation(const unsigned long long lhs) {                              \
        return {static_cast<double>(lhs)};                                                                                     \
    }

#define _IMPL_UNIT_TYPE(type, singular_name)                                                                                   \
    namespace type {                                                                                                           \
        struct singular_name {};                                                                                               \
    }                                                                                                                          \
    template <> struct Unit<type::singular_name> {                                                                             \
        double value;                                                                                                          \
        static constexpr UnitTypeID get_type() { return UnitTypeID::type; }                                                    \
        template <typename U> constexpr Unit<type::singular_name> operator+(const Unit<U> &u) const {                          \
            return Unit<type::singular_name>();                                                                                \
        }                                                                                                                      \
    };

#define _IMPL_UNIT_OPERATOR_PLUS(type, name1, name2, factor)                                                                   \
    template <> constexpr Unit<type::name1> Unit<type::name1>::operator+(const Unit<type::name2> &u) const {                   \
        return {value + u.value / factor};                                                                                     \
    }                                                                                                                          \
    template <> constexpr Unit<type::name2> Unit<type::name2>::operator+(const Unit<type::name1> &u) const {                   \
        return {value + u.value * factor};                                                                                     \
    }

#define _IMPL_UNIT(type, singular_name, plural_name, abbreviation)                                                             \
    _IMPL_UNIT_TYPE(type, singular_name)                                                                                       \
    _IMPL_LITERAL_SUFFIX(type, singular_name, abbreviation)

_IMPL_UNIT(length, meter, meters, m)
_IMPL_UNIT(length, foot, feet, ft)
_IMPL_UNIT(length, yard, yards, yd)
_IMPL_UNIT(length, mile, miles, mi)

_IMPL_UNIT_OPERATOR_PLUS(length, meter, foot, 0.3048)
_IMPL_UNIT_OPERATOR_PLUS(length, meter, yard, 0.9144)
_IMPL_UNIT_OPERATOR_PLUS(length, meter, mile, 1609.344)
_IMPL_UNIT_OPERATOR_PLUS(length, foot, yard, 3)
_IMPL_UNIT_OPERATOR_PLUS(length, foot, mile, 5280)
_IMPL_UNIT_OPERATOR_PLUS(length, yard, mile, 1760)

_IMPL_UNIT(angle, radian, radians, rad)
_IMPL_UNIT(angle, gradian, gradians, grad)
_IMPL_UNIT(angle, degree, degrees, deg)

_IMPL_UNIT_OPERATOR_PLUS(angle, radian, gradian, 63.6619772368)
_IMPL_UNIT_OPERATOR_PLUS(angle, radian, degree, 57.2957795131)
_IMPL_UNIT_OPERATOR_PLUS(angle, gradian, degree, 0.9)

int main() {
    {
        constexpr auto a = 2_ft;
        constexpr auto b = 10_yd;
        constexpr auto c = a + b;
        printf("%f\n", c.value);
    }
    {
        constexpr auto a = 0_deg;
        constexpr auto b = 1_rad;
        constexpr auto c = a + b;
        printf("%f\n", c.value);
    }
}
