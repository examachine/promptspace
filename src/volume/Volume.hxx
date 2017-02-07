//
//
// C++ Interface for module: Volume
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Volume_Interface
#define Volume_Interface

#include "General.hxx"
#include "Numerical_Vector.hxx"
#include "Voxel.hxx"
#include "Prism.hxx"

struct Encoded_Normal
{
  Encoded_Normal() {}
  
  Encoded_Normal(Vector3 n) {
    // length is 1...

    alpha = fix(asin(n.x()));
    alpha = fix(atan(n.x()));
  }

//   operator Vector3() {
//     Vector3 n;
//     n.mod_x(unfix(alpha
//   }

  char alpha;
  char beta;

  // x is in ( -pi/2, pi/2 )
  char fix(double x) {
    return char(x * 256 / M_PI);
  }

  double unfix(char x) {
    return double(x) * M_PI / 256;
  }

};


template<typename _Voxel> class Raw_Line_Ptr;
template<typename _Voxel> class Raw_Slice_Ptr;

// Volume data sampled over a regular grid
// The boundaries are indicated by a parametric prism
// A rectangular volume

template <class _Voxel>
class Raw_Volume : public Prism, public Component
{
public:

  typedef _Voxel Voxel;
  typedef Raw_Volume<Voxel> Volume;
  typedef Volume Self;

  class Input_Iterator;

  friend class Input_Iterator;

  friend class Raw_Line_Ptr<Voxel>;
  friend class Raw_Slice_Ptr<Voxel>;

  // typedef Raw_Slice_Ptr<Voxel> Slice;
  // Slice slice(int z);

  //static Vector3 default_boundaries;

//    Raw_Volume( Vector3 boundaries = default_boundaries,
//  	      Voxel voxel = default_voxel ) {
//    }

  Raw_Volume()
  {
    samples = NULL; lut_y = lut_z = NULL;
  }

  Raw_Volume( Vector3 boundaries) : Prism(boundaries)
  {
    samples = NULL; lut_y = lut_z = NULL;
  }

  Raw_Volume( Vector3 boundaries, int _subdivision)
    : Prism(boundaries)  {
    samples = NULL; lut_y = lut_z = NULL;
    init(_subdivision);
  }

  void init(int xlen, int ylen, int zlen, int _subdivision) {
    subdivision = _subdivision;
    I(subdivision > 1);
    Vector3 bounds(xlen,ylen,zlen);
    bounds /= subdivision;
    init_boundaries(bounds);
    compute_num_samples(xlen,ylen,zlen);
    alloc_volume();
    init_luts();
    init_volume(Voxel::blank());
  }

  void init(Vector3 bounds, int _subdivision) {
    init_boundaries(bounds);
    init(_subdivision);
  }

  void init(int _subdivision) {
    subdivision = _subdivision;
    I(subdivision > 1);
    compute_num_samples();
    alloc_volume();
    init_luts();
    init_volume(Voxel::blank());
  }

  ~Raw_Volume() {
    I(samples && lut_y && lut_z && normals);
    delete samples;
    delete lut_y;
    delete lut_z;
    delete normals;
  }

  void compute_num_samples( int x, int y, int z  ) {
    len_x = x + 1; len_y = y + 1; len_z = z + 1;
    num_samples = len_x * len_y * len_z;
  }

  void compute_num_samples() {
    I( volume() > 0 && subdivision >0 );
    // okay, alloc a bit more so that mid_ptr won't ceash
    len_x = int( length_boundary(sel_x_bnd()) * (subdivision) ) + 1;
    len_y = int( length_boundary(sel_y_bnd()) * (subdivision) ) + 1;
    len_z = int( length_boundary(sel_z_bnd()) * (subdivision) ) + 1;
    num_samples = len_x * len_y * len_z;
  }

  int sel_num_samples() const { return num_samples; }

