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
      cout << std::fixed; cout << std::setprecision(4);
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
      return (dist * (glm::dot(norm, norm)) - glm::dot(r.start, norm)) / denom;
   }
   glm::vec3 getColor(){return color;}
   string getObjType(){return "Plane";}

   glm::vec3 blinnPhong(Ray & r, float distance, glm::vec3 & color, glm::vec3 & light, glm::vec3 & lcolor)
   {
      glm::vec3 point = r.start + (distance * r.direction);
      glm::vec3 v = glm::normalize(r.start - point);
      glm::vec3 light_d = glm::normalize(light - point);
      
      float diff = diffuse * (glm::dot(norm, light_d));

      //AMBIENT LIGHTING
      float amb = ambient;
      glm::vec3 ln = glm::vec3(lcolor.x, lcolor.y, lcolor.z);
      if(ln.x > 1) ln.x = 1;
      if(ln.y > 1) ln.y = 1;
      if(ln.z > 1) ln.z = 1;

      glm::vec3 newColor = amb * color * ln;
      
      //DIFFUSE LIGHTING
      newColor = newColor + color * lcolor * diff;
      return newColor;
   }

};
