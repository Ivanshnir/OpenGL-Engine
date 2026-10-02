#ifndef SHADER_HPP_
#define SHADER_HPP_
#include <string>
#include <vector>
#include <glad/glad.h>
#include <algorithm>
#include "../../core/Debugger/debug.hpp"
enum UniformType{
    U_Float,
    U_Int,
    U_Uint,
    U_Double 
};
class Shader{
    
    public:
    const unsigned int ID() const;
    static std::vector<Shader*> ptrs_links;
    std::string get_debug();
    bool use_shader();
    // static polymorhism to setting vars into shaders
    void SetMat4(const std::string &name, const void * value, const UniformType type, GLboolean Transpose = false);
    void SetVar(const std::string  &name, const void * value, const UniformType type);
    void SetVec4(const std::string  &name, const void * value, const UniformType type);
    void SetVec3(const std::string  &name, const void * value, const UniformType type);
    void SetVec2(const std::string  &name, const void * value, const UniformType type);

    //Destructor with Constructors
    ~Shader();
    Shader(const std::string & Vertex_Path, const std::string & Fragmment_Path);
    bool operator==(const Shader & other);
    bool operator==(const Shader * other);
    explicit operator bool() const; 
    private:
    unsigned int  * current_shader_programm = nullptr;
    std::string debug_message;
    const std::string _shader_read(const std::string & path); // reads info about shaders from glsl shader files("file to string data")
    const std::string * _shader_compile_log(unsigned int id, const char * const settings_data);// Applying shaders through created shader program with logging result in return
    const std::string * _shader_link(const unsigned int shaders[], const unsigned int shader_program);// compiles created shader with logging result in return 




};
#endif