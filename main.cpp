#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <glm/glm.hpp>
#include "Object.h"
#include "Parse.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


using namespace std;

vector<Object *> Scene;
Camera* cam;
vector<Light*> lights;
//Image class from CSC471. Used this for image writing.
class Image
{
public:
	Image(int w, int h) :
	width(w),
	height(h),
	comp(3),
	pixels(width*height*comp, 0)
{
}

Image()
{
}

void setPixel(int x, int y, float r, float g, float b)
{
	// The pixel data is laid out row by row. Each row consists of 'width'
	// columns, and each column consists of 3 unsigned chars.

	// First check for bounds
	if(y < 0 || y >= height) {
		cout << "Row " << y << " is out of bounds" << endl;
		return;
	}
	if(x < 0 || x >= width) {
		cout << "Col " << x << " is out of bounds" << endl;
		return;
	}

	// Since the origin (0, 0) of the image is the upper left corner, we need
	// to flip the row to make the origin be the lower left corner.
	y = height - y - 1;
	// index corresponding to row and col, (assuming single component image)
	int index = y*width + x;
	// Multiply by 3 to get the index for the rgb components.
	assert(index >= 0);
	assert(3*index + 2 < (int)pixels.size());
	pixels[3*index + 0] = r;
	pixels[3*index + 1] = g;
	pixels[3*index + 2] = b;
}


void getPixel(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b)
{
	// The pixel data is laid out row by row. Each row consists of 'width'
	// columns, and each column consists of 3 unsigned chars.

	// First check for bounds
	if (y < 0 || y >= height) {
		cout << "Row " << y << " is out of bounds" << endl;
		return;
	}
	if (x < 0 || x >= width) {
		cout << "Col " << x << " is out of bounds" << endl;
		return;
	}

	// Since the origin (0, 0) of the image is the upper left corner, we need
	// to flip the row to make the origin be the lower left corner.
	y = height - y - 1;
	// index corresponding to row and col, (assuming single component image)
	int index = y*width + x;
	// Multiply by 3 to get the index for the rgb components.
	assert(index >= 0);
	assert(3 * index + 2 < (int)pixels.size());
	r = pixels[3 * index + 0];
	g = pixels[3 * index + 1];
	b = pixels[3 * index + 2];
}


void writeToFile(const string &filename)
{
	// The distance in bytes from the first byte of a row of pixels to the
	// first byte of the next row of pixels
	int stride_in_bytes = width*comp*sizeof(unsigned char);
	int rc = stbi_write_png(filename.c_str(), width, height, comp, &pixels[0], stride_in_bytes);
}

private:
	int width;
	int height;
	int comp;
	std::vector<unsigned char> pixels;
};











void parse_scene(char* filename)
{
   ifstream inFile;
   inFile.open(filename);
   Parse *p;

   if(!inFile)
   {
      cerr << "Usage: raytrace raycast <inputfile> <width> <height>" << endl;
      exit(1);  
   }

   string x;
   while(inFile >> x)
   {
      if(x[0] == '/' || x[1] == '/')
      {
         getline(inFile, x);
         continue;
      }
      else if(x == "camera")
         cam = p->parse_cam(inFile);
      else if(x == "light_source")
         lights.push_back(p->parse_light(inFile));
      else if(x == "sphere")
         Scene.push_back(p->parse_sphere(inFile));
      else if(x == "plane")
         Scene.push_back(p->parse_plane(inFile));
      else if(x == "triangle")
         Scene.push_back(p->parse_tri(inFile));
   }
}

void print_info()
{
   cam->printstuff();
   cout << endl << "---\n" << endl;

   cout << lights.size() << " light(s)\n" << endl;
   for(int i = 0; i < lights.size(); i++)
   {
      cout << "Light[" << i << "]:" << endl;
      lights[i]->printstuff();
      cout << endl;
   }

   cout << "---\n" << endl;
   
   cout << Scene.size() << " object(s)\n" << endl;
   for(int i = 0; i < Scene.size(); i++)
   {
      cout << "Object[" << i << "]:" << endl;
         Scene[i]->printstuff();
         cout << endl;
   }

}

glm::vec3 getDvec(int i, int j, int width, int height)
{
   float U = (i + 0.5) / width - 0.5;
   float V = (j + 0.5) / height - 0.5;
   glm::vec3 new_look = cam->look_at - cam->location;
   glm::vec3 w = glm::normalize(new_look) * -1.0f;

   glm::vec3 p = cam->location + U * cam->right + V * cam->up + -1.0f * w;

   glm::vec3 dvec = p - cam->location;
   dvec = normalize(dvec);

   return normalize(dvec);   
}

bool inShadow(Light & l, glm::vec3 point)
{
  float epsilon = 0.001f;
  glm::vec3 ld = glm::normalize(l.location - point);
  
  Ray * lRay = new Ray(point, ld);

  for(int i = 0; i < Scene.size(); i++)
  {
    float s = Scene[i]->intersect(*lRay);
    if(s > epsilon && s < glm::distance(l.location, point))
    {
      return true;
    }
  }

  return false;
}

