#include "Camera.h"

const float PI = atan(1.0)*4.0;

CFlyingCamera::CFlyingCamera()
{
    vEye = glm::vec3(0.0f, 0.0f, 0.0f);
    vView = glm::vec3(0.0f, 0.0, -1.0f);
    vUp = glm::vec3(0.0f, 1.0f, 0.0f);
    fSpeed = 50.0f;
    fSensitivity = 0.1f;
}

CFlyingCamera::CFlyingCamera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity, GLFWwindow *win)
{
    window = win;
    vEye = a_vEye; vView = a_vView; vUp = a_vUp;
    fSpeed = a_fSpeed;
    fSensitivity = a_fSensitivity;
}

void CFlyingCamera::setUp(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity, GLFWwindow *win)
{
    window = win;
    vEye = a_vEye; vView = a_vView; vUp = a_vUp;
    fSpeed = a_fSpeed;
    fSensitivity = a_fSensitivity;
}

/*-----------------------------------------------
 
 Name:	rotateWithMouse
 
 Params:	none
 
 Result:	Checks for moving of mouse and rotates
 camera.
 
 ---------------------------------------------*/

void CFlyingCamera::rotateWithMouse(double x, double y)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int iCentX = (width)>>1,
    iCentY = (height)>>1;

    float deltaX = (float)(iCentX-x)*fSensitivity;
    float deltaY = (float)(iCentY-y)*fSensitivity;
//    if (deltaX || deltaY)
//        std::cout << deltaX << " " << deltaY << std::endl;
    
    if(deltaX != 0.0f)
    {
        vView -= vEye;
        vView = glm::rotate(vView, deltaX, glm::vec3(0.0f, 1.0f, 0.0f));
        vView += vEye;
    }
    if(deltaY != 0.0f)
    {
        glm::vec3 vAxis = glm::cross(vView-vEye, vUp);
        vAxis = glm::normalize(vAxis);
        float fAngle = deltaY;
        float fNewAngle = fAngle+getAngleX();
        if(fNewAngle > -89.80f && fNewAngle < 89.80f)
        {
            vView -= vEye;
            vView = glm::rotate(vView, deltaY, vAxis);
            vView += vEye;
        }
    }
    glfwSetCursorPos(window, iCentX, iCentY);
}

/*-----------------------------------------------
 
 Name:	getAngleY
 
 Params:	none
 
 Result:	Gets Y angle of camera (head turning left
 and right).
 
 ---------------------------------------------*/

float CFlyingCamera::getAngleY()
{
    glm::vec3 vDir = vView-vEye; vDir.y = 0.0f;
    glm::normalize(vDir);
    float fAngle = acos(glm::dot(glm::vec3(0, 0, -1), vDir))*(180.0f/PI);
    if(vDir.x < 0)fAngle = 360.0f-fAngle;
    return fAngle;
}

/*-----------------------------------------------
 
 Name:	getAngleX
 
 Params:	none
 
 Result:	Gets X angle of camera (head turning up
 and down).
 
 ---------------------------------------------*/

float CFlyingCamera::getAngleX()
{
    glm::vec3 vDir = vView-vEye;
    vDir = glm::normalize(vDir);
    glm::vec3 vDir2 = vDir; vDir2.y = 0.0f;
    vDir2 = glm::normalize(vDir2);
    float fAngle =  acos(glm::dot(vDir2, vDir))*(180.0f/PI);
    if(vDir.y < 0)fAngle *= -1.0f;
    return fAngle;
}

/*-----------------------------------------------
 
 Name:	update
 
 Params:	none
 
 Result:	Performs updates of camera - moving and
 rotating.
 
 ---------------------------------------------*/

void CFlyingCamera::update()
{
//    rotateWithMouse(x, y);
    
    // Get view direction
    glm::vec3 vMove = vView-vEye;
    vMove = glm::normalize(vMove);
    vMove *= fSpeed;
    
    glm::vec3 vStrafe = glm::cross(vView-vEye, vUp);
    vStrafe = glm::normalize(vStrafe);
    vStrafe *= fSpeed;
    
//    int iMove = 0;
    glm::vec3 vMoveBy;
    // Get vector of move
    
//    
//    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        fSunAngle -= M_PI / 4;
//    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//        fSunAngle += M_PI / 4;
//    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vMoveBy += vMove * 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vMoveBy -= vMove * 1.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vMoveBy -= vStrafe * 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vMoveBy += vStrafe * 1.0f;
    }
    vEye += vMoveBy; vView += vMoveBy;
}

/*-----------------------------------------------
 
 Name:	resetMouse
 
 Params:	none
 
 Result:	Sets mouse cursor back to the center of
 window.
 
 ---------------------------------------------*/

void CFlyingCamera::resetMouse()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    int iCentX = (width)>>1,
    iCentY = (height)>>1;
    glfwSetCursorPos(window, iCentX, iCentY);
}

/*-----------------------------------------------
 
 Name:	look
 
 Params:	none
 
 Result:	Returns proper modelview matrix to make
 camera look.
 
 ---------------------------------------------*/

glm::mat4 CFlyingCamera::look()
{
    return glm::lookAt(vEye, vView, vUp);
}