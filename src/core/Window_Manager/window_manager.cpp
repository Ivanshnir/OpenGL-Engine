#include "window.hpp"
#include "../../modules/GameObject/GameObject.hpp"
#include "../Render/render_manager.hpp"
#include "../linked_libraries/img_reading/stb_image.h"
#include "..\Debugger\debug.hpp"
#define BACKGROUND_COLOR 0.2f,0.3f, 0.4f, 1.0f
GLFWwindow * window_creating(int height, int width, const std::string & name){
    GLFWwindow * window;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    if(!glfwInit()){
        LOG("Failed to init glfw", FATAL);
        return nullptr;
    }
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        LOG("Failed to load glad", FATAL);
        return nullptr;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    LOG("Window was created succesfull", INFO);
    return window;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height){
    glViewport(0,0, width, height);
}
void frame_process_window(GLFWwindow * window){
    glClearColor(BACKGROUND_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    frame_texture_attach(); 
}
void frame_texture_attach(){

}
void texture_attach(){

}
void setup(GLFWwindow * window){
    float previous_frame = (float)glfwGetTime();
    stbi_set_flip_vertically_on_load(true);
    Start();
    Render_Manager& rend_mng = Render_Manager::GET();
    glEnable(GL_DEPTH_TEST);
    float frame_cap_delay = 1.0f/static_cast<float>(frame_cap);
    float cap_capacitor = 0.0f;
    while(!glfwWindowShouldClose(window)){
        deltaTime = -(previous_frame - (float)glfwGetTime());
        previous_frame = (float)glfwGetTime();
        cap_capacitor += deltaTime;
        if(cap_capacitor > frame_cap_delay){
            deltaTime = cap_capacitor;
            cap_capacitor = 0.0f;
            frame_process_window(window);
            rend_mng.FrameRender();
            Update();
            glfwSwapBuffers(window);
            glfwPollEvents();  
        }
    }
    rend_mng.RemoveAll();
    glfwTerminate();
}
