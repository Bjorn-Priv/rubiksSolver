#include "include/camera.h"

#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(CameraAction *a) : action(a) {
  view = {
    {10, 10, 10},
    {0, 0, 0},
    {0, 1, 0}
  };

  proj = {
    glm::radians(45.0f),
    1.0f,
    0.1f,
    100.0f
  };
} //default constructor

float Camera::getRadius() const {
  return glm::distance(view.eye, view.center);
} //getRadius

float Camera::getVelocity() const {
  return 0.2f * std::pow(getRadius(), 2.0f);
} //getVelocity

void Camera::handleAction(float deltatime) {
  SDL_Log("Before handling: --------------------------------------------------");
  SDL_Log("view.eye: {%f, %f, %f}", view.eye[0], view.eye[1], view.eye[2]);
  SDL_Log("view.center: {%f, %f, %f}", view.center[0], view.center[1], view.center[2]);
  SDL_Log("view.up: {%f, %f, %f}", view.up[0], view.up[1], view.up[2]);

  //relative position to center point
  glm::vec3 relativeEye = view.eye - view.center;

  float v = getVelocity();
  
  //grab radius but avoid division by zero
  float r = getRadius();
  if (r < 0.001f) r = 0.001f;

  //angle compared to up direction of Y
  //0* directly above looking straight down
  //90* on XZ plane perfectly level
  //180* directly below object
  //theta = arccos(y / r)
  float theta = std::acos(relativeEye[1]/r);

  //angle compared to object on the XZ plane
  //0* directly on pos X
  //90* directly on pos Z
  //180* directly on neg X
  //360* directly on neg Z
  //phi = arctan2(z, x) (no idea what difference is between atan and atan2)
  float phi = std::atan2(relativeEye[2], relativeEye[0]);

  //calculate new radius after moving
  //zooming in means radius becomes smaller
  r -= action->zoom * v * deltatime;
  //clamp it at minimum and maximum so not too close and not too far
  if (r < 5.0f) r = 5.0f; 
  if (r > 20.0f) r = 20.0f;

  //calculate new theta based on deltatime
  //newtheta = oldtheta + (direction * (v / r) * deltatime)
  theta -= action->Y * (v / r) * deltatime;

  //clamp theta between 0.01f and pi
  float minTheta = 0.5f;
  float maxTheta = glm::pi<float>() - 0.5f;

  if (theta < minTheta) {
      theta = minTheta;
  } else if (theta > maxTheta) {
      theta = maxTheta;
  }

  //calculate new phi based on deltatime
  //new phi = oldphi + (direction * (v / (r * sin(theta))) * deltatime)
  phi -= action->X * (v / (r * std::sin(theta))) * deltatime;

  //calculate cartesian coordinates in 3d plane
  relativeEye.x = r * std::sin(theta) * std::cos(phi);
  relativeEye.y = r * std::cos(theta);
  relativeEye.z = r * std::sin(theta) * std::sin(phi);

  view.eye = view.center + relativeEye;

  SDL_Log("After handling:");
  SDL_Log("view.eye: {%f, %f, %f}", view.eye[0], view.eye[1], view.eye[2]);
  SDL_Log("view.center: {%f, %f, %f}", view.center[0], view.center[1], view.center[2]);
  SDL_Log("view.up: {%f, %f, %f}", view.up[0], view.up[1], view.up[2]);
  SDL_Log("Distance to object: %f", glm::distance({0, 0, 0}, view.eye));
  SDL_Log("------------------------------------------------------------");
} //handleAction