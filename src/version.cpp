#include <string>
#include "uRayTracer1D/version.hpp"

using namespace URayTracer1D;

int Version::getMajor() noexcept
{
    return URAY_TRACER_1D_MAJOR;
}

int Version::getMinor() noexcept
{
    return URAY_TRACER_1D_MINOR;
}

int Version::getPatch() noexcept
{
    return URAY_TRACER_1D_PATCH;
}

// NOLINTBEGIN
bool Version::isAtLeast(const int major, const int minor,
                        const int patch) noexcept
// NOLINTEND
{
    if (URAY_TRACER_1D_MAJOR < major){return false;}
    if (URAY_TRACER_1D_MAJOR > major){return true;}
    if (URAY_TRACER_1D_MINOR < minor){return false;}
    if (URAY_TRACER_1D_MINOR > minor){return true;}
    if (URAY_TRACER_1D_PATCH < patch){return false;}
    return true;
}

std::string Version::getVersion() noexcept
{
    return std::string{URAY_TRACER_1D_VERSION};
}
