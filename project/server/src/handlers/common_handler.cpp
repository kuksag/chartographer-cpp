#include "common_handler.h"

std::string charta::get_id(const Poco::URI &uri) {
    std::vector<std::string> segments;
    uri.getPathSegments(segments);
    return segments.size() >= 2 ? segments[1] : "";
}

std::pair<std::string, std::string> charta::get_dimensions(
    const Poco::URI &uri) {
    std::string height;
    std::string width;

    for (auto &[key, value] : uri.getQueryParameters()) {
        if (key == charta::HEIGHT) {
            height = value;
        } else if (key == charta::WIDTH) {
            width = value;
        }
    }

    return {height, width};
}