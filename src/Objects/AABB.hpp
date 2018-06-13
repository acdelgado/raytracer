#pragma once

#include <Scene/Object.hpp>

class AABB
{
   glm::vec3 bmin, bmax;

public:
   void reset(glm::vec3 pt) {
      bmin = bmax = pt;
   }

   void addPoint(glm::vec3 pt) {
      bmin.x = glm::min(bmin.x, pt.x);
      bmin.y = glm::min(bmin.y, pt.y);
      bmin.z = glm::min(bmin.z, pt.z);
      bmax.x = glm::max(bmax.x, pt.x);
      bmax.y = glm::max(bmax.y, pt.y);
      bmax.z = glm::max(bmax.z, pt.z);
   }

   void addBox(AABB other) {
      addPoint(other.bmin);
      addPoint(other.bmax);
   }
};
