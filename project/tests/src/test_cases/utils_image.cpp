#include "catch2/catch.hpp"
#include "image_tools.h"

TEST_CASE("Simple rectangles intersection", "[rectangle_intersection]") {
    CHECK(ImageTools::rectangle_intersection(0, 0, 2, 2,
                                             1, 1, 2, 2));
}

TEST_CASE("Rectangles border touch", "[rectangle_intersection]") {
    CHECK_FALSE(ImageTools::rectangle_intersection(0, 0, 2, 2,
                                                   2, 0, 2, 2));
}

TEST_CASE("Nested rectangles", "[rectangle_intersection]") {
    CHECK(ImageTools::rectangle_intersection(0, 0, 2, 2,
                                             1, 1, 1, 1));
}

TEST_CASE("No intersection", "[rectangle_intersection]") {
    CHECK_FALSE(ImageTools::rectangle_intersection(0, 0, 2, 2,
                                                   10, 10, 1, 1));
}
