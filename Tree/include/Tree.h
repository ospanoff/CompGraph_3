//
//  Tree.h
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#ifndef __Tree__Tree__
#define __Tree__Tree__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <ctime>
#include <cmath>

#include "TextureLoader.h"
#include "ShaderLoader.h"

#include "Branch.h"
#include "Leaf.h"

struct branchInfo {
    branchInfo *parent;
    vector<branchInfo *> children;
    int childNum;
    float offsX;
    float offsY;
    float offsY_0;
    float offsZ;
    float angTheta;
    float angPhi;
    float len;
    float maxLen;
    float speed;
    vector<vector<float>> leaves;
    glm::mat4 MVMatrix;
};

class Tree {
public:
    Tree();
    Tree(int maxBranchOnOneBranch, int branchNum, int leafNum,int height);
    void load(double time);
    void genBranch();
    void genMainBranch(glm::mat4 mModelView);
    void render(GLint mvLoc, double time);
    void drawLeaf(GLint mvLoc, bool alpha);

private:
    Branch branch;
    const int branchNum;
    const int maxBranchOnOneBranch;
    vector<branchInfo> treeInfo;
    glm::mat4 mCurrent;
    float treeHeight;
    int curNum;
    double lastTime;
    double period;
    const int leafNum;
    void drawBranch(branchInfo &binfo, GLint iModelViewLoc);
public:
    float speedCoef;
};

#endif /* defined(__Tree__Tree__) */
