#pragma once
#include <vector>
#include "VertexArray.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Quads{
    public:
        Quads();

        std::vector<float> Vertices() const {return _vertices;}
        std::vector<unsigned int> Indices() const {return _indices;}
        std::vector<VertexType> Types() const {return _types;}

        void makeSquare(
            float r, float g, float b, float a,
            float textureIndex,
            glm::vec3 translation = glm::vec3(0.0f,0.0f,0.0f), 
            glm::vec3 size = glm::vec3(1.0f,1.0f,1.0f), 
            float degrees = 0,
            glm::vec3 postRotTranslation = glm::vec3(0.0f,0.0f,0.0f) 
        );

        void clear();
        
        static Quads* combine(Quads* q1, Quads* q2);


    private:
        std::vector<float> _vertices;
        std::vector<unsigned int> _indices;
        int _indexOffset = 0;
        std::vector<VertexType> _types;

};

