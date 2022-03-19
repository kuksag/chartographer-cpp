#pragma once
#include <string>
#include "Poco/URI.h"

namespace charta {

static const char *HEIGHT = "height";
static const char *WIDTH = "width";
static const char *ID = "id";
static const char *BMP_EXT = ".bmp";

std::string get_id(const Poco::URI &uri);

std::pair<std::string, std::string> get_dimensions(const Poco::URI &uri);
}  // namespace charta
