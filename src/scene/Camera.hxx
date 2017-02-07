//
//
// C++ Interface for module: Camera
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Camera_Interface
#define Camera_Interface

#include "General.hxx"

#include "Numerical_Vector.hxx"
#include "Derived_Vectors.hxx"
#include "Component.hxx"
#include "Ray.hxx"
#include "Screen.hxx"

// Camera class, simulates a camera that may be positioned as any
// component in the world may be.
// Defines a view Frustrum which is visible by the camera
// with the help of feld of view angle, and clip planes' distance
// from the z-plane
// The camera by default looks down the z-axis
// CODE: look_at(Component)
//       track(Component)

//  struct Camera_Params
//  {
//    Camera_Params( Ray r, Vector4 x, Vector4 y)
//      : ray(r),  screen_x_2(x), screen_y_2(y)
//  }

class Camera : public Component, public Screen {
public:

  //static double default_field_of_view=1.7;

//    Camera(double _fov) {
//      fov=_fov;
//      translate(Vector3(0, 0, -2));
//      frontClip=1; backClip=16;
//    }

  Camera( Screen screen )
    : Screen(screen),
//        Ray(Vector4(screen.width/2,screen.height/2,-2,1),
//  	  Vector4(0,0,1,1) )
      ray( Vector3(0,0,0),
	   Vector3(0,0,1) )
      //, look_at_pos(screen.width/2, screen.height/2, 0.5)
    //: front_clip(1), back_clip(64)
  {
    //fov=deafult_field_of_view;

    // let the camera rotate about the centre of the volume

    // place camera in front of the volume that extends from the
    // origin towards the positive axis
    //parent.translate( Vector3(screen.width/2, screen.height/2, -2));

    //parent.rotate(Angles(0,M_PI/4*0,0));

    //local.rotate(Angles(0,M_PI/4*0,0));
    screen_x_2 = Vector3(screen.width/2, 0, 0);
    screen_y_2 = Vector3(0, screen.height/2, 0);
    parent.mod_joint(Vector3(0,0,0));
  }

  // CODE: how do we make the virtual screen perpendicular to
  // our viewing ray?
//      Camera(Ray r, Vector3 x, Vector3 y)
//        : Ray(r), screen_x_2(x), screen_y_2(y) 
//  	//front_clip(1), back_clip(64)
//      {}

  // CODE: this sucks exclusively

  Matrix4 T_cam; // there should be no need
  Translation_Matrix4 T;
  Camera transform() {
    Camera cam_p = *this; // copy cam
    //const Matrix4 & parent_R = static_cast<Matrix4 &> ( parent.sel_R() ) ;
    const Matrix4 & parent_R = * parent.sel_R();
    T.mod(parent.sel_joint());
    T.multiply_inplace(parent.sel_M(), T_cam);
    //cam_p.ray = ray.transform(parent.sel_M(), parent_R);
    cam_p.ray = ray.transform(T_cam, parent_R);
    cam_p.screen_x_2 = screen_x_2.transform(parent_R);
    cam_p.screen_y_2 = screen_y_2.transform(parent_R);
    return cam_p;
//      return Camera( Ray::transform( sel_M() ),
//  		   screen_x_2( sel_M() ),
//  		   screen_y_2( sel_M() ));
  }
  
//    Matrix4 &  sel_viewing_transformation() {
//      //CODE: ad hoc merge viewing transformations, and normal ones
    
//      return parent.sel_V();
//    }

  // a ray at the centre of screen which points at the viewing direction
  Ray ray;
  // vectors to indicate the size of our screen
  Vector3 screen_x_2, screen_y_2;

  Vector3 sel_screen_tl() {
    return (ray.start_pos - screen_x_2 - screen_y_2);
  }

  Vector3 sel_screen_tr() {
    return (ray.start_pos + screen_x_2 - screen_y_2);
  }

  Vector3 sel_screen_bl() {
    return (ray.start_pos - screen_x_2 + screen_y_2);
  }

  Vector3 sel_screen_br() {
    return (ray.start_pos + screen_x_2 + screen_y_2);
  }

  double fov;
  double front_clip, back_clip;

};


#endif
