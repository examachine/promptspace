//
//
// C++ Interface for module: Numerical_Vector_Generic
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Numerical_Vector_Generic_Interface
#define Numerical_Vector_Generic_Interface

#include "General.hxx"

// Generic Numerical Vector class
// class Number: type of components, must be closed under arithmetic ops
// int dimension: Vector's dimension

//class Vector3;

// Reference Class
template <class T>
class Ref {
public:
  typedef T C_Object;
  typedef T & C_Reference;
  typedef T * C_Pointer;

  Ref(C_Reference _ref) : ref(_ref) {}
  Ref(C_Pointer _ref) : ref(*_ref) {}
  C_Pointer operator *() { return & ref; }
  C_Pointer operator ->() { return & ref; }
  
private:
  C_Reference ref;
};

// CODE: now how do we define intermediate types for user-def type conversion?

template <class T, int dim>
class Num_Vector_Base {

public:

  typedef T Number;
  static const int dimension = dim;
  //static int dimension;// = _dimension;
  typedef Num_Vector_Base<Number, dimension> Self;

  // component selector/modifier
  // no bounds checking

  Number sel_component(int d) const {
    return component[d];
  }

  Number scomp(int d) const {
    return component[d];
  }

  void mod_component(int d, Number value) {
    component[d] = value;
  }

  void mcomp(int d, Number value) {
    component[d] = value;
  }

  Number & ref_component(int d) {
    return component[d];
  }

  const Number & comp(int d) const {
    return component[d];
  }

  // select dimension
  int sel_dimension() {
    return dimension;
  }

  // Operators

  Number & operator[](int i) { return ref_component(i); }

protected:

  Num_Vector_Base() {}

  Number component[dimension]; // representation is an array of numbers

  //friend class SqrMatrix;
};

template <class T, int dim>
class Num_Vector : public Num_Vector_Base<T, dim> {
public:

  typedef Num_Vector_Base<Number, dim> Base;
  typedef Num_Vector<Number, dim> Self;

  using Base::dimension;

  // constructors

  Num_Vector() {}

  Num_Vector(const Number val) {
    for (int i = 0; i < dimension; i++)
      mod_component(i, val);
  }

  // copy constructor
  Num_Vector(const Self & v) {
    for (int i=0; i < dimension; i++)
      component[i] = v.component[i];    
  }

  Num_Vector(const Number *v) {
    for (int i=0; i < dimension; i++)
      component[i] = v[i];    
  }

  // conversion functions;
  //class Vector3;
  //operator Vector3() const { return *this; }


  // copy from another vector
  void copy(const Self & v) {
    for (int i=0; i<dimension; i++)
      component[i] = v.component[i];
  }


  // this + v -> sum
  void add(const Self &v, Self & sum) const {
    for (int i=0; i<dimension; i++)
      sum.component[i] = component[i] + v.component[i];
  }

  // this <- this + v
  void add_self(const Self &v) {
    for (int i=0; i<dimension; i++)
      component[i] += v.component[i];
  }

  // add and generate CODE: this _gen funs are a bit sucky
  Self & add_gen(const Self &v) const {
    Self & sum = *new Self;
    for (int i=0; i<dimension; i++)
      sum.component[i] = component[i]+v.component[i];
    return sum;
  }

  // this - v -> diff
  void sub(const Self &v, Self & diff) const {
    for (int i=0; i<dimension; i++)
      diff.component[i] = component[i] - v.component[i];
  }

  // this <- this - v
  void sub_self(const Self &v) {
    for (int i=0; i<dimension; i++)
      component[i] -= v.component[i];
  }

  // subtract
  Self & sub_gen(const Self & v) const {
    Self & diff = *new Self;
    for (int i=0; i<dimension; i++)
      diff.component[i] = component[i]-v.component[i];
    return diff;
  }

  // dot product of two vectors
  Number dot_product(const Self & v) {
    Number product = 0; 
    for (int i=0; i<dimension; i++)
      product += component[i]*v.component[i];
    return product;
  }

  // assigns scalar product of this vec to another
  // this * k -> scl
  void scalar_product(const Number k, Self & scl) const {
    for (int i=0; i<dimension; i++)
      scl.component[i] = k * component[i];
  }

  // destructive scalar product
  // this <- this * k
  void scalar_product_self(const Number k) {
    for (int i=0; i<dimension; i++)
      component[i] *= k;
  }

  // scalar product
  Self scalar_product_gen(Number k) const {
    Self & scl = * new Self;
    for (int i=0; i<dimension; i++)
      scl.component[i] = component[i]*k;
    return scl;
  }

