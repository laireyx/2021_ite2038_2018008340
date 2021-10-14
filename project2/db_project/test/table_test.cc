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

constexpr int test_count = 300000;

class BasicTableTest : public ::testing::Test {
   protected:
    /// @brief Random indexes for test count
    int test_order[test_count];

    BasicTableTest() {
        srand(2);

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

/**
 * @brief   Tests database insertion API.
 * @details 1. Open a database and write 1024 random values in random order.
 *          2. Find the value using the key and compare it to the value.
 */
TEST_F(BasicTableTest, RandomDeletionTest) {
    tableid_t table_id = open_table("test_delete.db");
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    for (int i = 0; i < test_count; i++) {
        uint8_t temp_value[1024] = {};
        uint8_t temp_size = 512 + rand() % 512;
        for (int j = 0; j < temp_size; j++) {
            temp_value[j] = rand() % 256;
        }

        ASSERT_EQ(db_insert(table_id, test_order[i],
                            reinterpret_cast<char*>(temp_value),
                            temp_size),
                  0);
    }

    for (int i = 0; i < test_count; i++) {
        uint16_t value_size;
        uint8_t return_value[128] = {};

        if(db_find(table_id, test_order[95255],
                            reinterpret_cast<char*>(return_value),
                            &value_size) < 0) {
            std::cerr << i << "\n";
        }

        if(i >= 64614) {
            db_find(table_id, test_order[95255],
                            reinterpret_cast<char*>(return_value),
                            &value_size);
            std::cerr << i << "\n";
        }

        ASSERT_EQ(db_delete(table_id, test_order[i]), 0);
        ASSERT_TRUE(db_find(table_id, test_order[i],
                            reinterpret_cast<char*>(return_value),
                            &value_size) < 0);
    }
}

/**
 * @brief   Tests database insertion API.
 * @details 1. Open a database and write 1024 random values in random order.
 *          2. Find the value using the key and compare it to the value.
 */
TEST_F(BasicTableTest, RandomInsertTest) {
    tableid_t table_id = open_table("test_insert.db");
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    uint8_t temp_size[test_count] = {};
    static uint8_t temp_value[test_count][128] = {};

    for (int i = 0; i < test_count; i++) {
        uint16_t value_size;
        uint8_t return_value[128] = {};
        
        temp_size[test_order[i]] = 50 + rand() % 63;
        for(int j = 0; j < 128; j++) {
            temp_value[test_order[i]][j] = rand() % 256;
        }

        ASSERT_EQ(db_insert(table_id, test_order[i], reinterpret_cast<char*>(temp_value[test_order[i]]), temp_size[test_order[i]]), 0);
    }

    for (int i = 0; i < test_count; i++) {
        uint16_t value_size;
        uint8_t return_value[128] = {};

        ASSERT_FALSE(db_find(table_id, test_order[i], reinterpret_cast<char*>(return_value), &value_size) < 0);
        ASSERT_EQ(value_size, temp_size[test_order[i]]);
        ASSERT_EQ(memcmp(temp_value[test_order[i]], return_value, temp_size[test_order[i]]), 0);
    }
}

/** @}*/