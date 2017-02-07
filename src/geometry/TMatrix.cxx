//
//
// C++ Implementation for module: TMatrix
//
// Description: 
//
//
// Author: exa
//
//

#include "TMatrix.hxx"

/* Rotation Matrix for Euler Angles
   [ cosb*cosc                 cosb*sinc                 -sinb      0 ]
   [ sina*sinb*cosc-cosa*sinc  sina*sinb*sinc-cosa*cosc  sina*cosb  0 ]
   [ cosa*sinb*cosc+sina*sinc  cosa*sinb*sinc-sina*cosc  cosa*cosb  0 ]
   [ 0                         0                         0          1 ]
   a angles about x, b angles about y, c angles about z                */

void Rotation_Matrix4::mod(const Angles & A)
{
  double cosa, sina, cosb, sinb, cosc, sinc;

  cosa=cos(A.yaw);     sina=sin(A.yaw);
  cosb=cos(A.pitch);   sinb=sin(A.pitch);
  cosc=cos(A.roll);    sinc=sin(A.roll);

  mod_element(0, 0, cosb*cosc);
  mod_element(0, 1, cosb*sinc);
  mod_element(0, 2, -sinb);
  mod_element(1, 2, sina*cosb);
  mod_element(2, 2, cosa*cosb);

  double cosacosc, sinacosc, cosasinc, sinasinc;

  cosacosc=cosa*cosc;
  sinacosc=sina*cosc;
  cosasinc=cosa*sinc;
  sinasinc=sina*sinc;

  mod_element(1, 0, sinacosc*sinb - cosasinc);
  mod_element(1, 1, sinasinc*sinb + cosacosc);
  mod_element(2, 0, cosacosc*sinb + sinasinc);
  mod_element(2, 1, cosasinc*sinb - sinacosc);

  for (int i=0;i<=2;i++) {
    mod_element(3,i,0); 
    mod_element(i,3,0); 
  }
  mod_element(3, 3, 1);
}
