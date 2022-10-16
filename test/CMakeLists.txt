find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(unit_gtest unit_gtest.cpp)
target_link_libraries(unit_gtest GTest::GTest)
target_include_directories(unit_gtest PUBLIC ${CMAKE_SOURCE_DIR}/src)
gtest_discover_tests(unit_gtest)