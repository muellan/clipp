/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 * Tests: "missing" detection for required parameters
 *    'if_missing' error handler should be called:
 *         - for required parameters only
 *         - if required parameter is not matched but reachable
 *           in terms of the previously matched parameters
 *         - with the correct argument:
 *           the index of the last command line argument that should
 *           have been a match
 *
 *****************************************************************************/

#include "testing.h"


//-------------------------------------------------------------------
template<class T>
struct assignments {
    assignments() = default;
    explicit
    assignments(T a_, T b_, T c_, T d_, T e_, T f_, T g_, T h_, T i_,
                T j_, T k_, T l_, T m_, T n_, T o_, T p_, T q_, T r_)
    :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, g{g_}, h{h_}, i{i_},
        j{j_}, k{k_}, l{l_}, m{m_}, n{n_}, o{o_}, p{p_}, q{q_}, r{r_}
    {}

    T a = T(0), b = T(0), c = T(0), d = T(0), e = T(0), f = T(0);
    T g = T(0), h = T(0), i = T(0), j = T(0), k = T(0), l = T(0);
    T m = T(0), n = T(0), o = T(0), p = T(0), q = T(0), r = T(0);

    friend bool operator == (const assignments& x, const assignments& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f && x.g == y.g && x.h == y.h &&
                x.i == y.i && x.j == y.j && x.k == y.k && x.l == y.l &&
                x.m == y.m && x.n == y.n && x.o == y.o && x.p == y.p &&
                x.q == y.q && x.r == y.r);
    }
};

