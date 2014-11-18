//
//  OGLApp.cpp
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#include "OGLApp.h"
#include "StaticGeometry.h"

//Leaf leaf;

CTexture OGLApp::tGold, OGLApp::tGround;
GLFWwindow *OGLApp::_mainWindow;
float OGLApp::phi, OGLApp::theta, OGLApp::rad;
GLint OGLApp::_width, OGLApp::_height;
CFlyingCamera OGLApp::camera;
int OGLApp::camType;

float fRotationAngleCube = 0.0f, fRotationAnglePyramid = 0.0f;
float fCubeRotationSpeed = 0.0f, fPyramidRotationSpeed = 0.0f;
float speedBranch = 1.0f, len = 0.0f;

void displayTextureFiltersInfo()
{
    char buf[255];
    string sInfoMinification[] =
    {
        "Nearest",
        "Bilinear"
    };
    string sInfoMagnification[] =
    {
        "Nearest",
        "Bilinear",
        "Nearest on closest mipmap",
        "Bilinear on closest mipmap",
        "Trilinear"
    };
    sprintf(buf, "Mag. Filter: %s, Min. Filter: %s", sInfoMinification[OGLApp::tGround.getMagnificationFilter()].c_str(),
            sInfoMagnification[OGLApp::tGround.getMinificationFilter()-2].c_str());
    std::cout << buf << std::endl;
}

OGLApp::OGLApp(GLint w, GLint h, bool isFullScreen, const char *appTitle)
    :tree(5, 100, 70, 1000)
{
    _width = w;
    _height = h;
    _isFS = isFullScreen;
    _appTitle = appTitle;
    phi = 0.0f;
    theta = 0.5f;
    rad = 900.0f;
    camType = 0;
}

OGLApp::~OGLApp()
{
    vboSceneObjects.releaseVBO();
    glDeleteVertexArrays(1, &uiVAO);
    
    tGold.releaseTexture();
    tGround.releaseTexture();
}

bool OGLApp::init()
{
    if (!glfwBackendInit())
        return false;
    if (!glfwBackendCreateWindow())
        return false;
    
    initCallbacks();
    initScene();
    
    camera.setUp(glm::vec3(0,10,50), glm::vec3(0,0,0), glm::vec3(0,1,0), 20.0f, 0.1f, _mainWindow);

    return true;
}

void OGLApp::initScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    vboSceneObjects.createVBO();
    glGenVertexArrays(1, &uiVAO);
    glBindVertexArray(uiVAO);
    
    vboSceneObjects.bindVBO();
    
    // Add cube to VBO
    
    for (int i = 0; i < 36; i++) {
        vboSceneObjects.addData(&vCubeVertices[i], sizeof(glm::vec3));
        vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
    }
    
    // Add pyramid to VBO
    
    for (int i = 0; i < 12; i++) {
        vboSceneObjects.addData(&vPyramidVertices[i], sizeof(glm::vec3));
        vboSceneObjects.addData(&vPyramidTexCoords[i%3], sizeof(glm::vec2));
    }
    
    // Add ground to VBO
    
    for (int i = 0; i < 6; i++) {
        vboSceneObjects.addData(&vGround[i], sizeof(glm::vec3));
        vCubeTexCoords[i] *= 5.0f;
        vboSceneObjects.addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
    }
    
    vboSceneObjects.uploadDataToGPU(GL_STATIC_DRAW);
    
    // Vertex positions start on zero index, and distance between two consecutive is sizeof whole
    // vertex data (position and tex. coord)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), 0);
    // Texture coordinates start right after positon, thus on (sizeof(glm::vec3)) index,
    // and distance between two consecutive is sizeof whole vertex data (position and tex. coord)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
    
    // Load shaders and create shader program
    
    shader.setPath("data/shaders/shader.vert", "data/shaders/shader.frag");
    shader.load();
    shader.use();
    
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    
    // Finally, load our texture
    
    tGold.loadTexture2D("data/textures/golddiag.jpg", true);
    tGold.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
    
    tGround.loadTexture2D("data/textures/grass.jpg", true);
    tGround.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
    glEnable(GL_TEXTURE_2D);
    
    sbMainSkybox.loadSkybox("data/skyboxes/jajlands1/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");
    
    tree.load(glfwGetTime());
}

void OGLApp::run()
{
    double lastTime = glfwGetTime();
    curTime = lastTime;
    int frame = 0;
    while (!glfwWindowShouldClose(_mainWindow)) {
        curTime = glfwGetTime();
        if (curTime - lastTime > 1) {
//            std::cout << frame << std::endl;
            frame = 0;
            lastTime = curTime;
        }
        frame++;
        renderScene();
        keyHandler();
    }
    
    glfwTerminate();
}

void OGLApp::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int iModelViewLoc = shader.getUniformLocation("modelViewMatrix");
    int iProjectionLoc = shader.getUniformLocation("projectionMatrix");
    
    glfwGetWindowSize(_mainWindow, &_width, &_height);
    glm::mat4 proj = glm::perspective(60.0f, (float) _width / (float) _height, 0.001f, 10000.0f);
    
    glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(proj));
    
    static glm::vec3 camPos;
    static glm::mat4 mModelView;
    static glm::vec3 targetPos;
    
    if (camType) {
        mModelView = camera.look();
        camPos = camera.vEye;
    } else {
        targetPos = glm::vec3(0.0f, 600, 0.0f);
        camPos = glm::vec3(rad * cosf(theta) * sinf(phi), 600 + rad * sinf(theta), rad * cosf(theta) * cosf(phi));
        mModelView = glm::lookAt(camPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));
        camera.vView = glm::vec3(0.0f, 0.0f, 0.0f);
        camera.vEye = camPos;
    }

    static glm::mat4 mCurrent;
    
