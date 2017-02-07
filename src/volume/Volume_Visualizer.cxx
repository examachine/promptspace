//
//
// C++ Implementation for module: Volume_Visualizer
//
// Description: 
//
//
// Author: exa
//
//

#include "Volume_Visualizer.hxx"
#include "Derived_Vectors.hxx"
#include "TMatrix.hxx"

Volume_Visualizer::Volume_Visualizer(Model & m, bool f)
  : model(m), T_v(model.cam, static_cast<Screen&>(model.cam)),
    cam(m.cam), vol(m.volume), light(m.light), fast(f) {
  image_i.init(vol.sel_subdivision()*1, vol.sel_subdivision()*1);
  init();
}

void Volume_Visualizer::init()
{
  // initialize drawing area
  drawing_area.size(model.cam.sel_viewing_width(),
		    model.cam.sel_viewing_height());
  drawing_area.show();

  //make_test_volume();
}

void Volume_Visualizer::update_view()
{
  if (fast) {
    clear();
    render_fast();
  }
  else {
    clear();
    render();
  }
  display_fb();
}

void Volume_Visualizer::transform()
{
}

void Volume_Visualizer::render()
{
  nlog << "Volume_Visualizer: render" << endl;

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
      if ( intersection.comp(0) != 0 && intersection.comp(1) != 0) {
	double length = intersection.comp(1) - intersection.comp(0);
	Vector3 point = ray_p.shoot(intersection.comp(0));
	Vector3 diff = ray_p.direction;
	diff *= length / vol.sel_subdivision();

	// CODE: correct floating point errors in code, check rounding funs
	// make sure we're in bounds
	int num_samples = int (length * vol.sel_subdivision());
//  	Vector3 back_ints = diff;
//  	back_ints *= num_samples-1; back_ints += point;
//  	for ( ; !vol.in_bounds(back_ints); back_ints-= diff, num_samples--);

	Gray_Voxel voxel = Gray_Voxel::blank();
	// early ray termination
	for ( int i = 0; !voxel.opaque()  && i < num_samples;
	      i++, point += diff) {
	  if (!vol.in_bounds(point)) {
// 	    cerr << "out of bounds at img:(" << x << ","<< y
// 		 << ") depth: " << i << " point: " << point << endl;
	    continue;
	  }
	  Gray_Voxel sample;
	  vol.sel_voxel(point, sample);
	  if (!sample.transparent()) { // transparent voxel omission
	    sample.color =
	      shader.shade(sample.color, vol.attribute,
			   vol.sel_normal(point), light,
			   ray_p.direction);
  	    voxel.over(sample);
	  }
	}
	plot(x,y,voxel.color);
      }
    }
    cout << '.' << flush;
  }
  cout << "[ " << double(clock() - start_clock) / CLOCKS_PER_SEC
       << " sec ]" << endl;
}

