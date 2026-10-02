#include "Shader.hpp"
const unsigned int Shader::ID() const{
    return *current_shader_programm;
}

// Setting vars into shaders
void Shader::SetMat4(const std::string &name, const void * value, const UniformType type, GLboolean Transpose){
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
// private work functions 
const std::string Shader::_shader_read(const std::string & path){
    std::fstream file(path);
    if(!file.is_open()){
        LOG("file not opens", WARN);
        return "";
    }
    std::string Shader_Data;
    getline(file, Shader_Data, '\0');
    return Shader_Data;
}

const std::string * Shader::_shader_compile_log(unsigned int id, const char * const settings_data){
    int success; 
    char info_log[512];
    glShaderSource(id, 1, &settings_data, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(id, 512, NULL, info_log);

        std::string * buffer = new std::string(std::string(info_log)); // Pointer which can leak if not clean
        LOG(info_log, FATAL);
        return buffer;
    }
    else {

        return nullptr;
    }

}
const std::string * Shader::_shader_link(const unsigned int shaders[], const unsigned int shader_program){
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
        std::string * buffer = new std::string(std::string(info_log));
        LOG(info_log, FATAL);
        return buffer; // Pointer which can leak if not clean
    }else{

        return nullptr;
    }
}
Shader::Shader(const std::string & vertext_shader_path, const std::string & fragment_shader_path) {
    Shader::ptrs_links.push_back(this);
    debug_message = "";
    current_shader_programm = nullptr;
    const std::string vertices_setting_data = _shader_read(vertext_shader_path);
    const std::string fragment_setting_data = _shader_read(fragment_shader_path);
    if(vertices_setting_data == "" || fragment_setting_data == ""){
        LOG("Shaders didnt read", FATAL);
        return;
    } 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    bool mini_check = false;
    const std::string * res_vert = _shader_compile_log(vertexShader, vertices_setting_data.c_str());
    if(res_vert != nullptr){
        LOG("Compilation vertex shader failed", FATAL);
        mini_check = true;
    }
    delete res_vert;
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string * res_frag = _shader_compile_log(fragShader, fragment_setting_data.c_str());
    if(res_frag != nullptr){
        LOG("Compilation fragment shader failed", FATAL);
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
        LOG("Linking shaders fails "+*res_apply, FATAL);
        return;
    }
    LOG("Shader - "+std::to_string(ID())+" was created succesfull", INFO);
}
Shader::~Shader(){
    if(current_shader_programm != nullptr){
        LOG("Shader - "+std::to_string(ID())+" will be deleted", INFO);
        glDeleteProgram(*current_shader_programm);
    }
    auto it = std::find(Shader::ptrs_links.begin(), Shader::ptrs_links.end(),   this);
    if(it != Shader::ptrs_links.end()){
        LOG("ptrs_links erased by 1, now size - "+std::to_string(Shader::ptrs_links.size()), INFO);
        Shader::ptrs_links.erase(it);
        
    }
}
bool Shader::use_shader(){
    if(current_shader_programm != nullptr){
        glUseProgram(*current_shader_programm);

        return true;
    }
    else{
        LOG("Failed current_shader_programm == nullptr", FATAL);

        return false;
    }
    
}

std::string Shader::get_debug(){
    return debug_message;
}



//overload operators
bool Shader::operator==(const Shader & other){
    if(this->ID() == other.ID()) return true;
    return false;
}
bool Shader::operator==(const Shader * other){
    if(this->ID() == (*other).ID()) return true;
    return false;
}
