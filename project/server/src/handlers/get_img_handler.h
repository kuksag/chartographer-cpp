#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include "chartographer_application.h"
#include "image_tools.h"

namespace charta {
class GetImageHandler : public Poco::Net::HTTPRequestHandler {
private:
    Poco::URI uri_;
    ChartographerApplication &app_;

public:
    explicit GetImageHandler(Poco::URI uri, ChartographerApplication &app);

    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response) override;
};
}  // namespace charta