  // allocate voxel data
  void alloc_volume() {
    I(sizeof(Voxel)==( sizeof(Voxel)/ sizeof(char) ));
    I(sel_num_samples() > 0);
    if (samples) {
      I(samples && normals);
      delete samples;
      delete normals;
    }
    samples = new char[  sel_num_samples() * sizeof(Voxel) ];
    normals = new char[  sel_num_samples() * sizeof(Vector3) ];
  }

  // compute lookup tables for indexing
  void init_luts()
  {
    if (lut_y) {
      I(lut_z);
      delete lut_y;
      delete lut_z;
    }
    lut_y = new int[ len_y ];
    lut_z = new int[ len_z ];
    int index = 0;
    for (int i = 0; i < len_y ; i ++) {
      lut_y[i] = index;
      index += len_x;
    }
    index = 0;
    for (int i = 0; i < len_z ; i ++) {
      lut_z[i] = index;
      index += len_x * len_y;
    }
  }

  void init_volume(Voxel voxel) {
    Vector3 default_normal(0,0,-1);
    for (int i = 0; i < sel_num_samples(); i++) {
      mod_voxel(i, voxel);
      mod_normal(i, default_normal);
    }
  }

  // resample a point in volume
  Voxel sel_voxel(Vector3 &  pnt) {
    Vector3 v;
    sel_voxel(pnt, v);
    return v;
  }


  void sel_voxel(Vector3 point, Voxel & v) {

    point *= subdivision;

    // okay get the neighboring 8 samples
    double x0 = floor(point.sel_x());
    int x0_i = int(x0);
    double y0 = floor(point.sel_y());
    int y0_i = int(y0);
    double z0 = floor(point.sel_z());
    int z0_i = int(z0);

    double dx = point.x() - x0;
    double dy = point.y() - y0;
    double dz = point.z() - z0;

    Voxel smp_0 = sel_voxel(x0_i,   y0_i,   z0_i);

    Voxel smp_1 = sel_voxel(x0_i,   y0_i,   z0_i+1);

    Voxel smp_2 = sel_voxel(x0_i,   y0_i+1, z0_i);

    Voxel smp_3 = sel_voxel(x0_i,   y0_i+1, z0_i+1);

    Voxel smp_4 = sel_voxel(x0_i+1, y0_i,   z0_i);

    Voxel smp_5 = sel_voxel(x0_i+1, y0_i,   z0_i+1);

    Voxel smp_6 = sel_voxel(x0_i+1, y0_i+1, z0_i);

    Voxel smp_7 = sel_voxel(x0_i+1, y0_i+1, z0_i+1);

    // bilinear filter

    Voxel v_left, v_right;
    v_left = ( smp_3*dz + smp_2*(1-dz) ) * dy +
           ( smp_1*dz + smp_0*(1-dz) ) * (1-dy);
    v_right = ( smp_7*dz + smp_6*(1-dz) ) * dy +
           ( smp_5*dz + smp_4*(1-dz) ) * (1-dy);
    v = v_right*dx + v_left*(1-dx); 

  }


  void sel_voxel_no_resampling(Vector3 point, Voxel & v) {

    Vector3 pnt = point; pnt *= subdivision;

    // okay get the nearest 8 samples
    double x0 = floor(pnt.sel_x());
    int x0_i = int(x0);
    double y0 = floor(pnt.sel_y());
    int y0_i = int(y0);
    double z0 = floor(pnt.sel_z());
    int z0_i = int(z0);

    v = sel_voxel(x0_i, y0_i, z0_i);
  }

  void compute_normals() {
    for (int z=1; z<len_z-1; z++)
      for (int y=1; y<len_y-1; y++)
	for (int x=1; x<len_x-1; x++) {
	  Vector3 gradient ( sel_voxel(x+1, y,   z  ).sel_density(),
			     sel_voxel(x,   y+1, z  ).sel_density(),
			     sel_voxel(x,   y,   z+1).sel_density() );
	  gradient.sub_self(Vector3 ( sel_voxel(x-1, y,   z  ).sel_density(),
				      sel_voxel(x,   y-1, z  ).sel_density(),
				      sel_voxel(x,   y,   z-1).sel_density() ) );
	  gradient.normalize_self();
	  mod_normal(x,y,z,gradient);
	}
    
  }

//    void init_origin_sample() {
//      mid_ptr = ( x
//    }

