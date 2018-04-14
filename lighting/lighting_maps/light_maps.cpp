//
//  light.cpp
//  opengl
//
//  Created by lqlin on 2018/4/1.
//  Copyright © 2018年 lqlin. All rights reserved.
//

#include "util/prefix.hpp"
#include "util/callback.hpp"
#include "shader/Shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "util/camera.h"

namespace LIGHT3 {

const char *vertexSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0f);\n"
"}\0";

const char *frameSource2 = "#version 330 core\n"
"struct Material {\n"
    "   vec3 ambient;\n"
    "   vec3 diffuse;\n"
    "   vec3 specular;\n"
    "   float shininess;\n"
    "};\n"
"struct Light {\n"
    "   vec3 ambient;\n"
    "   vec3 diffuse;\n"
    "   vec3 specular;\n"
    "   vec3 position;\n"
    "};\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"out vec4 FragColor;\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform Material material;\n"
"uniform Light light;\n"
"void main()\n"
"{\n"
"   vec3 ambient = light.ambient * material.ambient;\n"
    
"   vec3 norm = normalize(Normal);\n"
"   vec3 lightDir = normalize(light.position - FragPos);\n"
    
"   float diff = max(dot(norm, lightDir), 0.0);\n"
"   vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
    
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, norm);\n"
    
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"   vec3 specular = light.specular * (spec * material.specular);\n"
    
"   vec3 result = ambient + diffuse + specular;\n"
"   FragColor = vec4(result, 1.0);\n"
//    "FragColor = vec4(1.0);\n"
"}\0";

const char *lampVertexSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

const char *lampFragSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0);\n"
"}\0";

glm::vec3 lightPos(1.2, 1, 2);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX2 = 800 / 2.0f;
float lastY2 = 600 / 2.0f;
bool firstMouse = true;

// timing
float deltaTime2 = 0.0f;
float lastFrame2 = 0.0f;

void processInput3(GLFWwindow *window);
void mouse_callback2(GLFWwindow* window, double xpos, double ypos);
void scroll_callback2(GLFWwindow* window, double xoffset, double yoffset);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "light", NULL, NULL);
    if (window == 0) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, LIGHT3::mouse_callback2);
    glfwSetScrollCallback(window, LIGHT3::scroll_callback2);
    
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    unsigned int cubeVAO,VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Shader lightShader(LIGHT3::vertexSource2, LIGHT3::frameSource2, 0);
    Shader lampShader(LIGHT3::lampVertexSource, LIGHT3::lampFragSource, 0);
    
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        LIGHT3::deltaTime2 = currentFrame - LIGHT3::lastFrame2;
        LIGHT3::lastFrame2 = currentFrame;
        
        // 1. change lightPos
        LIGHT3::lightPos.x = sin(glfwGetTime());
        LIGHT3::lightPos.y = cos(glfwGetTime());
        
        LIGHT3::processInput3(window);
        
        glClearColor(0.1,0.1,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightShader.use();
        
        // light
        lightShader.setVec3("light.position", LIGHT3::lightPos);
        lightShader.setVec3("viewPos", LIGHT3::camera.Position);
        
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        lightShader.setVec3("light.ambient", ambientColor);
        lightShader.setVec3("light.diffuse", diffuseColor);
        lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
//        lightShader.setVec3("objectColor", 0.8, 0.5, 0.31);
//        lightShader.setVec3("lightColor", 1, 1, 1);
        
        // material
        lightShader.setVec3("material.ambient", 1.0, 0.5, 0.31);
        lightShader.setVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
        lightShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightShader.setFloat("material.shininess", 32.0f);
        
        glm::mat4 projection = glm::perspective(glm::radians(LIGHT3::camera.Zoom), float(800.0/600.0), 0.1f, 100.0f);
        glm::mat4 view = LIGHT3::camera.GetViewMatrix();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        glm::mat4 model;
        model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1,1,0));
        lightShader.setMat4("model", model);
        
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        /* ============================== */
        
        lampShader.use();
        lampShader.setMat4("projection", projection);
        //        view = glm::mat4();
        lampShader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, LIGHT3::lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", model);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

namespace LIGHT3 {

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput3(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime2);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime2);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime2);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime2);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback2(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX2 = xpos;
        lastY2 = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX2;
    float yoffset = lastY2 - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX2 = xpos;
    lastY2 = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback2(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
}
