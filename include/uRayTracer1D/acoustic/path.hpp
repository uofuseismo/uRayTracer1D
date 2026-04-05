#ifndef URAY_TRACER_1D_ACOUSTIC_PATH_HPP
#define URAY_TRACER_1D_ACOUSTIC_PATH_HPP
#include <memory>
#include <vector>
namespace URayTracer1D::Acoustic
{
 class Segment;
}
namespace URayTracer1D::Acoustic
{
/// @brief Defines a ray path which is a sequence of segments that start
///        at the source and finish at the receiver. 
class Path
{
private:
    using PathType = std::vector<Segment>;
public:
    using iterator = typename PathType::iterator;
    using const_iterator = typename PathType::const_iterator;
public:
    /// @brief Constructor.
    Path();
    /// @brief Copy constructor.
    Path(const Path &path);
    /// @brief Move constructor.
    Path(Path &&path) noexcept;

    /// @name Path Construction Option 1
    /// @{

    /// @brief Opens the path for writing.
    void open() noexcept;
    /// @result True indicates the path is open for construction.
    [[nodiscard]] bool isOpen() const noexcept;
    /// @brief Appends to the path.
    /// @param[in] segment  The segment to append to the path.
    /// @throws std::invalid_argument if the segment does not have a start/end
    ///         point and a velocity.  Additionally, if this is the not the
    ///         source segment then the segment's start point must coincide
    ///         with the previous segment's end point.
    /// @throws std::runtime_error if \c isOpen() is false.
    void append(Segment &&segment);
    void append(const Segment &segment);
    /// @brief Closes the path construction.
    /// @throws std::runtime_error if \c isOpen() is false.
    void close();
    /// @}

    /// @name Path Construction Option 2
    /// @{
    void set(const std::vector<Segment> &segments);
    void set(std::vector<Segment> &&segments);
    /// @}

    /// @result The travel time along the ray path in seconds.
    [[nodiscard]] double travelTime() const;
    /// @result The ray path's length in meters.
    [[nodiscard]] double length() const;

    /// @brief Reverses the ray path.
    void reverse();

    /// @result The number of segments.
    [[nodiscard]] int size() const noexcept; 
    /// @result True indicates there are no segments in the ray path.
    [[nodiscard]] bool empty() const noexcept;
 
    ///@brief Resets the class.
    void clear() noexcept;
    /// @brief Destructor. 
    ~Path();
    /// @brief Copy assignment.
    Path& operator=(const Path &path);
    /// @brief Move assignment.
    Path& operator=(Path &&path) noexcept;

    /// @result A reference to the first ray segment in the path.
    iterator begin();
    /// @result A constant reference to the first ray segment in the path.
    const_iterator begin() const noexcept;
    /// @result A constant reference to the first ray segment in the path.
    const_iterator cbegin() const noexcept;

    /// @result A reference to the last ray segment in the path. 
    iterator end(); 
    /// @result A reference to the last ray segment in the path.
    const_iterator end() const noexcept;
    /// @result A reference to the last ray segment in the path.
    const_iterator cend() const noexcept;

    /// @param[in] index  The index of the desired segment.
    /// @result A reference to the segment at the given position.
    /// @throws std::out_of_range  if this is out of bounds.
    Segment& at(size_t index);
    /// @param[in] index  The index of the desired segment.
    /// @result A reference to the segment at the given position.
    /// @throws std::out_of_range  if this is out of bounds.
    const Segment& at(size_t index) const;
private:
    class PathImpl;
    std::unique_ptr<PathImpl> pImpl;
};
}
#endif
