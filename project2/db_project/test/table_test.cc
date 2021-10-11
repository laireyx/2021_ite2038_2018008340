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

class BasicTableTest : public ::testing::Test {
   protected:
    /// @brief Test table id
    int table_id = 0;

    BasicTableTest() {
        table_id = open_table("test.db");
        srand(time(NULL));
    }
    ~BasicTableTest() { file_close_table_files(); }
};

/**
 * @brief   Tests file open/close APIs.
 * @details 1. Open a file and check the descriptor
 *          2. Check if the file's initial size is 10 MiB
 */
TEST_F(BasicTableTest, SequentialInsertTest) {
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    for(int i = 0; i < 250; i++) {
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

    for(int i = 250; i < 256; i++) {
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