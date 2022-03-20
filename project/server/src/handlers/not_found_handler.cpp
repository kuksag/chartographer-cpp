#include "handlers/not_found_handler.h"
#include "Poco/Net/HTTPServerResponse.h"

using namespace charta;
using namespace Poco::Net;

void NotFoundHandler::handleRequest([[maybe_unused]] HTTPServerRequest &request,
                                    HTTPServerResponse &response) {
    response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND);
    response.send();
}
