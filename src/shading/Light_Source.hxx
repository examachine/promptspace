//
//
// C++ Interface for module: Light_Source
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Light_Source_Interface
#define Light_Source_Interface

#include "General.hxx"
#include "Component.hxx"
#include "Attribute.hxx"
#include "Numerical_Vector.hxx"
#include "Derived_Vectors.hxx"
#include "Ray.hxx"

class Light_Source : public Component {
public:

  Gray_Color color;
  float I;
  float ambient;
  Ray ray;
  
  Light_Source() {
    init(Gray_Color(1),  0.80, 0.20, Ray( Vector4(-1,-2,-5,1),
					 Vector3(-1,-1,-5) ) );
  }

  Light_Source(Gray_Color c, double i, double a, Ray r) {
    init(c, i, a, r);
  }

  // init(Color(1,1,1), 0.7, 0.2, Vector3(0,0,1));

  void init(Gray_Color c, double i, double a, Ray r) {
    color = c; I = i; ambient = a; ray = r;
    //parent.translate(Vector3(0, 0, -2));
  }

};


#endif
