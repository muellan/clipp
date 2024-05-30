#include "pch.h"
#include "example-def.h"

#ifdef ALL_EXAMPLES_AS_SINGLE_PRJ

// #TODO: append test cases

int main(int argc, char* argv[])
{
    RUN_EXAMPLE_AV(actions_main, argc, argv);
    RUN_EXAMPLE_AV(align_main, argc, argv);
    RUN_EXAMPLE_AV(alternatives_main, argc, argv);
    RUN_EXAMPLE_AV(annotate_main, argc, argv);
    RUN_EXAMPLE_AV(argv0_main, argc, argv);
    RUN_EXAMPLE_AV(commands_main, argc, argv);
    RUN_EXAMPLE_AV(complex_nesting_main, argc, argv);
    RUN_EXAMPLE_AV(convert_main, argc, argv);
    RUN_EXAMPLE_AV(counter_main, argc, argv);
    RUN_EXAMPLE(documentation_main);
    RUN_EXAMPLE_AV(doc_filter_main, argc, argv);
    RUN_EXAMPLE_AV(finder_main, argc, argv);
    RUN_EXAMPLE_AV(float_vector_main, argc, argv);
    RUN_EXAMPLE_AV(groups_main, argc, argv);
    RUN_EXAMPLE_AV(joinable_flags_main, argc, argv);
    RUN_EXAMPLE_AV(model_main, argc, argv);
    RUN_EXAMPLE_AV(naval_fate_main, argc, argv);
    RUN_EXAMPLE_AV(nested_alternatives_main, argc, argv);
    RUN_EXAMPLE_AV(numbers_main, argc, argv);
    RUN_EXAMPLE_AV(options_main, argc, argv);
    RUN_EXAMPLE_AV(options_values_main, argc, argv);
    RUN_EXAMPLE_AV(parsing_main, argc, argv);
    RUN_EXAMPLE_AV(positional_values_main, argc, argv);
    RUN_EXAMPLE_AV(repeatable_main, argc, argv);
    RUN_EXAMPLE_AV(required_flags_main, argc, argv);
    RUN_EXAMPLE(sanity_main);
    RUN_EXAMPLE_AV(send_main, argc, argv);
    RUN_EXAMPLE_AV(simplify_main, argc, argv);
    RUN_EXAMPLE_AV(switches_main, argc, argv);
    RUN_EXAMPLE_AV(tagnames_main, argc, argv);
    RUN_EXAMPLE_AV(text_formatting_main, argc, argv);
    RUN_EXAMPLE_AV(timing_main, argc, argv);
    RUN_EXAMPLE_AV(transform_main, argc, argv);
    return 0;
}

#endif
