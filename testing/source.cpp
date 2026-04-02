#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "uRayTracer1D/source.hpp"

TEST_CASE("URayTracer1D", "[Source]")
{
    constexpr double depth{4823};
    URayTracer1D::Source source;
    REQUIRE(!source.hasDepth());  
    REQUIRE_NOTHROW(source.setDepth(depth));
    REQUIRE_THAT(source.getDepth(), Catch::Matchers::WithinAbs(depth, 1.e-14));

    SECTION("Copy")
    {
        //NOLINTBEGIN
        URayTracer1D::Source copy{source};
        //NOLINTEND
        REQUIRE_THAT(copy.getDepth(),
                     Catch::Matchers::WithinAbs(depth, 1.e-14));
    }
}

