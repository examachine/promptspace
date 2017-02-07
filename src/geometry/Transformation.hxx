//
//
// C++ Interface for module: Transformation
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Transformation_Interface
#define Transformation_Interface

#include "General.hxx"
#include "Numerical_Vector.hxx"
#include "Angles.hxx"
#include "TMatrix.hxx"
#include "Derived_Vectors.hxx"

// Transformation class encapsulates a chain of linear transformations
// which prepares an object space for translation into another; the
// linear transformations are collapsed into a compound transformation
// matrix available to the client.

// These transformations typically involve a scaling operation,
// a joint merging, a rotation, and a repositioning within a single frame.

// For a top level object, the rotation and repositioning indicate the
// whereabouts of it.

// In a terminal child object, we would usually not require a repositioning
// if the joints are not flexible.

// A joint object, on the other hand, would typically consist of
// just a reposition (Considering that the joint itself is fixed)


class Transformation {
public:

  // scaling: scale the model into an appropriate size for inclusion
  // in the target space
  Vector3 scl;			// scaling
  Scaling_Matrix4 S;
  bool do_scl;

  // translation: offset the centre of object so that it has a
  // meaningful center of rotation
  Vector3 pos;
  Translation_Matrix4 T;
  bool do_translate, T_updated;

  // joint: determine the position of the joint for inclusion in a
  // hierarchical model
  Vector3 joint;		// joint to higher level
  //Translation_Matrix4 T_j;

  // rotation: rotate the object space about (translation-joint).
  Angles orn;			// orientation
  Rotation_Matrix4 R;
  bool do_rot, R_updated;

  // the compound transformation matrix
  Matrix4 M;
  bool M_updated;
  Matrix4 M_1;
  bool M_noS;
  bool M_noS_updated;

  // construction

  Transformation()
    : scl(1,1,1), orn(0,0,0), pos(0,0,0),
      do_scl(false), do_rot(false), R_updated(false), M_noS(false),
      M_noS_updated(false) {
    //cerr << "transformation: " << scl << orn << pos << endl;
  }
  
  Transformation(Vector3 s, Angles a, Vector3 t)
    : scl(s), orn(a), pos(t), joint(0,0,0),
      do_scl(false), do_rot(false), R_updated(false), M_noS(false),
      M_noS_updated(false) {}
  
  Transformation(Angles a, Vector3 t)
    : scl(1,1,1), orn(a), pos(t), joint(0,0,0),
      do_scl(false), do_rot(false), R_updated(false), M_noS(false),
      M_noS_updated(false) {}

  Transformation(Vector3 t)
    : scl(1,1,1), orn(0,0,0), pos(t), joint(0,0,0),
      do_scl(false), do_rot(false), R_updated(false), M_noS(false),
      M_noS_updated(false) {}

  // scaling

  Vector3 sel_scl() {
    return scl;
  }

  void mod_scl(Vector3 s) {
    scl=s;
    do_scl=true;
    M_updated=false;
  }

  const Scaling_Matrix4 & sel_S() {
    S.mod(scl);
    return S;
  }

  void get_S(Scaling_Matrix4 & S) {
    S.mod(scl);
  }

  void scale(Vector3 s) {
    scl.mod_x(scl.sel_x() * s.sel_x());
    scl.mod_y(scl.sel_y() * s.sel_y());
    scl.mod_z(scl.sel_z() * s.sel_z());
  }

  // joint

  Vector3 sel_joint() {
    return joint;
  }
  
  void mod_joint(Vector3 j) {
    joint=j;
    M_updated=false;
  }

//   const Translation_Matrix4 & sel_T_j() {
//     update_T_j();
//     return T_j;
//   }

//   void update_T_j() {
//     Vector3 neg_joint; joint.negate(neg_joint);
//     T_j.mod(neg_joint);
//   }

  // translation

  Vector3 sel_pos() {
    return pos;
  }

  void mod_pos(Vector3 p) {
    pos=p;
    M_updated=false;
    M_noS_updated=false;
  }

  const Translation_Matrix4 & sel_T() {
    update_T();
    return T;
  }

  void get_T(Translation_Matrix4 & _T) {
    update_T();
    T = _T;
  }

  void update_T() {
    Vector3 v = pos - joint;
    T.mod(v);
    M_updated=false;
    M_noS_updated=false;
  }

  void translate(const Vector3 & v) {
    pos.add_self(v);
    M_updated=false;
    M_noS_updated=false;
  }

  // rotation

  Angles sel_orn() {
    return orn;
  }

  void mod_orn(Angles o) {
    orn=o;
    do_rot=true;
    R_updated=false;
    M_updated=false;
    M_noS_updated=false;
  }

  const Rotation_Matrix4 & sel_R() {
    update_R();
    return R;
  }

  void get_R(Rotation_Matrix4 & _R) {
    update_R();
    _R = R;
  }

  void update_R() {
    if (!R_updated) {
      R.mod(orn);
      R_updated = true;
    }
  }

  void rotate(Angles a) {
    //orn=orn.multiply(o); // quaternion multiplication
    orn.add(a);
    do_rot=true;
    R_updated=false;
    M_updated=false;
    M_noS_updated=false;
  }


  // composite transformation matrix

  // Transformation order: joint, rotate, scale, translate
  void update_M() {
    S.mod(scl);
    update_T();
    //update_T_j();
    R.mod(orn);
    T.multiply_inplace(S, M_1);
    R.multiply_inplace(M_1, M);
    //R.multiply_inplace(M, M_1);
  }

  Matrix4 & sel_M() {
    update_M();
    return M;
  }

//    // do not consider scaling
//    const TMatrix & getMnoS() {
//      if (!M_noS_updated) {
//        M_noS_updated=true;
//        if (do_R) {
	
//        }
//        else {
	
//        }
//        //M_noS = do_R ? getT() :  getT().multiply(getR());
//      }
//      return MnoS;
//    }

//    TMatrix4 getM() {
//      if (!MUpdated) {
//        MUpdated=true;
//        //M = noS ? getMnoS() : getMnoS().multiply(getS());
//      }
//      return M;
//    }


};

#endif
