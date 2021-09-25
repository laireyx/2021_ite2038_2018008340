#include <iostream>
#include "file.h"

int main() {
    file_open_database_file("db");
    
    std::cout << file_alloc_page() << std::endl;
    std::cout << file_alloc_page() << std::endl;
    std::cout << file_alloc_page() << std::endl;
    std::cout << file_alloc_page() << std::endl;

    file_free_page(4);
    file_free_page(3);
    file_free_page(2);
    file_free_page(1);
    
    file_close_database_file();
    return 0;
}