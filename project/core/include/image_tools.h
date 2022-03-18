#pragma once

#include <cstdint>
#include <string>
#include <exception>
#include <vector>

namespace charta::ImageTools {
    enum {
        MAX_HEIGHT = 20'000,
        MAX_WIDTH = 50'000,

        RGB_CHANELLS_NO = 3,
    };

    struct DimensionsError : std::exception {
    };

    struct DumpError : std::exception {
    };

    struct CreationError : std::exception {
    };


    class Image {
        size_t height;
        size_t width;

        std::vector<uint8_t> pixels;

        static uint64_t unique_id;

    public:
        Image(size_t height, size_t width);

        void dump(std::string_view filename);

        const std::vector<uint8_t> &get_pixels();
    };

}