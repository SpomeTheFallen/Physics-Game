#include "Renderer.hpp"
#include <iostream>
#include "../simulation/ball.hpp"
#include "../simulation/level_grids.hpp"

enum class compassDir{
    n = 2,
    ne,
    e,
    se,
    s,
    sw,
    w,
    nw
};

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float ball_x = ballPos::col;
    float ball_y = l0Prop::rows - ballPos::row;

    _quads = new Quads();

    _VAO = new VertexArray();
    _VAO->Package(_quads->Indices().data(), _quads->Indices().size(), _quads->Vertices().data(), _quads->Vertices().size(), _quads->Types());
    
    
    Texture* normal = new Texture(0);
    Texture* ball = new Texture(1, "Rendering/resources/textures/ball.png");
    Texture* c = new Texture(10, "Rendering/resources/textures/compass.png");
    Texture* c1 = new Texture((unsigned int)compassDir::n, "Rendering/resources/textures/compassN.png");
    Texture* c2 = new Texture((unsigned int)compassDir::ne, "Rendering/resources/textures/compassNE.png");
    Texture* c3 = new Texture((unsigned int)compassDir::e, "Rendering/resources/textures/compassE.png");
    Texture* c4 = new Texture((unsigned int)compassDir::se, "Rendering/resources/textures/compassSE.png");
    Texture* c5 = new Texture((unsigned int)compassDir::s, "Rendering/resources/textures/compassS.png");
    Texture* c6 = new Texture((unsigned int)compassDir::sw, "Rendering/resources/textures/compassSW.png");
    Texture* c7 = new Texture((unsigned int)compassDir::w, "Rendering/resources/textures/compassW.png");
    Texture* c8 = new Texture((unsigned int)compassDir::nw, "Rendering/resources/textures/compassNW.png");

    _textures.push_back(normal);
    _textures.push_back(ball);
    _textures.push_back(c);
    _textures.push_back(c1);
    _textures.push_back(c2);
    _textures.push_back(c3);
    _textures.push_back(c4);
    _textures.push_back(c5);
    _textures.push_back(c6);
    _textures.push_back(c7);
    _textures.push_back(c8);

    _shader = new Shader("Rendering/resources/Default.shader");
    _shader->SetUniform4f("uColor", .8f, .8f, .8f, 1.0f);

    glm::mat4 proj = glm::ortho(0.0f, 86.0f, 0.0f, 35.0f, -1.0f, 1.0f);
    
    int samplers[_textures.size()];
    for(int i = 0 ; i < _textures.size() ; i++){
        samplers[i] = i;
    }
    _shader->SetUniform1iv("uTexture", _textures.size(), samplers);
    _shader->SetUniformMat4f("uProj", proj);
    
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
    //background
    
    _quads->makeSquare(4.0f/255, 3.0f/255, 26.0f/255, 1.0f, 0, glm::vec3(43.0f, 17.5f, 0.0f), glm::vec3(86.0f, 35.0f, 1.0f));

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
    if(forceCompass::north)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::n, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::northe)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::ne,glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::northw)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::nw,glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::east)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::e, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::southe)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::se,glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::south)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::s, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::southw)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::sw,glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else if(forceCompass::west)
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, (float)compassDir::w, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
    else    
        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 10, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));

    
    //energy bar
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(energyBar::col-1, 35-energyBar::row-4.5f, 0.0f), glm::vec3(.5f, 10.0f, 1.0f));
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(energyBar::col+1, 35-energyBar::row-4.5f, 0.0f), glm::vec3(.5f, 10.0f, 1.0f));
    for(int i = 0 ; i < 10 ; i++){
        if(energyBar::bar[i] == 1)
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(energyBar::col, 35-energyBar::row-i, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //force bar
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(forceBar::col-1, 35-forceBar::row-4.5f, 0.0f), glm::vec3(.5f, 10.0f, 1.0f));
    _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(forceBar::col+1, 35-forceBar::row-4.5f, 0.0f), glm::vec3(.5f, 10.0f, 1.0f));
    for(int i = 0 ; i < 10 ; i++){
        if(forceBar::bar[i] == 1)
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(forceBar::col, 35-forceBar::row-i, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }

   


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