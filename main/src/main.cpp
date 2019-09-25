#include <debug.hpp>

int main() {
    debug::time::reset();
    constexpr unsigned int count = 10;
    for (unsigned int i = 0; i < count; ++i)
        debug::message("Hello, Message!");
    debug::info("%f\n", debug::time::now() / count);
    debug::time::reset();
    for (unsigned int i = 0; i < count; ++i)
        debug::warning("Hello, Warning!");
    debug::info("%f\n", debug::time::now() / count);
    debug::time::reset();
    for (unsigned int i = 0; i < count; ++i)
        debug::error("Hello, Error!");
    debug::info("%f\n", debug::time::now() / count);
}
