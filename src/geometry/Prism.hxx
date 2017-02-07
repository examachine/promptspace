//
//
// C++ Interface for module: Prism
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Prism_Interface
#define Prism_Interface

#include "General.hxx"
#include "Ray.hxx"
#include "Derived_Vectors.hxx"

class Prism
{
public:
  Prism() {
    init_boundaries( Vector3(0,0,0) );
  }

  Prism(Vector3 boundaries) {
    init_boundaries(boundaries.x(), boundaries.y(), boundaries.z());
  }

  void init_boundaries(Vector3 bounds) {
    init_boundaries(bounds.sel_x(), bounds.sel_y(), bounds.sel_z());
  }

  void init_boundaries(double x, double y, double z) {
//      x_bnd = Vector2(-x/2, x/2);
//      y_bnd = Vector2(-y/2, y/2);
//      z_bnd = Vector2(-z/2, z/2);
    x_bnd = Vector2(0, x);
    y_bnd = Vector2(0, y);
    z_bnd = Vector2(0, z);
  }

  Vector3 sel_center() {
    return Vector3( (x_bnd.comp(0) + x_bnd.comp(1))/2,
		    (y_bnd.comp(0) + y_bnd.comp(1))/2,
		    (z_bnd.comp(0) + z_bnd.comp(1))/2 );
  }
 
  Vector2 sel_x_bnd() { return x_bnd; }
  Vector2 sel_y_bnd() { return y_bnd; }
  Vector2 sel_z_bnd() { return z_bnd; }

  bool on_boundary(Vector3 point);

  bool in_bounds(const Vector3 & point) const {
    return in_boundary(point.sel_x(), x_bnd) &&
      in_boundary(point.sel_y(), y_bnd) &&
      in_boundary(point.sel_z(), z_bnd);
  }

  static const double e = 0.001;
  bool in_e_bounds(const Vector3 & point) const {
    // BUG: no p here of course
    //Vector3 p = point;
    //cerr << "in_e_bounds: " << p << endl;
    Vector2 x_e(x_bnd + Vector2(-e,e));
    Vector2 y_e(y_bnd + Vector2(-e,e));
    Vector2 z_e(z_bnd + Vector2(-e,e));
    return in_boundary(point.sel_x(), x_e) &&
      in_boundary(point.sel_y(), y_e) &&
      in_boundary(point.sel_z(), z_e);
  }

  double volume();

  Vector2 shoot_boundary(Ray ray);

protected:
  Vector2 x_bnd, y_bnd, z_bnd;

  double length_boundary(Vector2 boundary) {
    return abs(boundary.comp(0) - boundary.comp(1));
  }

  bool in_boundary (double point, Vector2 boundary) const {
    //return boundary.comp(0) <= point <= boundary.comp(1);
    return (boundary.comp(0) <= point) && (point <= boundary.comp(1));
  }

  bool on_boundary (double point, Vector2 boundary) {
    return boundary.comp(0) == point  || boundary.comp(1) == point;
  }

  double shoot_boundary(double r_s, double r_d, double boundary) {
    double delta = boundary - r_s;
    if (r_s!=0)
      return delta/r_d;
    else
      return 0;
  }

  //  bool on_boundary (double point, Vector2 boundary, Vector2 boundary1) {
//      return on_boundary(point, boundary0) &&
//        on_boundary(point, boundary1);
//    }

  bool in_boundary (Vector2 point, Vector2 boundary0, Vector2 boundary1) {
    return in_boundary(point.comp(0), boundary0) &&
      in_boundary(point.comp(1), boundary1);
  }

};


#endif
