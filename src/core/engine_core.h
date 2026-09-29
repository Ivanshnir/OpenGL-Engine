#ifndef ENGINE_CORE_H_ 
#define ENGINE_CORE_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <bits/stdc++.h>
#include "linked_libraries/img_reading/stb_image.h"
#define GLFW_INIT_ERROR "[ERROR 0] FAILED TO INITIALISE GLFW\n"
#define GLAD_ATTACHING_ERROR "[ERROR 1] FAILED TO ATTACH GLAD\n"
#define SHADER_READ_ERROR "[ERROR 2] FAILED TO READ A SHADER FROM THE FILE(TIP: CHECK THE PATH)\n"
#define SHADER_COMPILE_ENTIRE_ADDITION "[ERROR 3] SHADER COMPILING ERROR! DETAILS: \n"
#define SHADER_LINK_ERROR "[ERROR 4] SHADER LINKING WENT WRONG! DETAILS: \n "
#define SHADER_USING_FAIL "[ERROR 5] FAILED TO USE SHADER PROGRAM (P.S. TRY TO RECONSTRUCT SHADER)\n"
#define IMAGE_LOAD_ERROR "[ERROR 6] FAILED TO LOAD IMAGE\n"
#define IMAGE_BROKEN  "[ERROR 7] IMAGE LOADED BUT CAN`T BE USED\n"
#define Vector3 glm::vec3
#define DEFAULT_SHADER_VERTEX_PATH "./src/shaders/DefaultShaders/vert_shader.glsl"
#define DEFAULT_SHADER_FRAGMENT_PATH "./src/shaders/DefaultShaders/frag_shader.glsl"

// LOGGING-DEBUGGING PART
#define DEBUG_MODE
#define DEBUG_END_FUNC() std::cout << "[DEBUG]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << " function finished\n";
#define DEBUG_START_FUNC() std::cout << "[DEBUG]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << " function called\n";
#define DEBUG_INFO(info) std::cout << "[DEBUG][INFO]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << info << "\n";
#define DEBUG_WARN(x) std::cout << "[DEBUG][WARN]" << "[" << __FUNCTION__ << "][" << __LINE__ << "]" << x << "\n";
GLFWwindow * window_creating(int height, int width, const std::string & name); // Creates Window with all initializing and glad connecting,framebuffing_size
void framebuffer_size_callback(GLFWwindow * window, int width, int height); // changes glViewport after resizing window
float deltaTime();
void Update();
void Start();
void frame_process_window(GLFWwindow * window);
void frame_texture_attach();
void texture_attach();
void setup(GLFWwindow * window);
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
    private:
    unsigned int  * current_shader_programm;
    std::string debug_message;
    const std::string _shader_read(const std::string & path); // reads info about shaders from glsl shader files("file to string data")
    const std::string * _shader_compile_log(unsigned int id, const char * const settings_data);// Applying shaders through created shader program with logging result in return
    const std::string * _shader_link(const unsigned int shaders[], const unsigned int shader_program);// compiles created shader with logging result in return 




};
struct Transform{
    public:
    Vector3 Rotation;
    Vector3 Scale;
    Vector3 Position;
    Transform();
};
struct ObjInstruction{
    std::string name;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

extern ObjInstruction CUBE;

class GameObject{
    public: 
    // public needs
    static std::vector<GameObject*> ptrs_links;
    std::string name;
    const unsigned int my_id;
    bool isDeleted = false;
    // Main interaction interface
    void Apply(GLint BufferVertsFlag = GL_STATIC_DRAW, GLint BufferTrisFlag = GL_STATIC_DRAW);
    void Rotate(Vector3 RotationalVector);
    void Translate(Vector3 TransitionalVector);
    void Scale(Vector3 ScaleVector);
    void Show();
    //Destructor with Constructors
    ~GameObject();
    GameObject(ObjInstruction & MeshTemplate);
    GameObject(std::vector<float> & vertices, std::vector<unsigned int> & trises, Transform & transform, std::string new_name ="");
    GameObject(std::string new_name ="");

    //Changing Meshes and reading data mesh
    void Set_Shader(Shader * new_ptr);
    void Set_Shader();
    void SetVertices(std::vector<float> & vertices);
    void SetTrises(std::vector<unsigned int> & trises);
    std::vector<unsigned int> get_trises() const;
    std::vector<float> get_verts() const;
    Transform get_transform() const;

    bool operator==(const GameObject & other);
    GameObject &operator=(const GameObject & ClassToAssign);   
    bool operator==(const GameObject * other);
    
    
    private: 
    Shader * _ptr_link_shader;
    Transform _my_transform;
    static unsigned int _last_id;
    

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    void _load_buffs(GLint BufferVertsFlag = GL_STATIC_DRAW, GLint BufferTrisFlag = GL_STATIC_DRAW);
    void _load_attributes();
    unsigned int _vertices_size = 0;
    unsigned int _tris_size = 0;
    std::vector<float> _vertices;
    std::vector<unsigned int> _tris;

    friend std::ostream& operator << (std::ostream &out, const GameObject &);
};

void shader_attach(Shader & ShaderToAttach);

#endif