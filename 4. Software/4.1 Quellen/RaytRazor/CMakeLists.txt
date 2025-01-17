cmake_minimum_required(VERSION 3.25.0)
project(RaytRazor)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    # Windows specific compiler flags.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -Ofast -fopenmp")
else ()
    # Linux/MacOS specific compiler flags. -> Functionality has not yet been tested.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -Ofast")
endif ()

set(SDL2_PATH "Utility/External_Libraries/SDL2-2.30.9/x86_64-w64-mingw32")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Utility/External_Libraries/SDL2-2.30.9/x86_64-w64-mingw32/lib/cmake/SDL2)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Include Boost
include_directories(Utility/External_Libraries/boost)
include_directories(Utility/External_Libraries/boost/boost/uuid)

# Include GLM
include_directories(Utility/External_Libraries/glm)

# Include NanoGUI
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_SHARED OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

add_subdirectory(Utility/External_Libraries/nanogui)
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

add_definitions(${NANOGUI_EXTRA_DEFS})
include_directories(${NANOGUI_EXTRA_INCS})

include_directories(Utility/External_Libraries/nanogui/include)

# Include nlohmann
include_directories(Utility/External_Libraries/nlohmann)

# Include STB_Image
include_directories(Utility/External_Libraries/stb_image)

# Include SDL2
include_directories( ${SDL2_INCLUDE_DIR} )

set(SOURCE_FILES
        main.cpp
        Utility/Logger/Logger.cpp
        Utility/Logger/Logger.h
        Import/Importers/Object/Object_Importer.cpp
        Import/Importers/Object/Object_Importer.h
        Parsing/Json_Parser.cpp
        Parsing/Json_Parser.h
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
        Raytracer/RT_App.h
        Raytracer/RT_Image.h
        Raytracer/RT_App.cpp
        Raytracer/RT_Image.cpp
        Raytracer/RT_Camera.h
        Raytracer/RT_Camera.cpp
        Raytracer/RT_geometricTransform.h
        Raytracer/RT_geometricTransform.cpp
        Raytracer/RT_Light.h
        Raytracer/RT_Light.cpp
        Raytracer/RT_Matrix.h
        Raytracer/RT_Object.h
        Raytracer/RT_Object.cpp
        Raytracer/RT_Plane.h
        Raytracer/RT_Plane.cpp
        Raytracer/RT_PointLight.h
        Raytracer/RT_PointLight.cpp
        Raytracer/RT_Ray.h
        Raytracer/RT_Ray.cpp
        Raytracer/RT_Scene.h
        Raytracer/RT_Scene.cpp
        Raytracer/RT_Sphere.h
        Raytracer/RT_Sphere.cpp
        Raytracer/RT_Vector.h
        UI/Widget/TreeView_Widget.cpp
        UI/Widget/TreeView_Widget.h
        UI/Widget/MenuBar_Widget.cpp
        UI/Widget/MenuBar_Widget.h
        Shaders/Vertex/Vertex_Shader.cpp
        Shaders/Vertex/Vertex_Shader.h
        Shaders/Fragment/Fragment_Shader.cpp
        Shaders/Fragment/Fragment_Shader.h
        UI/Widget/ComponentAttributes_Widget.cpp
        UI/Widget/ComponentAttributes_Widget.h
        UI/utility/Custom_Label.cpp
        UI/utility/Custom_Label.h
        Converter/Converter.cpp
        Converter/Converter.h
        Raytracer/RT_Triangle.h
        Raytracer/RT_Triangle.cpp
)

add_executable(RaytRazor ${SOURCE_FILES})

# Copy SDL.dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/resources/SDL2.dll"
        ${CMAKE_BINARY_DIR}/SDL2.dll)

# Linking libraries
target_link_libraries(RaytRazor nanogui nanogui-obj ${NANOGUI_EXTRA_LIBS} ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})