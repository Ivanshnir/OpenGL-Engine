#include "engine_core.h"

const unsigned int Shader::ID() const{
    return *current_shader_programm;
}

// Setting vars into shaders
void Shader::SetMat4(const std::string &name, const void * value, const UniformType type, GLboolean Transpose){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
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
            DEBUG_WARN("NOTHING CHOOSE");
            break;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

void Shader::SetVar(const std::string  &name, const void * value, const UniformType type){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif

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
            DEBUG_WARN("NOTHING CHOOSE");
            break;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

void Shader::SetVec4(const std::string  &name, const void * value, const UniformType type){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
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
            DEBUG_WARN("NOTHING CHOOSE");
            break;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec3(const std::string  &name, const void * value, const UniformType type){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
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
            DEBUG_WARN("NOTHING CHOOSE");
            break;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void Shader::SetVec2(const std::string  &name, const void * value, const UniformType type){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
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
            DEBUG_WARN("NOTHING CHOOSE");
            break;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
// private work functions 
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
Shader::Shader(const std::string & vertext_shader_path, const std::string & fragment_shader_path) {
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif

    Shader::ptrs_links.push_back(this);
    #ifdef DEBUG_MODE
    DEBUG_INFO("Shader pool pushed for 1 element, now capacity - " << Shader::ptrs_links.capacity() << ", added address - " << this);
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
Shader::~Shader(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(current_shader_programm != nullptr){
        glDeleteProgram(*current_shader_programm);
    }
    auto it = std::find(Shader::ptrs_links.begin(), Shader::ptrs_links.end(),   this);
    if(it != Shader::ptrs_links.end()){
        #ifdef DEBUG_MODE
        DEBUG_INFO("Shader pool removed for 1 element, now capacity - " << Shader::ptrs_links.capacity()-1 << ", removed address - " << this);
        #endif
        Shader::ptrs_links.erase(it);
        
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
