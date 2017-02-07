//
//
// C++ Implementation for module: Test
//
// Description: 
//
//
// Author: exa
//
//

#include "Numerical_Vector.hxx"
#include "Matrix.hxx"
#include "TMatrix.hxx"
#include "Derived_Vectors.hxx"

void Numerical_Vector_test()
{
  int vec_data[] = {4,6,9,13,14};
  Num_Vector<int,5> v5(vec_data);
  cout << " Num_Vector<int,5> v5 = " << v5 << endl;

  // Vector3
  Vector3 a(2.3, 4.1, 7.2);

  cout << "3d vec a: " << a << endl;
  a.negate_self();
  cout << "negation of a: " << a << endl;
  a *= 2;
  cout << "2*a = " << a << endl;
  cout << "length of a = " << a.length() << endl;

  Vector3::Number a_num = 10;
  cout << "num with type vec3d::Number = " << a_num << endl;
  cout << "a's dimension = " << a.sel_dimension() << endl;

  // Vector2
  Vector2 b(3,2);
  cout << "2d vec b = " << b << endl;

}

void Matrix_test()
{
  // Vector3
  Vector3 a(2.3, 4.1, 7.2);

  // CODE: vecs of different Number types must work
  //       as long as Number types themselves are compatible
  cout << "Identity Matrix with size 3" << endl;
  Identity_Matrix<int,3> I;
  cout << I;

  cout << "a " << " transformed by identity matrix " << endl;
  Num_Vector<double,3> shit = multiply_gen( a , I);
  Vector3 & aa = (Vector3&) shit;
  //CODE: Some problem with templates or subclasses? Revisit!
  Vector3 & a_prime = static_cast< Vector3& > ( multiply_gen( a ,
							  I) );
  Vector3 & a_prime2 = static_cast< Vector3& > ( multiply_gen( I ,
							   a_prime) );
  //Vector3 & a_x = multiply( a , I) ;
  cout << a_prime2 << endl;
}
