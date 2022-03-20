#include "get_img_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"
#include "image_tools.h"

charta::GetImageHandler::GetImageHandler(Poco::URI uri,
                                         charta::ChartographerApplication &app)
    : uri_(std::move(uri)), app_(app) {}

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
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    }

    if (!app_.is_present_id(id)) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    } else {
        try {
            ImageTools::Image image(app_.get_working_folder() /
                                    (id_s + BMP_EXT));

            std::filesystem::path temp_path =
                app_.get_working_folder() /
                (ImageTools::gen_unique_id() + BMP_EXT);

            try {
                image
                    .crop(args[X_FIELD], args[Y_FIELD], args[HEIGHT],
                          args[WIDTH])
                    .dump(temp_path);
                response.sendFile(temp_path, IMAGE_MEDIA_TYPE);
            } catch (std::exception &e) {
                remove(temp_path);
                throw e;
            }
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            return;
        } catch (Poco::InvalidArgumentException &) {
            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
        } catch (...) {
            response.setStatus(
                Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        }
    }

    response.send();
}
