find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(unit_gtest unit_gtest.cpp ../src/main.h ../src/coupled.cpp)
target_link_libraries(unit_gtest movement display config GTest::GTest)
target_include_directories(unit_gtest
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/display
        ${CMAKE_SOURCE_DIR}/src/movement
        ${CMAKE_SOURCE_DIR}/src/movement/obstacles
        ${CMAKE_SOURCE_DIR}/src/movement/fruits
        ${CMAKE_SOURCE_DIR}/src/movement/agents
        ${CMAKE_SOURCE_DIR}/src/config)
gtest_discover_tests(unit_gtest)