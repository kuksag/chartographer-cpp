#pragma once
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "chartographer_application.h"
#include "accumulator.h"

namespace charta {
class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    Accumulator::Accumulator &accumulator;

public:
    explicit HandlerFactory(Accumulator::Accumulator &accumulator_);
    Poco::Net::HTTPRequestHandler *createRequestHandler(
        const Poco::Net::HTTPServerRequest &request) override;
};
}  // namespace charta
