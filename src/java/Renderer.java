package exa.modeller.threed;

import exa.modeller.math.*;
import exa.modeller.util.*;
import java.awt.*;
import java.awt.image.*;
import java.util.*;

public abstract class Renderer extends Canvas {

  World      w;       // the world that is going to be rendered
  Camera     cam;     // seeing thru
  Polygonal  obj;     // current object
  ViewT      vT;      // the primary viewing transformation
  Vector     visible; // list of visible objects
  Vector     group;   // current group of objects

  int SCR_WIDTH, SCR_HEIGHT, SCR_SIZE, midOffset;
  int MID_WIDTH, MID_HEIGHT;

  // Frame Buffer
  int[] frameBuffer;

  // z-buffer
  short[] zBuffer;   // z-buffer with 16-bit cells
  
  // Java specific
  Graphics g;         // graphics context
  boolean Rendered;
  Image image;
  MemoryImageSource source;
  boolean customRendering;

  // create a new renderer object for the
  // specified 3d world and dimensions of rendering
  // the computations are made the time renderer was instantiated
  // further updates are via calls to updateView method

  public Renderer(World world, Dimension dim) {

    w = world;

    SCR_WIDTH=(dim.width/2)*2+1;
    SCR_HEIGHT=(dim.height/2)*2+1;
    SCR_SIZE=SCR_WIDTH*SCR_HEIGHT;
    MID_WIDTH=SCR_WIDTH/2;
    MID_HEIGHT=SCR_HEIGHT/2;
    midOffset=(SCR_HEIGHT>>1)*SCR_WIDTH+(SCR_WIDTH>>1);

    // allocate mem for our buffers
    frameBuffer = new int[SCR_SIZE];
    zBuffer = new short[SCR_SIZE];

    setSize(SCR_WIDTH, SCR_HEIGHT);
    setBackground(Color.black);
    setForeground(Color.white);
    visible = new Vector();

    source=new MemoryImageSource(SCR_WIDTH, SCR_HEIGHT,
				 ColorModel.getRGBdefault(), 
				 frameBuffer, 0, SCR_WIDTH);

    updateView();
  }


  public void updateView() {

    Rendered=false;

   // we need to clean up the memory really
    System.gc();
    // cleanse the the frame buffer
    if (customRendering)
      for (int i=0; i<SCR_SIZE; i++)
	frameBuffer[i]=0;
    // and the z-buffer
    for (int i=0; i<SCR_SIZE; i++)
      zBuffer[i]=32767; // initialize it to the biggest possible value...
 
    cam = w.viewCam;         // get the active cam
    
    // fetch the current viewing transformation
    vT = new ViewT(cam, new Dimension(MID_WIDTH, MID_HEIGHT));

    // now rendering...
    // traverse all world components but render only Polygonal components
    for (int i=0; i<w.componentSize(); i++) {
      WorldComponent c=w.getComponent(i);
      if (c instanceof Polygonal) {
	obj=(Polygonal)c;
	if (obj.bounding.isVisible(vT)) {
	  transform();
	  shade();
	  visible.addElement(group);
	}
      }
    }
    repaint();
  }

  // transform object's all vertices and normals
  // also cull the polygons as their normals are transformed
  void transform() {
    group = new Vector();
    transform(obj, vT.getM(), vT.getM(), vT.getV(), vT, group);
  }

