#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime>
#include <cstdlib>
#include <assert.h>
using namespace std;



// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
//window =2 use this my_map below
typedef  map<string, map<string, int> >MY_MAP1;
//window =2 use this my_map below
typedef  map<string, map<string, map<string,int> > >MY_MAP2;
//extra credit
typedef  map<string, vector<string> >  MY_MAP;



// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
	char c;
	word.clear();
	while (istr) {
		// just "peek" at the next character in the stream
		c = istr.peek();
		if (isspace(c)) {
			// skip whitespace before a word starts
			istr.get(c);
			if (word != "") {
				// break words at whitespace
				return true;
			}
		} else if (c == '"') {
			// double quotes are a delimiter and a special "word"
			if (word == "") {
				istr.get(c);
				word.push_back(c);
			}
			return true;
		} else if (isalpha(c)) {
			// this a an alphabetic word character
			istr.get(c);
			word.push_back(tolower(c));
		} else {
			// ignore this character (probably punctuation)
			istr.get(c);
		}
	}
	return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
	// returns a vector of strings of the different words
	std::vector<std::string> answer;
	std::string word;
	bool open_quote = false;
	while (ReadNextWord(istr,word)) {
		if (word == "\"") {
			if (open_quote == false) {
				open_quote=true;
			} else {
				break;
			}
		} else {
			// add each word to the vector
			answer.push_back(word);
		}
	}
	return answer;
}



// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(MY_MAP1 &data1, MY_MAP2 &data2, const std::string &filename,
                    int window, const std::string &parse_method, string& last_word, string& pre_last_word) {
	// open the file stream
	std::ifstream istr(filename.c_str());
	if (!istr) {
		std::cerr << "ERROR cannot open file: " << filename << std::endl;
		exit(1);
	}
	// verify the window parameter is appropriate
	if (window < 2) {
		std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
	}
	// verify that the parse method is appropriate
	bool ignore_punctuation = false;
	if (parse_method == "ignore_punctuation") {
		ignore_punctuation = true;
	} else {
		std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
		exit(1);
	}



	//
	// ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
	//

	//use to store three word
	string word;
	string pre_word;
	string prepre_word;
	//use to help to count whether or not the preword, prepre_word appear.
	int count=0;
	//a helper string for editing last tow words at the end;
	string tmp_word;
	while (ReadNextWord(istr,word)) {
		count++;
		// skip the quotation marks (not used for this part)
		if (word == "\"") continue;

		//when window ==2
		if (window==2) {
			//add the first to map, with uinitialized key
			if(count==1)
				data1[word];
			else {
				//begin to creat or add 1 to int value
				data1[pre_word][word]++;
			}
			pre_word=word;
		}

		if (window==3) {
			if(count==1)
				//add firt to map with unitialized value
				data2[word];
			else if(count==2) {
				//add second to map with unitialized value
				map<string, map<string,int> >tmp;
				tmp[word];
				data2[pre_word]=tmp;
			} else {
				//begin to creat or add 1 to int value
				data2[prepre_word][pre_word][word]++;
			}

			//after count=2, prepre_word appear
			if(count>=2)
				prepre_word=pre_word;

			tmp_word=pre_word;
			//renew preword each time
			pre_word=word;
		}
	}

	//word=pre_word;
	//pre_word=tmp_word;
	//renew the last word and pre last word;
	last_word=pre_word;
	pre_last_word=tmp_word;


	// do somthing to the last element

	if(window==2) {
		data1[last_word];
	}

	if(window==3) {
		data2[pre_last_word][last_word];
		data2[last_word];
	}



	//
	// ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
	// INSERTING THESE WORDS INTO THE DATA STRUCTURE
	//


}

//count how many time the key appear, my_map is the inner map of the key
//count the first string below.
//map<string, map<string, int> >
int count_inner_key(const map<string, int>& my_map) {
	int count=0;
	//go through each pair and add the integer to together
	for(map<string, int>::const_iterator itr=my_map.begin(); itr!=my_map.end(); itr++) {
		count+=itr->second;
	}
	return count;
}

//count how many time the outer key appear, my_map is the second inner map of the key
//count the first string below
//map<string, map<string, map<string, int> > >
int count_outer_key(const map<string, map<string,int> >& my_map) {
	int count=0;
	//go through the second map
	for(map<string,map<string,int> >::const_iterator itr=my_map.begin(); itr!=my_map.end(); itr++) {
		//itr->second is a type map<string, map<string, int> >
		//follow this map to counter the inner string
		count+=count_inner_key(itr->second);
	}
	return count;
}

