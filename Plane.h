#include <glm/vec3.hpp>
#include "Object.h"

class Plane : public Object
{
public:
   float poop;
   glm::vec3 norm;
   double dist;
   glm::vec3 color;
   double ambient, diffuse;

   void printstuff()
   {
      cout << "- Type: Plane" << endl;
      cout << "- Normal: {";
      cout << norm.x << " " << norm.y << " " << norm.z << "}" << endl;
      cout << "- Distance: " << dist << endl;
      cout << "- Color: {";
      cout << color.x << " " << color.y << " " << color.z << "}" << endl;
      cout << "- Material:" << endl;
      cout << "  - Ambient: " << ambient << endl;
      cout << "  - Diffuse: " << diffuse << endl;
 
   }

   float intersect(const Ray & r)
   {
      float denom = glm::dot(r.direction, norm);
      if(denom == 0){return -1000;}
      return ((1 * dist) * (glm::dot(norm, norm)) - glm::dot(r.start, norm)) / denom;
   }
   glm::vec3 getColor(){return color;}

};
