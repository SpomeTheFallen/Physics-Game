//Vertex array = index buffer + vertex buffer + attribpointer
#pragma once
#include <vector>

struct VertexType{
    const int offset;
    const int stride;
    const int count;
};

class IndexBuffer{
    public:
        IndexBuffer(const unsigned int* indices, int count);
        ~IndexBuffer();
        void Bind();
        void Unbind();
        void editIndices(const unsigned int* indices, int count);
    private:
        unsigned int _id;
};

class VertexBuffer{
    public:
        VertexBuffer(const float* vertices, int count);
        ~VertexBuffer();
        void Bind();
        void editVertices(const float* vertices, int count);
        void addVertexType(const int offset, const int stride, const int count);
        void Unbind();
    private:
        unsigned int _id;
        int _type = 0;
};


class VertexArray{
    public:
        VertexArray();
        ~VertexArray();
        void Bind();
        void Unbind();
        void Package(const unsigned int* indices, int iCount, const float* vertices, int vCount, std::vector<VertexType> types);
        void Repackage(const unsigned int* indices, int iCount, const float* vertices, int vCount);
    private:
        unsigned int _id;
        IndexBuffer* _IB;
        VertexBuffer* _VB;
};



