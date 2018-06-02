#include "Box.hpp"

Box::Box(const glm::vec3 & minvec, const glm::vec3 & maxvec)
{
   bmin = minvec;
   bmax = maxvec;
}

float Box::Intersect(const Ray & ray) const
{
      glm::vec3 p0 = ray.origin;
      glm::vec3 d = ray.direction;
      float txmin = (bmin.x - p0.x) / d.x;
      float txmax = (bmax.x - p0.x) / d.x;
      float tymin = (bmin.y - p0.y) / d.y;
      float tymax = (bmax.y - p0.y) / d.y;
      float tzmin = (bmin.z - p0.z) / d.z;
      float tzmax = (bmax.z - p0.z) / d.z;

      if(tzmin > tzmax){
         float tmp = tzmin;
         tzmin = tzmax;
         tzmax = tmp;
      }

      if(tymin > tymax){
         float tmp = tymin;
         tymin = tymax;
         tymax = tmp;
      }

      if(txmin > txmax){
         float tmp = txmin;
         txmin = txmax;
         txmax = tmp;
      }


      float smallestMax = std::min(std::min(txmax, tymax), tzmax);
      float largestMin = std::max(std::max(txmin, tymin), tzmin);
      if(smallestMax < largestMin || smallestMax < 0)
         return -1000;

      if(largestMin > 0)
         return largestMin;
      return smallestMax;
}

glm::vec3 Box::CalculateNormal(glm::vec3 const & pt) const
{
      float eps = 0.0001;
      if(std::abs(pt.x - bmin.x) < eps)
         return glm::vec3(-1,0,0);
      else if(std::abs(pt.x - bmax.x) < eps)
         return glm::vec3(1,0,0);
      else if(std::abs(pt.y - bmin.y) < eps)
         return glm::vec3(0,-1,0);
      else if(std::abs(pt.y - bmax.y) < eps)
         return glm::vec3(0,1,0);
      else if(std::abs(pt.z - bmin.z) < eps)
         return glm::vec3(0,0,-1);
      else
         return glm::vec3(0,0,1);
}

std::string Box::GetObjectType() const
{
   return "Box";
}
