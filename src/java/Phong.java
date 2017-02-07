package exa.modeller.threed;

import exa.modeller.math.*;
import exa.modeller.util.*;
import java.awt.*;

public class Phong extends Renderer {

  public Phong(World world, Dimension d) {
    super(world, d);
    customRendering=true;
  }


  float   ambient, kd, ks;
  int     e;
  Vector3 L;

  // single light source for phong yet...

  // calculate shading parameters of the current object
  // determine hidden vertices also
  void shade() {

  }

  void drawPoly(Polygon3d Poly, Graphics g) {

    if (Poly.hidden) return;

    LightSource light=w.getLight(0);

    L = light.direction.neg();

    // viewed ambient intensity over material is constant
    ambient = obj.attribute.ambient*light.ambient;

    kd=obj.attribute.diffuse;  // material characteristix
    ks=obj.attribute.specular;
    e=obj.attribute.exponent;

    // the color of polygon we're inclined to draw
    int col = obj.attribute.color.getRGB();
    Color c = obj.attribute.color;
    int cr, cg, cb;
    cr=c.getRed();cg=c.getGreen();cb=c.getBlue();

    // vertices to traverse polygon
    Point    TopLeft, TopRight, BottomLeft, BottomRight;

    // interpolation across edges
    int      SpanBeg=0, SpanBegD=0, SpanEnd=0, SpanEndD=0;
    Vector3  NBeg, NBegD, NEnd, NEndD; 
    Vector3  HBeg, HBegD, HEnd, HEndD; 

    NBeg=NBegD=NEnd=NEndD=HBeg=HBegD=HEnd=HEndD=new Vector3(0,0,0); //java 

    // current y position
    int      YPos;

    // flags: which side to iterate next
    boolean  NextLeft, NextRight;

    // array of vertex references (circular array)
    Vertex[] v=Poly.vertex;

    // have to set this (global var)
    last=v.length-1;
    
    // find out top vertex (or vertices)
    findTopVertices(Poly);
    bottomleft=topleft; bottomright=topright; //java

    // assume that polygons do nto intersect!!
    short z=v[0].zVal;

    // get first set of coordinates
    TopLeft=BottomLeft=v[bottomleft].prj;  // java again
    TopRight=BottomRight=v[bottomright].prj;
    YPos=BottomLeft.y;

    // we have fed the bottom coordinates
    // set next flags to true for the full first set of coordinates
    NextLeft = NextRight = true;

    // main loop, executed until the whole polygon is drawn
    // the body renders one trapezoid portion of the polygon
    do {
 
      if (NextLeft) { // left higher
	topleft=bottomleft;
	bottomleft=nextV(bottomleft);
	TopLeft=BottomLeft;
	BottomLeft=v[bottomleft].prj;

	SpanBeg = (TopLeft.x + TopLeft.y*SCR_WIDTH) << 8;
	NBeg = new Vector3(v[topleft].Nmodel);
	HBeg = v[topleft].posmodel.neg();
	HBeg.addv(L);
	if (BottomLeft.y-TopLeft.y > 0) {
	  SpanBegD = ( ((BottomLeft.x-TopLeft.x) << 8) /
		       (BottomLeft.y-TopLeft.y)         
		       + (SCR_WIDTH << 8)                   );
	  NBegD = (Vector3) ( (v[bottomleft].Nmodel.sub(v[topleft].Nmodel))
			      .divide (BottomLeft.y-TopLeft.y)         );         
	  HBegD = (Vector3) ((v[topleft].posmodel.sub(v[bottomleft].posmodel))
			     .divide (BottomLeft.y-TopLeft.y)         );         
	}
      }

      if (NextRight) {
	topright=bottomright;
	bottomright=prevV(bottomright);
	TopRight=BottomRight;
	BottomRight=v[bottomright].prj;

	SpanEnd = (TopRight.x + TopRight.y*SCR_WIDTH) << 8;
	NEnd = new Vector3(v[topright].Nmodel);
	HEnd = v[topright].posmodel.neg();
	HEnd.addv(L);
	if (BottomRight.y-TopRight.y > 0) {
	  SpanEndD = ( ((BottomRight.x-TopRight.x) << 8) /
		       (BottomRight.y-TopRight.y)
		       + (SCR_WIDTH << 8)                    );
	  NEndD = (Vector3) ( (v[bottomright].Nmodel.sub(v[topright].Nmodel))
			      .divide(BottomRight.y-TopRight.y)         );         
	  HEndD = (Vector3) ( (v[topright].posmodel.sub(v[bottomright].posmodel))
			      .divide (BottomRight.y-TopRight.y)         );
	}
      }

      if ( BottomLeft.y<YPos || BottomRight.y<YPos ) {
	break; // terminate loop
      }

      int count; // number of scanlines that will be drawn

      if (BottomLeft.y<=BottomRight.y) { //left higher
	NextRight=(BottomLeft.y==BottomRight.y);
	NextLeft=true;
	count=BottomLeft.y-YPos;
	YPos=BottomLeft.y;
      }
      else {
	NextLeft=false; NextRight=true;
	count=BottomRight.y-YPos;
	YPos=BottomRight.y;
      }
      
      // draw trapezoid
      for (int i=count; i>0; i--) {
	
	span(SpanBeg>>8, SpanEnd>>8,
	     (Vector3) NBeg.normalize(), (Vector3) NEnd.normalize(),
	     (Vector3) HBeg.normalize(), (Vector3) HEnd.normalize(),
	     z, cr, cg, cb);
	SpanBeg += SpanBegD;
	SpanEnd += SpanEndD;
	NBeg.addv(NBegD);
	NEnd.addv(NEndD);
	HBeg.addv(HBegD);
	HEnd.addv(HEndD);
      }
    } while (true);
  }

  final void span(int s, int e, Vector3 N1, Vector3 N2,
		  Vector3 H1, Vector3 H2,
		  short z, int r, int g, int b) {
    // get to the midst of the screen
    s+=midOffset;
    e+=midOffset;

    // calc interpolants
    Vector3 dN = (e-s)==0 ? new Vector3(0,0,0)
      : (Vector3) N2.sub(N1).divide(e-s);
    Vector3 dH = (e-s)==0 ? new Vector3(0,0,0)
      : (Vector3) H2.sub(H1).divide(e-s);

    // stamp pixels
    for (; s <= e; s++) {
      if (s>0 && s<SCR_SIZE && zBuffer[s]>z) {
	zBuffer[s]=z;

	// compute intensity
	Vector3 Nn=(Vector3)N1.normalize();
	Vector3 Hn=(Vector3)H1.normalize();
	double diffuse=Nn.dot(L);  // diffuse component
	diffuse = Math.max(0, diffuse)*kd;

      	double specular=Nn.dot(Hn); // specular component
	specular = Math.pow(Math.max(0, specular)*ks, e);
	float I=(float)Math.min(1, ambient + diffuse + specular);
	frameBuffer[s] = ( (255<<24)+
			   ((int)(r*I)<<16)+
			   ((int)(g*I)<<8)+
			   (int)(b*I) );
      }
      N1.addv(dN);
      H1.addv(dH);
    }
  }

}




