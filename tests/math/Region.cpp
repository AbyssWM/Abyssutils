#include <abyssutils/math/Region.hpp>

#include <gtest/gtest.h>

using namespace Abyssutils::Math;

TEST(Math, region) {
    CRegion rg(CBox{{20, 20}, {40, 40}});

    auto    extents = rg.getExtents();
    EXPECT_EQ(extents.pos(), Vector2D(20, 20));
    EXPECT_EQ(extents.size(), Vector2D(40, 40));

    rg.scale(2);
    extents = rg.getExtents();
    EXPECT_EQ(extents.pos(), Vector2D(40, 40));
    EXPECT_EQ(extents.size(), Vector2D(80, 80));
}

TEST(Math, regionClosestPoint) {
    CRegion rg;
    rg.add(CBox{{0, 0}, {10, 10}});
    rg.add(CBox{{1000, 1000}, {10, 10}});

    // the point is next to the far box, not the one at the origin
    EXPECT_EQ(rg.closestPoint({1020, 1005}), Vector2D(1009, 1005));
    EXPECT_EQ(rg.closestPoint({5, 20}), Vector2D(5, 9));
}
