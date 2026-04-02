#ifndef URAY_TRACER_1D_RECEIVER_HPP
#define URAY_TRACER_1D_RECEIVER_HPP
#include <memory>
namespace URayTracer1D
{
/// @class Receiver receiver.hpp
/// @brief Defines the receiver (station) location.
/// @copyright Ben Baker (University of Utah) distributed under the
///            MIT NO AI license.
class Receiver
{
public:
    /// @brief Constructor. 
    Receiver();
    /// @brief Copy constructor.
    Receiver(const Receiver &receiver);
    /// @brief Move constructor.
    Receiver(Receiver &&receiver) noexcept;

    /// @brief Sets the receiver's elevation in meters.
    /// @param[in] elevationInMeters   The receiver's elevation (positive up).
    void setElevation(double elevationInMeters); 
    /// @result The elevation in meters.
    /// @throws std::runtime_error if \c hasElevation() is false.
    [[nodiscard]] double getElevation() const;
    /// @result True indicates the elevation was set.
    [[nodiscard]] bool hasElevation() const noexcept;

    /// @brief Sets the source-receiver offset in meters.
    /// @param[in] offsetInMeters   The source-receiver offset in meters.
    /// @throws std::invalid_argument if this is negative.
    void setOffset(double offsetInMeters);
    /// @result The offset in meters.
    /// @throws std::runtime_error if \c hasOffset() is false.
    [[nodiscard]] double getOffset() const;
    /// @result True indicates the offset was set.
    [[nodiscard]] bool hasOffset() const noexcept;

    /// @brief Destructor.
    ~Receiver();
    /// @brief Copy assignment.
    Receiver& operator=(const Receiver &);
    /// @brief Move assignment.
    Receiver& operator=(Receiver &&) noexcept;
private:
    class ReceiverImpl;    
    std::unique_ptr<ReceiverImpl> pImpl;
};
}
#endif
