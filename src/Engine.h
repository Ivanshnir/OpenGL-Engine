#ifndef ENGINE_H_ 
#define ENGINE_H_
#include "glad.h"
#include <GLFW/glfw3.h>
#include <string>

#define GLFW_INIT_ERROR "[ERROR 0] FAILED TO INITIALISE GLFW\n"
#define GLAD_ATTACHING_ERROR "[ERROR 1] FAILED TO ATTACH GLAD\n"
#define SHADER_READ_ERROR "[ERROR 2] FAILED TO READ A SHADER FROM THE FILE(TIP: CHECK THE PATH)\n"
#define SHADER_COMPILE_ENTIRE_ADDITION "[ERROR 3] SHADER COMPILING ERROR! DETAILS: \n"
#define SHADER_LINK_ERROR "[ERROR 4] SHADER LINKING WENT WRONG! DETAILS: \n "
#define SHADER_USING_FAIL "[ERROR 5] FAILED TO USE SHADER PROGRAM (P.S. TRY TO RECONSTRUCT SHADER)\n"
#define IMAGE_LOAD_ERROR "[ERROR 6] FAILED TO LOAD IMAGE\n"
#define IMAGE_BROKEN  "[ERROR 7] IMAGE LOADED BUT CAN`T BE USED\n"

GLFWwindow * window_creating(int height, int width, const char * name); // Creates Window with all initializing and glad connecting,framebuffing_size
void framebuffer_size_callback(GLFWwindow * window, int width, int height); // changes glViewport after resizing window

class Shader{
    
    public:
    const unsigned int ID() const;
    Shader(const char * Vertex_Path, const char * Fragmment_Path);
    std::string get_debug();
    bool use_shader();
    
    ~Shader();
    private:
    static unsigned int _last_id;
    unsigned int  * current_shader_programm;
    std::string debug_message;
    const std::string _shader_read(const char * path); // reads info about shaders from glsl shader files("file to string data")
    const std::string * _shader_compile_log(unsigned int id, const char * settings_data);// Applying shaders through created shader program with logging result in return
    const std::string * _shader_link(const unsigned int shaders[], const unsigned int shader_program);// compiles created shader with logging result in return 
};
#endif