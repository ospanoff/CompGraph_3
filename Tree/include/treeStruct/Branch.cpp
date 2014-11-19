//
//  Branch.cpp
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#include "Branch.h"

void Branch::load()
{
    texture.loadTexture2D("data/textures/branch.jpg");
    //    for (int i = 0; i < 6; i++)	{
        texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
        texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    }
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vboRenderData.createVBO();
    vboRenderData.bindVBO();
    
    numberOfDots = 4;
    float rad = 1.0f;
    float step = 2 * rad / numberOfDots;
    
    glm::vec3 *branchVertices = new glm::vec3 [6*numberOfDots];
    
    for (int i = 0; i < numberOfDots; i += 1) {
        
        branchVertices[3 * i] = glm::vec3(0, 0, 1);
        branchVertices[3 * numberOfDots + 3 * i + 0] = glm::vec3(0, 0, 1);
        
        double x = step * i - rad;
        double y = sqrtf(rad * rad - x * x);
        branchVertices[3 * i + 1] = glm::vec3(x, y, 0);
        branchVertices[3 * numberOfDots + 3 * i + 1] = glm::vec3(-x, -y, 0);
        
        x = step * (i + 1) - rad;
        y = sqrtf(rad * rad - x * x);
        branchVertices[3 * i + 2] = glm::vec3(x, y, 0);
        branchVertices[3 * numberOfDots + 3 * i + 2] = glm::vec3(-x, -y, 0);
    }

    glm::vec2 branchTexCoords[3] = {
        glm::vec2(0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)
    };
    
    
    glm::vec3 branchNormals[6] =
    {
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    
    for (int i = 0; i < 6 * numberOfDots; i++) {
        vboRenderData.addData(&branchVertices[i], sizeof(glm::vec3));
        vboRenderData.addData(&branchTexCoords[i%3], sizeof(glm::vec2));
        vboRenderData.addData(&branchNormals[i/3], sizeof(glm::vec3));
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


void Branch::renderBranch()
{
    glBindVertexArray(VAO);
    for (int i = 0; i < 2 * numberOfDots; i++)	{
        texture.bindTexture();
        glDrawArrays(GL_TRIANGLE_STRIP, 3 * i, 3);
    }
}