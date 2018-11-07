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
struct counters {
    counters() = default;

    explicit
    counters(int a_, int b_): a{a_}, b{b_} {}

    int a = 0, b = 0;

    friend bool operator == (const counters& x, const counters& y) noexcept {
        return (x.a == y.a && x.b == y.b);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const counters& matches)
{
    using namespace clipp;

    counters m;
    auto cli1 = joinable(
        repeatable(
            option("a").call([&]{++m.a;}),
            option("b").call([&]{++m.b;})
        )
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli1,
              [&]{ m = counters{}; },
              [&]{ return m == matches; });


    auto cli2 = joinable(
        repeatable( option("a").call([&]{++m.a;}) ),
        repeatable( option("b").call([&]{++m.b;}) )
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli2,
              [&]{ m = counters{}; },
              [&]{ return m == matches; });
}



//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {}, counters{});

        test(__LINE__, {"a"}, counters{1,0});
        test(__LINE__, {"b"}, counters{0,1});

        test(__LINE__, {"a","a"}, counters{2,0});
        test(__LINE__, {"b","b"}, counters{0,2});
        test(__LINE__, {"a","b"}, counters{1,1});

        test(__LINE__, {"a","a","a"}, counters{3,0});
        test(__LINE__, {"b","b","b"}, counters{0,3});
        test(__LINE__, {"a","b","a"}, counters{2,1});
        test(__LINE__, {"b","a","b"}, counters{1,2});

        test(__LINE__, {"b","a","a","b"}, counters{2,2});
        test(__LINE__, {"a","b","b","a"}, counters{2,2});


        test(__LINE__, {"aa"}, counters{2,0});
        test(__LINE__, {"bb"}, counters{0,2});

        test(__LINE__, {"aaa"}, counters{3,0});
        test(__LINE__, {"bbb"}, counters{0,3});

        test(__LINE__, {"abbb"}, counters{1,3});
        test(__LINE__, {"abba"}, counters{2,2});
        test(__LINE__, {"baba"}, counters{2,2});
        test(__LINE__, {"abab"}, counters{2,2});

        test(__LINE__, {"abaabbabbbbaaabaabababab"}, counters{12,12});

        test(__LINE__, {"a", "bba", "b", "aba", "bb", "aaaa"}, counters{8,6});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
