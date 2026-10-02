#ifndef GAMEOBJECTS_HPP_
#define GAMEOBJECTS_HPP_
#include "..\Shader\Shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <algorithm>
#include "../../core/Debugger/debug.hpp"
#include "../../core/defaults.hpp"
struct Transform{
    public:
    glm::vec3 Rotation;
    glm::vec3 Scale;
    glm::vec3 Position;
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
    void Rotate (glm::vec3 RotationalVector);
    void Translate  (glm::vec3 TransitionalVector);
    void Scale  (glm::vec3 ScaleVector);
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
    Shader * _ptr_link_shader = nullptr;
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
#endif