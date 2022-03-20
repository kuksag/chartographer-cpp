#include "common_handler.h"
#include "Poco/Exception.h"
namespace charta {

std::string get_id(const Poco::URI &uri) {
    std::vector<std::string> segments;
    uri.getPathSegments(segments);
    return segments.size() >= 2 ? segments[1] : "";
}

std::unordered_map<std::string, std::string> get_uri_arguments(
    const Poco::URI &uri) {
    std::unordered_map<std::string, std::string> result;
    auto params = uri.getQueryParameters();
    result.reserve(params.size());

    for (auto &entry : params) {
        result.emplace(entry);
    }

    return result;
}

std::unordered_map<std::string, int> enrich_arguments(
    const Poco::URI &uri,
    const std::vector<std::string> &expected) {
    using std::string;
    std::unordered_map<string, int> result;

    auto args = get_uri_arguments(uri);
    for (const string &key : expected) {
        auto it = args.find(key);
        if (it != args.end()) {
            result.emplace(key, strtoul(it->second.c_str(), nullptr, 10));
        } else {
            throw Poco::InvalidArgumentException();
        }
    }

    return result;
}

}  // namespace charta
