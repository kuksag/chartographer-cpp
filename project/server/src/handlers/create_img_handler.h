#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include "image_tools.h"

namespace charta
{
    class CreateImageHandler : public Poco::Net::HTTPRequestHandler
    {
    private:
        Poco::URI uri_;
        std::filesystem::path folder_path_;

    public:
        CreateImageHandler(Poco::URI uri, std::filesystem::path folder_path);

        void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
    };
}