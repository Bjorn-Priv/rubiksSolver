#include "shader.h"

bool readFile(const char *name, std::string &out) {
  std::ifstream file(name);
  if (!file) return false;

  //go to end and find size
  file.seekg(0, std::ios::end);
  size_t size = file.tellg();

  //go back to start
  file.seekg(0, std::ios::beg);

  //resize string
  out.resize(size);

  //read entire string
  file.read(out.data(), size);
  return true;
} //readShaderFile

bool compileShader(GLenum type, const char *src, GLuint &out) {
  GLuint shader = glCreateShader(type); //create single shader 

  //set and compile source
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  //check compiling errors
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  out = shader;

  return success;
} //compileShader

bool createShaderProgram(const char* vertexSrc, const char *fragmentSrc, GLuint &out) {
  GLuint vs; //vertex shader program
  GLuint fs; //fragment shader program

  //check whether both sources compile
  if (!compileShader(GL_VERTEX_SHADER, vertexSrc, vs)) { 
    out = vs;
    return false;
  }
  if (!compileShader(GL_FRAGMENT_SHADER, fragmentSrc, fs)) {
    out = fs;
    return false;
  } 

  //create entire shader program
  //and attach/link vertex and fragment shaders
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  //check linking errors
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  out = program;

  glDeleteShader(vs);
  glDeleteShader(fs);

  return success;
} //createShaderProgram