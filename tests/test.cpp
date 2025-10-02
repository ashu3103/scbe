#include "cases/cases.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstdlib>
#include <string>

using namespace scbe;

TEST_CASE("Tests") {
    auto target = GENERATE("x86_64", "aarch64");
    auto debug = GENERATE(0, 1);
    CAPTURE(target, debug);

    Target::TargetSpecification spec(std::string(target) + "-pc-linux-gnu");

    REQUIRE(case0(spec, debug));
    REQUIRE(case1(spec, debug));
    REQUIRE(case2(spec, debug));
    REQUIRE(case3(spec, debug));
    REQUIRE(case4(spec, debug));
    REQUIRE(case5(spec, debug));
    REQUIRE(case6(spec, debug));
    REQUIRE(case7(spec, debug));
    REQUIRE(case8(spec, debug));
    REQUIRE(case9(spec, debug));
    REQUIRE(case10(spec, debug));
    REQUIRE(case11(spec, debug));
    REQUIRE(case12(spec, debug));
    REQUIRE(case13(spec, debug));
    REQUIRE(case14(spec, debug));
}