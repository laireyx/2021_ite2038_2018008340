#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include "../file.h"

TEST(SequentialTest, SequentialAllocateTest) {
    file_open_database_file("db/sequential_allocate.db");
    EXPECT_EQ(1, file_alloc_page());
    EXPECT_EQ(2, file_alloc_page());
    EXPECT_EQ(3, file_alloc_page());
    EXPECT_EQ(4, file_alloc_page());

    file_free_page(4);
    file_free_page(3);
    file_free_page(2);
    file_free_page(1);
    file_close_database_file();
}

TEST(RandomTest, RandomAllocateTest) {
    file_open_database_file("db/random_allocate.db");

    srand(time(NULL));

    constexpr int test_count = 2048;
    int test_order[test_count];

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

    for(int i = 0; i < test_count; i++) {
        file_alloc_page();
    }
    
    for(int i = 0; i < test_count; i++) {
        file_free_page(test_order[i]);
    }

    for(int i = 0; i < test_count; i++) {
        EXPECT_EQ(test_order[test_count - 1 - i], file_alloc_page());
    }

    for(int i = 1; i < test_count; i++) {
        file_free_page(test_order[i]);
    }
    file_close_database_file();
}