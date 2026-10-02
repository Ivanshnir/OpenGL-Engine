#ifndef ENGINE_CORE_H_ 
#define ENGINE_CORE_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include <iostream>
#include <typeinfo>
#include <fstream>
#include <bits/stdc++.h>
#include "linked_libraries/img_reading/stb_image.h"
#include "../modules/GameObject/GameObject.hpp"
#include "../modules/Shader/Shader.hpp"
#define GLFW_INIT_ERROR "FAILED TO INITIALISE GLFW"
#define GLAD_ATTACHING_ERROR "FAILED TO ATTACH GLAD"
#define SHADER_READ_ERROR "FAILED TO READ A SHADER FROM THE FILE(TIP: CHECK THE PATH)"
#define SHADER_COMPILE_ENTIRE_ADDITION "SHADER COMPILING ERROR! DETAILS: "
#define SHADER_LINK_ERROR "SHADER LINKING WENT WRONG! DETAILS:  "
#define SHADER_USING_FAIL "FAILED TO USE SHADER PROGRAM (P.S. TRY TO RECONSTRUCT SHADER)"
#define IMAGE_LOAD_ERROR "FAILED TO LOAD IMAGE"
#define IMAGE_BROKEN  "IMAGE LOADED BUT CAN`T BE USED"
#define Vector3 glm::vec3
#define DEFAULT_SHADER_VERTEX_PATH 
#define DEFAULT_SHADER_FRAGMENT_PATH 
inline float deltaTime = 0.0f;
// LOGGING-DEBUGGING PART
GLFWwindow * window_creating(int height, int width, const std::string & name); // Creates Window with all initializing and glad connecting,framebuffing_size
void framebuffer_size_callback(GLFWwindow * window, int width, int height); // changes glViewport after resizing window
void Update();
void Start();
void frame_process_window(GLFWwindow * window);
void frame_texture_attach();
void texture_attach();
void setup(GLFWwindow * window);
#endif