#include <abyssutils/string/VarList.hpp>

#include <gtest/gtest.h>

using namespace Abyssutils::String;

TEST(String, varlist) {
    CVarList list("hello    world!", 0, 's', true);
    EXPECT_EQ(list[0], "hello");
    EXPECT_EQ(list[1], "world!");
}

TEST(String, varlistLastArgAfterEmpty) {
    CVarList list("a,,b,c", 3, ',', true);
    EXPECT_EQ(list[0], "a");
    EXPECT_EQ(list[1], "b");
    EXPECT_EQ(list[2], "c");
}
