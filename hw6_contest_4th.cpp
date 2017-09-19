#include <iostream>
#include <vector>
#include <fstream>
#include <ostream>
#include <sstream>
#include <set>
#include <algorithm>
using namespace std;
//use to store the point
class Point {
	public:
		Point():row(0),col(0){}
		Point(int m_row, int m_col): row(m_row),col(m_col) {}
		Point(const Point& other):row(other.row),col(other.col) {}
		int row;
		int col;
};
bool operator== (const Point& lhs, const Point& rhs) {
	return lhs.col == rhs.col && lhs.row == rhs.row;
}
//use to combine point and direction together, convenient for programming
class Path {
	public:
		Path(int m_row,int m_col,int m_direction):start_point(m_row,m_col),direction(m_direction) {};
		Point start_point;  //the point the head of the word start
		int direction;
};
//use to print the final ouput
void print(const set<string>& collection, int col,ostream& ostr,bool one_solution) {
	if(collection.size()==0) {
		ostr<<"No solutions found"<<endl;
		return;
	}
	if(!one_solution)
		ostr<<collection.size()<<" solution(s)"<<endl;
	for(set<string>::const_iterator itr=collection.begin(); itr!=collection.end(); itr++) {
		ostr<<"Board:"<<endl;
		for(unsigned int k=0; k<(*itr).size(); k++) {
			if((k+1)%col==1) ostr<<"  ";
			ostr<<(*itr)[k];
			if((k+1)%col==0) ostr<<endl;
		}
	}

}
//check if the word insert in  a location following a direction will out of bond
bool check_out_bond( const vector<string>& puzzle,const string& m_word,int m_row,int m_col,int direction ) {
	//this direction will out of bond
	if(direction==0&&(m_col+1-int(m_word.size())<0||m_row+1-int(m_word.size())<0)) return true;
	else if(direction==1&&(m_row+1-int(m_word.size())<0)) return true;
	else if(direction==2&&(m_col+m_word.size()>puzzle[0].size()||m_row+1-int(m_word.size())<0)) return true;
	else if(direction==3&&(m_col+m_word.size()>puzzle[0].size())) return true;
	else if(direction==4&&(m_col+m_word.size()>puzzle[0].size()||m_row+m_word.size()>puzzle.size())) return true;
	else if(direction==5&&(m_row+m_word.size()>puzzle.size())) return true;
	else if(direction==6&&(m_col+1-int(m_word.size())<0||m_row+m_word.size()>puzzle.size())) return true;
	else if(direction==7&&(m_col+1-int(m_word.size())<0)) return true;
	//won't out of bond;
	else return false;
}

//search the word and insert into the puzzle
bool search_insert(unsigned int i,unsigned int j,const Point& point,const vector<string>& puzzle,const string& m_word) {
	for(unsigned int s=0; s<m_word.size(); s++) {
		if(i>=puzzle.size()||j>=puzzle[0].size()) return false;
		//when a location is '-' or match the letter itself, means can insert
		if(m_word[s]!=puzzle[i][j]&&puzzle[i][j]!='-')
			return false;
		//go to the next
		i+=point.row;
		j+=point.col;
	}
	return true;
}
//use to insert the word to the puzzle
void insert_word(const Point& point, vector<string>& new_puzzle, int row, int col,
                 const vector<string>& puzzle, const string& word) {
	//copy the old puzzle to new
	for(unsigned int i=0; i<puzzle.size(); i++) {
		new_puzzle.push_back(puzzle[i]);
	}

	//add the word
	for(unsigned int i=0; i<word.size(); i++) {
		new_puzzle[row][col]=word[i];
		row+=point.row;
		col+=point.col;
	}
}

//helper check_word
bool check_word(Point position,const vector<string>& puzzle,const string& word, const Point one_path,unsigned int index ) {
	//when whole word find, means the word in the puzzle
	if(index>=word.size()) return true;
	//out bond
	if(position.col<0||position.row<0||position.row>=int(puzzle.size())||position.col>=int(puzzle[0].size())) return false;
	else {
		//onece a letter not match, return false
		if(word[index]!=puzzle[position.row][position.col]) return false;
		//if match, go to next letter
		position.col+=one_path.col;
		position.row+=one_path.row;
		return check_word(position,puzzle,word,one_path,index+1);
	}
}

//check if the word should not appear in puzzle appear
bool check_word(Point position,const vector<string>& puzzle,const string& word,
                const vector<Point>& eight_paths) {
	//go through all the direction to check if word exist
	for(int i=0; i<8; i++) {
		if(!check_out_bond( puzzle,word, position.row,position.col,i ))
			if(check_word(position,puzzle,word,eight_paths[i],0))
				return true;
	}
	return false;
}


//check if the word should not appear in puzzle appear
//if contain return ture
bool check_delete_word(const vector<string>& puzzle,const vector<string>& delete_words,
                       const vector<Point>& eight_paths) {
//go through each word to see if the puzzle contain one of them
	for(unsigned int w=0; w<delete_words.size(); w++) {
		//go through the puzzle
		for(unsigned int i=0; i<puzzle.size(); i++) {
			for(unsigned int j=0; j<puzzle[i].size(); j++) {
				if(delete_words[w][0]==puzzle[i][j])
					//return ture, the word in the puzzle
					if(check_word(Point(i,j),puzzle,delete_words[w],eight_paths)==true)
						return true;
			}
		}
	}
	return false;

}

