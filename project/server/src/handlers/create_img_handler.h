#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include "image_tools.h"
#include "chartographer_application.h"

namespace charta
{
    class CreateImageHandler : public Poco::Net::HTTPRequestHandler
    {
    private:
        Poco::URI uri_;
        ChartographerApplication &app_;

    public:
        explicit CreateImageHandler(Poco::URI uri, ChartographerApplication &app);

        void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
    };
}