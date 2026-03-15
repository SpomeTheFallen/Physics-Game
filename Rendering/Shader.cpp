#include "Shader.hpp"
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <sstream>


struct DefualtShaders{
    std::string vertex;
    std::string fragment;
};

DefualtShaders getShaders(const std::string &filePath){
    std::ifstream stream(filePath);

    std::string line;
    std::stringstream ss[2];

    int type = -1;

    while (getline(stream, line)){
        if(line.find("#vertex") != std::string::npos){
            type = 0;
        }
        else if(line.find("#fragment") != std::string::npos){
            type = 1;
        }
        else{
            ss[type] << line << '\n';
        }
    }

    return {ss[0].str() , ss[1].str()};
}

Shader::Shader(const std::string &filepath)
    :_id(0)
{
    _id = glCreateProgram();

    DefualtShaders shader = getShaders(filepath);


    const char* vertexSrc = shader.vertex.c_str();
    unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertexSrc, nullptr);
    glCompileShader(vertex_id);

    glAttachShader(_id, vertex_id);


    const char* fragmentSrc = shader.fragment.c_str();
    unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragmentSrc, nullptr);
    glCompileShader(fragment_id);
    
    glAttachShader(_id, fragment_id);

    glLinkProgram(_id);
    glValidateProgram(_id); 
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    glUseProgram(_id);
}

Shader::~Shader(){
    glDeleteProgram(_id);
}

void Shader::Bind(){
    glUseProgram(_id);
}

void Shader::Unbind(){
    glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string &name){
    if(UniformLocationCache.find(name) != UniformLocationCache.end()){
        return UniformLocationCache[name];
    }

    int location = glGetUniformLocation(_id, name.c_str());
    
    UniformLocationCache[name] = location;
    return location;
} 

void Shader::SetUniform1i(const std::string &name, int value){
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1iv(const std::string &name, int count, const int* values){
    glUniform1iv(GetUniformLocation(name), count, values);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3){
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix){
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


