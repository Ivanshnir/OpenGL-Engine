#include "engine_core.h"

#define BACKGROUND_COLOR 0.2f,0.3f, 0.4f, 1.0f
GLFWwindow * window_creating(int height, int width, const std::string & name){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    GLFWwindow * window;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    if(!glfwInit()){
        std::cout << GLFW_INIT_ERROR;
        return nullptr;
    }
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << GLAD_ATTACHING_ERROR;
        return nullptr;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
    return window;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    glViewport(0,0, width, height);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
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
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    float previous_frame = (float)glfwGetTime();
    stbi_set_flip_vertically_on_load(true);
    Start();
    
    while(!glfwWindowShouldClose(window)){
        deltaTime = previous_frame - (float)glfwGetTime();
        previous_frame = (float)glfwGetTime();
        frame_process_window(window);
        Update();
        #ifdef DEBUG_MODE
        DEBUG_START_FUNC();
        #endif
        for(long i = 0; i < GameObject::ptrs_links.size(); i++){
            (*GameObject::ptrs_links[i]).Show();
        }
        #ifdef DEBUG_MODE
        DEBUG_END_FUNC();
        #endif
        
        glfwSwapBuffers(window);
        glfwPollEvents();  
    }
    glfwTerminate();
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
