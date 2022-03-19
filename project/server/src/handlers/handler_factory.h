#pragma once
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "chartographer_application.h"
#include <filesystem>
#include <unordered_set>

namespace charta
{
	class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
        ChartographerApplication &app_;
	public:
        explicit HandlerFactory(ChartographerApplication &app);
		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
	};
}
