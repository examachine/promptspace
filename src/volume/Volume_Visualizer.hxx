//
//
// C++ Interface for module: Volume_Visualizer
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Volume_Visualizer_Interface
#define Volume_Visualizer_Interface

#include "General.hxx"
#include "GUI.hxx"
#include "Model.hxx"
#include "Viewing_Transformation.hxx"
#include "Screen.hxx"
#include "Volume.hxx"
#include "Phong_Shader.hxx"
#include "Light_Source.hxx"
#include "Image.hxx"

class Volume_Visualizer
{
public:
  Model &  model;         // the world that is going to be rendered
  Camera & cam;          // seeing thru
  Light_Source & light; // and shading with
  Raw_Volume<Gray_Voxel> & vol;       // current object
  Viewing_Transformation   T_v;      // primary viewing transformation
  Phong_Shader<Gray_Color> shader;
  Raw_Image<Gray_Voxel> image_i; // intermediate image
  bool fast;

  //Screen screen;

  // Output device
  Drawing_Area drawing_area;
  
  // create a new renderer object for the
  // specified 3d world and dimensions of rendering
  // the computations are made the time renderer was instantiated
  // further updates are via calls to updateView method
  
  Volume_Visualizer(Model & m, bool f);

  void init();

  virtual void update_view();

  void transform();

  virtual void render();

  void render_fast();

  //virtual void render() = 0;

  // inline
  void clear() {
    drawing_area.clear();
  }

  void plot( int x, int y, Gray_Color col) {
//     cerr << "plot(" << x << "," << y << "," << col << ")" << endl;
    drawing_area.draw_gray_point(x, y, col);
  }

  // inline
  void plot( int x, int y,  float r, float g, float b) {
    drawing_area.draw_point(x,y,r,g,b);
  }

  void display_fb() {
    drawing_area.update_all();
  }

  void make_test_volume();
};

#endif


