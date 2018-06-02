#include <glm/vec3.hpp>
#include "Object.h"

class Box : public Object
{
public:
  float ambient, diffuse, specular, roughness, reflection, refraction, filter, ior; 
  glm::vec3 color;
  glm::vec3 scale, rotate, translate;
  string order;
  glm::mat4 model, inverse, normMat;

  glm::vec3 bmin, bmax;

   void printstuff()
   {
      cout << std::fixed; cout << std::setprecision(4);
      cout << "- Type: Box" << endl;
      cout << "- Min: {";
      cout << bmin.x << " " << bmin.y << " " << bmin.z << "}" << endl;
      cout << "- Max: {";
      cout << bmax.x << " " << bmax.y << " " << bmax.z << "}" << endl;
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
 
   float intersect(const Ray & r)
   {
      glm::vec3 p0 = r.start;
      glm::vec3 d = r.direction;
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


      float smallestMax = min(min(txmax, tymax), tzmax);
      float largestMin = max(max(txmin, tymin), tzmin);
      if(smallestMax < largestMin || smallestMax < 0)
         return -1000;

      if(largestMin > 0)
         return largestMin;
      return smallestMax;
   };

   glm::vec3 getColor() {return color;}
   string getObjType() {return "Box";}
   glm::vec3 getNormal(glm::vec3 & pt)
   {
      float eps = 0.0001;
      glm::vec4 norm4;
      if(eq(pt.x, bmin.x, eps))
         norm4 = glm::vec4(-1,0,0,0);
      else if(eq(pt.x, bmax.x, eps))
         norm4 = glm::vec4(1,0,0,0);
      else if(eq(pt.y, bmin.y, eps))
         norm4 = glm::vec4(0,-1,0,0);
      else if(eq(pt.y, bmax.y, eps))
         norm4 = glm::vec4(0,1,0,0);
      else if(eq(pt.z, bmin.z, eps))
         norm4 = glm::vec4(0,0,-1,0);
      else
         norm4 = glm::vec4(0,0,1,0);

      norm4 = normMat * norm4;
      return glm::vec3(norm4.x, norm4.y, norm4.z);
   }

   bool eq(float const a, float const b, float const epsilon)
   {
      return std::abs(a - b) < epsilon;
   }

   float getReflection() {return reflection;}
   float getRefraction() {return refraction;}
   float getFilter() {return filter;}
   float getIOR() {return ior;}
   glm::vec3 ambColor(Light & l) {return l.color * color * ambient;}

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
   glm::vec3 getTranslate() {return translate;}
   glm::vec3 getScale() {return scale;}
   glm::vec3 getRotate() {return rotate;}

   glm::mat4 getModel() {return model;}
   glm::mat4 getInverse() {return inverse;}
   glm::mat4 getNormMat() {return normMat;}
   
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

   glm::vec3 blinnPhong(Ray & r, float distance, Light & l, bool inShadow)
   {
      glm::vec3 newColor = glm::vec3(0,0,0);
      glm::vec3 point = r.start + (distance * r.direction);
      glm::vec3 v = glm::normalize(r.start - point);
      glm::vec3 light_d = glm::normalize(l.location - point);
      glm::vec3 normal = glm::normalize(getNormal(point));
      glm::vec3 h = glm::normalize(v + light_d);

 
      float amb = ambient;
      glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
      ln = glm::clamp(ln, 0.0f, 1.0f);

      newColor = amb * color * ln;

      float alpha = 2.f / (roughness * roughness) - 2.f;  
      float diff = diffuse * (glm::dot(normal, light_d));
      float spec = specular * std::pow(glm::dot(h, normal), alpha);
      if(diff < 0) diff = 0;
      if(diff > 1) diff = 1;
      if(spec < 0) spec = 0;
      if(spec > 1) spec = 1; 
      
      //DIFFUSE LIGHTING
      if(!inShadow)
        newColor = newColor + color * l.color * (diff+spec);
      return newColor;
   }
};
