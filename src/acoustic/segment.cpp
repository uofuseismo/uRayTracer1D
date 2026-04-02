#include <optional>
#include <utility>
#include <string>
#include <cmath>
#include <memory>
#include <stdexcept>
#ifndef NDEBUG
#include <cassert>
#endif
#include "uRayTracer1D/acoustic/segment.hpp"

using namespace URayTracer1D::Acoustic;

class Segment::SegmentImpl
{
public:
    void updateLength()
    {
        mLength =-1; 
        if (mHasStartPoint && mHasEndPoint)
        {
            const auto [r0, z0] = mStartPoint;
            const auto [r1, z1] = mEndPoint;
            mLength = std::hypot( r1 - r0, z1 - z0 );
        }
    }
    void updateTravelTime()
    {
        mTravelTime =-1;
        if (mLength >= 0 && mHasSlowness)
        {
            mTravelTime = mSlowness*mLength;
        }
    }
    void update()
    {
        updateLength();
        updateTravelTime();
    }
 
    std::pair<double, double> mStartPoint;
    std::pair<double, double> mEndPoint;
    double mLength{-1};
    double mTravelTime{-1};
    double mSlowness{-1};
    int mLayerIndex{-1};
    bool mHasStartPoint{false};
    bool mHasEndPoint{false};
    bool mHasSlowness{false};
};

/// Constructor
Segment::Segment() :
    pImpl(std::make_unique<SegmentImpl> ())
{
}

/// Constructor
Segment::Segment(const double slowness,
                 const std::pair
                 <
                    std::pair<double, double>,
                    std::pair<double, double>
                 > &startAndEndPoint) :
    pImpl(std::make_unique<SegmentImpl> ()) 
{
    setSlowness(slowness);
    setStartPoint(startAndEndPoint.first);
    setEndPoint(startAndEndPoint.second);
}

Segment::Segment(const double slowness,
                 const std::pair
                 <
                    std::pair<double, double>,
                    std::pair<double, double>
                 > &startAndEndPoint,
                 const int layerIndex) :
    Segment(slowness, startAndEndPoint)
{
    setLayerIndex(layerIndex);
}

/// Copy constructor
Segment::Segment(const Segment &segment) 
{
    *this = segment;
}

/// Move constructor
Segment::Segment(Segment &&segment) noexcept
{
    *this = std::move(segment);
}

/// Copy assignment
Segment& Segment::operator=(const Segment &segment)
{
    if (&segment == this){return *this;}
    pImpl = std::make_unique<SegmentImpl> (*segment.pImpl);
    return *this;
}

/// Move assignent
Segment& Segment::operator=(Segment &&segment) noexcept
{
    if (&segment == this){return *this;}
    pImpl = std::move(segment.pImpl);
    return *this;
}

/// Destructor 
Segment::~Segment() = default;

/// Start point
void Segment::setStartPoint(const std::pair<double, double> &point)
{
    if (point.first < 0)
    {
        throw std::invalid_argument("Start point offset must be non-negative");
    }
    if (point.second < -8600)
    {
        throw std::invalid_argument("Start point depth "
                                  + std::to_string(point.second)
                                  + " must be greater than -8600");
    }
    if (point.second > 6400000)
    {   
        throw std::invalid_argument("Start point depth "
                                  + std::to_string(point.second)
                                  + " must be less than 6400000");
    }
    pImpl->mStartPoint = point;
    pImpl->mHasStartPoint = true;
    pImpl->update();
}

std::pair<double, double> Segment::getStartPoint() const
{
    if (!hasStartPoint())
    {
        throw std::runtime_error("Start point not set");
    }
    return pImpl->mStartPoint;
}

bool Segment::hasStartPoint() const noexcept
{
    return pImpl->mHasStartPoint;
}

/// End point
void Segment::setEndPoint(const std::pair<double, double> &point)
{
    if (point.first < 0)
    {   
        throw std::invalid_argument("End point offset must be non-negative");
    }   
    if (point.second < -8600)
    {   
        throw std::invalid_argument("End point depth "
                                  + std::to_string(point.second)
                                  + " must be greater than -8600");
    }   
    if (point.second > 6400000)
    {
        throw std::invalid_argument("End point depth "
                                  + std::to_string(point.second)
                                  + " must be less than 6400000");
    }   
    pImpl->mEndPoint = point;
    pImpl->mHasEndPoint = true;
    pImpl->update();
}


std::pair<double, double> Segment::getEndPoint() const
{
    if (!hasEndPoint())
    {
        throw std::runtime_error("End point not set");
    }
    return pImpl->mEndPoint;
}

bool Segment::hasEndPoint() const noexcept
{
    return pImpl->mHasEndPoint;
}

/// Slowness
void Segment::setSlowness(const double slowness)
{
    if (slowness <= 0)
    {
        throw std::invalid_argument("Layer slowness must be positive");
    }
    pImpl->mSlowness = slowness;
    pImpl->mHasSlowness = true;
    pImpl->update(); 
}

double Segment::getSlowness() const
{
    if (!hasSlowness()){throw std::runtime_error("Slowness not set");}
    return pImpl->mSlowness;
}

bool Segment::hasSlowness() const noexcept
{
    return pImpl->mSlowness > 0;
}

/// Layer index
void Segment::setLayerIndex(const int layerIndex)
{
    if (layerIndex <= 0)
    {
        throw std::invalid_argument("Layer index " + std::to_string(layerIndex)
                                  + " must be non-negative");
    }
    pImpl->mLayerIndex = layerIndex;
}

std::optional<int> Segment::getLayerIndex() const noexcept
{
    return pImpl->mLayerIndex >= 0 ?
           std::make_optional<int> (pImpl->mLayerIndex) : std::nullopt;
}

/// Length
double Segment::length() const
{
    if (pImpl->mLength < 0)
    {
        if (!hasStartPoint()){throw std::runtime_error("Start point not set");}
        if (!hasEndPoint()){throw std::runtime_error("End poitn not set");}
        pImpl->update();
#ifndef NDEBUG
        assert(pImpl->mLength >= 0);
#endif
    }
    return pImpl->mLength;
}

double Segment::travelTime() const
{
    if (pImpl->mTravelTime < 0)
    {
        if (!hasSlowness()){throw std::runtime_error("Slowness not set");}
        if (!hasStartPoint()){throw std::runtime_error("Start point not set");}
        if (!hasEndPoint()){throw std::runtime_error("End poitn not set");}
        pImpl->update();
#ifndef NDEBUG
        assert(pImpl->mTravelTime >= 0);
#endif
    }
    return pImpl->mTravelTime;
}

/// Reverse segment
void Segment::reverse()
{
    if (!hasStartPoint()){throw std::runtime_error("Start point not set");}
    if (!hasEndPoint()){throw std::runtime_error("End point not set");}
    std::swap(pImpl->mStartPoint, pImpl->mEndPoint);
    pImpl->update();
}
