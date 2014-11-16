//
//  OGLApp.h
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#ifndef __Tree__OGLApp__
#define __Tree__OGLApp__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.h"
#include "VertexBufferObject.h"
#include "TextureLoader.h"
#include "Skybox.h"
#include "Camera.h"

class OGLApp {
public:
    
    OGLApp(GLint w, GLint h, bool isFullScreen, const char *appTitle);
    ~OGLApp();
    
    bool init();
    void run();
    void renderScene();
    void initScene();
    void keyHandler();
    
    bool glfwBackendInit();
    bool glfwBackendCreateWindow();
    
    void initCallbacks();
    static void keyboardCB(GLFWwindow *, int key, int scanCode, int action, int mods);
    static void mouseCB(GLFWwindow *, double xPos, double yPos);
    
    friend void displayTextureFiltersInfo();
    

private:
    static GLFWwindow *_mainWindow;
    static GLint _width;
    static GLint _height;
    bool _isFS;
    const char *_appTitle;
    
    ShaderLoader shader;
    CSkybox sbMainSkybox;
    CVertexBufferObject vboSceneObjects;
    GLuint uiVAO;
    static CTexture tGold, tSnow;
    static CFlyingCamera camera;
    
    static int camType;
    
    static float phi, theta, rad;
};

#endif /* defined(__Tree__OGLApp__) */
