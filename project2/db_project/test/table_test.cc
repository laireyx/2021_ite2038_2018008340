/**
 * @addtogroup TestCode
 * @{
 */
#include "file.h"
#include "table.h"

#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>
#include <ctime>

constexpr int test_count = 1024;

class BasicTableTest : public ::testing::Test {
   protected:
    /// @brief Random indexes for test count
    int test_order[test_count];

    BasicTableTest() {
        srand(12345);

        // Generate random indexes
        for (int i = 0; i < test_count; i++) {
            test_order[i] = i;
        }

        for (int i = 0; i < test_count; i++) {
            int x, y, temp;
            x = rand() % test_count;
            y = rand() % test_count;

            temp = test_order[x];
            test_order[x] = test_order[y];
            test_order[y] = temp;
        }
    }
    ~BasicTableTest() { file_close_table_files(); }
};

TEST_F(BasicTableTest, RandomInsertTest) {
    tableid_t table_id = open_table("test2.db");
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    uint8_t temp_value[1024][1024] = {};

    for(int i = 0; i < 1024; i++) {

        for(int j = 0; j < 1024; j++) {
            temp_value[test_order[i]][j] = rand() % 256;
        }

        ASSERT_EQ(db_insert(table_id, test_order[i], reinterpret_cast<char*>(temp_value[test_order[i]]), 1024), 0);
    }

    for(int i = 0; i < 1024; i++) {
        uint16_t value_size;
        uint8_t return_value[1024] = {};
        
        ASSERT_FALSE(db_find(table_id, i, reinterpret_cast<char*>(return_value), &value_size) < 0);
        ASSERT_EQ(value_size, 1024);
        ASSERT_EQ(memcmp(temp_value[i], return_value, 1024), 0);
    }
}

/**
 * @brief   Tests file open/close APIs.
 * @details 1. Open a file and check the descriptor
 *          2. Check if the file's initial size is 10 MiB
 */
TEST_F(BasicTableTest, SequentialInsertTest) {
    tableid_t table_id = open_table("test.db");
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    for(int i = 0; i < 1024; i++) {
        uint8_t temp_value[1024] = {};
        uint8_t return_value[1024] = {};
        uint16_t value_size;

        for(int j = 0; j < 1024; j++) {
            temp_value[j] = rand() % 256;
        }

        db_insert(table_id, i, reinterpret_cast<char*>(temp_value), 1024);

        ASSERT_FALSE(db_find(table_id, i, reinterpret_cast<char*>(return_value), &value_size) < 0);
        ASSERT_EQ(value_size, 1024);
        ASSERT_EQ(memcmp(temp_value, return_value, 1024), 0);
        ASSERT_TRUE(db_find(table_id, i + 1, reinterpret_cast<char*>(return_value), &value_size) < 0);
    }
}

/** @}*/