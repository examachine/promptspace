//
//
// C++ Interface for module: Screen
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Screen_Interface
#define Screen_Interface

#include "General.hxx"

class Screen
{
public:
  Screen(float _width, float _height, int s)
    : width(_width), height(_height), subdivision(s)
  {
    viewing_width = int(width * subdivision);
    viewing_height = int(height * subdivision);
    nlog << "Screen: " << viewing_width << "X" << viewing_height << endl;
  }

  int sel_subdivision() const {
    return subdivision;
  }

  int sel_viewing_width() const {
    return viewing_width; 
  }

  int sel_viewing_height() const {
    return viewing_height;
  }

  //protected:
  float width, height;
  int subdivision;
  int viewing_width, viewing_height;
};


#endif
