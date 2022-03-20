#include "image_tools.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Poco/Exception.h"
#include "stb_image_write.h"

namespace ImageTools {

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
    auto *img = reinterpret_cast<Pixel *>(
        stbi_load(path.c_str(), reinterpret_cast<int *>(&width_),
                  reinterpret_cast<int *>(&height_), nullptr, STBI_rgb));
    if (img == nullptr) {
        throw;
    }
    check_dimensions_or_throw();
    pixels_ = std::vector<Pixel>(img, img + width_ * height_);
    check_pixels_or_throw();
}

Image::Image(std::istream &is) {
    /*
     * TODO: use stbi_load_from_callbacks because of asynchronous IO
     * I tried to implement in that way but somehow it doesn't work
     */
    auto data = exhaust_stream(is);
    auto *img = reinterpret_cast<Pixel *>(stbi_load_from_memory(
        data.data(), data.size(), &width_, &height_, nullptr, STBI_rgb));
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
        throw Poco::InvalidArgumentException();
    }
}

void Image::check_dimensions_or_throw(int height, int width) {
    bool flag = 0 < abs(height) && abs(height) < MAX_HEIGHT && 0 < abs(width) &&
                abs(width) < MAX_WIDTH;
    if (!flag) {
        throw Poco::InvalidArgumentException();
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
        throw Poco::WriteFileException();
    }
}

const std::vector<Pixel> &Image::get_pixels() const {
    return pixels_;
}

Pixel &Image::at(size_t row, size_t col) noexcept {
    size_t ptr = col + row * width_;
    return pixels_[ptr];
}

const Pixel &Image::at(size_t row, size_t col) const noexcept {
    size_t ptr = col + row * width_;
    return pixels_[ptr];
}

Image Image::crop(int row, int col, int other_height, int other_width) const {
    check_dimensions_or_throw(other_height, other_width);

    row = std::max(0, row);
    col = std::max(0, col);
    other_height = abs(other_height);
    other_width = abs(other_width);
    int height_bound = std::min(abs(height_), row + other_height);
    int width_bound = std::min(abs(width_), col + other_width);
    other_height = height_bound - row;
    other_width = width_bound - col;

    size_t size = other_height * other_width;
    std::vector<Pixel> other_pixels;
    other_pixels.reserve(size);

    for (size_t i = row; i < static_cast<size_t>(height_bound); i++) {
        for (size_t j = col; j < static_cast<size_t>(width_bound); j++) {
            other_pixels.push_back(this->at(i, j));
        }
    }

    return {other_height, other_width, other_pixels};
}

void Image::overwrite(const Image &other_image, int row, int col) {
    for (size_t i = 0; i < static_cast<size_t>(other_image.height_); i++) {
        for (size_t j = 0; j < static_cast<size_t>(other_image.width_); j++) {
            if (this->contains(row + i, col + j)) {
                this->at(row + i, col + j) = other_image.at(i, j);
            }
        }
    }
}

bool Image::contains(size_t row, size_t col) const noexcept {
    return row < static_cast<size_t>(abs(height_)) &&
           col < static_cast<size_t>(abs(width_));
}

int Image::get_height() const noexcept {
    return height_;
}

int Image::get_width() const noexcept {
    return width_;
}

bool Pixel::operator==(const Pixel &other) const {
    return red == other.red && green == other.green && blue == other.blue;
}


bool rectangle_intersection(int x1, int y1, int w1, int h1,
                            int x2, int y2, int w2, int h2) {
    auto cut_intersect = [](int l1, int r1, int l2, int r2) {
        return std::max(l1, l2) <= std::min(r1, r2);
    };
    bool flag = true;
    flag &= cut_intersect(x1, x1 + w1 - 1, x2, x2 + w2 - 1);
    flag &= cut_intersect(y1, y1 + h1 - 1, y2, y2 + h2 - 1);
    return flag;
}
}  // namespace charta::ImageTools