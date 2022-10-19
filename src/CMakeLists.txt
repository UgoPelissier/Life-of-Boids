FIND_PACKAGE(glfw3 3.3 REQUIRED) # REQUIRED means : "fatal if not found"
FIND_PACKAGE(glad REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

add_subdirectory(display)
add_subdirectory(movement)
add_subdirectory(config)

add_executable(life-of-boids main.cpp)
target_include_directories(life-of-boids PUBLIC display/ movement/ config/)
target_link_libraries(life-of-boids PUBLIC display movement config)
