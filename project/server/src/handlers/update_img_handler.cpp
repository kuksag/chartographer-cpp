#include "update_img_handler.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "accumulator.h"
#include "common_handler.h"
#include "image_tools.h"

using namespace charta;
using namespace Poco::Net;

namespace charta {

UpdateImageHandler::UpdateImageHandler(Poco::URI uri,
                                       Accumulator::Accumulator &accumulator_)
    : uri_(std::move(uri)), accumulator(accumulator_) {}

void UpdateImageHandler::handleRequest(HTTPServerRequest &request,
                                       HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    try {
        string id_s = get_id(uri_);
        size_t id = std::stoul(id_s);
        std::unordered_map<string, int> args =
            enrich_arguments(uri_, {X_FIELD, Y_FIELD, HEIGHT, WIDTH});

        std::shared_lock lock(accumulator.mutex_id_path);
        if (!accumulator.exist(id)) {
            response.setStatusAndReason(
                Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
            response.send();
            return;
        }
        auto path = accumulator.get_path(id);
        std::unique_lock lock1(accumulator.get_mutex(id));

        ImageTools::Image image_to_insert(request.stream());
        if (!(image_to_insert.get_height() == args[HEIGHT] &&
              image_to_insert.get_width() == args[WIDTH])) {
            throw Poco::InvalidArgumentException();
        }

        ImageTools::Image image_to_edit(path);

        if (!ImageTools::rectangle_intersection(
                0, 0, image_to_edit.get_width(), image_to_edit.get_height(),
                args[X_FIELD], args[Y_FIELD], args[WIDTH], args[HEIGHT])) {
            throw Poco::InvalidArgumentException();
        }

        image_to_edit.overwrite(image_to_insert, args[Y_FIELD], args[X_FIELD]);

        image_to_edit.dump(path);
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);

    } catch (Poco::InvalidArgumentException &) {
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (Poco::NotFoundException &) {
        response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND);
    } catch (...) {
        response.setStatusAndReason(
            Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}
}  // namespace charta