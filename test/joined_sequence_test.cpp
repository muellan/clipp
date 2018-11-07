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
    active(int i_, char a_, char b_, char c_) : i{i_}, a{a_}, b{b_}, c{c_} {}

    active(int i_, const std::string& s_) : i{i_}, s{s_} {}

    int i = 0;
    char a = ' ', b = ' ', c = ' ';
    std::string s = "";

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.i == y.i &&
                x.a == y.a && x.b == y.b && x.c == y.c &&
                x.s == y.s);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;

    auto one_char = [](const std::string& arg) {
        if(arg.empty() || !std::isalpha(arg[0])) return subrange{};
        return subrange{0,1};
    };

    active m;
    auto cli = group(
        option("-i").set(m.i,1) & value(one_char, "A", m.a),
        option("-j").set(m.i,2) & value(one_char, "A", m.a) & value(one_char, "B", m.b),
        option("-k").set(m.i,3) & value(one_char, "A", m.a) & value(one_char, "B", m.b) & value(one_char, "C", m.c)
        ,
        option("-m").set(m.i,4) & values(one_char, "char", [&](const std::string& a){m.s += a;})
        ,
        option("-o").set(m.i,9) & value("str", m.s)
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

        test(__LINE__, {"-i"               }, active{1, ' ', ' ', ' '});
        test(__LINE__, {"-i", "x"          }, active{1, 'x', ' ', ' '});
        test(__LINE__, {"-i", "x", "y"     }, active{1, 'x', ' ', ' '});
        test(__LINE__, {"-i", "x", "y", "z"}, active{1, 'x', ' ', ' '});

        test(__LINE__, {"-j"               }, active{2, ' ', ' ', ' '});
        test(__LINE__, {"-j", "x"          }, active{2, 'x', ' ', ' '});
        test(__LINE__, {"-j", "x", "y"     }, active{2, 'x', 'y', ' '});
        test(__LINE__, {"-j", "x", "y", "z"}, active{2, 'x', 'y', ' '});

        test(__LINE__, {"-k"               }, active{3, ' ', ' ', ' '});
        test(__LINE__, {"-k", "x"          }, active{3, 'x', ' ', ' '});
        test(__LINE__, {"-k", "x", "y"     }, active{3, 'x', 'y', ' '});
        test(__LINE__, {"-k", "x", "y", "z"}, active{3, 'x', 'y', 'z'});

        test(__LINE__, {"-m"               }, active{4, ""});
        test(__LINE__, {"-m", "x"          }, active{4, "x"});
        test(__LINE__, {"-m", "x", "y"     }, active{4, "xy"});
        test(__LINE__, {"-m", "x", "y", "z"}, active{4, "xyz"});

        test(__LINE__, {"-o"               }, active{9, ""});
        test(__LINE__, {"-o", "x"          }, active{9, "x"});
        test(__LINE__, {"-o", "x", "y"     }, active{9, "x"});
        test(__LINE__, {"-o", "x", "y", "z"}, active{9, "x"});


        //joined sequence
        test(__LINE__, {"-i"   }, active{1, ' ', ' ', ' '});
        test(__LINE__, {"-ix"  }, active{1, 'x', ' ', ' '});
        test(__LINE__, {"-ixy" }, active{0, ' ', ' ', ' '});
        test(__LINE__, {"-ixyz"}, active{0, ' ', ' ', ' '});

        test(__LINE__, {"-j"   }, active{2, ' ', ' ', ' '});
        test(__LINE__, {"-jx"  }, active{2, 'x', ' ', ' '});
        test(__LINE__, {"-jxy" }, active{2, 'x', 'y', ' '});
        test(__LINE__, {"-jxyz"}, active{0, ' ', ' ', ' '});

        test(__LINE__, {"-k"    }, active{3, ' ', ' ', ' '});
        test(__LINE__, {"-kx"   }, active{3, 'x', ' ', ' '});
        test(__LINE__, {"-kxy"  }, active{3, 'x', 'y', ' '});
        test(__LINE__, {"-kxyz" }, active{3, 'x', 'y', 'z'});
        test(__LINE__, {"-kxyza"}, active{0, ' ', ' ', ' '});

        test(__LINE__, {"-m"    }, active{4, ""});
        test(__LINE__, {"-mx"   }, active{4, "x"});
        test(__LINE__, {"-mxy"  }, active{4, "xy"});
        test(__LINE__, {"-mxyz" }, active{4, "xyz"});
        test(__LINE__, {"-mxyza"}, active{4, "xyza"});

        test(__LINE__, {"-o"    }, active{9, ""});
        test(__LINE__, {"-ox"   }, active{9, "x"});
        test(__LINE__, {"-oxy"  }, active{9, "xy"});
        test(__LINE__, {"-oxyz" }, active{9, "xyz"});
        test(__LINE__, {"-oxyza"}, active{9, "xyza"});


    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
