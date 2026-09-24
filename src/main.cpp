
#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iterator>
#include <string>
#include "Engine.h"

#define HEIGHT 640
#define WIDTH 500
#define NAME "My_Engine0.0.1"

#define TEXTURE1_PATH "../src/textures/awesomeface.png"
#define TEXTURE2_PATH "../src/textures/container.jpg"
#define SHADER_VERTEX_PATH "../src/shaders/Vertex_Shader.glsl"
#define SHADER_FRAGMENT_PATH "../src/shaders/Fragment_Shader.glsl"
unsigned int * learning_stuff();
void LearnMatrices(Shader * ourShader);


int main(){
    GLFWwindow * window = window_creating(HEIGHT, WIDTH, NAME);
    // float vertices[] = {
    //     // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    //     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
    // };  
    // unsigned int tris[] = {
    //     0, 1, 3,
    //     1, 2, 3
    // };
    stbi_set_flip_vertically_on_load(true);
    Shader my_mini_shader = Shader(SHADER_VERTEX_PATH, SHADER_FRAGMENT_PATH);
    if(my_mini_shader.get_debug() != ""){
        std::cout << my_mini_shader.get_debug();
        return -1;
    }
    unsigned int * texture;
    texture = learning_stuff();

    my_mini_shader.use_shader();
    glUniform1i(glGetUniformLocation(my_mini_shader.ID(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(my_mini_shader.ID(), "texture2"), 1);

//    unsigned int VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tris), tris, GL_STATIC_DRAW);

//     // position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     // texture coord attribute
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);    


    GameObject MyNewCube = GameObject::Cube(new Vector3(0.0f, 0.0f, 0.0f));
    Vector3 myRotation = Vector3(0.5f, 0.2f, 0.3f);
    float my_angle;
    if(window == nullptr) return -1;
    float comp = 1.0f;
    float comp_factor = 0.01f; 
    while(!glfwWindowShouldClose(window))
    {
        LearnMatrices(&my_mini_shader);
        glClearColor(0.2f,0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, *(texture+1));
        if(!my_mini_shader.use_shader()) {
            std::cout << my_mini_shader.get_debug();
            return -1;
        }   
        
        my_angle = ((float)glfwGetTime()/comp);
        comp += comp_factor;
        MyNewCube.Rotate(myRotation, my_angle);
        MyNewCube.Show(&my_mini_shader);
        glfwSwapBuffers(window);
        glfwPollEvents();    
        
    };
    glfwTerminate();


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
void LearnMatrices(Shader* ourShader) {
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    // std::cout << ourShader.ID() << " , ";
    (*ourShader).SetMat4("model", glm::value_ptr(model));
    (*ourShader).SetMat4("view", glm::value_ptr(view));
    (*ourShader).SetMat4("projection", glm::value_ptr(projection));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
}