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
    active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_, int i_ = 0) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, i{i_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;
    int i = 0;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f && x.i == y.i);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;

    active m;

    auto cli = (
        (   command("a"),
            option("-x").set(m.a)
        ).doc("group (~a,-x)")
        |
        (   command("b"),
            option("-x").set(m.b),
            (   command("u").set(m.i,1) |
                command("v").set(m.i,2) |
                command("w").set(m.i,3)
            ).doc("group (u|v|w)"),
            option("-e").set(m.e)
        ).doc("group (~b,-x,(~u|~v|~w),-e)")
        |
        (   command("c"),
            option("-x").set(m.c),
            command("d").set(m.d),
            required("-i").set(m.e) & value("i", m.i),
            option("-f").set(m.f)
        ).doc("group (~c,-x,d,(~-i,~i),-f)")
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{});

        test(__LINE__, {"a"}, active{});
        test(__LINE__, {"b"}, active{});
        test(__LINE__, {"c"}, active{});
        test(__LINE__, {"-x"}, active{});

        test(__LINE__, {"a", "-x"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"b", "-x"}, active{0,1,0,0,0,0, 0});
        test(__LINE__, {"c", "-x"}, active{0,0,1,0,0,0, 0});

        test(__LINE__, {"b", "u"}, active{0,0,0,0,0,0, 1});
        test(__LINE__, {"b", "v"}, active{0,0,0,0,0,0, 2});
        test(__LINE__, {"b", "w"}, active{0,0,0,0,0,0, 3});

        test(__LINE__, {"b", "-x", "u"}, active{0,1,0,0,0,0, 1});
        test(__LINE__, {"b", "-x", "v"}, active{0,1,0,0,0,0, 2});
        test(__LINE__, {"b", "-x", "w"}, active{0,1,0,0,0,0, 3});

        test(__LINE__, {"b", "u", "-x"}, active{0,0,0,0,0,0, 1});
        test(__LINE__, {"b", "v", "-x"}, active{0,0,0,0,0,0, 2});
        test(__LINE__, {"b", "w", "-x"}, active{0,0,0,0,0,0, 3});

        test(__LINE__, {"b", "-x", "u", "-e"}, active{0,1,0,0,1,0, 1});
        test(__LINE__, {"b", "-x", "v", "-e"}, active{0,1,0,0,1,0, 2});
        test(__LINE__, {"b", "-x", "w", "-e"}, active{0,1,0,0,1,0, 3});

        test(__LINE__, {"b", "u", "-x", "-e"}, active{0,0,0,0,1,0, 1});
        test(__LINE__, {"b", "v", "-x", "-e"}, active{0,0,0,0,1,0, 2});
        test(__LINE__, {"b", "w", "-x", "-e"}, active{0,0,0,0,1,0, 3});

        test(__LINE__, {"b", "-x", "u", "-e"}, active{0,1,0,0,1,0, 1});
        test(__LINE__, {"b", "-x", "v", "-e"}, active{0,1,0,0,1,0, 2});
        test(__LINE__, {"b", "-x", "w", "-e"}, active{0,1,0,0,1,0, 3});

        test(__LINE__, {"b", "-x", "-e", "u"}, active{0,1,0,0,0,0, 1});
        test(__LINE__, {"b", "-x", "-e", "v"}, active{0,1,0,0,0,0, 2});
        test(__LINE__, {"b", "-x", "-e", "w"}, active{0,1,0,0,0,0, 3});

        test(__LINE__, {"c", "-x", "-i"},  active{0,0,1,0,0,0, 0});
        test(__LINE__, {"c", "-x", "-f"},  active{0,0,1,0,0,0, 0});
        test(__LINE__, {"c", "-x", "123"}, active{0,0,1,0,0,0, 0});

        test(__LINE__, {"c", "-x", "-i", "123"}, active{0,0,1,0,0,0, 0});
        test(__LINE__, {"c", "-x", "-i", "-f"},  active{0,0,1,0,0,0, 0});
        test(__LINE__, {"c", "-x", "123", "-i"}, active{0,0,1,0,0,0, 0});
        test(__LINE__, {"c", "-x", "123", "-f"}, active{0,0,1,0,0,0, 0});

        test(__LINE__, {"c", "-x", "-i", "123", "-f"}, active{0,0,1,0,0,0, 0});

        test(__LINE__, {"c", "-x", "d"}, active{0,0,1,1,0,0, 0});

        test(__LINE__, {"c", "-x", "d", "-i"},  active{0,0,1,1,1,0, 0});
        test(__LINE__, {"c", "-x", "d", "-f"},  active{0,0,1,1,0,1, 0});
        test(__LINE__, {"c", "-x", "d", "123"}, active{0,0,1,1,0,0, 0});

        test(__LINE__, {"c", "-x", "d", "-i", "123"}, active{0,0,1,1,1,0, 123});
        test(__LINE__, {"c", "-x", "d", "-i", "-f"},  active{0,0,1,1,1,1, 0});
        test(__LINE__, {"c", "-x", "d", "123", "-i"}, active{0,0,1,1,1,0, 0});
        test(__LINE__, {"c", "-x", "d", "123", "-f"}, active{0,0,1,1,0,1, 0});

        test(__LINE__, {"c", "-x", "d", "-i", "123", "-f"}, active{0,0,1,1,1,1, 123});

        test(__LINE__, {"c", "-x", "d", "-f", "-i"}, active{0,0,1,1,1,1, 0});
        test(__LINE__, {"c", "-x", "d", "-f", "-i", "123"}, active{0,0,1,1,1,1, 123});

        test(__LINE__, {"c", "d", "-f", "-i", "123"}, active{0,0,0,1,1,1, 123});
        test(__LINE__, {"c", "d", "-i", "123", "-f"}, active{0,0,0,1,1,1, 123});

        test(__LINE__, {"c", "d", "-x", "-i", "123", "-f"}, active{0,0,0,1,1,1, 123});
        test(__LINE__, {"c", "d", "-f", "-i", "123", "-x"}, active{0,0,0,1,1,1, 123});


    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
