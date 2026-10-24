#pragma once
#define GLFW_INCLUDE_NONE
#include <glad.h>
#include <GLFW/glfw3.h>
#include <string>
class Window{
    public:
    void Create(std::string title,int height, int width);
    const bool Ready() const{ return _is_ready; };
    GLFWwindow * GetWindow () {return window;};
    private:
    static void _framebuffer_size_callback(GLFWwindow *, int, int);
    GLFWwindow * window = nullptr;
    inline static const GLFWwindow * main_window = nullptr;
    bool _is_ready = false;
    bool _is_main = false;
    int _height;
    int _width;
    std::string _title;
    
};