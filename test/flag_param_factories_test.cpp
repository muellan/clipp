/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"


//-------------------------------------------------------------------
/** @brief test if parameter factory call expressions compile
 */
void test_init_expressions_compile(int argc, char* argv[])
{
    using namespace clipp;

    arg_string a ("-a");
    arg_string b ("-b");
    arg_string c ("-c");

    auto cli = (
        option("-a", "-b", "-c"),
        option(a, "-b", "-c"),
        option("-a", b, "-c"),
        option("-a", "-b", c),
        option(a, b, "-c"),
        option(a, "-b", c),
        option("-a", b, c),
        option(a, b, c)
        ,
        required("-a", "-b", "-c"),
        required(a, "-b", "-c"),
        required("-a", b, "-c"),
        required("-a", "-b", c),
        required(a, b, "-c"),
        required(a, "-b", c),
        required("-a", b, c),
        required(a, b, c)
        ,
        command("-a", "-b", "-c"),
        command(a, "-b", "-c"),
        command("-a", b, "-c"),
        command("-a", "-b", c),
        command(a, b, "-c"),
        command(a, "-b", c),
        command("-a", b, c),
        command(a, b, c)
    );

    parse(argc, argv, cli);
}


//-------------------------------------------------------------------
template<class... Strings>
void test_param_init(int lineNo,
                    std::initializer_list<const char*> expexted,
                    Strings&&... init)
{
    using namespace clipp;

    auto p = parameter{std::forward<Strings>(init)...};

    if( !std::equal(begin(p.flags()), end(p.flags()), begin(expexted)) ) {
        throw std::runtime_error{"failed test " + std::string(__FILE__) +
            " in line " + std::to_string(lineNo)};
    }
}



//-------------------------------------------------------------------
int main(int argc, char* argv[])
{
    test_init_expressions_compile(argc, argv);

    using clipp::arg_string;
    using clipp::arg_list;

    try {
        //one letter flags
        test_param_init( __LINE__ , {"a"}, "a");
        test_param_init( __LINE__ , {"a", "b"}, "a", "b");
        test_param_init( __LINE__ , {"a", "b", "c"}, "a", "b", "c");
        test_param_init( __LINE__ , {"a", "b", "c", "d"}, "a", "b", "c", "d");

        //mixed string types
        test_param_init( __LINE__ , {"a"}, arg_string("a"));
        test_param_init( __LINE__ , {"a", "b"}, arg_string("a"), "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", arg_string("b"));
        test_param_init( __LINE__ , {"a", "b", "c"}, "a", arg_string("b"), "c");

        //lists
        test_param_init( __LINE__ , {"a"}, arg_list{"a"});
        test_param_init( __LINE__ , {"a", "b"}, arg_list{"a", "b"});
        test_param_init( __LINE__ , {"a", "b", "c"}, arg_list{"a", "b", "c"});
        test_param_init( __LINE__ , {"a", "b", "c", "d"}, arg_list{"a", "b", "c", "d"});

        //mulit-letter flags
        test_param_init( __LINE__ , {"-a"}, "-a");
        test_param_init( __LINE__ , {"-a", "-b"}, "-a", "-b");
        test_param_init( __LINE__ , {"-a", "-b", "-c"}, "-a", "-b", "-c");
        test_param_init( __LINE__ , {"-a", "-b", "-c", "-d"}, "-a", "-b", "-c", "-d");

        test_param_init( __LINE__ , {"a", "-b"}, "a", "-b");
        test_param_init( __LINE__ , {"a", "-b", "c"}, "a", "-b", "c");
        test_param_init( __LINE__ , {"a", "-b", "c", "d"}, "a", "-b", "c", "d");

        test_param_init( __LINE__ , {"--aa"}, "--aa");
        test_param_init( __LINE__ , {"--aa", "--bb-ee"}, "--aa", "--bb-ee");
        test_param_init( __LINE__ , {"--aa", "--bb-ee", "cee"}, "--aa", "--bb-ee", "cee");

        //empty flags are not allowed
        test_param_init( __LINE__ , {}, "");
        test_param_init( __LINE__ , {"a"}, "a", "");
        test_param_init( __LINE__ , {"a"}, "", "a");
        test_param_init( __LINE__ , {"a"}, "", "a", "");

        test_param_init( __LINE__ , {"a", "b"}, "", "a", "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", "", "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", "b", "");

        test_param_init( __LINE__ , {"a", "b", "c"}, "", "a", "b", "c");
        test_param_init( __LINE__ , {"a", "b", "c"}, "a", "", "b", "c");
        test_param_init( __LINE__ , {"a", "b", "c"}, "a", "b", "", "c");
        test_param_init( __LINE__ , {"a", "b", "c"}, "a", "b", "c", "");

        //flags with spaces are not allowed
        test_param_init( __LINE__ , {"a", "b"}, " ", "a", "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", " ", "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", "b", " ");

        test_param_init( __LINE__ , {"a", "b"}, "  ", "a", "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", "  ", "b");
        test_param_init( __LINE__ , {"a", "b"}, "a", "b", "  ");

        test_param_init( __LINE__ , {"-a"}, "- a" );
        test_param_init( __LINE__ , {"-a", "-b"}, "-a", "- b");
        test_param_init( __LINE__ , {"-a", "-b", "-c"}, "-a", "- b", "-c");
        test_param_init( __LINE__ , {"-a", "-b", "-c", "-d"}, "-a", "-b", "- c", "-d");

        test_param_init( __LINE__ , {"-a"}, " -  a" );
        test_param_init( __LINE__ , {"-a", "-b"}, "- a ", "   -   b");
        test_param_init( __LINE__ , {"-a", "-b", "-c"}, "-a", "-   b", " -   c");
        test_param_init( __LINE__ , {"-a", "-b", "-c", "-d"}, "-  a    ", "-    b", "  - c", "-d");

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
