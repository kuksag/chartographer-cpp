#pragma once
#include "Poco/Util/ServerApplication.h"
#include <filesystem>
#include <unordered_set>

namespace charta
{
	class ChartographerApplication final : public Poco::Util::ServerApplication
	{
	private:
        std::filesystem::path working_folder_;
        std::unordered_set<uint64_t> present_objects;

		void defineOptions(Poco::Util::OptionSet& options) override;
		void initialize(Application& app) override;
		int main(const std::vector<std::string>& args) override;

		void handle_working_folder(const std::string& name, const std::string& value);

    public:
        const std::filesystem::path& get_working_folder();
        bool is_present_id(uint64_t id);
        void insert_id(uint64_t id);
        bool atomic_delete_id(uint64_t id);
    };
}
