#pragma once
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include <filesystem>

namespace charta
{
	class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
        std::filesystem::path working_folder_;
	public:
        explicit HandlerFactory(std::filesystem::path working_folder);
		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
	};
}
