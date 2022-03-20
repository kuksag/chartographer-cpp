#include "update_img_handler.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"
#include "image_tools.h"

using namespace charta;
using namespace Poco::Net;

namespace charta {

UpdateImageHandler::UpdateImageHandler(Poco::URI uri,
                                       ChartographerApplication &app)
    : uri_(std::move(uri)), app_(app) {}
void UpdateImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    string id_s = get_id(uri_);
    size_t id;
    std::unordered_map<string, int> args;

    try {
        id = std::stoul(id_s);
        if (!app_.is_present_id(id)) {
            throw Poco::NotFoundException();
        }
        args = enrich_arguments(uri_, {X_FIELD, Y_FIELD, HEIGHT, WIDTH});

        ImageTools::Image image_to_insert(request.stream());
        if (!(image_to_insert.get_height() == args[HEIGHT] &&
              image_to_insert.get_width() == args[WIDTH])) {
            throw Poco::InvalidArgumentException();
        }

        std::filesystem::path path =
            app_.get_working_folder() / (id_s + BMP_EXT);
        ImageTools::Image image_to_edit(path);

        if (!ImageTools::rectangle_intersection(
                0, 0, image_to_edit.get_width(), image_to_edit.get_height(),
                args[X_FIELD], args[Y_FIELD], args[WIDTH], args[HEIGHT])) {
            throw Poco::InvalidArgumentException();
        }

        image_to_edit.overwrite(image_to_insert, args[X_FIELD], args[Y_FIELD]);

        image_to_edit.dump(path);
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

    } catch (Poco::InvalidArgumentException &) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (Poco::NotFoundException &) {
        response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
    } catch (...) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}
}  // namespace charta