#include "delete_img_handler.h"
#include <filesystem>
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"

using namespace charta;
using namespace Poco::Net;

charta::DeleteImageHandler::DeleteImageHandler(
    Poco::URI uri,
    charta::ChartographerApplication &app)
    : uri_(std::move(uri)), app_(app) {}

void charta::DeleteImageHandler::handleRequest(
    Poco::Net::HTTPServerRequest &,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;

    string id_s = get_id(uri_);
    uint64_t id;
    try {
        id = std::stoi(id_s);
    } catch (...) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    }

    try {
        if (app_.atomic_delete_id(id)) {
            if (!remove(app_.get_working_folder() / (id_s + BMP_EXT))) {
                throw;
            }
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        } else {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        }
    } catch (...) {
        response.setStatus(
            Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}