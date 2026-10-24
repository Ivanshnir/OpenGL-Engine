#include "managers.hpp"
#include <algorithm>
#include "../debugger/debugger.hpp"
#include "../object/object.hpp"
#include "../scripts/scripts.hpp"

void Object_Manager::Add(Object * Target){
    objects_pool.emplace_back(Target);
}
void Object_Manager::Remove(Object * Target){
    auto it = std::find(objects_pool.begin(), objects_pool.end(), Target);
    if(it != objects_pool.end()){
        objects_pool.erase(it);
    }
    else{
        LOG("Element didn`t found", ERROR);
    }
}
Object & Object_Manager::operator[](size_t i){
    return *objects_pool[i];
}
size_t Object_Manager::size(){
    return objects_pool.size();
}
void Render_Manager::Add(Object * Target){
    objects_pool.emplace_back(Target);
}
void Render_Manager::Remove(Object * Target){
    auto it = std::find(objects_pool.begin(), objects_pool.end(), Target);
    if(it != objects_pool.end()){
        objects_pool.erase(it);
    }
    else{
        LOG("Element didn`t found", ERROR);
    }
}
bool Render_Manager::RenderGameWindow(GLFWwindow* window) {
    if (window == nullptr) {
        LOG("Window == nullptr", ERROR);
        return false;
    }

    if (glfwWindowShouldClose(window)) {
        return false;
    }
    if(!_GameWindows_is_init){
        InitGameWindows();
    }
    for(size_t i = 0; i < scripts_pool.size(); i++){
        scripts_pool[i]->Update();
    }
    glfwMakeContextCurrent(window);
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
    return true;
}
void Render_Manager::InitGameWindows(){
    LOG("Init GameWindows", TRACE);
    Object_Manager& obj = Object_Manager::GET();
    for(size_t i = 0; i < obj.size(); i++){
        if(obj[i].GetType() == type::Script){
            Script* script = dynamic_cast<Script*>(&obj[i]);
            script->Awake();
            script->Start();
            scripts_pool.emplace_back(script);
        }
    }
    _GameWindows_is_init = true;
}