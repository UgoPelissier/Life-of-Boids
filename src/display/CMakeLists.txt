FIND_PACKAGE(glfw3 3.3 REQUIRED) # REQUIRED means : "fatal if not found"
FIND_PACKAGE(glad REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

add_library(display display.cpp display.hpp glx/glx.cpp glx/glx.hpp shaders/points.hpp shaders/points.cpp shaders/lines.hpp shaders/triangle.hpp)
target_link_libraries(display PUBLIC glfw::glfw glad::glad opencv::opencv)