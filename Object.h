#ifndef OBJECT_H
#define OBJECT_H
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "Ray.h"
#include "Light.h"

using namespace std;

class Object
{
public:
  float ambient, diffuse, reflection, refraction, filter, ior; 
  glm::vec3 color;
  glm::vec3 scale, rotate, translate;
  string order;
  virtual ~Object(){}

  virtual void printstuff() = 0;

  virtual float intersect(const Ray & r) = 0;

  virtual glm::vec3 getColor() = 0;
  virtual string getObjType() = 0;
  virtual glm::vec3 getNormal(glm::vec3 & pt) = 0;
  virtual glm::vec3 blinnPhong(Ray & r, float distance, Light & l, bool inShadow) = 0;
 
  virtual float getReflection() = 0;
  virtual float getRefraction() = 0;
  virtual float getFilter() = 0; 
  virtual float getIOR() = 0;
  virtual glm::vec3 ambColor(Light & l) = 0;

  virtual float getTheta() = 0;
  virtual glm::vec3 getRot() = 0;

  virtual glm::vec3 getTranslate() = 0;
  virtual glm::vec3 getScale() = 0;
  virtual glm::vec3 getRotate() = 0;

  virtual glm::mat4 getModel() = 0;
  virtual glm::mat4 getInverse() = 0;
  virtual glm::mat4 getNormMat() = 0;
  virtual void calcTransformations() = 0;
};
#endif
