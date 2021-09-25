cmake_minimum_required(VERSION 3.14)
project(dbms)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e4717df71a4f45bf9f0ac88c6cd9846a0bc248dd.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# GoogleTest stuff
enable_testing()

add_executable(
  my_test
  file.cc
  test/test.cc
)
target_link_libraries(
  my_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(my_test)