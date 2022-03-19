#include "create_img_handler.h"

#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "image_tools.h"

using namespace charta;
using namespace Poco::Net;

namespace {
    uint64_t get_unique_id() {
        static uint64_t id = 0;
        return id++;
    }
}

charta::CreateImageHandler::CreateImageHandler(Poco::URI uri, std::filesystem::path folder_path)
    : uri_(std::move(uri)), folder_path_(std::move(folder_path)) {}

void charta::CreateImageHandler::handleRequest([[maybe_unused]] Poco::Net::HTTPServerRequest &request,
                                               Poco::Net::HTTPServerResponse &response)
{
    using std::string;
    using std::vector;

    const string WIDTH = "width";
    const string HEIGHT = "height";
    const string ID = "id";

    size_t width = -1;
    size_t height = -1;

    try
    {
        for (auto &[key, value] : uri_.getQueryParameters())
        {
            if (key == WIDTH)
            {
                width = strtoul(value.c_str(), nullptr, 10);
            } else if (key == HEIGHT)
            {
                height = strtoul(value.c_str(), nullptr, 10);
            }
        }
    } catch (...)
    {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    }


    try
    {
        uint64_t id = get_unique_id();
        std::filesystem::path name = std::to_string(id) + ".bmp";

        ImageTools::Image image(height, width);
        image.dump(folder_path_ / name);

        response.add(ID, std::to_string(id));
        response.setStatus(HTTPResponse::HTTP_CREATED);
    } catch (std::bad_alloc &)
    {
        response.setStatus(HTTPResponse::HTTP_INSUFFICIENT_STORAGE);
    } catch (std::invalid_argument &)
    {
        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
    } catch (...)
    {
        response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }

    response.send();
}
