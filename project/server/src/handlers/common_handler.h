#pragma once
#include <unordered_map>
#include "Poco/URI.h"

namespace charta {

static const char *HEIGHT = "height";
static const char *WIDTH = "width";
static const char *ID = "id";
static const char *BMP_EXT = ".bmp";
static const char *X_FIELD = "x";
static const char *Y_FIELD = "y";
static const char *IMAGE_MEDIA_TYPE = "image";
static const char *CHARTAS_METHOD = "chartas";


std::string get_id(const Poco::URI &uri);

std::unordered_map<std::string, std::string> get_uri_arguments(
    const Poco::URI &uri);
std::unordered_map<std::string, size_t> enrich_arguments(
    const Poco::URI &uri, const std::vector<std::string> &expected);
}  // namespace charta
