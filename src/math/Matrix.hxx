//
//
// C++ Interface for module: Matrix
//
// Description: 
//
//
// Author: exa
//
//

#ifndef Matrix_Interface
#define Matrix_Interface

#include "General.hxx"

#include "Numerical_Vector.hxx"

// Generic Dense Matrix class
// Number: numerical type to store in each element
// Rows: number of rows
// Columns: number of columns

template <class T, int rows, int columns>
class Matrix
{
public:
  typedef T Number;
  //static int rows = r;
  //static int columns = c;

  typedef Matrix<Number, rows, columns> Self;

  //constructors
  Matrix() {}

  // generic parameter selectors
  int sel_rows() const { return rows; } 
  int sel_columns() const { return columns; } 

  // access element first row, second column
  // we keep it like this to remain compatible with mathematical notation
  Number sel_element(int i, int j) const {
    return M[i][j];
  }
  void mod_element(int i, int j, Number val) {
    M[i][j] = val;
  }
  Number elt(int i, int j) const {
     return sel_element(i,j);
  }
  void mod_each_element(Number val) {
    for (int i=0; i<rows; i++)
      for (int j=0; j<columns; j++)
	M[i][j] = val;    
  }

  // add matrix A
  void add(Self& m) {
    for (int i=0; i<rows; i++)
      for (int j=0; j<columns; j++)
	M[i][j] += m.M[i][j];
  }

  // multiply object with m and return resulting matrix
  template <int columns2> Matrix<Number, rows, columns2> &
  multiply_gen(const Matrix<Number, columns, columns2> & m) {
    Matrix<Number, rows, columns2>* product =
      *new Matrix<Number, rows, columns2>;
    for (int i=0; i<rows; i++)
      for (int j=0; j<columns; j++) {
	Number product_elt = 0;
	for (int k=0; k<columns2; k++)
	  product_elt += M[i][k] * m.M[k][j];
	product->M[i][j] = product_elt;
      }
    return *product;
  }

  // multiply object with m and return resulting matrix
  template <int columns2> void
  multiply_inplace(const Matrix<Number, columns, columns2> & m,
		   Matrix<Number, rows, columns2> & product) {
    for (int i=0; i<rows; i++)
      for (int j=0; j<columns; j++) {
	Number product_elt = 0;
	for (int k=0; k<columns2; k++)
	  product_elt += M[i][k] * m.M[k][j];
	product.M[i][j] = product_elt;
      }
  }
	
  Num_Vector<Number, rows>& sel_column_gen(int c) const {
    Num_Vector<Number, rows> & col = *new Num_Vector<Number, rows>;
    for (int i=0; i<rows; i++)
      col.component[i]=M[i][c];
    return v;
  }

  void get_column(int c, Num_Vector<Number, rows> col) const {
    for (int i=0; i<rows; i++)
      col.component[i]=M[i][c];
  }
  /*     
     public NumVector get_row(int r) {
     NumVector v = new NumVector(COL);
     for (int i=0; i<COL; i++)
     v.component[i]=M[r][i];
     return v;
     }
     */

  //friend template<class Number, int rows, int columns>
  //istream &operator >>(istream& in, Matrix<Number, rows, columns> & A);
  //friend template<class N, int r, int c>
  //ostream &operator <<(ostream& out, const Matrix<Number, rows, columns> & A);

  // transpose the matrix
  void transpose() {
    for (int i=0; i<rows; i++)
      for (int j=i+1; j<columns; j++)
	swap(M[i][j], M[j][i]);
  }
  
protected:

  // Matrix is stored in a row major order array
  Number M[rows][columns];		
  
};

// Matrix streaming

template <class Number, int ext>
class Sqr_Matrix : public Matrix<Number, ext, ext>
{
public:
  Sqr_Matrix() {}

  // equivalence
  //operator Matrix<Number,ext,ext> () { return *this; }

  // load identity matrix
  void load_identity() {
    for (int i=0; i<ext; i++)
      for (int j=0; j<ext; j++) 
 	M[i][j] =  i!=j ? 0:1;
  }
};

//library design rule: do short names, but allow for the full ones
//since there is no template typedef
template <class Number, int ext>
class Square_Matrix : public Sqr_Matrix<Number,ext>
{
public:
  Square_Matrix() {}
  //operator Sqr_Matrix<Number,ext> () { return *this; }
};

typedef Square_Matrix<float,4> Matrix4;

template <class Number, int ext>
class Identity_Matrix : public Sqr_Matrix<Number, ext>
{
public:
  Identity_Matrix() {
    load_identity();
  }
  // operator Sqr_Matrix<Number,ext> () { return *this; }
};

template<class N, int r, int c>
istream &operator >>(istream& in, Matrix<N, r, c> & A)
{
  for (int i=0; i<c; i++)
    for (int j=0; j<r; j++) {
      N num;
      in >> num;
      A.mod_element(i,j,num);
    }
  return out;
}

template<class N, int r, int c>
ostream &operator <<(ostream& out, const Matrix<N, r, c> & A)
{
  for (int i=0; i<c; i++) {
    for (int j=0; j<r; j++)
      out << setw(5) << " " << A.sel_element(i,j);
    out << endl;
  }

  return out;
};

//CODE: is a TMatrix different than an ordinary one?
// don't think so!
//  class TMatrix : Matrix<Number, rows, columns>
//  {
//  public:
//      TMatrix() {}
    
//      Num_Vector<>
    
//  };

// generic functions

// v' = v.A where v and v' are row vectors
template <class Number, class Number2,  int ext>
Num_Vector<Number,ext>& multiply_gen(const Num_Vector<Number,ext>& v,
				     const Matrix<Number2,ext,ext>& T) {
  Num_Vector<Number,ext> & v_p = *new Num_Vector<Number,ext>;
  int i,j;
  for (j=0; j<ext; j++) {
    Number comp = 0;
    for (i=0; i<ext; i++) {
      comp += v.sel_component(i) * T.sel_element(i,j);
    }
    v_p.mod_component(j, comp);
  }
  return v_p;
}

// v' = T.v where v and v' are column vectors
template <class Number, class Number2, /*class Number3,*/ int ext>
Num_Vector<Number,ext>& multiply_gen(const Matrix<Number2,ext,ext>& T,
				     const Num_Vector<Number,ext>& v) {
  Num_Vector<Number,ext> & v_p = *new Num_Vector<Number,ext>;
  int i,j;
  for (i=0; i<ext; i++) {
    Number comp = 0;
    for (j=0; j<ext; j++) {
      comp +=  T.sel_element(i,j) * v.sel_component(j);
    }
    v_p.mod_component(i, comp);
  }
  return v_p;
}

template <class Number, class Number2, /*class Number3,*/ int ext>
void multiply
(const Matrix<Number2,ext,ext>& T,
 const Num_Vector<Number,ext>& v,
 Num_Vector<Number,ext>& v_p) {
  int i,j;
  for (i=0; i<ext; i++) {
    Number comp = 0;
    for (j=0; j<ext; j++) {
      comp +=  T.sel_element(i,j) * v.sel_component(j);
    }
    v_p.mod_component(i, comp);
  }
}


void Matrix_test();

#endif


