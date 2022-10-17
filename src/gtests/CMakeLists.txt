
add_executable(gtest gtest.hpp gtest.cpp)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gtest)
# add_test(NAME Canary COMMAND gtest 2)
# add_test(NAME Canary COMMAND gtest 3)
# add_test(NAME Canary COMMAND gtest 4)
# add_test(NAME Canary COMMAND gtest 5)
# add_test(NAME Canary COMMAND gtest 6)