//    camPos = glm::vec3(camPos.x, camPos.y + 5, camPos.z);
    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(glm::translate(mModelView, camPos)));
    sbMainSkybox.renderSkybox();

    
    // Texture binding - we set GL_ACTIVE_TEXTURE0, and then we tell fragment shader,
    // that gSampler variable will fetch data from GL_ACTIVE_TEXTURE0
    
//    tGold.bindTexture(0);
//    
//    // Rendering of cube
//    
//    mCurrent = glm::translate(mModelView, glm::vec3(-8.0f, 0.0f, 0.0f));
//    mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
//    mCurrent = glm::rotate(mCurrent, fRotationAngleCube, glm::vec3(1.0f, 0.0f, 0.0f));
//    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
//    
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    
//    // Rendering of pyramid
//    
//    mCurrent = glm::translate(mModelView, glm::vec3(8.0f, 0.0f, 0.0f));
//    mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
//    mCurrent = glm::rotate(mCurrent, fRotationAnglePyramid, glm::vec3(0.0f, 1.0f, 0.0f));
//    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
//    
//    glDrawArrays(GL_TRIANGLES, 36, 12);

    // Render ground
    glBindVertexArray(uiVAO);
    int iSamplerLoc = shader.getUniformLocation("gSampler");
    glUniform1i(iSamplerLoc, 0);
    tGround.bindTexture();
    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mModelView));
    glDrawArrays(GL_TRIANGLES, 48, 6);
    tree.genMainBranch(mModelView);
    tree.render(iModelViewLoc, curTime);
    tree.drawLeaf(iModelViewLoc);
    tree.speedCoef = speedBranch;

    
//    fRotationAngleCube += fCubeRotationSpeed;
//    fRotationAnglePyramid += fPyramidRotationSpeed;

//    glm::rotate(mModelView, (float) (rand() % 10), glm::vec3(1.0f, 0.0f, 0.0f))

    camera.update();
    
    glfwSwapBuffers(_mainWindow);
    glfwPollEvents();
}

void OGLApp::keyHandler()
{
    if (!camType) {
        if (glfwGetKey(_mainWindow, GLFW_KEY_W)) {
            if (theta < M_PI/2 - 0.01f)
                theta += 0.01f;
        }
        if (glfwGetKey(_mainWindow, GLFW_KEY_S)) {
            if (theta > 0.02f)
                theta -= 0.01f;
        }
        if (glfwGetKey(_mainWindow, GLFW_KEY_A)) {
            phi -= 0.01f;
        }
        if (glfwGetKey(_mainWindow, GLFW_KEY_D)) {
            phi += 0.01f;
        }
    }
}

void OGLApp::keyboardCB(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_UP:
            fCubeRotationSpeed -= M_PI / 2;
            break;
        case GLFW_KEY_DOWN:
            fCubeRotationSpeed += M_PI / 2;
            break;
        case GLFW_KEY_RIGHT:
            fPyramidRotationSpeed += M_PI / 2;
            break;
        case GLFW_KEY_LEFT:
            fPyramidRotationSpeed -= M_PI / 2;
            break;
        // F1 and F2 change the texture filterings and set window text about that
        case GLFW_KEY_F1:
            if (action == GLFW_PRESS) {
                tGold.setFiltering((tGold.getMagnificationFilter()+1)%2, tGold.getMinificationFilter());
                tGround.setFiltering((tGround.getMagnificationFilter()+1)%2, tGround.getMinificationFilter());
                displayTextureFiltersInfo();
                break;
            }
        case GLFW_KEY_F2:
            if (action == GLFW_PRESS) {
                int iNewMinFilter = tGround.getMinificationFilter() == TEXTURE_FILTER_MIN_TRILINEAR ? TEXTURE_FILTER_MIN_NEAREST : tGround.getMinificationFilter()+1;
                tGround.setFiltering(tGround.getMagnificationFilter(), iNewMinFilter);
                tGold.setFiltering(tGold.getMagnificationFilter(), iNewMinFilter);
                displayTextureFiltersInfo();
                break;
            }
        case GLFW_KEY_E:
            if (action == GLFW_PRESS)
                camType = (camType + 1) % 2;
            break;
        case GLFW_KEY_F:
            rad += 10.0f;
            break;
        case GLFW_KEY_R:
            if (rad > 1.0f)
                rad -= 10.0f;
            break;
        case GLFW_KEY_Z:
            camera.fSpeed -= 1.0f;
            break;
        case GLFW_KEY_X:
            camera.fSpeed += 1.0f;
            break;
        case GLFW_KEY_COMMA:
            if (speedBranch > 0.1f) {
                speedBranch -= 1.0f;
            }
            break;
        case GLFW_KEY_PERIOD:
            speedBranch += 1.0f;
            break;

    }
}

void OGLApp::mouseCB(GLFWwindow *window, double xPos, double yPos)
{
    camera.rotateWithMouse(xPos, yPos);
}

void OGLApp::initCallbacks()
{
    glfwSetKeyCallback(_mainWindow, keyboardCB);
    glfwSetCursorPosCallback(_mainWindow, mouseCB);
}

bool OGLApp::glfwBackendInit()
{
    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    return true;
}

bool OGLApp::glfwBackendCreateWindow()
{
    _mainWindow = glfwCreateWindow(_width, _height, _appTitle, (_isFS ? glfwGetPrimaryMonitor() : NULL), NULL);
    if (_mainWindow == NULL) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(_mainWindow);
    glfwSetInputMode(_mainWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE; // for core profile
    GLenum res = glewInit();
    if (res != GLEW_OK){
        std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        return false;
    }
    
    return true;
}
