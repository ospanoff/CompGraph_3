//
//  Branch.h
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#ifndef __Tree__Branch__
#define __Tree__Branch__

#include <GL/glew.h>

class Branch {
public:
    void renderSkybox();
    
    void releaseSkybox();
private:
    GLuint VAO;
//    CVertexBufferObject vboRenderData;
//    CTexture tTextures[6];
//    string sDirectory;
//    string sFront, sBack, sLeft, sRight, sTop, sBottom;
};

#endif /* defined(__Tree__Branch__) */
