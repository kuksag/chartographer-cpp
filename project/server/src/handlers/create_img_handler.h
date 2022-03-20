#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include "accumulator.h"
#include "image_tools.h"

namespace charta {
class CreateImageHandler : public Poco::Net::HTTPRequestHandler {
private:
    Poco::URI uri_;
    Accumulator::Accumulator &accumulator;

public:
    explicit CreateImageHandler(Poco::URI uri,
                                Accumulator::Accumulator &accumulator_);

    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response) override;
};
}  // namespace charta