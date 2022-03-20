#pragma once
#include <stdexcept>
namespace charta::ImageTools {

struct DimensionsError : std::invalid_argument {
    explicit DimensionsError();
};

}  // namespace charta::ImageTools
