#include "delete_img_handler.h"
#include <filesystem>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"

using namespace charta;
using namespace Poco::Net;

charta::DeleteImageHandler::DeleteImageHandler(
    Poco::URI uri,
    charta::ChartographerApplication &app)
    : uri_(std::move(uri)), app_(app) {
}

void charta::DeleteImageHandler::handleRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;

    string id_s = get_id(uri_);
    uint64_t id;
    try {
        id = strtoul(id_s.c_str(), nullptr, 10);
    } catch (...) {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    }

    if (app_.atomic_delete_id(id)) {
        try {
            if (!remove(app_.get_working_folder() / (id_s + BMP_EXT))) {
                throw;
            }
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        } catch (...) {
            response.setStatus(
                Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        }
    } else {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    }

    response.send();
}