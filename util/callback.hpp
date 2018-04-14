//
//  callback.hpp
//  opengl
//
//  Created by lqlin on 2018/3/24.
//  Copyright © 2018年 lqlin. All rights reserved.
//

#ifndef callback_hpp
#define callback_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//void processInput(GLFWwindow *window);

inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0,width,height);
}

inline void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

#endif /* callback_hpp */
