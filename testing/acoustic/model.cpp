#include <utility>
#include <limits>
#include <cmath>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "uRayTracer1D/acoustic/segment.hpp"
#include "uRayTracer1D/acoustic/path.hpp"
#include "uRayTracer1D/acoustic/constantLayer.hpp"

namespace
{
double computeDistance(const std::pair<double, double> &startPoint,
                       const std::pair<double, double> &endPoint)
{
    const double length
        = std::sqrt( std::pow(endPoint.first  - startPoint.first,  2)
                   + std::pow(endPoint.second - startPoint.second, 2) );
    return length;
}
double computeTravelTime(const std::pair<double, double> &startPoint,
                         const std::pair<double, double> &endPoint,
                         const double slowness)
{
    return ::computeDistance(startPoint, endPoint)*slowness;
}
bool matches(const URayTracer1D::Acoustic::Segment &lhs,
             const URayTracer1D::Acoustic::Segment &rhs)
{
    constexpr double tolerance{std::numeric_limits<double>::epsilon()*100};
    if (std::abs(lhs.travelTime() - rhs.travelTime()) > tolerance)
    {
        return false;
    }
    if (std::abs(lhs.getStartPoint().first
               - rhs.getStartPoint().first) > tolerance){return false;}
    if (std::abs(lhs.getStartPoint().second
               - rhs.getStartPoint().second) > tolerance){return false;}
    if (std::abs(lhs.getEndPoint().first
               - rhs.getEndPoint().first) > tolerance){return false;}
    if (std::abs(lhs.getEndPoint().second 
               - rhs.getEndPoint().second) > tolerance){return false;}
    if (std::abs(lhs.length() - rhs.length()) > tolerance){return false;}
    if (lhs.getLayerIndex() != rhs.getLayerIndex()){return false;}
    if (lhs.getLayerIndex() && rhs.getLayerIndex())
    {
        if (*lhs.getLayerIndex() != *rhs.getLayerIndex()){return false;}
    }
    return true;
}
bool matchesReversed(const URayTracer1D::Acoustic::Segment &lhs,
                     const URayTracer1D::Acoustic::Segment &rhs)
{
    constexpr double tolerance{std::numeric_limits<double>::epsilon()*100};
    if (std::abs(lhs.travelTime() - rhs.travelTime()) > tolerance)
    {
        return false;
    }
    if (std::abs(lhs.getStartPoint().first
               - rhs.getEndPoint().first) > tolerance){return false;}
    if (std::abs(lhs.getStartPoint().second
               - rhs.getEndPoint().second) > tolerance){return false;}
    if (std::abs(lhs.getEndPoint().first
               - rhs.getStartPoint().first) > tolerance){return false;}
    if (std::abs(lhs.getEndPoint().second 
               - rhs.getStartPoint().second) > tolerance){return false;}
    if (std::abs(lhs.length() - rhs.length()) > tolerance){return false;}
    if (lhs.getLayerIndex() != rhs.getLayerIndex()){return false;}
    if (lhs.getLayerIndex() && rhs.getLayerIndex())
    {   
        if (*lhs.getLayerIndex() != *rhs.getLayerIndex()){return false;}
    }   
    return true;
}
}

