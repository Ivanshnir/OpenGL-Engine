#pragma once
#include "../object/object.hpp"
#include <memory>
#include "../render/managers.hpp"
#include <iostream>
class Script : public Object{
    public:
    virtual void Update() = 0;
    virtual void Start() = 0;
    virtual void Awake() = 0;
    type GetType() override { return type::Script; } ; 
    ~Script() {};
    Script() : Object() {};
};
template <typename T>
class MonoScript : public Script{
    public:
    MonoScript() : Script() {};
    void Update() override {};
    void Start() override {};
    void Awake() override {};
    private:
    inline static std::unique_ptr<T> instance = nullptr;

    inline static const bool registered = [] {
        instance = std::make_unique<T>();
        return true;
    }();
    void _register_child_class();
    
};
template<typename T>
struct Registry{
    Registry(){
        Object_Manager& mng = Object_Manager::GET();
        std::cout << "here 36" <<  std::endl;
        mng.Add(new T);
    };
};
#define REGISTER_SCRIPT(script)  static inline Registry<script> __reg__##script