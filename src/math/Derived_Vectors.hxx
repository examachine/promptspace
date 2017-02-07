//
//
// C++ Interface for module: Derived_Vectors
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Derived_Vectors_Interface
#define Derived_Vectors_Interface

#include "General.hxx"
#include "Matrix.hxx"

// CODE: special output for these? 
class Vector2 : public Num_Vector<double,2> {
public:
  Vector2() {}

  Vector2(double x, double y) {
    component[0] = x;
    component[1] = y;
  }

  // CODE: Alleviate use of copy cons' for this
  Vector2(const Num_Vector<double, 2> &v) {
    component[0] = v.comp(0);
    component[1] = v.comp(1);
  }

  double x() { return component[0]; }
  double y() { return component[1]; }

  double sel_x() { return component[0]; }
  double sel_y() { return component[1]; }

  double mod_x(double _x) { component[0] = _x; }
  double mod_y(double _y) { component[1] = _y; }

};

//  inline bool operator ==(const Vector2 & u, const Vector2 &v) {
//    return u.x==v.x && u.y==v.y;
//  }


// 3 dimensional scalar vector class
class Vector3 : public Num_Vector<double,3> {
public:
  Vector3() {}

  Vector3(double x, double y, double z) {
    component[0] = x;
    component[1] = y;
    component[2] = z;
  }

  Vector3(const Num_Vector<double, 3> & v) {
    component[0] = v.comp(0);
    component[1] = v.comp(1);
    component[2] = v.comp(2);
  }

  double x() const { return component[0]; }
  double y() const { return component[1]; }
  double z() const { return component[2]; }

  double sel_x() const { return component[0]; }
  double sel_y() const {  return component[1]; }
  double sel_z() const { return component[2]; }

  double mod_x(double _x) { component[0] = _x; }
  double mod_y(double _y) { component[1] = _y; }
  double mod_z(double _z) { component[2] = _z; }

  Vector3 transform(const Matrix4 & M) const {
    Vector3 v_p ( M.elt(0,0)*x() + M.elt(0,1)*y() +
		  M.elt(0,2)*z() + M.elt(0,3) ,
		  M.elt(1,0)*x() + M.elt(1,1)*y() +
		  M.elt(1,2)*z() + M.elt(1,3) ,
		  M.elt(2,0)*x() + M.elt(2,1)*y() +
		  M.elt(2,2)*z() + M.elt(2,3) );
    return v_p;
  }

  void transform(const Matrix4 & M, Vector3 & v_p) const {
    v_p.mod_x(M.elt(0,0)*x() + M.elt(0,1)*y()
	      + M.elt(0,2)*z() + M.elt(0,3) );
    v_p.mod_y(M.elt(1,0)*x() + M.elt(1,1)*y()
	      + M.elt(1,2)*z() + M.elt(1,3) );
    v_p.mod_z(M.elt(2,0)*x() + M.elt(2,1)*y()
	      + M.elt(2,2)*z() + M.elt(2,3) );
  }

};

// 4 dimensional scalar vector class
class Vector4 : public Num_Vector<double,4> {
public:
  Vector4() {}

  Vector4(double x, double y, double z, double w) {
    component[0] = x;
    component[1] = y;
    component[2] = z;
    component[3] = w;
  }

  Vector4(const Num_Vector<double, 3> & v) {
    component[0] = v.comp(0);
    component[1] = v.comp(1);
    component[2] = v.comp(2);
    component[3] = 1;
  }

  Vector4(const Num_Vector<double, 4> & v) {
    component[0] = v.comp(0);
    component[1] = v.comp(1);
    component[2] = v.comp(2);
    component[3] = v.comp(3);
  }

  double x() { return component[0]; }
  double y() { return component[1]; }
  double z() { return component[2]; }
  double w() { return component[3]; }

  double sel_x() { return component[0]; }
  double sel_y() { return component[1]; }
  double sel_z() { return component[2]; }
  double sel_w() { return component[3]; }

  double mod_x(double _x) { component[0] = _x; }
  double mod_y(double _y) { component[1] = _y; }
  double mod_z(double _z) { component[2] = _z; }
  double mod_w(double _w) { component[3] = _w; }

  operator Vector3() { return Vector3(sel_x(),sel_y(),sel_z()); }

  Vector4 transform(const Matrix4 & M) const {
    Vector4 v_p;
    multiply(M, *this, v_p);
    return v_p;
  }
};

// 3 dimensional integer vector class
class IVector3 : public Num_Vector<int,3> {
public:
  IVector3() {}

  IVector3(int x, int y, int z) {
    component[0] = x;
    component[1] = y;
    component[2] = z;
  }

  IVector3(const Num_Vector<int, 3> & v) {
    component[0] = v.comp(0);
    component[1] = v.comp(1);
    component[2] = v.comp(2);
  }

  int x() const { return component[0]; }
  int y() const { return component[1]; }
  int z() const { return component[2]; }

  int sel_x() const { return component[0]; }
  int sel_y() const {  return component[1]; }
  int sel_z() const { return component[2]; }

  int mod_x(int _x) { component[0] = _x; }
  int mod_y(int _y) { component[1] = _y; }
  int mod_z(int _z) { component[2] = _z; }

};


#endif
