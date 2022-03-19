#pragma once

#include <cstdint>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

namespace charta::ImageTools {

enum {
    MAX_HEIGHT = 20'000,
    MAX_WIDTH = 50'000,

    RGB_CHANELLS_NO = 3,
};

uint64_t gen_unique_id();

struct DimensionsError : std::invalid_argument {
    explicit DimensionsError(const std::string &);
};

struct IOError : std::system_error {};

struct CreationError : std::bad_alloc {};

#pragma pack(push, 1)
struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool operator==(const Pixel &other) const;
};
#pragma pack(pop)

class Image {
    size_t height_{};
    size_t width_{};

    std::vector<Pixel> pixels_;

public:
    Image(size_t height, size_t width);
    Image(size_t height, size_t width, std::vector<Pixel> pixels);
    explicit Image(const std::filesystem::path &path);

    void dump(const std::filesystem::path &filename) const;

    [[nodiscard]] const std::vector<Pixel> &get_pixels() const;

    static bool check_dimensions(size_t height, size_t width);

    [[nodiscard]] Image crop(size_t x,
                             size_t y,
                             size_t other_height,
                             size_t other_width) const;
    [[nodiscard]] bool contain_point(size_t x, size_t y) const;
};

}  // namespace charta::ImageTools