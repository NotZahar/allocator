#include <gtest/gtest.h>

#include <iostream>
#include <allocator.h>

TEST(Allocator, content_check) {
    std::map<int, int> stdMap;

    for (int i = 0; i < 10; ++i)
        stdMap.insert({ i, i * i });

    std::map<int, int, std::less<int>, allocator::PoolAllocator<std::pair<const int, int>, 10>> poolMap;

    for (int i = 0; i < 10; ++i)
        poolMap.insert({ i, i * i });

    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(stdMap.at(i) == poolMap.at(i), true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}