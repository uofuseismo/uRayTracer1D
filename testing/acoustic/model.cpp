#include <utility>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "uRayTracer1D/acoustic/constantLayer.hpp"

TEST_CASE("URayTracer1D::Acoustic", "[Layer]")
{
    constexpr double thickness{1223};
    constexpr double velocity{4352};
    URayTracer1D::Acoustic::ConstantLayer layer;
    REQUIRE(!layer.hasVelocity());
    REQUIRE(!layer.hasThickness());
    layer = URayTracer1D::Acoustic::ConstantLayer {std::pair {thickness, velocity}};
    REQUIRE_THAT(layer.getThickness(),
                 Catch::Matchers::WithinAbs(thickness, 1.e-14));
    REQUIRE_THAT(layer.getVelocity(),
                 Catch::Matchers::WithinAbs(velocity, 1.e-14));
    SECTION("Copy")
    {   
        //NOLINTBEGIN
        URayTracer1D::Acoustic::ConstantLayer copy{layer};
        //NOLINTEND
        REQUIRE_THAT(copy.getThickness(),
                     Catch::Matchers::WithinAbs(thickness, 1.e-14));
        REQUIRE_THAT(copy.getVelocity(),
                     Catch::Matchers::WithinAbs(velocity, 1.e-14));

    }   
}

