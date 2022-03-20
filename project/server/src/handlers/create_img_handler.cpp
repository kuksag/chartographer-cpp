#include "create_img_handler.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"
#include "image_tools.h"
using namespace charta;
using namespace Poco::Net;

CreateImageHandler::CreateImageHandler(Poco::URI uri,
                                       ChartographerApplication &app)
    : uri_(std::move(uri)), app_(app) {}

void charta::CreateImageHandler::handleRequest(
    Poco::Net::HTTPServerRequest &,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    std::unordered_map<string, int> args;
    try {
        args = enrich_arguments(uri_, {HEIGHT, WIDTH});
    } catch (...) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    }

    try {
        uint64_t id = ImageTools::gen_unique_id();
        std::filesystem::path name = std::to_string(id) + BMP_EXT;

        ImageTools::Image image(args[HEIGHT], args[WIDTH]);
        image.dump(app_.get_working_folder() / name);

        app_.insert_id(id);
        response.add(ID, std::to_string(id));
        response.setStatus(HTTPResponse::HTTP_CREATED);
    } catch (Poco::InvalidArgumentException &) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (...) {
        response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}
