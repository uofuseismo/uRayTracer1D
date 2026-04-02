#include <utility>
#include <cmath>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "uRayTracer1D/acoustic/segment.hpp"
#include "uRayTracer1D/acoustic/constantLayer.hpp"

// NOLINTBEGIN
TEST_CASE("UrayTracer1D::Segment", "[Segment]")
{
    constexpr std::pair<double, double> startPoint{10, 200};
    constexpr std::pair<double, double> endPoint{50, 600};
    constexpr double velocity{4000};
    constexpr double slowness{1./velocity};
    const double length
        = std::sqrt( std::pow(endPoint.first  - startPoint.first,  2)
                   + std::pow(endPoint.second - startPoint.second, 2) );
    const double travelTime = slowness*length;
    const int layerIndex{4};
    URayTracer1D::Acoustic::Segment segment{slowness,
                                            std::pair {startPoint, endPoint},
                                            layerIndex};
    REQUIRE_THAT(segment.getSlowness(),
                 Catch::Matchers::WithinAbs(slowness, 1.e-14));
    REQUIRE_THAT(segment.getStartPoint().first,
                 Catch::Matchers::WithinAbs(startPoint.first, 1.e-14));
    REQUIRE_THAT(segment.getStartPoint().second,
                 Catch::Matchers::WithinAbs(startPoint.second, 1.e-14));
    REQUIRE_THAT(segment.getEndPoint().first,
                 Catch::Matchers::WithinAbs(endPoint.first, 1.e-14));
    REQUIRE_THAT(segment.getEndPoint().second,
                 Catch::Matchers::WithinAbs(endPoint.second, 1.e-14));
    REQUIRE(*segment.getLayerIndex() == layerIndex);
    REQUIRE_THAT(segment.length(),
                 Catch::Matchers::WithinAbs(length, 1.e-14));
    REQUIRE_THAT(segment.travelTime(),
                 Catch::Matchers::WithinAbs(travelTime, 1.e-14));
    SECTION("Copy")
    {
        auto copy = URayTracer1D::Acoustic::Segment {segment};
        REQUIRE_THAT(copy.getSlowness(),
                     Catch::Matchers::WithinAbs(slowness, 1.e-14));
        REQUIRE_THAT(copy.getStartPoint().first,
                     Catch::Matchers::WithinAbs(startPoint.first, 1.e-14));
        REQUIRE_THAT(copy.getStartPoint().second,
                     Catch::Matchers::WithinAbs(startPoint.second, 1.e-14));
        REQUIRE_THAT(copy.getEndPoint().first,
                     Catch::Matchers::WithinAbs(endPoint.first, 1.e-14));
        REQUIRE_THAT(copy.getEndPoint().second,
                     Catch::Matchers::WithinAbs(endPoint.second, 1.e-14));
        REQUIRE(*copy.getLayerIndex() == layerIndex);
        REQUIRE_THAT(copy.length(),
                     Catch::Matchers::WithinAbs(length, 1.e-14));
        REQUIRE_THAT(copy.travelTime(),
                     Catch::Matchers::WithinAbs(travelTime, 1.e-14));
    }
    SECTION("Reverse")
    {
        segment.reverse();
        REQUIRE_THAT(segment.getStartPoint().first,
                     Catch::Matchers::WithinAbs(endPoint.first, 1.e-14));
        REQUIRE_THAT(segment.getStartPoint().second,
                     Catch::Matchers::WithinAbs(endPoint.second, 1.e-14));
        REQUIRE_THAT(segment.getEndPoint().first,
                     Catch::Matchers::WithinAbs(startPoint.first, 1.e-14));
        REQUIRE_THAT(segment.getEndPoint().second,
                     Catch::Matchers::WithinAbs(startPoint.second, 1.e-14));
        REQUIRE_THAT(segment.length(),
                     Catch::Matchers::WithinAbs(length, 1.e-14));
        REQUIRE_THAT(segment.travelTime(),
                     Catch::Matchers::WithinAbs(travelTime, 1.e-14));
    }
}
// NOLINTEND

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

