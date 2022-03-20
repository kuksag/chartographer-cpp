#include "image_tools.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

namespace charta::ImageTools {

namespace {
std::vector<uint8_t> exhaust_stream(std::istream &is) {
    std::vector<uint8_t> data;
    char block;
    while (is.get(block)) {
        data.push_back(block);
    }
    return data;
}
}  // namespace

uint64_t gen_unique_id() {
    static uint64_t id = 0;
    return id++;
}

Image::Image(int height, int width) : height_(height), width_(width) {
    check_dimensions_or_throw();
    pixels_.resize(height_ * width_);
}

Image::Image(int height, int width, std::vector<Pixel> pixels)
    : height_(height), width_(width), pixels_(std::move(pixels)) {
    check_dimensions_or_throw();
    check_pixels_or_throw();
}

Image::Image(const std::filesystem::path &path) {
    auto *img = reinterpret_cast<Pixel *>(stbi_load(
        path.c_str(), reinterpret_cast<int *>(&width_),
        reinterpret_cast<int *>(&height_), nullptr, STBI_rgb));
    if (img == nullptr) {
        throw;
    }
    check_dimensions_or_throw();
    pixels_ = std::vector<Pixel>(img, img + width_ * height_);
    check_pixels_or_throw();
}

Image::Image(std::istream &is) {
    auto data = exhaust_stream(is);
    auto *img = reinterpret_cast<Pixel *>(
        stbi_load_from_memory(data.data(), data.size(), &width_,
                              &height_, nullptr, STBI_rgb));
    if (img == nullptr) {
        throw;
    }
    check_dimensions_or_throw();
    pixels_ = std::vector<Pixel>(img, img + width_ * height_);
    check_pixels_or_throw();
}

void Image::check_pixels_or_throw(const std::vector<Pixel> &pixels,
                                  int height,
                                  int width) {
    if (static_cast<int>(pixels.size()) != abs(height * width)) {
        throw DimensionsError();
    }
}

void Image::check_dimensions_or_throw(int height, int width) {
    bool flag = 0 < abs(height) && abs(height) < MAX_HEIGHT &&
                0 < abs(width) && abs(width) < MAX_WIDTH;
    if (!flag) {
        throw DimensionsError();
    }
}

void Image::check_dimensions_or_throw() const {
    check_dimensions_or_throw(height_, width_);
}

void Image::check_pixels_or_throw() const {
    check_pixels_or_throw(pixels_, height_, width_);
}

void Image::dump(const std::filesystem::path &filename) const {
    if (!stbi_write_bmp(filename.c_str(), width_, height_, STBI_rgb,
                        pixels_.data())) {
        throw std::runtime_error("");
    }
}

const std::vector<Pixel> &Image::get_pixels() const {
    return pixels_;
}
Image Image::crop(int x,
                  int y,
                  int other_height,
                  int other_width) const {
    check_dimensions_or_throw(other_height, other_width);

    x = std::max(0, x);
    y = std::max(0, y);
    other_height = abs(other_height);
    other_width = abs(other_width);
    int height_bound = std::min(abs(height_), x + other_height);
    int width_bound = std::min(abs(width_), y + other_width);
    other_height = height_bound - x;
    other_width = width_bound - y;

    size_t size = other_height * other_width;
    std::vector<Pixel> other_pixels;
    other_pixels.reserve(size);

    for (size_t row = x; row < static_cast<size_t>(height_bound); row++) {
        for (size_t col = y; col < static_cast<size_t>(width_bound); col++) {
            size_t ptr = col + row * width_;
            other_pixels.push_back(pixels_[ptr]);
        }
    }

    return {other_height, other_width, other_pixels};
}

bool Pixel::operator==(const Pixel &other) const {
    return red == other.red && green == other.green &&
           blue == other.blue;
}
}  // namespace charta::ImageTools