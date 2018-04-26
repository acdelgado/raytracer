#include <glm/vec3.hpp>
#include "Object.h"

using namespace std;

class Camera : public Object
{
public:
   float poop;
   glm::vec3 location, up, right, look_at, color;

   void printstuff()
   {
      cout << std::fixed; cout << std::setprecision(4);
      cout << "Camera: " << endl;
      cout << "- Location: {";
      cout <<location.x << " " << location.y << " " << location.z << "}" <<endl;
      cout << "- Up: {";
      cout <<up.x << " " << up.y << " " << up.z << "}" <<endl;
      cout << "- Right: {";
      cout <<right.x << " " << right.y << " " << right.z << "}" <<endl;
      cout <<"- Look at: {";
      cout <<look_at.x << " " << look_at.y << " " << look_at.z << "}" <<endl;
   }

   float intersect(const Ray & r){return 0;}
   glm::vec3 getColor(){return color;}
   string getObjType(){return "Camera";}

   glm::vec3 blinnPhong(Ray & r, float distance, glm::vec3 & color, glm::vec3 & light,glm::vec3 & lcolor)
   {
      return glm::vec3(0,0,0);
   }

};
