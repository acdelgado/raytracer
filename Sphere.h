#include <glm/vec3.hpp>
#include "Object.h"

class Sphere : public Object
{
public:
   glm::vec3 xyz;
   double radius;
   glm::vec3 color;
   double ambient, diffuse, reflection, refraction, filter, ior, specular, roughness;
   glm::vec3 translate, rotate, scale;
   string order;
   glm::mat4 model, inverse, normMat;

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
      cout << "  - Filter: " << filter << endl;
      cout << "- Rotate: {";
      cout << rotate.x << " " << rotate.y << " " << rotate.z << "}" << endl;
      cout << "- Scale: {";
      cout << scale.x << " " << scale.y << " " << scale.z << "}" << endl;
      cout << "- Translate: {";
      cout << translate.x << " " << translate.y << " " << translate.z << "}" << endl;
      cout << "order: " << order << endl;
  }

   void calcTransformations()
   {
      float theta = getTheta();
      glm::vec3 rot = getRot();
      model = glm::mat4(1.f);

      for(int i = 0; i < order.length(); i++)
      {
         if(order[i] == 't')
            model = glm::translate(glm::mat4(1), translate) * model;
         if(order[i] == 's')
            model = glm::scale(glm::mat4(1), scale) * model;
         if(order[i] == 'r')
            model = glm::rotate(glm::mat4(1), glm::radians(theta), rot) * model;
      }     
      inverse = glm::inverse(model);
      normMat = glm::transpose(inverse);
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

   float getTheta()
   {
      if(rotate.x != 0)
         return rotate.x;
      if(rotate.y != 0)
         return rotate.y;
      return rotate.z;
   }

   glm::vec3 getRot()
   {
      if(rotate.x != 0)
         return glm::vec3(1,0,0);
      if(rotate.y != 0)
         return glm::vec3(0,1,0);
      if(rotate.z != 0)
         return glm::vec3(0,0,1);
      return glm::vec3(0,0,0);
   }

   glm::mat4 getModel(){return model;}
   glm::mat4 getInverse(){return inverse;}
   glm::mat4 getNormMat(){return normMat;}
 
   glm::vec3 getTranslate(){return translate;}
   glm::vec3 getRotate(){return rotate;}
   glm::vec3 getScale(){return scale;}
   float getFilter() {return filter;}
   float getReflection() { return reflection;}
   float getRefraction() {return refraction;}
   float getIOR() {return ior;}
   glm::vec3 getColor(){return color;}
   string getObjType(){return "Sphere";}

   glm::vec3 getNormal(glm::vec3 & pt)
   {
      glm::vec4 normal = glm::vec4((pt.x-xyz.x)/radius, (pt.y-xyz.y)/radius, (pt.z-xyz.z)/radius, 0);

      normal = normMat * normal;
      return glm::vec3(normal.x, normal.y, normal.z);
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
      glm::vec3 normal = getNormal(point);
      normal = glm::normalize(normal);
      glm::vec3 h = glm::normalize(v + light_d);

      float amb = ambient;
      glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
      ln = glm::clamp(ln, 0.0f, 1.0f);

      newColor = amb * color * ln;

      float alpha = 2.f / (roughness * roughness) - 2.f;
      float diff = diffuse * (glm::dot(normal, light_d));
      float spec = specular * std::pow(glm::dot(h, normal), alpha);
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
