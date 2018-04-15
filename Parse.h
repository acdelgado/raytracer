#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <glm/vec3.hpp>

using namespace std;

class Parse
{
public:

   static glm::vec3 parse_vec(std::ifstream & streamy)
   {
      glm::vec3 v;
      v.x = v.y = v.z = 0;
      std::stringbuf buf;
      std::stringbuf garbage;
      streamy.get(garbage,'<');
      streamy.ignore(1,'<');
      streamy.get(buf,'>');
      streamy.ignore(1,'>');

      std::string line = buf.str();
      int read = sscanf(line.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);

      if(read != 3)
      {
         cerr << "Expected to read 3 vector elements but found '" << line << "'" << endl;
      }

      return v;
   }
};
