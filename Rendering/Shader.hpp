#pragma once
#include <string>
#include <unordered_map>
#include "vendor/glm/glm.hpp"



class Shader{
    public:
        Shader(const std::string &filepath);
        ~Shader();
        void Bind();
        void Unbind();

        
        void SetUniform1i(const std::string &name, int value);
        void SetUniform1iv(const std::string &name, int count, const int* values);
        void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

    private:
        unsigned int _id;
        std::unordered_map<std::string, int> UniformLocationCache;
        int GetUniformLocation(const std::string& name); 

};