  // voxel sel/mod

  Voxel *sample_ptr(int x, int y, int z) const {
    I( 0 <= x <= subdivision &&
       0 <= y <= subdivision &&
       0 <= z <= subdivision );
    return reinterpret_cast<Voxel *>
      (&samples[ (x + lut_y[y] + lut_z[z]) * sizeof(Voxel) ]);
  }

  Voxel *sample_ptr(int i) const {
    I( 0 <= i <= subdivision );
    return reinterpret_cast<Voxel *>
      (&samples[ i * sizeof(Voxel) ]);
  }

  Voxel sel_voxel(IVector3 vec) const {
    return *sample_ptr(vec.x, vec.y, vec.z);
  }

  //CODE: maybe reference works faster
  Voxel sel_voxel(int x, int y, int z) const {
    return *sample_ptr(x, y, z);
    //    return samples[x + lut_y[y] + lut_z[z] ];
  }

  void mod_voxel(IVector3 vec, const Voxel & v) {
    *sample_ptr(vec.x, vec.y, vec.z) = v;
  }

  void mod_voxel(int x, int y, int z, const Voxel & v) {
    *sample_ptr(x,y,z) = v;
  }

  void mod_voxel(int i, const Voxel & v) {
    *sample_ptr(i) = v;
  }

  // normal sel/mod

  Vector3  sel_normal(Vector3 point) {
    Vector3 n;

    point *= subdivision;

    // okay get the neighboring 8 samples
    double x0 = floor(point.sel_x());
    int x0_i = int(x0);
    double y0 = floor(point.sel_y());
    int y0_i = int(y0);
    double z0 = floor(point.sel_z());
    int z0_i = int(z0);

    double dx = point.x() - x0;
    double dy = point.y() - y0;
    double dz = point.z() - z0;

    Vector3 smp_0 = sel_normal(x0_i,   y0_i,   z0_i);

    Vector3 smp_1 = sel_normal(x0_i,   y0_i,   z0_i+1);

    Vector3 smp_2 = sel_normal(x0_i,   y0_i+1, z0_i);

    Vector3 smp_3 = sel_normal(x0_i,   y0_i+1, z0_i+1);

    Vector3 smp_4 = sel_normal(x0_i+1, y0_i,   z0_i);

    Vector3 smp_5 = sel_normal(x0_i+1, y0_i,   z0_i+1);

    Vector3 smp_6 = sel_normal(x0_i+1, y0_i+1, z0_i);

    Vector3 smp_7 = sel_normal(x0_i+1, y0_i+1, z0_i+1);

    // bilinear filter

    Vector3 n_left, n_right;
    n_left = (1-dy) * ( (1-dz)*smp_0 + dz*smp_1 ) +
             dy     * ( (1-dz)*smp_2 + dz*smp_3 ) ;
    n_right = (1-dy) * ( (1-dz)*smp_4 + dz*smp_5 ) +
              dy     * ( (1-dz)*smp_6 + dz*smp_7 );
    n = (1-dx)*n_left + dx*n_right; 

    // The combination is a unit vector
    //n.normalize_self();

    //cerr << dx << "," << dy << "," << dz << ":" << n << endl;
    return n;
  }

  Vector3 *normal_ptr(int x, int y, int z) const {
    I( 0 <= x <= subdivision &&
       0 <= y <= subdivision &&
       0 <= z <= subdivision );
    return reinterpret_cast<Vector3 *>
      (&normals[ (x + lut_y[y] + lut_z[z]) * sizeof(Vector3) ]);
  }

  Vector3 *normal_ptr(int i) const {
    I( 0 <= i <= subdivision );
    return reinterpret_cast<Vector3 *>
      (&normals[ i * sizeof(Vector3) ]);
  }

  Vector3 & sel_normal(int x, int y, int z) const {
    return *normal_ptr(x, y, z);
  }

