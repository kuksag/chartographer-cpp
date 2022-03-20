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

namespace {
class AutoDeleteFile {
    const std::filesystem::path &file_path_;

public:
    explicit AutoDeleteFile(const std::filesystem::path &file_path)
        : file_path_(file_path) {}

    ~AutoDeleteFile() {
        try {
            remove(file_path_);
        } catch (...) {
        }
    }
};
}  // namespace

void charta::GetImageHandler::handleRequest(
    Poco::Net::HTTPServerRequest &,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    string id_s = get_id(uri_);
    size_t id;
    std::unordered_map<string, int> args;
    try {
        id = strtoul(id_s.c_str(), nullptr, 10);
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

            AutoDeleteFile autoDeleteFile(temp_path);
            image.crop(args[X_FIELD], args[Y_FIELD], args[HEIGHT], args[WIDTH])
                .dump(temp_path);

            response.sendFile(temp_path, IMAGE_MEDIA_TYPE);
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
