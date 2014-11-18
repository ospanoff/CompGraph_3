//
//  Leaf.cpp
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#include "Leaf.h"

void Leaf::load()
{
    texture.loadTexture2D("data/textures/leafNA.jpg");
    texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
    texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vboRenderData.createVBO();
    vboRenderData.bindVBO();
    
    glm::vec3 leafVertices[] = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.9238f, -0.3826f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9238f, -0.3826f, 0.0f), glm::vec3(0.7071f, -0.7071f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.3826f, -0.9238f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3826f, -0.9238f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-0.3826f, -0.9238f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.3826f, -0.9238f, 0.0f), glm::vec3(-0.7071f, -0.7071f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.7071f, -0.7071f, 0.0f), glm::vec3(-0.9238f, -0.3826f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.9238f, -0.3826f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec3(-0.9238f, 0.3826f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.9238f, 0.3826f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(-0.3826f, 0.9238f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.3826f, 0.9238f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.3826f, 0.9238f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3826f, 0.9238f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.9238f, 0.3826f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9238f, 0.3826f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)
    };
    
    glm::vec2 *leafTexCoords = new glm::vec2 [16 * 3];
    
    for (int i = 0; i < 16 * 3; i++) {
        leafTexCoords[i] = glm::vec2(leafVertices[i].x / 2.5f + 0.5f, leafVertices[i].y / 2.5f + 0.5f);
    }
    
    glm::vec3 vSkyBoxNormals[16 * 3] =
    {
        glm::vec3(0.0f, 0.0f, -1.0f),
        //        glm::vec3(0.0f, 0.0f, 1.0f)
    };
    
    for (int i = 0; i < 16 * 3; i++) {
        vboRenderData.addData(&leafVertices[i], sizeof(glm::vec3));
        vboRenderData.addData(&leafTexCoords[i], sizeof(glm::vec2));
        vboRenderData.addData(&vSkyBoxNormals[i%1], sizeof(glm::vec3));
    }
    
    vboRenderData.uploadDataToGPU(GL_STATIC_DRAW);
    
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
}

void Leaf::renderLeaf()
{
    glBindVertexArray(VAO);
    texture.bindTexture();
    for (int i = 0; i < 16; i++)	{
        glDrawArrays(GL_TRIANGLES, 3 * i, 3);
    }
}

void Leaf::loadAlpha()
{
    texture.loadTexture2D("data/textures/leaf.png");
    //    for (int i = 0; i < 6; i++)	{
        texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
        texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    }
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vboRenderData.createVBO();
    vboRenderData.bindVBO();
    
    glm::vec3 leafVertices[6] = {
        glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
    };
    
    glm::vec2 leafTexCoords[6] = {
        glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)
    };
    
    glm::vec3 vSkyBoxNormals[6] =
    {
        glm::vec3(0.0f, 0.0f, -1.0f),
//        glm::vec3(0.0f, 0.0f, 1.0f)
    };
    
    for (int i = 0; i < 6; i++) {
        vboRenderData.addData(&leafVertices[i], sizeof(glm::vec3));
        vboRenderData.addData(&leafTexCoords[i], sizeof(glm::vec2));
        vboRenderData.addData(&vSkyBoxNormals[i%1], sizeof(glm::vec3));
    }
    
    vboRenderData.uploadDataToGPU(GL_STATIC_DRAW);
    
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
}


void Leaf::renderLeafAlpha()
{
//    glDepthMask(0);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(VAO);
    texture.bindTexture();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 2; i++)	{
        glDrawArrays(GL_TRIANGLES, 3 * i, 3);
    }
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
//    glDepthMask(1);
}