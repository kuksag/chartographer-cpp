#include "create_img_handler.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"

using namespace charta;
using namespace Poco::Net;

CreateImageHandler::CreateImageHandler(Poco::URI uri,
                                       Accumulator::Accumulator &accumulator_)
    : uri_(std::move(uri)), accumulator(accumulator_) {}

void charta::CreateImageHandler::handleRequest(
    [[maybe_unused]] Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;
    using std::vector;

    std::unordered_map<string, int> args;
    try {
        args = enrich_arguments(uri_, {HEIGHT, WIDTH});
    } catch (...) {
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
        response.send();
        return;
    }

    try {
        uint64_t id = accumulator.create_object();
        ImageTools::Image image(args[HEIGHT], args[WIDTH]);
        image.dump(accumulator.get_path(id));

        response.add(ID, std::to_string(id));
        response.setStatusAndReason(HTTPResponse::HTTP_CREATED);
    } catch (Poco::InvalidArgumentException &) {
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (...) {
        response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}
