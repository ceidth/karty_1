#ifndef OPENGLPAG_VBO_H
#define OPENGLPAG_VBO_H

#include <glad/glad.h>
#include <vector>

class VBO
{
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(std::vector<GLfloat> vertices, GLsizeiptr size);

    // Binds the VBO
    void Bind();
    // Unbinds the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};

#endif //OPENGLPAG_VBO_H
