#include "GameObject.hpp"
#include "../../core/Debugger/debug.hpp"
#include "../../core/Render/render_manager.hpp"
#include "../../core/defaults.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <format>
#include <algorithm>
#include <utility>
const unsigned int GameObject::ID() const{
    return my_id.GetID();
}
const unsigned int GameObject::VAO() const{ return _VAO; };
template <typename T>
T& GameObject::GetComponent() const{
    LOG("GetComponent didn`t took any args(Object: " << *this << ")", FATAL);
    throw std::runtime_error("GetComponent didn`t took any args");
}
template <>
Shader& GameObject::GetComponent<Shader>() const{
    if(shader == nullptr){
        LOG("Shader is nullptr(Object: " << *this << ")", FATAL);
        throw std::runtime_error("GetComponent<Shader>() is nullptr");
    }
    return *shader;
}
template <>
Texture& GameObject::GetComponent<Texture>() const{
    if(texture == nullptr){
        LOG("Texture is nullptr(Object: " << *this << ")", FATAL);
        throw std::runtime_error("GetComponent<Texture>() is nullptr");
    }
    return *texture;
}
template <>
Transform& GameObject::GetComponent<Transform>() const{
    if(transform == nullptr){

        LOG("Transform is nullptr(Object: " << *this << ")", FATAL);
        throw std::runtime_error("GetComponent<Transform>() is nullptr");
    }
    return *transform;
}

template <typename T>
T * GameObject::Get() const{
    LOG("GetComponent didn`t took any args(Object: " << *this << ")", FATAL);
    throw std::runtime_error("GetComponent didn`t took any args");
}
template <>
Shader * GameObject::Get<Shader>() const{
    if(shader == nullptr) return nullptr;
    return shader;
}
template <>
Texture * GameObject::Get<Texture>() const{
    if(texture == nullptr) return nullptr;
    return texture;
}
template <>
Transform * GameObject::Get<Transform>() const{
    if(transform == nullptr) return nullptr;
    return transform;
}


bool GameObject::_gl_load(){
    LOG("", TRACE);
    if(vertices.size()%3 != 0 || indices.size()%3 != 0){
        LOG("Vertices or indices not correct", ERROR);
        return false;
    }
    if(shader == nullptr || transform == nullptr){
        LOG("Haven`t transform or shader",ERROR);
        return false;
    }
    std::vector<float> full_buf;
    size_t v_pl = 0;
    size_t uv_pl = 0;
    for(size_t i = 0; i < vertices.size()/3; i++){
        for(unsigned char j = 0; j < 3; j++){
            full_buf.emplace_back(vertices[v_pl+j]);
        }
        v_pl+=3;
        for(unsigned char j = 0; j < 2; j++){
            if(uv.size() == 0 || uv.size() <= i+j){
                full_buf.emplace_back(0.0f);
            }else{
                full_buf.emplace_back(uv[uv_pl+j]);
            }
            
        }
        uv_pl+=2;
    }
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, full_buf.size() * sizeof(full_buf[0]), full_buf.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return true;
}

GameObject::GameObject(){
    LOG("", TRACE);
    force_unload = false;
    loaded = false;
    my_id.OccupyID();
    vertices = {};
    indices = {};
    uv = {};
    shader = nullptr;
    texture = nullptr;
    transform = new Transform();
    name = "Empty"+std::to_string(my_id.GetID());
    LOG("GameObject '"+name+"' was created succesfull", INFO);
    LOG(*this, DEBUG);
    objects_pool.emplace_back(this);
}
GameObject::GameObject(ObjInstruction & MeshTemplate){
    LOG("", TRACE);
    force_unload = false;
    loaded = false;
    
    my_id.OccupyID();
    shader = Shader::get_default_shader();
    if(!shader){
        LOG("GameObject '"+name+"' haven`t working shader linked", WARN);
    }
    vertices = MeshTemplate.vertices;
    indices = MeshTemplate.indices;
    uv = MeshTemplate.uv;
    transform = new Transform();
    name = MeshTemplate.name+std::to_string(my_id.GetID());
    LOG("GameObject '"+name+"' was created succesfull", INFO);
    LOG(*this, DEBUG);
    objects_pool.emplace_back(this);
}

