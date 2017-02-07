//
//
// C++ Interface for module: Attribute
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Attribute_Interface
#define Attribute_Interface

#include "General.hxx"
#include "Color.hxx"

class Attribute {
public:

  Attribute(Gray_Color c, float a, float d, float s, int e)
    : color(c), ambient(a), diffuse(d), specular(s), exponent(e) {}

  Attribute()
    : color(1), ambient(0.8), diffuse(0.8), specular(0.85), exponent(7) {}

  Gray_Color color;
  float ambient,
    diffuse,
    specular;
  int  exponent;

};

#endif
