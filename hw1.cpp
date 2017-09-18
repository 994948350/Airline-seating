#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include<sstream>
using namespace std;
const string alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

/*find seat defaultly,assign passenger besides, and then middle
s:seats,clas:class of the passenger*/
bool find_default(vector<vector<string> >&s,string clas,string ID) {
	vector<int> better;
	//better: to store the index of seat wich is near the window or aisle
	better.push_back(1);
	if (clas!="F"&&clas!="B"&&clas!="C") {
		cout<<"Invalid seating class -"<<clas<<endl;	//error message catching
		return false;
	}
	for (int i=0; i<s.size(); i++) { //go through vector to find better seat
		if(clas==s[i][0]) {
			for(int j=0; j<s[i].size(); j++) {
				if(s[i][j]=="!!!!") {
					better.push_back(j-1);//seats near aisle
					better.push_back(j+1);
				}

			}
			break;
		}
	}
	better.push_back(s[0].size()-1);
	//assgin passenger to better seat
	for(int i=0; i<s.size(); i++) {
		if(clas==s[i][0]) {
			for (int j=0; j<better.size(); j++) {
				if(s[i][ better[j] ]=="....") {
					s[i][ better[j] ]=ID;
					cout<<"Passenger "<<ID<<" was given seat "<<i+1<<alpha[better[j]-1]<<"."<<endl;
					return false;
				}
			}
		}
	}
	//if no better seat available, go through and assign them to seat which is vacnat
	for(int i=0; i<s.size(); i++) {
		if(clas==s[i][0]) {
			for(int j=0; j<s[0].size(); j++) {
				if(s[i][j]=="....") {
					s[i][j]=ID;
					cout<<"Passenger "<<ID<<" was given seat "<<i+1<<alpha[j-1]<<"."<<endl;
					return false;
				}
			}
		}
	}
	return true;
}




/* check if a passenger required a seat, function to read the seat number
and add into vector if required seat exist.
codes are given by homework instruction, a little change on it
seat_request:requirment of passenger, vector s:seat information*/
bool GetSeatRowCol(string seat_request,int total_rows,
                   int total_columns,vector<vector<string> >&s,string ID) {
	string row_str; //use to store the string splited by request
	string col_str;
	string clas;//class the passenger will in
	//check the input string and discreminate the row and column
	for (int i = 0; i < seat_request.size(); ++i) {
		if (seat_request[i] >= '0' && seat_request[i] <= '9')
			row_str += seat_request[i];
		else
			col_str += seat_request[i];
	}
	int seat_row = atoi(row_str.c_str()) - 1; //change the string to int
	if (seat_row < 0 || seat_row > (int) total_rows - 1) {//seat required out of range
		return true;
	}

	int seat_col = alpha.find(col_str)+1;// find the position of character in alpha
	if (seat_col == std::string::npos || seat_col > total_columns) {//seat required out of range
		return true;
	}
	if (s[seat_row][seat_col]=="!!!") { //invalid seat,not occupied
		return true;
	}
	if (s[seat_row][seat_col]!="....") { //occupied seat
		cout<<"Passenger "<<ID<<" requested an occupied seat "<<seat_request<<"."<<endl;
		clas=s[seat_row][0];//find the class of the passnger(store at every start of vector s)
		find_default(s,clas,ID);
		return false;
	}
	s[seat_row][seat_col]=ID;   //if seat existm, add the ID to vector
	cout<<"Passenger "<<ID<<" was given seat "<<seat_request<<"."<<endl;
	clas=s[seat_row][0];
	return false;
}


/*function of SRN(assign passenger defaultly)vector s:seat vector,
clas:class passenger required,ID: id of passenger
freq: frequency level of a passenger, up_F:possibel to upgrade to F class
up_B: possible to be upgrade to B class*/
void SRN(vector<vector<string> >&s,string ID,string clas,string freq
         ,vector<string>& up_F,vector<string>& up_B) {
	bool full=find_default(s,clas,ID);  //use to check if there is seat available, and add passenger to vector
	if(full) {
		if (freq=="Y") {//frequent passenger will be upgrade
			if(clas=="C") up_B.push_back(ID);
			if(clas=="B") up_F.push_back(ID);
		}
	}

}

/*function of SSR(assign passenger to seat he wants)vector s:seat vector,
clas:class passenger required,ID: id of passenger
freq: frequency level of a passenger, up_F,up_B:
passenger who are able to upgrade class F,B, request: the seat passenger wants*/
void SSR(vector<vector<string> >&s,string ID,string request,string freq) {
	int total_rows=s.size();// get the totle number of rows
	int total_columns=s[0].size()-1;//totle number of column,-1 because vector start with class
	bool invalid=GetSeatRowCol(request,total_rows,total_columns,s,ID);
	//using function to find and add seat,check if valid
	if (invalid) {
		cout<<"Passenger "<<ID<<" requested an invalid seat "<<request<<"."<<endl;
	}

}

