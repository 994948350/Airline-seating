#include <iostream>
#include "Matrix.h"
using namespace std;
int main(){
	int row=4;
	int col=4;
	int count=0;
	const double val=0;
	
	Matrix a(row,col,val);
	Matrix b(row,col,val);
	Matrix c(row,col,val);
	Matrix d(row,col,val);
	Matrix e(row,col,val);
	
	
	
	cout<<"before add value in to Matrix a,b,c,d,e(which are equal to each other)"<<endl;
	cout<<a<<endl;
	cout<<endl;
	
	cout<<"first case: new row, new col both >rwo, col, resize a: "<<endl;
	a.resize(7,7,5);
	cout<<a<<endl;
	cout<<endl;
	
	
	cout<<"second case: new row, new col both <rwo, col, resize b: "<<endl;
	b.resize(2,2,5);
	cout<<b<<endl;
	cout<<endl;
	
	cout<<"third case: new row, new col both =rwo, col, resize c: "<<endl;
	c.resize(4,4,5);
	cout<<c<<endl;
	cout<<endl;
	
	cout<<"fourth case: new row > row, new col <col, resize d"<<endl;
	d.resize(6,2,5);
	cout<<d<<endl;
	cout<<endl;
	
	cout<<"fith case: new row<row, new col>col, resize e"<<endl;
	e.resize(2,6,5);
	cout<<e<<endl;
	cout<<endl;
	
	
} 
