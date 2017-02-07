//
//
// C++ Interface for module: Phong_Shader
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Phong_Shader_Interface
#define Phong_Shader_Interface

#include "General.hxx"
#include "Derived_Vectors.hxx"
#include "Color.hxx"
#include "Light_Source.hxx"
#include "Attribute.hxx"

template <class _Color>
class Phong_Shader
{
public:
  typedef _Color Color;

  Phong_Shader() {}

  Color shade(const Color col, Attribute & obj_attribute, Vector3 N,
	     Light_Source & light, Vector3 V) {
    
    Vector3 L = light.ray.direction;
    L.negate_self();
    
    float   ambient, kd, ks;
    int     e;

    // viewed ambient intensity over material is constant
    ambient = obj_attribute.ambient*light.ambient;
    
    kd=obj_attribute.diffuse*light.I;  // material characteristix
    ks=obj_attribute.specular*light.I;
    e=obj_attribute.exponent;

    // Assume that L, N, V have been normalized already
    double diffuse = N.dot_product(L);  // diffuse component
    diffuse = max(double(0), diffuse)*kd;
    //diffuse = abs(diffuse)*kd;

    Vector3 H = V + L; H.normalize_self();
    double specular = N.dot_product(H); // specular component
    specular = pow(max(0.0, specular)*ks, e);

    return Color(col.scl(ambient + diffuse + specular) );
  }

};


#endif
