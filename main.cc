#include "cube/include/rcube.h"
#include "app/include/cuberenderer.h"
#include "shader/shader.h"

//shader files 
const char *VERTEX_SHADER = "shader/shader.vert";
const char *FRAGMENT_SHADER = "shader/shader.frag";

const char *NAME = "RCUBE";
const int HEIGHT = 320;
const int WIDTH = 320;

//runs the main loop of the program
void main_loop(SDL_Window* window, GLuint shader) {
  bool running = true; //main program boolean

  float deltaTime; //deltatime to control animation update time in seconds
  uint64 lastTicks = SDL_GetTicks(); //last tick count (in milliseconds)
  uint64 currentTicks; //current tick count (in miliseconds)

  RCube cube; //main logical rubiks cube

  //action containers
  std::queue<CubeAction> cubeActions;
  CameraAction cameraAction;

  Camera camera(&cameraAction); //main camera to be changed

  Keyboard keyboard(&cameraAction, &cubeActions); //keyboard state reader
  int nKeys;

  CubeRenderer renderer(&cube, &cubeActions, &camera, shader); //main renderer for 3d cube

  SDL_Event event; //event stack

  while (running) { //main loop
    while (SDL_PollEvent(&event)) { //handle events
      if (event.type == SDL_EVENT_QUIT) { //why quit? :(
        running = false;
      }

      if (event.type == SDL_EVENT_WINDOW_RESIZED) { //window is resized
        int w = event.window.data1; //grab width
        int h = event.window.data2; //grab height
        int offsetW = 0;
        if (w > h) {
          offsetW = w-h;
          w = h;
        }
        //update gl viewport so that cube stays centered in a square viewport
        glViewport((0.5f * offsetW), 0, w, h);
      }

      if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
        keyboard.readState(SDL_GetKeyboardState(&nKeys), nKeys);
      }
    } //while

    //reset color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //calculate deltatime
    currentTicks = SDL_GetTicks();
    deltaTime = (currentTicks - lastTicks) / 1000.0f;

    lastTicks = currentTicks; //update lastticks

    //handle camera movement
    camera.handleAction(deltaTime);
    //create new move
    renderer.startMove();

    //update and render cube
    renderer.update(deltaTime);
    renderer.render();

    //update window and stop if it fails
    if (!SDL_GL_SwapWindow(window)) {
      SDL_Log("Updating window failed!");
      SDL_Log("%s", SDL_GetError());
      running = false;
    }
  } //while
} //main_loop

//**************************************************************************************

//create an SDL window and place it in window pointer
//returns true if window is not nullptr
//returns false if window is nullptr
bool createWindow(const char* name, int w, int h, 
                  SDL_WindowFlags FLAGS, SDL_Window *&window) {
  window = SDL_CreateWindow(name, w, h, FLAGS);
  return window != nullptr;
} //createWindow

//create a GL context linked to window w, and places it in ctx
//returns true if ctx is not nullptr
//returns false if ctx is nullptr
bool createGLContext(SDL_Window *w, SDL_GLContext &ctx) {
  ctx = SDL_GL_CreateContext(w);
  return ctx != nullptr;
} //createGLContext

//initialise glew which is an extension wrangler for openGL 
//meaning it finds functions in the graphics driver of ur computer
//that it can use for openGL shenanigans
//because graphics manufacturers cant agree on a common function
//:)
bool initGlew() {
  //makes it so that it also grabs functions that are not in the official
  //driver functions list yet but are in the pre release drivers
  glewExperimental = GL_TRUE;

  //find functions
  if (glewInit() != GLEW_OK) {
    return false;
  }

  //apparantly it just pushes a silent error which is unimportant 
  //so we can just clear errors by getting it
  glGetError();

  //makes it so OpenGL tracks pixel distance to camera,
  //allowing foreground pixels to obscure background pixels
  //so that background pixels arent incorrectly rendered in front
  glEnable(GL_DEPTH_TEST);

  //needs to be same size as window so that rendered object is centered
  //is often changed in main event loop
  glViewport(0, 0, WIDTH, HEIGHT);

  //main background colour of rendering window
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  return true;
} //initGlew

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) return 0;

  std::string vertexShaderSrc;
  std::string fragmentShaderSrc;

  //check if shader source code can be read
  if (!readFile(VERTEX_SHADER, vertexShaderSrc) || !readFile(FRAGMENT_SHADER, fragmentShaderSrc)) {
    SDL_Log("Couldn't find or read Shader files!");
    SDL_Quit();
    return 0;
  }

  //grab openGL version 3.3 
  //MAJOR_VERSION is minimum major version to be used so version 3
  //MINOR_VERSION is minimum minor version to be used so version 3.3 to be exact
  //PROFILE MASK sets profile of openGL
    //in this case to CORE
    //meaning it removes all deprecated functions and features
  if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) ||
      !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) ||
      !SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) {
    SDL_Log("GL Set Attributes failed!");
    SDL_Log("%s", SDL_GetError());
    SDL_Quit();
    return 0;
  }

  //main window
  SDL_Window *window;
  
  SDL_GLContext gl_ctx; //openGL context
  GLuint shader; //shader for openGL

  //window attributes
  const SDL_WindowFlags FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

  if (!createWindow(NAME, HEIGHT, WIDTH, FLAGS, window)) { //create window
    SDL_Log("Create window failed!");
    SDL_Log("%s", SDL_GetError());
    SDL_Quit();
    return 0;
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  if (!createGLContext(window, gl_ctx)) { //gl context creation
    SDL_Log("Create GL context failed!");
    SDL_Log("%s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
  }

  if (!initGlew()) { //glew stuff
    SDL_Log("initialise Glew failed!");
    SDL_Log("%s", SDL_GetError());
    //call destroy ctx before window!!
    SDL_GL_DestroyContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
  }

  if (!createShaderProgram(vertexShaderSrc.c_str(), fragmentShaderSrc.c_str(), shader)) {
    SDL_Log("Creating shader program failed!");
    char log[512];
    glGetProgramInfoLog(shader, 512, nullptr, log);
    SDL_Log("Program link error: %s", log);
    //call destroy ctx before window!!
    SDL_GL_DestroyContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
  }

  main_loop(window, shader);

  //destroy everything
  if (!SDL_GL_DestroyContext(gl_ctx)) {
    SDL_Log("Destroying OpenGL context failed!");
    SDL_Log("%s", SDL_GetError());
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}