//
//
// C++ Interface for module: Voxel
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Voxel_Interface
#define Voxel_Interface

#include "General.hxx"
#include "Color.hxx"

template<class _Color>
class Generic_Voxel
{
public:
  typedef _Color Color;
  typedef Generic_Voxel<_Color> Self;
  typedef double Transparency;

  const static double transparent_alpha = 0.95;
  const static double occluded_alpha = 0.05;

  Generic_Voxel() {}
  Generic_Voxel(Color c, double a) : color(c), alpha(a) {}

  Color color;			// Color of voxel
  Transparency alpha;           // Transparency of voxel

  Color sel_color() const { return color; }
  Transparency sel_transparency() const { return alpha; }

  double sel_density() {
    return color.sel_density();
  }
  
  bool transparent() {
    //return  (alpha < 0.9);
    return  (alpha > transparent_alpha);
  }

  bool opaque() {
    return  (alpha <= occluded_alpha);
  }

  void over(Self projected) {
    Transparency new_alpha = projected.alpha;
    color.add_self( projected.color.scl( alpha  * (1-new_alpha) ) );
    alpha *= new_alpha;
  }

  void over_alt(Self projected) {
    Transparency new_alpha = 0;
  }

  Self scl(double s) const {
    return Self(color.scl(s), alpha*s);
  }

  Self div_self(double s) {
    color.div_self(s);
    alpha /= s;
  }

  Self div(double s) const {
    return Self(color.div(s), alpha/s);
  }

  Self add(const Self & x) const {
    return Self(color.add(x.color), alpha+x.alpha);
  }

  //static const Self blank;
  // a blank voxel is a transparent voxel with a default color value,
  // preferably that of backdrop.
  static Self blank() { return Self(Color::blank(), 1); }
};

// static data member ... forget about it
//  template<class _Color>
//  const Generic_Voxel<_Color> Generic_Voxel<_Color>::blank
//    = Generic_Voxel<_Color> ( Voxel(_Color::blank, 1) );

template<class _Color>
Generic_Voxel<_Color> operator *(const Generic_Voxel<_Color> & x, double s)
{
  return x.scl(s);
}

template<class _Color>
Generic_Voxel<_Color> operator /(const Generic_Voxel<_Color> & x, double s)
{
  return x.div(s);
}

template<class _Color>
Generic_Voxel<_Color> operator +(const Generic_Voxel<_Color> & x,
				 const Generic_Voxel<_Color> & y)
{
  return x.add(y);
}

template<class _Color>
const double Generic_Voxel<_Color>::transparent_alpha;
template<class _Color>
const double Generic_Voxel<_Color>::occluded_alpha;

typedef Generic_Voxel<Gray_Color> Gray_Voxel;
typedef Generic_Voxel<Color> Voxel;

template<class C>
inline bool operator ==(Generic_Voxel<C>& a, Generic_Voxel<C>& b) {
return false; }


template <class C>
istream& operator >>(istream &in, Generic_Voxel<C>& v)
{
  if (in)
    return in;
}

template <class C>
ostream& operator <<(ostream &out, const Generic_Voxel<C>& v)
{
  out << "(" << v.sel_color() << "," << v.sel_transparency() << ")";
  return out;
}


//  inline bool operator !=(Gray_Voxel& a, Gray_Voxel& b) { return false; }
//  inline bool operator <(Gray_Voxel& a, Gray_Voxel& b) { return false; }
//  inline bool operator >(Gray_Voxel& a, Gray_Voxel& b) { return false; }

#endif
