#include "Quads.hpp"


Quads::Quads(){
    _types.push_back({0, 9, 2});
    _types.push_back({2, 9, 4});
    _types.push_back({6, 9, 2});
    _types.push_back({8, 9, 1});
}

void Quads::makeSquare(float r, float g, float b, float a, float textureIndex, glm::vec3 translation, glm::vec3 size, float degrees){
    unsigned int indices[6] ={
        0 , 1 , 2,
        2, 1, 3,
    };


    glm::vec2 localPos[4] = {
        {-.5f ,-.5f},
        { .5f ,-.5f}, 
        {-.5f , .5f}, 
        { .5f , .5f},   
    };

    glm::vec2 texturePos[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f}, 
        {0.0f, 1.0f},  
        {1.0f, 1.0f}, 
    };

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(degrees), glm::vec3(0,0,1));
    model = glm::scale(model, size);

    for(int i = 0; i < 4; i++)
    {
        glm::vec4 world = model * glm::vec4(localPos[i], 0, 1);

        _vertices.push_back(world.x);
        _vertices.push_back(world.y);
        _vertices.push_back(r);
        _vertices.push_back(g);
        _vertices.push_back(b);
        _vertices.push_back(a);
        _vertices.push_back(texturePos[i].x);
        _vertices.push_back(texturePos[i].y);
        _vertices.push_back(textureIndex);
  
    }
    
    for(int i = 0; i < 6; i++)
        _indices.push_back(indices[i] + _indexOffset);

    _indexOffset += 4;
}

void Quads::clear(){
    _indexOffset = 0;
    _vertices.clear();
    _indices.clear();
}

Quads* Quads::combine(Quads* q1, Quads* q2){
    return q1;
}