#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"
#include "image_tools.h"
#include "accumulator.h"

namespace charta {
class DeleteImageHandler : public Poco::Net::HTTPRequestHandler {
private:
    Poco::URI uri_;
    Accumulator::Accumulator &accumulator;

public:
    DeleteImageHandler(Poco::URI uri, Accumulator::Accumulator &accumulator_);

    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response) override;
};
}  // namespace charta