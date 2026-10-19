#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "..\Render\render_manager.hpp"
#define HEIGHT 640
#define WIDTH 500
static unsigned int frame_cap = 144;
inline float deltaTime = 0.0f;

// Creates Window with all initializing and glad connecting,framebuffing_size
 // changes glViewport after resizing window


class Window{
    public:

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window &&) = delete;
    Window& operator=(Window&&) = delete;
    static Window & Create_Window(int height = WIDTH, int width = HEIGHT, const std::string & name = "MyWindow");
    void setup();
    
    private:
    Window(int, int, const std::string & name); 
    void _frame_process_window();
    static void _framebuffer_size_callback(GLFWwindow *,int, int);
    GLFWwindow * window;
    static float WdivHRatio;
    friend Render_Manager;
};
class IOSystem{

};
