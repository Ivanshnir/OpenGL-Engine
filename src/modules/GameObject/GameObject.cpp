#include "GameObject.hpp"
#include "../../core/Debugger/debug.hpp"
#include "../../core/Render/render_manager.hpp"
#include "../../core/defaults.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <format>
const unsigned int GameObject::ID() const{
    return my_id.GetID();
}
const unsigned int GameObject::VAO() const{ return _VAO; };
template <typename T>
T& GameObject::GetComponent() const{
    return nullptr;
}
template <>
Shader& GameObject::GetComponent<Shader>() const{
    return *shader;
}
template <>
Texture& GameObject::GetComponent<Texture>() const{
    return *texture;
}
template <>
Transform& GameObject::GetComponent<Transform>() const{
    return *transform;
}

void GameObject::_gl_load(){
    LOG("", TRACE);
    Render_Manager& rnd = Render_Manager::GET();
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    rnd.Render_Add(this);
}

GameObject::GameObject(){
    LOG("", TRACE);
    my_id.OccupyID();
    vertices = {};
    indices = {};
    uv = {};
    shader = nullptr;
    texture = nullptr;
    transform = std::make_unique<Transform>();
    name = "Empty"+std::to_string(my_id.GetID());
    LOG("GameObject '"+name+"' was created succesfull", INFO);
    _gl_load();
     LOG(*this, DEBUG);
}
GameObject::GameObject(ObjInstruction & MeshTemplate){
    LOG("", TRACE);
    my_id.OccupyID();
    shader = std::make_shared<Shader>(SHD_DEF_VERT_PATH, SHD_DEF_FRAG_PATH);
    if(!shader){
        LOG("GameObject '"+name+"' haven`t working shader linked", WARN);
    }
    vertices = MeshTemplate.vertices;
    indices = MeshTemplate.indices;
    transform = std::make_unique<Transform>();
    name = MeshTemplate.name+std::to_string(my_id.GetID());
    LOG("GameObject '"+name+"' was created succesfull", INFO);
    _gl_load();
    LOG(*this, DEBUG);
}

