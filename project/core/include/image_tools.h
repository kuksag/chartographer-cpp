#pragma once

#include <cstdint>
#include <string>
#include <exception>
#include <vector>
#include <filesystem>

namespace charta::ImageTools
{
    enum
    {
        MAX_HEIGHT = 20'000,
        MAX_WIDTH = 50'000,

        RGB_CHANELLS_NO = 3,
    };

    struct DimensionsError : std::invalid_argument
    {
        explicit DimensionsError(const std::string &);
    };

    struct DumpError : std::system_error {};

    struct CreationError : std::bad_alloc {};


    class Image
    {
        size_t height;
        size_t width;

        std::vector<uint8_t> pixels;

    public:
        Image(size_t height, size_t width);

        void dump(const std::filesystem::path &filename);

        const std::vector<uint8_t> &get_pixels();
    };

}