#ifndef Earthquake_h
#define Earthquake_h

#include <string>
#include <memory>
#include "Mesh.h"
#include "Sphere.h"
//#include "Texture.h"

#include "config.h"

#include "Date.h"

class Earthquake {
public:
  Earthquake();
  
  // Create an earthquake from a datafile's line of text
  Earthquake(std::string s);
  
  Date getDate();
  
  double getLongitude();
  
  double getLatitude();
  
  double getMagnitude();

  void draw(basicgraphics::GLSLProgram &shader,mat4 model);
  
protected:
  
  std::string line;

  double parseFloat(std::string s);

  int parseInt(std::string s);
  
};

#endif
