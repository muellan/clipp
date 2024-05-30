#pragma once

#ifdef ALL_EXAMPLES_AS_SINGLE_PRJ
#  ifndef THIS_EXAMPLE_FNAME
#    error macro THIS_EXAMPLE_FNAME must be defined!
#  endif // !THIS_EXAMPLE_FNAME
#  define EXAMPLE_MAIN _STR_CAT(THIS_EXAMPLE_FNAME, _main)
#else
#  define EXAMPLE_MAIN main
#endif

#ifdef ALL_EXAMPLES_AS_SINGLE_PRJ

// clang warnings if no fn proto defined

int actions_main(int argc, char* argv[]);
int align_main(int argc, char* argv[]);
int alternatives_main(int argc, char* argv[]);
int annotate_main(int argc, char* argv[]);
int argv0_main(int argc, char* argv[]);
int commands_main(int argc, char* argv[]);
int complex_nesting_main(int argc, char* argv[]);
int convert_main(int argc, char* argv[]);
int counter_main(int argc, char* argv[]);
int documentation_main();
int doc_filter_main(int argc, char* argv[]);
int finder_main(int argc, char* argv[]);
int float_vector_main(int argc, char* argv[]);
int groups_main(int argc, char* argv[]);
int joinable_flags_main(int argc, char* argv[]);
int model_main(int argc, char* argv[]);
int naval_fate_main(int argc, char* argv[]);
int nested_alternatives_main(int argc, char* argv[]);
int numbers_main(int argc, char* argv[]);
int options_main(int argc, char* argv[]);
int options_values_main(int argc, char* argv[]);
int parsing_main(int argc, char* argv[]);
int positional_values_main(int argc, char* argv[]);
int repeatable_main(int argc, char* argv[]);
int required_flags_main(int argc, char* argv[]);
int sanity_main();
int send_main(int argc, char* argv[]);
int simplify_main(int argc, char* argv[]);
int switches_main(int argc, char* argv[]);
int tagnames_main(int argc, char* argv[]);
int text_formatting_main(int argc, char* argv[]);
int timing_main(int argc, char* argv[]);
int transform_main(int argc, char* argv[]);

namespace {

int ex_main(int(*routine)(), char const* test_name)
{
    int const rv = routine();
    if (rv != 0) {
        std::cerr << "Test '" << test_name << "' failed (" << rv << ")" << std::endl;
    }
    return rv;
}

int ex_main_av(int(*routine)(int, char**), char const* test_name, int argc, char** argv)
{
    int const rv = routine(argc, argv);
    if (rv != 0) {
        std::cerr << "Test '" << test_name << "' failed (" << rv << ")" << std::endl;
    }
    return rv;
}

#define RUN_EXAMPLE(Func)               \
    do {                                 \
        int rv{ex_main(Func, #Func)};     \
        if (rv != 0) {                     \
            return rv;                      \
        }                                    \
    } while (0)

#define RUN_EXAMPLE_AV(Func, argc, argv)              \
    do {                                               \
        int rv{ex_main_av(Func, #Func, argc, argv)};    \
        if (rv != 0) {                                   \
            return rv;                                    \
        }                                                  \
    } while (0)

} // namespace

#endif

