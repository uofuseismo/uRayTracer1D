#include <utility>
#include <string>
#include <memory>
#include <stdexcept>
#include "uRayTracer1D/receiver.hpp"

using namespace URayTracer1D;

class Receiver::ReceiverImpl
{
public:
    double mElevationInMeters{0};
    double mOffsetInMeters{0};
    bool mHasElevation{false};
    bool mHasOffset{false};
};

/// Constructor
Receiver::Receiver() :
    pImpl(std::make_unique<ReceiverImpl> ())
{
}

/// Copy constructor
Receiver::Receiver(const Receiver &receiver)
{
    *this = receiver;
}

/// Move constructor
Receiver::Receiver(Receiver &&receiver) noexcept
{
    *this = std::move(receiver);
}

/// Copy assignment
Receiver& Receiver::operator=(const Receiver &receiver)
{
    if (&receiver == this){return *this;}
    pImpl = std::make_unique<ReceiverImpl> (*receiver.pImpl);
    return *this;
}

/// Move assignment
Receiver& Receiver::operator=(Receiver &&receiver) noexcept
{
    if (&receiver == this){return *this;}
    pImpl = std::move(receiver.pImpl);
    return *this;
}

/// Set the receiver elevation
void Receiver::setElevation(const double elevationInMeters)
{
    constexpr double minimumElevation{-10000}; // Never dug a hole deeper than this 
    constexpr double maximumElevation{+8600};  // Top of Mt. Everest
    if (elevationInMeters < minimumElevation)
    {
        throw std::invalid_argument("Receiver elevation of "
                                  + std::to_string(elevationInMeters)
                                  + " must be greater than "
                                  + std::to_string(minimumElevation));
    }
    if (elevationInMeters > maximumElevation)
    {   
        throw std::invalid_argument("Receiver elevation of "
                                  + std::to_string(elevationInMeters)
                                  + " must be less than "
                                  + std::to_string(maximumElevation));
    }
    pImpl->mElevationInMeters = elevationInMeters;
    pImpl->mHasElevation = true;
}

double Receiver::getElevation() const
{ 
    if (!hasElevation())
    {
        throw std::runtime_error("Elevation not set");
    }
    return pImpl->mElevationInMeters;
}

bool Receiver::hasElevation() const noexcept
{
    return pImpl->mHasElevation;
}

/// Offset
void Receiver::setOffset(const double offsetInMeters)
{
    if (offsetInMeters < 0)
    {
        throw std::invalid_argument("Offset of " 
                                 + std::to_string(offsetInMeters)
                                 + " must be non-negative");
    }
    pImpl->mOffsetInMeters = offsetInMeters;
    pImpl->mHasOffset = true;
}

double Receiver::getOffset() const
{
    if (!hasOffset())
    {
        throw std::runtime_error("Offset not set");
    }
    return pImpl->mOffsetInMeters;
}

bool Receiver::hasOffset() const noexcept
{
    return pImpl->mHasOffset;
}
/*
/// Set the receiver depth
void Receiver::setDepth(const double depthInMeters)
{
    constexpr double minimumDepth{-8600};   // Top of Mt. Everest
    constexpr double maximumDepth{ 900000}; // Deepest earthquakes are like 800 km  
    if (depthInMeters < minimumDepth)
    {
        throw std::invalid_argument("Receiver depth of "
                                  + std::to_string(depthInMeters)
                                  + " must be greater than "
                                  + std::to_string(minimumDepth));
    }
    if (depthInMeters > maximumDepth)
    {
        throw std::invalid_argument("Receiver depth of "
                                  + std::to_string(depthInMeters)
                                  + " must be less than "
                                  + std::to_string(maximumDepth));
    }
    pImpl->mDepthInMeters = depthInMeters;
    pImpl->mHasDepth = true;
}

double Receiver::getDepth() const
{ 
    if (!hasDepth())
    {
        throw std::runtime_error("Depth not set");
    }
    return pImpl->mDepthInMeters;
}

bool Receiver::hasDepth() const noexcept
{
    return pImpl->mHasDepth;
}
*/

/// Destructor
Receiver::~Receiver() = default;
