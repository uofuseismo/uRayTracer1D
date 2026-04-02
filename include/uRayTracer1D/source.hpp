#ifndef URAY_TRACER_1D_SOURCE_HPP
#define URAY_TRACER_1D_SOURCE_HPP
#include <memory>
namespace URayTracer1D
{
/// @class Source source.hpp
/// @brief Defines the source location.  The source is emplaced at a depth
///        in meters below the free-surface. 
/// @copyright Ben Baker (University of Utah) distributed under the
///            MIT NO AI license.
class Source
{
public:
    /// @brief Constructor. 
    Source();
    /// @brief Copy constructor.
    Source(const Source &source);
    /// @brief Move constructor.
    Source(Source &&source) noexcept;

    /// @brief Sets the source depth in meters.
    /// @param[in] depthInMeters   The source's the depth (positive down) from
    ///                            the surface.
    void setDepth(double depthInMeters); 
    /// @result The source depth in meters.
    /// @throws std::runtime_error if \c hasDepth() is false.
    [[nodiscard]] double getDepth() const;
    /// @result True indicates the depth was set.
    [[nodiscard]] bool hasDepth() const noexcept;

    /// @brief Destructor.
    ~Source();
    /// @brief Copy assignment.
    Source& operator=(const Source &);
    /// @brief Move assignment.
    Source& operator=(Source &&) noexcept;
private:
    class SourceImpl;    
    std::unique_ptr<SourceImpl> pImpl;
};
}
#endif
