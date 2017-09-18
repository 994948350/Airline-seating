#ifndef Matrix_h_
#define Matrix_h_

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
class Matrix{
	typedef unsigned int size_type;
	public:
		//constructors, destructor,operater
		Matrix(){this->creat();}  //default constructor, intialize with creat function
		Matrix(size_type row,size_type col,const double fill){this->creat(row,col,fill);}
		Matrix(const Matrix& v){copy(v);} //copy constructor: copy all the value from another Matrix object
		Matrix& operator=(const Matrix& v){copy(v);return *this;}
		~Matrix();
		 
		//accessor
		const size_type num_rows() const {return m_row;} // return the row of the matrix
		const size_type num_cols() const {return m_col;} //return the col of matrix
		bool get(size_type row, size_type col, double& val); //change the values in val, return false if the row or col not valid
		
		//advanced accessor
		double* get_row(size_type row);  //get one the row copy it in arry and return the arry(pointer)
		double* get_col(size_type col);	 //get one the column copy it in arry and return the arry(pointer)
		Matrix* quarter();
		
		//selfadded
		const double get_val(size_type row, size_type col) const {return m_matrix[row][col];}
		
		//modifier
		void clear();    //reset the number of row and col to zero 
		bool set(size_type row, size_type col, double val);//set the matrix[row][col] to val
		void resize(size_type row, size_type col,double fill);//extra credit, resize
		
		//operator
		const double* operator[](size_type location);   //should a const after?, why the double is a pointer?
		void multiply_by_coecient(int times);  //times every value in matrix
		bool swap_row(size_type r1,size_type r2); //switch the two row
		void transpose(); 
		
		//binary operator
		bool add(Matrix& other);
		bool subtract(Matrix& other);
		
	private:
		//private member function
		void creat();
		void creat(size_type row, size_type col,const double fill);
		Matrix& copy(const Matrix& v);
		
		//member variable
		double** m_matrix;
		size_type m_row;
		size_type m_col;
		size_type loc_row; //record where the location is  do I have to initialize?
		size_type loc_col; 
		size_type step;    //to decide which operator[] is operating		
};
ostream& operator<< (std::ostream& out,  Matrix& m);
bool operator!=(const Matrix& a,const Matrix& b);
bool operator==(const Matrix& a,const Matrix& b);

#endif
