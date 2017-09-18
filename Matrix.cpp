#include <iostream>
#include <string>
#include <cstdlib>
#include "Matrix.h" 
//destructor
Matrix::~Matrix() {
	for(unsigned int i=0;i<m_row;i++){
		delete []m_matrix[i];
	}
		delete []m_matrix;
}
		
//creat function help constructor to creat and initialize values in matrix
void Matrix::creat(){
	m_row=0;
	m_col=0;
	step=0;
	m_matrix=NULL;
}
//use to help construct 
void Matrix::creat(size_type row,size_type col, double fill){
	m_row=row;
	m_col=col;
	m_matrix=new double*[m_row];
	for(size_type i=0;i<m_row;i++){
		m_matrix[i]=new double[m_col];
		for(size_type j=0;j<m_col;j++){
			m_matrix[i][j]=fill;
		}
	}
}

//copy: to help copy constructor to get the value from another matrix
Matrix& Matrix::copy(const Matrix& v){
	this->m_row=v.m_row;
	this->m_col=v.m_col;
	this->step=v.step;
	this->m_matrix=new double* [m_row];
	// go through the "row", and creat column for each "row"
	for(unsigned int i=0;i<m_row;i++){
		m_matrix[i]=new double[m_col];
		for(unsigned int j=0;j<m_col;j++){
			this->m_matrix[i][j]=v.m_matrix[i][j];
		}
	}
	return *this;
}

//accessors:----------------------------------------------------------------------------------------------------------------
//get: the row/col exist return true, else false, and change the value which is passed as an reference
bool Matrix::get(size_type row, size_type col, double& val){
	if(row>=m_row||col>=m_col){ //if the required row,col out of bound, return false;
		return false;
	}
	else{
		val=m_matrix[row][col];
		return true;
	}
}

//advanced accessor
//get all row
double* Matrix::get_row(size_type row){
			double* temp=NULL;   //temp use to copy the required row
			if(row<m_row){
				temp=new double[m_col];
				for(size_type j=0;j<m_col;j++){  
					temp[j]=m_matrix[row][j];
					
				}
			}
				
			return temp;
		}
// get all col
double* Matrix::get_col(size_type col){
	double* temp=NULL;
		if(col<m_col){
			temp=new double[m_row];
			for(size_type j=0;j<m_row;j++){
				temp[j]=m_matrix[j][col];
				
				}
			}
				
			return temp;
}

//ordinary modifier======================================================================
//set required row and col to a value
bool Matrix::set(size_type row, size_type col, double val){
	if(row>=m_row||col>=m_col){
		return false;
	}
	else{
		m_matrix[row][col]=val;
		return true;
	}
	
}

// not sure if the clear right 
void Matrix::clear() {
	for(size_type i=0;i<m_row;i++){
		delete []m_matrix[i];
	}
	delete []m_matrix;
	
	creat();
	m_row=0;
	m_col=0;	
}


//binary matirx operation=====================================
// add two matrix together
//(not const variable because it call a method which changes something)
bool Matrix::add(Matrix& other){
		//make sure the size match
		if(other.num_rows()==m_row&&other.num_cols()==m_col ){
			for(unsigned int i=0;i<m_row;i++){   
			//go through arry, add each other
				for(unsigned int j=0;j<m_col;j++){
					double val=0;
					other.get(i,j,val);
					m_matrix[i][j]+=val;
					}
				}
				return true;
			}
			else return false;
		} 
		
//subtract each other
bool Matrix::subtract(Matrix& other){
	if(other.num_rows()==m_row&&other.num_cols()==m_col ){
	//go through arry, subtract each other
			for(unsigned int i=0;i<m_row;i++){
			//go through arry, sub
				for(unsigned int j=0;j<m_col;j++){
					double val=0;
					other.get(i,j,val);
					m_matrix[i][j]-=val;
					}
				}
				return true;
			}
			else return false;
}


//"slef" operation==========================
//times each element
void Matrix::multiply_by_coecient(int times){
	for(unsigned int i=0;i<m_row;i++){
		for(unsigned int j=0;j<m_col;j++){
			m_matrix[i][j]*=times;
				}
		
		}
		
}

//change two row
bool Matrix::swap_row(size_type r1,size_type r2){
	if(r1<m_row&&r2<m_row){
		double temp=0;  //use a temp to store elemnt 
		for(unsigned int i=0;i<m_col;i++){
			temp=m_matrix[r1][i];
			m_matrix[r1][i]=m_matrix[r2][i];
			m_matrix[r2][i]=temp;
		
		}
		return true;
	}
	else return false;
	
}

