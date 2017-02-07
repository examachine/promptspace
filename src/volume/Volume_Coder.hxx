//
//
// C++ Interface for module: Volume_Coder
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Volume_Coder_Interface
#define Volume_Coder_Interface

#include "General.hxx"
#include "Volume.hxx"

template < class _Volume >
class RV_Coder
{
public:
  typedef _Volume Volume;
  typedef typename Volume::Voxel Voxel;

  RV_Coder(Volume & _vol) : vol(_vol) {}

  Volume & vol;

  void load(string filename)
  {
    // load raw volume from volpack file
    //      xlen = 128; ylen=128; zlen=64;
    // int header_size = 52;

    nlog << "Loading raw volume from " << filename << endl << flush;

    ifstream rv(filename.c_str());//BUG: , ios::bin) doesn't work;

    if (!rv) {
      nerr << "File not found " << endl << flush;
    }
    
    // read volpack header
    VP_RV_Header header;
    if (!rv.read(reinterpret_cast<void*>(&header), sizeof(header)))
      nerr << "Cannot read header " << endl << flush;
    bool swap_byte_order = false;
    if (!header.magic_correct()) {
      // Either file is not volpack or it's byte order is reverse
      swap_words(&header, sizeof(header));
      if (!header.magic_correct())
	throw runtime_error("File doesn't have volpack raw volume format");
      swap_byte_order = true;
    }

    // read voxel layout information
    // sucky
    char field_data[18]; // ad hoc
    unsigned int size = 3 * header.num_voxel_fields * sizeof(unsigned int);
    rv.read(field_data, size);
    if (swap_byte_order)
      swap_words(field_data, size);
    unsigned int voxel_offset = sizeof(header) + size;


    // load new volume size
    //      vpc->xlen = header.xlen;
    //      vpc->ylen = header.ylen;
    //      vpc->zlen = header.zlen;
    //      vpc->raw_bytes_per_voxel = header.bytes_per_voxel;
    //      vpc->num_voxel_fields = header.num_voxel_fields;
    //      vpc->num_shade_fields = header.num_shade_fields;
    //      vpc->num_clsfy_params = header.num_clsfy_fields;
    //      vpc->xstride = header.xstride;
    //      vpc->ystride = header.ystride;
    //      vpc->zstride = header.zstride;
    //      nvf = header.num_voxel_fields;
    //      for (c = 0; c < nvf; c++) {
    //  	vpc->field_size[c] = field_data[c];
    //  	vpc->field_offset[c] = field_data[nvf + c];
    //  	vpc->field_max[c] = field_data[2*nvf + c];
    //      }


    // initialize volume
    I(header.xlen>0);
    int num_voxels = header.xlen * header.ylen * header.zlen;
    nlog << "Volume dimensions are "
	 << header.xlen << " X "
	 << header.ylen << " X "
	 << header.zlen << endl;

    // subdivision
    // as default compress x ais into (0,1)
    double subdivision = header.xlen;

    // load volume data
    typename Volume::Input_Iterator slices(vol, header.xlen, header.ylen,
				  header.zlen, subdivision);
    for (; !slices.end(); slices.inc()) {
      typedef typename Volume::Input_Iterator::Slice Slice;
      typename Slice::Input_Iterator lines(*slices);
      for (; !lines.end(); lines.inc()) {
	typedef typename Slice::Input_Iterator::Line Line;
	typename Line::Input_Iterator points(*lines);
	for (; !points.end(); points.inc()) {
	  Raw_Voxel raw_voxel;
	  rv.read(reinterpret_cast<void*>(&raw_voxel),
		  sizeof(raw_voxel));
	  double density = double(raw_voxel.density)/255;
	  Voxel voxel(density,
		      1 - density/2);
	  points.load(voxel);
	}
      }
    }

    //cout << " done." << endl << flush;
    nlog << num_voxels << " voxels read. " << endl << flush;

  }

private:

  struct VP_RV_Header {
    static const int magic_constant = 0x4F4F7F7F;
    bool magic_correct() {
      return magic == magic_constant;
    }
    unsigned int magic;		        // magic number for identification
    unsigned int xlen;		       // voxels in each dimension
    unsigned int ylen;
    unsigned int zlen;
    unsigned int bytes_per_voxel;     // size of a raw voxel
    unsigned int num_voxel_fields;   // number of fields in a voxel
    unsigned int num_shade_fields;  // number of fields for shading
    unsigned int num_clsfy_fields; // number of fields for classification 
    int xstride;		  // strides for voxel data
    int ystride;
    int zstride;
  };

  struct Raw_Voxel {		// contents of a voxel
    short normal;		//   encoded surface normal vector
    unsigned char density;	//   original density
    unsigned char gradient;	//   original gradient
  };

  static void swap_words(void *data, unsigned int size)
  {
    unsigned char *ptr;
    int tmp1, tmp2;
    
    ptr = (unsigned char *)data;
    while (size >= 4) {
      tmp1 = ptr[0]; ptr[0] = ptr[3]; ptr[3] = tmp1;
      tmp2 = ptr[1]; ptr[1] = ptr[2]; ptr[2] = tmp2;
      size -= 4;
      ptr += 4;
    }
  }

};


#endif