// rule of 5
GameObject& GameObject::operator=(const GameObject& Target){
    LOG("", TRACE);
    if(this != &Target){
        transform = std::make_unique<Transform>(Target.GetComponent<Transform>());
        shader = std::make_shared<Shader>(Target.GetComponent<Shader>());
        name = Target.name;
        if(Target.texture != nullptr)
            texture = std::make_shared<Texture>(Target.GetComponent<Texture>());
        else 
            texture = nullptr;
    }
    _gl_load();
    LOG(*this, DEBUG);
    return *this;
}
GameObject& GameObject::operator=(GameObject && Target){
    LOG("", TRACE);
    if(this != &Target){
        transform = std::make_unique<Transform>(Target.GetComponent<Transform>());
        shader = std::make_shared<Shader>(Target.GetComponent<Shader>());
        name = Target.name;
        if(Target.texture != nullptr)
            texture = std::make_shared<Texture>(Target.GetComponent<Texture>());
        else 
            texture = nullptr;
        
        Target.shader = nullptr;
        Target.transform = nullptr;
        Target.texture = nullptr;
    }
    _gl_load();
    LOG(*this, DEBUG);
    return *this;
}
GameObject::GameObject(const GameObject & Target){
    LOG("", TRACE);
    my_id.OccupyID();
    transform = std::make_unique<Transform>(Target.GetComponent<Transform>());
    shader = std::make_shared<Shader>(Target.GetComponent<Shader>());
    name = Target.name;
    if(Target.texture != nullptr)
        texture = std::make_shared<Texture>(Target.GetComponent<Texture>());
    else 
        texture = nullptr;
    
    _gl_load();
    LOG(*this, DEBUG);
}
GameObject::GameObject(GameObject && Target){
    LOG("", TRACE);
    my_id.OccupyID();
    name = Target.name;
    transform = std::make_unique<Transform>(Target.GetComponent<Transform>());
    shader = std::make_shared<Shader>(Target.GetComponent<Shader>());
    if(Target.texture != nullptr)
        texture = std::make_shared<Texture>(Target.GetComponent<Texture>());
    else 
        texture = nullptr;
        
    
    _gl_load();
    
    Target.~GameObject();
    LOG(*this, DEBUG);
}
GameObject::~GameObject(){
    LOG("", TRACE);
    LOG(*this, DEBUG);
    transform = nullptr;
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    shader = nullptr;
    texture = nullptr;
    my_id.~IDBinder();
    Render_Manager& rnd = Render_Manager::GET();
    rnd.Render_Remove<GameObject>(this);
    LOG("GameObject '"+name+"' was deleted succesfull", INFO);
    name = "";
}
Transform::Transform(){
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}
void Transform::Rotate(glm::vec3 & add){
    rotation +=  add;
}
void Transform::Scale(glm::vec3 & add){
    scale = add;
}
void Transform::Translate(glm::vec3 & add){
    position += add;
}
glm::mat4 Transform::get_matrix(){
    glm::mat4 transform_matrix = glm::mat4(1.0f);
    transform_matrix = glm::translate(transform_matrix, position);
    transform_matrix = glm::rotate(transform_matrix,glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform_matrix = glm::rotate(transform_matrix,glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform_matrix = glm::rotate(transform_matrix,glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform_matrix = glm::scale(transform_matrix, scale);
    return transform_matrix;
}

std::ostream& operator<<(std::ostream & out, const GameObject & Target){
    try{
        out << std::format("< TYPE: GAMEOBJECT, NAME: {}, VERTICES_SIZE: {}, INDICES_SIZE: {}, TRANSFORM_PTR: {:p}, SHADER_PTR: {:p}, TEXTURE_PTR: {:p}, ID: {}, VBO: {}, VAO: {}, EBO: {} >", Target.name, Target.vertices.size(), Target.indices.size(), static_cast<const void*>(Target.transform.get()), static_cast<const void*>(Target.shader.get()), static_cast<const void*>(Target.texture.get()), Target.ID(), Target._VBO, Target._VAO, Target._EBO);
    }
    catch (const std::exception & e){
        std::cerr << e.what();
    }
    return out;
}
// #include <algorithm>
// // Main interaction functions
// void GameObject::Apply(GLint OpenGLCloseFlag, GLint OpenGLFarFlag){
//     _load_buffs(OpenGLCloseFlag, OpenGLFarFlag);
//     _load_attributes();
// }

// // Constructors and Destructor
// unsigned int GameObject::_last_id = 0;
// GameObject::GameObject(std::string new_name):  my_id(_last_id){
//     this->Set_Shader();
//     if(new_name == ""){
//         name = "Empty"+std::to_string(my_id);
//     }
//     _last_id += 1;
//     transform.Position = glm::vec3(0.0f,0.0f,0.0f);
//     transform.Rotation = glm::vec3(0.0f,0.0f,0.0f);
//     transform.Scale = glm::vec3(1.0f,1.0f,1.0f);
//     GameObject::ptrs_links.push_back(this);
//     LOG("GameObject - "+name+" was created succesfull", INFO);
//     objects_pool.emplace_back(this);
// }

// GameObject::GameObject(const GameObject & ToCopy): my_id(ToCopy.my_id){
//     vertices = ToCopy.vertices;
//     tris = ToCopy.tris;
//     transform = ToCopy.transform;
//     name = ToCopy.name;
//     this->Apply();
//     LOG("GameObject - "+name+" was copied succesfull", INFO);
//     objects_pool.emplace_back(this);
// }
// GameObject::GameObject(std::vector<float> & _vertices, std::vector<unsigned int> & _trises, Transform & _transform, std::string new_name): my_id(_last_id){
//     this->Set_Shader();
//     if(new_name == ""){
//         name = "My_Mesh"+std::to_string(my_id);
//     }
//     _last_id += 1;
//     vertices = _vertices;
//     tris = _trises;
//     transform = _transform;
//     this->Apply();
//     GameObject::ptrs_links.push_back(this);
//     LOG("GameObject - "+name+" was created succesfull", INFO);
//     objects_pool.emplace_back(this);
// }

// GameObject::GameObject(ObjInstruction & MeshTemplate): my_id(_last_id){
//     _last_id++;
    
//     this->Set_Shader();
//     name = MeshTemplate.name+std::to_string(my_id);
    
//     vertices = MeshTemplate.vertices;
//     tris = MeshTemplate.indices;
//     this->Apply();
//     transform = Transform();
//     GameObject::ptrs_links.push_back(this);
//     LOG("GameObject - "+name+" was created succesfull", INFO);
// }
// Shader & GameObject::shader(){
//     return *_ptr_link_shader;
// }
// GameObject::~GameObject(){
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
//     LOG("GameObject - "+name+" will be deleted", INFO);
//     auto it = std::find(GameObject::ptrs_links.begin(), GameObject::ptrs_links.end(), this);
//     if(it != GameObject::ptrs_links.end()){
//         GameObject::ptrs_links.erase(it);
//     }else{
//         LOG("GameObject - "+name+" wasnt in ptrs_links ", INFO);
//     }
// }
// Transform::Transform(){
//     Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//     Position = glm::vec3(0.0f, 0.0f, 0.0f);
//     Scale = glm::vec3(1.0f, 1.0f, 1.0f);
// }

// // Changing Mesh and Read Mesh-Obj Data Stuff 

// void GameObject::Set_Shader(Shader * new_ptr){
//     if(_ptr_link_shader != nullptr){
//         LOG("_ptr_link_shader != nullptr", INFO);
//         delete _ptr_link_shader;
//         _ptr_link_shader = nullptr;
//     }
//     _ptr_link_shader = new_ptr;
// }
// void GameObject::Set_Shader(){
//     if(_ptr_link_shader != nullptr){
//         LOG("_ptr_link_shader != nullptr", INFO);
//         delete _ptr_link_shader;
//         _ptr_link_shader = nullptr;
//     }
//     LOG("Setting new Shader", INFO);
//     _ptr_link_shader = new Shader(SHD_DEF_VERT_PATH, SHD_DEF_FRAG_PATH);
//     if(!(*_ptr_link_shader)){
//         LOG("Can`t create Shader", FATAL);
//         return;
//     }
// }

// // inside working funcs 

// void GameObject::_load_buffs(GLint BufferVertsFlag, GLint BufferTrisFlag){

    
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(tris[0]), tris.data(), GL_STATIC_DRAW);
    
// }



// void GameObject::_load_attributes(){

//     // position attribute

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     // texture coord attribute
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);
// }


// // operator overloading
// GameObject & GameObject::operator=(const GameObject & ClassToAssign){

//     vertices = ClassToAssign.vertices;
//     tris = ClassToAssign.tris;
//     transform = ClassToAssign.transform;
//     name = ClassToAssign.name;
//     this->Apply();
//     LOG("GameObject - "+name+" was copied succesfull", INFO);
//     return *this;
// }
// bool GameObject::operator==(const GameObject & other){

//     if(this->my_id == other.my_id) true;
//     return false;
// }

// bool GameObject::operator==(const GameObject * other){

//     if(this->my_id == (*other).my_id) true;
//     return false;
// }
// std::ostream& operator<<(std::ostream & out, const GameObject & ClassToView){
//     out << "< name: " << ClassToView.name << " , ";
//     out << "verts: " << ClassToView.vertices.size() << " , ";
//     out << "trises: " << ClassToView.tris.size() << " , ";
//     out << "pos: " << "("<< ClassToView.transform.Position.x << "," << ClassToView.transform.Position.y << "," << ClassToView.transform.Position.z  << ")" << " , ";
//     out << "Rotation: " << "("<< ClassToView.transform.Rotation.x << "," << ClassToView.transform.Rotation.y << "," << ClassToView.transform.Rotation.z  << " ) " << " , ";
//     out << "Scale: " << "("<< ClassToView.transform.Scale.x << "," << ClassToView.transform.Scale.y << "," << ClassToView.transform.Scale.z  << ")" << " >" << std::endl;
//     return out;
// }


// // Mesh Templates

ObjInstruction CUBE{
    "Cube",
    {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.5f,   1.0f, 0.0f,    // bottom right
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,    // top left 
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // top right
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   // top left         
    },
    {
        0, 1, 3,
        3, 2, 1,
        4, 1, 0,
        4, 5, 1,
        6, 5, 1,
        2, 1, 6,
        7, 6, 2,
        7, 2, 3,
        6, 5, 4,
        4, 6, 7,
        3, 0, 4,
        4, 7, 3


    }
};

