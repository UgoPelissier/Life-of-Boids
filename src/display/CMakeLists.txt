FIND_PACKAGE(glfw3 3.3 REQUIRED) # REQUIRED means : "fatal if not found"
FIND_PACKAGE(glad REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

add_library(libGLX glx/glx.cpp glx/glx.hpp)
target_link_libraries(libGLX PUBLIC glfw::glfw glad::glad opencv::opencv)

add_executable(life-of-boids display.cpp)
target_link_libraries(life-of-boids libGLX)
