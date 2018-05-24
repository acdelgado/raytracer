#include <glm/vec3.hpp>
#include "Object.h"

class Triangle : public Object
{
public:
  float ambient,diffuse,reflection,filter,refraction,ior;; 
  glm::vec3 color, scale, rotate, translate, a, b, c;
  glm::mat4 model, inverse, normMat;
  string order;

  void printstuff()
  {
     cout << std::fixed; cout << std::setprecision(4);
     cout << "- Type: Triangle" << endl;
     cout << "- Color: {";
     cout << color.x << " " << color.y << " " << color.z << "}" << endl;
     cout << "- Point A: {";
     cout << a.x << " " << a.y << " " << a.z << "}" << endl;
     cout << "- Point B: {";
     cout << b.x << " " << b.y << " " << b.z << "}" << endl;
     cout << "- Point C: {";
     cout << c.x << " " << c.y << " " << c.z << "}" << endl;
     cout << "  - Ambient: " << ambient << endl;
     cout << "  - Diffuse: " << diffuse << endl;
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


  float intersect(const Ray & r)
  {
    float aa = a.x - b.x;
    float bb = a.y - b.y;
    float cc = a.z - b.z;
    float d = a.x - c.x;
    float e = a.y - c.y;
    float f = a.z - c.z;
    float g = r.direction.x;
    float h = r.direction.y;
    float i = r.direction.z;
    float j = a.x - r.start.x;
    float k = a.y - r.start.y;
    float l = a.z - r.start.z;

    float eihf = e*i - h*f;
    float gfdi = g*f - d*i;
    float dheg = d*h - e*g;

    float det = aa*eihf + bb*gfdi + cc*dheg;
    float beta = (j*eihf + k*gfdi + l*dheg) / det;
    
    float akjb = aa*k - j*bb;
    float jcal = j*cc - aa*l;
    float blkc = bb*l - k*cc;

    float gamma = (i*akjb + h*jcal + g*blkc) / det;
    float t = (-1*f*akjb - e*jcal - d*blkc) / det;

    if(gamma < 0 || gamma > 1)
      return -1000;

    if(beta < 0 || beta > 1 - gamma)
      return -1000;

    return t;
  }
  float getReflection() { return reflection;}
  float getRefraction() {return refraction;}
  float getIOR() {return ior;}
  float getFilter() {return filter;}
  glm::vec3 getColor()
  {
    return color;
  }

  string getObjType()
  {
    return "Triangle";
  }


  glm::vec3 getNormal(glm::vec3 & pt)
  {
     glm::vec3 ab = a - b;
     glm::vec3 ac = a - c;
     glm::vec3 n3 = glm::cross(ab,ac);

     glm::vec4 normal = glm::vec4(n3.x, n3.y, n3.z, 0);
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
     glm::vec3 light_d = glm::normalize(l.location - point);

     glm::vec3 ab = a - b;
     glm::vec3 ac = a - c;
     glm::vec3 n = glm::normalize(getNormal(point));

     if(glm::dot(r.direction, n) > 0)
        n *= -1;

     float amb = ambient;
     glm::vec3 ln = glm::vec3(l.color.x, l.color.y, l.color.z);
     ln = glm::clamp(ln, 0.0f, 1.0f);

     newColor = amb * color * ln;

     
     float diff = diffuse * (glm::dot(n, light_d));
     if(diff < 0) diff = 0;
     if(diff > 1) diff = 1;
      
      
     //DIFFUSE LIGHTING
     if(!inShadow)
       newColor = newColor + color * l.color * diff;
     return newColor;
  }

};
