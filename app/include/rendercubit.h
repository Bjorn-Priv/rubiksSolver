#ifndef HCUBITRENH
#define HCUBITRENH

#include "camera.h"

//datastructure to hold orientation of cubit
class RenderCubit {
  public: 
    //holds the orientation of the cubit 
    //starts on identity matrix
    glm::mat4 orientation = glm::mat4(1.0f);

    int indexOffset;

    void setVertices(float *d) {data = d;};
    void setCentroid(glm::vec3 c) {centroid = c;};
    glm::vec3 getCentroid() const {return centroid;};
    void setCubitData(Cubit c) {cubit = c;};
    unsigned int *initCubit();

    RenderCubit(){};

  private:
    float *data;
    unsigned int indices[36];
    Cubit cubit;
    glm::vec3 centroid = {0.0f, 0.0f, 0.0f};
    //helper functions to create vertices and indices
    void createXWall(Colour col, float offset, int &Vindex, int &Iindex);

    void createYWall(Colour col, float offset, int &Vindex, int &Iindex);

    void createZWall(Colour col, float offset, int &Vindex, int &Iindex);
};

#endif