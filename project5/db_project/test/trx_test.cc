/**
 * @addtogroup TestCode
 * @{
 */
#include <db.h>
#include <transaction.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#include <cstdlib>
#include <cstring>
#include <ctime>

constexpr int test_count = 20000;
constexpr int trx_count = 4;

tableid_t table_id;
uint8_t temp_size[test_count] = {};
static uint8_t temp_value[test_count][128] = {};

class BasicTableTest : public ::testing::Test {
   protected:
    /// @brief Random indexes for test count
    int test_order[test_count];

    BasicTableTest() {
        init_db();
        srand(1);

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
    ~BasicTableTest() { shutdown_db(); }
};

void* trx(void* data) {
    int trx = *reinterpret_cast<int*>(data);
    trxid_t trx_id = trx_begin();

    for(int i = 0; i < test_count; i++) {
        db_find(table_id, i, nullptr, nullptr, trx_id);
    }

    trx_commit(trx_id);
    return nullptr;
}

/**
 * @brief   Tests database insertion API.
 * @details 1. Open a database and write random values in random order.
 *          2. Find the value using the key and compare it to the value.
 */
TEST_F(BasicTableTest, RandomInsertTest) {
    table_id = open_table("test_trx.db");
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    for (int i = 0; i < test_count; i++) {
        valsize_t value_size;
        uint8_t return_value[128] = {};

        temp_size[test_order[i]] = 50 + rand() % 63;
        for (int j = 0; j < 128; j++) {
            temp_value[test_order[i]][j] = rand() % 256;
        }

        ASSERT_EQ(db_insert(table_id, test_order[i],
                            reinterpret_cast<char*>(temp_value[test_order[i]]),
                            temp_size[test_order[i]]),
                  0);
    }

    pthread_t trx_thread[trx_count];
    for (int i = 0; i < trx_count; i++) {
        pthread_create(&trx_thread[i], nullptr, trx, reinterpret_cast<void*>(&i));
    }
    for(int i = 0; i < trx_count; i++) {
        pthread_join(trx_thread[i], nullptr);
    }

    for (int i = 0; i < test_count; i++) {
        valsize_t value_size;
        uint8_t return_value[128] = {};

        ASSERT_FALSE(db_find(table_id, test_order[i],
                             reinterpret_cast<char*>(return_value),
                             &value_size) < 0);
        ASSERT_EQ(value_size, temp_size[test_order[i]]);
        ASSERT_EQ(memcmp(temp_value[test_order[i]], return_value,
                         temp_size[test_order[i]]),
                  0);
    }
}

/** @}*/