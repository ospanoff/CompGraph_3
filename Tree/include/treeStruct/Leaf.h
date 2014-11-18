//
//  Leaf.h
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#ifndef __Tree__Leaf__
#define __Tree__Leaf__

#include <glm/glm.hpp>
#include "VertexBufferObject.h"
#include "TextureLoader.h"


class Leaf {
public:
    void load();
    void loadAlpha();
    void renderLeaf();
    void renderLeafAlpha();
private:
    GLuint VAO;
    GLuint VAOAlpha;
    CVertexBufferObject vboRenderData;
    CVertexBufferObject vboRenderDataAlpha;
    CTexture texture;
    CTexture textureAlpha;
};

#endif /* defined(__Tree__Leaf__) */
