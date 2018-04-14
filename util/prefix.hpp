#ifndef _PREFIX_H
#define _PREFIX_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

static char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec3 ourPos;\n"
    "out vec2 TexCoord;\n"
    "uniform float xoffset;\n"
    "uniform float yoffset;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x + xoffset,aPos.y + yoffset,aPos.z,1.0);\n"
    "   ourColor = aColor;\n"
    "   ourPos = aPos;\n"
    "   TexCoord = TexCoord;\n"
    "}\0";

static char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourPos;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture,TexCoord);\n"
    "}\n\0";

#endif //_PREFIX_H
