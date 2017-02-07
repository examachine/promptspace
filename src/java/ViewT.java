package exa.modeller.threed;

import exa.modeller.math.*;
import java.awt.Dimension;

// involves transformation from model coordinates to view coordinates
// first a translation, then a rotation which is preferably followed by
// a projection matrix

class ViewT {

  Camera c;
  Dimension d;

  TranslationMatrix4 T;
  RotationMatrix4 R;
  PerspectiveMatrix4 P;
  double prjConst;
  double zFactor;
  double frontClip;

  TMatrix4 M, V;
  boolean MUpdated;

  ViewT(Camera _c, Dimension _d) {
    c=_c; d=_d;
    MUpdated=false;
    //update();
  }

  void update() {
    if (!MUpdated) {
      T = new TranslationMatrix4(c.parent.getPos().neg());
      R = new RotationMatrix4(c.parent.getOrn().inverse());
      P = new PerspectiveMatrix4(c.fov, d.width*2+1);
      M = R.multiply(T);
      V = P.multiply(M);
      MUpdated=true;
      prjConst=P.getPrjConst();
      zFactor=32767/(c.backClip-c.frontClip);
      frontClip=c.frontClip;
    }
  }

  TMatrix4 getM() {
    update();
    return M;
  }

  TMatrix4 getV() {
    update();
    return V;
  }
  
  double getPrjConst() {
    update();
    return prjConst;
  }

  Vector3 project(Vector3 v) {
    return v.c(2)!=0 ? new Vector3(v.c(0)/v.c(2),
				   -v.c(1)/v.c(2),
				   (v.c(2)-frontClip)*zFactor ) :
      new Vector3(0, 0, 0);
  }

  public String toString() {
    return "pos:" + c.parent.getPos().toString()+ ", rot:" +
      c.local.getOrn().toString() + ", k:" + getPrjConst();
  }

}





