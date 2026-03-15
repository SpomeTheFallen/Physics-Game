#include "Texture.hpp"
#include <GL/glew.h>
#include "vendor/stb_image/stb_image.h"

Texture::Texture(unsigned int slot, const std::string &path){
    glActiveTexture(GL_TEXTURE0 + slot);
    stbi_set_flip_vertically_on_load(1);
    unsigned char* localBuffer = stbi_load(path.c_str(), &_width, &_height, &_BPP, 4);

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);

    if(localBuffer) stbi_image_free(localBuffer);
    
}
Texture::Texture(unsigned int slot){
    glActiveTexture(GL_TEXTURE0 + slot);
    unsigned int white = 0xffffffff;

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);   
}

Texture::~Texture(){
    glDeleteTextures(1, &_id);
}



void Texture::Bind(unsigned int slot){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}