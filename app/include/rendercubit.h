#ifndef HCUBITRENH
#define HCUBITRENH

#include "camera.h"

//datastructure to hold orientation of cubit
class RenderCubit {
  public: 
    //default constructor
    RenderCubit() = default;

    //initialise all colours and coordinates of all vertices
    //centroid, cubit and vertices need to be set beforehand
    unsigned int *initCubit();
    
    //update all vertices by rotating them around center 
    //with axis as rotation axis
    //so axis being Z then vertices coords rotated: X and Y etc...
    //returns pointer to indices array 
    //(which contains vertices to draw triangle with)
    void update(float angle, glm::vec3 center, glm::vec3 axis);

    //setters ---------------------------------------------------

    //set vertices pointer data into large VBO
    void setVertices(float *d) {data = d;};

    //setter for centroid
    void setCentroid(glm::vec3 c) {centroid = c;};

    //setter for cubit data
    void setCubitData(Cubit c) {cubit = c;};

    //setter for IndexOffset
    void setIndexOffset(int nwoffset) {indexOffset = nwoffset;};

    //getters ---------------------------------------------------

    //centroid getter
    glm::vec3 getCentroid() const {return centroid;};

    //indexOffset getter
    int getIndexOffset() const {return indexOffset;};

  private:
    //pointer to all vertices and colour data of this cubit
    //should only access until NFLOATS_PER_VERTEX * NVERTICES_PER_CUBIT
    //which is 6 * 24 = 144
    float *data;
    //index offset to be used in vertices
    int indexOffset;
    //indices to be used when drawing triangles
    unsigned int indices[36];
    //data of cubit connected to this render object
    Cubit cubit;
    //centroid of this cubit in space
    glm::vec3 centroid = {0.0f, 0.0f, 0.0f};

    //helper functions to create vertices and indices
    void createXWall(Colour col, float offset, int &Vindex, int &Iindex),
         createYWall(Colour col, float offset, int &Vindex, int &Iindex),
         createZWall(Colour col, float offset, int &Vindex, int &Iindex);
};

#endif