add_subdirectory(display)
add_subdirectory(movement)
add_subdirectory(config)

add_executable(life-of-boids main.cpp coupled.cpp main.h)
target_include_directories(life-of-boids
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/config
        display/
        movement/
        config/)
target_link_libraries(life-of-boids PUBLIC display movement config)
