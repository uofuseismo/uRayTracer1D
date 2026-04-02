#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "uRayTracer1D/receiver.hpp"

TEST_CASE("URayTracer1D", "[Receiver]")
{
    constexpr double elevation{1223};
    constexpr double offset{130052};
    URayTracer1D::Receiver receiver;
    REQUIRE(!receiver.hasOffset());
    REQUIRE(!receiver.hasElevation());
    REQUIRE_NOTHROW(receiver.setElevation(elevation));
    REQUIRE_THAT(receiver.getElevation(),
                 Catch::Matchers::WithinAbs(elevation, 1.e-14));
    REQUIRE_NOTHROW(receiver.setOffset(offset));
    REQUIRE_THAT(receiver.getOffset(),
                 Catch::Matchers::WithinAbs(offset, 1.e-14));

    SECTION("Copy")
    {
        //NOLINTBEGIN
        URayTracer1D::Receiver copy{receiver};
        //NOLINTEND
        REQUIRE_THAT(copy.getElevation(),
                     Catch::Matchers::WithinAbs(elevation, 1.e-14));
        REQUIRE_THAT(copy.getOffset(),
                     Catch::Matchers::WithinAbs(offset, 1.e-14));

    }
}