glm::vec3 raytrace(Ray & r, int lightType, int bounce)
{
   if(bounce == 6)
      return glm::vec3(0,0,0);
   //if(bounce > 0)
     // cout << "Bounce " << bounce << endl;
   float best = std::numeric_limits<float>::max();
   glm::vec3 bp;
   for(int k = 0; k < Scene.size(); k++)
   {
      float tmp = Scene[k]->intersect(r);
      if(tmp < best && tmp > 0)
      {
        float amb = Scene[k]->ambient;
        bp = glm::vec3(0,0,0);
        for(int m = 0; m < lights.size(); m++)
        {
          best = tmp;
          glm::vec3 pt = r.start + best * r.direction;
          if(lightType == 1)
           bp = bp + Scene[k]->blinnPhong(r, best, *(lights[m]), inShadow(*(lights[m]), pt));
          glm::vec3 normal = glm::normalize(Scene[k]->getNormal(pt));
          glm::vec3 reflectD = r.direction - 2 * glm::dot(r.direction, normal) * normal;
          pt = pt + 0.001f;
          Ray *reflectRay = new Ray(pt,reflectD);
          float finrefl = Scene[k]->getReflection();
          if(finrefl > 0)
          {
             glm::vec3 ambColor = Scene[k]->ambColor(*(lights[m]));
             bp = bp + finrefl * raytrace(*reflectRay, lightType, bounce + 1);
          }         
        }

        bp = glm::clamp(bp, 0.0f, 1.0f);
        
      }
   }
   
   return bp;
}

void raycast(int width, int height, int lightType)
{
  Image* image = new Image(width, height);
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
       glm::vec3 dvec = getDvec(i, j, width, height);
       Ray *r = new Ray(cam->location, dvec);

       glm::vec3 pixel = raytrace(*r, lightType, 0);
       float rr = pixel.x*255; float gg = pixel.y*255; float bb = pixel.z*255;
       image->setPixel(i, j, rr, gg, bb);
    }
  }

  image->writeToFile("output.png");
}

void pixelray(int width, int height, int i, int j, int type)
{
   glm::vec3 dvec = getDvec(i, j, width, height);
   Ray *r = new Ray(cam->location, dvec);
   cout << "Pixel: [" << i << ", " << j;
   cout << std::fixed; cout << std::setprecision(4);
   cout << "] Ray: {" << r->start.x << " " << r->start.y << " " << r->start.z;
   cout << "} -> {" << dvec.x << " " << dvec.y << " " << dvec.z << "}" << endl;
   if(type == 0)
      return;
   float best = std::numeric_limits<float>::max();
   string o;
   glm::vec3 bp;
   for(int k = 0; k < Scene.size(); k++)
   {
      float tmp = Scene[k]->intersect(*r);
      if(tmp < best && tmp > 0)
      {
       o = Scene[k]->getObjType();
       float amb = Scene[k]->ambient;
       bp = glm::vec3(0,0,0);
       for(int m = 0; m < lights.size(); m++)
       {
         best = tmp;
         glm::vec3 pt = r->start + best * r->direction;
         if(type == 0) bp = Scene[k]->getColor();
         if(type == 2)
           bp = bp + Scene[k]->blinnPhong(*r, best, *(lights[m]), inShadow(*(lights[m]), pt));
       }
       bp = glm::clamp(bp, 0.0f, 1.0f);
      }
   }
   if(best == std::numeric_limits<float>::max()){
      cout << "No Hit" << endl;
      return;
   }
   cout << "T = " << best << endl;
   cout << "Object Type: " << o << endl;
   if(type == 2)
   {
      cout << "BRDF: Blinn-Phong" << endl;
   }
   int rr = bp.x * 255;
   int gg = bp.y * 255;
   int bb = bp.z * 255;
   cout << "Color: (" << rr << ", " << gg << ", " <<  bb << ")" << endl;
}

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    cerr << "Usage: raytrace raycast <inputfile> <width> <height>" << endl;
    return 1;
  }

  string typeOfRun = argv[1];
  char *name = strchr(argv[2], '.');
  parse_scene(argv[2]);
  if(typeOfRun == "sceneinfo")
     print_info();
  else if(typeOfRun == "raycast")
     raycast(atoi(argv[3]), atoi(argv[4]), 0);
  else if(typeOfRun == "render")
     raycast(atoi(argv[3]), atoi(argv[4]), 1);
  else if(typeOfRun == "pixelray")
     pixelray(atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 0);
  else if(typeOfRun == "pixelcolor")
     pixelray(atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 2);
  else if(typeOfRun == "firsthit")
     pixelray(atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), 1);
  return 0; 
}
