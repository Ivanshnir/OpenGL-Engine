#include "render_manager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../modules/GameObject/GameObject.hpp"
#include "../Debugger/debug.hpp"
#include "../Window_Manager/window.hpp"
#include <memory>


Render_Manager & Render_Manager::GET(){
    static Render_Manager instance;
    return instance;
}

void Render_Manager::_obj_render(GameObject & Target){
    LOG(Target, DEBUG);
    // if(Target.force_unload){
    //     LOG("GameObject can`t be loaded: " << Target, ERROR);
    //     return;
    // }
    if(!Target.loaded){
        LOG("Loading buffers of GameObject ", DEBUG);
        if(!Target._gl_load()){
            Target.force_unload = true;
        }else{
            Target.loaded = true;
        }
        
    }
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
    for(size_t i = 0; i < GameObject::objects_pool.size(); i++){

        if(GameObject::objects_pool[i] != nullptr){
            _obj_render(*GameObject::objects_pool[i]);
        }else{
            LOG("Objects_pool have nullptrs! num - " << i, WARN);
        }

    }

}
void Render_Manager::RemoveAll(){
    GameObject::objects_pool.clear();

}