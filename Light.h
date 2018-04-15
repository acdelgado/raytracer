#include <glm/vec3.hpp>
#include "Object.h"

using namespace std;

class Light : public Object
{
public:
   float poop;
   glm::vec3 location, color;

   void printstuff()
   {
      cout << std::fixed; cout << std::setprecision(4);
      cout << "- Location: {";
      cout <<location.x << " " << location.y << " " << location.z << "}" <<endl;
      cout << "- Color: {";
      cout <<color.x << " " << color.y << " " << color.z << "}" <<endl;
   }

   float intersect(const Ray & r){return 1;}
glm::vec3 getColor(){return color;}
   string getObjType(){return "Light";}
};
