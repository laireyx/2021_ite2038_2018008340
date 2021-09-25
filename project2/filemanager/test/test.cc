#include <gtest/gtest.h>
#include "../file.h"

TEST(SequentialTest, OpenCloseDatabaseTest) {
    EXPECT_EQ(1, file_open_database_file("db"));
    EXPECT_EQ(2, file_open_database_file("db2"));
    EXPECT_EQ(3, file_open_database_file("db3"));
    EXPECT_EQ(4, file_open_database_file("db4"));
    file_close_database_file();
}

TEST(SequentialTest, SequentialAllocateTest) {
    EXPECT_EQ(1, file_open_database_file("db"));
    EXPECT_EQ(1, file_alloc_page());
    EXPECT_EQ(2, file_alloc_page());
    EXPECT_EQ(3, file_alloc_page());
    EXPECT_EQ(4, file_alloc_page());
    file_close_database_file();
}

TEST(SequentialTest, SequentialFreeTest) {
    EXPECT_EQ(1, file_open_database_file("db"));
    file_free_page(4);
    file_free_page(3);
    file_free_page(2);
    file_free_page(1);
    file_close_database_file();
}