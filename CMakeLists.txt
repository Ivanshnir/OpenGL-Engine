cmake_minimum_required(VERSION 3.10)  
project(MyEngine VERSION 0.1.0)
  
add_executable(MyEngine 
    src/main.cpp
    src/Engine_IMP.cpp
    src/glad.c
    src/stb_image_imp.cpp
    ) 
find_package(glfw3 REQUIRED) 
target_link_libraries(MyEngine PRIVATE glfw)
