#include <utility>
#include <string>
#include <memory>
#include <stdexcept>
#include "uRayTracer1D/source.hpp"

using namespace URayTracer1D;

class Source::SourceImpl
{
public:
    double mDepthInMeters{0};
    bool mHasDepth{false};
};

/// Constructor
Source::Source() :
    pImpl(std::make_unique<SourceImpl> ())
{
}

/// Copy constructor
Source::Source(const Source &source)
{
    *this = source;
}

/// Move constructor
Source::Source(Source &&source) noexcept
{
    *this = std::move(source);
}

/// Copy assignment
Source& Source::operator=(const Source &source)
{
    if (&source == this){return *this;}
    pImpl = std::make_unique<SourceImpl> (*source.pImpl);
    return *this;
}

/// Move assignment
Source& Source::operator=(Source &&source) noexcept
{
    if (&source == this){return *this;}
    pImpl = std::move(source.pImpl);
    return *this;
}


/// Set the source depth
void Source::setDepth(const double depthInMeters)
{
    constexpr double minimumDepth{-8600};   // Top of Mt. Everest
    constexpr double maximumDepth{ 900000}; // Deepest earthquakes are like 800 km  
    if (depthInMeters < minimumDepth)
    {
        throw std::invalid_argument("Source depth of "
                                  + std::to_string(depthInMeters)
                                  + " must be greater than "
                                  + std::to_string(minimumDepth));
    }
    if (depthInMeters > maximumDepth)
    {
        throw std::invalid_argument("Source depth of "
                                  + std::to_string(depthInMeters)
                                  + " must be less than "
                                  + std::to_string(maximumDepth));
    }
    pImpl->mDepthInMeters = depthInMeters;
    pImpl->mHasDepth = true;
}

double Source::getDepth() const
{ 
    if (!hasDepth())
    {
        throw std::runtime_error("Depth not set");
    }
    return pImpl->mDepthInMeters;
}

bool Source::hasDepth() const noexcept
{
    return pImpl->mHasDepth;
}

/// Destructor
Source::~Source() = default;
