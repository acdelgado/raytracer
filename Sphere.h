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
   string getObjType(){return "Sphere";}

   glm::vec3 blinnPhong(Ray & r, float distance, glm::vec3 & color, glm::vec3 & light, glm::vec3 & lcolor)
   {
      glm::vec3 point = r.start + (distance * r.direction);
      glm::vec3 v = glm::normalize(r.start - point);
      glm::vec3 light_d = glm::normalize(light - point);
      glm::vec3 normal = glm::vec3((point.x - xyz.x)/radius, (point.y - xyz.y)/radius, (point.z - xyz.z)/radius);
      normal = glm::normalize(normal);
      glm::vec3 h = glm::normalize(v + light_d);

      float diff = diffuse * (glm::dot(normal, light_d));
      float spec = 0 * std::pow(glm::dot(h, normal), 20);
      if(diff < 0) diff = 0;
      if(diff > 1) diff = 1;

      if(spec < 0) spec = 0;
      if(spec > 1) spec = 1;

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
