#ifndef HCAMERAH
#define HCAMERAH

#include "../../cube/include/rcube.h"
#include "keyboard.h"

#include <vector>

#include <GL/glew.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//create viewing vector space
//using three vectors as it is a 3d space
//eye : location of camera 
//center : the location where the camera is looking
//up : defines the Y-axis (0, 1, 0) is default up to positive y
  //basically which way is up in respect to (0, 0, 0)
//the X-axis and Z-axis are calculated based on eye and center
struct View {
  glm::vec3 eye = {0, 0, 0}; //camera position
  glm::vec3 center = {0, 0, 0}; //point camera looks at
  glm::vec3 up = {0, 1, 0}; //point that is up from {0, 0, 0} starts on {0, 1, 0}
};

//4 arguments
//FOV : how wide the camera lens is in radians (this case 45 degrees)
//aspect ratio : width / height (this case just 1 to 1)
//near clipping plane : anything closer than this distance to the camera
  //will not be rendered to prevent clipping
//far clipping plane : maximum draw distance (any object further away is ignored)
struct Projection {
  float fov; //fov in radians
  float aspect; //aspect ratio
  float near; //near clipping
  float far; //far clipping
};

//main class for an orbiting camera
class Camera {
  private:
    View view; //camera data
    Projection proj; //environment data

    //action container
    CameraAction *action = nullptr;

    float getRadius() const;

    float getVelocity() const;
    
  public:
    //default constructor
    //pass both a container for actions and the velocity of the camera
    Camera(CameraAction *a);

    //getter for view
    glm::highp_mat4 getView() 
      {return glm::lookAt(view.eye, view.center, view.up);}

    //getter for projection
    glm::highp_mat4 getProjection()
      {return glm::perspective(proj.fov, proj.aspect, proj.near, proj.far);}

    void handleAction(float deltatime);

    ~Camera() = default;
};

#endif