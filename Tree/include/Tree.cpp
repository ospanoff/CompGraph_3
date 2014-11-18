//
//  Tree.cpp
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//

#include "Tree.h"

Leaf leaf;

Tree::Tree(int maxBranch, int brNum, int lNum, int height)
    :maxBranchOnOneBranch(maxBranch), branchNum(brNum), leafNum(lNum),treeHeight(height)
{
    treeInfo.resize(branchNum);
    curNum = 0;
    period = 3; // seconds
    treeInfo[0].parent = &treeInfo[0];
    treeInfo[0].offsX = 0;
    treeInfo[0].offsY = 0;
    treeInfo[0].offsZ = 0;
    treeInfo[0].angPhi = 0;
    treeInfo[0].angTheta = 0;
    treeInfo[0].children.resize(maxBranchOnOneBranch);
    treeInfo[0].childNum = 0;
    treeInfo[0].len = 0;
    treeInfo[0].maxLen = treeHeight;
    treeInfo[0].speed = 2.0f;
    treeInfo[0].leaves.resize(leafNum);
    for (int i = 0; i < leafNum; i++) {
        treeInfo[curNum].leaves[i].push_back(0.4 + (float) (i * 0.6) / (float) (leafNum));//(0.5 + (float) (rand() % 4) / (float) 10);
        treeInfo[curNum].leaves[i].push_back(rand() % 90);
        treeInfo[curNum].leaves[i].push_back(rand() % 360);
        //        std::cout << treeInfo[curNum].leaves[i][1] << std::endl;
    }
}

void Tree::load(double time)
{
    lastTime = time;
    leaf.load();
    branch.load();
}

void Tree::genMainBranch(glm::mat4 mModelView)
{
    treeInfo[0].MVMatrix = mModelView;
}

void Tree::genBranch()
{
    srand((int)time(NULL));
//    std::cout << rand() << " ";
    curNum++;
    int num = 0;
    do {
        num = rand() % (curNum > maxBranchOnOneBranch ? curNum : 1);
        if (num == 0) {
            bool a180 = false, a270 = false, a360 = false;

            for (int i = 0; i < treeInfo[num].childNum; i++) {
                float phi = treeInfo[num].children[i]->angPhi;
                if (90.0f <= phi && phi < 180.0f) {
                    a180 = true;
                } else if (180.0f <= phi && phi < 270.0f) {
                    a270 = true;
                } else if (270.0f <= phi && phi < 360.0f) {
                    a360 = true;
                }
            }
            treeInfo[curNum].angPhi = rand() % 90;
            if (!a180) {
                treeInfo[curNum].angPhi += 90;
                break;
            } else if (!a270) {
                treeInfo[curNum].angPhi += 180;
                break;
            } else if (!a360) {
                treeInfo[curNum].angPhi += 270;
                break;
            }
            break;
        }
//        if (num == 1)
//            break;
    } while (treeInfo[num].childNum == maxBranchOnOneBranch ||
             (treeInfo[curNum - 1].parent == &treeInfo[num] && (curNum - 1 == num)));
    treeInfo[curNum].parent = &treeInfo[num];
    treeInfo[num].children[treeInfo[num].childNum++] = &treeInfo[curNum];

    double t = (0.25 + (float) (rand() % 7) / (float) 10);
    
    treeInfo[curNum].offsX = 0;
    treeInfo[curNum].offsY = t * treeInfo[curNum].parent->len;
    treeInfo[curNum].offsY_0 = t;
    treeInfo[curNum].offsZ = 0;
    treeInfo[curNum].angTheta = 10.0f + rand() % 60;
    treeInfo[curNum].childNum = 0;
    treeInfo[curNum].len = 0;
    treeInfo[curNum].maxLen = (1 - t) * treeInfo[curNum].parent->maxLen;
    treeInfo[curNum].speed = (1 - t) * treeInfo[curNum].parent->speed;
    treeInfo[curNum].children.resize(maxBranchOnOneBranch);
    treeInfo[curNum].leaves.resize(leafNum);
    for (int i = 0; i < leafNum; i++) {
        treeInfo[curNum].leaves[i].push_back(0.25 + (float) (i * 0.75) / (float) (leafNum));
        treeInfo[curNum].leaves[i].push_back(rand() % 90);
        treeInfo[curNum].leaves[i].push_back(rand() % 360);
//        std::cout << treeInfo[curNum].leaves[i][1] << std::endl;
    }
    
//    std::cout << curNum << " " << num << "; " << t << " " << treeInfo[curNum].maxLen << " " << treeInfo[curNum].parent->maxLen << std::endl;
}

