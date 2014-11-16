#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glfw/glfw3.h>

#include <iostream>

class CFlyingCamera
{
public:
    glm::vec3 vEye, vView, vUp;
    float fSpeed;
    float fSensitivity; // How many degrees to rotate per pixel moved by mouse (nice value is 0.10)
    
    // Main functions
    void rotateWithMouse(double x, double y);
    void update();
    glm::mat4 look();

    void resetMouse();
    
    // Functions that get viewing angles
    float getAngleX(), getAngleY();
    
    void setUp(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity, GLFWwindow *window);
    
    // Constructors
    CFlyingCamera();
    CFlyingCamera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity, GLFWwindow *window);
    
private:
    GLFWwindow *window;
};