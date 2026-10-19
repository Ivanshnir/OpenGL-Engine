
#include <vector>
#include "../modules/GameObject/GameObject.hpp"
#include "Window_Manager\window.hpp"
#define NAME "My_Engine0.1.2"
#include "Debugger\debug.hpp"
#define TEXTURE1_PATH "./src/textures/awesomeface.png"
#define TEXTURE2_PATH "./src/textures/container.jpg"
#define SHADER_VERTEX_PATH "./src/shaders/Vertex_Shader_WoutT.glsl"
#define SHADER_FRAGMENT_PATH "./src/shaders/Fragment_Shader_WoutT.glsl"
#include <filesystem>
int main(){
    if(!logger::init("log.ansi", 8192)){
        std::cout << "Not opens log\n";
        std::cerr << "Working directory: "
          << std::filesystem::current_path() << '\n';
        return -1;
    }
    LOG("Log initialized and main starts", INFO);
    Window& myWindow = Window::Create_Window(HEIGHT, WIDTH, NAME);
    myWindow.setup();
    logger::closeFile();


}

// unsigned int * learning_stuff(){
//     float texCoords[] = {
//         0.0f, 0.0f,
//         1.0f, 0.0f, 
//         0.5f, 1.0f
//     };




//     if(data){
//         try{
//             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//             glGenerateMipmap(GL_TEXTURE_2D);
//         }
//         catch (int i){
//             std::cout << IMAGE_BROKEN;
//         }

//     }
//     else{
//         std::cout << IMAGE_LOAD_ERROR;
//     }
//     stbi_image_free(data);
//     glGenTextures(1, &*(textures+1));
//     glBindTexture(GL_TEXTURE_2D, *(textures+1));
//     glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//     glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//     glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
//     glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     data = stbi_load(TEXTURE1_PATH, &width, &height, &nrChannels, 0);


        

//     }    else{
//         std::cout << IMAGE_LOAD_ERROR;
//     }
//     stbi_image_free(data);
    
//     return textures;
// }
