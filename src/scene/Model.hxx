//
//
// C++ Interface for module: Model
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Model_Interface
#define Model_Interface

#include "General.hxx"

#include "Light_Source.hxx"
#include "Camera.hxx"
#include "Volume.hxx"
#include "Volume_Coder.hxx"

class Model
{
public:
  Model(Screen & screen)
    : volume(Vector3(1,1,1), 16),
      cam(screen)
  {
    cam.parent.mod_joint( volume.sel_center() );
  }
 
  Model(Screen & screen, int volume_subdivision)
    : volume(Vector3(1,1,1), volume_subdivision),
      cam(screen)
  {
    cam.parent.mod_joint( volume.sel_center() );
  }

  // load volume from file
  Model(Screen & screen, string filename)
    : cam(screen)
  {
    RV_Coder< Raw_Volume<Gray_Voxel> > coder(volume);
    coder.load(filename);
    cam.parent.mod_joint( volume.sel_center() );
    cam.parent.mod_pos( volume.sel_center() - Vector3(0,0,3) );
  }
  
  Light_Source light;
  Camera cam;
  Raw_Volume<Gray_Voxel> volume;
};

#endif
