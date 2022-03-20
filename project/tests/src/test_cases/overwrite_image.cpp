#include "catch2/catch.hpp"
#include "image_tools.h"

using namespace charta;

TEST_CASE("Paint from black to green", "[overwrite_image]") {
    typedef std::vector<charta::ImageTools::Pixel> pixelVector;

    ImageTools::Pixel black_pixel = {
        0,
        0,
        0,
    };

    ImageTools::Pixel green_pixel = {
        0,
        255,
        0,
    };

    ImageTools::Image image_green_pixel(1, 1, {green_pixel});
    ImageTools::Image image(1, 3);

    image.overwrite(image_green_pixel, 0, 0);

    CHECK(image.get_pixels() ==
          pixelVector{green_pixel, black_pixel, black_pixel});

    image.overwrite(image_green_pixel, 0, 1);
    CHECK(image.get_pixels() ==
          pixelVector{green_pixel, green_pixel, black_pixel});

    image.overwrite(image_green_pixel, 0, 2);
    CHECK(image.get_pixels() ==
          pixelVector{green_pixel, green_pixel, green_pixel});
}

TEST_CASE("Simple overwrite", "[overwrite_image]") {
    typedef std::vector<charta::ImageTools::Pixel> pixelVector;

    ImageTools::Pixel red_pixel{255, 0, 0};
    ImageTools::Pixel green_pixel{0, 255, 0};
    ImageTools::Pixel blue_pixel{0, 0, 255};

    ImageTools::Image red_image(1, 5, pixelVector(5, red_pixel));
    ImageTools::Image green_image(1, 5, pixelVector(5, green_pixel));
    ImageTools::Image blue_image(5, 1, pixelVector(5, blue_pixel));

    ImageTools::Image canvas(1, 3);
    // [B, B, B]

    canvas.overwrite(red_image, 0, 0);
    // [R, R, R]

    canvas.overwrite(green_image, 0, 2);
    // [R, R, G]

    canvas.overwrite(blue_image, -1, 1);
    // [R, B, G]

    CHECK(canvas.get_pixels() ==
          pixelVector{red_pixel, blue_pixel, green_pixel});
}

TEST_CASE("Overwrite with no intersection", "[overwrite_image]") {
    typedef std::vector<charta::ImageTools::Pixel> pixelVector;

    ImageTools::Pixel black_pixel{0, 0, 0};
    ImageTools::Pixel red_pixel{255, 0, 0};

    ImageTools::Image red_image(10, 10, pixelVector(100, red_pixel));

    ImageTools::Image canvas(10, 10);
    canvas.overwrite(red_image, -10, 0);
    canvas.overwrite(red_image, 0, -10);
    canvas.overwrite(red_image, 10, 0);
    canvas.overwrite(red_image, 0, 10);
    CHECK(canvas.get_pixels() == pixelVector(100, black_pixel));
}