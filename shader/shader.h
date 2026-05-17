#include "../app/include/cuberenderer.h"
#include <fstream>

//reads contents of file <name> into out
bool readFile(const char *name, std::string &out);

//compile single shader source code, 
//type contains shader type:
    //GL_VERTEX_SHADER
    //GL_FRAGMENT_SHADER
//src contains source code of shader
//upon succes out contains shader program
//upon failure out contains failed shader program so that error can be extracted
bool compileShader(GLenum type, const char* src, GLuint &out);

//create entire shader program using vertexSrc and fragmentSrc as source code
//respectively for vertex shaders and fragment shaders
//upon succes out contains shader program
//upon failure out contains shader program that failed, so error can be extracted
bool createShaderProgram(const char* vertexSrc, const char* fragmentSrc, GLuint &out);