#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
static unsigned int frame_cap = 144;
inline float deltaTime = 0.0f;
GLFWwindow * window_creating(int height, int width, const std::string & name); // Creates Window with all initializing and glad connecting,framebuffing_size
void framebuffer_size_callback(GLFWwindow * window, int width, int height); // changes glViewport after resizing window
void Update();
void Start();
void frame_process_window(GLFWwindow * window);
void frame_texture_attach();
void texture_attach();
void setup(GLFWwindow * window);
