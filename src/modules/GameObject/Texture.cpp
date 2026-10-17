#include "GameObject.hpp"
#include "../../core/Debugger/debug.hpp"
#include "../../core/linked_libraries/img_reading/stb_image.h"

Texture::Texture(const std::string new_path, std::vector <float> & new_text_coords): path(new_path){
    LOG("Texture generated", INFO);
    text_coords = new_text_coords;
    int width, height, nrChannels;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_borders);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_borders);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MARK_MIN);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_MARK_FAR);
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        try {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } catch (int i) {
            LOG("Image broken, can`t be loaded as texture!", WARN);
        }   
    }
    stbi_image_free(data);
    canUse = true;
}
// void Texture::LoadTexture(){
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, texture_id);
// }