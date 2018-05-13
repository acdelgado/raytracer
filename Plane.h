#include <glm/vec3.hpp>
#include "Object.h"

class Plane : public Object
{
public:
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

   glm::vec3 blinnPhong(Ray & r, float distance, Light & l, bool inShadow)
   {
      glm::vec3 newColor = glm::vec3(0,0,0);
      glm::vec3 point = r.start + (distance * r.direction);
      glm::vec3 v = glm::normalize(r.start - point);
      glm::vec3 light_d = glm::normalize(l.location - point);
 
      float amb = ambient;
      glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
      ln = glm::clamp(ln, 0.0f, 1.0f);

      newColor = amb * color * ln;

     
      float diff = diffuse * (glm::dot(norm, light_d));
      if(diff < 0) diff = 0;
      if(diff > 1) diff = 1;
      
      
      //DIFFUSE LIGHTING
      if(!inShadow)
        newColor = glm::clamp(newColor + color * l.color * diff, 0.0f, 1.0f);
      return newColor;
   }

};
