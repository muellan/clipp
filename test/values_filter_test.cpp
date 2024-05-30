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
    test_active(int i_, int j_, int k_,
           std::size_t l_, std::size_t m_, std::size_t n_,
           float u_, float v_, float w_,
           double x_, double y_, double z_)
    :
        i{i_}, j{j_}, k{k_}, l{l_}, m{m_}, n{n_},
        u{u_}, v{v_}, w{w_}, x{x_}, y{y_}, z{z_}
    {}

    int i = 1, j = 1, k = 1;
    std::size_t l = 1, m = 1, n = 1;
    float u = 1.0f, v = 1.0f, w = 1.0f;
    double x = 1.0, y = 1.0, z = 1.0;

    friend bool operator == (const test_active& a, const test_active& b) noexcept {
        return (a.i == b.i && a.j == b.j && a.k == b.k && a.l == b.l &&
                a.m == b.m && a.n == b.n && a.u == b.u && a.v == b.v &&
                a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches)
{
    using namespace clipp;

    test_active m;
    auto cli = group(
        option("i") & value("I", m.i),
        option("j") & value("J", m.j),
        option("k") & value("K", m.k),
        option("l") & value("L", m.l),
        option("m") & value("M", m.m),
        option("n") & value("N", m.n),
        option("u") & value("U", m.u),
        option("v") & value("V", m.v),
        option("w") & value("W", m.w),
        option("x") & value("X", m.x),
        option("y") & value("Y", m.y),
        option("z") & value("Z", m.z)
        ,
        option("-i") & number("I", m.i),
        option("-j") & number("J", m.j),
        option("-k") & number("K", m.k),
        option("-l") & number("L", m.l),
        option("-m") & number("M", m.m),
        option("-n") & number("N", m.n),
        option("-u") & number("U", m.u),
        option("-v") & number("V", m.v),
        option("-w") & number("W", m.w),
        option("-x") & number("X", m.x),
        option("-y") & number("Y", m.y),
        option("-z") & number("Z", m.z)
        ,
        option("--i") & integer("I", m.i),
        option("--j") & integer("J", m.j),
        option("--k") & integer("K", m.k),
        option("--l") & integer("L", m.l),
        option("--m") & integer("M", m.m),
        option("--n") & integer("N", m.n)
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

    test(__LINE__, {"u", "3.45ee"}, test_active{1,1,1, 1u, 1u, 1u,  3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"v", "3.45ee"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"w", "3.45ee"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"x", "3.45ee"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45, 1.00, 1.00});
    test(__LINE__, {"y", "3.45ee"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45, 1.00});
    test(__LINE__, {"z", "3.45ee"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45});

    test(__LINE__, {"u", "3.45e+"}, test_active{1,1,1, 1u, 1u, 1u,  3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"v", "3.45e+"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"w", "3.45e+"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"x", "3.45e+"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45, 1.00, 1.00});
    test(__LINE__, {"y", "3.45e+"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45, 1.00});
    test(__LINE__, {"z", "3.45e+"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45});

    test(__LINE__, {"u", "3.45e-"}, test_active{1,1,1, 1u, 1u, 1u,  3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"v", "3.45e-"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"w", "3.45e-"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"x", "3.45e-"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45, 1.00, 1.00});
    test(__LINE__, {"y", "3.45e-"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45, 1.00});
    test(__LINE__, {"z", "3.45e-"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45});

    test(__LINE__, {""}, test_active{});

    test(__LINE__, {"i", "2"}, test_active{2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"j", "2"}, test_active{1,2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"k", "2"}, test_active{1,1,2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"l", "2"}, test_active{1,1,1, 2u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"m", "2"}, test_active{1,1,1, 1u, 2u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"n", "2"}, test_active{1,1,1, 1u, 1u, 2u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"u", "2"}, test_active{1,1,1, 1u, 1u, 1u,  2.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"v", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 2.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"w", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 2.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"x", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  2.0, 1.0, 1.0});
    test(__LINE__, {"y", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 2.0, 1.0});
    test(__LINE__, {"z", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 2.0});

    test(__LINE__, {"i", "2.3"}, test_active{2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"j", "2.3"}, test_active{1,2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"k", "2.3"}, test_active{1,1,2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"l", "2.3"}, test_active{1,1,1, 2u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"m", "2.3"}, test_active{1,1,1, 1u, 2u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"n", "2.3"}, test_active{1,1,1, 1u, 1u, 2u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"u", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  2.3f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"v", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 2.3f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"w", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 2.3f,  1.0, 1.0, 1.0});
    test(__LINE__, {"x", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  2.3, 1.0, 1.0});
    test(__LINE__, {"y", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 2.3, 1.0});
    test(__LINE__, {"z", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 2.3});

    test(__LINE__, {"i", "3.45"}, test_active{3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"j", "3.45"}, test_active{1,3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"k", "3.45"}, test_active{1,1,3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"l", "3.45"}, test_active{1,1,1, 3u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"m", "3.45"}, test_active{1,1,1, 1u, 3u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"n", "3.45"}, test_active{1,1,1, 1u, 1u, 3u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"u", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"v", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"w", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"x", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45, 1.00, 1.00});
    test(__LINE__, {"y", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45, 1.00});
    test(__LINE__, {"z", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45});

    test(__LINE__, {"i", "-2"}, test_active{-2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"j", "-2"}, test_active{1,-2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"k", "-2"}, test_active{1,1,-2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"l", "-2"}, test_active{1,1,1, 0u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"m", "-2"}, test_active{1,1,1, 1u, 0u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"n", "-2"}, test_active{1,1,1, 1u, 1u, 0u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"u", "-2"}, test_active{1,1,1, 1u, 1u, 1u, -2.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"v", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f,-2.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"w", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f,-2.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"x", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f, -2.0, 1.0, 1.0});
    test(__LINE__, {"y", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0,-2.0, 1.0});
    test(__LINE__, {"z", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0,-2.0});

    test(__LINE__, {"i", "-2.3"}, test_active{-2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"j", "-2.3"}, test_active{1,-2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"k", "-2.3"}, test_active{1,1,-2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"l", "-2.3"}, test_active{1,1,1, 0u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"m", "-2.3"}, test_active{1,1,1, 1u, 0u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"n", "-2.3"}, test_active{1,1,1, 1u, 1u, 0u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"u", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u, -2.3f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"v", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f,-2.3f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"w", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f,-2.3f,  1.0, 1.0, 1.0});
    test(__LINE__, {"x", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f, -2.3, 1.0, 1.0});
    test(__LINE__, {"y", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0,-2.3, 1.0});
    test(__LINE__, {"z", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0,-2.3});

    test(__LINE__, {"i", "-3.45"}, test_active{-3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"j", "-3.45"}, test_active{1,-3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"k", "-3.45"}, test_active{1,1,-3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"l", "-3.45"}, test_active{1,1,1, 0u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"m", "-3.45"}, test_active{1,1,1, 1u, 0u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"n", "-3.45"}, test_active{1,1,1, 1u, 1u, 0u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"u", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u, -3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"v", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f,-3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"w", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f,-3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"x", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f, -3.45, 1.00, 1.00});
    test(__LINE__, {"y", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00,-3.45, 1.00});
    test(__LINE__, {"z", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00,-3.45});


    test(__LINE__, {"-i", "2"}, test_active{2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-j", "2"}, test_active{1,2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-k", "2"}, test_active{1,1,2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-l", "2"}, test_active{1,1,1, 2u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-m", "2"}, test_active{1,1,1, 1u, 2u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-n", "2"}, test_active{1,1,1, 1u, 1u, 2u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-u", "2"}, test_active{1,1,1, 1u, 1u, 1u,  2.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-v", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 2.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-w", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 2.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-x", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  2.0, 1.0, 1.0});
    test(__LINE__, {"-y", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 2.0, 1.0});
    test(__LINE__, {"-z", "2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 2.0});

    test(__LINE__, {"-i", "2.3"}, test_active{2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-j", "2.3"}, test_active{1,2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-k", "2.3"}, test_active{1,1,2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-l", "2.3"}, test_active{1,1,1, 2u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-m", "2.3"}, test_active{1,1,1, 1u, 2u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-n", "2.3"}, test_active{1,1,1, 1u, 1u, 2u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-u", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  2.3f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-v", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 2.3f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-w", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 2.3f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-x", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  2.3, 1.0, 1.0});
    test(__LINE__, {"-y", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 2.3, 1.0});
    test(__LINE__, {"-z", "2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 2.3});

    test(__LINE__, {"-i", "3.45"}, test_active{3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-j", "3.45"}, test_active{1,3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-k", "3.45"}, test_active{1,1,3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-l", "3.45"}, test_active{1,1,1, 3u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-m", "3.45"}, test_active{1,1,1, 1u, 3u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-n", "3.45"}, test_active{1,1,1, 1u, 1u, 3u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-u", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-v", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-w", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-x", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45, 1.00, 1.00});
    test(__LINE__, {"-y", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45, 1.00});
    test(__LINE__, {"-z", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45});

    test(__LINE__, {"-u", "3.45e3"}, test_active{1,1,1, 1u, 1u, 1u,  3.45e3f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-v", "3.45e3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45e3f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-w", "3.45e3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45e3f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-x", "3.45e3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45e3, 1.00, 1.00});
    test(__LINE__, {"-y", "3.45e3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45e3, 1.00});
    test(__LINE__, {"-z", "3.45e3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45e3});

    test(__LINE__, {"-u", "3.45e+3"}, test_active{1,1,1, 1u, 1u, 1u,  3.45e3f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-v", "3.45e+3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45e3f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-w", "3.45e+3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45e3f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-x", "3.45e+3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45e3, 1.00, 1.00});
    test(__LINE__, {"-y", "3.45e+3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45e3, 1.00});
    test(__LINE__, {"-z", "3.45e+3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45e3});

    test(__LINE__, {"-u", "3.45e-3"}, test_active{1,1,1, 1u, 1u, 1u,  3.45e-3f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-v", "3.45e-3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.45e-3f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-w", "3.45e-3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.45e-3f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-x", "3.45e-3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.45e-3, 1.00, 1.00});
    test(__LINE__, {"-y", "3.45e-3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.45e-3, 1.00});
    test(__LINE__, {"-z", "3.45e-3"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.45e-3});


    test(__LINE__, {"-i", "-2"}, test_active{-2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-j", "-2"}, test_active{1,-2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-k", "-2"}, test_active{1,1,-2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-l", "-2"}, test_active{1,1,1, 0u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-m", "-2"}, test_active{1,1,1, 1u, 0u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-n", "-2"}, test_active{1,1,1, 1u, 1u, 0u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-u", "-2"}, test_active{1,1,1, 1u, 1u, 1u, -2.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-v", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f,-2.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-w", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f,-2.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-x", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f, -2.0, 1.0, 1.0});
    test(__LINE__, {"-y", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0,-2.0, 1.0});
    test(__LINE__, {"-z", "-2"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0,-2.0});

    test(__LINE__, {"-i", "-2.3"}, test_active{-2,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-j", "-2.3"}, test_active{1,-2,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-k", "-2.3"}, test_active{1,1,-2, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-l", "-2.3"}, test_active{1,1,1, 0u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-m", "-2.3"}, test_active{1,1,1, 1u, 0u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-n", "-2.3"}, test_active{1,1,1, 1u, 1u, 0u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-u", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u, -2.3f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-v", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f,-2.3f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-w", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f,-2.3f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-x", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f, -2.3, 1.0, 1.0});
    test(__LINE__, {"-y", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0,-2.3, 1.0});
    test(__LINE__, {"-z", "-2.3"}, test_active{1,1,1, 1u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0,-2.3});

    test(__LINE__, {"-i", "-3.45"}, test_active{-3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-j", "-3.45"}, test_active{1,-3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-k", "-3.45"}, test_active{1,1,-3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-l", "-3.45"}, test_active{1,1,1, 0u, 1u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-m", "-3.45"}, test_active{1,1,1, 1u, 0u, 1u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-n", "-3.45"}, test_active{1,1,1, 1u, 1u, 0u,  1.0f, 1.0f, 1.0f,  1.0, 1.0, 1.0});
    test(__LINE__, {"-u", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u, -3.45f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-v", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f,-3.45f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-w", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f,-3.45f,  1.00, 1.00, 1.00});
    test(__LINE__, {"-x", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f, -3.45, 1.00, 1.00});
    test(__LINE__, {"-y", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00,-3.45, 1.00});
    test(__LINE__, {"-z", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00,-3.45});


    test(__LINE__, {"--i", "3"}, test_active{3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--j", "3"}, test_active{1,3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--k", "3"}, test_active{1,1,3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--l", "3"}, test_active{1,1,1, 3u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--m", "3"}, test_active{1,1,1, 1u, 3u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--n", "3"}, test_active{1,1,1, 1u, 1u, 3u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});

    test(__LINE__, {"--i", "-3"}, test_active{-3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--j", "-3"}, test_active{1,-3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--k", "-3"}, test_active{1,1,-3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--l", "-3"}, test_active{1,1,1, 0u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--m", "-3"}, test_active{1,1,1, 1u, 0u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--n", "-3"}, test_active{1,1,1, 1u, 1u, 0u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});

    test(__LINE__, {"--i", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--j", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--k", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--l", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--m", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--n", "3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});

    test(__LINE__, {"--i", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--j", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--k", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--l", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--m", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"--n", "-3.45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});


    test(__LINE__, {"i", "3.."}, test_active{3,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"j", "3.."}, test_active{1,3,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"k", "3.."}, test_active{1,1,3, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"l", "3.."}, test_active{1,1,1, 3u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"m", "3.."}, test_active{1,1,1, 1u, 3u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"n", "3.."}, test_active{1,1,1, 1u, 1u, 3u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});

    test(__LINE__, {"u", "3..45"}, test_active{1,1,1, 1u, 1u, 1u,  3.00f, 1.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"v", "3..45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 3.00f, 1.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"w", "3..45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 3.00f,  1.00, 1.00, 1.00});
    test(__LINE__, {"x", "3..45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  3.00, 1.00, 1.00});
    test(__LINE__, {"y", "3..45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 3.00, 1.00});
    test(__LINE__, {"z", "3..45"}, test_active{1,1,1, 1u, 1u, 1u,  1.00f, 1.00f, 1.00f,  1.00, 1.00, 3.00});


    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
