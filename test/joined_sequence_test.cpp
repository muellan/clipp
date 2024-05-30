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

namespace {

//-------------------------------------------------------------------
struct test_active {
    test_active() = default;

    explicit
    test_active(int i_, char a_, char b_, char c_) : i{i_}, a{a_}, b{b_}, c{c_} {}

    test_active(int i_, const std::string& s_) : i{i_}, s{s_} {}

    int i = 0;
    char a = ' ', b = ' ', c = ' ';
    std::string s = "";

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.i == y.i &&
                x.a == y.a && x.b == y.b && x.c == y.c &&
                x.s == y.s);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches)
{
    using namespace clipp;

    auto one_char = [](const std::string& arg) -> subrange {
        if(arg.empty() || !std::isalpha(arg[0])) {
            return subrange{};
        }
        return subrange{0,1};
    };

    test_active m;

    auto mopt = option("-m").set(m.i,4) & values(one_char, "char", [&](const std::string& a) {
        m.s += a;
    });

    auto cli = group(
        option("-i").set(m.i,1) & value(one_char, "A", m.a),
        option("-j").set(m.i,2) & value(one_char, "A", m.a) & value(one_char, "B", m.b),
        option("-k").set(m.i,3) & value(one_char, "A", m.a) & value(one_char, "B", m.b) & value(one_char, "C", m.c),
        std::move(mopt),
        option("-o").set(m.i,9) & value("str", m.s)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"-i"               }, test_active{1, ' ', ' ', ' '});
        test(__LINE__, {"-i", "x"          }, test_active{1, 'x', ' ', ' '});
        test(__LINE__, {"-i", "x", "y"     }, test_active{1, 'x', ' ', ' '});
        test(__LINE__, {"-i", "x", "y", "z"}, test_active{1, 'x', ' ', ' '});

        test(__LINE__, {"-j"               }, test_active{2, ' ', ' ', ' '});
        test(__LINE__, {"-j", "x"          }, test_active{2, 'x', ' ', ' '});
        test(__LINE__, {"-j", "x", "y"     }, test_active{2, 'x', 'y', ' '});
        test(__LINE__, {"-j", "x", "y", "z"}, test_active{2, 'x', 'y', ' '});

        test(__LINE__, {"-k"               }, test_active{3, ' ', ' ', ' '});
        test(__LINE__, {"-k", "x"          }, test_active{3, 'x', ' ', ' '});
        test(__LINE__, {"-k", "x", "y"     }, test_active{3, 'x', 'y', ' '});
        test(__LINE__, {"-k", "x", "y", "z"}, test_active{3, 'x', 'y', 'z'});

        test(__LINE__, {"-m"               }, test_active{4, ""});
        test(__LINE__, {"-m", "x"          }, test_active{4, "x"});
        test(__LINE__, {"-m", "x", "y"     }, test_active{4, "xy"});
        test(__LINE__, {"-m", "x", "y", "z"}, test_active{4, "xyz"});

        test(__LINE__, {"-o"               }, test_active{9, ""});
        test(__LINE__, {"-o", "x"          }, test_active{9, "x"});
        test(__LINE__, {"-o", "x", "y"     }, test_active{9, "x"});
        test(__LINE__, {"-o", "x", "y", "z"}, test_active{9, "x"});


        //joined sequence
        test(__LINE__, {"-i"   }, test_active{1, ' ', ' ', ' '});
        test(__LINE__, {"-ix"  }, test_active{1, 'x', ' ', ' '});
        test(__LINE__, {"-ixy" }, test_active{0, ' ', ' ', ' '});
        test(__LINE__, {"-ixyz"}, test_active{0, ' ', ' ', ' '});

        test(__LINE__, {"-j"   }, test_active{2, ' ', ' ', ' '});
        test(__LINE__, {"-jx"  }, test_active{2, 'x', ' ', ' '});
        test(__LINE__, {"-jxy" }, test_active{2, 'x', 'y', ' '});
        test(__LINE__, {"-jxyz"}, test_active{0, ' ', ' ', ' '});

        test(__LINE__, {"-k"    }, test_active{3, ' ', ' ', ' '});
        test(__LINE__, {"-kx"   }, test_active{3, 'x', ' ', ' '});
        test(__LINE__, {"-kxy"  }, test_active{3, 'x', 'y', ' '});
        test(__LINE__, {"-kxyz" }, test_active{3, 'x', 'y', 'z'});
        test(__LINE__, {"-kxyza"}, test_active{0, ' ', ' ', ' '});

        test(__LINE__, {"-m"    }, test_active{4, ""});
        test(__LINE__, {"-mx"   }, test_active{4, "x"});
        test(__LINE__, {"-mxy"  }, test_active{4, "xy"});
        test(__LINE__, {"-mxyz" }, test_active{4, "xyz"});
        test(__LINE__, {"-mxyza"}, test_active{4, "xyza"});

        test(__LINE__, {"-o"    }, test_active{9, ""});
        test(__LINE__, {"-ox"   }, test_active{9, "x"});
        test(__LINE__, {"-oxy"  }, test_active{9, "xy"});
        test(__LINE__, {"-oxyz" }, test_active{9, "xyz"});
        test(__LINE__, {"-oxyza"}, test_active{9, "xyza"});


    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
