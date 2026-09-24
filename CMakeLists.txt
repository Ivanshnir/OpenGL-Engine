cmake_minimum_required(VERSION 3.10)  
project(MyEngine VERSION 0.1.0)
find_package(OpenGL REQUIRED)

# Build the GLFW copy stored in this project.
add_subdirectory(dependencies/GLFW)

add_executable(MyEngine 
    src/main.cpp
    src/Engine_IMP.cpp
    src/glad.c
    src/stb_image_imp.cpp
    ) 

target_include_directories(MyEngine PRIVATE
    dependencies
    dependencies/glad
    dependencies/KHR
    dependencies/GLFW/include
    dependencies/glm
    )

target_link_libraries(MyEngine PRIVATE glfw OpenGL::GL)
