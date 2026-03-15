#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Quads.hpp"
#include "Texture.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class Renderer{
    public:
        Renderer();
        ~Renderer();
        void render();
        bool windowOpen();
        
        bool readKey(int key);
    private:
        void updateQuads();
        Quads* _quads;
        GLFWwindow* _window;
        VertexArray* _VAO;
        Shader* _shader;
        std::vector<Texture*> _textures;
};