void insert_to_puzzle(vector<string>& puzzle, const vector<string>& words,
                      const vector<Point>& eight_paths,unsigned int index,
                      const vector<string>& delete_words,const string& alpha,set<string>& collection
                      ,bool one_solution,ostream& ostr);

void search_insert(const string& m_word, vector<string>& puzzle,
                   const vector<Point>& eight_paths, int index,const vector<string>& words,
                   const vector<string>& delete_words,const string& alpha,set<string>& collection,
                   bool one_solution,ostream& ostr) {
	//go through each point in the puzzle
	for(unsigned int i=0; i<puzzle.size(); i++) {
		for(unsigned int j=0; j<puzzle[i].size(); j++) {
			//go through each insert direction
			for(unsigned int d=0; d<eight_paths.size(); d++) {
				//out bond
				if(check_out_bond(puzzle,m_word,i,j,d)) continue;

				if(search_insert(i, j,eight_paths[d],puzzle,m_word)) {
					//if find a place to insert
					vector<string> new_puzzle;
					insert_word(eight_paths[d],new_puzzle,i,j,puzzle,m_word);

					insert_to_puzzle(new_puzzle,words,eight_paths,index+1,delete_words,alpha,collection, one_solution,ostr);
				}
			}
		}
	}
}

void add(vector<string>& puzzle,set<string>& collection,const string& alpha,
         const vector<string>& delete_words,const vector<Point>& eight_paths,bool one_solution,ostream& ostr);

//use to fill the '-' locations
void edit(vector<string>& puzzle,const string& alpha,int row, int col,int index,set<string>& collection,
          const vector<string>& delete_words,const vector<Point>& eight_paths,bool one_solution,ostream& ostr) {
	if(index>=26) {
		return;
	}
	puzzle[row][col]=alpha[index];
	//does not contain the delete_word
	if(!check_delete_word(puzzle,delete_words,eight_paths)) {
		add(puzzle,collection,alpha,delete_words,eight_paths,one_solution,ostr);
	}
	edit(puzzle,alpha,row,col,index+1,collection,delete_words,eight_paths, one_solution, ostr);

}

//add the puzzle to the collection
void add(vector<string>& puzzle,set<string>& collection,const string& alpha,
         const vector<string>& delete_words,const vector<Point>& eight_paths,bool one_solution,ostream& ostr) {
	string s;
	for(unsigned int i=0; i<puzzle.size(); i++) {
		for(unsigned int j=0; j<puzzle[i].size(); j++) {
			if(puzzle[i][j]=='-') {
				edit(puzzle,alpha,i,j,0,collection,delete_words,eight_paths, one_solution,ostr);
				return;

			}
		}
		s+=puzzle[i];
	}

	if(one_solution) {
		collection.insert(s);
		print(collection, puzzle[0].size(),ostr,one_solution);
		exit(0);
	}
	collection.insert(s);
}


void insert_to_puzzle(vector<string>& puzzle, const vector<string>& words,
                      const vector<Point>& eight_paths,unsigned int index,
                      const vector<string>& delete_words,const string& alpha,set<string>& collection,bool one_solution,ostream& ostr) {
	if(index>=words.size()) {
		if(!check_delete_word(puzzle,delete_words,eight_paths))
			//add to the collection
			add(puzzle,collection,alpha,delete_words,eight_paths,one_solution,ostr);
	} else {
		search_insert(words[index], puzzle, eight_paths, index,words,delete_words,alpha,collection,one_solution,ostr);
	}
}



bool arrange(const string& w1,const string& w2) {
	if(w1.size()>w2.size()) return true;
	else return false;
}

int main(int argc, char* argv[]) {
	if(argc!=3&&argc!=4) {
		cout<<"not enough argument"<<endl;
		return 0;
	}

	const string alpha("abcdefghijklmnopqrstuvwxyz");

	//creat puzzle
	ifstream istr(argv[1]);//puzzle readin
	ofstream ostr(argv[2]);//output txt file
	//third: option argument
	bool one_solution=false;
	if(argc==4){
		if (std::string(argv[3]) == std::string("one_solution"))
		one_solution=true;
	}
	


	int row;
	int col;
	istr>>col;
	istr>>row;

	//creat the puzzle
	vector<string> puzzle;
	string tmp(col,'-');
	for(int i=0; i<row; i++) {
		puzzle.push_back(tmp);
	}

	//read the word should appear and not
	vector<string> words;
	vector<string> delete_words;
	string line;
	while(getline(istr,line)) {
		stringstream ss;
		string simble;
		string m_word;
		ss<<line;
		ss>>simble>>m_word;
		if(simble=="+") {
			words.push_back(m_word);
		} else {
			if(m_word!="")
				delete_words.push_back(m_word);
		}
	}

	sort(words.begin(),words.end(),arrange);
	sort(delete_words.begin(),delete_words.end(),arrange);


	//eight paths
	vector<Point> eight_paths;
	eight_paths.push_back(Point(-1,-1));
	eight_paths.push_back(Point(-1,0));
	eight_paths.push_back(Point(-1,1));
	eight_paths.push_back(Point(0,1));
	eight_paths.push_back(Point(1,1));
	eight_paths.push_back(Point(1,0));
	eight_paths.push_back(Point(1,-1));
	eight_paths.push_back(Point(0,-1));

	int index=0;
	set<string> collection;
	insert_to_puzzle(puzzle, words,eight_paths,index,delete_words,alpha,collection,one_solution,ostr);
	print(collection, col,ostr,one_solution);
	return 0;
}