//split a arry to 4 parts
Matrix* Matrix::quarter(){
	//to check if odd
	bool odd_row=false;
	bool odd_col=false;
	Matrix* m_arry=new Matrix[4];
	unsigned int n_row=m_row/2;
	unsigned int n_col=m_col/2;
	//if odd, add 1 to ensure overlap
	if(m_row%2==1) {
		n_row+=1;
		odd_row=true;
	}
	if(m_col%2==1) {
		n_col+=1;
		odd_col=true;
	}
	//initialize four Matrix
	Matrix UL(n_row,n_col,0);
	Matrix UR(n_row,n_col,0);
	Matrix LL(n_row,n_col,0);
	Matrix LR(n_row,n_col,0);
	//copy constructor
	m_arry[0]=UL;
	m_arry[1]=UR;
	m_arry[2]=LL;
	m_arry[3]=LR;
	//UL assign
	for(unsigned int i=0;i<n_row;i++){
		for(unsigned int j=0;j<n_col;j++){
			m_arry[0].set(i,j,m_matrix[i][j]);
		
		}
	}
	//UR assign
	for(unsigned int i=0;i<n_row;i++){
		unsigned int start_col=n_col;
		if(odd_col) start_col=n_col-1;
		for(unsigned int j=start_col;j<m_col;j++){
			m_arry[1].set(i,j-start_col,m_matrix[i][j]);
		}
	}
	//LL assign
	unsigned int start_row=n_row;
	if(odd_row) start_row=n_row-1;
	for(unsigned int i=start_row;i<m_row;i++){
		for(unsigned int j=0;j<n_col;j++){
			m_arry[2].set(i-start_row,j,m_matrix[i][j]);
	
		}
	}
	//LR assign
	for(unsigned int i=start_row;i<m_row;i++){
		unsigned int start_col=n_col;
		if(odd_col) start_col=n_col-1;
		for(unsigned int j=start_col;j<m_col;j++){
			m_arry[3].set(i-start_row,j-start_col,m_matrix[i][j]);
	
		}
	}
	return m_arry;
}

//rotate and mirro a Matrix
void Matrix::transpose(){
	unsigned int temp_row=m_col;  //switch num row and col
	unsigned int temp_col=m_row;
	double** new_m=new double*[temp_row];
	//copy all element to new_m
	for(unsigned int i=0;i<temp_row;i++){
		new_m[i]=new double[temp_col];
		for(unsigned int j=0;j<temp_col;j++){
			new_m[i][j]=m_matrix[j][i];  // copy "row" to "col"
		}
	}
	clear();
	m_row=temp_row;
	m_col=temp_col;
	loc_row=0;
	loc_col=0; 
	m_matrix=new double*[temp_row];
	//copy new_m back
	for(unsigned int i=0;i<temp_row;i++){
		m_matrix[i]=new double[temp_col];
		for(unsigned int j=0;j<temp_col;j++){
			m_matrix[i][j]=new_m[i][j];
		}
	}
	//delete new_m	
	for(unsigned int i=0;i<temp_row;i++){
		delete []new_m[i];
	}
	delete []new_m;
}

//operator============================
//this operator is just for easir debuging(not in the requirment), it act like a 
//2D vector, the arry store can be call just by [][]
//for example: matrix_name[row][col] can directly get that vlaue;
const double* Matrix::operator[](size_type location){
	if(step==0){
		loc_row=location;
		step=1;
	}
	else if(step==1){
		loc_col=location;
		step=0;
	}
	return this->m_matrix[location];
}

//nun-member operator
ostream& operator<< (std::ostream& out,  Matrix& m){  // becausae the value change, the const has be deleted
	double val;
	out<<"\n"<<m.num_rows()<<" x "<<m.num_cols()<<" matrix:\n[ ";
	//go through access each values
	for(unsigned int i=0;i<m.num_rows();i++){
		for(unsigned int j=0;j<m.num_cols();j++){
			m.get(i,j,val);
			out<<val<<" ";
		}
		if(i!=m.num_rows()-1)
		out<<"\n  ";
	}
	out<<"]\n";
	return out;
}

//!= operator
bool operator!=(const Matrix& a,const Matrix& b){
	//check if row and col match
	if(a.num_rows()!=b.num_rows()||a.num_cols()!=b.num_cols())
		return true;
	else{//check each values match
		for(unsigned int i=0;i<a.num_rows();i++){
			for(unsigned int j=0;j<a.num_cols();j++){
				if(a.get_val(i,j) !=b.get_val(i,j))
				return true;
					
				}
			}
	}
	return false;
}
//==operator, same concept as !=operator
bool operator==(const Matrix& a,const Matrix& b){
	if(a.num_rows()!=b.num_rows()||a.num_cols()!=b.num_cols())
			return false;
	else{
		for(unsigned int i=0;i<a.num_rows();i++){
			for(unsigned int j=0;j<a.num_cols();j++){
				if(a.get_val(i,j) !=b.get_val(i,j))
				return false;
					
				}
			}
	}
	return true;
}

//extra credit function
//resize
void Matrix::resize(unsigned int row, unsigned int col, double fill) {
	
	//tmep row/ col use to stor the largest row/col
	unsigned int temp_row=m_row;
	unsigned int temp_col=m_col;
	bool large_row=(row>m_row);
	bool large_col=(col>m_col);
	
	if(large_row) temp_row=row;
	if(large_col) temp_col=col;
	
	//use a new 2d arry a_copy to store for new values
	double** a_copy=new double*[row];
	
	//go through the original arry, up to largest row/col
	for(unsigned int i=0;i<temp_row;i++){
		if(i==row) break;  //if the row all fill, stop goning on
		a_copy[i]=new double[col];
			for(unsigned int j=0;j<temp_col;j++){
				if(j>=col) continue;//if the col all fill, stop going on
				if(j>m_col-1||i>m_row-1) a_copy[i][j]=fill;//larger col/ row
				else a_copy[i][j]=m_matrix[i][j];//in the orignial bond;
			}	
		}
	
	clear();
	m_row=row;
	m_col=col;
	m_matrix=a_copy;		
}