// NOLINTBEGIN
TEST_CASE("UrayTracer1D::Segment", "[Segment]")
{
    constexpr std::pair<double, double> startPoint{10, 200};
    constexpr std::pair<double, double> endPoint{50, 600};
    constexpr double velocity{4000};
    constexpr double slowness{1./velocity};
    const double length = ::computeDistance(startPoint, endPoint);
    const double travelTime
        = ::computeTravelTime(startPoint, endPoint, slowness);
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

TEST_CASE("URayTracer1D::Acoustic", "[Path]")
{
    SECTION("Direct Ray In First Layer")
    {
        constexpr std::pair<double, double> startPoint{0 + 50, 0 + 50};
        constexpr std::pair<double, double> endPoint{0 + 100,  0 + 100};
        constexpr double velocity{4000};
        constexpr double slowness{1./velocity};
        const double length = ::computeDistance(startPoint, endPoint);
        const double travelTime = ::computeTravelTime(startPoint, endPoint, slowness);
        const int layerIndex{0};
        URayTracer1D::Acoustic::Segment segment{slowness,
                                                std::pair {startPoint, endPoint},
                                                layerIndex};

        URayTracer1D::Acoustic::Path path;
        REQUIRE(path.empty());
        REQUIRE(!path.isOpen());
        path.open();
        REQUIRE(path.isOpen());
        path.append(segment);
        path.close();
        REQUIRE(!path.isOpen());
        REQUIRE(!path.empty());
        REQUIRE_THAT(path.length(),
                     Catch::Matchers::WithinAbs(length, 1.e-14));
        REQUIRE_THAT(path.travelTime(),
                     Catch::Matchers::WithinAbs(travelTime, 1.e-14));
        REQUIRE(::matches(path.at(0), segment));

        // Reverse it
        path.reverse();
        REQUIRE(::matchesReversed(path.at(0), segment));
    }
    SECTION("Reflection In First Layer")
    {   
        constexpr std::pair<double, double> startPoint1{0 + 50, 0 + 50};
        constexpr std::pair<double, double> endPoint1{0 + 100,  0 + 100};
        constexpr std::pair<double, double> startPoint2{0 + 100, 0 + 100};
        constexpr std::pair<double, double> endPoint2{0 + 100 + 50,  0 + 50};

        constexpr double velocity{4000};
        constexpr double slowness{1./velocity};
        const double length = ::computeDistance(startPoint1, endPoint1)
                            + ::computeDistance(startPoint2, endPoint2);
        const double travelTime
            = ::computeTravelTime(startPoint1, endPoint1, slowness)
            + ::computeTravelTime(startPoint2, endPoint2, slowness);
        const int layerIndex{0};
        URayTracer1D::Acoustic::Segment segment1{slowness,
                                              std::pair {startPoint1, endPoint1},
                                              layerIndex};
        URayTracer1D::Acoustic::Segment segment2{slowness,
                                               std::pair {startPoint2, endPoint2},
                                               layerIndex};

        URayTracer1D::Acoustic::Path path;
        REQUIRE(path.empty());
        path.open();
        REQUIRE_NOTHROW(path.append(segment1));
        REQUIRE_NOTHROW(path.append(segment2));
        REQUIRE_THROWS(path.append(segment2));
        path.close();
        REQUIRE(!path.empty());
        REQUIRE_THAT(path.length(),
                     Catch::Matchers::WithinAbs(length, 1.e-14));
        REQUIRE_THAT(path.travelTime(),
                     Catch::Matchers::WithinAbs(travelTime, 1.e-14));
        REQUIRE(::matches(path.at(0), segment1));
        REQUIRE(::matches(path.at(1), segment2));

        // Reverse it
        path.reverse();
        REQUIRE(::matchesReversed(path.at(0), segment2));
        REQUIRE(::matchesReversed(path.at(1), segment1));
    }   
    SECTION("Reflection In First Layer From Vector")
    {   
        constexpr std::pair<double, double> startPoint1{0 + 50, 0 + 50};
        constexpr std::pair<double, double> endPoint1{0 + 100,  0 + 100};
        constexpr std::pair<double, double> startPoint2{0 + 100, 0 + 100};
        constexpr std::pair<double, double> endPoint2{0 + 100 + 50,  0 + 50};

        constexpr double velocity{4000};
        constexpr double slowness{1./velocity};
        const double length = ::computeDistance(startPoint1, endPoint1)
                            + ::computeDistance(startPoint2, endPoint2);
        const double travelTime
            = ::computeTravelTime(startPoint1, endPoint1, slowness)
            + ::computeTravelTime(startPoint2, endPoint2, slowness);
        const int layerIndex{0};
        URayTracer1D::Acoustic::Segment segment1{slowness,
                                              std::pair {startPoint1, endPoint1},
                                              layerIndex};
        URayTracer1D::Acoustic::Segment segment2{slowness,
                                               std::pair {startPoint2, endPoint2},
                                               layerIndex};
        std::vector<URayTracer1D::Acoustic::Segment> segments{segment1, segment2};

        URayTracer1D::Acoustic::Path path;
        path.set(segments);
        REQUIRE(!path.isOpen());
        REQUIRE_THAT(path.length(),
                     Catch::Matchers::WithinAbs(length, 1.e-14));
        REQUIRE_THAT(path.travelTime(),
                     Catch::Matchers::WithinAbs(travelTime, 1.e-14));
        REQUIRE(::matches(path.at(0), segment1));
        REQUIRE(::matches(path.at(1), segment2));

        // Poison pill
        segments.push_back(segment2);
        REQUIRE_THROWS(path.set(segments));
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

