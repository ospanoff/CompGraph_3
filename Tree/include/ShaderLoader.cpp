#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(){
    _shaderProg = 0;
}

ShaderLoader::ShaderLoader(const char *fn1, const char *fn2)
    :_shaderProg(0), vertexFile(fn1), fragmentFile(fn2)
{
}

ShaderLoader::~ShaderLoader(){
    if (_shaderProg != 0){
        glDeleteProgram(_shaderProg);
        _shaderProg = 0;
    }
    delete vertexFile;
    delete fragmentFile;
}

void ShaderLoader::setPath(const char *fnameVert, const char *fnameFrag)
{
    vertexFile = fnameVert;
    fragmentFile = fnameFrag;
}

void ShaderLoader::load(){
    if (!init()) {
        std::cerr << "Can't create shader program";
        return;
    }
    addShader(GL_VERTEX_SHADER, vertexFile);
    addShader(GL_FRAGMENT_SHADER, fragmentFile);
    finalize();
}


bool ShaderLoader::init(){
    _shaderProg = glCreateProgram();
    
    if (_shaderProg == 0){
        fprintf(stderr, "Error creating shader program\n");
        return false;
    }
    
    return true;
}

//Используем этот метод для добавления шейдеров в программу. Когда заканчиваем - вызываем finalize()
bool ShaderLoader::addShader(GLenum ShaderType, const char* filePath){
    GLuint ShaderObj = glCreateShader(ShaderType);
    
    if (ShaderObj == 0){
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        return false;
    }
    
    // read from file
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    
    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }
    
    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    
    fileStream.close();
    
    // end read from file
    
    const char *pShaderText = content.c_str();
    
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = (GLint) strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    
    glCompileShader(ShaderObj);
    
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    
    if (!success){
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        return false;
    }
    
    glAttachShader(_shaderProg, ShaderObj);
    
    return true;
}

// После добавления всех шейдеров в программу вызываем эту функцию
// для линковки и проверки программу на ошибки
bool ShaderLoader::finalize(){
    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};
    
    glLinkProgram(_shaderProg);
    
    glGetProgramiv(_shaderProg, GL_LINK_STATUS, &Success);
    if (Success == 0){
        glGetProgramInfoLog(_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        return false;
    }
    
    glValidateProgram(_shaderProg);
    glGetProgramiv(_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (Success == 0){
        glGetProgramInfoLog(_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        return false;
    }
    
    return true;
}

void ShaderLoader::use(){
    glUseProgram(_shaderProg);
}

GLint ShaderLoader::getUniformLocation(const char* pUniformName){
    GLint Location = glGetUniformLocation(_shaderProg, pUniformName);
    
    if (Location == 0xFFFFFFFF){
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }
    
    return Location;
}
