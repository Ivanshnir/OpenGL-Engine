#include "window.hpp"
#include "../../modules/API/GameLoopsRegistry.hpp"
#include "../../modules/GameObject/GameObject.hpp"
#include "../Render/render_manager.hpp"
#include "../linked_libraries/img_reading/stb_image.h"
#include "..\Debugger\debug.hpp"
#include "../../USER/Hz.cpp"
#define BACKGROUND_COLOR 0.2f,0.3f, 0.4f, 1.0f
float Window::WdivHRatio = 1.0f;
Window& Window::Create_Window(int height, int width, const std::string & name){
    static Window instance(height, width, name);
    return instance;
}
Window::Window(int height, int width, const std::string & name){
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    if(!glfwInit()){
        LOG("Failed to init glfw", ERROR);
       throw(std::runtime_error("Failed to init glfw"));
    }
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        LOG("Failed to load glad", ERROR);
        throw(std::runtime_error("Failed to load glad"));
    }
    glfwSetFramebufferSizeCallback(window,_framebuffer_size_callback);
    WdivHRatio = static_cast<float>(width)/static_cast<float>(height);
    LOG("Window was created succesfull", INFO);
}

void Window::_framebuffer_size_callback(GLFWwindow * window,int width, int height){
    WdivHRatio = static_cast<float>(width)/static_cast<float>(height);
    glViewport(0,0, width, height);
}
void Window::_frame_process_window(){
    glClearColor(BACKGROUND_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::setup(){
    float previous_frame = (float)glfwGetTime();
    stbi_set_flip_vertically_on_load(true);
    Render_Manager& rend_mng = Render_Manager::GET();
    struct funcs_to_reg
    {
        std::vector<void(*)()> Awake_Funcs;
        std::vector<void(*)()> Starts_Funcs;
        std::vector<void(*)()> Update_Funcs;
    };
    funcs_to_reg register_funcs;
    MySample monoFirstScript; 
    glEnable(GL_DEPTH_TEST);
    float frame_cap_delay = 1.0f/static_cast<float>(frame_cap);
    float cap_capacitor = 0.0f;
    monoFirstScript.Awake();
    monoFirstScript.Start();
    while(!glfwWindowShouldClose(window)){
        deltaTime = -(previous_frame - (float)glfwGetTime());
        previous_frame = (float)glfwGetTime();
        cap_capacitor += deltaTime;
        if(cap_capacitor > frame_cap_delay){
            deltaTime = cap_capacitor;
            cap_capacitor = 0.0f;
            _frame_process_window();
            rend_mng.FrameRender();
            monoFirstScript.Update();
            glfwSwapBuffers(window);
            glfwPollEvents();  
        }
    }
    rend_mng.RemoveAll();
    glfwTerminate();
}
