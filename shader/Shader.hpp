//
//  Shader.hpp
//  opengl
//
//  Created by lqlin on 2018/3/24.
//  Copyright © 2018年 lqlin. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include "../util/prefix.hpp"
#include "../glm/glm.hpp"

class Shader
{
public:
    Shader(const GLchar* vertexPath, const GLchar* framePath);
    Shader(const GLchar* vertexSource, const GLchar* fragSource, bool b);
    
    void setup(const GLchar* vertexSource, const GLchar* fragSource);
    
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
    unsigned int getId() { return Id; }
    
private:
    unsigned int Id;
};

#endif /* Shader_hpp */
