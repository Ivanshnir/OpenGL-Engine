#include <iostream>
#include "../../core/debugger/debugger.hpp"
#include "../../core/scripts/scripts.hpp"
#include "../../core/render/managers.hpp"
#include "../../core/render/window.hpp"
#include <chrono>
#include <string>
#include <memory>
#include <thread>
#define ENGINE_NAME "engine 0.0.0"
#define HEIGHT 800
#define WIDTH 600
using logger::States;
const int AMOUNT = 4;
namespace {
    std::string title = "engine 0.0.0";
    unsigned int width = 600;
    unsigned int height = 800;
    unsigned int fps_cap = 144;
};
int main(){

    if(!logger::init("log.log",8192, false)){
        std::cerr << "Files not opens";

    }
    logger::SetFormat("[$mmins:$ssec][$S][($F:$f):$l]$N\n");
    Window myWindows[AMOUNT];
    Render_Manager& rnd = Render_Manager::GET();
    std::thread ts[AMOUNT];
    for(int i = 0; i < AMOUNT; i++){
        myWindows[i].Create(title, height, width);
    }
    glEnable(GL_DEPTH_TEST);
    bool close = false;
    while(!close){
        for(int i = 0; i < AMOUNT; i++){
            if(!rnd.RenderGameWindow(myWindows[i].GetWindow())){
                glfwTerminate();
                goto skip;
            }
        }
        glfwPollEvents();
    }

    skip:
    
    logger::closeFile();
}