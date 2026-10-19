#include "../core/MyEngine.hpp"
#include <iostream>
#include <vector>

class MySample : MonoImplemantation<MySample>
{
    public:
    // static const int COUNT = 20;
    // GameObject myCubes[COUNT] = {GameObject(CUBE)};
    float speed = 20.0f;
    // void Awake() override{
    // }
    // void Start() override{
    // }
    // void Update() override{
    //     glm::vec3 calc_degreese = glm::vec3(1.0f, 1.0f, 0.0f)*deltaTime*speed;
    //     for(int i = 0; i < COUNT; i++){
    //         myCubes[i].GetComponent<Transform>().Rotate(calc_degreese);
    //     }
    // }

    bool check = true;
    const int COUNT = 16;
    
    std::vector<GameObject> Cubes = {};
    public:
    void Awake() override{
    }
    void Start() override{
    }
    void Update() override{
        if(check){
            
            float y_coord = -0.9f;
            float x_coord = -0.9f;
            for(int i = 0; i < COUNT; i++){
                LOG("CUBE FOR emplacing ", DEBUG);
                Cubes.emplace_back(CUBE);
                LOG("CUBE emplaced" << Cubes[i], DEBUG);
                Cubes[i].GetComponent<Transform>().position.x = x_coord;
                Cubes[i].GetComponent<Transform>().position.y = y_coord;
                Cubes[i].GetComponent<Transform>().scale = glm::vec3(0.2f, 0.2f, 0.2f);
                x_coord += 0.3f;
                if(x_coord > 1.0f){
                    x_coord = -0.9f;
                    y_coord += 0.3f;
                }
            }
            check = false;
        }
        glm::vec3 calc_degreese = glm::vec3(1.0f, 1.0f, 0.0f)*deltaTime*speed;
        for(int i = 0; i < COUNT; i++){
            Cubes[i].GetComponent<Transform>().Rotate(calc_degreese);
        }
    }


};