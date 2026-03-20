#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Quads.hpp"
#include "Texture.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

enum class RenderType{
    HomeScreen,
    currentLevel,
};


class Renderer{
    public:
        Renderer();
        ~Renderer();
        void setRender(RenderType type);
        RenderType currentRender() const {return _type;}
        void render();
        bool windowOpen();
        
        bool readKey(int key);
    private:
        void updateQuads();
        Quads* _quads;
        GLFWwindow* _window;
        VertexArray* _VAO;
        Shader* _shader;
        RenderType _type;
        std::vector<Texture*> _textures;
};