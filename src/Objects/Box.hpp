#pragma once

#include <Scene/Object.hpp>

class Box : public Object
{
public:

   Box(const glm::vec3 & minvec, const glm::vec3 & maxvec);
   float Intersect(const Ray & ray) const;
   glm::vec3 CalculateNormal(glm::vec3 const & intersectionPoint) const;
   std::string GetObjectType() const;

protected:
   glm::vec3 bmin;
   glm::vec3 bmax;
};
