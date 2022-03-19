#include "image_tools.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

using namespace charta::ImageTools;

Image::Image(size_t height, size_t width) : height_(height), width_(width) {
    if (!check_dimensions(height_, width_)) {
        throw DimensionsError("");
    }
    try {
        pixels_.resize(height_ * width_);
    } catch (std::bad_alloc &) {
        throw CreationError();
    }
}

Image::Image(size_t height, size_t width, std::vector<Pixel> pixels)
    : height_(height), width_(width), pixels_(std::move(pixels)) {
    if (!check_dimensions(height_, width_) ||
        pixels_.size() != height * width) {
        throw DimensionsError("");
    }
}

void Image::dump(const std::filesystem::path &filename) const {
    if (!stbi_write_bmp(filename.c_str(), width_, height_, RGB_CHANELLS_NO,
                        pixels_.data())) {
        throw DumpError();
    }
}

const std::vector<Pixel> &Image::get_pixels() const {
    return pixels_;
}

bool Image::check_dimensions(size_t height, size_t width) {
    return (0 < height && height < MAX_HEIGHT) &&
           (0 < width && width < MAX_WIDTH);
}

DimensionsError::DimensionsError(const std::string &e) : invalid_argument(e) {
}

Image Image::crop(size_t x,
                  size_t y,
                  size_t other_height,
                  size_t other_width) const {
    if (!check_dimensions(other_height, other_width) || !contain_point(x, y)) {
        throw DimensionsError("");
    }
    size_t height_bound = std::min(height_, x + other_height);
    size_t width_bound = std::min(width_, y + other_width);
    other_height = height_bound - x;
    other_width = width_bound - y;

    size_t size = other_height * other_width;
    std::vector<Pixel> other_pixels;
    other_pixels.reserve(size);

    for (size_t row = x; row < height_bound; row++) {
        for (size_t col = y; col < width_bound; col++) {
            size_t ptr = col + row * width_;
            other_pixels.push_back(pixels_[ptr]);
        }
    }

    return {other_height, other_width, other_pixels};
}

bool Image::contain_point(size_t x, size_t y) const {
    return x < height_ && y < width_;
}

bool Pixel::operator==(const Pixel &other) const {
    return red == other.red &&
           green == other.green
           && blue == other.blue;
}
