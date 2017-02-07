//
//
// C++ Implementation for module: Prism
//
// Description: 
//
//
// Author: exa
//
//

#include "Prism.hxx"
#include "Derived_Vectors.hxx"

double Prism::volume()
{
  return ( length_boundary(sel_x_bnd()) *
	   length_boundary(sel_y_bnd()) *
	   length_boundary(sel_z_bnd()) );
}

bool Prism::on_boundary(Vector3 point)
{
  //cerr << point;
  return   ( on_boundary( point.sel_x(), sel_x_bnd()) &&
	     in_boundary( Vector2(point.sel_y(), point.sel_z()),
			  sel_y_bnd(), sel_z_bnd()) )
         ||( on_boundary( point.sel_y(), sel_y_bnd()) &&
	     in_boundary( Vector2(point.sel_x(), point.sel_z()),
			  sel_x_bnd(), sel_z_bnd()) )
         ||( on_boundary( point.sel_z(), sel_z_bnd()) &&
	     in_boundary( Vector2(point.sel_x(), point.sel_y()),
			  sel_x_bnd(), sel_y_bnd()) );

}

// compute intersection points of a ray with prism boundaries
// returns (0,0) if no intersection detected
Vector2 Prism::shoot_boundary(Ray ray)
{
  vector<double> shot_distances(6);
  shot_distances[0] = shoot_boundary(ray.start_pos.sel_x(),
				     ray.direction.sel_x(),
				     sel_x_bnd().comp(0));
  shot_distances[1] = shoot_boundary(ray.start_pos.sel_x(),
				     ray.direction.sel_x(),
				     sel_x_bnd().comp(1));
  shot_distances[2] = shoot_boundary(ray.start_pos.sel_y(),
				     ray.direction.sel_y(),
				     sel_y_bnd().comp(0));
  shot_distances[3] = shoot_boundary(ray.start_pos.sel_y(),
				     ray.direction.sel_y(),
				     sel_y_bnd().comp(1));
  shot_distances[4] = shoot_boundary(ray.start_pos.sel_z(),
				     ray.direction.sel_z(),
				     sel_z_bnd().comp(0));
  shot_distances[5] = shoot_boundary(ray.start_pos.sel_z(),
				     ray.direction.sel_z(),
				     sel_z_bnd().comp(1));

  vector<double>::iterator i0, i0_end;
  Vector2 dists;
  double max_nz = 0, min_nz = 100000;
  for (i0 = shot_distances.begin(), i0_end = shot_distances.end();
       i0 != i0_end; i0++) {
    double dist = *i0;
    if (dist>0 && !isinf(dist) /*fpclassify(dist)==FP_NORMAL*/ 
	&& in_e_bounds(ray.shoot(dist)) ) {
      if (max_nz < dist)
	max_nz = dist;
      if (min_nz > dist)
	min_nz = dist;
    }
  }

  // front to back order
  if (min_nz < max_nz)
    return Vector2(min_nz, max_nz);
  else
    return Vector2(0,0); // no intersection
}
