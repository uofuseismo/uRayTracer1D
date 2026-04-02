#ifndef URAY_TRACER_1D_ACOUSTIC_SEGMENT_HPP
#define URAY_TRACER_1D_ACOUSTIC_SEGMENT_HPP
#include <memory>
#include <optional>
#include <utility>
namespace URayTracer1D::Acoustic
{
/// @class Segment segment.hpp
/// @brief A ray path is comprised of segments.
/// @copyright Ben Baker (University of Utah) distributed under the
///            MIT NO AI license.
class Segment
{
public:
    /// @brief Constructor.
    Segment();
    /// @brief Copy constructor.
    Segment(const Segment &segment);
    /// @brief Move constructor.
    Segment(Segment &&segent) noexcept;
    /// @brief Creates a segment from the start point, end point, slowness,
    ///        and, optionally, the layer number
    /// @param[in] startAndEndPoint   The start point and end point of the
    ///                               ray.  Each point is specified by the
    ///                               offset (positive away from source)
    ///                               and depth (positive down) in meters.
    /// @param[in] slowness    The layer slowness in seconds/meter.
    /// @param[in] layerIndex  The index of the layer in the velocity model
    ///                        containing the ray.
    /// @sa \c setSlowness(), \c setStartPoint(), \c setEndPoint(), \c setLayerNumber().
    Segment(const double slowness,
            const std::pair
            < 
                std::pair<double, double>,
                std::pair<double, double>
            > &startAndEndPoint,
            const int layerIndex);
    Segment(const double slowness,
            const std::pair
            <   
                std::pair<double, double>,
                std::pair<double, double>
            > &startAndEndPoint);

    /// @result The ray segment length in meters. 
    [[nodiscard]] double length() const;
    /// @result The travel time in seconds.
    [[nodiscard]] double travelTime() const;

    /// @brief Sets the layer slowness.
    /// @param[in] slowness   The layer slowness in seconds/meter.
    void setSlowness(double slowness);
    /// @result the slowness in the seconds/meter.
    [[nodiscard]] double getSlowness() const;
    /// @result True indicates the slowness was set.
    [[nodiscard]] bool hasSlowness() const noexcept;

    /// @brief Sets the ray start point.
    /// @param[in] startPoint   The offset and depth of the starting point of
    ///                         the ray in meters.
    void setStartPoint(const std::pair<double, double> &startPoint);
    /// @result The ray's start point in the layer.
    [[nodiscard]] std::pair<double, double> getStartPoint() const;
    /// @result True indicates the start point was set.
    [[nodiscard]] bool hasStartPoint() const noexcept;

    /// @brief Sets the ray end point.
    void setEndPoint(const std::pair<double, double> &endPoint);
    /// @result The ray's end point in the layer.
    [[nodiscard]] std::pair<double, double> getEndPoint() const;
    /// @result True indicates the end point was set.
    [[nodiscard]] bool hasEndPoint() const noexcept;

    /// @brief Sets the index of the layer in the velocity model
    ///        containing the ray.
    void setLayerIndex(int layer);
    /// @result The layer number.
    [[nodiscard]] std::optional<int> getLayerIndex() const noexcept;

    /// @brief Reverses the segment.
    void reverse();

    /// @brief Destructor.
    ~Segment();
    /// @brief Copy assignment.
    Segment& operator=(const Segment &segment);
    /// @brief Move assignment.
    Segment& operator=(Segment &&segment) noexcept;
private:
    class SegmentImpl;
    std::unique_ptr<SegmentImpl> pImpl;
};
}
#endif
