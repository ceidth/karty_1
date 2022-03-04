//
// Created by fujitsu on 02.03.2022.
//

#ifndef OPENGLPAG_TEXTURE_H
#define OPENGLPAG_TEXTURE_H

#include<glad/glad.h>
#include<stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
    GLuint ID;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    // Assigns a texture unit to a texture
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    // Binds a texture
    void Bind();
    // Unbinds a texture
    void Unbind();
    // Deletes a texture
    void Delete();
};

#endif //OPENGLPAG_TEXTURE_H