  // this * (1/k) -> scl
  void divide(Number k, Self & scl) const {
    for (int i=0; i<dimension; i++)
      scl.component[i] = component[i] / k;
  }

  // destructive divide
  // this <- this * (1/k)
  void divide_self(Number k) {
    for (int i=0; i<dimension; i++)
      component[i] /= k;
  }

  // divide
  Self & divide_gen(Number k) {
    Self & fac = * new Self;
    for (int i=0; i<dimension; i++)
      fac.component[i] = component[i]/k;
    return fac;
  }

  // destructive negation of each component
  // -(this) -> neg
  void negate(Self & neg) {
    for (int i=0; i<dimension; i++)
      component[i] = -component[i];
  }

  // destructive negation of each component
  // this <- (-this)
  void negate_self() {
    for (int i=0; i<dimension; i++)
      component[i] = -component[i];
  }

  // negation
  Self & negate_gen() {
    Self & neg = * new Self;
    for (int i=0; i<dimension; i++)
      neg.component[i]=-component[i];
    return neg;
  }

  // returns length of vector
  Number length() {
    Number len = 0;
    for (int i=0; i<dimension; i++)
      len += component[i]*component[i];
    return sqrt(len);
  }

  // normalize
  // post: norm.length() == 1
  void normalize(Self & norm) {
    divide(length(), norm);
  }

  // destructive normalize
  // post: length() == 1
  void normalize_self() {
    divide_self(length());
  }

  // generate a normalized vector
  Self & normalize_gen() {
    return divide_gen(length());
  }

  // Operators

  // Unary Operators
  //Self operator +() { return *this; }
  //Self operator -() { return this->negate_gen(); }
  
  /*
  // Binary Operators
  friend template<class Number, int dimension> Num_Vector<Number, dimension>
  operator+(Num_Vector<Number, dimension> A,  Num_Vector<Number, dimension> B); // addition
    { return A->add_gen(B); }

friend Self operator 
  -(Self A, Self); // subtraction
  { return A->sub_gen(B); }

friend Self operator 
  *(C k, Self A); // scalar product
  { return A->scalar_product_gen(k); }

friend Self operator 
  *(Self A, C k); // scalar product
  { return A->scalar_product_gen(k); }

friend Self operator 
  /(Self A, C d); // scalar product
  { return A->divide_gen(d); }

friend C  operator
  *(Self A, Self B); // dot product
  { return A->dot_product(B); }

friend Self operator 
  ^(Self A, Self B); // cross product
  {}//{ return A->cross(B); }
  */
//    template <class T, int dimension>
//    friend Num_Vector<T,d> operator *(Number k, Num_Vector<T,d> &V ); // scalar pro

  //friend Self operator *(Number k, Self &V); // scalar product

  // Compound-Assignment 
  Self & operator +=(Self & A) {
    add_self(A);
    return *this;
  }
  Self & operator -=(Self & A) {
    sub_self(A);
    return *this;
  }
  Self & operator *=(Number k) {
    scalar_product_self(k);
    return *this;
  }
  Self & operator /=(Number d) {
    divide_self(d);
    return *this;
  }

};

// binary operations

template <class T, int d>
Num_Vector<T,d> operator +(const Num_Vector<T,d> & x,
			   const Num_Vector<T,d> & y )
{
  Num_Vector<T,d> sum;
  x.add(y, sum);
  return sum;
}

template <class T, int d>
Num_Vector<T,d> operator -(const Num_Vector<T,d> & x,
			   const Num_Vector<T,d> & y )
{
  Num_Vector<T,d> diff;
  x.sub(y, diff);
  return diff;
}

// scalar product
template <class T, int d>
Num_Vector<T,d> operator *(T k, const Num_Vector<T,d> &v ) // scalar product
{
  Num_Vector<T,d> scl;
  v.scalar_product(k, scl);
  return scl;
}

template <class T, int d>
Num_Vector<T,d> operator /(const Num_Vector<T,d> &v, T k ) // scalar product
{
  Num_Vector<T,d> scl;
  v.divide(k, scl);
  return scl;
}


template <class C, int d>
istream& operator >>(istream &in, Num_Vector<C, d>& V)
{
  in >> "<";
  for (int i=0; i<d; i++)
    if (in)
      in >> V.ref_component(i);
  in >> ">";
  if (in)
    return in;
}

template <class C, int d>
ostream& operator <<(ostream &out, const Num_Vector<C, d>& V)
{
  out << "<";
  if (out)
    out << V.sel_component(0);
  for (int i=1; i<d; i++)
    if (out)
      //out << ", " << setw(5) << V.sel_component(i);
      out << ", " << V.sel_component(i);
  out << ">";
  return out;
}


#endif
