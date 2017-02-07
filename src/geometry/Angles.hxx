//
//
// C++ Interface for module: Angles
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Angles_Interface
#define Angles_Interface

#include "General.hxx"

class Angles
{
public:
  Angles() {}

  Angles(float y, float p, float r)
    : yaw(y), pitch(p), roll(r) {}

  double yaw, pitch, roll;

  void add(Angles d) {
    yaw = fmod(yaw + d.yaw, 2*M_PI);
    pitch = fmod(pitch + d.pitch, 2*M_PI);
    roll = fmod(roll + d.roll, 2*M_PI);
  }

  void inverse() {
    yaw = -yaw;   pitch = -pitch;   roll = -roll;
  }

  Angles inverse_gen() {
    Angles a;
    a.yaw = -yaw;   a.pitch = -pitch;   a.roll = -roll;
    return a;
  }

};

inline ostream& operator <<(ostream &out, const Angles & a)
{
  out << "(";
  out << a.yaw << "," << a.pitch << "," << a.roll;
  out << ")";
  return out;
}

#endif