  static void transform(Polygonal obj, TMatrix4 M, TMatrix4 MnoS,
			TMatrix4 V, ViewT vT, Vector group) {
    // M for viewing transformatiom
    // MnoS for viewing trans w/o scaling
    // V is for display transformation

    //System.out.println("centre->"+obj.centre);
    //System.out.println("local->"+obj.local);
    //System.out.println("parent->"+obj.parent);

    // compute transformation matrices
    TMatrix4 tmpM;
    tmpM = (obj.parent.getM().multiply(obj.local.getM())).multiply
      (obj.centre.getM());
    M = M.multiply(tmpM);
    V = V.multiply(tmpM);
    MnoS = MnoS.multiply( (obj.parent.getMnoS().multiply(obj.local.getMnoS()) )
			  .multiply(obj.centre.getMnoS()));

    //System.out.println("view T for vertices:"+M);
    //System.out.println("display T for vertices:"+V);
    //System.out.println("view T for polygons:"+MnoS);

    System.out.println("number of vertices "+obj.vertexSize());
    System.out.println("number of polygons "+obj.getSurface(0).polygonSize());

    // traverse Vertices
    for (int i=0; i<obj.vertexSize(); i++) {

      Vertex v = obj.getVertex(i);

      // transform vertex to viewing coordinates
      v.posmodel = M.transform(v.pos);

      // project vertex to screen coordinates
      v.posview = vT.project(V.transform(v.pos));
      v.prj = new Point((int)v.posview.c(0),
			(int)v.posview.c(1));
      v.zVal = (short)v.posview.c(2);

      // transform average normal to viewing coordinates
      v.Nmodel = MnoS.transform(v.N.add(v.pos)).sub(v.posmodel);

      //System.out.println("vertex("+i+"):"+v);
    }

    // traverse polygons
    for (int i=0; i<obj.surfaceSize(); i++) { 
      Surface surface=obj.getSurface(i);
      for (int j=0; j<surface.polygonSize(); j++) {

	Polygon3d polygon=surface.getPolygon(j);

	// transform normals of each polygon
	polygon.Nmodel = MnoS.transform(polygon.N.add(polygon.vertex[0].pos)).
	  sub(polygon.vertex[0].posmodel);

	// cull polygon
	polygon.hidden=( polygon.Nmodel.dot(polygon.vertex[0].posmodel) > 0 );

	//System.out.println("polygon("+j+"):"+polygon.N+polygon.Nmodel);
      }
    }    
   
    group.addElement(obj);

    // recursively transform children
    for (int i=0; i<obj.childSize(); i++) { 
      Polygonal child=obj.getChild(i);
      transform(child, M, MnoS, V, vT, group);
    }

  }


  // shader for the current object
  // to be implemented by concrete subclasses of Renderer
  abstract void shade();

  public void paint(Graphics g) {
    if (!Rendered) {
      // render all visible objects
      this.g=g;
      for (int i=0; i<visible.size(); i++) {
	group=(Vector)visible.elementAt(i);
	draw();
      }
      Rendered=true;
      image=createImage(source);
    }
    g.drawImage(image, 0, 0, Color.black, this);
  }

  // draw all polygons in the current group
  void draw() {
    for (int k=0; k<group.size(); k++) { 
      obj = (Polygonal)group.elementAt(k);
      for (int i=0; i<obj.surfaceSize(); i++) { 
	Surface surface = obj.getSurface(i);
	for (int j=0; j<surface.polygonSize(); j++) {
	  Polygon3d polygon = surface.getPolygon(j);
	  drawPoly(polygon, g);
	}
      }
    }
  }

  // plot a point in the frame buffer
  void plot(Vector3 scr, int c) {
    int x = (int)scr.c(0);
    int y = (int)scr.c(1);
    short z = (short)scr.c(2);
    // in screen coordinates?
    if (Math.abs(y)>MID_HEIGHT || Math.abs(x)>MID_WIDTH)
      return;
    int offset=x+y*SCR_WIDTH+midOffset;
    if (zBuffer[offset]>z) {
      zBuffer[offset]=z;
      frameBuffer[offset]=c;
    }
  }


  // some global variables to keep track of polygon vertices
  // for convex polygons of course...
  // vertices are arranged as a circular array in counter-clockwise order

  int topleft, topright, bottomleft, bottomright, last;

  // find the index of the next vertex
  final int nextV(int v) {
    return v == last ? 0 : v+1;
  }

  // find the index of previous vertex
  final int prevV(int v) {
    return v == 0 ? last : v-1;
  }

  // render a three dimensional polygon
  abstract void drawPoly(Polygon3d polygon, Graphics g);

  // fetch the top vertex
  final void findTopVertices(Polygon3d poly) {

    int       oldy, newy;
    Vertex[]  v;
    int       tl, tr;
    int y;

    // edges are arranged counter-clockwise
    // seek the top vertex (with minimum y)
    v=poly.vertex;
    int last = v.length-1;
    y=SCR_WIDTH; tl=0;
    for (int i=0; i<=last; i++)
      if ( (newy=v[i].prj.y)<y) {
	y=newy;
	tl=i;
      }

    if (v[nextV(tl)].prj.y==y) {
      tr=tl;
      tl=nextV(tl);
    }
    else if (v[prevV(tl)].prj.y==y)
      tr=prevV(tl);
    else
      tr=tl;

    topleft=tl;
    topright=tr;
  }

}



