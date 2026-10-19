#include "GameObject.hpp"
#include "../../core/Debugger/debug.hpp"
#include <algorithm>

const unsigned int Shader::ID() const{
    return *current_shader_programm;
} 

// Setting vars into shaders
void Shader::SetMat4(const std::string &name, const void * value, const UniformType type, GLboolean Transpose){
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return;
    }
        
    int Mat4Loc = glGetUniformLocation(ID(), name.c_str()); 
    switch (type)
    {
        case U_Float:
            glUniformMatrix4fv(Mat4Loc, 1, Transpose,static_cast<const GLfloat*>(value));
            break;
        case U_Double:
            glUniformMatrix4dv(Mat4Loc, 1, Transpose, static_cast<const GLdouble*>(value));
            break;
        default:
            LOG("Type of matrix unknown", WARN);
            break;
    }
}

void Shader::SetVar(const std::string  &name, const void * value, const UniformType type){
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return;
    }
    int Loc = glGetUniformLocation(ID(), name.c_str());
    switch (type)
    {
        case U_Int:
            glUniform1i(Loc, (*static_cast<const GLint*>(value)));
            break;
        case U_Float:
            glUniform1f(Loc, (*static_cast<const GLfloat*>(value)));
            break;
        case U_Uint:
            glUniform1ui(Loc, (*static_cast<const GLuint*>(value)));
            break;
        case U_Double:
            glUniform1d(Loc, (*static_cast<const GLdouble*>(value)));
            break;
        
        default:
            LOG("Type of var unknown", WARN);
            break;
    }
}

void Shader::SetVec4(const std::string  &name, const void * value, const UniformType type){
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return;
    }
    int Loc = glGetUniformLocation(ID(), name.c_str());
    switch (type)
    {
        case U_Int:
            glUniform4iv(Loc, 1,static_cast<const GLint*>(value));
            break;
        case U_Float:
            glUniform4fv(Loc, 1,static_cast<const GLfloat*>(value));
            break;
        case U_Uint:
            glUniform4uiv(Loc, 1,static_cast<const GLuint*>(value));
            break;
        case U_Double:
            glUniform4dv(Loc, 1,static_cast<const GLdouble*>(value));
            break;
        
        default:
            LOG("Type of vec4 unknown", WARN);
            break;
    }
}
void Shader::SetVec3(const std::string  &name, const void * value, const UniformType type){
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return;
    }
    int Loc = glGetUniformLocation(ID(), name.c_str());
    switch (type)
    {
        case U_Int:
            glUniform3iv(Loc, 1,static_cast<const GLint*>(value));
            break;
        case U_Float:
            glUniform3fv(Loc, 1,static_cast<const GLfloat*>(value));
            break;
        case U_Uint:
            glUniform3uiv(Loc, 1,static_cast<const GLuint*>(value));
            break;
        case U_Double:
            glUniform3dv(Loc, 1,static_cast<const GLdouble*>(value));
            break;
        
        default:
            LOG("Type of vec2 unknown", WARN);
            break;
    }
}
void Shader::SetVec2(const std::string  &name, const void * value, const UniformType type){
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return;
    }
    int Loc = glGetUniformLocation(ID(), name.c_str());
    switch (type)
    {
        case U_Int:
            glUniform2iv(Loc, 1,static_cast<const GLint*>(value));
            break;
        case U_Float:
            glUniform2fv(Loc, 1,static_cast<const GLfloat*>(value));
            break;
        case U_Uint:
            glUniform2uiv(Loc, 1,static_cast<const GLuint*>(value));
            break;
        case U_Double:
            glUniform2dv(Loc, 1,static_cast<const GLdouble*>(value));
            break;
        
        default:
            LOG("Type of vec2 unknown", WARN);
            break;
    }
}
Shader * Shader::get_default_shader(){
    if(shaders_pool.size() == 0){
        shaders_pool.emplace_back(new Shader(SHD_DEF_VERT_PATH, SHD_DEF_FRAG_PATH));
    }
    return shaders_pool[0];
}
// private work functions 
const std::string Shader::_shader_read(const std::string & path){
    LOG("", TRACE);
    std::fstream file(path);
    if(!file.is_open()){
        LOG("file not opens", ERROR);
        return "";
    }
    std::string Shader_Data;
    getline(file, Shader_Data, '\0');
    return Shader_Data;
}

bool Shader::_shader_compile_log(unsigned int id, const char * const settings_data){
    int success; 
    char info_log[512];
    glShaderSource(id, 1, &settings_data, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(id, 512, NULL, info_log);
        LOG(info_log, ERROR);
        return false;
    }
    else {

        return true;
    }

}
bool Shader::_shader_link(const unsigned int shaders[], const unsigned int shader_program){
    for(int i = 0; i < 2; i++){
        glAttachShader(shader_program, shaders[i]);
    }
    glLinkProgram(shader_program);
    for(int i = 0; i < 2; i++){
        glDeleteShader(shaders[i]);
    }
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        LOG(info_log, ERROR);
        return false; // Pointer which can leak if not clean
    }else{

        return true;
    }
}

