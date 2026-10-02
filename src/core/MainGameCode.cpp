#include "../modules/Shader/Shader.hpp"
#include "../modules/GameObject/GameObject.hpp"
#include "engine_core.h"
bool check = true;
const int AMOUNT = 50;
GameObject * Cubes = nullptr;
float speed = 20.0f;  
void Start(){
    Cubes = new GameObject[AMOUNT];
}
void Update(){
    if(check){
        float y_coord = -0.9f;
        float x_coord = -0.9f;
        for(int i = 0; i < AMOUNT; i++){
            Cubes[i] = GameObject(CUBE);
            Cubes[i].Translate(Vector3(x_coord, y_coord, 0.0f));
            Cubes[i].Scale(Vector3(0.2f, 0.2f, 0.2f));
            std::cout << Cubes[i];
            x_coord += 0.3f;
            if(x_coord > 1.0f){
                x_coord = -0.9f;
                y_coord += 0.3f;
            }
        }
        check = false;
    }
    for(int i = 0; i < AMOUNT; i++){
        Cubes[i].Rotate(Vector3(1.0f, 1.0f, 0.0f)*deltaTime*speed);
    }
}