#include "VertexArray.hpp"
#include <GL/glew.h>


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &_id);
    delete _IB;
    delete _VB;
}

void VertexArray::Bind(){
    glBindVertexArray(_id);
}

void VertexArray::Unbind(){
    glBindVertexArray(0);
}

void VertexArray::Package(const unsigned int* indices, int iCount, const float* vertices, int vCount, std::vector<VertexType> types){
    _IB = new IndexBuffer(indices, iCount);
    _VB = new VertexBuffer(vertices, vCount);
    for(VertexType type : types){
        _VB->addVertexType(type.offset, type.stride, type.count);
    }
}

void VertexArray::Repackage(const unsigned int* indices, int iCount, const float* vertices, int vCount){
    Bind();
    _IB->editIndices(indices, iCount);
    _VB->editVertices(vertices, vCount);
}

IndexBuffer::IndexBuffer(const unsigned int* indices, int count){
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer(){
    glDeleteBuffers(1, &_id);
}


void IndexBuffer::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::editIndices(const unsigned int* indices, int count){
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const float* vertices, int count){
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
}


VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &_id);
}

void VertexBuffer::addVertexType(const int offset, const int stride, const int count){
    glVertexAttribPointer(_type, count, GL_FLOAT, GL_FALSE, stride*sizeof(float), (const void*) (offset*sizeof(float)));
    glEnableVertexAttribArray(_type);
    _type++;
}

void VertexBuffer::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}
void VertexBuffer::Unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::editVertices(const float* vertices, int count){
    Bind();
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
}