Shader::Shader(const std::string & vertex_shader_path, const std::string & fragment_shader_path): vert_path(vertex_shader_path), frag_path(fragment_shader_path){
    debug_message = "";
    current_shader_programm = nullptr;
    const std::string vertices_setting_data = _shader_read(vert_path);
    const std::string fragment_setting_data = _shader_read(frag_path);
    if(vertices_setting_data == "" || fragment_setting_data == ""){
        LOG("Shaders didnt read", ERROR);
        return;
    } 
    unsigned int vertexShader;
    unsigned int fragShader;
    bool isError = false;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    bool mini_check = false;
    
    if(!(_shader_compile_log(vertexShader, vertices_setting_data.c_str()))){
        isError = true;
        LOG("Shaders compile vertex issue", ERROR);
    }
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if(!(_shader_compile_log(fragShader, fragment_setting_data.c_str()))){
        glDeleteShader(vertexShader);
        isError = true;
        LOG("Shaders compile Frag issue", ERROR);
    }
    auto func = [=, this](){
        current_shader_programm = new unsigned int;
        *current_shader_programm = glCreateProgram();
        unsigned int AllShaders[2] = {vertexShader, fragShader};
        if(!(_shader_link(AllShaders, *current_shader_programm))){
            delete current_shader_programm;
            current_shader_programm = nullptr;
            LOG("Shaders link issue", ERROR);
            return true;
        }else{
            return false;
        }
    };
    if(!isError)
        isError = func();
    if(!isError)
        LOG("Shader - was created succesfull", INFO);
    canBeUsed = true;
    return;
}
Shader::Shader(const Shader & Target){
    debug_message = "";
    current_shader_programm = nullptr;
    const std::string vertices_setting_data = _shader_read(Target.GetVertPath());
    const std::string fragment_setting_data = _shader_read(Target.GetFragPath());
    if(vertices_setting_data == "" || fragment_setting_data == ""){
        LOG("Shaders didnt read", ERROR);
        return;
    } 
    unsigned int vertexShader;
    unsigned int fragShader;
    bool isError = false;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    bool mini_check = false;
    
    if(!(_shader_compile_log(vertexShader, vertices_setting_data.c_str()))){
        isError = true;
        LOG("Shaders compile vertex issue", ERROR);
    }
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if(!(_shader_compile_log(fragShader, fragment_setting_data.c_str()))){
        glDeleteShader(vertexShader);
        isError = true;
        LOG("Shaders compile Frag issue", ERROR);
    }
    auto func = [=, this](){
        current_shader_programm = new unsigned int;
        *current_shader_programm = glCreateProgram();
        unsigned int AllShaders[2] = {vertexShader, fragShader};
        if(!(_shader_link(AllShaders, *current_shader_programm))){
            delete current_shader_programm;
            current_shader_programm = nullptr;
            LOG("Shaders link issue", ERROR);
            return true;
        }else{
            return false;
        }
    };
    if(!isError)
        isError = func();
    if(!isError)
        LOG("Shader - was created succesfull", INFO);
    canBeUsed = true;
    return;
}
const std::string & Shader::GetVertPath() const{
    return vert_path;
}
const std::string & Shader::GetFragPath() const{
    return frag_path;
}
Shader::~Shader(){
    if(current_shader_programm != nullptr){
        LOG("Shader -  will be deleted", INFO);
        glDeleteProgram(*current_shader_programm);
    }
    canBeUsed = false;
}
bool Shader::attach_shader(){
    if(!canBeUsed){
        LOG("Shader can`t be attached", ERROR);
        return false;
    }
    if(current_shader_programm != nullptr){
        glUseProgram(*current_shader_programm);
        return true;
    }
    else{
        LOG("Failed current_shader_programm == nullptr", ERROR);

        return false;
    }
    
}


Shader::operator bool() const{
    LOG("Check null or not - " << canBeUsed, DEBUG);
    return canBeUsed;
}

//overload operators
bool Shader::operator==(const Shader & other){
    LOG("Shaer operator == ", DEBUG);
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return false;
    }
    if(&other != nullptr)
    {
        if((*this).ID() == other.ID()) return true;
        return false;
    }else{
        LOG("This Shader compares with nullptr", WARN);
        return false;
    }
    
}
bool Shader::operator==(const Shader * other){
    if(!canBeUsed){
        LOG("Shader can`t be used", ERROR);
        return false;
    }
    if(other != nullptr)
    {
        if((*this).ID() == (*other).ID()) return true;
        return false;
    }else{
        LOG("This Shader compares with nullptr", WARN);
        return false;
    }
    
    
}




