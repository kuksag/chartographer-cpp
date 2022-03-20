#include "get_img_handler.h"
#include <shared_mutex>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"
#include "image_tools.h"

charta::GetImageHandler::GetImageHandler(Poco::URI uri,
                                         Accumulator::Accumulator &accumulator_)
    : uri_(std::move(uri)), accumulator(accumulator_) {}

using namespace charta;
using namespace Poco::Net;

void charta::GetImageHandler::handleRequest(
    Poco::Net::HTTPServerRequest &,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    string id_s = get_id(uri_);
    size_t id;
    std::unordered_map<string, int> args;
    try {
        id = std::stoul(id_s);
        args = enrich_arguments(uri_, {X_FIELD, Y_FIELD, WIDTH, HEIGHT});
    } catch (...) {
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
        response.send();
        return;
    }

    std::shared_lock lock(accumulator.mutex_id_path);
    if (!accumulator.exist(id)) {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.send();
        return;
    }
    std::shared_lock lock1(accumulator.get_mutex(id));

    try {
        auto path = accumulator.get_path(id);
        ImageTools::Image image(path);

        std::filesystem::path temp_path = string(path) + ".tmp";

        try {
            image.crop(args[Y_FIELD], args[X_FIELD], args[HEIGHT], args[WIDTH])
                .dump(temp_path);
            response.sendFile(temp_path, IMAGE_MEDIA_TYPE);
            remove(temp_path);
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
            return;
        } catch (std::exception &e) {
            remove(temp_path);
            throw e;
        }
    } catch (Poco::InvalidArgumentException &) {
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (...) {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }
    response.send();
}
