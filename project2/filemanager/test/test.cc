#include <gtest/gtest.h>
#include "../file.h"

TEST(TestSuiteName, TestName) {
    EXPECT_EQ(1, file_alloc_page());
}