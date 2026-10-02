cmake_minimum_required(VERSION 3.10)  
project(MyEngine VERSION 0.1.0)
find_package(OpenGL REQUIRED)

# Build the GLFW copy stored in this project.
add_subdirectory(dependencies/GLFW)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=")
add_executable(MyEngine 
    src/core/main.cpp
    src/core/Debugger/debug.cpp
    src/modules/Shader/Shader.cpp
    src/modules/GameObject/GameObject.cpp
    src/core/linked_libraries/glad/glad.c
    src/core/window_and_engine_manager.cpp
    src/core/MainGameCode.cpp
    src/core/linked_libraries/img_reading/stb_image_imp.cpp
    ) 

target_include_directories(MyEngine PRIVATE
    dependencies
    dependencies/glad
    dependencies/KHR
    dependencies/GLFW/include
    dependencies/glm
    )

target_link_libraries(MyEngine PRIVATE glfw OpenGL::GL)
