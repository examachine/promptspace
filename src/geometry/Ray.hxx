//
//
// C++ Interface for module: Ray
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Ray_Interface
#define Ray_Interface

#include "General.hxx"

#include "Numerical_Vector.hxx"
#include "TMatrix.hxx"
#include "Transformation.hxx"
#include "Derived_Vectors.hxx"

class Ray
{
public:
  Ray() : start_pos(0,0,0), direction(1,0,0) {}

  Ray(Vector3 s, Vector3 d) : start_pos(s), direction(d) {
    direction.normalize_self();
  }

  Vector3 start_pos, direction;

  Vector3 shoot(double distance) {
    Vector3 arrow;
    arrow = direction;
    arrow *= distance;
    arrow += start_pos;
    return arrow;
    //return (Vector3)(start_pos + distance * direction;)
  }

  Ray transform(const Matrix4 & M, const Matrix4 & M2) {
    Ray ray;
    start_pos.transform(M, ray.start_pos);
    direction.transform(M2, ray.direction);
    //multiply(M, start_pos, ray.start_pos);
    //multiply(M2, direction, ray.direction);
    return ray;
  }

};

#endif
