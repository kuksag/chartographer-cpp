#include "handlers/handler_factory.h"

#include <utility>
#include "handlers/not_found_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"

#include "create_img_handler.h"

using namespace charta;
using namespace Poco::Net;

HandlerFactory::HandlerFactory(std::filesystem::path working_folder) : working_folder_(std::move(working_folder)) {}

Poco::Net::HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    using std::string;
    using std::vector;

    static const string CHARTAS_METHOD = "chartas";

	Poco::URI uri{ request.getURI() };
    vector<string> segments;
    uri.getPathSegments(segments);

    if (segments.size() == 1 &&
        segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_POST)
    {
        return new CreateImageHandler(uri, working_folder_);
    }

    if (segments.size() == 2 &&
        segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_POST)
    {

    }

    if (segments.size() == 2 &&
        segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_GET)
    {

    }


    if (segments.size() == 2 &&
        segments.front() == CHARTAS_METHOD &&
        request.getMethod() == HTTPRequest::HTTP_DELETE)
    {

    }

	return new NotFoundHandler{};
}
