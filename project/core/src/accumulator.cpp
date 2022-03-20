#include "accumulator.h"
#include <Poco/Exception.h>

const std::filesystem::path &Accumulator::Accumulator::get_path(uint64_t id) {
    std::shared_lock lock(mutex_id_path);
    auto it = id_path.find(id);
    if (it == id_path.end()) {
        throw Poco::InvalidArgumentException();
    }
    std::shared_lock lock1(it->second.mutex);
    return it->second.path;
}

void Accumulator::Accumulator::set_working_directory(
    std::filesystem::path path) {
    working_directory = std::move(path);
}

uint64_t Accumulator::Accumulator::create_object() {
    uint64_t id = gen_unique_id();
    std::unique_lock lock(mutex_id_path);
    id_path[id].path =
        working_directory / (std::to_string(id) + ImageTools::BMP_EXT);
    return id;
}

uint64_t Accumulator::Accumulator::gen_unique_id() {
    static uint64_t id = 0;
    std::unique_lock lock(mutex_gen);
    return id++;
}

bool Accumulator::Accumulator::delete_object(uint64_t id) {
    std::unique_lock lock(mutex_id_path);
    auto it = id_path.find(id);
    if (it == id_path.end()) {
        return false;
    }
    std::unique_lock lock1(it->second.mutex);
    remove(it->second.path);
    id_path.erase(it);
    return true;
}

bool Accumulator::Accumulator::exist(uint64_t id) {
    std::shared_lock lock(mutex_id_path);
    auto it = id_path.find(id);
    return it != id_path.end();
}

std::shared_mutex &Accumulator::Accumulator::get_mutex(uint64_t id) {
    std::shared_lock lock(mutex_id_path);
    return id_path.find(id)->second.mutex;
}
