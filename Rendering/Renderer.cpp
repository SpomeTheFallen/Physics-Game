#include "Renderer.hpp"
#include <iostream>
#include "../simulation/ball.hpp"
#include "../simulation/level_grids.hpp"

Renderer::Renderer(){
    if (!glfwInit())
        std::cout << "Error";

    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(800, 800, "Physics Game", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        std::cout << "Error";
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
    
    if(glewInit() != GLEW_OK){
        std::cout << "Error";
    }

    float ball_x = ballPos::col;
    float ball_y = l0Prop::rows - ballPos::row;

    _quads = new Quads();

    _VAO = new VertexArray();
    _VAO->Package(_quads->Indices().data(), _quads->Indices().size(), _quads->Vertices().data(), _quads->Vertices().size(), _quads->Types());
    
    
    Texture* normal = new Texture(0);
    Texture* ball = new Texture(1, "Rendering/resources/textures/ball.png");
    _textures.push_back(normal);
    _textures.push_back(ball);

    _shader = new Shader("Rendering/resources/Default.shader");
    _shader->SetUniform4f("uColor", 1.0f, 1.0f, 1.0f, 1.0f);

    glm::mat4 proj = glm::ortho(0.0f, 86.0f, 0.0f, 35.0f, -1.0f, 1.0f);
    int samplers[2] = {0, 1};
    
    _shader->SetUniformMat4f("uProj", proj);
    _shader->SetUniform1iv("uTexture", 2, samplers);
}

Renderer::~Renderer(){
    delete _quads;
    delete _VAO;
    delete _shader;
    _textures.clear();
    glfwTerminate();
}


void Renderer::updateQuads(){
    float ball_x = ballPos::col;
    float ball_y = l0Prop::rows-1 - ballPos::row;
    _quads->clear();
    //ball
    _quads->makeSquare(1.0f, 0.0f, 1.0f, 1.0f, 1, glm::vec3(ball_x+1.5f, ball_y-.5f, 0.0f), glm::vec3(2.0f, 2.0f, 1.0f));

    //level
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(l0Prop::cols+.5f, l0Prop::rows/2.0f, 0.0f), glm::vec3(2.0f, l0Prop::rows, 1.0f));
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(0.5f, l0Prop::rows/2.0f, 0.0f), glm::vec3(2.0f, l0Prop::rows, 1.0f));
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(l0Prop::cols/2.0f, 0.0f, 0.0f), glm::vec3(l0Prop::cols, 1.0f, 1.0f));

    for(int i = 0; i < l0Prop::rows ; i++){
        for(int j = 0; j < l0Prop::cols ; j++){
            if(level0[i][j] == 1){
                _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(j+1.5f, l0Prop::rows-i, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
            }
        }
    }

    //force compass
    //_quads->makeSquare();

    _VAO->Repackage(_quads->Indices().data(), _quads->Indices().size(), _quads->Vertices().data(), _quads->Vertices().size());
}

void Renderer::render(){
    updateQuads();
    //while (!glfwWindowShouldClose(_window)){
        glClear(GL_COLOR_BUFFER_BIT);

        _VAO->Bind();
        _shader->Bind();

        glDrawElements(GL_TRIANGLES, _quads->Indices().size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(_window);

        glfwPollEvents();
    //}
    
}

bool Renderer::windowOpen(){
    if(!glfwWindowShouldClose(_window))
        return true;
    else 
        return false;
}
//Pass in GLFW_KEY
bool Renderer::readKey(int key){
    if (glfwGetKey(_window, key) == GLFW_PRESS)
        return true;
    return false;
}