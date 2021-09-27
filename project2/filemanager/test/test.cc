#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include "../file.h"

constexpr int test_count = 2048;

class FilemanagerTest : public ::testing::Test {
protected:
    int test_order[test_count];
    int db_id = 0;

    void SetUp() override {
        db_id = file_open_database_file("test.db");
        srand(time(NULL));

        for(int i = 0; i < test_count; i++) {
            test_order[i] = i + 1;
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
    void TearDown() override {
        file_close_database_file();
    }
};

TEST_F(FilemanagerTest, SequentialAllocateTest) {
    int allocation_result[test_count] = { 0, };

    for(int i = 0; i < 4; i++) {
        for(int i = 0; i < test_count; i++) {
            allocation_result[i] = file_alloc_page(db_id);
        }

        for(int i = 0; i < test_count; i++) {
            file_free_page(db_id, allocation_result[i]);
        }
    }
}

TEST_F(FilemanagerTest, RandomAllocateTest) {
    for(int i = 0; i < test_count; i++) {
        file_alloc_page(db_id);
    }
    
    for(int i = 0; i < test_count; i++) {
        file_free_page(db_id, test_order[i]);
    }

    for(int i = 0; i < test_count; i++) {
        EXPECT_EQ(test_order[test_count - 1 - i], file_alloc_page(db_id));
    }

    for(int i = 0; i < test_count; i++) {
        file_free_page(db_id, test_order[i]);
    }
}