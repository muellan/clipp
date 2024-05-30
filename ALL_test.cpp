#include "test/pch.h"

#define __STRNGZ(S) #S
#define _STRNGZ(S) __STRNGZ(S)
#pragma message("C++ std == " _STRNGZ(CLIPP_CXX_STD))

#ifdef ALL_TESTS_AS_SINGLE_PRJ

int actions_test_main();
int alternative_groups_test_main();
int alternative_options_test_main();
int alternative_required_test_main();
int blocking_test01_main();
int blocking_test02_main();
int blocking_test03_main();
int blocking_test04_main();
int blocking_test05_main();
int blocking_test06_main();
int blocking_test07_main();
int blocking_test08_main();
int blocking_test09_main();
int blocking_test10_main();
int documentation_test_main();
int documentation_test_wide_main();
int empty_args_main();
int flag_param_factories_test_main(int argc, char* argv[]);
int joined_flags_test1_main();
int joined_flags_test2_main();
int joined_flags_test3_main();
int joined_flags_test4_main();
int joined_flags_test5_main();
int joined_flags_test6_main();
int joined_params_test1_main();
int joined_params_test2_main();
int joined_sequence_test_main();
int mixed_params_test_main();
int nesting_test_main();
int options_test_main();
int prefix_free_test_main();
int prefix_test_main();
int repeatability_test_main();
int repeatable_alternatives_test_main();
int required_params_test1_main();
int required_params_test2_main();
int usage_lines_test_main();
int values_conversion_test_main();
int values_filter_test_main();
int values_sequencing_test_main();

namespace {

int test_main(int(*routine)(), char const* test_name)
{
    int const rv = routine();
    if (rv != 0) {
        std::cerr << "Test '" << test_name << "' failed (" << rv << ")" << std::endl;
        return rv;
    }
    std::cerr << "Test '" << test_name << "' OK" << std::endl;
    return 0;
}

int test_main_av(int(*routine)(int, char**), char const* test_name, int argc, char** argv)
{
    int const rv = routine(argc, argv);
    if (rv != 0) {
        std::cerr << "Test '" << test_name << "' failed (" << rv << ")" << std::endl;
        return rv;
    }
    std::cerr << "Test '" << test_name << "' OK" << std::endl;
    return 0;
}

#define RUN_TEST(Func)                  \
    do {                                 \
        int rv{test_main(Func, #Func)};   \
        if (rv != 0) {                     \
            return rv;                      \
        }                                    \
    } while (0)

#define RUN_TEST_AV(Func, argc, argv)                 \
    do {                                               \
        int rv{test_main_av(Func, #Func, argc, argv)};  \
        if (rv != 0) {                                   \
            return rv;                                    \
        }                                                  \
    } while (0)

} // namespace

int main(int argc, char* argv[])
{
    RUN_TEST(documentation_test_wide_main);

    RUN_TEST(actions_test_main);
    RUN_TEST(alternative_groups_test_main);
    RUN_TEST(alternative_options_test_main);
    RUN_TEST(alternative_required_test_main);
    RUN_TEST(blocking_test01_main);
    RUN_TEST(blocking_test02_main);
    RUN_TEST(blocking_test03_main);
    RUN_TEST(blocking_test04_main);
    RUN_TEST(blocking_test05_main);
    RUN_TEST(blocking_test06_main);
    RUN_TEST(blocking_test07_main);
    RUN_TEST(blocking_test08_main);
    RUN_TEST(blocking_test09_main);
    RUN_TEST(blocking_test10_main);
    RUN_TEST(documentation_test_main);
    RUN_TEST(empty_args_main);
    RUN_TEST_AV(flag_param_factories_test_main, argc, argv);
    RUN_TEST(joined_flags_test1_main);
    RUN_TEST(joined_flags_test2_main);
    RUN_TEST(joined_flags_test3_main);
    RUN_TEST(joined_flags_test4_main);
    RUN_TEST(joined_flags_test5_main);
    RUN_TEST(joined_flags_test6_main);
    RUN_TEST(joined_params_test1_main);
    RUN_TEST(joined_params_test2_main);
    RUN_TEST(joined_sequence_test_main);
    RUN_TEST(mixed_params_test_main);
    RUN_TEST(nesting_test_main);
    RUN_TEST(options_test_main);
    RUN_TEST(prefix_free_test_main);
    RUN_TEST(prefix_test_main);
    RUN_TEST(repeatability_test_main);
    RUN_TEST(repeatable_alternatives_test_main);
    RUN_TEST(required_params_test1_main);
    RUN_TEST(required_params_test2_main);
    RUN_TEST(usage_lines_test_main);
    RUN_TEST(values_conversion_test_main);
    RUN_TEST(values_filter_test_main);
    RUN_TEST(values_sequencing_test_main);
    return 0;
}
#endif
