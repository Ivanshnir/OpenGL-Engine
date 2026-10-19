#pragma once
#include <string>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <iostream>
constexpr const char * const SHD_DEF_VERT_PATH = "./src/shaders/DefaultShaders/vert_shader.glsl";
constexpr const char * const SHD_DEF_FRAG_PATH = "./src/shaders/DefaultShaders/frag_shader.glsl";
enum UniformType{
    U_Float,
    U_Int,
    U_Uint,
    U_Double 
};
template<typename T>
struct IDBinder{
    private:
    static std::vector<unsigned int> IDs;
    static std::vector<size_t> DeletedIDs;
    bool needsToSort;
    int AssignedID;
    public:
    const unsigned int GetID() const;
    IDBinder();
    void OccupyID();
    ~IDBinder();
    size_t SizeID() const;
    friend std::ostream& operator<<(std::ostream & out,const IDBinder<T>  & Target){
        out << '{';
        const size_t b = Target.SizeID();
        for(size_t i = 0; i < b; i++){
            if(i+1 != b){
                out << ' ' << std::to_string(Target.IDs[i]) << " ,";
            }else{
                out << ' ' << std::to_string(Target.IDs[i]) << ' ';
            }
            
        }
        out << '}';
        return out; 
    }
};

class Shader{
    
    public:
    
    const unsigned int ID() const;
    bool attach_shader();
    // static polymorhism to setting vars into shaders
    void SetMat4(const std::string &name, const void * value, const UniformType type, GLboolean Transpose = false);
    void SetVar(const std::string  &name, const void * value, const UniformType type);
    void SetVec4(const std::string  &name, const void * value, const UniformType type);
    void SetVec3(const std::string  &name, const void * value, const UniformType type);
    void SetVec2(const std::string  &name, const void * value, const UniformType type);

    //Destructor with Constructors
    ~Shader();
    Shader(const std::string & Vertex_Path, const std::string & Fragmment_Path);
    Shader(const Shader &);
    static Shader * get_default_shader();
    const std::string & GetVertPath() const;
    const std::string & GetFragPath() const;
    bool operator==(const Shader & other);
    bool operator==(const Shader * other);
    explicit operator bool() const; 
    private:
    inline static std::vector<Shader *>shaders_pool;
    std::string vert_path;
    std::string frag_path;
    bool canBeUsed = false;
    unsigned int  * current_shader_programm = nullptr;
    std::string debug_message;
    const std::string _shader_read(const std::string & path); // reads info about shaders from glsl shader files("file to string data")
    bool _shader_compile_log(unsigned int id, const char * const settings_data);// Applying shaders through created shader program with logging result in return
    bool _shader_link(const unsigned int shaders[], const unsigned int shader_program);// compiles created shader with logging result in return 




};
class Texture{
    public:
    const std::string path;
    unsigned int texture_id;
    GLenum GL_MARK_FAR = GL_NEAREST_MIPMAP_NEAREST;
    GLenum GL_MARK_MIN = GL_NEAREST;
    GLenum texture_borders = GL_MIRRORED_REPEAT;
    std::vector <float> text_coords;
    Texture(const std::string new_path, std::vector <float> & new_text_coords);
    Texture() {};
    private:
    bool canUse = false;
};

struct ObjInstruction{
    std::string name;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uv;
};

extern ObjInstruction CUBE;
class Transform{
    public:
    Transform();
    void Translate(glm::vec3 &);
    void Rotate(glm::vec3 &);
    void Scale(glm::vec3 &);
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 position;
    glm::mat4 get_matrix();
    friend std::ostream& operator<<(std::ostream &,const Transform  &);

};
class GameObject{
    public:
    std::string name;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> uv;
    template <typename T>
    T& GetComponent() const;
    template <typename T>
    T* Get() const;
    GameObject();
    GameObject(ObjInstruction &);
    // rule of 5
    ~GameObject();
    GameObject& operator=(const GameObject&);
    GameObject(const GameObject&);
    GameObject(GameObject &&);
    GameObject& operator=(GameObject &&);
    const unsigned int VAO() const;
    const unsigned int ID() const;
    friend std::ostream& operator<<(std::ostream &, const GameObject &);
    private:
    inline static std::vector<GameObject *>objects_pool;
    friend class Render_Manager;
    IDBinder<GameObject> my_id;
    bool loaded;
    bool force_unload;
    bool _gl_load();
    unsigned int _VBO;
    unsigned int _EBO;
    unsigned int _VAO;
    Transform * transform;
    Texture * texture;
    Shader * shader;

};



// class GameObject{
//     public: 
//     std::vector<float> vertices;
//     std::vector<unsigned int> tris;
//     // public needs
//     static std::vector<GameObject*> ptrs_links;
//     std::string name;
//     const unsigned int my_id;
//     // Main interaction interface
//     void Apply(GLint BufferVertsFlag = GL_STATIC_DRAW, GLint BufferTrisFlag = GL_STATIC_DRAW);
//     Texture texture;
//     Transform transform;
//     //Destructor with Constructors
//     ~GameObject();
//     GameObject(const GameObject & ToCopy);
//     GameObject(ObjInstruction & MeshTemplate);
//     GameObject(std::vector<float> & vertices, std::vector<unsigned int> & trises, Transform & transform, std::string new_name ="");
//     GameObject(std::string new_name ="");
//     unsigned int & vao() { return VAO; };
//     //Changing Meshes and reading data meshw
//     void Set_Shader(Shader * new_ptr);
//     void Set_Shader();
//     bool operator==(const GameObject & other);
//     GameObject &operator=(const GameObject & ClassToAssign);   
//     bool operator==(const GameObject * other);
//     Shader & shader();
    
//     private: 


//     Shader * _ptr_link_shader = nullptr;
//     static unsigned int _last_id;
    
    
//     unsigned int VAO;
//     unsigned int VBO;
//     unsigned int EBO;
//     void _load_buffs(GLint BufferVertsFlag = GL_STATIC_DRAW, GLint BufferTrisFlag = GL_STATIC_DRAW);
//     void _load_attributes();
//     friend std::ostream& operator << (std::ostream &out, const GameObject &);
//     friend class Render_Manager;
// };