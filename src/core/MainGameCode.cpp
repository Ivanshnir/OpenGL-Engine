#include "engine_core.h"


bool check = true;
const int AMOUNT = 50;
GameObject * Cubes = nullptr;  
void Start(){
    Cubes = new GameObject[AMOUNT];
}
void Update(){
    if(check){
        float y_coord = -0.9f;
        float x_coord = -0.9f;
        std::cout << std::endl << "Shader pooL: " << Shader::ptrs_links.size() << std::endl;
        for(int i = 0; i < AMOUNT; i++){
            Cubes[i] = GameObject(CUBE);
            std::cout << std::endl << "Object pooL: " << GameObject::ptrs_links.size() << std::endl;
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
        Cubes[i].Rotate(Vector3(1.0f, 1.0f, 0.0f));
    }
}