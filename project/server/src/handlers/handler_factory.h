#pragma once
#include <filesystem>
#include <unordered_set>
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "chartographer_application.h"

namespace charta {
class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    ChartographerApplication &app_;

public:
    explicit HandlerFactory(ChartographerApplication &app);
    Poco::Net::HTTPRequestHandler *createRequestHandler(
        const Poco::Net::HTTPServerRequest &request) override;
};
}  // namespace charta
