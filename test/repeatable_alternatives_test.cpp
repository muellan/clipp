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
    active(bool a_, bool b_,
           std::initializer_list<int> i_, std::initializer_list<int> j_)
    :
        a{a_}, b{b_}, i{i_}, j{j_}
    {}
    bool a = false, b = false;
    std::vector<int> i, j;

    friend bool operator == (const active& x, const active& y) noexcept {
        return x.a == y.a && x.b == y.b &&
			(
				(x.i.size() == 0 && y.i.size() == 0) ||
				(x.i.size() > 0 && y.i.size() > 0 && x.i.size() == y.i.size() &&
					std::equal(x.i.begin(), x.i.end(), y.i.begin()))
			) &&
			(
				(x.j.size() == 0 && y.j.size() == 0) ||
				(x.j.size() > 0 && y.j.size() > 0 && x.j.size() == y.j.size() &&
					std::equal(x.j.begin(), x.j.end(), y.j.begin()))
			);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;

    active m;

    auto cli = repeatable(
        ( option("a").set(m.a) & value("i",m.i) ) |
        ( option("b").set(m.b) & value("j",m.j) )
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

        test(__LINE__, {"a"}, active{1,0, {}, {} });
        test(__LINE__, {"b"}, active{0,1, {}, {} });

        test(__LINE__, {"a", "12"}, active{1,0, {12}, {} });
        test(__LINE__, {"b", "34"}, active{0,1, {}, {34} });

        test(__LINE__, {"a", "12", "b", "34"}, active{1,1, {12}, {34} });

        test(__LINE__, {"a", "12", "a", "34"}, active{1,0, {12,34}, {} });

        test(__LINE__, {"b", "12", "b", "34"}, active{0,1, {}, {12,34} });

        test(__LINE__, {"a", "1", "b", "2", "a", "3"}, active{1,1, {1,3}, {2} });

        test(__LINE__, {"a", "1", "b", "2", "b", "4", "a", "3"}, active{1,1, {1,3}, {2,4} });

        test(__LINE__, {"a", "b", "2", "b", "a", "3"}, active{1,1, {3}, {2} });

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