//print window2
void print_window2(vector<string>& sentence,const MY_MAP1& d1, const string& last_word) {
	//make sure the size of sentence is 1
	if(sentence.size()!=1) return;
	//find where to start to find
	MY_MAP1::const_iterator itr=d1.find(sentence[0]);
	if(itr==d1.end()) {//check if the itr exist
		cout<<sentence[0]<<" (0)"<<endl;
		cout<<endl;
		return;
	}

	//follow the second map of string, count the number;
	//itr->second is a type of map<string, int>
	int occurrence=count_inner_key(itr->second);
	//consider if the word is the last one, if so, add the occurrence
	if(sentence[0]==last_word) occurrence+=1;
	cout<<sentence[0]<<" ("<<occurrence<<")"<<endl;

	//start to print out each inner map
	for(map<string, int>::const_iterator in_itr=(itr->second).begin(); in_itr!=(itr->second).end(); in_itr++) {
		cout<<itr->first<<" "<<in_itr->first<<" ("<<in_itr->second<<")"<<endl;
	}
	cout<<endl;
}

//print 3 window
void print_window3(vector<string>& sentence,const MY_MAP2& d2, const string& last_word, const string& pre_last_word) {
	//first situation, print out just one word;
	int occurrence=0;
	//follow the first string
	MY_MAP2::const_iterator itr2=d2.find(sentence[0]);
	//check if the itr2 exist
	if(itr2==d2.end()) {
		for(int i=0; i<sentence.size(); i++) {
			cout<<sentence[i]<<" ";
		}
		cout<<"(0)"<<endl;
		cout<<endl;
		return;
	}

	//only need to follow one word;
	if(sentence.size()==1) {
		//itr2->second is a type map<string, map<string, int> >
		occurrence=count_outer_key(itr2->second);
		//check if the sentence is the last two word;
		if(sentence[0]==pre_last_word) occurrence++;
		if(sentence[0]==last_word) occurrence++;

		cout<<sentence[0]<<" ("<<occurrence<<")"<<endl;

		//get into the next layer
		//next layer: follow the sentence[0] get in to its map<string, map<string, int> >
		MY_MAP1::const_iterator itr;//map<string, map<string,int> >
		for(itr=(itr2->second).begin(); itr!=(itr2->second).end(); itr++) {
			//the itr is a type map<string, map<string, int> >
			occurrence=count_inner_key(itr->second);
			//to check if is the last two word
			if(itr2->first==pre_last_word&&itr->first==last_word)
				occurrence++;

			cout<<itr2->first<<" "<<itr->first<<" ("<<occurrence<<")"<<endl;
		}

	} else { //the sentence size==2, means get in to the most inner map
		//according the second string find the map
		MY_MAP1::const_iterator itr=(itr2->second).find(sentence[1]);

		//check if exist
		if(itr==(itr2->second).end()) return;
		//itr->second is a type map<string, int>
		occurrence=count_inner_key(itr->second);
		//check if the sentence[0] sentence[1] is the last two word
		if(sentence[0]==pre_last_word&&sentence[1]==last_word)
			occurrence++;


		cout<<sentence[0]<<" "<<sentence[1]<<" ("<<occurrence<<")"<<endl;

		//follow to keys, get in to the most inner map<string,int>
		for(map<string, int>::const_iterator in_itr=(itr->second).begin(); in_itr!=(itr->second).end(); in_itr++) {
			cout<<itr2->first<<" "<<itr->first<<" "<<in_itr->first<<" ("<<in_itr->second<<")"<<endl;
		}
	}

	cout<<endl;
}

