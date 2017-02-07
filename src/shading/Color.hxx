//
//
// C++ Interface for module: Color
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Color_Interface
#define Color_Interface

#include "General.hxx"
#include "Numerical_Vector.hxx"
#include "Derived_Vectors.hxx"

struct RGB {
  RGB() {}

  RGB(float _r, float _g, float _b) {
    r = convert_intensity(_r);
    g = convert_intensity(_g);
    b = convert_intensity(_b);
  }

  unsigned char convert_intensity(float i) {
    return int( i * 255 );
  }

  unsigned char r,g,b;
};

class Color {
public:
  Color() {}

  Color(float _r, float _g, float _b)
    : r(_r), g(_g), b(_b) {}

  Color(Vector3 col) {
    mod(col.comp(0), col.comp(1), col.comp(2));
  }

  void mod(float _r, float _g, float _b) {
    r = _r, g = _g; b = _b;
  }

  double sel_r() const { return r; }
  double sel_g() const { return g; }
  double sel_b() const { return b; }

  double sel_density() { return (r+g+b)/3; }

  double mod_r(double _r) { r = _r; }
  double mod_g(double _g) { g = _g; }
  double mod_b(double _b) { b = _b; }

  operator RGB() { return RGB(r,g,b); }
  operator Vector3() { return Vector3(r,g,b); }

  Color scl(double s) const { return Color(r*s, g*s, b*s); }
  Color div(double s) const { return Color(r/s, g/s, b/s); }
  void div_self(double s) { r/=s; g/=s; b/=s; }
  Color add(const Color & x) const { return Color(r+x.r, g+x.g, b+x.b); }
  void add_self(const Color & x) { r+=x.r; g+=x.g; b+=x.b; }

  float r,g,b;

  static Color blank() { return Color(0,0,0); }
  //static const Color blank = Color(0,0,0);
};

inline ostream& operator <<(ostream &out, const Color &c)
{
  out << "(" << c.sel_r() << "," << c.sel_g()
      << "," << c.sel_b() << ")";
  return out;
}


class Gray_Color {
public:
  Gray_Color() {}

  Gray_Color(float _i)
    : i(_i) {}

  double sel_i() const { return i; }

  double mod_i(double _i) { i = _i; }

  double sel_density() { return i; }

  operator float() { return i; }

  operator RGB() { return RGB(i,i,i); }
  operator Vector3() { return Vector3(i,i,i); }

  Gray_Color scl(double s) const { return Gray_Color(i*s); }
  Gray_Color div(double s) const { return Gray_Color(i/s); }
  void div_self(double s) { i/=s; }
  Gray_Color add(const Gray_Color & x) const { return Gray_Color(i+x.i); }
  void add_self(const Gray_Color & x) { i+=x.i; }

  float i;

  static Gray_Color blank() { return Gray_Color(0); }
  //static const Gray_Color blank = Gray_Color(0);
};

inline ostream& operator <<(ostream &out, const Gray_Color &c)
{
  out << "(" << c.sel_i() << ")";
  return out;
}

//static const Color Color::blank(0,0,0);

//static const Gray_Color Gray_Color::blank(0);


#endif
