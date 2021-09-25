#include <gtest/gtest.h>
#include "../file.h"

TEST(SequentialTest, SequentialAllocateTest) {
    EXPECT_EQ(1, file_alloc_page());
    EXPECT_EQ(2, file_alloc_page());
    EXPECT_EQ(3, file_alloc_page());
    EXPECT_EQ(4, file_alloc_page());
}

TEST(SequentialTest, SequentialFreeTest) {
    file_free_page(4);
    file_free_page(3);
    file_free_page(2);
    file_free_page(1);
}