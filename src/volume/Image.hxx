//
//
// C++ Interface for module: Image
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Image_Interface
#define Image_Interface

#include "General.hxx"

template <class _Voxel>
class Raw_Image
{
public:
  typedef _Voxel Voxel;
  typedef Raw_Image<Voxel> Image;
  typedef Image Self;

  Raw_Image(){
    samples = NULL;
    lut_y = NULL;
  }

  Raw_Image(int w, int h){
    init(w, h);
  }

  ~Raw_Image() {
    I(samples && lut_y);
    delete samples;
    delete lut_y;
  }

  void init(int xlen, int ylen) {
    I(xlen > 1 && ylen > 1);
    compute_num_samples(xlen,ylen);
    alloc_image();
    init_luts();
    init_image(Voxel::blank());
  }

  int sel_width() const { return width; }

  int sel_height() const { return height; }

  void compute_num_samples( int x, int y) {
    width = x; height = y;
    num_samples = width * height;
  }

  int sel_num_samples() const { return num_samples; }

  // allocate voxel data
  void alloc_image() {
    I(sizeof(Voxel)==( sizeof(Voxel)/ sizeof(char) ));
    I(sel_num_samples() > 0);
    if (samples)
      delete samples;
    samples = new char[  sel_num_samples() * sizeof(Voxel) ];
  }

  // compute lookup tables for indexing
  void init_luts()
  {
    if (lut_y)
      delete lut_y;
    lut_y = new int[ height ];
    int index = 0;
    for (int i = 0; i < height ; i ++) {
      lut_y[i] = index;
      index += width;
    }
  }

  void init_image(Voxel voxel) {
    for (int i = 0; i < sel_num_samples(); i++) {
      mod_sample(i, voxel);
    }
  }

  Voxel *sample_ptr(int x, int y) const {
    I( 0 <= x < sel_width() &&
       0 <= y < sel_height() );
    return reinterpret_cast<Voxel *>
      (&samples[ (x + lut_y[y]) * sizeof(Voxel) ]);
  }

  Voxel *sample_ptr(int i) const {
    I( 0 <= i < sel_num_samples() );
    return reinterpret_cast<Voxel *>
      (&samples[ i * sizeof(Voxel) ]);
  }

  //CODE: maybe reference works faster
  Voxel sel_sample(int x, int y) const {
    return *sample_ptr(x, y);
  }

  Voxel & ref_sample(int x, int y) const {
    return *sample_ptr(x, y);
  }

  void mod_sample(int x, int y,  const Voxel & v) {
    *sample_ptr(x, y) = v;
  }

  void mod_sample(int i, const Voxel & v) {
    *sample_ptr(i) = v;
  }

  bool in_bounds(int x, int y) {
    return 0 <= x < width && 0 <= y < height;
  }

private:
  
  int width;
  int height;
  int num_samples;

  char *samples;		// Voxel *samples; indeed

  int *lut_y;


};

#endif
