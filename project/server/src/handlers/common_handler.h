#pragma once
#include <unordered_map>
#include "Poco/URI.h"

namespace charta {

static const char *const HEIGHT = "height";
static const char *const WIDTH = "width";
static const char *const ID = "id";
static const char *const BMP_EXT = ".bmp";
static const char *const X_FIELD = "x";
static const char *const Y_FIELD = "y";
static const char *const IMAGE_MEDIA_TYPE = "image";
static const char *const CHARTAS_METHOD = "chartas";

std::string get_id(const Poco::URI &uri);

std::unordered_map<std::string, std::string> get_uri_arguments(
    const Poco::URI &uri);
std::unordered_map<std::string, int> enrich_arguments(
    const Poco::URI &uri,
    const std::vector<std::string> &expected);

}  // namespace charta
