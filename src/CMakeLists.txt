FIND_PACKAGE(glfw3 3.3 REQUIRED) # REQUIRED means : "fatal if not found"
FIND_PACKAGE(glad REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

add_library(common common.cpp common.hpp)
#add_library(display display/display.cpp display/display.hpp)
#add_library(libGLX display/glx/glx.cpp display/glx/glx.hpp)

add_subdirectory(display)
add_subdirectory(movement)

<<<<<<< HEAD
add_subdirectory(movement)
=======
add_executable(life-of-boids main.cpp)
target_include_directories(life-of-boids PUBLIC display/)
target_link_libraries(life-of-boids PUBLIC display common)
>>>>>>> d4937db52f7e71e81891a858a2fd228dcbf8c4e1