// rule of 5
GameObject& GameObject::operator=(const GameObject& Target){

    LOG("", TRACE);
    force_unload = false;
    loaded = false;
    if(this != &Target){
        transform = new Transform(Target.GetComponent<Transform>());
        shader = Target.Get<Shader>();
        name = Target.name;
        vertices = Target.vertices;
        indices = Target.indices;
        uv = Target.uv;
        if(Target.texture != nullptr)
            texture = Target.Get<Texture>();
        else 
            texture = nullptr;
    }
    LOG(*this, DEBUG);
    objects_pool.emplace_back(this);
    return *this;
}
GameObject& GameObject::operator=(GameObject && Target){
    LOG("", TRACE);
    force_unload = false;
    loaded = false;
    if(this != &Target){
        transform = std::exchange(Target.transform, nullptr);
        shader = Target.Get<Shader>();
        Target.shader = nullptr;
        name = Target.name;
        vertices = Target.vertices;
        indices = Target.indices;
        uv = Target.uv;
        if(Target.texture != nullptr){
            texture = Target.Get<Texture>();
            Target.texture = nullptr;
        }
        else{
            texture = nullptr;
        }
            
        my_id.OccupyID();
    }
    LOG(*this, DEBUG);
    objects_pool.emplace_back(this);
    return *this;
}
GameObject::GameObject(const GameObject & Target){
    LOG("", TRACE);
    force_unload = false;
    loaded = false;
    my_id.OccupyID();
    transform = new Transform(Target.GetComponent<Transform>());
    shader = Target.Get<Shader>();
    name = Target.name;
    vertices = Target.vertices;
    indices = Target.indices;
    uv = Target.uv;
    if(Target.texture != nullptr)
        texture = Target.Get<Texture>();
    else 
        texture = nullptr;
    LOG(*this, DEBUG);
    objects_pool.emplace_back(this);
}
GameObject::GameObject(GameObject && Target){

    LOG("", TRACE);
    force_unload = false;
    loaded = false;
    name = Target.name;
    transform = std::exchange(Target.transform, nullptr);
    shader = Target.Get<Shader>();
    Target.shader = nullptr;
    vertices = Target.vertices;
    indices = Target.indices;
    uv = Target.uv;

    if(Target.texture != nullptr){
        texture = Target.Get<Texture>();
        Target.texture = nullptr;
    }
    else{
        texture = nullptr;
    }
        
    my_id.OccupyID();
    LOG(*this, DEBUG);
    objects_pool.emplace_back(this);
}
GameObject::~GameObject(){
    LOG("", TRACE);
    LOG(*this, DEBUG);
    delete transform;
    transform = nullptr;
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    shader = nullptr;
    texture = nullptr;
    vertices = {};
    indices = {};
    uv = {};
    auto it = std::find(objects_pool.begin(), objects_pool.end(), this);
    if(it != objects_pool.end()){
        objects_pool.erase(it);
    }else{
        LOG("PTR not cleans from pool", WARN);
    }
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
        out << std::format("< TYPE: GAMEOBJECT, NAME: {}, VERTICES_SIZE: {}, INDICES_SIZE: {}, TRANSFORM_PTR: {:p}, SHADER_PTR: {:p}, TEXTURE_PTR: {:p}, ID: {}, VBO: {}, VAO: {}, EBO: {} >", Target.name, Target.vertices.size(), Target.indices.size(), static_cast<const void*>(Target.transform), static_cast<const void*>(Target.shader), static_cast<const void*>(Target.texture), Target.ID(), Target._VBO, Target._VAO, Target._EBO);
    }
    catch (const std::exception & e){
        std::cerr << e.what();
    }
    return out;
}
std::ostream& operator<<(std::ostream & out,const Transform  & Target){
    try{
        out << std::format("<Position:[{},{},{}]; Rotation: [{},{},{}]; Scale:[{},{},{}]>", Target.position.x, Target.position.y, Target.position.z, Target.rotation.x, Target.rotation.y, Target.rotation.z, Target.scale.x, Target.scale.y, Target.scale.z);
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
        0.5f,  0.5f, 0.5f,     // top right
        0.5f, -0.5f, 0.5f,       // bottom right
        -0.5f, -0.5f, 0.5f,     // bottom left
        -0.5f,  0.5f, 0.5f,      // top left 
        0.5f,  0.5f, -0.5f,     // top right
        0.5f, -0.5f, -0.5f,   // bottom right
        -0.5f, -0.5f, -0.5f,    // bottom left
        -0.5f,  0.5f, -0.5f,      // top left         
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


    },
    {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
    }
};