//to find next word according to pre word and the map
//map is used to constrain the range the next word can be chosed
//MY_MAP1: map< string, map<string, int> >
string choose_next_word(const string& key,const MY_MAP1& my_map,bool random,
                        const string& pre_last_word,const string& last_word) {
	string next_word;
	//not random part
	if(!random) {
		MY_MAP1::const_iterator itr=my_map.find(key);

		//use to store the max_num
		int max_num=0;
		//use to stroe the key with max_num
		string max_word;

		if(itr==my_map.end()) return max_word;

		//check if the last last word ==next
		//only work in window 3, when window ==2, pre_last_word is a null
		if(pre_last_word==key) {
			map<string, int>::const_iterator in_itr=(itr->second).find(last_word);
			//the last word does in the following map
			if(in_itr==(itr->second).end()) {
				max_num=1;
			} else
				max_num=in_itr->second+1;
			max_word=last_word;
		}



		//go through the map and find the max number
		for(map<string,int>::const_iterator in_itr=(itr->second).begin(); in_itr!=(itr->second).end(); in_itr++) {
			if(in_itr->second>=max_num) {
				if(in_itr->second==max_num&&in_itr->first>max_word) continue;
				//compare which word come first
				max_word=in_itr->first;
				max_num=in_itr->second;

			}
		}
		next_word=max_word;
	} else {
		//random case
		//to find what the 'range' of the random number is,
		//map<string, map<string, int> >
		MY_MAP1::const_iterator itr=my_map.find(key);
		double ran_numf=0.0;
		//itr->second is a type map<string, int>
		double denominator=count_inner_key(itr->second);
		//check if the p
		if(pre_last_word==key) {
			denominator++;
		}

		double prob=1.0;
		//generate the random number
		ran_numf=rand()/(double)(RAND_MAX);
		//go through the inner map to find the count
		for(map<string, int>::const_iterator in_itr=(itr->second).begin(); in_itr!=(itr->second).end(); in_itr++) {
			//use the magnitude(range of the number) to indicate how much the probobility is
			prob-=(double(in_itr->second))/denominator;
			//find the range of random number which indicate the word
			if(ran_numf>prob) {
				next_word=in_itr->first;
				break;
			}
		}
		//cuze the pre_last_word does not add the the count
		if(ran_numf<prob) {
			next_word=last_word;
		}
	}
	return next_word;
}

//generate most_commond word or random in 2-window situation
void generate_2_window(bool random, const string& word,const MY_MAP1& data1,
                       int length,const string& pre_last_word,const string& last_word) {
	string start_word=word;
	//find next word 'length' times
	for(int i=0; i<length; i++) {
		//renew and print the start word;
		start_word=choose_next_word(start_word,data1,random,pre_last_word, last_word);
		if(start_word=="") break;
		cout<<" "<<start_word;
	}
}

//generate most_common word or random in 3-window situation
void generate_3_window(bool random, const vector<string>& sentence,const MY_MAP2& data2,
                       int length, const string& pre_last_word, const string& last_word) {
	//accroding to the first key word given find the map
	MY_MAP2::const_iterator itr2=data2.find(sentence[0]);
	if(itr2==data2.end()) return;
	int max=0;
	string max_next;
	// if the second word is not given
	if(sentence.size()==1) {
		int occurrence=count_outer_key(itr2->second);
		//check if the pre last word is equal to the string, if so take it into consider
		if(pre_last_word==sentence[0]) {
			max=1;
			max_next=last_word;
			occurrence+=1;
		}

		//generate random number
		double ran_numf=0.0;

		double denominator=occurrence;
		double prob=1.0;
		ran_numf=rand()/(double)(RAND_MAX);

		for(MY_MAP1::const_iterator itr=(itr2->second).begin(); itr!=(itr2->second).end(); itr++) {
			//random part
			if(random) {
				prob-=double(count_inner_key(itr->second))/denominator;
				if(prob<ran_numf) {
					max_next=itr->first;
					break;
				}
			}
			int num=count_inner_key(itr->second);
			if(itr->first==last_word) num++;
			//not random
			//start from the second outer layer, add int number together
			if(!random&&max<=num) {
				if(max==num&&max_next<itr->first) continue;
				//compare which comes first

				max=num;
				max_next=itr->first;


			}
		}
		//the situation that max_next does not generate in the for loop
		//cuz add the occurrence of a word which inner map does not initialized
		if(random&&prob>ran_numf) {
			max_next=last_word;
		}

		cout<<" "<<max_next;
		length=length-1;
	}
	//second word given
	if(sentence.size()==2) {
		max_next=sentence[1];
	}

	//start to find and print the word
	string start=sentence[0];
	string next=max_next;
	for(int i=0; i<length; i++) {
		//find map<string, map<string,map<string,int> > >
		itr2=data2.find(start);
		start=next;
		next=choose_next_word(next,itr2->second,random,pre_last_word, last_word);
		if(next=="") break;
		cout<<" "<<next;
	}
	cout<<endl;
}

//--------------below are extra credit ------------------------
string merge(const string& w1,const string& w2) {
	if(w1.size()==0) return w2;
	string new_word= w1+" "+w2;
	return new_word;
}

bool exist(const string& key, const string& value, MY_MAP &data ) {
	MY_MAP::iterator itr=data.find(key);
	//key does not exist
	if(itr==data.end() ) {
		return false;
	} else { //key exist but value does not exist;
		//itr->second is a vector
		for(int i=0; i< itr->second.size() ; i++) {
			//once if match
			if(value==itr->second[i])
				return true;
		}
		return false;
	}
}

// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText2(MY_MAP &data, const std::string &filename, 
					int window, const std::string &parse_method, map<string, int>& occurrence) {
	// open the file stream
	std::ifstream istr(filename.c_str());
	if (!istr) {
		std::cerr << "ERROR cannot open file: " << filename << std::endl;
		exit(1);
	}
	// verify the window parameter is appropriate
	if (window < 2) {
		std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
	}
	// verify that the parse method is appropriate
	bool ignore_punctuation = false;
	if (parse_method == "ignore_punctuation") {
		ignore_punctuation = true;
	} else {
		std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
		exit(1);
	}



	//
	// ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
	//


	string word;
	vector<string> words;
	int counter=0;//use to count the word, every "window" times merge a word to a new word
	while (ReadNextWord(istr,word)) {
		// skip the quotation marks (not used for this part)
		if (word == "\"") continue;
		occurrence[word]++;//add occurrence to every word;
		words.push_back(word);
		//if the vector words size reach the window size
		if(words.size()==window) {
			//merge to creat new word
			string new_word=words[0];

			for(int i=1; i<window; i++) {
				if(!exist(new_word,words[i],data)) {
					data[new_word].push_back(words[i]);
				}
				//renew the new word
				new_word=merge(new_word, words[i]);
				occurrence[new_word]++;
			}
			
			

			//use to erase the first element
			vector<string> tmp;
			for(int i=1; i<words.size(); i++) {
				tmp.push_back(words[i]);
			}
			words.clear();
			for(int i=0; i<tmp.size(); i++) {
				words.push_back(tmp[i]);
			}
		}
		counter++;
		
	}
	
	
	
	//merge the words left in the vector words
	for(int size=1;size<=words.size();size++){
		//i represent a start point
		for(int i=0;i<=words.size()-size;i++){
			string tmp=words[i];
			for(int j=i;j<i+size-1;j++){
				tmp=merge(tmp,words[j+1]);
			}
			//tmp finish;
			if(size!=1) occurrence[tmp]++;
			
			if(i+size<words.size()&&(!exist(tmp,words[i+size],data))){
				data[tmp].push_back(words[i+size]);
			}
				
				
		}
	}



	//
	// ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
	// INSERTING THESE WORDS INTO THE DATA STRUCTURE
	//
	
	for(MY_MAP::iterator itr=data.begin(); itr!=data.end(); itr++) {
		sort(itr->second.begin(),itr->second.end());
	}


}



void print(const map<string, int>& occurrence, MY_MAP& data, const string& word, int windows) {
	map<string, int>::const_iterator itr=occurrence.find(word);
	if(itr!=occurrence.end())
		cout<<word<<" ("<<itr->second<<")"<<endl;

	for(MY_MAP::iterator detail_itr=data.begin(); detail_itr!=data.end(); detail_itr++) {
		//word in data follow in vector;

		if(detail_itr->first==word) {
			for(int i=0; i<detail_itr->second.size(); i++) {
				string new_word=merge(word,detail_itr->second[i]);
				itr=occurrence.find(new_word);
				cout<<itr->first<<" ("<<itr->second<<")"<<endl;
			}
		}
	}

}

string delete_merge(const string& new_word,const string& max_word,int window) {
	int counter=0;
	for(int i=0; i<new_word.size(); i++) {
		if(new_word[i]==' ') counter++;
	}

	//not match the space suppose to be, just merge
	if(counter!=window-2) {
		return merge(new_word,max_word);
	} else { //match the space suppose, delete the first part
		string return_word;
		bool start_to_add=false;
		for(int i=0; i<new_word.size(); i++) {
			if(new_word[i]==' '&&start_to_add==false) {
				start_to_add=true;
				i++;
			}
			if(start_to_add)
				return_word+=new_word[i];
		}
		return merge(return_word,max_word);

	}

}

string choose_next_word2(const string& new_word,double ran_numf,const MY_MAP& data,const map<string, int>& occurrence) {
	double denominator=0.0;
	MY_MAP::const_iterator itr=data.find(new_word);

	//find the denominator
	for(int i=0; i<itr->second.size(); i++) {
		string tmp=merge(new_word,itr->second[i] );
		map<string, int>::const_iterator in_itr=occurrence.find(tmp);
		denominator+=double(in_itr->second);
	}
	
	double prob=1;//to find the rank of the random number
	for(int i=0;i<itr->second.size(); i++){
		string tmp=merge(new_word,itr->second[i] );
		map<string, int>::const_iterator in_itr=occurrence.find(tmp);
		prob-=(double)(in_itr->second)/(denominator);
		if(prob<ran_numf)
			return itr->second[i];
	}

}

