#include "include/camera.h"

Camera::Camera(CameraAction *a, float v) : action(a), velocity(v) {
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
}

float Camera::getRadius() const {
  return glm::distance(view.eye, view.center);
}

void Camera::handleAction(float deltatime) {
  //relative position to center point
  glm::vec3 relativeEye = view.eye - view.center;

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
  r -= action->zoom * velocity * deltatime;
  //clamp it at minimum and maximum so not too close and not too far
  if (r < 0.5f) r = 0.5f; 
  if (r > 100.0f) r = 100.0f;

  //calculate new theta based on deltatime
  //newtheta = oldtheta + (direction * (v / r) * deltatime)
  theta -= action->Y * (velocity / r) * deltatime;

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
  phi -= action->X * (velocity / (r * std::sin(theta))) * deltatime;


  //calculate cartesian coordinates in 3d plane
  relativeEye.x = r * std::sin(theta) * std::cos(phi);
  relativeEye.y = r * std::cos(theta);
  relativeEye.z = r * std::sin(theta) * std::sin(phi);

  view.eye = view.center + relativeEye;
}