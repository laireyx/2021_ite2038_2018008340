#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include "../file.h"

constexpr int test_count = 2559;

class BasicFileManagerTest : public ::testing::Test {
protected:
    int test_order[test_count];
    int database_fd = 0;

    void SetUp() override {
        database_fd = file_open_database_file("test.db");
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
    void TearDown() override { }
};

TEST_F(BasicFileManagerTest, FileInitializationTest) {
    headerpage_t header_page;
    
    file_read_page(database_fd, 0, reinterpret_cast<page_t*>(&header_page));

    // 2560 pages is equal to 10MiB(4096B * 2560 = 4KiB * 2560 = 10240KiB = 10MiB)
    EXPECT_EQ(header_page.page_num, 2560);

    struct stat database_stat;
    lstat("test.db", &database_stat);
    EXPECT_EQ(database_stat.st_size, 4096 * 2560);
}

TEST_F(BasicFileManagerTest, PageManagementTest) {
    int first_page_num = file_alloc_page(database_fd);
    int second_page_num = file_alloc_page(database_fd);
    file_free_page(database_fd, first_page_num);

    headerpage_t header_page;
    file_read_page(database_fd, 0, reinterpret_cast<page_t*>(&header_page));

    bool is_freed_correctly = false;
    int current_page_idx = header_page.free_page_idx;
    freepage_t free_page;
    while(current_page_idx != 0) {
        is_freed_correctly = is_freed_correctly || (current_page_idx == first_page_num);
        EXPECT_NE(current_page_idx, second_page_num);

        file_read_page(database_fd, current_page_idx, reinterpret_cast<page_t*>(&free_page));
        current_page_idx = free_page.next_free_idx;
    }
    EXPECT_TRUE(is_freed_correctly);
    
    file_free_page(database_fd, second_page_num);
}

TEST_F(BasicFileManagerTest, PageIOTest) {
    int free_page_num = file_alloc_page(database_fd);

    allocatedpage_t page;
    int* page_data = reinterpret_cast<int*>(page.reserved);

    int random_values[1024] = {};
    for(int i = 0; i < 1024; i++) {
        random_values[i] = rand();
        page_data[i] = random_values[i];
    }

    file_write_page(database_fd, free_page_num, &page);
    file_read_page(database_fd, free_page_num, &page);

    for(int i = 0; i < 1024; i++) {
        EXPECT_EQ(random_values[i], page_data[i]);
    }
    
    file_free_page(database_fd, free_page_num);
}

TEST_F(BasicFileManagerTest, SequentialAllocateTest) {
    int allocation_result[test_count] = { 0, };

    for(int i = 0; i < test_count; i++) {
        allocation_result[i] = file_alloc_page(database_fd);
    }

    for(int i = 0; i < test_count; i++) {
        file_free_page(database_fd, allocation_result[i]);
    }
}

TEST_F(BasicFileManagerTest, RandomAllocateTest) {
    for(int i = 0; i < test_count; i++) {
        file_alloc_page(database_fd);
    }
    
    for(int i = 0; i < test_count; i++) {
        file_free_page(database_fd, test_order[i]);
    }

    for(int i = 0; i < test_count; i++) {
        EXPECT_EQ(test_order[test_count - 1 - i], file_alloc_page(database_fd));
    }

    for(int i = 0; i < test_count; i++) {
        file_free_page(database_fd, test_order[i]);
    }
}