/*cancel passenger seat requiest.s:seats, up:passenger possible be upgraded.*/
void CXL(vector<vector<string> >&s,string ID,vector<string>&up_F,vector<string>& up_B) {
	vector<string> temp_up_F;     //use to store possible upgtade passenger ID
	vector<string> temp_up_B;
	for(int i=0;i<s.size();i++){//delete the passenger from the seat char.
		for(int j=0;j<s[0].size();j++){
			if(s[i][j]==ID){
				s[i][j]="....";
			}
		}
	}
	for (int i=0; i<up_F.size(); i++) { //cancel the passenger in upgradeing vector
		if(up_F[i]!=ID) temp_up_F.push_back(up_F[i]);//add those who will not be cancled to new vector
	}
	for (int i=0; i<up_B.size(); i++) {
		if(up_B[i]!=ID) temp_up_B.push_back(up_B[i]);
	}
	up_F=temp_up_F;//copy the data in new vector to original one
	up_B=temp_up_B;
}

/*print out those who is able to upgrade*/
void UPL(vector<vector<string> >s,
         vector<string>& up_F,vector<string>& up_B) {
	cout<<"The following passengers are scheduled to be upgraded to first class:"<<endl;
	for(int i=0; i<up_F.size(); i++) { // go through the up grade vectors and print
		cout<<up_F[i]<<endl;
	}
	cout<<"The following passengers are scheduled to be upgraded to business class:"<<endl;
	for(int i=0; i<up_B.size(); i++) {
		cout<<up_B[i]<<endl;
	}

}
//use to upgrade passnger
void UPP(vector<vector<string> >&s,
         vector<string>& up_F,vector<string>& up_B) {
	bool full;//check if upper class is still full
	string clas;//class the passenger will be up grade to
	string ID;
	for (int i=0; i<up_F.size(); i++) { // go through the passenger up grade vector
		clas="F";
		ID=up_F[i];
		full=find_default(s,clas,ID);//find seat defaulty for passenger
		if (!full) {
			cout<<"Passenger "<<ID<<" was upgraded to the first class section."<<endl;
		} else cout<<"Could not find seat in F section for passenger "<<ID<<endl; //erro catching: if the passenger cannot be upgrade
	}
	for (int i=0; i<up_B.size(); i++) {
		clas="B";
		ID=up_B[i];
		full=find_default(s,clas,ID);
		if (!full) {
			cout<<"Passenger "<<ID<<" was upgraded to the second class section."<<endl;
		} else cout<<"Could not find seat in B section for passenger "<<ID<<endl;
	}

}

//use to print the char of seats;name:flight name
void PRT(vector<vector<string> >s,string name) {
	cout<<"Seating Chart for Flight "<<name<<endl;
	for(int i=0; i<s.size(); i++) { //go through seat vector
		if(i==0) {
			cout<<"\t";
			for(int k=0; k<s[0].size()-1; k++) {
				cout<<alpha[k]<<"    ";//print the alphablet character at the head
			}
			cout<<"Seating Class"<<endl;
		}
		cout<<i+1<<"\t";//print the number of rwo
		for(int j=1; j<s[i].size(); j++) {
			cout<<s[i][j]<<" ";
		}
		cout<<s[i][0]<<endl;
	}
}
// use to read seat chart from the file,f1_name:file name
void read_seat(vector<vector<string> >&s,string f1_name) {
	ifstream fin;
	
	fin.open(f1_name.c_str());
	string x;
	while(getline(fin,x)) {
		vector<string> temp;
		string y;
		for (int i=0; i<x.length(); i++) {
			y=x[i];//to check if the first charactor(class) is alpha charactor
			if(y!=" ") { //skip the space
				if(isalpha(x[i]) ) { //if is alpha(class), store in first element of vector
					string string_temp(1,x[i]);
					temp.push_back(string_temp);
				}//the first element of a vector shows class

				if(!isalpha(x[i])) {
					string string_temp(4,x[i]); //type 4 times,for example"." to "...."
					temp.push_back(string_temp);
				}
			}
		}
		s.push_back(temp);
	}
	fin.close();
}
/*use to read cmd file and generate functions,f2_name:name of second file*/
void read_cmd(vector<vector<string> >&s,
              vector<string>& up_F,vector<string>& up_B,string f2_name) {
	ifstream fin;
	fin.open(f2_name.c_str());
	string cmd;//command
	string ID;
	string clas;//class
	string request;//seat request
	string freq;//frequent flier "Y" or "N"
	string name;//name of flight
	stringstream ss;
	string line;//line
	while(getline(fin,line)) {
		ss<<line;
		ss>>cmd;
		if(isdigit(cmd[0])) { //name of flight
			name=line;
		} else if(cmd=="SRN") {
			ss>>ID>>clas>>freq;
			SRN(s,ID,clas,freq,up_F,up_B);
		} else if (cmd=="SSR") {
			ss>>ID>>request>>freq;
			SSR(s,ID,request,freq);
		} else if (cmd=="CXL") {
			ss>>ID;
			//cout<<"hhh";
			CXL(s,ID,up_F,up_B);
		} else if (cmd=="UPL") UPL(s,up_F,up_B);
		else if (cmd=="UPP") UPP(s,up_F,up_B);
		else if (cmd=="PRT") PRT(s,name);
		else cout<<"Invalid command: "<<cmd;
		ss.clear();
	}
}

int main(int argc, char* argv[]) {
	if (argc!=3) {
		exit(0);
	}
	string f1_file=argv[1];
	string f2_file=argv[2];
	vector <vector<string> >s;//seats vector
	vector<string> up_F;//upgrade to first class(ID)
	vector<string> up_B;//upgrade to second class(ID)
	read_seat(s,f1_file);//read seat char
	read_cmd(s,up_F,up_B,f2_file);//read command and generate functions/.

	return 0;
}
