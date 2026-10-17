#include "render_manager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../modules/GameObject/GameObject.hpp"
#include "../Debugger/debug.hpp"
#include <memory>


Render_Manager & Render_Manager::GET(){
    static Render_Manager instance;
    return instance;
}

void Render_Manager::_obj_render(GameObject & Target){
    if(!Target.GetComponent<Shader>().attach_shader()){
        LOG("GameObject "+std::to_string(Target.ID())+" failed attach shader", ERROR);
        return;
    }
    Target.GetComponent<Shader>().SetMat4("model", glm::value_ptr(Target.GetComponent<Transform>().get_matrix()), U_Float);
    glBindVertexArray(Target.VAO());
    glDrawElements(GL_TRIANGLES, Target.indices.size(), GL_UNSIGNED_INT, 0);
}
void Render_Manager::FrameRender(){
    LOG("", TRACE);
    for(int i = 0; i < objects_pool.size(); i++){
        
        if(objects_pool[i] != nullptr){
            LOG(*objects_pool[i], DEBUG);
            _obj_render(*objects_pool[i]);
        }else{
            LOG("Objects_pool have nullptrs! num - " << i, WARN);
        }
        
    }
    
}

template <typename T>
void Render_Manager::Render_Add(T * Target) {};

template<>
void Render_Manager::Render_Add<Shader>(Shader * Target){
    shaders_pool.emplace_back(Target);
    LOG("Shaders pool appendded: "+std::to_string(shaders_pool.size()), INFO);
};
template<>
void Render_Manager::Render_Add<GameObject>(GameObject * Target){
    objects_pool.emplace_back(Target);
    LOG("GameObject pool appendded: "+std::to_string(objects_pool.size()), INFO);
};
template<>
void Render_Manager::Render_Add<Texture>(Texture * Target){
    textures_pool.emplace_back(Target);
    LOG("Texture pool appendded: "+std::to_string(textures_pool.size()), INFO);
};

template <typename T>
void Render_Manager::Render_Remove(T * Target) {};

template<>
void Render_Manager::Render_Remove<Shader>(Shader * Target){
    for(size_t i = 0; i < shaders_pool.size(); i++){
        if(shaders_pool[i] == Target){
            shaders_pool.erase(shaders_pool.begin()+i);
            LOG("Shaders pool seized: "+std::to_string(shaders_pool.size()), INFO);
        }
    }
    
};
template<>
void Render_Manager::Render_Remove<GameObject>(GameObject * Target){
    for(size_t i = 0; i < objects_pool.size(); i++){
        if(objects_pool[i] == Target){
            objects_pool.erase(objects_pool.begin()+i);
            LOG("GameObject pool seized: "+std::to_string(objects_pool.size()), INFO);
        }
    }
    
};
template<>
void Render_Manager::Render_Remove<Texture>(Texture * Target){
    for(size_t i = 0; i < textures_pool.size(); i++){
        if(textures_pool[i] == Target){
            textures_pool.erase(textures_pool.begin()+i);
            LOG("Texture pool seized: "+std::to_string(textures_pool.size()), INFO);
        }
    }
};
void Render_Manager::RemoveAll(){
    textures_pool.clear();
    objects_pool.clear();
    shaders_pool.clear();
    LOG("GameObject pool seized: "+std::to_string(objects_pool.size()), INFO);
    LOG("Shaders pool seized: "+std::to_string(shaders_pool.size()), INFO);
    LOG("Texture pool seized: "+std::to_string(textures_pool.size()), INFO);
}