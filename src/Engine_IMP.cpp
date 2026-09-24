#pragma once
#include "Engine.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#define DEBUG_MODE
#define DEBUG_MSG "[" << __FUNCTION__ << " : " << __LINE__ << "]"
#define DEBUG_END_FUNC() std::cout << "[DEBUG]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << " function finished\n";
#define DEBUG_START_FUNC() std::cout << "[DEBUG]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << " function called\n";
#define DEBUG_WARN(x) std::cout << "[DEBUG][WARN]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << x << "\n";
GLFWwindow * window_creating(int height, int width, const std::string & name){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    GLFWwindow * window;
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




const std::string Shader::_shader_read(const std::string & path){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    std::fstream file(path);
    if(!file.is_open()){
        return "";
    }
    std::string Shader_Data;
    getline(file, Shader_Data, '\0');
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
    return Shader_Data;
}

const std::string * Shader::_shader_compile_log(unsigned int id, const char * const settings_data){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int success; 
    char info_log[512];
    glShaderSource(id, 1, &settings_data, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(id, 512, NULL, info_log);
        std::string * buffer = new std::string(SHADER_COMPILE_ENTIRE_ADDITION + std::string(info_log)); // Pointer which can leak if not clean
        #ifdef DEBUG_MODE
        DEBUG_WARN(std::string(info_log));
        #endif
        return buffer;
    }
    else {
        #ifdef DEBUG_MODE
        DEBUG_END_FUNC();
        #endif
        return nullptr;
    }

}
const std::string * Shader::_shader_link(const unsigned int shaders[], const unsigned int shader_program){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
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
        #ifdef DEBUG_MODE
        DEBUG_WARN("shader_link returns log");
        #endif
        return buffer; // Pointer which can leak if not clean
    }else{
        #ifdef DEBUG_MODE
        DEBUG_END_FUNC();
        #endif
        return nullptr;
    }
}
unsigned int Shader::_last_id = 0;
Shader::Shader(const std::string & vertext_shader_path, const std::string & fragment_shader_path) {
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    debug_message = "";
    current_shader_programm = nullptr;
    const std::string vertices_setting_data = _shader_read(vertext_shader_path);
    const std::string fragment_setting_data = _shader_read(fragment_shader_path);
    if(vertices_setting_data == "" || fragment_setting_data == ""){
        debug_message += SHADER_READ_ERROR;
        #ifdef DEBUG_MODE
        DEBUG_WARN(SHADER_READ_ERROR);
        #endif
        return;
    } 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    bool mini_check = false;
    const std::string * res_vert = _shader_compile_log(vertexShader, vertices_setting_data.c_str());
    if(res_vert != nullptr){
        debug_message += "\n[VERTEX SHADER]" + *res_vert + "\n";
        #ifdef DEBUG_MODE
        DEBUG_WARN(*res_vert);
        #endif
        mini_check = true;
    }
    delete res_vert;
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string * res_frag = _shader_compile_log(fragShader, fragment_setting_data.c_str());
    if(res_frag != nullptr){
        #ifdef DEBUG_MODE
        DEBUG_WARN(*res_frag);
        #endif
        debug_message += "\n[FRAGMENT SHADER]" + *res_frag + "\n";
        glDeleteShader(vertexShader);
        delete res_frag;
        return; 
    }
    else if(mini_check){
        delete res_frag;
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
        delete current_shader_programm;
        current_shader_programm = nullptr;
        #ifdef DEBUG_MODE
        DEBUG_WARN(*res_apply);
        #endif
        return;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
bool Shader::use_shader(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(current_shader_programm != nullptr){
        glUseProgram(*current_shader_programm);
        #ifdef DEBUG_MODE
        DEBUG_END_FUNC();
        #endif
        return true;
    }
    else{
        debug_message += SHADER_USING_FAIL;
        #ifdef DEBUG_MODE
        DEBUG_WARN(SHADER_USING_FAIL);
        #endif
        return false;
    }
    
}
Shader::~Shader(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(current_shader_programm != nullptr){
        glDeleteProgram(*current_shader_programm);
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetMat4(const std::string &name, GLfloat * value, GLboolean Transpose){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Mat4Loc = glGetUniformLocation(ID(), name.c_str());    
    glUniformMatrix4fv(Mat4Loc, 1, Transpose,value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetMat4(const std::string  &name, GLdouble * value, GLboolean Transpose){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Mat4Loc = glGetUniformLocation(ID(), name.c_str());
    glUniformMatrix4dv(Mat4Loc, 1, Transpose, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetInt(const std::string  &name, GLint value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int IntLoc = glGetUniformLocation(ID(), name.c_str());
    glUniform1i(IntLoc, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetUInt(const std::string  &name, GLuint value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif 
    int UIntLoc = glGetUniformLocation(ID(), name.c_str());
    glUniform1ui(UIntLoc, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetFloat(const std::string  &name, GLfloat value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int FloatLoc = glGetUniformLocation(ID(), name.c_str());
    glUniform1f(FloatLoc, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetDouble(const std::string  &name, GLdouble value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int DoubleLoc = glGetUniformLocation(ID(), name.c_str());
    glUniform1d(DoubleLoc, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec4(const std::string  &name, GLint * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec4Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform4iv(Vec4Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec4(const std::string  &name, GLfloat * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec4Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform4fv(Vec4Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec4(const std::string  &name, GLuint * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec4Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform4uiv(Vec4Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec4(const std::string  &name, GLdouble * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif 
    int Vec4Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform4dv(Vec4Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec3(const std::string  &name, GLint * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec3Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform3iv(Vec3Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec3(const std::string  &name, GLfloat * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec3Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform3fv(Vec3Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec3(const std::string  &name, GLuint * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec3Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform3uiv(Vec3Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec3(const std::string  &name, GLdouble * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec3Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform3dv(Vec3Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec2(const std::string  &name, GLint * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec2Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform2iv(Vec2Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec2(const std::string  &name, GLfloat * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec2Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform2fv(Vec2Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec2(const std::string  &name, GLuint * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec2Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform2uiv(Vec2Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec2(const std::string  &name, GLdouble * value){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    int Vec2Loc = glGetUniformLocation(ID(), name.c_str());
    glUniform2dv(Vec2Loc, 1, value);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
const unsigned int Shader::ID() const{
    return *current_shader_programm;
}
std::string Shader::get_debug(){
    return debug_message;
}
GameObject::GameObject(std::vector<float> & vertices, std::vector<unsigned int> & trises, Vector3 * origin){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    this->SetVertices(vertices);
    this->SetTrises(trises);
    _obj_transform = glm::mat4(1.0f);
    _obj_transform = glm::translate(_obj_transform, glm::vec3(origin->x, origin->y, origin->z));
    this->Apply();
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
GameObject GameObject::Cube(Vector3 * origin){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    std::vector<float> cube_vertices =         {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,    // top left 
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f    // top left         
    };
    std::vector<unsigned int> cube_trises = {
        0, 1, 3,
        1, 2, 3,
        4, 1, 0,
        4, 5, 1,
        6, 5, 1,
        6, 1, 2,
        7, 6, 2,
        7, 2, 3,
        4, 5, 6,
        4, 6, 7,
        4, 0, 3,
        3, 7, 4


    };
    GameObject NewGameObject = GameObject(cube_vertices, cube_trises,origin);
    
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
    return NewGameObject;
}
GameObject::GameObject(Vector3 * origin){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _obj_transform = glm::mat4(1.0f);
    _obj_transform = glm::translate(_obj_transform, glm::vec3(origin->x, origin->y, origin->z));
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::SetTrises(std::vector<unsigned int> & trises){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(!trises.empty()){
        _tris = trises;
    }
    
    
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::SetVertices(std::vector<float> & vertices){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(!vertices.empty()){
        _vertices = vertices;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
Vector3::Vector3(float _x, float _y, float _z){
    x = _x;
    y = _y;
    z = _z;
}
Vector3::Vector3(){
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}
void GameObject::_load_buffs(GLint BufferVertsFlag, GLint BufferTrisFlag){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _tris.size() * sizeof(_tris[0]), _tris.data(), GL_STATIC_DRAW);
    
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
GameObject::~GameObject(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::Apply(GLint OpenGLCloseFlag, GLint OpenGLFarFlag){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _load_buffs(OpenGLCloseFlag, OpenGLFarFlag);
    _load_attributes();
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::_load_attributes(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    // position attribute

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

const unsigned int GameObject::GET_VAO() const{
    return VAO;
}
const unsigned int GameObject::GET_VBO() const{
    return VBO;
}
const unsigned int GameObject::GET_EBO() const{
    return EBO;
}
void GameObject::Show(Shader * ourShader){
    ourShader->SetMat4("model", glm::value_ptr(_obj_transform), false);
    glBindVertexArray(this->GET_VAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameObject::Translate(Vector3 & TransitionalVector){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::Scale(Vector3 & ScaleVector){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::Rotate(Vector3 & RotationalVector, float & angle){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _obj_transform = glm::rotate(_obj_transform, glm::radians(angle), glm::vec3(RotationalVector.x, RotationalVector.y, RotationalVector.z));
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}