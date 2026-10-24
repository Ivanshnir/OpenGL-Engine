#include "window.hpp"
#include "../debugger/debugger.hpp"

void Window::Create(std::string name, int height, int width){
    LOG("Creating window", TRACE);
    _title = name;
    _height = height;
    _width = width;
    if(!glfwInit()){
        LOG("GLFW didn`t init", FATAL)
        _is_ready = false;
        return;
    }
    if(main_window == nullptr){
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            LOG("Failed to load glad", ERROR);
            _is_ready = false;
            return;
        }
        glfwSetFramebufferSizeCallback(window, _framebuffer_size_callback);
        main_window = window;
        _is_main = true;
    }
    else{
        window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, const_cast<GLFWwindow *>(main_window));
        glfwMakeContextCurrent(window);
        _is_main = false;
    }

    _is_ready = true;
}
void Window::_framebuffer_size_callback(GLFWwindow * window, int width, int height){
    glViewport(0,0,width,height);
}