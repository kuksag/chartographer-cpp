#include "handlers/handler_factory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"
#include "create_img_handler.h"
#include "delete_img_handler.h"
#include "handlers/not_found_handler.h"

using namespace charta;
using namespace Poco::Net;

HandlerFactory::HandlerFactory(ChartographerApplication &app) : app_(app) {
}

Poco::Net::HTTPRequestHandler *HandlerFactory::createRequestHandler(
    const HTTPServerRequest &request) {
    using std::string;
    using std::vector;

    static const string CHARTAS_METHOD = "chartas";

    Poco::URI uri{request.getURI()};
    vector<string> segments;
    uri.getPathSegments(segments);

    if (segments.size() == 1 && segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_POST) {
        return new CreateImageHandler(uri, app_);
    }

    if (segments.size() == 2 && segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_POST) {
    }

    if (segments.size() == 2 && segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_GET) {
    }

    if (segments.size() == 2 && segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_DELETE) {
        return new DeleteImageHandler(uri, app_);
    }

    return new NotFoundHandler{};
}
