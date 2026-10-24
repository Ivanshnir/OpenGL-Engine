cmake_minimum_required(VERSION 3.10)  
project(MyEngine VERSION 0.1.2)
find_package(OpenGL REQUIRED)

# Build the GLFW copy stored in this project.
add_subdirectory(../../modules/glfw "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")
file(GLOB helloworld_SRC CONFIGURE_DEPENDS
     "../../../user/Scripts/*.h"
     "../../../user/Scripts/*.cpp"
)
add_executable(MyEngine 
    ../../main/main.cpp
    ../../core/debugger/debugger.cpp
    ../../core/scripts/scripts.cpp
    ../../core/render/managers.cpp
    ../../core/object/object.cpp
    ../../core/render/window.cpp
    ../../modules/glad/glad.c
    ${helloworld_SRC}
    ) 


target_include_directories(MyEngine PRIVATE
    
    ../../modules/glad
    ../../modules/KHR
    ../../modules/GLFW/include
    ../../modules/glm
    ../../modules/MyEngine
    )

target_compile_features(MyEngine PRIVATE cxx_std_20)
target_link_libraries(MyEngine PRIVATE glfw OpenGL::GL)