void print_generate(bool random, const vector<string>& sentence,int window,
                    int length,const MY_MAP& data,const map<string, int>& occurrence) {

	string new_word;
	//according to the window, merge the word

	if(sentence.size()<=window-1) {
		new_word=sentence[0];
		for(int i=1; i<sentence.size(); i++) {
			new_word=merge(new_word,sentence[i]);
		}
	} else {
		new_word=sentence[sentence.size()-window+1];

		for(int i=sentence.size()-window+2; i<sentence.size(); i++) {
			new_word=merge(new_word,sentence[i]);
		}
	}



	for(int i=0; i<length; i++) {
		if(!random) { //find most_commond;
			MY_MAP::const_iterator itr=data.find(new_word);
			int max=0;
			string max_word;
			//go through the vector, get the word follow this word
			for(int j=0; j<itr->second.size(); j++) {
				string tmp=merge(new_word,itr->second[j]);
				map<string, int>::const_iterator in_itr=occurrence.find(tmp);
				if(in_itr!=occurrence.end())//coner case at the end, no next word can be found 
					if(in_itr->second>max) {
						max=in_itr->second;
						max_word=itr->second[j];
					}
			}
			//after for loop, the max_word found
			cout<<max_word<<" ";
			
			new_word=delete_merge(new_word,max_word,window);
			//cout<<"test: "<<i<<" \""<<new_word<<"\""<<endl;
		} 
		else { //find rand
			string rand_word;
			double ran_numf=0.0;
			ran_numf=rand()/(double)(RAND_MAX);

			rand_word=choose_next_word2(new_word,ran_numf,data,occurrence);

			cout<<rand_word<<" ";
			new_word=delete_merge(new_word,rand_word,window);
		}


	}
	cout<<endl;
}

int main () {
	srand((unsigned)time(0));//use for random
	// ASSIGNMENT: THE MAIN DATA STRUCTURE
	MY_MAP1 data1;
	MY_MAP2 data2;
	
	//extra credit needed
	MY_MAP data;//data use to store words follow a word
	map<string, int> occurrence;//use to store all the occurrence of a word


	int window;
	string last_word;
	string pre_last_word;
	
	// Parse each command
	std::string command;
	while (std::cin >> command) {

		// load the sample text file
		if (command == "load") {
			std::string filename;
			std::string parse_method;
			cin >> filename >> window >> parse_method;

			cout<<"Loaded "<<filename<<" with window = "<<window<<" and parse method = "<<parse_method<<endl;
			cout<<endl;
			
			if(window!=2&&window!=3) LoadSampleText2(data,filename,  window, parse_method,occurrence) ;
			LoadSampleText(data1,data2,filename,  window, parse_method,last_word,pre_last_word) ;
			
			//
			// ASSIGNMENT: ADD YOUR COMMANDS HERE
			//

		}

		// print the portion of the map structure with the choices for the
		// next word given a particular sequence.
		else if (command == "print") {
			std::vector<std::string> sentence = ReadQuotedWords(cin);

			if(window==2) print_window2(sentence,data1,last_word);
			else if(window==3) print_window3(sentence,data2,last_word,pre_last_word);
			else{
				string new_word=sentence[0];
				print(occurrence,data,new_word,window);
				cout<<endl;
			}
			


			//
			// ASSIGNMENT: ADD YOUR COMMANDS HERE
			//


		}

		// generate the specified number of words
		else if (command == "generate") {
			std::vector<std::string> sentence = ReadQuotedWords(cin);
			// how many additional words to generate
			int length;
			cin >> length;
			std::string selection_method;
			cin >> selection_method;
			bool random_flag;
			if (selection_method == "random") {
				random_flag = true;
			} else {
				assert (selection_method == "most_common");
				random_flag = false;
			}

			if(window==2) {
				cout<<sentence[0];
				generate_2_window(random_flag, sentence[0],data1,length,pre_last_word, last_word);
				cout<<endl;
				cout<<endl;
			}
			else if(window==3) {
				for(int i=0; i<sentence.size(); i++) {
					cout<<sentence[i];
					if(i!=sentence.size()-1) cout<<" ";
				}
				generate_3_window(random_flag, sentence,data2,length,pre_last_word, last_word);
				cout<<endl;
			}
			else{
				print_generate(random_flag,sentence,window,length,data,occurrence);
				cout<<endl;
			}
			//
			// ASSIGNMENT: ADD YOUR COMMANDS HERE
			//


		} else if (command == "quit") {
			break;
		} else {
			std::cout << "WARNING: Unknown command: " << command << std::endl;
		}
	}
}
