//
//
// C++ Interface for module: Numerical_Vector_Minor
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Numerical_Vector_Minor_Interface
#define Numerical_Vector_Minor_Interface

#include "General.hxx"


// Template Specializations

template <class T>
class Num_Vector<T, 2> : public Num_Vector_Base<T, 2> {
public:
  typedef Num_Vector<Number, 2> Self;

  Num_Vector() {}

  Num_Vector(const Number val) {
    component[0] = component[1] = val;
  }

  Num_Vector(const Self & v) {
    component[0] = v.component[0];
    component[1] = v.component[1];
  }
  
  Num_Vector(const Number *v) {
    component[0] = v[0];
    component[1] = v[1];
  }

  void copy(const Self &v) {
    component[0] = v[0];
    component[1] = v[1];
  }

  // this + v -> sum
  void add(const Self &v, Self & sum) const {
    sum.component[0] = component[0] + v.component[0];
    sum.component[1] = component[1] + v.component[1];
  }

  // this <- this + v
  void add_self(const Self &v) {
    component[0] += v.component[0];
    component[1] += v.component[1];
  }

  // this - v -> diff
  void sub(const Self &v, Self & diff) const {
    diff.component[0] = component[0] - v.component[0];
    diff.component[1] = component[1] - v.component[1];
  }

  // this <- this - v
  void sub_self(const Self &v) {
    component[0] -= v.component[0];
    component[1] -= v.component[1];
  }

  // dot product of two vectors
  Number dot_product(const Self & v) const {
    Number product = component[0]*v.component[0];
    product += component[1]*v.component[1];
    return product;
  }

  // assigns scalar product of this vec to another
  // this * k -> scl
  void scalar_product(const Number k, Self & scl) const {
    scl.component[0] = k * component[0];
    scl.component[1] = k * component[1];
  }

  // destructive scalar product
  // this <- this * k
  void scalar_product_self(const Number k) {
    component[0] *= k;
    component[1] *= k;
  }

  // this * (1/k) -> scl
  void divide(Number k, Self & scl) const {
    scl.component[0] = component[0] / k;
    scl.component[1] = component[1] / k;
  }

  // destructive divide
  // this <- this * (1/k)
  void divide_self(Number k) {
    component[0] /= k;
    component[1] /= k;
  }

  // destructive negation of each component
  // -(this) -> neg
  void negate(Self & neg) const {
    component[0] = -component[0];
    component[1] = -component[1];
  }

  // destructive negation of each component
  // this <- (-this)
  void negate_self() {
    component[0] = -component[0];
    component[1] = -component[1];
  }

  // returns length of vector
  Number length() const {
    Number len = component[0]*component[0] ;
    len += component[1]*component[1];
    return sqrt(len);
  }

  // normalize
  // post: norm.length() == 1
  void normalize(Self & norm) const {
    divide(length(), norm);
  }

  // destructive normalize
  // post: length() == 1
  void normalize_self() {
    divide_self(length());
  }

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


template <class T>
class Num_Vector<T, 3> : public Num_Vector_Base<T, 3> {
public:
  typedef Num_Vector<Number, 3> Self;

  Num_Vector() {}

  Num_Vector(const Number val) {
    component[0] = component[1] = component[2] = val;
  }

  Num_Vector(const Self & v) {
    component[0] = v.component[0];
    component[1] = v.component[1];
    component[2] = v.component[2];
  }
  
  Num_Vector(const Number *v) {
    component[0] = v[0];
    component[1] = v[1];
    component[2] = v[2];
  }

  void copy(const Self &v) {
    component[0] = v[0];
    component[1] = v[1];
    component[2] = v[2];
  }

  // this + v -> sum
  void add(const Self &v, Self & sum) const {
    sum.component[0] = component[0] + v.component[0];
    sum.component[1] = component[1] + v.component[1];
    sum.component[2] = component[2] + v.component[2];
  }

  // this <- this + v
  void add_self(const Self &v) {
    component[0] += v.component[0];
    component[1] += v.component[1];
    component[2] += v.component[2];
  }

  // this - v -> diff
  void sub(const Self &v, Self & diff) const {
    diff.component[0] = component[0] - v.component[0];
    diff.component[1] = component[1] - v.component[1];
    diff.component[2] = component[2] - v.component[2];
  }

  // this <- this - v
  void sub_self(const Self &v) {
    component[0] -= v.component[0];
    component[1] -= v.component[1];
    component[2] -= v.component[2];
  }

  // dot product of two vectors
  Number dot_product(const Self & v) const {
    Number product = component[0]*v.component[0];
    product += component[1]*v.component[1];
    product += component[2]*v.component[2];
    return product;
  }

  // assigns scalar product of this vec to another
  // this * k -> scl
  void scalar_product(const Number k, Self & scl) const {
    scl.component[0] = k * component[0];
    scl.component[1] = k * component[1];
    scl.component[2] = k * component[2];
  }

  // destructive scalar product
  // this <- this * k
  void scalar_product_self(const Number k) {
    component[0] *= k;
    component[1] *= k;
    component[2] *= k;
  }

