#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <GL/glew.h>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

class ShaderLoader {
public:
    ShaderLoader();
    ShaderLoader(const char *fnameVert, const char *fnameFrag);
    ~ShaderLoader();
    void setPath(const char *fnameVert, const char *fnameFrag);
    void load();
    bool init();
    void use();
    GLint getUniformLocation(const char* uniformName);

protected:
    bool addShader(GLenum ShaderType, const char* fileName);
    bool finalize();
    
private:
    GLuint _shaderProg;
    const char *vertexFile;
    const char *fragmentFile;
};

#endif /* SHADERLOADER_H */
