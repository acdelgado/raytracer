#include <glm/vec3.hpp>
#include "Object.h"

class Plane : public Object
{
public:
   glm::vec3 norm;
   double dist;
   glm::vec3 color, scale, rotate, translate;
   double ambient, diffuse, filter, reflection, refraction, ior;
   glm::mat4 model, inverse, normMat;
   string order;

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
      cout << "  - Reflection: " << reflection << endl;
      cout << "- Rotate: {";
      cout << rotate.x << " " << rotate.y << " " << rotate.z << "}" << endl;
      cout << "- Scale: {";
      cout << scale.x << " " << scale.y << " " << scale.z << "}" << endl;
      cout << "- Translate: {";
      cout << translate.x << " " << translate.y << " " << translate.z << "}" << endl;


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
      float denom = glm::dot(r.direction, norm);
      if(denom == 0){return -1000;}
      return (dist * (glm::dot(norm, norm)) - glm::dot(r.start, norm)) / denom;
   }

   glm::mat4 getModel(){return model;}
   glm::mat4 getInverse(){return inverse;}
   glm::mat4 getNormMat(){return normMat;}
 
   glm::vec3 getTranslate(){return translate;}
   glm::vec3 getRotate(){return rotate;}
   glm::vec3 getScale(){return scale;}

  
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

   float getReflection() {return reflection;}
   float getRefraction() {return refraction;}
   float getIOR() {return ior;}
   float getFilter() {return filter;}
   glm::vec3 getColor(){return color;}
   string getObjType(){return "Plane";}


   glm::vec3 getNormal(glm::vec3 & pt)
   {
      glm::vec4 normal = glm::vec4(norm.x, norm.y, norm.z, 0);

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
      glm::vec3 normal = glm::normalize(getNormal(point));

 
      float amb = ambient;
      glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
      ln = glm::clamp(ln, 0.0f, 1.0f);

      newColor = amb * color * ln;

     
      float diff = diffuse * (glm::dot(normal, light_d));
      if(diff < 0) diff = 0;
      if(diff > 1) diff = 1;
      
      
      //DIFFUSE LIGHTING
      if(!inShadow)
        newColor = newColor + color * l.color * diff;
      return newColor;
   }

};
