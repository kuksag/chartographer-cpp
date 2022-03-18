#include "catch2/catch.hpp"
#include "image_tools.h"

using namespace charta;

TEST_CASE("Create common image", "[create_image]") {
    int height = 10;
    int width = 10;
    ImageTools::Image image(height, width);
    CHECK(image.get_pixels() == std::vector<uint8_t>(height * width * 3, 0));
}

TEST_CASE("Create image with bad bounds", "[create_image]") {
    CHECK_THROWS_AS(ImageTools::Image(1e9, 10), ImageTools::DimensionsError);
    CHECK_THROWS_AS(ImageTools::Image(10, 1e9), ImageTools::DimensionsError);

    CHECK_THROWS_AS(ImageTools::Image(-10, 10), ImageTools::DimensionsError);
    CHECK_THROWS_AS(ImageTools::Image(10, -10), ImageTools::DimensionsError);

    CHECK_THROWS_AS(ImageTools::Image(0, 10), ImageTools::DimensionsError);
    CHECK_THROWS_AS(ImageTools::Image(10, 0), ImageTools::DimensionsError);
}

TEST_CASE("Dump image", "[create_image]") {
    ImageTools::Image image(10, 10);
    image.dump("temp.bmp");
    CHECK_THROWS_AS(image.dump("/perms_denied.bmp"), ImageTools::DumpError);
}