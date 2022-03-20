#include "catch2/catch.hpp"
#include "image_tools.h"

using namespace charta;

TEST_CASE("Crop simple image", "[crop_image]") {
    typedef std::vector<charta::ImageTools::Pixel> pixelVector;

    // 3 pixels:
    // [[RED], [GREEN], [BLUE]]
    pixelVector pixels = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
    };

    pixelVector red_pixel = {
        {255, 0, 0},
    };

    pixelVector green_pixel = {
        {0, 255, 0},
    };

    pixelVector blue_pixel = {
        {0, 0, 255},
    };

    ImageTools::Image image(1, 3, pixels);

    CHECK(red_pixel == image.crop(0, 0, 1, 1).get_pixels());
    CHECK(green_pixel == image.crop(0, 1, 1, 1).get_pixels());
    CHECK(blue_pixel == image.crop(0, 2, 1, 1).get_pixels());

    CHECK(image.get_pixels() == image.crop(0, 0, 1, 3).get_pixels());
}

TEST_CASE("Crop with bad dimensions", "[crop_image]") {
    ImageTools::Image image(10, 10);
    CHECK_THROWS(image.crop(20, 0, 1, 1));
    CHECK_THROWS(image.crop(0, 20, 1, 1));
    CHECK_THROWS(image.crop(0, 0, 1e9, 1));
    CHECK_THROWS(image.crop(0, 0, 1, 1e9));
}

TEST_CASE("Crop with overhead arguments", "[crop_image]") {
    typedef std::vector<charta::ImageTools::Pixel> pixelVector;

    // 3 pixels:
    // [[RED], [GREEN], [BLUE]]
    pixelVector pixels = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
    };

    ImageTools::Image image(1, 3, pixels);

    CHECK(image.get_pixels() == image.crop(0, 0, 1000, 1000).get_pixels());

    CHECK(image.get_pixels() ==
          image.crop(-1000, -1000, 1000, 1000).get_pixels());

    CHECK(image.crop(0, 1, 1, 1).get_pixels() ==
          image.crop(-1000, 1, 1000, 1).get_pixels());
}