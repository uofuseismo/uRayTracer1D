#ifndef URAY_TRACER_1D_ACOUSTIC_CONSTANT_LAYER_HPP
#define URAY_TRACER_1D_ACOUSTIC_CONSTANT_LAYER_HPP
#include <utility>
#include <memory>
namespace URayTracer1D::Acoustic
{
/// @class ConstantLayer constantLayer.hpp
/// @brief A constant acoustic layer is defined by a single phase
///        that moves at a constant velocity throughout the layer.
/// @copyright Ben Baker (University of Utah) distributed under the MIT NO
///            AI license.
class ConstantLayer
{
public:
    /// @brief Constructor.
    ConstantLayer();
    /// @brief Copy constructor.
    ConstantLayer(const ConstantLayer &layer);
    /// @brief Move constructor.
    ConstantLayer(ConstantLayer &&layer) noexcept;
    /// @brief Constructs from a given thickness and velocity.
    /// @param[in] thicknessAndVelocity   The layer thickness in meters and
    ///                                   the layer velocity in meters/second. 
    /// @sa \c setVelocity and \c setThickness.
    explicit ConstantLayer(const std::pair<double, double> &thicknessAndVelocity);

    /// @brief Sets the layer's velocity in meter's per second.
    /// @param[in] velocityInMetersPerSecond  The phase velocity in meters per
    ///                                       second.
    /// @throws std::invalid_argument if this is not positive.
    void setVelocity(double velocityInMetersPerSecond);
    /// @result The phase velocity in meters per second.
    /// @throws std::runtime_error if \c hasVelocity() is false.
    [[nodiscard]] double getVelocity() const;
    /// @result True indicates the velocity was set.
    [[nodiscard]] bool hasVelocity() const noexcept;

    /// @brief Sets the layer's thickness in meters.
    /// @param[in] thicknessInMeters  The layer thickness in meters.
    /// @throws std::invalid_argument if this is not postiive.
    void setThickness(double thicknessInMeters);
    /// @result The thickness in meters.
    /// @throws std::runtime_error if \c hasThickness() is false.
    [[nodiscard]] double getThickness() const;
    /// @result True indicates the thickness was set.
    [[nodiscard]] bool hasThickness() const noexcept;
    
    /// @brief Destructor.
    ~ConstantLayer();
    /// @brief Copy assignment.
    ConstantLayer& operator=(const ConstantLayer &layer);
    /// @brief Move assignment.
    ConstantLayer& operator=(ConstantLayer &&layer) noexcept;
private:
    class ConstantLayerImpl;
    std::unique_ptr<ConstantLayerImpl>  pImpl;
};
}
#endif
