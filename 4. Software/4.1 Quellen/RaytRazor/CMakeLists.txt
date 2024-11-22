cmake_minimum_required(VERSION 3.3)
project(RaytRazor)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

# Include GLFW
add_subdirectory(Utility/External_Libraries/glfw)
include_directories(Utility/External_Libraries/glfw/include)

# Include GLAD
include_directories(Utility/External_Libraries/glad/include)

# Include GLM
include_directories(Utility/External_Libraries/glm)

# Include STB_Image
include_directories(Utility/External_Libraries/stb_image)

# Include Eigen
include_directories(Utility/External_Libraries/eigen-3.4.0)

# Include NanoVG
include_directories(Utility/External_Libraries/nanovg/src)

# Include NanoGUI
include_directories(Utility/External_Libraries/nanogui)

# Include Boost
include_directories(Utility/External_Libraries/boost)
include_directories(Utility/External_Libraries/boost/boost/uuid)

set(SOURCE_FILES main.cpp Utility/External_Libraries/glad/src/glad.c
        Utility/Logger/Logger.cpp
        Utility/Logger/Logger.h
        Import/Importers/Object/Object_Importer.cpp
        Import/Importers/Object/Object_Importer.h
        Parsing/Json_Parser.cpp
        Parsing/Json_Parser.h
        UI/Scenes/Base_Scene.cpp
        UI/Scenes/Base_Scene.h
        UI/Scenes/Main/Main_Scene.cpp
        UI/Scenes/Main/Main_Scene.h
        UI/Scenes/Settings/Settings_Scene.cpp
        UI/Scenes/Settings/Settings_Scene.h
        Import/Importers/Material/Material_Importer.cpp
        Import/Importers/Material/Material_Importer.h
        Components/Base_Component.cpp
        Components/Base_Component.h
        Components/Entities/Render_Component.cpp
        Components/Entities/Render_Component.h
        Components/Camera/Camera_Component.cpp
        Components/Camera/Camera_Component.h
        Components/Lighting/Light_Component.cpp
        Components/Lighting/Light_Component.h
        Import/Resources/Base_Resource.cpp
        Import/Resources/Base_Resource.h
        Import/Resources/Material/Material_Resource.cpp
        Import/Resources/Material/Material_Resource.h
        Import/Resources/Object/Object_Resource.cpp
        Import/Resources/Object/Object_Resource.h
        UI/SceneManager/Scene_Manager.cpp
        UI/SceneManager/Scene_Manager.h)

add_executable(RaytRazor ${SOURCE_FILES})

# Linking GLFW and OpenGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})