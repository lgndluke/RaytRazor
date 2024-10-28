cmake_minimum_required(VERSION 3.3)
project(RaytRazor)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

# Including GLFW
add_subdirectory(Libraries/glfw)
include_directories(Libraries/glfw/include)

# Including GLAD
include_directories(Libraries/glad/include)

set(SOURCE_FILES main.cpp Libraries/glad/src/glad.c)

add_executable(RaytRazor ${SOURCE_FILES})

# Linking GLFW and OpenGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})