#pragma once
#include "Engine.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
GLFWwindow * window_creating(int height, int width, const char * name){
    GLFWwindow * window;
    if(!glfwInit()){
        std::cout << GLFW_INIT_ERROR;
        return nullptr;
    }
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << GLAD_ATTACHING_ERROR;
        return nullptr;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height){
    glViewport(0,0, width, height);
}




const std::string Shader::_shader_read(const char * path){
    std::fstream file(path);
    if(!file.is_open()){
        return "";
    }
    std::string Shader_Data;
    getline(file, Shader_Data, '\0');
    return Shader_Data;
}

const std::string * Shader::_shader_compile_log(unsigned int id, const char * settings_data){
    int success; 
    char info_log[512];
    glShaderSource(id, 1, &settings_data, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(id, 512, NULL, info_log);
        std::string * buffer = new std::string(SHADER_COMPILE_ENTIRE_ADDITION + std::string(info_log));
        return buffer;
    }
    else {
        return nullptr;
    }
}
const std::string * Shader::_shader_link(const unsigned int shaders[], const unsigned int shader_program){
    for(int i = 0; i < sizeof(shaders)/shaders[0]; i++){
        glAttachShader(shader_program, shaders[i]);
    }
    glLinkProgram(shader_program);
    for(int i = 0; i < sizeof(shaders)/shaders[0]; i++){
        glDeleteShader(shaders[i]);
    }
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::string * buffer = new std::string(SHADER_LINK_ERROR + std::string(info_log));
        return buffer;
    }else{
        return nullptr;
    }
}
unsigned int Shader::_last_id = 0;
Shader::Shader(const char * vertext_shader_path, const char * fragment_shader_path) {
    debug_message = "";
    current_shader_programm = nullptr;
    const std::string vertices_setting_data = _shader_read("/home/ivan/Documents/Scripts/MyEngine0.1.0/src/Vertex_Shader.glsl");
    const std::string fragment_setting_data =  _shader_read("/home/ivan/Documents/Scripts/MyEngine0.1.0/src/Fragment_Shader.glsl");
    if(vertices_setting_data == "" || fragment_setting_data == ""){
        debug_message += SHADER_READ_ERROR;\
        return;
    } 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    bool mini_check = false;
    const std::string * res_vert = _shader_compile_log(vertexShader, vertices_setting_data.c_str());
    if(res_vert != nullptr){
        debug_message += "\n[VERTEX SHADER]" + *res_vert + "\n";
        mini_check = true;
    }
    delete res_vert;
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string * res_frag = _shader_compile_log(fragShader, fragment_setting_data.c_str());
    if(res_frag != nullptr){
        debug_message += "\n[FRAGMENT SHADER]" + *res_frag + "\n";
        glDeleteShader(vertexShader);
        return; 
    }

    if(mini_check){
        return;
    }
    delete res_frag;
    unsigned int AllShaders[2] = {vertexShader, fragShader};
    current_shader_programm = new unsigned int;
    *current_shader_programm = glCreateProgram();
    
    const std::string * res_apply = _shader_link(AllShaders, *current_shader_programm);
    if(res_apply != nullptr){
        debug_message = *res_apply + '\n';
        delete res_apply;
        current_shader_programm = nullptr;
        return;
    }
}
bool Shader::use_shader(){
    if(current_shader_programm != nullptr){
        glUseProgram(*current_shader_programm);
        return true;
    }
    else{
        debug_message += SHADER_USING_FAIL;
        return false;
    }
    
}
Shader::~Shader(){
    if(current_shader_programm != nullptr){
        glDeleteProgram(*current_shader_programm);
    }
    
}
const unsigned int Shader::ID() const{
    return *current_shader_programm;
}
std::string Shader::get_debug(){
    return debug_message;
}