//
//
// C++ Interface for module: Slice_Visualizer
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Slice_Visualizer_Interface
#define Slice_Visualizer_Interface

#include "General.hxx"
#include "Volume_Visualizer.hxx"

class Slice_Visualizer : public Volume_Visualizer
{
public:
  Slice_Visualizer(Model &m, bool f)
    : Volume_Visualizer(m,f), slice(40) {}

  int slice;

  void render() {
    nlog << "Slice: render" << endl;

    clock_t start_clock = clock();

    Camera cam_p = cam.transform();
    Vector3 tl = cam_p.sel_screen_tl();
    Vector3 tr = cam_p.sel_screen_tr();
    Vector3 bl = cam_p.sel_screen_bl();
    Vector3 dx = (tr - tl) / double(cam_p.sel_viewing_width()) ;
    Vector3 dy = (bl - tl) / double(cam_p.sel_viewing_height()) ;
    int y = 0;
    for ( Vector3 span_y = tl;
	  y < cam_p.sel_viewing_height();
	  span_y += dy, y++) {
      int x = 0;
      for ( Vector3 span_x = span_y;
	    x < cam_p.sel_viewing_width();
	    span_x += dx, x++) {
	// cast ray from x,y
	cam_p.ray.start_pos = span_x;
	Ray & ray_p = cam_p.ray;
	Vector2 intersection = vol.shoot_boundary(ray_p);
	// does the ray intersect with the volume?
	if ( intersection.comp(0) != 0 && intersection.comp(1) != 0) {
	  double length = intersection.comp(1) - intersection.comp(0);
	  Vector3 point = ray_p.shoot(intersection.comp(0));
	  Vector3 diff = ray_p.direction;
	  diff *= length / vol.sel_subdivision() * slice;
	  point += diff;
	  // point represents slice k which is parallel to viewing plane
	  
	  Gray_Voxel voxel;
	  if ( vol.in_bounds(point)) {
	    Gray_Voxel sample;
	    vol.sel_voxel(point, sample);
	    //cerr << point << endl;
	    //vol.sel_voxel_no_resampling(point, sample);
	    // if (!sample.transparent()) { // transparent voxel omission
	    shader.shade(sample.color, vol.attribute,
			 vol.sel_normal(point), light, ray_p.direction);
	    voxel.color = sample.color;
	  }
	  plot(x,y,voxel.color);
	} // plot
      } // x interpolation
      cout << '.' << flush;
    } // y interpolation
    cout << "[ " << double(clock() - start_clock) / CLOCKS_PER_SEC
	 << " sec ]" << endl;

    slice = (slice + 1) % 80;
  }

  // test raster subsystem by drawing a gray gradient band.
  void test_raster() {
    Camera cam_p = cam.transform();
    int y = 0;
    for ( ;
	  y==0 && y < cam_p.sel_viewing_height()/20;
	  y++) {
      int x = 0;
      for ( ;
	    x < cam_p.sel_viewing_width();
	    x++) {
	plot(x,y,Gray_Color(float(x)/256));
      }
    }
  }

  void update_view()
  {
    clear();
    render();
    //test_raster();
    display_fb();
  }
  
};


#endif
