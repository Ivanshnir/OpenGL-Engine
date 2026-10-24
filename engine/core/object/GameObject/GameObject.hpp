#pragma once
#include "../object.hpp"
class GameObject : Object{
    template<typename T>
    void AddComponent();
    template<typename T>
    void GetComponent();
    ~GameObject() override {};
    private:
    std::unique_ptr<Component>
};
class Component : Object{

};
class Shader : Component{

}