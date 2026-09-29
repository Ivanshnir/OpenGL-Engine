#include "engine_core.h"

// Main interaction functions

void GameObject::Show(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(!(*_ptr_link_shader).use_shader()){
        (*_ptr_link_shader).get_debug();
    }
    glm::mat4 _obj_transform = glm::mat4(1.0f);
    bool warning = false;
    if(_ptr_link_shader == nullptr){
        warning = true;
        goto warn;
    }

    _obj_transform = glm::translate(_obj_transform, _my_transform.Position);
    _obj_transform = glm::rotate(_obj_transform,glm::radians(_my_transform.Rotation.x), Vector3(1.0f, 0.0f, 0.0f));
    _obj_transform = glm::rotate(_obj_transform,glm::radians(_my_transform.Rotation.y), Vector3(0.0f, 1.0f, 0.0f));
    _obj_transform = glm::rotate(_obj_transform,glm::radians(_my_transform.Rotation.z), Vector3(0.0f, 0.0f, 1.0f));
    _obj_transform = glm::scale(_obj_transform, _my_transform.Scale);
    (*_ptr_link_shader).SetMat4("model", glm::value_ptr(_obj_transform), U_Float);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _tris.size(), GL_UNSIGNED_INT, 0);
    warn:
    if(warning) DEBUG_WARN("nullptr to shader");
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
}
void GameObject::Translate(Vector3 TransitionalVector){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _my_transform.Position += TransitionalVector;
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::Scale(Vector3 ScaleVector){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _my_transform.Scale = ScaleVector;
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::Rotate(Vector3 RotationalVector){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _my_transform.Rotation += RotationalVector;
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

void GameObject::Apply(GLint OpenGLCloseFlag, GLint OpenGLFarFlag){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _load_buffs(OpenGLCloseFlag, OpenGLFarFlag);
    _load_attributes();
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

// Constructors and Destructor
unsigned int GameObject::_last_id = 0;
GameObject::GameObject(std::string new_name):  my_id(_last_id){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    this->Set_Shader();
    if(new_name == ""){
        name = "Empty"+std::to_string(my_id);
    }
    _last_id += 1;
    _my_transform.Position = Vector3(0.0f,0.0f,0.0f);
    _my_transform.Rotation = Vector3(0.0f,0.0f,0.0f);
    _my_transform.Scale = Vector3(1.0f,1.0f,1.0f);
    GameObject::ptrs_links.push_back(this);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

GameObject::GameObject(std::vector<float> & vertices, std::vector<unsigned int> & trises, Transform & transform, std::string new_name): my_id(_last_id){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    this->Set_Shader();
    if(new_name == ""){
        name = "My_Mesh"+std::to_string(my_id);
    }
    _last_id += 1;
    this->SetVertices(vertices);
    this->SetTrises(trises);
    _my_transform = transform;
    this->Apply();
    GameObject::ptrs_links.push_back(this);
    #ifdef DEBUG_MODE
    DEBUG_INFO("GameObject pool pushed for 1 element, now size - " << GameObject::ptrs_links.size() << ", added address - " << this);
    #endif
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

GameObject::GameObject(ObjInstruction & MeshTemplate): my_id(_last_id){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _last_id++;
    this->Set_Shader();
    name = MeshTemplate.name+std::to_string(my_id);
    this->SetVertices(MeshTemplate.vertices);
    this->SetTrises(MeshTemplate.indices);
    this->Apply();
    _my_transform = Transform();
    GameObject::ptrs_links.push_back(this);
    #ifdef DEBUG_MODE
    DEBUG_INFO("GameObject pool pushed for 1 element, now size - " << GameObject::ptrs_links.size() << ", added address - " << this);
    #endif
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

GameObject::~GameObject(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    isDeleted = true;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    auto it = std::find(GameObject::ptrs_links.begin(), GameObject::ptrs_links.end(), this);
    if(it != GameObject::ptrs_links.end()){
        #ifdef DEBUG_MODE
        DEBUG_INFO("GameObject pool erased for 1 element, now size - " << GameObject::ptrs_links.size()-1 << ", added address - " << this);
        #endif
        GameObject::ptrs_links.erase(it);
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
Transform::Transform(){
    Rotation = Vector3(0.0f, 0.0f, 0.0f);
    Position = Vector3(0.0f, 0.0f, 0.0f);
    Scale = Vector3(1.0f, 1.0f, 1.0f);
}

// Changing Mesh and Read Mesh-Obj Data Stuff 

void GameObject::Set_Shader(Shader * new_ptr){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(_ptr_link_shader != nullptr){
        delete _ptr_link_shader;
        _ptr_link_shader = nullptr;
    }
    _ptr_link_shader = new_ptr;
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::Set_Shader(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(_ptr_link_shader != nullptr){
        delete _ptr_link_shader;
        _ptr_link_shader = nullptr;
    }
    _ptr_link_shader = new Shader(DEFAULT_SHADER_VERTEX_PATH, DEFAULT_SHADER_FRAGMENT_PATH);
    if((*_ptr_link_shader).get_debug() != ""){
        #ifdef DEBUG_MODE
        DEBUG_WARN((*_ptr_link_shader).get_debug());
        #endif
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::SetTrises(std::vector<unsigned int> & trises){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(!trises.empty()){
        _tris = trises;
    }
    
    
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
void GameObject::SetVertices(std::vector<float> & vertices){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(!vertices.empty()){
        _vertices = vertices;
    }
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}

std::vector<float> GameObject::get_verts() const{
    return _vertices;
}
std::vector<unsigned int> GameObject::get_trises() const{
    return _tris;
}
Transform GameObject::get_transform() const{
    return _my_transform;
}

// inside working funcs 

void GameObject::_load_buffs(GLint BufferVertsFlag, GLint BufferTrisFlag){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _tris.size() * sizeof(_tris[0]), _tris.data(), GL_STATIC_DRAW);
    
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}



void GameObject::_load_attributes(){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    // position attribute

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}


// operator overloading
GameObject & GameObject::operator=(const GameObject & ClassToAssign){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    _vertices = ClassToAssign.get_verts();
    _tris = ClassToAssign.get_trises();
    _my_transform = ClassToAssign.get_transform();
    this->Apply();
    return *this;
    #ifdef DEBUG_MODE
    DEBUG_END_FUNC();
    #endif
}
bool GameObject::operator==(const GameObject & other){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(this->my_id == other.my_id) true;
    return false;
}

bool GameObject::operator==(const GameObject * other){
    #ifdef DEBUG_MODE
    DEBUG_START_FUNC();
    #endif
    if(this->my_id == (*other).my_id) true;
    return false;
}
std::ostream& operator<<(std::ostream & out, const GameObject & ClassToView){
    out << "Name: " << ClassToView.name << std::endl;
    out << "Count of Vertices: " << ClassToView._vertices.size() << std::endl;
    out << "Count of Trises: " << ClassToView._tris.size() << std::endl;
    out << "Position: " << "( "<< ClassToView.get_transform().Position.x << " , " << ClassToView.get_transform().Position.y << " , " << ClassToView.get_transform().Position.z  << " ) " << std::endl;
    out << "Rotation: " << "( "<< ClassToView.get_transform().Rotation.x << " , " << ClassToView.get_transform().Rotation.y << " , " << ClassToView.get_transform().Rotation.z  << " ) " << std::endl;
    out << "Scale: " << "( "<< ClassToView.get_transform().Scale.x << " , " << ClassToView.get_transform().Scale.y << " , " << ClassToView.get_transform().Scale.z  << " ) " << std::endl;
    out << "Is Deleted?: " << ClassToView.isDeleted << std::endl;
    return out;
}


// Mesh Templates

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


