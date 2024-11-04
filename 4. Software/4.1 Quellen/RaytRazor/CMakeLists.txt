cmake_minimum_required(VERSION 3.3)
project(RaytRazor)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

# Including GLFW
add_subdirectory(Utility/External_Libraries/glfw)
include_directories(Utility/External_Libraries/glfw/include)

# Including GLAD
include_directories(Utility/External_Libraries/glad/include)

# Including GLM
include_directories(Utility/External_Libraries/glm)

set(SOURCE_FILES main.cpp Utility/External_Libraries/glad/src/glad.c
        Utility/Logger/Logger.cpp
        Utility/Logger/Logger.h)

add_executable(RaytRazor ${SOURCE_FILES})

# Linking GLFW and OpenGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})