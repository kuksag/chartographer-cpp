#include "image_tools.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

using namespace charta::ImageTools;

uint64_t Image::unique_id = 0;

Image::Image(size_t height_, size_t width_) : height(height_), width(width_) {
    if (!(0 < height && height <= MAX_HEIGHT) ||
        !(0 < width && width <= MAX_WIDTH)) {
        throw DimensionsError();
    }
    try {
        pixels.resize(height * width * RGB_CHANELLS_NO);
    } catch (std::bad_alloc &) {
        throw CreationError();
    }
}

void Image::dump(std::string_view filename) {
    if (!stbi_write_bmp(filename.data(), width, height, RGB_CHANELLS_NO, pixels.data())) {
        throw DumpError();
    }
}

const std::vector<uint8_t> &Image::get_pixels() {
    return pixels;
}