void Volume_Visualizer::render_fast()
{
  nlog << "Volume_Visualizer: render_fast" << endl;

  clock_t start_clock = clock();

  Matrix4 & M = T_v.sel_V();
  
  // factor the viewing transformation into shear and warp2d matrices

  Vector2 shear( (M.elt(1,1)*M.elt(0,2) - M.elt(0,1)*M.elt(1,2)) /
		 (M.elt(0,0)*M.elt(1,1) - M.elt(1,0)*M.elt(0,1)),
		 (M.elt(0,0)*M.elt(1,2) - M.elt(1,0)*M.elt(0,2)) /
		 (M.elt(0,0)*M.elt(1,1) - M.elt(1,0)*M.elt(0,1)) );

  Shear_Matrix4 M_shear(shear);

  // direction +z
  // compose x-y slices onto intermediate image

  // compute translation to pin intermediate image's coordinates to
  // top left of sheared object space

  double t_i, t_j;

  if ( shear.sel_x() >= 0 )
    t_i = 0;
  else
    t_i = -shear.sel_x() * (vol.sel_len_z()-1);

  if ( shear.sel_y() >= 0 )
    t_j = 0;
  else
    t_j = -shear.sel_y() * (vol.sel_len_z()-1);

  Matrix4 M_warp;
  M_warp.load_identity();
  M_warp.mod_element(0, 0, M.elt(0,0));
  M_warp.mod_element(0, 1, M.elt(0,1));
  M_warp.mod_element(0, 2, M.elt(0,3) - t_i* M.elt(0,0) - t_j *M.elt(0,1));
  M_warp.mod_element(1, 0, M.elt(1,0));
  M_warp.mod_element(1, 1, M.elt(0,0));
  M_warp.mod_element(1, 2, M.elt(1,3) - t_i* M.elt(1,0) - t_j *M.elt(1,1));

  // CODE: compute it!

  image_i.init_image(Gray_Voxel::blank());

  Camera cam_p = cam.transform();
  Ray & ray_p = cam_p.ray;

  // compose intermediate image
  for (int z_i = 0; z_i < vol.sel_len_z(); z_i++) {
    for (int y_i = 0; y_i < image_i.sel_height(); y_i++)
      for (int x_i = 0; x_i < image_i.sel_width(); x_i++) {
	//image_i.mod_sample(x_i,y_i,Gray_Voxel(Gray_Color(0.5), 0));
	double x_o = x_i + z_i * shear.sel_x() + t_i * vol.sel_subdivision();
	double y_o = y_i + z_i * shear.sel_y() + t_j * vol.sel_subdivision();
	int x_of = int(x_o);
	int y_of = int(y_o);
	//if (! vol.in_bounds(Vector3(x_of, y_of, z_i)) )
	//continue;
	Gray_Voxel sample =
	  vol.sel_voxel( x_of, y_of, z_i );
	//if (! image_i.in_bounds(x_i,y_i) )
	//  continue;
	Gray_Voxel & image_sample = image_i.ref_sample(x_i, y_i);
  	if (!image_sample.opaque() && !sample.transparent()) {
    	  sample.color =
    	    shader.shade(sample.color, vol.attribute,
			 vol.sel_normal(x_of, y_of, z_i),
			 light, ray_p.direction);
	  image_sample.over(sample);
  	}
      }
    cout << '.' << flush;
  }

  M_warp.mod_element(0,2,0);
//    cerr << "si = " << shear.sel_x() << ", sj = " << shear.sel_y() << endl;
//    cerr << "ti = " << t_i << ", tj = " << t_j << endl;
//    cerr << M_warp << endl;

  // warp the intermediate image into final image
  for (int y_i = 0; y_i < image_i.sel_height(); y_i++)
    for (int x_i = 0; x_i < image_i.sel_width(); x_i++) {
      // object coordinates
      double x = double(x_i) / vol.sel_subdivision(); 
      double y = double(y_i) / vol.sel_subdivision();
      int x_f = int(M_warp.elt(0,0) * x + M_warp.elt(0,1) * y +
		    M_warp.elt(0,2));
      int y_f = int(M_warp.elt(1,0) * x + M_warp.elt(1,1) * y +
		    M_warp.elt(1,2));
      //x_f -= shear.sel_x() * vol.sel_subdivision();
      //y_f -= shear.y() * vol.sel_subdivision();
      if ( (0 <= x_f) && (x_f < cam.sel_viewing_width()) && 
	   (0 <= y_f) && (y_f < cam.sel_viewing_height()) ) {
//  	cerr << cam.sel_viewing_width() << ":" <<
//  	  cam.sel_viewing_height() << endl;
//  	cerr << x_f << ":" << y_f << endl;
	plot(x_f, y_f, image_i.sel_sample(x_i,y_i).color);
      }
    }

//    for (int v_y = 0; v_y <cam.sel_viewing_height(); v_y++)
//      for (int v_x = 0; v_x < cam.sel_viewing_width(); v_x++) {
//        int x = v_x;
//        int y = v_y;
//        if ( image_i.in_bounds(x,y) )
//  	plot(v_x, v_y, image_i.sel_sample(x,y).color);
//      }
  cout << '.' << flush;

  cout << "[ " << double(clock() - start_clock) / CLOCKS_PER_SEC
       << " sec ]" << endl;
}

void Volume_Visualizer::make_test_volume()
{
  cerr << " Generating test volume..." ;
  for (int x = 0; x < 30; x++)
    for (int y = 0; y < 32; y++)
      for (int z = 0; z < 45; z++) {
	Gray_Voxel voxel(1, 0);
// 	Gray_Voxel voxel(0.8*double(y+1)/31, 
// 				       //0.8*(double(x+1))/16,
// 				       //1 );
// 				       (double(x+1))/30 );
	vol.mod_voxel(x,y,z, voxel );
	//cerr << "[" << voxel.color << ":" << voxel.alpha <<  "]," ;
      }

//    for (int x = 0; x < 6; x++)
//      for (int y = 0; y < 6; y++)
//        for (int z = 0; z < 6; z++) {
//  	Gray_Voxel voxel =  vol.sel_voxel(x,y,z);
//  	//cerr << "[" << voxel.color << ":" << voxel.alpha <<  "]," ;
//        }
//    cerr << endl;
  cerr << " Test volume generated" << endl;

//    for (int x = 10; x < 20; x++)
//      for (int y = 10; y < 20; y++)
//        for (int z = 10; z < 20; z++) {
//  	Gray_Voxel voxel =  Gray_Voxel(0.7, (double(x)-10)/10 );
//  	vol.mod_voxel(x,y,z, voxel );
//  	//cerr << "[" << voxel.color << ":" << voxel.alpha <<  "]," ;
//        }

//    for (int x = 7; x < 23; x++)
//      for (int y = 7; y < 23; y++)
//        for (int z = 7; z < 23; z++) {
//  	Gray_Voxel voxel =  vol.sel_voxel(x,y,z);
//  	cerr << "[" << voxel.color << ":" << voxel.alpha <<  "]," ;
//        }

}

//    Matrix4 & M = T_v.sel_V();
//    Transformation T;
//    T.rotate(a);
//    Vector4 v;
//    multiply(T.sel_M(), v_0, v);
  
//    Vector4 v_p;
//    multiply(M, v, v_p);
//    plot(v_p.x(), v_p.y(), 1, 0, 0);
//  a.add(Angles(M_PI/20, -M_PI/49, M_PI/40));




