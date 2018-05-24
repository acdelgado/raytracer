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

   static glm::vec4 parse_vec4(std::ifstream & streamy)
   {
      glm::vec4 v;
      v.x = v.y = v.z = v.a = 0;
      std::stringbuf buf;
      std::stringbuf garbage;
      streamy.get(garbage,'<');
      streamy.ignore(1,'<');
      streamy.get(buf,'>');
      streamy.ignore(1,'>');

      std::string line = buf.str();
      int read = sscanf(line.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.a);

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
   string order = "";
   sphere->xyz = parse_vec(inFile);
   string radString;
   getline(inFile, x, ',');
   inFile >> x;
   sphere->radius = atof(x.c_str());
   
   glm::vec4 rgbf = parse_vec4(inFile);

   sphere->color = glm::vec3(rgbf.x, rgbf.y, rgbf.z);
   sphere->filter = rgbf.a;
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

     
      if(x == "reflection")
      {
         inFile >> x;
         sphere->reflection = atof(x.c_str());
      }
      
      if(x == "refraction")
      {
         inFile >> x;
         sphere->refraction = atof(x.c_str());
      }
      
      if(x == "ior")
      {
         inFile >> x;
         sphere->ior = atof(x.c_str());
      }

      if(x == "rotate"){
         sphere->rotate = parse_vec(inFile);
         order = order + "r";
      }
      if(x == "scale"){
         sphere->scale = parse_vec(inFile);
         order = order + "s";
      }
      if(x == "translate"){
         sphere->translate = parse_vec(inFile);
         order = order + "t";
      }

      inFile.ignore(1,'{');
      inFile >> x;
   }
   sphere->order = order;
   return sphere;
}

Plane* parse_plane(ifstream & inFile)
{
   string x;
   Plane *plane = new Plane();
   string order = "";
   plane->norm = parse_vec(inFile);
   inFile >> x;
   inFile >> x;
   plane->dist = atof(x.c_str());
   glm::vec4 rgbf = parse_vec4(inFile);

   plane->color = glm::vec3(rgbf.x, rgbf.y, rgbf.z);
   plane->filter = rgbf.a;
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

      if(x == "reflection")
      {
         inFile >> x;
         plane->reflection = atof(x.c_str());
      }

      if(x == "refraction")
      {
         inFile >> x;
         plane->refraction = atof(x.c_str());
      }
      
      if(x == "ior")
      {
         inFile >> x;
         plane->ior = atof(x.c_str());
      }

      if(x == "rotate"){
         plane->rotate = parse_vec(inFile);
         order = order + "r";
      }
      if(x == "scale"){
         plane->scale = parse_vec(inFile);
         order = order + "s";
      }
      if(x == "translate"){
         plane->translate = parse_vec(inFile);
         order = order + "t";
      }
 
      inFile.ignore(1,'{');
      inFile >> x;
   }

   plane-> order = order;
   return plane;
}

Triangle* parse_tri(ifstream & inFile)
{
   string x;
   Triangle *tri = new Triangle();
   string order = "";
   tri->a = parse_vec(inFile);
   tri->b = parse_vec(inFile);
   tri->c = parse_vec(inFile); 
   glm::vec4 rgbf = parse_vec4(inFile);

   tri->color = glm::vec3(rgbf.x, rgbf.y, rgbf.z);
   tri->filter = rgbf.a;
 
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

      if(x == "refraction")
      {
         inFile >> x;
         tri->refraction = atof(x.c_str());
      }
      
      if(x == "ior")
      {
         inFile >> x;
         tri->ior = atof(x.c_str());
      }

      if(x == "rotate"){
         tri->rotate = parse_vec(inFile);
         order = order + "r";
      }
      if(x == "scale"){
         tri->scale = parse_vec(inFile);
         order = order + "s";
      }
      if(x == "translate"){
         tri->translate = parse_vec(inFile);
         order = order + "t";
      }
      inFile.ignore(1,'{');
      inFile >> x;
   }

   tri->order = order;
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
