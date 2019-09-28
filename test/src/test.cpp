#include <math.h>
#include <stdio.h>

enum class UnitTypeID { none, length, angle };
template <typename T> struct Unit {
    static constexpr UnitTypeID get_type() { return UnitTypeID::none; }
    template <typename U> constexpr Unit<T> operator+(const Unit<U>) { return Unit<T>(); }
};

#define _IMPL_LITERAL_SUFFIX(type, singular_name, abbreviation)                                                                \
    constexpr Unit<type::singular_name> operator""_##abbreviation(const long double lhs) {                                     \
        return {static_cast<double>(lhs), 1.0};                                                                                \
    }                                                                                                                          \
    constexpr Unit<type::singular_name> operator""_##abbreviation(const unsigned long long lhs) {                              \
        return {static_cast<double>(lhs), 1.0};                                                                                \
    }

#define _IMPL_UNIT_TYPE(type, singular_name, plural_name)                                                                      \
    namespace type {                                                                                                           \
        struct singular_name {};                                                                                               \
    }                                                                                                                          \
    template <> struct Unit<type::singular_name> {                                                                             \
        double value, power = 1.0;                                                                                             \
        static constexpr UnitTypeID get_type() { return UnitTypeID::type; }                                                    \
        template <typename U> constexpr Unit<type::singular_name> operator+(const Unit<U> &u) const {                          \
            return Unit<type::singular_name>();                                                                                \
        }                                                                                                                      \
        template <typename U> constexpr Unit<type::singular_name> operator-(const Unit<U> &u) const {                          \
            return Unit<type::singular_name>();                                                                                \
        }                                                                                                                      \
        constexpr Unit<type::singular_name> operator^(const long double x) {                                                   \
            power *= x;                                                                                                        \
            return *this;                                                                                                      \
        }                                                                                                                      \
        void print() { printf("%f " #plural_name "\n", value); }                                                               \
    };

#define _IMPL_UNIT_OPERATOR_PLUS(type, name1, name2, factor)                                                                   \
    template <> constexpr Unit<type::name1> Unit<type::name1>::operator+(const Unit<type::name2> &u) const {                   \
        if (power == u.power) return {value + u.value * factor /*pow(factor, power)*/, power};                                 \
        printf("you may not add scalars at different powers\n");                                                               \
        return {0, 1};                                                                                                         \
    }                                                                                                                          \
    template <> constexpr Unit<type::name2> Unit<type::name2>::operator+(const Unit<type::name1> &u) const {                   \
        if (power == u.power) return {value + u.value / factor /*pow(factor, power)*/, power};                                 \
        printf("you may not add scalars at different powers\n");                                                               \
        return {0, 1};                                                                                                         \
    }

#define _IMPL_UNIT_OPERATOR_MINUS(type, name1, name2, factor)                                                                  \
    template <> constexpr Unit<type::name1> Unit<type::name1>::operator-(const Unit<type::name2> &u) const {                   \
        if (power == u.power) return {value - u.value * factor /*pow(factor, power)*/, power};                                 \
        printf("you may not subtract scalars at different powers\n");                                                          \
        return {0, 1};                                                                                                         \
    }                                                                                                                          \
    template <> constexpr Unit<type::name2> Unit<type::name2>::operator-(const Unit<type::name1> &u) const {                   \
        if (power == u.power) return {value - u.value / factor /*pow(factor, power)*/, power};                                 \
        printf("you may not subtract scalars at different powers\n");                                                          \
        return {0, 1};                                                                                                         \
    }

#define _IMPL_UNIT(type, singular_name, plural_name, abbreviation)                                                             \
    _IMPL_UNIT_TYPE(type, singular_name, plural_name)                                                                          \
    _IMPL_LITERAL_SUFFIX(type, singular_name, abbreviation)

#define _IMPL_UNIT_OPERATORS(type, name1, name2, factor)                                                                       \
    _IMPL_UNIT_OPERATOR_PLUS(type, name1, name2, factor)                                                                       \
    _IMPL_UNIT_OPERATOR_MINUS(type, name1, name2, factor)

_IMPL_UNIT(length, meter, meters, m)
_IMPL_UNIT(length, foot, feet, ft)
_IMPL_UNIT(length, yard, yards, yd)
_IMPL_UNIT(length, mile, miles, mi)
_IMPL_UNIT(angle, radian, radians, rad)
_IMPL_UNIT(angle, gradian, gradians, grad)
_IMPL_UNIT(angle, degree, degrees, deg)

_IMPL_UNIT_OPERATORS(length, meter, foot, 0.3048)
_IMPL_UNIT_OPERATORS(length, meter, yard, 0.9144)
_IMPL_UNIT_OPERATORS(length, meter, mile, 1609.344)
_IMPL_UNIT_OPERATORS(length, foot, yard, 3)
_IMPL_UNIT_OPERATORS(length, foot, mile, 5280)
_IMPL_UNIT_OPERATORS(length, yard, mile, 1760)
_IMPL_UNIT_OPERATORS(angle, gradian, radian, 63.6619772368)
_IMPL_UNIT_OPERATORS(angle, degree, radian, 57.2957795131)
_IMPL_UNIT_OPERATORS(angle, degree, gradian, 0.9)

int main() {
    {
        constexpr auto a = 9_ft;
        constexpr auto b = 1_yd;
        constexpr auto c = a + b;
    }
    {
        constexpr auto a = 0_deg;
        constexpr auto b = 1_rad;
        constexpr auto c = a + b;
        constexpr auto d = a - b;
    }
}

// freddie 80cm
// antoinette 420cm
// boat 250cm
