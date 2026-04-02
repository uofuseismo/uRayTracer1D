#ifndef URAY_TRACER_1D_ACOUSTIC_MODEL_HPP
#define URAY_TRACER_1D_ACOUSTIC_MODEL_HPP
#include <memory>
#include <vector>
#include <iterator>
#include <uRayTracer1D/acoustic/layer.hpp>
namespace URayTracer1D::Acoustic
{
 class ConstantLayer;
}

namespace URayTracer1D::Acoustic
{

/// @class Model model.hpp
/// @brief Defines a layer-cake model.  
/// @copyright Ben Baker (University of Utah) distributed under the MIT NO
///            AI license.
class Model
{
public:
    Model(const std::vector<URayTracer1D::Acoustic::ConstantLayer> &layer);


    ~Model();
private:
    class ModelImpl;
    std::unique_ptr<ModelImpl> pImpl; 
};

}
#endif
