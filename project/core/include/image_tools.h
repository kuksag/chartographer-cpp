#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace ImageTools {

enum {
    MAX_HEIGHT = 20'000,
    MAX_WIDTH = 50'000,
};

static const char *const BMP_EXT = ".bmp";

#pragma pack(push, 1)
struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool operator==(const Pixel &other) const;
};
#pragma pack(pop)

class Image {
    int height_{};
    int width_{};

    std::vector<Pixel> pixels_;

    void check_dimensions_or_throw() const;
    void check_pixels_or_throw() const;

public:
    Image(int height, int width);
    Image(int height, int width, std::vector<Pixel> pixels);
    explicit Image(const std::filesystem::path &path);
    explicit Image(std::istream &is);

    void dump(const std::filesystem::path &filename) const;

    [[nodiscard]] const std::vector<Pixel> &get_pixels() const;

    static void check_dimensions_or_throw(int height, int width);
    static void check_pixels_or_throw(const std::vector<Pixel> &pixels,
                                      int height,
                                      int width);

    [[nodiscard]] Image crop(int row,
                             int col,
                             int other_height,
                             int other_width) const;

    Pixel &at(size_t row, size_t col) noexcept;
    [[nodiscard]] const Pixel &at(size_t row, size_t col) const noexcept;

    [[nodiscard]] bool contains(size_t row, size_t col) const noexcept;
    void overwrite(const Image &other_image, int row, int col);

    [[nodiscard]] int get_height() const noexcept;
    [[nodiscard]] int get_width() const noexcept;
};

bool rectangle_intersection(int x1, int y1, int w1, int h1,
                            int x2, int y2, int w2, int h2);

}  // namespace charta::ImageTools