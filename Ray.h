#include <glm/vec3.hpp>

class Ray
{
public:
   glm::vec3 start;
   glm::vec3 direction;
   Ray(glm::vec3 & s, glm::vec3 & dir)
   {
      start = s; direction = dir;
   }
}; 
