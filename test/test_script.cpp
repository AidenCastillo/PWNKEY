#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


TEST_CASE("Basic Test") {
    CHECK(1 + 1 == 2);
    CHECK(2 * 2 == 4);
    CHECK(3 - 1 == 2);
}
