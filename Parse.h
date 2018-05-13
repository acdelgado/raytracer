#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <glm/vec3.hpp>
#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

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

Camera* parse_cam(ifstream & inFile)
{
   string x;
   Camera *cam = new Camera();
   while(x != "}")
   {
      if(x == "location")
      {
         cam->location = parse_vec(inFile);
      }
      
      if(x == "up")
      {
         cam->up = parse_vec(inFile);
      }

      if(x == "right")
      {
         cam->right = parse_vec(inFile);
      }

      if(x == "look_at")
      {
         cam->look_at = parse_vec(inFile);
      }

      inFile >> x;
   }
   return cam;
}

Sphere* parse_sphere(ifstream & inFile)
{
   string x;
   Sphere *sphere = new Sphere();
   
   sphere->xyz = parse_vec(inFile);
   string radString;
   getline(inFile, x, ',');
   inFile >> x;
   sphere->radius = atof(x.c_str());
   sphere->color = parse_vec(inFile);
   inFile >> x;
   inFile >> x;
   while(x != "}")
   {
      if(x == "ambient")
      {
         inFile >> x;
         sphere->ambient = atof(x.c_str());
      }

      if(x == "diffuse")
      {
         inFile >> x;
         sphere->diffuse = atof(x.c_str());
      }

      if(x == "specular")
      {
         inFile >> x;
         sphere->specular = atof(x.c_str());
      }

      if(x == "roughness")
      {
         inFile >> x;
         sphere->roughness = atof(x.c_str());
      }

      inFile.ignore(1,'{');
      inFile >> x;
   }
   
   return sphere;
}

Plane* parse_plane(ifstream & inFile)
{
   string x;
   Plane *plane = new Plane();
   
   plane->norm = parse_vec(inFile);
   inFile >> x;
   inFile >> x;
   plane->dist = atof(x.c_str());
   plane->color = parse_vec(inFile);
   inFile >> x;
   inFile >> x;
   while(x != "}")
   {
      if(x == "ambient")
      {
         inFile >> x;
         plane->ambient = atof(x.c_str());
      }

      if(x == "diffuse")
      {
         inFile >> x;
         plane->diffuse = atof(x.c_str());
      }

      inFile.ignore(1,'{');
      inFile >> x;
   }
   return plane;
}

Triangle* parse_tri(ifstream & inFile)
{
   string x;
   Triangle *tri = new Triangle();
   
   tri->a = parse_vec(inFile);
   tri->b = parse_vec(inFile);
   tri->c = parse_vec(inFile); 
   tri->color = parse_vec(inFile);
   while(x != "}")
   {
      if(x == "ambient")
      {
         inFile >> x;
         tri->ambient = atof(x.c_str());
      }

      if(x == "diffuse")
      {
         inFile >> x;
         tri->diffuse = atof(x.c_str());
      }

      inFile.ignore(1,'{');
      inFile >> x;
   }
   return tri;
}

Light* parse_light(ifstream & inFile)
{
   string x;
   Light *l = new Light();

   l->location = parse_vec(inFile);
   l->color = parse_vec(inFile);
   return l;
}


};
