#include "Renderer.hpp"
#include <iostream>
#include "../simulation/ball.hpp"
#include "../simulation/level_grids.hpp"

Renderer::Renderer(){
    if (!glfwInit())
        std::cout << "Error";

    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(1000, 800, "Physics Game", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        std::cout << "Error";
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK){
        std::cout << "Error";
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    _quads = new Quads();

    _VAO = new VertexArray();
    _VAO->Package(_quads->Indices().data(), _quads->Indices().size(), _quads->Vertices().data(), _quads->Vertices().size(), _quads->Types());
    _shader = new Shader("Rendering/resources/Default.shader");
    _shader->SetUniform4f("uColor", .8f, .8f, .8f, 1.0f);


}

Renderer::~Renderer(){
    delete _quads;
    delete _VAO;
    delete _shader;
    _textures.clear();
    glfwTerminate();
}

void Renderer::setRender(RenderType type){
    _type = type;
    _textures.clear();
    switch (type){
        case RenderType::HomeScreen:{
            glm::mat4 proj = glm::ortho(0.0f, 1000.0f, 0.0f, 800.0f, -1.0f, 1.0f);
            _shader->SetUniformMat4f("uProj", proj);

            Texture* homescreen = new Texture(0, "Rendering/resources/textures/homescreen.png");
            Texture* homescreenB1 = new Texture(1, "Rendering/resources/textures/homescreenB1.png");
            Texture* homescreenB2 = new Texture(2, "Rendering/resources/textures/homescreenB2.png");
            Texture* homescreenB3 = new Texture(3, "Rendering/resources/textures/homescreenB3.png");
            _textures.push_back(homescreen);
            _textures.push_back(homescreenB1);
            _textures.push_back(homescreenB2);
            _textures.push_back(homescreenB3);
            

            const int samplerSize = _textures.size();
            int samplers[samplerSize];
            for(int i = 0 ; i < _textures.size() ; i++){
                samplers[i] = i;
            }

            _shader->SetUniform1iv("uTexture", samplerSize, samplers);
        }
        break;
        case RenderType::currentLevel:{  
            glm::mat4 proj = glm::ortho(0.0f, 86.0f, 0.0f, 35.0f, -1.0f, 1.0f);
            _shader->SetUniformMat4f("uProj", proj);

            Texture* normal = new Texture(0);
            Texture* ball = new Texture(1, "Rendering/resources/textures/ball.png");
            Texture* ballc1 = new Texture(2, "Rendering/resources/textures/ballc1.png");
            Texture* ballc2 = new Texture(3, "Rendering/resources/textures/ballc2.png");
            Texture* compass = new Texture(4, "Rendering/resources/textures/compass.png");

            _textures.push_back(normal);
            _textures.push_back(ball);
            _textures.push_back(compass);
            _textures.push_back(ballc1);
            _textures.push_back(ballc2);

            
            const int samplerSize = _textures.size();
            int samplers[samplerSize];
            for(int i = 0 ; i < _textures.size() ; i++){
                samplers[i] = i;
            }
            _shader->SetUniform1iv("uTexture", samplerSize, samplers);

            
        }
        break;
    }

}

void Renderer::updateQuads(){
    switch(_type){
        case RenderType::HomeScreen:{
            _quads->clear();
            int tex;
            if(homescreen::row == 0 && homescreen::col == 0)
                tex = 1;
            else if(homescreen::row == 0 && homescreen::col == 1)
                tex = 2;
            else if(homescreen::row == 1 && homescreen::col == 0)
                tex = 3;
            else
                tex = 0;
            
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, tex, glm::vec3(1000.0f/2.0f, 800.0f/2.0f, 0.0f), glm::vec3(1000.0f, 800.0f, 1.0f));
        }
        break;
        case RenderType::currentLevel:{
            float ball_x = ballPos::col+2.0f;
            float ball_y = levels::rows+1.5f - (ballPos::row+2.0f);
            _quads->clear();
            //background
            
            _quads->makeSquare(4.0f/255, 3.0f/255, 26.0f/255, 1.0f, 0, glm::vec3(43.0f, 17.5f, 0.0f), glm::vec3(86.0f, 35.0f, 1.0f));

            //ball
            int ballTex;
            if(signals::springed1)
                ballTex = 2;
            else if(signals::springed2)
                ballTex = 3;
            else
                ballTex = 1;
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, ballTex, glm::vec3(ball_x, ball_y, 0.0f), glm::vec3(4.0f, 4.0f, 1.0f), ballProp::theta);
            //grapple
            if(signals::grappled){
                _quads->makeSquare(0.2f, 0.2f, 0.2f, 1.0f, 0, glm::vec3((grapple::col+(ball_x+1.5f))/2.0f, (36-grapple::row + (ball_y))/2.0f, 0.0f), glm::vec3(.1f, grapple::radius+2.0f, 1.0f), grapple::theta), glm::vec3(0.0f, (grapple::radius+2.0f)/2.0f, 0.0f);
            }
            //level
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(levels::cols+.5f, levels::rows/2.0f, 0.0f), glm::vec3(1.0f, levels::rows, 1.0f));
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(0.5f, levels::rows/2.0f, 0.0f), glm::vec3(1.0f, levels::rows, 1.0f));
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(levels::cols/2.0f, 0.0f, 0.0f), glm::vec3(levels::cols, 1.0f, 1.0f));

            for(int i = 0; i < levels::rows ; i++){
                for(int j = 0; j < levels::cols ; j++){
                    if(levels::currentLevel[i][j] == 1){
                        _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(j+1.5f, levels::rows-i, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                    }
                    else if(levels::currentLevel[i][j] == 2){
                        _quads->makeSquare(0.0f, 1.0f, 1.0f, 1.0f, 0, glm::vec3(j+1.5f, levels::rows-i, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                    }
                    else if(levels::currentLevel[i][j] == 4){
                        _quads->makeSquare(1.0f, 0.0f, 0.0f, 1.0f, 0, glm::vec3(j+1.5f, levels::rows-i, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                    }

                }
            }

            //force compass
            _quads->makeSquare(1.0f, 1.0f, 1.0f, 1.0f, 4, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(5.0f, 5.0f, 1.0f));
            if(forceBar::force > 0)
                _quads->makeSquare(1.0f, 0.0f, 1.0f, 1.0f, 0, glm::vec3(forceCompass::col, 35-forceCompass::row-.5f, 0.0f), glm::vec3(2.5f, .2f, 1.0f), forceCompass::theta, glm::vec3(2.5f/2.0f, 0.0f, 0.0f));
            

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
        }
        break;
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