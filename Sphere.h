#include <glm/vec3.hpp>
#include "Object.h"

class Sphere : public Object
{
public:
   float poop;
   glm::vec3 xyz;
   double radius;
   glm::vec3 color;
   double ambient, diffuse;
   glm::vec3 translate;

   void printstuff()
   {
      cout << std::fixed; cout << std::setprecision(4);
      cout << "- Type: Sphere" << endl;
      cout << "- Center: {";
      cout << xyz.x << " " << xyz.y << " " << xyz.z << "}" << endl;
      cout << "- Radius: " << radius << endl;
      cout << "- Color: {";
      cout << color.x << " " << color.y << " " << color.z << "}" << endl;
      cout << "- Material:" << endl;
      cout << "  - Ambient: " << ambient << endl;
      cout << "  - Diffuse: " << diffuse << endl;
   }

   float intersect(const Ray & r)
   {
      float a = glm::dot(r.direction,r.direction);
      float b = glm::dot(2.0f * r.direction, r.start - xyz);
      float c = glm::dot(r.start - xyz, r.start - xyz) - (radius * radius);

      float determinant = (b * b) - (4 * a * c);
      if(determinant < 0 || a == 0)
      {
         return -1000;
      }

      float top1 = -1 * (b + sqrt(determinant)) / (2 * a);
      float top2 = -1 * (b - sqrt(determinant)) / (2 * a);
      return min(top1, top2);
   }
   glm::vec3 getColor(){return color;}

};
