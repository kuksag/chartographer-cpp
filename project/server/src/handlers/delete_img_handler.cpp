#include "delete_img_handler.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "common_handler.h"

using namespace charta;
using namespace Poco::Net;

charta::DeleteImageHandler::DeleteImageHandler(
    Poco::URI uri,
    Accumulator::Accumulator &accumulator_)
    : uri_(std::move(uri)), accumulator(accumulator_) {}

void charta::DeleteImageHandler::handleRequest(
    Poco::Net::HTTPServerRequest &,
    Poco::Net::HTTPServerResponse &response) {
    using std::string;

    string id_s = get_id(uri_);
    uint64_t id;
    try {
        id = std::stoi(id_s);
    } catch (...) {
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
        response.send();
        return;
    }

    try {
        response.setStatusAndReason(accumulator.delete_object(id)
                               ? Poco::Net::HTTPResponse::HTTP_OK
                               : Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    } catch (...) {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}