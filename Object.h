#ifndef OBJECT_H
#define OBJECT_H
#include <algorithm>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "Ray.h"

using namespace std;

class Object
{
public:
  float poop; 
  glm::vec3 color;
  virtual ~Object(){}
  virtual void printstuff() = 0;
  virtual float intersect(const Ray & r) = 0;
  virtual glm::vec3 getColor() = 0;
  virtual string getObjType() = 0;
};

#endif
