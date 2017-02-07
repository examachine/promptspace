//
//
// C++ Interface for module: Viewing_Transformation
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Viewing_Transformation_Interface
#define Viewing_Transformation_Interface

#include "General.hxx"

#include "Screen.hxx"
#include "Derived_Vectors.hxx"

class Viewing_Transformation
{
public:
  Camera & camera;
  Screen & screen;

  Translation_Matrix4 T;
  Rotation_Matrix4 R;
  Scaling_Matrix4 S;
  Translation_Matrix4 T2;
  
  Matrix4 M1; 
  Matrix4 M2;
  Matrix4 V; // M : temporary, V : transformation matrix

  Viewing_Transformation(Camera & c, Screen & s)
    : camera(c), screen(s){
  }

  void update() {

    // translate, so that the world is centered around the cam!
    Vector3 pos = (Vector3)camera.parent.sel_pos();
    pos.negate_self();
    //pos.add(Vector3(screen.width/2, screen.height/2, 0));
    T.mod(pos);
//      cerr << "pos: " << pos << endl;
//      cerr << "T: " << endl << T << endl;

    // rotate the world!
    Angles orn = camera.parent.sel_orn();
    orn.inverse();
    R.mod(orn);
//      cerr << "angles: " << a.yaw << "," <<
//        a.pitch << "," <<
//        a.roll << endl;
//      cerr << "R: " << endl << R << endl;


//      S.mod(Vector3(screen.sel_viewing_width(),
//  		  -screen.sel_viewing_height(), 1));
    S.mod(Vector3(screen.sel_viewing_width(),
		  screen.sel_viewing_height(), 1));
//      cerr << "screen: " << screen.width << ":" << screen.height<< endl;
//      cerr << "S: " << endl << S << endl;
    
    R.multiply_inplace(T, M1);
    S.multiply_inplace(M1, M2);
    T2.mod(Vector3(screen.sel_viewing_width()/2,
		   screen.sel_viewing_height()/2, 1));
    T2.multiply_inplace(M2, V);
  }

  Matrix4 & sel_V() {
    update();
    return V;
  }

};

#endif





