#pragma once
#include <string>

class Texture{
    public:
        Texture(unsigned int slot, const std::string &path);
        Texture(unsigned int slot);
        ~Texture();
        void Bind(unsigned int slot);
        void Unbind();
    private:
        unsigned int _id;
        int _width, _height, _BPP;
};