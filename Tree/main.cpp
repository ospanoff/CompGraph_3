//
//  main.cpp
//  Tree
//
//  Created by Ayat Ospanov on 16/11/14.
//  Copyright (c) 2014 Ayat Ospanov. All rights reserved.
//
#include "OGLApp.h"
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, const char * argv[]) {
    
    OGLApp *app = new OGLApp(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tree");
    if (!app->init()) {
        return -1;
    }
    
    app->run();
    
    return 0;
}
