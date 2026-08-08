#include <unity.h>

void setUp() {}
void tearDown() {}

void test_dummy() {
    TEST_ASSERT_TRUE(true);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_dummy);

    UNITY_END();

    // Now run doctest tests
    extern int run_doctest_tests();
    return run_doctest_tests();
}