  void mod_normal(int x, int y, int z, const Vector3 & n) {
    *normal_ptr(x,y,z) = n;
  }

  void mod_normal(int i, const Vector3 & v) {
    *normal_ptr(i) = v;
  }

  int sel_subdivision() const { return subdivision; }
  int sel_len_x() const { return len_x; }
  int sel_len_y() const { return len_y; }
  int sel_len_z() const { return len_z; }

private:
  int subdivision;
  int num_samples;
  int len_x, len_y, len_z;

  char *samples;		// Voxel *samples; indeed
  char *normals;		// Vector3 *normals

  //char *origin_sample;

  int *lut_y;
  int *lut_z;
};


// A line across the last two axises axis
template <typename _Voxel>
class Raw_Line_Ptr
{
public:

  typedef _Voxel Voxel;
  typedef Raw_Line_Ptr<Voxel> Line;
  typedef Raw_Slice_Ptr<Voxel> Slice;
  //  typedef Slice Self;

  class Input_Iterator;
  friend class Input_Iterator;

  Raw_Line_Ptr(const Slice & s, int _y) : slice(s), y(_y) {
  }

  int sel_y() const {
    return y;
  }

protected:
  Slice slice;
  int y;
};

template <typename _Voxel>
class Raw_Line_Ptr<_Voxel>::Input_Iterator
{
public:

  Input_Iterator(const Line & _line) : line(_line) {
    x = 0;
    end_x = line.slice.vol.sel_len_x() - 1;
  }

  int sel_x() const {
    return x;
  }

  void inc() {
    x++;
  }

  void load(Voxel & voxel) {
    line.slice.vol.mod_voxel(IVector3(sel_x(), line.sel_y(),
				      line.slice.sel_z()),
			     voxel);
  }

  bool end() {
    bool f;
    if  (f = (x == end_x))
      ;
    return f;
  }

private:
  Line line;
  int  x;
  int end_x;
};


// A slice across the last axis
template <typename _Voxel>
class Raw_Slice_Ptr
{
public:

  typedef _Voxel Voxel;
  typedef Raw_Line_Ptr<Voxel> Line;
  typedef Raw_Slice_Ptr<Voxel> Slice;
  typedef Raw_Volume<Voxel> Volume;
  //  typedef Slice Self;

  class Input_Iterator;
  friend class Input_Iterator;
  friend class Line;

  Raw_Slice_Ptr(Volume &v, int _z) : vol(v), z(_z) {
    
  }

  int sel_z() const {
    return z;
  }

protected:
  Volume &vol;
  int z;
};

template <typename _Voxel>
class Raw_Slice_Ptr<_Voxel>::Input_Iterator
{
public:

  typedef Raw_Line_Ptr<Voxel> Line;

  Input_Iterator(const Slice & _slice) : slice(_slice) {
    y = 0;
    end_y = slice.vol.sel_len_y() - 1;
  }

  int sel_y() const {
    return y;
  }

  void inc() {
    y++;
  }

  Line operator *() {
    return Line(slice, sel_y());
  }

  bool end() {
    bool f;
    if  (f = (y == end_y))
      ;
    return f;
  }

private:
  Slice slice;
  int  y;
  int end_y;
};


template <typename _Voxel>
class Raw_Volume<_Voxel>::Input_Iterator
{
public:

  typedef Raw_Slice_Ptr<Voxel> Slice;
  typedef Raw_Line_Ptr<Voxel> Line;

  friend class Slice;
  friend class Line;

  Input_Iterator(Volume & v, int _len_x, int _len_y, int _len_z,
		 int subdivision) : vol(v) {
    vol.init(_len_x, _len_y, _len_z, subdivision);
    z = 0;
    end_z = vol.len_z - 1;
  }

  int sel_z() const {
    return z;
  }

  void inc() {
    z++;
  }

  Slice operator *() {
    return Slice(vol, z);
  }

  bool end() {
    bool f;
    if  (f = (z == end_z))
      vol.compute_normals();
    return f;
  }

private:
  Volume & vol;
  int z;
  int end_z;
};



#endif
