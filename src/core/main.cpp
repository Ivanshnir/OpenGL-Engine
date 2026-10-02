

#include "engine_core.h"
#define HEIGHT 640
#define WIDTH 500
#define NAME "My_Engine0.0.1"
#include "Debugger\debug.hpp"
#define TEXTURE1_PATH "./src/textures/awesomeface.png"
#define TEXTURE2_PATH "./src/textures/container.jpg"
#define SHADER_VERTEX_PATH "./src/shaders/Vertex_Shader_WoutT.glsl"
#define SHADER_FRAGMENT_PATH "./src/shaders/Fragment_Shader_WoutT.glsl"
unsigned int * learning_stuff();
void LearnMatrices(Shader * ourShader);
std::vector <GameObject*> GameObject::ptrs_links;
std::vector <Shader*> Shader::ptrs_links;
int main(){
    if(!logger::init("log.txt"))
        return -1;
    LOG("Start", TRACE);
    GLFWwindow * window = window_creating(HEIGHT, WIDTH, NAME);
    setup(window);

    
    // glUniform1i(glGetUniformLocation(my_mini_shader.ID(), "texture1"), 0);
    // glUniform1i(glGetUniformLocation(my_mini_shader.ID(), "texture2"), 1);


    //     float my_angle;
    //     if(window == nullptr) return -1;
    //     float comp = 20.0f;
    //     float comp_factor = 0.03f; 
    //     glEnable(GL_DEPTH_TEST);
    //     unsigned int i = 0;

    //     float step1 = 0.001f;
    //     bool check1 = false;
    //     while(!glfwWindowShouldClose(window))
    //     {
            // glClearColor(0.2f,0.3f, 0.4f, 1.0f);
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //         // glActiveTexture(GL_TEXTURE0);
    //         // glBindTexture(GL_TEXTURE_2D, *texture);
    //         // glActiveTexture(GL_TEXTURE1);
    //         // glBindTexture(GL_TEXTURE_2D, *(texture+1));
    //         if(!my_mini_shader.use_shader()) {
    //             std::cout << my_mini_shader.get_debug();
    //             return -1;
    //         }   
    //         // for(int i = 0; i < std::size(MyCubes); i++){
    //         //     MyCubes[i].Show(&my_mini_shader);
    //         // }
    //         // NewCube.Show(&my_mini_shader);
    //         for (int i = 0; i < sizeof(Cubes)/sizeof(Cubes[0]); i++)
    //         {

    //         }
            
    //         glfwSwapBuffers(window);
    //         glfwPollEvents();    
            
    //     };
        // glfwTerminate();


}

unsigned int * learning_stuff(){
    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f, 
        0.5f, 1.0f
    };


    int width, height, nrChannels;
    
    unsigned int * textures = new unsigned int[2];
    glGenTextures(1, &*textures);
    glBindTexture(GL_TEXTURE_2D, *textures);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char * data = stbi_load(TEXTURE2_PATH, &width, &height, &nrChannels, 0);

    if(data){
        try{
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        catch (int i){
            std::cout << IMAGE_BROKEN;
        }

    }
    else{
        std::cout << IMAGE_LOAD_ERROR;
    }
    stbi_image_free(data);
    glGenTextures(1, &*(textures+1));
    glBindTexture(GL_TEXTURE_2D, *(textures+1));
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load(TEXTURE1_PATH, &width, &height, &nrChannels, 0);

    if (data)
    {
        try {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } catch (int i) {
            std::cout << IMAGE_BROKEN; 
        }
        

    }    else{
        std::cout << IMAGE_LOAD_ERROR;
    }
    stbi_image_free(data);
    
    return textures;
}
