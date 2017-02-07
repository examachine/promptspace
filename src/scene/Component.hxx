//
//
// C++ Interface for module: Component
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Component_Interface
#define Component_Interface

#include "General.hxx"
#include "Transformation.hxx"
#include "Attribute.hxx"

class Component
{
public:
  Component() {}

  Transformation parent;  // transformation in parent's space
  Transformation local;   // transformation in local space, includes
  // articulated modeling

  Attribute attribute;

  const Matrix4 & sel_transformation_mtx() {
    local.sel_M().multiply_inplace(parent.sel_M(), M);
    return M;
  }
  Matrix4 M;

};

#endif
