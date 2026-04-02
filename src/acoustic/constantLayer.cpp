#include <utility>
#include <stdexcept>
#include <memory>
#include <string>
#include "uRayTracer1D/acoustic/constantLayer.hpp"

using namespace URayTracer1D::Acoustic;

class ConstantLayer::ConstantLayerImpl
{
public:
    double mVelocityInMetersPerSecond{0};
    double mThicknessInMeters{0};
    bool mHasVelocity{false};
    bool mHasThickness{false};
};

/// Constructor
ConstantLayer::ConstantLayer() :
    pImpl(std::make_unique<ConstantLayerImpl> ())
{
}

/// Copy constructor
ConstantLayer::ConstantLayer(const ConstantLayer &layer)
{
    *this = layer;
}

/// Move constructor
ConstantLayer::ConstantLayer(ConstantLayer &&layer) noexcept
{
    *this = std::move(layer);
}

/// Constructor
ConstantLayer::ConstantLayer
    (
        const std::pair<double, double> &thicknessAndVelocity
    ) :
    pImpl(std::make_unique<ConstantLayerImpl> ())
{
    setThickness(thicknessAndVelocity.first);
    setVelocity(thicknessAndVelocity.second);
}

/// Copy assignment
ConstantLayer& ConstantLayer::operator=(const ConstantLayer &layer)
{
    if (&layer == this){return *this;}
    pImpl = std::make_unique<ConstantLayerImpl> (*layer.pImpl);
    return *this;
}

/// Move assignment
ConstantLayer& ConstantLayer::operator=(ConstantLayer &&layer) noexcept
{
    if (&layer == this){return *this;}
    pImpl = std::move(layer.pImpl);
    return *this;
}

/// Velocity
void ConstantLayer::setVelocity(const double velocity)
{
    if (velocity <= 0)
    {
        throw std::invalid_argument("Velocity of "
                                  + std::to_string(velocity)
                                  + " must be positive.");
    }
    pImpl->mVelocityInMetersPerSecond = velocity;
    pImpl->mHasVelocity = true;
}

double ConstantLayer::getVelocity() const
{
    if (!hasVelocity())
    {
        throw std::runtime_error("Velocity not set");
    }
    return pImpl->mVelocityInMetersPerSecond;
}

bool ConstantLayer::hasVelocity() const noexcept
{
    return pImpl->mHasVelocity;
}

/// Thickness
void ConstantLayer::setThickness(const double thickness)
{
    if (thickness <= 0)
    {
        throw std::invalid_argument("Thickness of "
                                  + std::to_string(thickness)
                                  + " must be positive.");
    }
    pImpl->mThicknessInMeters = thickness;
    pImpl->mHasThickness = true;
}

double ConstantLayer::getThickness() const
{
    if (!hasThickness())
    {
        throw std::runtime_error("Thickness not set");
    }
    return pImpl->mThicknessInMeters;
}

bool ConstantLayer::hasThickness() const noexcept
{
    return pImpl->mHasThickness;
}

/// Constructor
ConstantLayer::~ConstantLayer() = default;
