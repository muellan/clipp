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
class test_active {
public:
    test_active() { for(auto& x : b) x = false; }
    test_active(std::initializer_list<int> il) {
        if(il.size() != 17)
            throw std::logic_error{"test active not properly initialized"};

        auto i = il.begin();
        for(auto& x : b) {
            x = *i;
            ++i;
        }
    }

    bool& operator [] (int i)       noexcept { return b[i]; }
    bool  operator [] (int i) const noexcept { return b[i]; }

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        using std::begin; using std::end;
        return std::equal(begin(x.b), end(x.b), begin(y.b));
    }

private:
    bool b[17];
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches,
          const test_active& conflict = test_active{})
{
    using namespace clipp;

    test_active m;
    test_active c;

    auto cmd_a = command ("a"    ).set(m[ 0]).if_conflicted(set(c[ 0]));
    auto cmd_b = command ("b"    ).set(m[ 1]).if_conflicted(set(c[ 1]));
    auto cmd_c = command ("c"    ).set(m[ 2]).if_conflicted(set(c[ 2]));
    auto opt_d = option  ("-d"   ).set(m[ 3]).if_conflicted(set(c[ 3]));
    auto opt_e = option  ("-e"   ).set(m[ 4]).if_conflicted(set(c[ 4]));
    auto cmd_f = command ("f"    ).set(m[ 5]).if_conflicted(set(c[ 5]));
    auto opt_g = option  ("--gee").set(m[ 6]).if_conflicted(set(c[ 6]));
    auto cmd_h = command ("h"    ).set(m[ 7]).if_conflicted(set(c[ 7]));
    auto cmd_i = command ("i"    ).set(m[ 8]).if_conflicted(set(c[ 8]));
    auto cmd_j = command ("j"    ).set(m[ 9]).if_conflicted(set(c[ 9]));
    auto opt_k = option  ("k"    ).set(m[10]).if_conflicted(set(c[10]));
    auto cmd_l = command ("l"    ).set(m[11]).if_conflicted(set(c[11]));
    auto cmd_m = command ("m"    ).set(m[12]).if_conflicted(set(c[12]));
    auto req_n = required(">n"   ).set(m[13]).if_conflicted(set(c[13]));
    auto opt_o = option  ("-o"   ).set(m[14]).if_conflicted(set(c[14]));
    auto opt_x = option  ("-x"   ).set(m[15]).if_conflicted(set(c[15]));
    auto opt_y = option  ("y"    ).set(m[16]).if_conflicted(set(c[16]));

    auto cli = (
        cmd_a
        | ( (cmd_b | cmd_c), opt_d, opt_e)
        | ( cmd_f, opt_g)
        | ( cmd_h, ( cmd_i
                     | (cmd_j, opt_k)
                     | ((cmd_l | cmd_m), req_n, opt_o)
                   )
        ),
        opt_x, opt_y
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; c = test_active{}; },
              [&]{ return m == matches && c == conflict; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"-x"},      test_active{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0});
        test(__LINE__, {"y"},       test_active{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1});
        test(__LINE__, {"-x", "y"}, test_active{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"y", "-x"}, test_active{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1});

        test(__LINE__, {"a"}, test_active{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0});

        test(__LINE__, {"b"},             test_active{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"b", "-d", "-e"}, test_active{0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"b", "-e", "-d"}, test_active{0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0});

        test(__LINE__, {"c"},             test_active{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"c", "-d", "-e"}, test_active{0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"c", "-e", "-d"}, test_active{0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0});

        test(__LINE__, {"f"},          test_active{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"f", "--gee"}, test_active{0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0});

        test(__LINE__, {"h"},                  test_active{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"h", "i"},             test_active{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0});

        test(__LINE__, {"h", "j"},             test_active{0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0});
        test(__LINE__, {"h", "j", "k"},        test_active{0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0});

        test(__LINE__, {"h", "l"},             test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0});
        test(__LINE__, {"h", "l", ">n"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0});
        test(__LINE__, {"h", "l", "-o", ">n"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0});
        test(__LINE__, {"h", "l", ">n", "-o"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0});

        test(__LINE__, {"h", "m"},             test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0});
        test(__LINE__, {"h", "m", ">n"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0});
        test(__LINE__, {"h", "m", "-o", ">n"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0});
        test(__LINE__, {"h", "m", ">n", "-o"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0});


        test(__LINE__, {"a", "y", "-x"}, test_active{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1});

        test(__LINE__, {"b", "y", "-x"},             test_active{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"b", "-e", "-d", "y", "-x"}, test_active{0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"b", "-e", "-d", "-x", "y"}, test_active{0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1});

        test(__LINE__, {"c", "y", "-x"},             test_active{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"c", "-e", "-d", "y", "-x"}, test_active{0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"c", "-e", "-d", "-x", "y"}, test_active{0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1});

        test(__LINE__, {"f", "y", "-x"},          test_active{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"f", "--gee", "y", "-x"}, test_active{0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1});
        test(__LINE__, {"f", "--gee", "-x", "y"}, test_active{0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1});

        test(__LINE__, {"h"},                 test_active{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0});
        test(__LINE__, {"h", "i", "y", "-x"}, test_active{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1});

        test(__LINE__, {"h", "j"},                        test_active{0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0});
        test(__LINE__, {"h", "j", "k", "y", "-x"},        test_active{0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1});
        test(__LINE__, {"h", "j", "k", "-x", "y"},        test_active{0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1});

        test(__LINE__, {"h", "l"},                        test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0});
        test(__LINE__, {"h", "l", ">n"},                  test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0});
        test(__LINE__, {"h", "l", ">n", "y", "-x"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1});
        test(__LINE__, {"h", "l", ">n", "-x", "y"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1});
        test(__LINE__, {"h", "l", ">n", "-x", "y"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1});
        test(__LINE__, {"h", "l", "-x", ">n", "y"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1});
        test(__LINE__, {"h", "l", "-x", "y", ">n"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1});
        test(__LINE__, {"h", "l", "y", "-x", ">n"},       test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1});
        test(__LINE__, {"h", "l", "-o", ">n", "y", "-x"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1});
        test(__LINE__, {"h", "l", ">n", "-o", "y", "-x"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1});
        test(__LINE__, {"h", "l", "-o", ">n", "-x", "y"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1});
        test(__LINE__, {"h", "l", ">n", "-x", "-o", "y"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1});
        test(__LINE__, {"h", "l", "-x", "-o", ">n", "y"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1});
        test(__LINE__, {"h", "l", "-x", "y", ">n", "-o"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1});
        test(__LINE__, {"h", "l", "y", "-o", "-x", ">n"}, test_active{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1});

        test(__LINE__, {"h", "m"},                        test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0});
        test(__LINE__, {"h", "m", ">n"},                  test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0});
        test(__LINE__, {"h", "m", ">n", "y", "-x"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,1,1});
        test(__LINE__, {"h", "m", ">n", "-x", "y"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,1,1});
        test(__LINE__, {"h", "m", "-x", ">n", "y"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1});
        test(__LINE__, {"h", "m", ">n", "-x", "y"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,1,1});
        test(__LINE__, {"h", "m", "-x", "y", ">n"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1});
        test(__LINE__, {"h", "m", "y", "-x", ">n"},       test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1});
        test(__LINE__, {"h", "m", "-o", ">n", "y", "-x"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1});
        test(__LINE__, {"h", "m", ">n", "-o", "y", "-x"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1});
        test(__LINE__, {"h", "m", "-o", ">n", "-x", "y"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1});
        test(__LINE__, {"h", "m", "-x", "-o", ">n", "y"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1});
        test(__LINE__, {"h", "m", ">n", "-x", "-o", "y"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,1,1});
        test(__LINE__, {"h", "m", "-x", "y", ">n", "-o"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1});
        test(__LINE__, {"h", "m", "y", "-o", "-x", ">n"}, test_active{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
