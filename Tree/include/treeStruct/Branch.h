//
//  Branch.h
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#ifndef __Tree__Branch__
#define __Tree__Branch__

#include <glm/glm.hpp>
#include "VertexBufferObject.h"
#include "TextureLoader.h"


class Branch {
public:
    void load();
    void renderBranch();
private:
    GLuint VAO;
    CVertexBufferObject vboRenderData;
    CTexture texture;
    int numberOfDots;
};

#endif /* defined(__Tree__Branch__) */
