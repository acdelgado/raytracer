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

class Params
{
 public:
   glm::vec2 imageSize;
   bool useShading;
   bool useShadows;
   bool useCookTorrance;
   bool useReflections;
   bool useRefractions;
   bool useFresnel;
   bool useBeers;

   int recursiveDepth;
   int superSampling;

   bool debugNormals;

   Params(){
     useShading = true;
     useShadows = true;
     useCookTorrance = false;
     useReflections = true;
     useRefractions = true;
     useFresnel = false;
     useBeers = false;

     recursiveDepth = 6;
     superSampling = 1;

     debugNormals = false;
   }
};


Params params;

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
      else if(x == "box")
         Scene.push_back(p->parse_box(inFile));
   }

   for(int i = 0; i < Scene.size(); i++)
      Scene[i]->calcTransformations();
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

Ray * transformRay(glm::mat4 & M, Ray & r)
{
   glm::vec4 newDir = glm::vec4(r.direction.x, r.direction.y, r.direction.z, 0);
   newDir = glm::inverse(M) * newDir;
   glm::vec3 transDir = glm::vec3(newDir.x, newDir.y, newDir.z);

   glm::vec4 newpt = glm::vec4(r.start.x, r.start.y, r.start.z, 1);
   newpt = glm::inverse(M) * newpt;
   glm::vec3 transpt = glm::vec3(newpt.x, newpt.y, newpt.z);

   return new Ray(transpt, transDir);
}

bool inShadow(Light & l, glm::vec3 point)
{
  float epsilon = 0.001f;
  glm::vec3 ld = glm::normalize(l.location - point);
  
  Ray * lRay = new Ray(point, ld);

  for(int i = 0; i < Scene.size(); i++)
  {
      glm::mat4 Model = Scene[i]->getModel();
      Ray * tRay = transformRay(Model, *lRay);

      float s = Scene[i]->intersect(*tRay);
      if(s > epsilon && s < glm::distance(l.location, point))
      {
        return true;
      }
  }

  return false;
}

void printvec(glm::vec3 v)
{
   cout << v.x << " " << v.y << " " << v.z << endl;
}



glm::vec3 raytrace(Ray & r, int lightType, int bounce)
{
   if(bounce >= 6)
      return glm::vec3(0,0,0);
   float best = std::numeric_limits<float>::max();
   glm::vec3 bp;
   for(int k = 0; k < Scene.size(); k++)
   {
      glm::mat4 Model = Scene[k]->getModel();
      glm::mat4 Inv = Scene[k]->getInverse();
      glm::mat4 Norm = Scene[k]->getNormMat();
      Ray * transformR = transformRay(Model, r);

      float tmp = Scene[k]->intersect(*transformR);
      if(tmp < best && tmp > 0)
      {
        bp = glm::vec3(0,0,0);
        for(int m = 0; m < lights.size(); m++)
        {
          best = tmp;

          glm::vec3 pt = r.start + best * r.direction;
          glm::vec3 tpt = transformR->start + best * transformR->direction;
          
          if(lightType == 1)
           bp = bp + Scene[k]->blinnPhong(*transformR, best, *(lights[m]), inShadow(*(lights[m]), pt));

          glm::vec3 normal = glm::normalize(Scene[k]->getNormal(tpt));

          glm::vec3 reflectD = r.direction - 2 * glm::dot(r.direction, normal) * normal;
          glm::vec3 view = -(r.direction);
          float dDotN = glm::dot(view, normal);

         // float f0 = pow((n2 - 1),2.f) / pow((n2 + 1), 2.f);
         // float fresnel = 0;
         // if(params.useFresnel)
         //    fresnel =  f0 + (1 - f0) * pow(1 - glm::abs(glm::dot(normal, view)), 5.f);
          float finrefl = Scene[k]->getReflection();
          float finrefr = Scene[k]->getFilter();

          bp = bp * (1.0f - finrefl) * (1.0f - finrefr);


          if(finrefl > 0)
          {
             glm::vec3 reflectPt = glm::vec3(pt.x,pt.y,pt.z);

             if(dDotN >= 0){
                reflectPt += normal * 0.001f;
             }
             else{
                reflectPt -= normal * 0.001f;
             }
             Ray *reflectRay = new Ray(reflectPt,reflectD);
             glm::vec3 reflectResult = raytrace(*reflectRay, lightType, bounce + 1);
             bp += reflectResult * finrefl * Scene[k]->getColor() * (1.0f - finrefr);// + finrefr * fresnel;
          }
          if(Scene[k]->getRefraction() > 0)
          {
             float n = 0;
             glm::vec3 normie = normal;

             float n2 = Scene[k]->getIOR();          
             glm::vec3 refractPt = glm::vec3(pt.x, pt.y, pt.z);

             if(dDotN >= 0){
                refractPt -= normal * 0.001f;
                n = 1.f/n2;
             }
             else{
                refractPt += normal * 0.001f;
                n = n2/1.f;
                normie = -normal;
             }
             const float c1 = glm::dot(view, normie);
             const float c2 = 1 - (n * n) * (1 - (c1 * c1));
             glm::vec3 refractD = glm::vec3(0,0,0);
             if(c2 > 0)
                refractD = (-view * n) + normie * (n * c1 - glm::sqrt(c2));
             
             Ray *t = new Ray(refractPt, refractD);
             glm::vec3 result = raytrace(*t, lightType, bounce+1) * finrefr;//(1 - fresnel);
             result = result * Scene[k]->getColor();
             bp += result;
             
          }
       }

        
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
       bool print = false;
       if(i == 399 && j == 280)
          print = true;

       glm::vec3 pixel = glm::clamp(raytrace(*r, lightType, 0),0.0f, 1.0f);
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

void readFlags(int argc, char **argv)
{
   params = Params();

   for(int i = 0; i < argc; i++)
   {
      if(!strcmp(argv[i], "-fresnel"))
         params.useFresnel = true;
      if(!strcmp(argv[i], "-altbrdf"))
         params.useCookTorrance = true;
      if(!strcmp(argv[i], "-beers"))
         params.useBeers = true;
   }

}

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    cerr << "Usage: raytrace raycast <inputfile> <width> <height>" << endl;
    return 1;
  }


  readFlags(argc, argv);


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
