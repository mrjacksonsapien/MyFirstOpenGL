cmake_minimum_required(VERSION 3.10)
project(MyFirstOpenGL)

set(CMAKE_CXX_STANDARD 17)

# Add GLFW and GLAD include paths
include_directories(include)
include_directories(external/glfw/include)

# Add GLFW as subdirectory
add_subdirectory(external/glfw)

# Add your executable
add_executable(MyFirstOpenGL main.cpp src/glad.c)

# Link to glfw (target from their CMakeLists)
target_link_libraries(MyFirstOpenGL glfw OpenGL32)