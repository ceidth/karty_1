//
// Created by fujitsu on 22.10.2021.
//

#ifndef OPENGLPAG_EBO_H
#define OPENGLPAG_EBO_H

#include<glad/glad.h>
#include <vector>

class EBO
{
public:
    // ID reference of Elements Buffer Object
    GLuint ID;
    // Constructor that generates a Elements Buffer Object and links it to indices
    EBO(std::vector<GLuint> indices, GLsizeiptr size);

    // Binds the EBO
    void Bind();
    // Unbinds the EBO
    void Unbind();
    // Deletes the EBO
    void Delete();
};

#endif //OPENGLPAG_EBO_H
