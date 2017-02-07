//
//
// C++ Interface for module: TMatrix
//
// Description: 
//
//
// Author: exa
//
//

#ifndef TMatrix_Interface
#define TMatrix_Interface

#include "General.hxx"
#include "Numerical_Vector.hxx"
#include "Matrix.hxx"


#include "Derived_Vectors.hxx"

// specialized transformations

// Linear Transformations

class Translation_Matrix4 : public Identity_Matrix<float,4>
{
public:
  Translation_Matrix4() {}
  Translation_Matrix4(const Vector3 & T) {
    mod(T);
  }
  void mod(const Vector3 & T) {
    mod_element(0, 3, T.sel_component(0) );
    mod_element(1, 3, T.sel_component(1) );
    mod_element(2, 3, T.sel_component(2) );
  }
};

class Scaling_Matrix4 : public Identity_Matrix<float,4>
{
public:
  Scaling_Matrix4() {}
  Scaling_Matrix4(const Vector3 & S) {
    mod(S);
  }
  void mod(const Vector3 & S) {
    mod_element(0, 0, S.sel_component(0) );
    mod_element(1, 1, S.sel_component(1) );
    mod_element(2, 2, S.sel_component(2) );
  }
};


#include "Angles.hxx"

// This class not derived from Identity_Matrix
// since it does implicitly
class Rotation_Matrix4 : public Sqr_Matrix<float,4>
{
public:
  Rotation_Matrix4() {}
  Rotation_Matrix4(const Angles & A) {
    mod(A);
  }
  void mod(const Angles & A);
  const Matrix4& operator *() const { 
    //return static_cast<Matrix4&>(*this); 
    //BUG: better way to cast this shit
    return (const Matrix4 &)(*this); 
  }
};

class Shear_Matrix4 : public Identity_Matrix<float,4>
{
public:
  Shear_Matrix4() {}
  Shear_Matrix4(const Vector2 & shear) {
    mod(shear);
  }
  void mod(const Vector2 & shear) {
    mod_element(0, 2, shear.sel_component(0) );
    mod_element(1, 2, shear.sel_component(1) );
  }
  const Matrix4& operator *() const { 
    //return static_cast<Matrix4&>(*this); 
    //BUG: better way to cast this shit
    return (const Matrix4 &)(*this); 
  }
};

class Permutation_Matrix4 : public Sqr_Matrix<float,4>
{
public:
  Permutation_Matrix4() {}
  Permutation_Matrix4(int px, int py, int pz) {
    mod(px,py,pz);
  }

  void mod(int px, int py, int pz) {
    mod_each_element(0);
    mod_element(0,px,1);
    mod_element(1,py,1);
    mod_element(2,pz,1);
  }

  const Matrix4& operator *() const { 
    //return static_cast<Matrix4&>(*this); 
    //BUG: better way to cast this shit
    return (const Matrix4 &)(*this); 
  }

};


#endif
