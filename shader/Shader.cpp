//
//  Shader.cpp
//  opengl
//
//  Created by lqlin on 2018/3/24.
//  Copyright © 2018年 lqlin. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader(const GLchar* vertexPath, const GLchar* framePath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(framePath);
        std::stringstream vShaderStream,fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
    }
    
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    
    setup(vShaderCode, fShaderCode);
}

Shader::Shader(const GLchar* vertexSource, const GLchar* fragSource, bool b)
{
    setup(vertexSource, fragSource);
}

void Shader::setup(const GLchar *vShaderCode, const GLchar *fShaderCode)
{
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    Id = glCreateProgram();
    glAttachShader(Id, vertex);
    glAttachShader(Id, fragment);
    glLinkProgram(Id);
    glGetProgramiv(Id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
//    std::cout<<"use ID: "<<Id<<std::endl;
    glUseProgram(Id);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(Id, name.c_str()), (float)value);
}

void Shader::setInt(const std::string &name,int value) const
{
    glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    setVec3(name, vec.x, vec.y, vec.z);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
