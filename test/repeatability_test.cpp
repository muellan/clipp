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
struct active {
    active() = default;

    explicit
    active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_,
           std::initializer_list<int> i_) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, i{i_}
    {}

    bool a = false, b = false, c = false, d = false, e = false, f = false;
    std::vector<int> i;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c &&
                x.d == y.d && x.e == y.e && x.f == y.f &&
                std::equal(begin(x.i), end(x.i), begin(y.i)));
    }
};

struct repeats {
    repeats() = default;

    explicit
    repeats(int a_, int b_, int c_, int d_, int e_, int f_, int i_) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, i{i_}
    {}

    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, i = 0;

    friend bool operator == (const repeats& x, const repeats& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c &&
                x.d == y.d && x.e == y.e && x.f == y.f && x.i == y.i);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches,
          const repeats& repeated)
{
    using namespace clipp;

    active m;
    repeats r;
    auto cli = group(
        //repeatable option
        option("a").repeatable(true).set(m.a).if_repeated(increment(r.a))
        ,
        //repeatable option + exactly one optional value
        option("b").repeatable(true).set(m.b).if_repeated(increment(r.b)) &
            opt_value("B", m.i).if_repeated(increment(r.i))
        ,
        //option + values...
        option("c").set(m.c).if_repeated(increment(r.c)) &
            values("C", m.i).if_repeated(increment(r.i))
        ,
        //repeatable group of (option + value)
        repeatable(
            option("d").set(m.d).if_repeated(increment(r.d)) &
            value("D", m.i).if_repeated(increment(r.i))
        )
        ,
        //non-repeatable option
        option("e").set(m.e).if_repeated(increment(r.e))
        ,
        //repeatable group of (option + values...)
        //Note that we have to make sure, that args {"f", "3", "f", "4"}
        //won't be interpreted as "f" followed by 3 values.
        //This is achieved with the prefix_not predicate which excludes "f"
        //itself from beeing matched as a value.
        repeatable(
            option("f").set(m.f).if_repeated(increment(r.f)) &
            values(match::prefix_not("f"), "F", m.i).if_repeated(increment(r.i))
        )
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; r = repeats{}; },
              [&]{ return m == matches && r == repeated; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{}, repeats{});
                                                            //     a b c d e f i           a b c d e f i
        test(__LINE__, {"a"},                          active{1,0,0,0,0,0,{}}, repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"a", "a"},                     active{1,0,0,0,0,0,{}}, repeats{1,0,0,0,0,0,0});
        test(__LINE__, {"a", "a", "a"},                active{1,0,0,0,0,0,{}}, repeats{2,0,0,0,0,0,0});
        test(__LINE__, {"a", "a", "a", "a"},           active{1,0,0,0,0,0,{}}, repeats{3,0,0,0,0,0,0});
        test(__LINE__, {"a", "a", "a", "a", "a" },     active{1,0,0,0,0,0,{}}, repeats{4,0,0,0,0,0,0});
        test(__LINE__, {"a", "a", "e", "a", "a", "a"}, active{1,0,0,0,1,0,{}}, repeats{4,0,0,0,0,0,0});

        test(__LINE__, {"b"},                     active{0,1,0,0,0,0,{}},  repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"b", "b", "2"},           active{0,1,0,0,0,0,{2}}, repeats{0,1,0,0,0,0,0});
        test(__LINE__, {"b", "2", "b"},           active{0,1,0,0,0,0,{2}}, repeats{0,1,0,0,0,0,0});
        test(__LINE__, {"b", "a", "b"},           active{1,1,0,0,0,0,{}},  repeats{0,1,0,0,0,0,0});
        test(__LINE__, {"b", "b", "2", "3"},      active{0,1,0,0,0,0,{2}}, repeats{0,1,0,0,0,0,1});
        test(__LINE__, {"b", "2", "b", "b", "3"}, active{0,1,0,0,0,0,{2}}, repeats{0,2,0,0,0,0,1});

        test(__LINE__, {"c"},                active{0,0,1,0,0,0,{}},    repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"c", "c"},           active{0,0,1,0,0,0,{}},    repeats{0,0,1,0,0,0,0});
        test(__LINE__, {"c", "2"},           active{0,0,1,0,0,0,{2}},   repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"c", "2", "3"},      active{0,0,1,0,0,0,{2,3}}, repeats{0,0,0,0,0,0,1});
        test(__LINE__, {"c", "c", "2", "3"}, active{0,0,1,0,0,0,{2,3}}, repeats{0,0,1,0,0,0,1});
        test(__LINE__, {"c", "2", "3", "c"}, active{0,0,1,0,0,0,{2,3}}, repeats{0,0,1,0,0,0,1});

        test(__LINE__, {"d"},                active{0,0,0,1,0,0,{}},    repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"d", "d"},           active{0,0,0,1,0,0,{}},    repeats{0,0,0,1,0,0,0});
        test(__LINE__, {"d", "2"},           active{0,0,0,1,0,0,{2}},   repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"d", "2", "d", "3"}, active{0,0,0,1,0,0,{2,3}}, repeats{0,0,0,1,0,0,1});
        test(__LINE__, {"d", "2", "3"},      active{0,0,0,1,0,0,{2}},   repeats{0,0,0,0,0,0,0});

        test(__LINE__, {"e"},                          active{0,0,0,0,1,0,{}}, repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"e", "a"},                     active{1,0,0,0,1,0,{}}, repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"e", "e"},                     active{0,0,0,0,1,0,{}}, repeats{0,0,0,0,1,0,0});
        test(__LINE__, {"e", "e", "e"},                active{0,0,0,0,1,0,{}}, repeats{0,0,0,0,2,0,0});
        test(__LINE__, {"e", "a", "e", "e"},           active{1,0,0,0,1,0,{}}, repeats{0,0,0,0,2,0,0});
        test(__LINE__, {"e", "b", "e", "a", "e" },     active{1,1,0,0,1,0,{}}, repeats{0,0,0,0,2,0,0});
        test(__LINE__, {"e", "a", "a", "e", "e", "a"}, active{1,0,0,0,1,0,{}}, repeats{2,0,0,0,2,0,0});

        test(__LINE__, {"f"},                          active{0,0,0,0,0,1,{}},      repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"f", "2"},                     active{0,0,0,0,0,1,{2}},     repeats{0,0,0,0,0,0,0});
        test(__LINE__, {"f", "2", "3"},                active{0,0,0,0,0,1,{2,3}},   repeats{0,0,0,0,0,0,1});
        test(__LINE__, {"f", "2", "f", "3"},           active{0,0,0,0,0,1,{2,3}},   repeats{0,0,0,0,0,1,1});
        test(__LINE__, {"f", "2", "3", "4"},           active{0,0,0,0,0,1,{2,3,4}}, repeats{0,0,0,0,0,0,2});
        test(__LINE__, {"f", "2", "f", "3", "4"},      active{0,0,0,0,0,1,{2,3,4}}, repeats{0,0,0,0,0,1,2});
        test(__LINE__, {"f", "2", "3", "f", "4"},      active{0,0,0,0,0,1,{2,3,4}}, repeats{0,0,0,0,0,1,2});
        test(__LINE__, {"f", "2", "f", "3", "f", "4"}, active{0,0,0,0,0,1,{2,3,4}}, repeats{0,0,0,0,0,2,2});

        test(__LINE__, {"f", "f"},                     active{0,0,0,0,0,1,{}},    repeats{0,0,0,0,0,1,0});
        test(__LINE__, {"f", "2", "3", "f"},           active{0,0,0,0,0,1,{2,3}}, repeats{0,0,0,0,0,1,1});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
