#include <glm/vec3.hpp>
#include "Object.h"

class Sphere : public Object
{
public:
   glm::vec3 xyz;
   double radius;
   glm::vec3 color;
   double ambient, diffuse, reflection, refraction, filter, ior, specular, roughness;
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
      cout << "  - Reflection: " << reflection << endl;
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

      float top1 = ((-1 * b) + sqrt(determinant)) / (2 * a);
      float top2 = ((-1 * b) - sqrt(determinant)) / (2 * a);
      
      if(top2 < 0)
         return top1;

      return min(top1, top2);
   }
   float getFilter() {return filter;}
   float getReflection() { return reflection;}
   float getRefraction() {return refraction;}
   float getIOR() {return ior;}
   glm::vec3 getColor(){return color;}
   string getObjType(){return "Sphere";}

   glm::vec3 getNormal(glm::vec3 & pt)
   {
      return glm::vec3((pt.x-xyz.x)/radius, (pt.y-xyz.y)/radius, (pt.z-xyz.z)/radius);
   }

   glm::vec3 ambColor(Light & l)
   {
      glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
      ln = glm::clamp(ln, 0.0f, 1.0f);
      float amb = ambient;
      return amb * color * ln;
   }

   glm::vec3 blinnPhong(Ray & r, float distance, Light & l, bool inShadow)
   {
      glm::vec3 newColor = glm::vec3(0,0,0);

      glm::vec3 point = r.start + (distance * r.direction);
      glm::vec3 v = glm::normalize(r.start - point);
      glm::vec3 light_d = glm::normalize(l.location - point);
      glm::vec3 normal = glm::vec3((point.x - xyz.x)/radius, (point.y - xyz.y)/radius, (point.z - xyz.z)/radius);
      normal = glm::normalize(normal);
      glm::vec3 h = glm::normalize(v + light_d);

      float amb = ambient;
      glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
      ln = glm::clamp(ln, 0.0f, 1.0f);

      newColor = amb * color * ln;

      float diff = diffuse * (glm::dot(normal, light_d));
      float spec = specular * std::pow(glm::dot(h, normal), 1/roughness);
      diff = glm::clamp(diff, 0.0f, 1.0f);
      spec = glm::clamp(spec, 0.0f, 1.0f); 
      
      //DIFFUSE LIGHTING
      if(!inShadow)
      {
        newColor = newColor + color * l.color * (diff+spec);
      }
      return newColor;
   }
};
