add_subdirectory(obstacles)
add_subdirectory(agents)
add_subdirectory(fruits)

add_library(movement object.cpp object.h)
target_link_libraries(movement PUBLIC obstacles agents fruits)
