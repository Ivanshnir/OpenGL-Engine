#include "../modules/GameObject/GameObject.hpp"
#include "Window_Manager\window.hpp"
#include <iostream>
#include <vector>
bool check = true;
const int AMOUNT = 20;
float speed = 20.0f;
std::vector<GameObject> Cubes = {};
void Start(){
}
void Update(){
    
    if(check){
        
        float y_coord = -0.9f;
        float x_coord = -0.9f;
        for(int i = 0; i < AMOUNT; i++){
            Cubes.emplace_back(CUBE);
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
    for(int i = 0; i < AMOUNT; i++){
        Cubes[i].GetComponent<Transform>().Rotate(calc_degreese);
    }
}