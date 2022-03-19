#include "create_img_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"
#include "image_tools.h"

using namespace charta;
using namespace Poco::Net;

namespace {
uint64_t get_unique_id() {
    static uint64_t id = 0;
    return id++;
}
}  // namespace

CreateImageHandler::CreateImageHandler(Poco::URI uri,
                                       ChartographerApplication &app)
    : uri_(std::move(uri)), app_(app) {
}

void charta::CreateImageHandler::handleRequest(
    [[maybe_unused]] Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    size_t width = -1;
    size_t height = -1;

    try {
        auto [height_s, width_s] = get_dimensions(uri_);
        height = strtoul(height_s.c_str(), nullptr, 10);
        width = strtoul(width_s.c_str(), nullptr, 10);
    } catch (...) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    }

    try {
        uint64_t id = get_unique_id();
        std::filesystem::path name = std::to_string(id) + BMP_EXT;

        ImageTools::Image image(height, width);
        image.dump(app_.get_working_folder() / name);

        app_.insert_id(id);
        response.add(ID, std::to_string(id));
        response.setStatus(HTTPResponse::HTTP_CREATED);
    } catch (std::bad_alloc &) {
        response.setStatus(HTTPResponse::HTTP_INSUFFICIENT_STORAGE);
    } catch (std::invalid_argument &) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (...) {
        response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}