void Tree::drawBranch(branchInfo &binfo, GLint iModelViewLoc)
{
    mCurrent = glm::translate(binfo.parent->MVMatrix, glm::vec3(0, binfo.offsY, 0));
    mCurrent = glm::rotate(mCurrent, binfo.angPhi, glm::vec3(0.0f, 1.0f, 0.0f));
    if (binfo.angTheta + binfo.parent->angTheta > 90.0f)
        binfo.angTheta = -binfo.angTheta;
    mCurrent = glm::rotate(mCurrent, binfo.angTheta, glm::vec3(0.0f, 0.0f, 1.0f));
    binfo.MVMatrix = mCurrent;
    mCurrent = glm::scale(mCurrent,
                          glm::vec3(binfo.len / 50, // X
                                    binfo.len / 1, // Y
                                    binfo.len / 50)); // Z
    mCurrent = glm::rotate(mCurrent, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    
    if (&binfo != binfo.parent && binfo.parent->len < binfo.parent->maxLen)
        binfo.offsY += speedCoef * binfo.parent->speed * binfo.offsY_0;
    binfo.offsX = binfo.len / 50;
    binfo.offsZ = binfo.len / 50;

    if (binfo.len < binfo.maxLen) {
        binfo.len += speedCoef * binfo.speed;
    }
    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
    branch.renderBranch();
}

void Tree::drawLeaf(GLint iModelViewLoc)
{
//    for (int i = 1; i <= curNum; i++) {
//        float t = treeInfo[i].len / 50;
//        for (int j = 0; j < leafNum; j++) {
//
//            mCurrent = glm::translate(treeInfo[i].MVMatrix, glm::vec3(0, 0, 0));
//            mCurrent = glm::rotate(mCurrent, treeInfo[i].angPhi + treeInfo[i].leaves[j][2], glm::vec3(0.0f, 1.0f, 0.0f));
//            mCurrent = glm::translate(mCurrent, glm::vec3((t - 1)/2, treeInfo[i].leaves[j][0] * treeInfo[i].len, 0));
//            mCurrent = glm::rotate(mCurrent, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//            mCurrent = glm::rotate(mCurrent, treeInfo[i].angTheta + treeInfo[i].leaves[j][1], glm::vec3(1.0f, 0.0f, 0.0f));
//        
//            mCurrent = glm::translate(mCurrent, glm::vec3(0, 0, (t-1)/2));
////            t = (1 - treeInfo[i].leaves[j]) * 100;
//            mCurrent = glm::scale(mCurrent, glm::vec3(1 - t, 1 - t, 1 - t));
//        
//            glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
//            leaf.renderLeaf();
//        }
//    }
    for (int i = 0; i <= curNum; i++) {
        
        float t = treeInfo[i].len / 45;
        
        for (int j = 0; j < leafNum; j++) {

            mCurrent = glm::translate(treeInfo[i].MVMatrix, glm::vec3(0, treeInfo[i].leaves[j][0] * treeInfo[i].len, 0));
//            std::cout << treeInfo[i].leaves[j][0] << std::endl;
            mCurrent = glm::rotate(mCurrent, treeInfo[i].angPhi + treeInfo[i].leaves[j][2], glm::vec3(0.0f, 1.0f, 0.0f));

            mCurrent = glm::rotate(mCurrent, treeInfo[i].angTheta + treeInfo[i].leaves[j][1], glm::vec3(1.0f, 0.0f, 0.0f));
//            mCurrent = glm::translate(mCurrent, glm::vec3(0, , 0));
//            t = 1.1 * t;
            mCurrent = glm::scale(mCurrent, glm::vec3(1 - t, (1 - t)/2, 0));
            mCurrent = glm::translate(mCurrent, glm::vec3(2 * (1 - treeInfo[i].leaves[j][0] / 2), 0, 0));

            glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
            leaf.renderLeaf();
        }
    }
}

void Tree::render(GLint iModelViewLoc, double time)
{
    if (time - lastTime > period / 3) {
        lastTime = time;
        if (curNum < branchNum)
            genBranch();
//        std::cout << curNum << " ";
    }
    for (int i = 0; i <= curNum; i++) {
        drawBranch(treeInfo[i], iModelViewLoc);
    }
}
