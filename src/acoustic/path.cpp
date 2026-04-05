#include <cstddef>
#include <cmath>
#include <limits>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>
#include <stdexcept>
#ifndef NDEBUG
#include <cassert>
#endif
#include "uRayTracer1D/acoustic/path.hpp"
#include "uRayTracer1D/acoustic/segment.hpp"

using namespace URayTracer1D::Acoustic;

namespace
{
/// Require valid segments
void checkSegment(const Segment &segment)
{
    if (!segment.hasStartPoint())
    {
        throw std::invalid_argument("Segment start point not set");
    }
    if (!segment.hasEndPoint())
    {
        throw std::invalid_argument("Segment end point not set");
    }
    if (!segment.hasSlowness())
    {
        throw std::invalid_argument("Segment slowness not set");
    }
#ifndef NDEBUG
    assert(segment.length() >= 0);
    assert(segment.travelTime() >= 0);
#endif
}

/// Distance between start ane end point 
double computeLength(const std::pair<double, double> &startPoint,
                     const std::pair<double, double> &endPoint)
{
    const auto dx = endPoint.first  - startPoint.first;
    const auto dz = endPoint.second - startPoint.second;
    return std::hypot(dx, dz);
}

}

class Path::PathImpl
{
public:
    void reverse()
    {
        if (mSegments.empty()){return;}
        std::reverse(mSegments.begin(), mSegments.end());
        for (auto &segment : mSegments)
        {
            segment.reverse();
        }
#ifndef NDEBUG
        constexpr double tolerance{1.e-8};
        for (int i = 0; i < static_cast<int> (mSegments.size()) - 1; ++i)
        {
            const auto point0 = mSegments[i].getEndPoint();
            const auto point1 = mSegments[i + 1].getStartPoint();
            assert(std::abs(point0.first  - point1.first) < tolerance);
            assert(std::abs(point0.second - point1.second) < tolerance);
        }
#endif
    }
    void integrate()
    {
        if (mIntegrated){return;}
        double length{0};
        double travelTime{0};
        for (const auto &segment : mSegments)
        {
            travelTime = travelTime + segment.travelTime();
            length = length + segment.length();
        }
        mTravelTime = travelTime;
        mLength = length;
        mIntegrated = true;
    }

    std::vector<Segment> mSegments;
    std::vector<Segment> mSegmentsConstruction;
    double mLength{0};
    double mTravelTime{0};
    bool mOpened{false};
    bool mIntegrated{false};
};

/// Constructor
Path::Path() :
    pImpl(std::make_unique<PathImpl> ())
{
}

/// Copy constructor
Path::Path(const Path &path)
{
    *this = path;
}

/// Move constructor
Path::Path(Path &&path) noexcept
{
    *this = std::move(path);
}

/// Copy assignment
Path& Path::operator=(const Path &path)
{
    if (&path == this){return *this;}
    pImpl = std::make_unique<PathImpl> (*path.pImpl);
    return *this;
}

/// Move assignment
Path& Path::operator=(Path &&path) noexcept
{
    if (&path == this){return *this;}
    pImpl = std::move(path.pImpl);
    return *this;
}
 
/// Destructor
Path::~Path() = default;

/// Reset class
void Path::clear() noexcept
{
    pImpl = std::make_unique<PathImpl> ();
}

/// Open the ray path
void Path::open() noexcept
{
    constexpr int DEFAULT_INITIAL_SIZE{128};
    pImpl->mSegmentsConstruction.clear();
    pImpl->mSegmentsConstruction.reserve(
        std::max(DEFAULT_INITIAL_SIZE,
                 static_cast<int> (pImpl->mSegments.size())));
    pImpl->mOpened = true;
}

bool Path::isOpen() const noexcept
{
    return pImpl->mOpened;
}

/// Append to the ray path
void Path::append(const Segment &segment)
{
    auto temporarySegment = segment;
    append(std::move(temporarySegment));
}

void Path::append(Segment &&segment)
{
    ::checkSegment(segment);
    if (pImpl->mSegmentsConstruction.empty())
    {
        pImpl->mSegmentsConstruction.push_back(std::move(segment));
    }
    else
    {
        const auto point0 = pImpl->mSegmentsConstruction.back().getEndPoint();
        const auto point1 = segment.getStartPoint();
        auto distance = ::computeLength(point0, point1);
        constexpr double tolerance{100*std::numeric_limits<double>::epsilon()};
        if (distance > tolerance)
        {
            throw std::invalid_argument(
               "Segment does not start at last segments's end point"); 
        }
        pImpl->mSegmentsConstruction.push_back(std::move(segment));
    }
}

/// Close the ray path
void Path::close()
{
    if (!isOpen()){throw std::runtime_error("Path not open");}
    // Do some tap dancing and release memory
    pImpl->mSegments = std::move(pImpl->mSegmentsConstruction);
    std::vector<Segment> nullSegment{};
    pImpl->mSegmentsConstruction = std::move(nullSegment);
    pImpl->mOpened = false;
    pImpl->mIntegrated = false;
    // Sum along the path
    pImpl->integrate();
}

/// Set ray path from a vector
void Path::set(const std::vector<Segment> &segments)
{
    auto temporarySegments = segments;
    set(std::move(temporarySegments));
}

void Path::set(std::vector<Segment> &&segments)
{
    for (int i = 0; i < static_cast<int> (segments.size()); ++i)
    {
        ::checkSegment(segments[i]); // Throws
        if (i > 0)
        {
            const auto point0 = segments[i - 1].getEndPoint();
            const auto point1 = segments[i].getStartPoint();
            auto distance = ::computeLength(point0, point1);
            constexpr double
                tolerance{100*std::numeric_limits<double>::epsilon()};
            if (distance > tolerance)
            {
                throw std::invalid_argument("Segment " + std::to_string(i)
                                          + " does no start at last segments "
                                          + std::to_string(i - 1)
                                          + " end point");
            }
        }
    }
    pImpl->mSegments = std::move(segments);
    std::vector<Segment> nullSegment{};
    pImpl->mSegmentsConstruction = std::move(nullSegment);
    pImpl->mOpened = false;             
    pImpl->mIntegrated = false;
    // Sum along the path
    pImpl->integrate();
}


/// Number of segments
int Path::size() const noexcept
{
    return static_cast<int> (pImpl->mSegments.size());
}

bool Path::empty() const noexcept
{
    return pImpl->mSegments.empty();
}

/// Travel time
double Path::travelTime() const
{
    pImpl->integrate();
    return pImpl->mTravelTime;
}

/// Length
double Path::length() const
{
    pImpl->integrate();
    return pImpl->mLength;
}

/// Reverse a ray path
void Path::reverse()
{
    if (isOpen()){throw std::runtime_error("Construction is open");}
    pImpl->reverse();
}

/// Iterators
Path::iterator Path::begin()
{
    return pImpl->mSegments.begin();
}

Path::const_iterator Path::begin() const noexcept
{
    return pImpl->mSegments.begin();
}

Path::const_iterator Path::cbegin() const noexcept
{
    return pImpl->mSegments.cbegin();
}

Path::iterator Path::end()
{
    return pImpl->mSegments.end();
}

Path::const_iterator Path::end() const noexcept
{
    return pImpl->mSegments.cend();
}

Path::const_iterator Path::cend() const noexcept
{
    return pImpl->mSegments.cend();
}

Segment& Path::at(const size_t index)
{
    return pImpl->mSegments.at(index);
}

const Segment& Path::at(const size_t index) const
{
    return pImpl->mSegments.at(index);
}

