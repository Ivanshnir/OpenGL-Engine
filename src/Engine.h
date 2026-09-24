#ifndef ENGINE_H_ 
#define ENGINE_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLFW_INIT_ERROR "[ERROR 0] FAILED TO INITIALISE GLFW\n"
#define GLAD_ATTACHING_ERROR "[ERROR 1] FAILED TO ATTACH GLAD\n"
#define SHADER_READ_ERROR "[ERROR 2] FAILED TO READ A SHADER FROM THE FILE(TIP: CHECK THE PATH)\n"
#define SHADER_COMPILE_ENTIRE_ADDITION "[ERROR 3] SHADER COMPILING ERROR! DETAILS: \n"
#define SHADER_LINK_ERROR "[ERROR 4] SHADER LINKING WENT WRONG! DETAILS: \n "
#define SHADER_USING_FAIL "[ERROR 5] FAILED TO USE SHADER PROGRAM (P.S. TRY TO RECONSTRUCT SHADER)\n"
#define IMAGE_LOAD_ERROR "[ERROR 6] FAILED TO LOAD IMAGE\n"
#define IMAGE_BROKEN  "[ERROR 7] IMAGE LOADED BUT CAN`T BE USED\n"

GLFWwindow * window_creating(int height, int width, const std::string & name); // Creates Window with all initializing and glad connecting,framebuffing_size
void framebuffer_size_callback(GLFWwindow * window, int width, int height); // changes glViewport after resizing window

class Shader{
    
    public:
    const unsigned int ID() const;
    Shader(const std::string & Vertex_Path, const std::string & Fragmment_Path);
    std::string get_debug();
    bool use_shader();
     // Overloading for using matrix4x4
    void SetMat4(const std::string  &name, GLfloat * value, GLboolean Transpose = GL_FALSE);
    void SetMat4(const std::string  &name, GLdouble * value, GLboolean Transpose = GL_FALSE);

    // Main sets into shader
    void SetInt(const std::string  &name, GLint value);
    void SetUInt(const std::string  &name, GLuint value);
    void SetFloat(const std::string  &name, GLfloat value);
    void SetDouble(const std::string  &name, GLdouble value);


    // Overloading for using vec4 into shader
    void SetVec4(const std::string  &name, GLint * value);
    void SetVec4(const std::string  &name, GLfloat * value);
    void SetVec4(const std::string  &name, GLuint * value);
    void SetVec4(const std::string  &name, GLdouble * value);

    // Overloading for using vec3 into shader
    void SetVec3(const std::string  &name, GLint * value);
    void SetVec3(const std::string  &name, GLfloat * value);
    void SetVec3(const std::string  &name, GLuint * value);
    void SetVec3(const std::string  &name, GLdouble * value);

    // Overloading for using vec2 into shader
    void SetVec2(const std::string  &name, GLint * value);
    void SetVec2(const std::string  &name, GLfloat * value);
    void SetVec2(const std::string  &name, GLuint * value);
    void SetVec2(const std::string  &name, GLdouble * value);
    ~Shader();
    private:
    static unsigned int _last_id;
    unsigned int  * current_shader_programm;
    std::string debug_message;
    const std::string _shader_read(const std::string & path); // reads info about shaders from glsl shader files("file to string data")
    const std::string * _shader_compile_log(unsigned int id, const char * const settings_data);// Applying shaders through created shader program with logging result in return
    const std::string * _shader_link(const unsigned int shaders[], const unsigned int shader_program);// compiles created shader with logging result in return 




};
struct Vector3{
    public:
    float x;
    float y;
    float z;
    Vector3(float _x, float _y, float _z);
    Vector3();
};
class GameObject{
    public: 
    void Apply(GLint BufferVertsFlag = GL_STATIC_DRAW, GLint BufferTrisFlag = GL_STATIC_DRAW);
    void Rotate(Vector3 & RotationalVector, float & angle);
    void Translate(Vector3 & TransitionalVector);
    void Scale(Vector3 & ScaleVector);
    static GameObject Cube(Vector3 * origin = new Vector3(0.0f,0.0f,0.0f));
    ~GameObject();
    void Show(Shader * ourShader);
    const unsigned int GET_VAO() const;
    const unsigned int GET_VBO() const;
    const unsigned int GET_EBO() const;
    void SetVertices(std::vector<float> & vertices);
    void SetTrises(std::vector<unsigned int> & trises);
    GameObject(std::vector<float> & vertices, std::vector<unsigned int> & trises, Vector3 * origin = new Vector3(0.0f,0.0f,0.0f));
    GameObject(Vector3 * origin = new Vector3(0.0f,0.0f,0.0f));
    private: 
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    void _load_buffs(GLint BufferVertsFlag = GL_STATIC_DRAW, GLint BufferTrisFlag = GL_STATIC_DRAW);
    void _load_attributes();
    unsigned int _vertices_size = 0;
    unsigned int _tris_size = 0;
    std::vector<float> _vertices;
    std::vector<unsigned int> _tris;
    glm::mat4 _obj_transform;
};
#endif