using active = assignments<bool>;
using counts = assignments<int>;


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches,
          const counts& misses,
          counts blocked,
          const counts& conflicts,
          const counts& repeats)
{
    using namespace clipp;

    //blocked @ index; compensated for arg index offset
    --blocked.a; --blocked.b; --blocked.c; --blocked.d; --blocked.e;
    --blocked.f; --blocked.g; --blocked.h; --blocked.i; --blocked.j;
    --blocked.k; --blocked.l; --blocked.m; --blocked.n; --blocked.o;
    --blocked.p; --blocked.q; --blocked.r;

    active a;
    counts m; //missing
    counts c; //conflicts
    counts r; //repeats
    counts b; //blocked

    auto cli = (
        option("a").set(a.a).if_blocked([&](int i){b.a=i;}).if_missing([&]{++m.a;}).if_repeated([&]{++r.a;}).if_conflicted(set(c.a,1)),
        required("b").set(a.b).if_blocked([&](int i){b.b=i;}).if_missing([&]{++m.b;}).if_repeated([&]{++r.b;}).if_conflicted(set(c.b,1)),
        repeatable(
            (   command("c").set(a.c).if_blocked([&](int i){b.c=i;}).if_missing([&]{++m.c;}).if_repeated([&]{++r.c;}).if_conflicted(set(c.c,1)),
                required("d").set(a.d).if_blocked([&](int i){b.d=i;}).if_missing([&]{++m.d;}).if_repeated([&]{++r.d;}).if_conflicted(set(c.d,1)),
                option("e").set(a.e).if_blocked([&](int i){b.e=i;}).if_missing([&]{++m.e;}).if_repeated([&]{++r.e;}).if_conflicted(set(c.e,1))
            ) |
            (   command("f").set(a.f).if_blocked([&](int i){b.f=i;}).if_missing([&]{++m.f;}).if_repeated([&]{++r.f;}).if_conflicted(set(c.f,1)),
                required("g").set(a.g).if_blocked([&](int i){b.g=i;}).if_missing([&]{++m.g;}).if_repeated([&]{++r.g;}).if_conflicted(set(c.g,1)),
                required("h").set(a.h).if_blocked([&](int i){b.h=i;}).if_missing([&]{++m.h;}).if_repeated([&]{++r.h;}).if_conflicted(set(c.h,1))
            ) |
            (   required("i").set(a.i).if_blocked([&](int i){b.i=i;}).if_missing([&]{++m.i;}).if_repeated([&]{++r.i;}).if_conflicted(set(c.i,1)),
                required("j").set(a.j).if_blocked([&](int i){b.j=i;}).if_missing([&]{++m.j;}).if_repeated([&]{++r.j;}).if_conflicted(set(c.j,1))
            ) |
            option("k").set(a.k).if_blocked([&](int i){b.k=i;}).if_missing([&]{++m.k;}).if_repeated([&]{++r.k;}).if_conflicted(set(c.k,1)) |
            required("l").set(a.l).if_blocked([&](int i){b.l=i;}).if_missing([&]{++m.l;}).if_repeated([&]{++r.l;}).if_conflicted(set(c.l,1))
        ),
        option("m").set(a.m).if_blocked([&](int i){b.m=i;}).if_missing([&]{++m.m;}).if_repeated([&]{++r.m;}).if_conflicted(set(c.m,1)),
        required("n").set(a.n).if_blocked([&](int i){b.n=i;}).if_missing([&]{++m.n;}).if_repeated([&]{++r.n;}).if_conflicted(set(c.n,1)),
        repeatable(
            required("o").set(a.o).if_blocked([&](int i){b.o=i;}).if_missing([&]{++m.o;}).if_repeated([&]{++r.o;}).if_conflicted(set(c.o,1)) |
            option("p").set(a.p).if_blocked([&](int i){b.p=i;}).if_missing([&]{++m.p;}).if_repeated([&]{++r.p;}).if_conflicted(set(c.p,1))
        ),
        required("q").set(a.q).if_blocked([&](int i){b.q=i;}).if_missing([&]{++m.q;}).if_repeated([&]{++r.q;}).if_conflicted(set(c.q,1)),
        option("r").set(a.r).if_blocked([&](int i){b.r=i;}).if_missing([&]{++m.r;}).if_repeated([&]{++r.r;}).if_conflicted(set(c.r,1))
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
        [&] { a = active{}; m = counts{}; c = counts{}; r = counts{};
              b = counts{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
        },
        [&] { return a == matches && m == misses && b == blocked &&
                     c == conflicts && r == repeats; }
    );
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"b", "n", "q"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*matched*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"a"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{1,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"b"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"c"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 1,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,1,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"c", "e"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 1,0,1, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,1,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"c", "d"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 1,1,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"c", "e", "d"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 1,1,1, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"c", "e", "d", "b"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,1, 1,1,1, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"f"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 1,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,1,1, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"f", "g"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 1,1,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,1, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"f", "g", "h"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 1,1,1, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"i"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 1,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,1, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"i", "j"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 1,1, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"k"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 1, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"l"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 1, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"m"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 1,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"n"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"o"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"p"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,1, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"q"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 1,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 0,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"r"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,1}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"o", "a", "o"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{1,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,0, 0,0}  /*repeats*/);

        test(__LINE__, {"o", "a", "p"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{1,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,1, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"o", "p"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,1, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"o", "a", "o", "b", "p", "r", "o"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{1,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,1, 0,1}, /*matched*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 2,0, 0,0}  /*repeats*/);

        test(__LINE__, {"o", "o", "p", "o"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 1,1, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 2,0, 0,0}  /*repeats*/);

        test(__LINE__, {"c", "c"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 1,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,2,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 1,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"f", "f"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 1,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,2,2, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 1,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"f", "g", "f", "g", "h"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 1,1,1, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,1, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 1,1,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"i", "i"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 1,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,2, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 1,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"l", "l"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 1, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 1, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"d"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,1,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"g"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,1,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

        test(__LINE__, {"h"},
                 //  a b  c d e  f g h  i j  k  l  m n  o p  q r
              active{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*matched*/
              counts{0,1, 0,0,0, 0,0,0, 0,0, 0, 0, 0,1, 0,0, 1,0}, /*misses*/
              counts{0,0, 0,0,0, 0,0,1, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*blocked*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}, /*conflicts*/
              counts{0,0, 0,0,0, 0,0,0, 0,0, 0, 0, 0,0, 0,0, 0,0}  /*repeats*/);

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
