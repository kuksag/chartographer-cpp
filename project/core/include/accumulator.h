#pragma once
#include "image_tools.h"
#include <filesystem>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
namespace Accumulator {

class Accumulator {
    struct Resource {
        std::filesystem::path path;
        std::shared_mutex mutex{};
        // TODO: have only one instance of an image per time via shared_ptr
    };

    std::filesystem::path working_directory;

    std::unordered_map<uint64_t, Resource> id_path{};

    std::mutex mutex_gen;
    uint64_t gen_unique_id();

public:
    std::shared_mutex mutex_id_path;

    [[nodiscard]] const std::filesystem::path &get_path(uint64_t id);

    void set_working_directory(std::filesystem::path path);

    uint64_t create_object();

    bool delete_object(uint64_t id);
    bool exist(uint64_t id);
    std::shared_mutex &get_mutex(uint64_t id);
};

}  // namespace Accumulator