  // this * (1/k) -> scl
  void divide(Number k, Self & scl) const {
    scl.component[0] = component[0] / k;
    scl.component[1] = component[1] / k;
    scl.component[2] = component[2] / k;
  }

  // destructive divide
  // this <- this * (1/k)
  void divide_self(Number k) {
    component[0] /= k;
    component[1] /= k;
    component[2] /= k;
  }

  // destructive negation of each component
  // -(this) -> neg
  void negate(Self & neg) const {
    component[0] = -component[0];
    component[1] = -component[1];
    component[2] = -component[2];
  }

  // destructive negation of each component
  // this <- (-this)
  void negate_self() {
    component[0] = -component[0];
    component[1] = -component[1];
    component[2] = -component[2];
  }

  // returns length of vector
  Number length() const {
    Number len = component[0]*component[0] ;
    len += component[1]*component[1];
    len += component[2]*component[2];
    return sqrt(len);
  }

  // normalize
  // post: norm.length() == 1
  void normalize(Self & norm) const {
    divide(length(), norm);
  }

  // destructive normalize
  // post: length() == 1
  void normalize_self() {
    divide_self(length());
  }

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


template <class T>
class Num_Vector<T, 4> : public Num_Vector_Base<T, 4> {
public:

  typedef Num_Vector<Number, 4> Self;

  Num_Vector() {}

  Num_Vector(const Number val) {
    component[0] = component[1] = component[2] = component[3] = val;
  }

  Num_Vector(const Self & v) {
    component[0] = v.component[0];
    component[1] = v.component[1];
    component[2] = v.component[2];
    component[3] = v.component[3];
  }
  
  Num_Vector(const Number *v) {
    component[0] = v[0];
    component[1] = v[1];
    component[2] = v[2];
    component[3] = v[3];
  }

  void copy(const Self &v) {
    component[0] = v[0];
    component[1] = v[1];
    component[2] = v[2];
    component[3] = v[3];
  }

  // this + v -> sum
  void add(const Self &v, Self & sum) const {
    sum.component[0] = component[0] + v.component[0];
    sum.component[1] = component[1] + v.component[1];
    sum.component[2] = component[2] + v.component[2];
    sum.component[3] = component[3] + v.component[3];
  }

  // this <- this + v
  void add_self(const Self &v) {
    component[0] += v.component[0];
    component[1] += v.component[1];
    component[2] += v.component[2];
    component[3] += v.component[3];
  }

  // this - v -> diff
  void sub(const Self &v, Self & diff) const {
    diff.component[0] = component[0] - v.component[0];
    diff.component[1] = component[1] - v.component[1];
    diff.component[2] = component[2] - v.component[2];
    diff.component[3] = component[3] - v.component[3];
  }

  // this <- this - v
  void sub_self(const Self &v) {
    component[0] -= v.component[0];
    component[1] -= v.component[1];
    component[2] -= v.component[2];
    component[3] -= v.component[3];
  }

  // dot product of two vectors
  Number dot_product(const Self & v) const {
    Number product = component[0]*v.component[0];
    product += component[1]*v.component[1];
    product += component[2]*v.component[2];
    product += component[3]*v.component[3];
    return product;
  }

  // assigns scalar product of this vec to another
  // this * k -> scl
  void scalar_product(const Number k, Self & scl) const {
    scl.component[0] = k * component[0];
    scl.component[1] = k * component[1];
    scl.component[2] = k * component[2];
    scl.component[3] = k * component[3];
  }

  // destructive scalar product
  // this <- this * k
  void scalar_product_self(const Number k) {
    component[0] *= k;
    component[1] *= k;
    component[2] *= k;
    component[3] *= k;
  }

  // this * (1/k) -> scl
  void divide(Number k, Self & scl) const {
    scl.component[0] = component[0] / k;
    scl.component[1] = component[1] / k;
    scl.component[2] = component[2] / k;
    scl.component[3] = component[3] / k;
  }

  // destructive divide
  // this <- this * (1/k)
  void divide_self(Number k) {
    component[0] /= k;
    component[1] /= k;
    component[2] /= k;
    component[3] /= k;
  }
  // destructive negation of each component
  // -(this) -> neg
  void negate(Self & neg) const {
    component[0] = -component[0];
    component[1] = -component[1];
    component[2] = -component[2];
    component[3] = -component[3];
  }

  // destructive negation of each component
  // this <- (-this)
  void negate_self() {
    component[0] = -component[0];
    component[1] = -component[1];
    component[2] = -component[2];
    component[3] = -component[3];
  }

  // returns length of vector
  Number length() const {
    Number len = component[0]*component[0] ;
    len += component[1]*component[1];
    len += component[2]*component[2];
    len += component[3]*component[3];
    return sqrt(len);
  }

  // normalize
  // post: norm.length() == 1
  void normalize(Self & norm) const {
    divide(length(), norm);
  }

  // destructive normalize
  // post: length() == 1
  void normalize_self() {
    divide_self(length());
  }

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


#endif
