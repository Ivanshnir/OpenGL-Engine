#include <Engine.hpp>

class MyScript : public MonoScript<MyScript>{
    void Start() override{
        std::cout << "Hello World!" << std::endl;
    }
    void Update() override{
    }
};
REGISTER_SCRIPT(MyScript);