#include <abyssutils/string/ConstVarList.hpp>

#include <gtest/gtest.h>

using namespace Abyssutils::String;

TEST(String, constvarlist) {
    CConstVarList listConst("hello    world!", 0, 's', true);
    EXPECT_EQ(listConst[0], "hello");
    EXPECT_EQ(listConst[1], "world!");

    CConstVarList listConst2("0 set", 2, ' ');
    EXPECT_EQ(listConst2[0], "0");
    EXPECT_EQ(listConst2[1], "set");
}

TEST(String, constvarlistLastArgAfterEmpty) {
    CConstVarList list("a,,b,c", 3, ',', true);
    EXPECT_EQ(list[0], "a");
    EXPECT_EQ(list[1], "b");
    EXPECT_EQ(list[2], "c");

    CConstVarList rest("a,b,c", 2, ',');
    EXPECT_EQ(rest[1], "b,c");
}
